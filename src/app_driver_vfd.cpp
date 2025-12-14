#include "driver/inverter_vfd.h"

InverterVFD vfd(Serial2, 9600);

void handleSerialCommand()
{
    if (Serial.available() > 0)
    {

        String commandString = Serial.readStringUntil('\n');

        commandString.trim();
        commandString.toUpperCase();

        Serial.printf("\n[CMD] Received: %s\n", commandString.c_str());

        // (INV,RUN atau INV,STOP)
        if (commandString.startsWith("INV,"))
        {
            String action = commandString.substring(4);

            if (action == "RUN")
            {
                if (vfd.start())
                {
                    Serial.println("[VFD] Command RUN sent successfully.");
                }
                else
                {
                    Serial.println("[VFD] Failed to send RUN command (Modbus error).");
                }
            }
            else if (action == "STOP")
            {
                if (vfd.stop())
                {
                    Serial.println("[VFD] Command STOP sent successfully.");
                }
                else
                {
                    Serial.println("[VFD] Failed to send STOP command (Modbus error).");
                }
            }
            else
            {
                Serial.println("[CMD] Invalid INV action. Use INV,RUN or INV,STOP.");
            }
        }
        // (SETF,50)
        else if (commandString.startsWith("SETF,"))
        {
            String valueStr = commandString.substring(5);

            float newFreq = valueStr.toFloat();

            if (newFreq > 0.0f && newFreq <= 60.0f)
            {
                if (vfd.setSpeedHz(newFreq))
                {
                    Serial.printf("[VFD] Frequency set to %.2f Hz successfully.\n", newFreq);
                }
                else
                {
                    Serial.println("[VFD] Failed to set frequency (Modbus error).");
                }
            }
            else
            {
                Serial.println("[CMD] Invalid frequency value or out of range (0-60 Hz).");
            }
        }
        else
        {
            Serial.println("[CMD] Unknown command. Use INV,RUN; INV,STOP; or SETF,XX.");
        }
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("--- VFD Modbus Control Ready ---");
    Serial.println("Commands: INV,RUN | INV,STOP | SETF,XX (e.g., SETF,50)");

    vfd.begin(1);
}

void loop()
{
    handleSerialCommand();

    float hz, curr, volt;

    if (vfd.readOutputFrequency(hz))
        Serial.printf("Freq: %.2f Hz\t", hz);

    if (vfd.readOutputCurrent(curr))
        Serial.printf("Current: %.2f A\t", curr);

    if (vfd.readOutputVoltage(volt))
        Serial.printf("Voltage: %.2f V\n", volt);

    delay(2000);
}