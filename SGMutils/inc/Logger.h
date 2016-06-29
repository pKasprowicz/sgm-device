/*
 * Logger.h
 *
 *  Created on: 14 mar 2016
 *      Author: pkasprow
 */

#ifndef LOGGER_H_
#define LOGGER_H_


#ifdef SGM_SERVICE

#include <systemd/sd-journal.h>

#define SGM_LOG_INFO(message, ...)   { sd_journal_print(LOG_INFO, message, ##__VA_ARGS__); }

#define SGM_LOG_WARN(message, ...)   { sd_journal_print(LOG_WARNING, message, ##__VA_ARGS__); }

#define SGM_LOG_ERROR(message, ...)  { sd_journal_print(LOG_ERR, message, ##__VA_ARGS__); }

#define SGM_LOG_DEBUG(message, ...)  { sd_journal_print(LOG_DEBUG, message, ##__VA_ARGS__); }

#define SGM_LOG_FATAL(message, ...)  { sd_journal_print(LOG_CRIT, message, ##__VA_ARGS__); }
#else

#include "stdio.h"

#define SGM_LOG_INFO(message, ...)   { printf("[LOG_INFO]   " message "\n", ##__VA_ARGS__); \
                                      }

#define SGM_LOG_WARN(message, ...)   { printf("[LOG_WARN]   " message "\n", ##__VA_ARGS__); \
                                      }

#define SGM_LOG_ERROR(message, ...)  { printf("[LOG_ERROR]  " message "\n", ##__VA_ARGS__); \
                                      }

#define SGM_LOG_DEBUG(message, ...)  { printf("[LOG_DEBUG]  " message "\n", ##__VA_ARGS__); \
                                      }

#define SGM_LOG_FATAL(message, ...)  { printf("[LOG_FATAL]  " message "\n", ##__VA_ARGS__); \
                                      }

#endif

#endif /* LOGGER_H_ */
