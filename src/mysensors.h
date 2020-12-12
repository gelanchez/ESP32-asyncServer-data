/**
 * @file mysensors.h
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief Library to handle sensors. The child classes Photoresistor
 * and Thermistor inherit from the parent Sensor.
 * @version 0.0.1
 * @date 2020-09-17
 * @copyright GPL-3.0
 */

#ifndef MYSENSORS_H
#define MYSENSORS_H

#include "constants.h"

class Sensor
{
    protected:
        const uint8_t m_pin;
        float m_readings[Constants::numReadings];  // Readings from the analog input
        int m_readIndex;  // Index of the current reading
        float m_total;  // Total running
        float m_value;
        int _getRawMean(uint8_t iterations = Constants::iterations);
        virtual float _rawToValue(int rawValue);

    public:
        Sensor(uint8_t pin);
        ~Sensor();
        float read();
        float getLastValue();
        void printLastValue();
};

class Photoresistor: public Sensor
{
    protected:
        virtual float _rawToValue(int rawValue);
    public:
        Photoresistor(uint8_t pin);
        ~Photoresistor();
        float read();
        void printLastValue();
};

class Thermistor: public Sensor
{
    protected:
        virtual float _rawToValue(int rawValue);
    public:
        Thermistor(uint8_t pin);
        ~Thermistor();
        float read();
        void printLastValue();
};

#endif