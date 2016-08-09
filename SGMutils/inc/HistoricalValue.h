/*
 * HistoricalValue.h
 *
 *  Created on: 27 kwi 2016
 *      Author: pkasprow
 */

#ifndef HISTORICALVALUE_H_
#define HISTORICALVALUE_H_

template<typename Contained> class HistoricalValue
{
public:

  HistoricalValue<Contained>(Contained _val)
  {
    for(Contained &element : values)
    {
      element = _val;
    }
  }

  HistoricalValue<Contained>(const HistoricalValue<Contained> & ) = delete;
  HistoricalValue<Contained> & operator = (const HistoricalValue<Contained> & ) = delete;

  bool is(Contained testVal)
  {
    return testVal == values[0];
  }

  bool was(Contained testVal)
  {
    return testVal == values[0];
  }

  void set(Contained newVal)
  {
    values[1] = values[0];
    values[0] = newVal;
  }

  bool hasChanged()
  {
    return (values[0] != values[1]);
  }

  void equalize()
  {
    values[1] = values[0];
  }

  Contained & get()
  {
    return values[0];
  }

  Contained & getHistory()
  {
    return values[1];
  }

private:

  Contained values[2];


};



#endif /* HISTORICALVALUE_H_ */
