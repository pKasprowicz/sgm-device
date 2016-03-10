/*
 * Logging.h
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <systemd/sd-journal.h>

#define SGM_LOG_INFO(message) sd_journal_print(LOG_INFO, message);

#define SGM_LOG_DEBUG(message) sd_journal_print(LOG_DEBUG, __FILE__ message);

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H_ */
