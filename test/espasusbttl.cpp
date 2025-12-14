// #include <HardwareSerial.h>

// HardwareSerial RS485Serial(1);

// #define RX 16
// #define TX 17

// void setup()
// {
//     // put your setup code here, to run once:
//     Serial.begin(9600);
//     RS485Serial.begin(9600, SERIAL_8N1, RX, TX);
// }

// void loop()
// {
//     // put your main code here, to run repeatedly:
//     if (RS485Serial.available())
//     {
//         Serial.write(RS485Serial.read());
//     }

//     if (Serial.available())
//     {
//         RS485Serial.write(Serial.read());
//     }
// }
