/*
 * SharedDataType.h
 *
 *  Created on: 14 mar 2016
 *      Author: pkasprow
 */

#ifndef SHAREDDATATYPE_H_
#define SHAREDDATATYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>

struct SGMShared_t
{
  pthread_mutex_t mutex;
  bool isModemReady;
};

#ifdef __cplusplus
}
#endif

#endif /* SHAREDDATATYPE_H_ */
