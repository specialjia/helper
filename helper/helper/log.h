/**
* Copyright (c) 2017 rxi
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the MIT license. See `log.c` for details.
*/

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <Shlwapi.h>

#define LOG_VERSION "0.1.0"
#define LOG_USE_COLOR
#include <Windows.h>
#include <tchar.h>
#define lengthof(x) (sizeof(x)/sizeof(*x))
typedef void(*log_LockFn)(void *udata, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };
void logInit();
void  DUI__Trace(LPCTSTR pstrFormat, ...);
#define DebugLog  DUI__Trace
#define log_trace(msg,...) log_log(LOG_TRACE, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_debug(msg,...) log_log(LOG_DEBUG, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_info(msg,...)  log_log(LOG_INFO,  __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_warn(msg,...)  log_log(LOG_WARN,  __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_error(msg,...) log_log(LOG_ERROR, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_fatal(msg,...) log_log(LOG_FATAL, __FILE__, __LINE__, msg, ##__VA_ARGS__)


void log_log(int level, const char * file, int line, const char* fmt, ...);

void log_set_udata(void *udata);
void log_set_lock(log_LockFn fn);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_quiet(int enable);


#endif
