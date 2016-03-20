/*
 * SinkStateMachine.cpp
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#include "SinkStateMachine.h"

SinkStateMachine::SinkStateMachine() :
itsState(SinkState::INIT)
{

}

SinkStateMachine::~SinkStateMachine()
{

}

void SinkStateMachine::tick ()
{
  switch(itsState)
  {
  case SinkState::INIT:
    break;

  case SinkState::NO_CONNECTION:
    break;

  default:
    break;
  }

}
