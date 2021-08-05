#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

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

#define log(level, fmt, ...) \
	do { \
		if (level < this_log_level) \
			break; \
		log_it(fmt, log_level_str[level], ##__VA_ARGS__); \
	} while (0)

#endif
