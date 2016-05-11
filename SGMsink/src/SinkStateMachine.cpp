/*
 * SinkStateMachine.cpp
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#include "SinkStateMachine.h"

SinkStateMachine::SinkStateMachine()
{	// TODO Auto-generated constructor stub

}

SinkStateMachine::~SinkStateMachine()
{
	// TODO Auto-generated destructor stub
}

void SinkStateMachine::tick ()
{
  switch(itsCurrentState)
  {
    case SinkState::ACTIVE:
      break;

    case SinkState::CONN_ESTABLISHED:
      break;

    case SinkState::ERROR_PPP:
      break;

    case SinkState::ERROR_TCPIP:
      break;

    case SinkState::IDLE:
      break;

    case SinkState::MODEM_PRESENT:
      break;

      /*
       * Initial state of the SinkSM
       */
    case SinkState::NO_CONNECTION:
      break;

    case SinkState::RESTART_REQUESTED:
      break;
  }
}

void SinkStateMachine::operator ()()
{
}
