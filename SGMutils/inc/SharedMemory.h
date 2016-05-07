/*
 * SharedMemory.h
 *
 *  Created on: 14 mar 2016
 *      Author: pkasprow
 */

#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

extern "C"
{

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
}

#include "SharedDataType.h"

class SharedMemory
{
  //TODO implement RAII

public:

  /*
   * Representation of shared data
   */

  class SharedData
  {
  public:

    friend class SharedMemory;

    void startAccess();
    void endAccess();

    void setModemReady(const bool flag);
    bool getModemReady();

    void setCmuxReady(const bool flag);
    bool getCmuxReady();

  private:
    void setData(SGMShared_t * const pData);

    void initData();
    SGMShared_t * sharedMemory;
  };

  enum class Result
  {
    INIT_OK,
    INIT_ERROR,
  };

  SharedMemory();
  virtual ~SharedMemory();

  Result init();

  SharedData & getDataInstance();

  private:
    bool release();

    static SharedData sharedDataInstance;
    static bool isInitialized;
    static int fileDescriptor;

    static constexpr const char * resourceName = "/sgm_shared";
    static const int flags;
    static const int mode;
};

#endif /* SHAREDMEMORY_H_ */
