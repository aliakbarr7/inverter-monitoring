#include <dev_config.h>
#include "network_WiFi.h"


network_WiFi::network_WiFi()
{
    wifiWasConnected = false;
}

bool network_WiFi::init()
{
    WiFi.begin();
    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
        wifiWasConnected = true;
    }
    else
    {
        wifiWasConnected = false;
    }
    return WiFi.status() == WL_CONNECTED;
}

bool network_WiFi::checkStatus()
{
    if (WiFi.status() != WL_CONNECTED && wifiWasConnected)
    {
        wifiWasConnected = false;
        WiFi.reconnect();
    }

  
    if (WiFi.status() == WL_CONNECTED && !wifiWasConnected)
    {
        wifiWasConnected = true;
    }

    return wifiWasConnected;

}

bool network_WiFi::WiFiConfig()
{
    wifiManager.startConfigPortal(AP_WIFI, AP_PASSWORD);

    if (WiFi.status() == WL_CONNECTED)
    {
        wifiWasConnected = true;
        return true;
    }
    else
    {
        wifiWasConnected = false;
        return false;
    }
}

bool network_WiFi::disconnect()
{
    if (wifiWasConnected) 
    {
        Serial.println("Disconnecting from WiFi...");
        WiFi.disconnect(); 
        wifiWasConnected = false; 
        return true; 
    }
    else
    {
        Serial.println("WiFi was not connected.");
        return false; 
    }
}