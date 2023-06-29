#ifndef JC_JLOG_H
#define JC_JLOG_H

#include <stdbool.h>
#include "jvector.h"

typedef enum {
    JLOG_LEVEL_TRACE,
    JLOG_LEVEL_DEBUG,
    JLOG_LEVEL_INFO,
    JLOG_LEVEL_WARN,
    JLOG_LEVEL_ERROR,
    JLOG_LEVEL_FATAL,
} jlog_level;

const char *jlog_level_to_string(jlog_level level);

struct jlogger {
    Vector *sinks;
};

typedef struct jlogger jlogger_s;
typedef jlogger_s *jlogger_t;

struct jlogger_sink;
typedef struct jlogger_sink jlogger_sink_s;
typedef jlogger_sink_s *jlogger_sink_t;
typedef void jlogger_sink_fn(jlogger_sink_t self, jlog_level level, const char *fmt, ...);
typedef bool jlogger_sink_filter(jlogger_sink_t self, jlog_level level, const char *fmt, ...);
struct jlogger_sink {
    jlogger_sink_fn *sink;
    Vector filters;
};
void jlogger_sink_add_filter(jlogger_sink_t self, jlogger_sink_filter *filter);

jlogger_t jlogger_new();
void jlogger_free(jlogger_t self);
void jlogger_add_sink(jlogger_t self, jlogger_sink_t sink);

void jlogger_log(jlogger_t self, jlog_level level, const char *fmt, ...);
void jlogger_log_trace(jlogger_t self, const char *fmt, ...);
void jlogger_log_debug(jlogger_t self, const char *fmt, ...);
void jlogger_log_info(jlogger_t self, const char *fmt, ...);
void jlogger_log_warn(jlogger_t self, const char *fmt, ...);
void jlogger_log_error(jlogger_t self, const char *fmt, ...);
void jlogger_log_fatal(jlogger_t self, const char *fmt, ...);

void jlogger_stdout_sink_new(jlogger_sink_t sink);
void jlogger_stderr_sink_new(jlogger_sink_t sink);
void jlogger_std_hybrid_sink_new(jlogger_sink_t sink);

#endif //JC_JLOG_H
