#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "jlog.h"
#include "jvector.h"

const char *jlog_level_to_string(jlog_level level) {
    switch (level) {
        case JLOG_LEVEL_TRACE:
            return "TRACE";
        case JLOG_LEVEL_DEBUG:
            return "DEBUG";
        case JLOG_LEVEL_INFO:
            return "INFO";
        case JLOG_LEVEL_WARN:
            return "WARN";
        case JLOG_LEVEL_ERROR:
            return "ERROR";
        case JLOG_LEVEL_FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

jlogger_t jlogger_new() {
    jlogger_t self = calloc(1, sizeof(jlogger_t));
    self->sinks = calloc(1, sizeof(Vector));
    self->sinks->capacity = 1;
    self->sinks->growth_factor = 2;
    Vector_init(self->sinks);
    return self;
}

void jlogger_free(jlogger_t self) {
    for (uptr i = 0; i < self->sinks->length; i++) {
        jlogger_sink_t sink = self->sinks->data[i];
        if (sink->free) sink->free(sink);
    }
    Vector_dest(self->sinks);
    free(self);
}

void jlogger_add_sink(jlogger_t self, jlogger_sink_t sink) {
    Vector_push_back(self->sinks, sink);
}

static bool jlogger_vlog(jlogger_t self, jlog_level level, const char *fmt, va_list args) {
    bool result = true;
    for (uptr i = 0; i < self->sinks->length; i++) {
        jlogger_sink_t sink = self->sinks->data[i];
        result &= sink->sink(sink, level, fmt, args);
    }
    return result;
}

bool jlogger_log(jlogger_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bool result = jlogger_vlog(self, level, fmt, args);
    va_end(args);
    return result;
}

#define jlogger_log_level_impl(name, level) \
    bool jlogger_log_##name(jlogger_t self, const char *fmt, ...) { \
        va_list args; \
        va_start(args, fmt); \
        bool result = jlogger_vlog(self, level, fmt, args); \
        va_end(args); \
        return result; \
    }

jlogger_log_level_impl(trace, JLOG_LEVEL_TRACE)
jlogger_log_level_impl(debug, JLOG_LEVEL_DEBUG)
jlogger_log_level_impl(info, JLOG_LEVEL_INFO)
jlogger_log_level_impl(warn, JLOG_LEVEL_WARN)
jlogger_log_level_impl(error, JLOG_LEVEL_ERROR)
jlogger_log_level_impl(fatal, JLOG_LEVEL_FATAL)

#undef jlogger_log_level_impl

int stream_log(const jlog_level *level, const char *fmt, FILE *stream, va_list args) {
    time_t t = time(NULL);

    struct tm *info = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    int bytes_written = 0;
    bytes_written += fprintf(stream, "%s [%s]: ", buffer, jlog_level_to_string((*level)));
    bytes_written += vfprintf(stream, fmt, args);
    bytes_written += fprintf(stream, "\n");

    return bytes_written;
}

bool jlogger_stdout_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stdout, args);
    va_end(args);
    return true;
}

void jlogger_stdout_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_stdout_sink;
    Vector_default(&sink->filters);
}

bool jlogger_stderr_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stderr, args);
    va_end(args);
    return true;
}

void jlogger_stderr_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_stderr_sink;
    Vector_default(&sink->filters);
}

bool jlogger_std_hybrid_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    FILE *stream = level >= JLOG_LEVEL_WARN ? stderr : stdout;

    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stream, args);
    va_end(args);
    return true;
}

void jlogger_std_hybrid_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_std_hybrid_sink;
    Vector_default(&sink->filters);
}

void jlogger_sink_add_filter(jlogger_sink_t self, jlogger_sink_filter *filter) {
    Vector_push_back(&self->filters, filter);
}

typedef struct jloggger_file_sink_data {
    char *path;
    FILE *file;
} jloggger_file_sink_data_s;

bool jlogger_file_sink(jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jloggger_file_sink_data_s *data = self->data;
    stream_log(&level, fmt, data->file, args);
    va_end(args);
    return true;
}

void jlogger_file_sink_free(jlogger_sink_t self) {
    jloggger_file_sink_data_s *data = self->data;
    fclose(data->file);
    free(data->path);
    free(data);
}

/// @brief Create a new file sink.
/// \param sink The sink to initialise.
/// \param path A path to the file to log to.
/// Ownership is transferred to the sink.
void jlogger_file_sink_new(jlogger_sink_t sink, char *path) {
    sink->sink = jlogger_file_sink;
    sink->data = calloc(1, sizeof(jloggger_file_sink_data_s));
    sink->free = jlogger_file_sink_free;
    jloggger_file_sink_data_s *data = sink->data;
    data->path = path;
    data->file = fopen(path, "a");
    Vector_default(&sink->filters);
}

typedef struct jlogger_rotating_file_sink_data {
    char *path_template;
    uptr path_template_len;
    FILE *file;
    uptr max_size;
    uptr current_size;
    ulong counter;
    double max_age;
    time_t start_time;
} jlogger_rotating_file_sink_data_s;

bool jlogger_rotating_file_sink_rotate(jlogger_rotating_file_sink_data_s *data) {
    if (data->file) fclose(data->file);
    size_t buffer_size = data->path_template_len;
    const int NUL_TERMINATOR_SIZE = 1;
    char *path = calloc(1, buffer_size + NUL_TERMINATOR_SIZE);
    strncpy(path, data->path_template, buffer_size);

    // calculate the size of the counter
    uptr counter_size = 2;
    ulong counter = data->counter;
    while (counter /= 10) counter_size++;

    // create the counter string
    char *counter_str = calloc(1, counter_size + NUL_TERMINATOR_SIZE);
    snprintf(counter_str, counter_size + NUL_TERMINATOR_SIZE, ".%lu", data->counter);

    // keep doubling the buffer sizes until the formatted path fits
    while (true) {
        // format the datetime components
        size_t bytes_written = strftime(path, buffer_size, data->path_template, localtime(&data->start_time));
        // append the counter
        if (bytes_written != 0 && bytes_written + counter_size + NUL_TERMINATOR_SIZE <= buffer_size) {
            strncat(path, counter_str, counter_size);
            break;
        }

        buffer_size *= 2;
        char *new_path = realloc(path, buffer_size + NUL_TERMINATOR_SIZE);
        if (new_path == NULL) {
            goto error;
        }
        // zero out the new memory
        memset(new_path, 0, buffer_size + NUL_TERMINATOR_SIZE);
        path = new_path;
    }

    free(counter_str);
    counter_str = NULL;
    data->file = fopen(path, "a");
    if (data->file == NULL) {
        goto error;
    }

    data->current_size = 0;

    return true;
    error:
    free(path);
    free(counter_str);
    return false;
}

bool jlogger_rotating_file_sink_maybe_rotate(jlogger_rotating_file_sink_data_s *data) {
    // check if there is no file open
    if (data->file == NULL) {
        if (!jlogger_rotating_file_sink_rotate(data)) {
            return false;
        }
    }

    // check if we need to rotate on age
    time_t current_time = time(NULL);
    double elapsed_time = difftime(current_time, data->start_time);
    if (elapsed_time >= data->max_age) {
        // reset the counter and start time
        data->counter = 0;
        data->start_time = current_time;
        if (!jlogger_rotating_file_sink_rotate(data)) {
            return false;
        }
    }

    // check if we need to rotate on size
    if (data->current_size >= data->max_size) {
        // increment the counter
        data->counter++;
        if (!jlogger_rotating_file_sink_rotate(data)) {
            return false;
        }
    }
    return true;
}

bool jlogger_rotating_file_sink(jlogger_sink_t sink, jlog_level level, const char *fmt, ...) {
    bool result = true;
    va_list args;
    va_start(args, fmt);
    jlogger_rotating_file_sink_data_s *data = sink->data;
    if (!jlogger_rotating_file_sink_maybe_rotate(data)) {
        result = false;
        goto error;
    }

    data->current_size += stream_log(&level, fmt, data->file, args);
    fflush(data->file);
    error:
    va_end(args);
    return result;
}

void jloggger_rotating_file_sink_free(jlogger_sink_t self) {
    jlogger_rotating_file_sink_data_s *data = self->data;
    fclose(data->file);
    free(data->path_template);
    free(data);
}

void jlogger_rotating_file_sink_new(jlogger_sink_t sink, char *path_template, uptr path_template_len, u32 max_size, double max_age) {
    sink->sink = jlogger_rotating_file_sink;
    sink->free = jloggger_rotating_file_sink_free;
    sink->data = calloc(1, sizeof(jlogger_rotating_file_sink_data_s));
    jlogger_rotating_file_sink_data_s *data = sink->data;
    data->path_template = path_template;
    data->path_template_len = path_template_len;
    data->max_size = max_size;
    data->max_age = max_age;
    data->start_time = time(NULL);
    data->file = NULL;
    data->counter = 0;
    data->current_size = 0;
    Vector_default(&sink->filters);
}
