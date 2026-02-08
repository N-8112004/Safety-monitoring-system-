#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include "config.h"

// =====================================================
// SENSOR DATA STRUCTURE
// =====================================================
typedef struct {
    float temperature;      // °C
    float humidity;         // %
    int gasLevel;          // ADC value (0-4095)
    bool flameDetected;    // true if flame detected
    unsigned long timestamp; // millis() when read
} SensorData;

// =====================================================
// SENSOR CLASS
// =====================================================
class SensorManager {
private:
    DHT dht;
    SensorData currentData;
    unsigned long lastReadTime;

public:
    SensorManager();
    void begin();
    bool readSensors();
    SensorData getData();
    
    // Individual sensor reads
    float readTemperature();
    float readHumidity();
    int readGasLevel();
    bool readFlameStatus();
    
    // Utility functions
    bool isDataValid();
    void printData();
};

#endif // SENSORS_H
