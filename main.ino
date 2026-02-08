/*
 * Industrial Safety Monitoring System
 * ESP32-based hazard detection with multi-sensor monitoring
 * 
 * Features:
 * - Gas/Smoke detection
 * - Temperature monitoring
 * - Humidity monitoring
 * - Flame detection
 * - Real-time alerts (Buzzer + LED)
 */

#include "config.h"
#include "sensors.h"
#include "alerts.h"

// =====================================================
// GLOBAL OBJECTS
// =====================================================
SensorManager sensorManager;
AlertManager alertManager;

// =====================================================
// SETUP
// =====================================================
void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) { delay(10); }
    
    Serial.println("\n\n");
    Serial.println("=========================================");
    Serial.println("  INDUSTRIAL SAFETY MONITORING SYSTEM   ");
    Serial.println("=========================================");
    Serial.println("Initializing system...\n");
    
    // Initialize subsystems
    sensorManager.begin();
    alertManager.begin();
    
    Serial.println("\n[SYSTEM] Initialization complete!");
    Serial.println("[SYSTEM] Monitoring started...\n");
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop() {
    // Read sensor data
    if (sensorManager.readSensors()) {
        SensorData data = sensorManager.getData();
        
        // Print sensor readings
        sensorManager.printData();
        
        // Evaluate conditions and trigger alerts
        AlertLevel level = alertManager.evaluateConditions(data);
        alertManager.triggerAlert(level);
        
        // Print current status
        Serial.print("[STATUS] Current Alert Level: ");
        Serial.println(alertManager.getLevelString(level));
        Serial.println("----------------------------------------");
    }
    
    // Small delay for system stability
    delay(100);
}
