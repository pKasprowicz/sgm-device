/*
 * processing_thread.c
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */
#include "processing_thread.h"
#include "SinkStateMachine.h"

extern "C" void * processing_thread(void * args)
{
  SinkStateMachine stateMachine1;
  stateMachine1.tick();
}
