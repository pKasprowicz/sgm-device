/*
 * SharedMemory.cpp
 *
 *  Created on: 14 mar 2016
 *      Author: pkasprow
 */

#include "SharedMemory.h"

extern "C"
{
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Logger.h"

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
}

const int SharedMemory::flags = (O_CREAT | O_RDWR | O_EXCL);
const int SharedMemory::mode  = (S_IRUSR |
                                  S_IWUSR |
                                  S_IRGRP |
                                  S_IWGRP);

SharedMemory::SharedData SharedMemory::sharedDataInstance;
bool SharedMemory::isInitialized = false;
int SharedMemory::fileDescriptor = -1;

SharedMemory::SharedMemory()
{
  // TODO Auto-generated constructor stub

}

SharedMemory::~SharedMemory()
{
  release();
}

SharedMemory::Result SharedMemory::init()
{
  if(isInitialized)
  {
    SGM_LOG_INFO("Shared memory already initialized");
    return Result::INIT_ERROR;
  }

  if (-1 == (fileDescriptor = shm_open(resourceName, flags, mode)))
  {
    switch(errno)
    {
    case EEXIST:
      fileDescriptor = shm_open(resourceName, flags, O_CREAT | O_RDWR);
      isInitialized = true;
      break;

    default:
      SGM_LOG_INFO("Error on initialization of shared memory");
      return Result::INIT_ERROR;
    }
  }

  truncate(resourceName, sizeof(SGMShared_t));

  sharedDataInstance.setData(
      reinterpret_cast<SGMShared_t*>(mmap(NULL,
                                    sizeof(SGMShared_t),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    fileDescriptor,
                                    0))
                            );

  if (!isInitialized)
  {
    sharedDataInstance.initData();
  }

  return Result::INIT_OK;
}

bool SharedMemory::release()
{
  //TODO determine if needed to destroy the object somehow
}

SharedMemory::SharedData& SharedMemory::getDataInstance()
{
  return sharedDataInstance;
}

void SharedMemory::SharedData::initData()
{
  pthread_mutexattr_t mutexAttributes;

  sharedMemory->mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutexattr_init(&mutexAttributes);
  pthread_mutexattr_setpshared(&mutexAttributes, PTHREAD_PROCESS_SHARED);

  pthread_mutex_init(&sharedMemory->mutex, &mutexAttributes);
}

void SharedMemory::SharedData::setData(SGMShared_t * const pData)
{
  sharedMemory = pData;
  initData();
}
