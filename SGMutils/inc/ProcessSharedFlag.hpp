/*
 * SystemFlag.hpp
 *
 *  Created on: 21 mar 2016
 *      Author: pkasprow
 */

#ifndef PROCESSSHAREDFLAG_HPP_
#define PROCESSSHAREDFLAG_HPP_

#include <mutex>
#include <thread>

template<typename T> class ProcessSharedFlag
{
  public:
    ProcessSharedFlag(T initialVal)
    : itsFlag(initialVal),
      itsMutex()
    {

    }

    void set(T object)
    {
      itsMutex.lock();
      itsFlag = object;
      itsMutex.unlock();
    }
    T get()
    {
      itsMutex.lock();
      T temp = itsMutex;
      itsMutex.unlock();
      return temp;
    }

  private:
    T itsFlag;
    std::mutex itsMutex;
};



#endif /* PROCESSSHAREDFLAG_HPP_ */
