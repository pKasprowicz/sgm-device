/*
 * ISensor.hpp
 *
 *  Created on: 15 sty 2016
 *      Author: pkasprow
 */

#ifndef ISENSOR_HPP_
#define ISENSOR_HPP_

class ISensor
{
public:
    virtual float getCurrentValue() = 0;
    virtual float getMaxValue() = 0;
    virtual float getMinValue() = 0;
private:

protected:
    virtual ISensor() = 0;
};



#endif /* ISENSOR_HPP_ */
