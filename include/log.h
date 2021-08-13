#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>
#include <assert.h>

// #define LOG_DEBUG

enum log_level { DEBUG = 0, INFO, WARNING, ERROR };

static enum log_level this_log_level = DEBUG;

static const char *log_level_str[] = { "DEBUG", "INFO", "WARNING", "ERROR" };

#ifdef LOG_DEBUG
	#define log_it(fmt, level_str, ...) \
		printk_port("[%s:%u] %s: " fmt  "\n", __FILE__, __LINE__, \
				level_str, ##__VA_ARGS__);
#else
	#define log_it(fmt, level_str, ...) \
		printk_port("%s: " fmt "\n", level_str, ##__VA_ARGS__);
#endif

#define DEBUG_ON 1
#define LOG_ON 1

#define DEBUG_ON2 1
#define LOG_ON2 1

#ifdef LOG_ON
#define log(level, fmt, ...) \
	do { \
		if (level < this_log_level) \
			break; \
		log_it(fmt, log_level_str[level], ##__VA_ARGS__); \
	} while (0)
#else
#define log(level, fmt, ...) \
	do { \
		; \
	} while (0)
#endif

#ifdef DEBUG_ON
#define debug() \
    do { \
        printk_port("TRACE:%s in %s:%d\n",__FUNCTION__,__FILE__,__LINE__); \
    } while (0)
#else
#define debug() \
	do { \
		; \
	} while (0)
#endif

#ifdef LOG_ON2
#define log2(level, fmt, ...) \
	do { \
		if (level < this_log_level) \
			break; \
		log_it(fmt, log_level_str[level], ##__VA_ARGS__); \
	} while (0)
#else
#define log2(level, fmt, ...) \
	do { \
		; \
	} while (0)
#endif

#ifdef DEBUG_ON2
#define debug2() \
    do { \
        printk_port("TRACE:%s in %s:%d\n",__FUNCTION__,__FILE__,__LINE__); \
    } while (0)
#else
#define debug2() \
	do { \
		; \
	} while (0)
#endif

#endif
