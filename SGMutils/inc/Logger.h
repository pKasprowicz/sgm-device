/*
 * Logger.h
 *
 *  Created on: 14 mar 2016
 *      Author: pkasprow
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef __cplusplus
extern "C"
{
#endif

//#include <systemd/sd-journal.h>
//
//#define SGM_LOG_INFO(message) sd_journal_print(LOG_INFO, message);
//
//#define SGM_LOG_DEBUG(message) sd_journal_print(LOG_DEBUG, __FILE__ message);

#include "stdio.h"
#define SGM_LOG_INFO(message, ...)  printf(" [LOG_INFO]  " message "\n", ##__VA_ARGS__);

#define SGM_LOG_WARN(message, ...)  printf(" [LOG_WARN]  " message "\n", ##__VA_ARGS__);

#define SGM_LOG_ERROR(message, ...)  printf(" [LOG_ERROR]  " message "\n", ##__VA_ARGS__);

#define SGM_LOG_DEBUG(message, ...) printf("[LOG_DEBUG]  " message "\n", ##__VA_ARGS__);


#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H_ */
