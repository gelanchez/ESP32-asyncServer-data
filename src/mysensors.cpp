/**
 * @file mysensors.cpp
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief Library to handle sensors. The child classes Photoresistor
 * and Thermistor inherit from the parent Sensor.
 * @version 0.0.1
 * @date 2020-09-17
 * @copyright GPL-3.0
 */

#include "constants.h"
#include "mysensors.h"

Sensor::Sensor(uint8_t pin) : m_pin(pin), m_readings{}, m_readIndex(0), m_total(0), m_value(0)
{
    pinMode(pin, INPUT);
}

Sensor::~Sensor()
{
}

/**
 * @brief Get the mean value of a number of iterations, discarding the maximun
 * and minimun values.
 * @param iterations Number of iterations.
 * @return int 
 */
int Sensor::_getRawMean(uint8_t iterations)
{
    int mean(0);
    int readValue;
    int minValue;
    int maxValue;
    for (int i = 0; i < iterations; i++)
    {
        readValue = analogRead(m_pin);
        if (i == 0)
        {
            minValue = readValue;
            maxValue = readValue;
        }
        else
        {
            if (readValue > maxValue)
                maxValue = readValue;
            if (readValue < minValue)
                minValue = readValue;
        }
        mean = mean + readValue;
    }
    return static_cast<int>((mean - minValue - maxValue) / (iterations - 2));
}

/**
 * @brief Virtual method. It needs to be defined to avoid vtable error:
 * http://gcc.gnu.org/faq.html#vtables
 * @param rawValue 
 * @return float 
 */
float Sensor::_rawToValue(int rawValue)
{
}

float Sensor::read()
{
    m_total = m_total - m_readings[m_readIndex];
    m_readings[m_readIndex] = _rawToValue(_getRawMean());
    m_total = m_total + m_readings[m_readIndex];
    m_readIndex = m_readIndex + 1;

    if (m_readIndex >= Constants::numReadings)
        m_readIndex = 0; // Restart intex array

    // Calculate the average
    m_value = m_total / Constants::numReadings;
    return m_value;
}

float Sensor::getLastValue()
{
    return m_value;
}

Photoresistor::Photoresistor(uint8_t pin) : Sensor(pin)
{
}

Photoresistor::~Photoresistor()
{
}

float Photoresistor::_rawToValue(int rawValue)
{
    float voltage = (static_cast<float>(rawValue) / Constants::resolution) * Constants::maxVoltage;
    float resistance = Constants::resistancePhoto * ((static_cast<float>(Constants::maxVoltage) / voltage) - 1);
    if (resistance < Constants::minResistance)
        resistance = Constants::minResistance;
    float illuminance = pow(resistance, Constants::parameter1) * Constants::parameter2;
    return illuminance;
}

float Photoresistor::read()
{
    return round(Sensor::read()); // lux values only as integers
}

void Photoresistor::printLastValue()
{
    Serial.print("Luminosity: ");
    Serial.print(m_value);
    Serial.println(" lux");
}

Thermistor::Thermistor(uint8_t pin) : Sensor(pin)
{
}

Thermistor::~Thermistor()
{
}

float Thermistor::_rawToValue(int rawValue)
{
    float voltage = (static_cast<float>(rawValue) / Constants::resolution) * Constants::maxVoltage;
    float resistance = Constants::resitanceTherm * (static_cast<float>(Constants::maxVoltage) / voltage - 1);
    float temperature = 1 / (1 / 298.15 + (1 / Constants::parameterB) * log(resistance / Constants::parameterR0)) - 273.15; // 26000 @ 22 degC
    return temperature;
}

float Thermistor::read()
{
    return round(Sensor::read() * 10) / 10; // degC values only with one decimal
}

void Thermistor::printLastValue()
{
    Serial.print("Temperature: ");
    Serial.print(m_value);
    Serial.println(" degC");
}