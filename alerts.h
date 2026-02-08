#ifndef ALERTS_H
#define ALERTS_H

#include <Arduino.h>
#include "config.h"
#include "sensors.h"

// =====================================================
// ALERT LEVELS
// =====================================================
enum AlertLevel {
    LEVEL_SAFE = 0,
    LEVEL_WARNING = 1,
    LEVEL_DANGER = 2
};

// =====================================================
// ALERT MANAGER CLASS
// =====================================================
class AlertManager {
private:
    AlertLevel currentLevel;
    unsigned long lastBuzzerTime;
    
    void activateBuzzer(int frequency, int duration);
    void updateLEDs(AlertLevel level);

public:
    AlertManager();
    void begin();
    AlertLevel evaluateConditions(SensorData data);
    void triggerAlert(AlertLevel level);
    void clearAlerts();
    const char* getLevelString(AlertLevel level);
};

#endif // ALERTS_H
