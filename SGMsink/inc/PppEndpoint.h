/*
 * PppEndpoint.h
 *
 *  Created on: 11 maj 2016
 *      Author: pkasprow
 */

#ifndef PPPENDPOINT_H_
#define PPPENDPOINT_H_

class PppEndpoint
{
public:
  PppEndpoint();
  virtual ~PppEndpoint();

  bool open();
  bool close();

private:

  bool verifyConnection();
  bool performFailsafeCleanup();
};

#endif /* PPPENDPOINT_H_ */
