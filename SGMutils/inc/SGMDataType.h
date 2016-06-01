/*
 * SGMDataType.h
 *
 *  Created on: 10 mar 2016
 *      Author: pkasprow
 */

#ifndef SGMDATATYPE_H_
#define SGMDATATYPE_H_

#include <map>

namespace sgm
{

  enum class PhysQuantity
  {
    PRESSURE,
    TEMPERATURE,
    HUMIDITY,
    LIGHT
  };

  enum class Unit
  {
    hPa,
    Celsius,
    Percent,
    Lumen
  };

  enum class MeasurementPoint
  {
    SOIL,
    AIR,
    DEVICE,
    NONE
  };

  static std::map<MeasurementPoint, std::string> measPointStringMap =
  {
      {MeasurementPoint::AIR,    "air"},
      {MeasurementPoint::SOIL,   "soil"},
      {MeasurementPoint::DEVICE, "dev"}
  };

  static std::map<PhysQuantity, std::string> physicalStringMap =
  {
      {PhysQuantity::PRESSURE,   "press"},
      {PhysQuantity::TEMPERATURE,"temp"},
      {PhysQuantity::HUMIDITY,   "humid"},
      {PhysQuantity::LIGHT,      "light"}
  };

  static std::multimap<MeasurementPoint, PhysQuantity> unitMap =
  {
      {MeasurementPoint::AIR,    PhysQuantity::PRESSURE},
      {MeasurementPoint::AIR,    PhysQuantity::TEMPERATURE},
      {MeasurementPoint::AIR,    PhysQuantity::HUMIDITY},
      {MeasurementPoint::AIR,    PhysQuantity::LIGHT},
      {MeasurementPoint::SOIL,   PhysQuantity::HUMIDITY},
      {MeasurementPoint::DEVICE, PhysQuantity::TEMPERATURE}
  };

  struct SgmProcessData
  {
    /*
     * I have no idea what is it for. However, looks professional.
     */
    uint32_t packetId;
    /*
     * ID of measurement point.
     */
    MeasurementPoint measPoint;
    /*
     * Type of physical quantity
     */
    PhysQuantity physQuantityType;
    /*
     *  Unit. Simple, isn't it?
     */
    Unit unit;
    int32_t value;

    uint16_t divider;
    /*
     * I don't believe anybody treats that one seriously...
     */
    uint16_t crc;
  };

}

#endif /* SGMDATATYPE_H_ */
