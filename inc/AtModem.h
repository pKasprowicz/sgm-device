/*
 * AtModem.h
 *
 *  Created on: 16 kwi 2015
 *      Author: pkasprow
 */

#ifndef INC_ATMODEM_H_
#define INC_ATMODEM_H_

#include <string>

class ATModem
{
public:

	enum class AtResult
	{
		Ok,
		Error
	};

	void init();

	int sendRawAt(std::string tx, std::string & rx);

private:
	int fileDescriptor;

	static const char * terminalPath;
};



#endif /* INC_ATMODEM_H_ */
