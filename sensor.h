#include "sensors.h"

// =====================================================
// CONSTRUCTOR
// =====================================================
SensorManager::SensorManager() : dht(PIN_DHT22, DHT_TYPE) {
    lastReadTime = 0;
    currentData = {0};
}

// =====================================================
// INITIALIZATION
// =====================================================
void SensorManager::begin() {
    // Initialize DHT sensor
    dht.begin();
    
    // Configure pins
    pinMode(PIN_GAS_SENSOR, INPUT);
    pinMode(PIN_FLAME_SENSOR, INPUT);
    
    Serial.println("[SENSOR] Warming up sensors...");
    delay(SENSOR_WARMUP_TIME);
    Serial.println("[SENSOR] Sensors ready!");
}

// =====================================================
// READ ALL SENSORS
// =====================================================
bool SensorManager::readSensors() {
    unsigned long currentTime = millis();
    
    // Rate limiting
    if (currentTime - lastReadTime < SENSOR_READ_INTERVAL) {
        return false;
    }
    
    lastReadTime = currentTime;
    
    // Read all sensors
    currentData.temperature = readTemperature();
    currentData.humidity = readHumidity();
    currentData.gasLevel = readGasLevel();
    currentData.flameDetected = readFlameStatus();
    currentData.timestamp = currentTime;
    
    return isDataValid();
}

// =====================================================
// INDIVIDUAL SENSOR READS
// =====================================================
float SensorManager::readTemperature() {
    float temp = dht.readTemperature();
    return isnan(temp) ? -999.0 : temp;
}

float SensorManager::readHumidity() {
    float hum = dht.readHumidity();
    return isnan(hum) ? -999.0 : hum;
}

int SensorManager::readGasLevel() {
    int gasValue = analogRead(PIN_GAS_SENSOR);
    return gasValue;
}

bool SensorManager::readFlameStatus() {
    return (digitalRead(PIN_FLAME_SENSOR) == FLAME_DETECTED);
}

// =====================================================
// DATA VALIDATION
// =====================================================
bool SensorManager::isDataValid() {
    return (currentData.temperature > -100 && 
            currentData.humidity > 0);
}

// =====================================================
// GET CURRENT DATA
// =====================================================
SensorData SensorManager::getData() {
    return currentData;
}

// =====================================================
// DEBUG PRINT
// =====================================================
void SensorManager::printData() {
    Serial.println("\n========== SENSOR READINGS ==========");
    Serial.print("Temperature: "); 
    Serial.print(currentData.temperature, 1); 
    Serial.println(" °C");
    
    Serial.print("Humidity: "); 
    Serial.print(currentData.humidity, 1); 
    Serial.println(" %");
    
    Serial.print("Gas Level: "); 
    Serial.print(currentData.gasLevel);
    Serial.print(" / "); 
    Serial.println(GAS_SENSOR_MAX_ADC);
    
    Serial.print("Flame Status: "); 
    Serial.println(currentData.flameDetected ? "DETECTED!" : "Clear");
    Serial.println("====================================\n");
}
