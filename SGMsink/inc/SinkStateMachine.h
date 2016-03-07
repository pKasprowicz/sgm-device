/*
 * SinkStateMachine.h
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#ifndef SINKSTATEMACHINE_H_
#define SINKSTATEMACHINE_H_

class SinkStateMachine
  {
    public:
      SinkStateMachine();
      virtual ~SinkStateMachine();

      void tick();

      enum class SinkState_t
      {
        NO_CONNECTION,
        MODEM_PRESENT,
        CONN_ESTABLISHED,
        ACTIVE,
        IDLE,
        ERROR_TCPIP,
        ERROR_PPP,
        RESTART_REQUESTED
      };

    private:

  };

#endif /* SINKSTATEMACHINE_H_ */