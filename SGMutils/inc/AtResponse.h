/*
 * AtResponse.h
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#ifndef ATRESPONSE_H_
#define ATRESPONSE_H_

struct AtResponse
{
    enum class ReturnCode
    {
        OK,
        ERROR,
        CME_ERROR,
        CMS_ERROR
    };

    AtResponse() :
      responseCorrect(false),
      commandReturnCode(ReturnCode::ERROR)
    {

    }

    AtResponse(const AtResponse & ) = delete;
    AtResponse & operator = (const AtResponse &) = delete;

    bool responseCorrect;
    ReturnCode commandReturnCode;
};



#endif /* ATRESPONSE_H_ */
