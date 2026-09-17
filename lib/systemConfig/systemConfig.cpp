#include "systemConfig.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>

SystemConfig systemConfig = {
    .commMode = "rs485", // default
    .timezone = "CET-1CEST,M3.5.0,M10.5.0/3" // default
};

static const char* CONFIG_FILE = "/system_config.json";

void loadSystemConfig() {
    if (!LittleFS.begin(true)) {
        Log.error(F("[SysCfg]: LittleFS Mount Failed" CR));
        return;
    }
    
    if (!LittleFS.exists(CONFIG_FILE)) {
        Log.notice(F("[SysCfg]: No system config found, using defaults" CR));
        return;
    }

    File file = LittleFS.open(CONFIG_FILE, "r");
    if (!file) {
        Log.error(F("[SysCfg]: Failed to open config file for reading" CR));
        return;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Log.error(F("[SysCfg]: Failed to parse config file: %s" CR), error.c_str());
        return;
    }

    if (doc.containsKey("commMode")) {
        systemConfig.commMode = doc["commMode"].as<String>();
    }
    if (doc.containsKey("timezone")) {
        systemConfig.timezone = doc["timezone"].as<String>();
    }

    Log.notice(F("[SysCfg]: Loaded config - commMode: %s, timezone: %s" CR), 
               systemConfig.commMode.c_str(), systemConfig.timezone.c_str());
}

bool saveSystemConfig() {
    if (!LittleFS.begin(true)) {
        Log.error(F("[SysCfg]: LittleFS Mount Failed" CR));
        return false;
    }

    StaticJsonDocument<512> doc;
    doc["commMode"] = systemConfig.commMode;
    doc["timezone"] = systemConfig.timezone;

    File file = LittleFS.open(CONFIG_FILE, "w");
    if (!file) {
        Log.error(F("[SysCfg]: Failed to open config file for writing" CR));
        return false;
    }

    if (serializeJson(doc, file) == 0) {
        Log.error(F("[SysCfg]: Failed to write to config file" CR));
        file.close();
        return false;
    }
    
    file.close();
    Log.notice(F("[SysCfg]: Config saved successfully" CR));
    return true;
}
