#include "alerts.h"

// =====================================================
// CONSTRUCTOR
// =====================================================
AlertManager::AlertManager() {
    currentLevel = LEVEL_SAFE;
    lastBuzzerTime = 0;
}

// =====================================================
// INITIALIZATION
// =====================================================
void AlertManager::begin() {
    // Configure output pins
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    
    // Initial state: all off
    clearAlerts();
    
    Serial.println("[ALERT] Alert system initialized");
}

// =====================================================
// EVALUATE HAZARD CONDITIONS
// =====================================================
AlertLevel AlertManager::evaluateConditions(SensorData data) {
    AlertLevel level = LEVEL_SAFE;
    
    // Check for DANGER conditions (highest priority)
    if (data.flameDetected) {
        Serial.println("[HAZARD] FLAME DETECTED!");
        return LEVEL_DANGER;
    }
    
    if (data.gasLevel >= GAS_THRESHOLD_DANGER) {
        Serial.println("[HAZARD] CRITICAL GAS LEVEL!");
        return LEVEL_DANGER;
    }
    
    if (data.temperature >= TEMP_THRESHOLD_DANGER) {
        Serial.println("[HAZARD] CRITICAL TEMPERATURE!");
        return LEVEL_DANGER;
    }
    
    // Check for WARNING conditions
    if (data.gasLevel >= GAS_THRESHOLD_WARNING) {
        Serial.println("[WARNING] Elevated gas level");
        level = LEVEL_WARNING;
    }
    
    if (data.temperature >= TEMP_THRESHOLD_WARNING) {
        Serial.println("[WARNING] High temperature");
        level = LEVEL_WARNING;
    }
    
    if (data.humidity < HUMIDITY_THRESHOLD_LOW || 
        data.humidity > HUMIDITY_THRESHOLD_HIGH) {
        Serial.println("[WARNING] Humidity out of range");
        level = LEVEL_WARNING;
    }
    
    return level;
}

// =====================================================
// TRIGGER ALERT
// =====================================================
void AlertManager::triggerAlert(AlertLevel level) {
    if (level == currentLevel) {
        return; // No change
    }
    
    currentLevel = level;
    updateLEDs(level);
    
    switch(level) {
        case LEVEL_SAFE:
            Serial.println("[STATUS] System SAFE - All clear");
            noTone(PIN_BUZZER);
            break;
            
        case LEVEL_WARNING:
            Serial.println("[STATUS] WARNING - Monitor conditions");
            activateBuzzer(BUZZER_FREQ_WARNING, BUZZER_DURATION);
            break;
            
        case LEVEL_DANGER:
            Serial.println("[STATUS] DANGER - Take immediate action!");
            activateBuzzer(BUZZER_FREQ_DANGER, BUZZER_DURATION * 2);
            break;
    }
}

// =====================================================
// BUZZER CONTROL
// =====================================================
void AlertManager::activateBuzzer(int frequency, int duration) {
    unsigned long currentTime = millis();
    
    // Continuous alarm for danger
    if (currentLevel == LEVEL_DANGER) {
        tone(PIN_BUZZER, frequency);
    } 
    // Intermittent beep for warning
    else if (currentTime - lastBuzzerTime >= 2000) {
        tone(PIN_BUZZER, frequency);
        delay(duration);
        noTone(PIN_BUZZER);
        lastBuzzerTime = currentTime;
    }
}

// =====================================================
// LED INDICATORS
// =====================================================
void AlertManager::updateLEDs(AlertLevel level) {
    // Turn off all LEDs
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    
    // Activate appropriate LED
    switch(level) {
        case LEVEL_SAFE:
            digitalWrite(PIN_LED_GREEN, HIGH);
            break;
        case LEVEL_WARNING:
            digitalWrite(PIN_LED_YELLOW, HIGH);
            break;
        case LEVEL_DANGER:
            digitalWrite(PIN_LED_RED, HIGH);
            break;
    }
}

// =====================================================
// CLEAR ALL ALERTS
// =====================================================
void AlertManager::clearAlerts() {
    digitalWrite(PIN_LED_GREEN, HIGH);  // Safe by default
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    noTone(PIN_BUZZER);
    currentLevel = LEVEL_SAFE;
}

// =====================================================
// GET LEVEL STRING
// =====================================================
const char* AlertManager::getLevelString(AlertLevel level) {
    switch(level) {
        case LEVEL_SAFE: return "SAFE";
        case LEVEL_WARNING: return "WARNING";
        case LEVEL_DANGER: return "DANGER";
        default: return "UNKNOWN";
    }
}
