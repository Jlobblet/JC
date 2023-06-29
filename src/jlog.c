#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
    Vector_dest(self->sinks);
    free(self);
}

void jlogger_add_sink(jlogger_t self, jlogger_sink_t sink) {
    Vector_push_back(self->sinks, sink);
}

static void jlogger_vlog(jlogger_t self, jlog_level level, const char *fmt, va_list args) {
    for (uptr i = 0; i < self->sinks->length; i++) {
        jlogger_sink_t sink = self->sinks->data[i];
        sink->sink(sink, level, fmt, args);
    }
}

void jlogger_log(jlogger_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, level, fmt, args);
    va_end(args);
}

void jlogger_log_trace(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_TRACE, fmt, args);
    va_end(args);
}

void jlogger_log_debug(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}
void jlogger_log_info(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_INFO, fmt, args);
    va_end(args);
}

void jlogger_log_warn(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_WARN, fmt, args);
    va_end(args);
}

void jlogger_log_error(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_ERROR, fmt, args);
    va_end(args);
}

void jlogger_log_fatal(jlogger_t self, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jlogger_vlog(self, JLOG_LEVEL_FATAL, fmt, args);
    va_end(args);
}

void stream_log(const jlog_level *level, const char *fmt, FILE *stream, va_list args) {
    time_t t = time(NULL);

    struct tm *info = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);

    fprintf(stream, "%s [%s]: ", buffer, jlog_level_to_string((*level)));
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void jlogger_stdout_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stdout, args);
    va_end(args);
}

void jlogger_stdout_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_stdout_sink;
    Vector_default(&sink->filters);
}

void jlogger_stderr_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stderr, args);
    va_end(args);
}

void jlogger_stderr_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_stderr_sink;
    Vector_default(&sink->filters);
}

void jlogger_std_hybrid_sink(__attribute__((unused)) jlogger_sink_t self, jlog_level level, const char *fmt, ...) {
    FILE *stream = level >= JLOG_LEVEL_WARN ? stderr : stdout;

    va_list args;
    va_start(args, fmt);
    stream_log(&level, fmt, stream, args);
    va_end(args);
}

void jlogger_std_hybrid_sink_new(jlogger_sink_t sink) {
    sink->sink = jlogger_std_hybrid_sink;
    Vector_default(&sink->filters);
}

void jlogger_sink_add_filter(jlogger_sink_t self, jlogger_sink_filter *filter) {
    Vector_push_back(&self->filters, filter);
}
