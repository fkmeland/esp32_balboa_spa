#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <Arduino.h>

struct SystemConfig {
    String commMode; // "rs485" or "ttl"
    String timezone; // e.g. "CET-1CEST,M3.5.0,M10.5.0/3"
};

extern SystemConfig systemConfig;

void loadSystemConfig();
bool saveSystemConfig();

#endif // SYSTEM_CONFIG_H
