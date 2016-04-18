/*
 * AtResponse.h
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#ifndef ATGENERICRESPONSE_H_
#define ATGENERICRESPONSE_H_

class AtGenericResponse
{
  public:

    enum class ReturnCode
    {
        OK,
        ERROR,
        CME_ERROR,
        CMS_ERROR
    };

    AtGenericResponse()
    {

    }

    AtGenericResponse(bool _responseCorrect, ReturnCode _cmdRetCode, int _errNo) :
      responseCorrect(_responseCorrect),
      commandReturnCode(_cmdRetCode),
      errorNumber(_errNo)
    {

    }

    AtGenericResponse(const AtGenericResponse & ) = delete;
    AtGenericResponse & operator = (const AtGenericResponse &) = delete;

    bool isResponseCorrect()
    {
      return responseCorrect;
    }

    ReturnCode getReturnCode()
    {
      return commandReturnCode;
    }

    int getErrorNumber()
    {
      return errorNumber;
    }

  private:

    bool responseCorrect = false;
    ReturnCode commandReturnCode = ReturnCode::ERROR;
    int errorNumber = 0U;
};



#endif /* ATGENERICRESPONSE_H_ */
