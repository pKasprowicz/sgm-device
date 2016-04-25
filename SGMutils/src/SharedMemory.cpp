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
#include <stdlib.h>
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
      fileDescriptor = shm_open(resourceName, O_CREAT | O_RDWR, mode);
      isInitialized = true;
      break;

    default:
      SGM_LOG_INFO("Error on initialization of shared memory");
      return Result::INIT_ERROR;
    }
  }

  if (-1 == fileDescriptor)
  {
    SGM_LOG_ERROR("Error %d", errno);
    return Result::INIT_ERROR;
  }

  if (-1 == ftruncate(fileDescriptor, sizeof(SGMShared_t)))
  {
    SGM_LOG_ERROR("Truncate error %d", errno);
    return Result::INIT_ERROR;
  }

  void * mappedMem = mmap(NULL,
      sizeof(SGMShared_t),
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      fileDescriptor,
      0);

  if (-1 == reinterpret_cast<int>(mappedMem))
  {
    SGM_LOG_ERROR("Mmap error %d", errno);
    return Result::INIT_ERROR;
  }

  sharedDataInstance.setData(reinterpret_cast<SGMShared_t*>(mappedMem));

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

  sharedMemory->isModemReady = false;
}

void SharedMemory::SharedData::setData(SGMShared_t * const pData)
{
  sharedMemory = pData;
}

void SharedMemory::SharedData::startAccess()
{
  if (0U == pthread_mutex_lock(&sharedMemory->mutex)){
    SGM_LOG_DEBUG("SharedData::startAccess() : mutex locked");
  }
  else
  {
    SGM_LOG_DEBUG("SharedData::startAccess() : mutex acquire error");
    abort();
  }
}

void SharedMemory::SharedData::endAccess()
{

  if (0U == pthread_mutex_unlock(&sharedMemory->mutex)){
    SGM_LOG_DEBUG("SharedData::endAccess() : mutex released");
  }
  else
  {
    SGM_LOG_DEBUG("SharedData::endAccess() : mutex release error");
    abort();
  }
}

void SharedMemory::SharedData::setModemReady(const bool flag)
{
  SGM_LOG_DEBUG("SharedData::setModemReady() : setting modem rady flag to %d", flag);
  sharedMemory->isModemReady = flag;
}

bool SharedMemory::SharedData::getModemReady()
{
  return sharedMemory->isModemReady;
}
