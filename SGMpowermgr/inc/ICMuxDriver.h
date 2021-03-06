/*
 * ICMuxDriver.h
 *
 *  Created on: 7 maj 2016
 *      Author: pkasprow
 */

#ifndef ICMUXDRIVER_H_
#define ICMUXDRIVER_H_

class ICMuxDriver
{
public:

  enum class Result
    {
      MUX_ON,
      MUX_OFF,
      MUX_ERROR
    };

  virtual ~ICMuxDriver()
  {

  }

  virtual Result turnOn()  = 0;
  virtual Result turnOff() = 0;


private:
};



#endif /* ICMUXDRIVER_H_ */
