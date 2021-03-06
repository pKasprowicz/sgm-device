/*
 * IModemQuery.h
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#ifndef IMODEMQUERY_H_
#define IMODEMQUERY_H_

class IModemQuery
{
public:

  enum class Result
  {
    OK_RAW,
    OK_CMUX, //reserved for future use
    NOK,
  };

  virtual Result run() = 0;

  virtual ~IModemQuery()
  {

  }

private:
};



#endif /* IMODEMQUERY_H_ */
