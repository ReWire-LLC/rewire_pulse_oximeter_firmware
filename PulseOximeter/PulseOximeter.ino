/*
 This example sketch gives you exactly what the SparkFun Pulse Oximiter and
 Heart Rate Monitor is designed to do: read heart rate and blood oxygen levels.
 This board requires I-squared-C connections but also connections to the reset
 and mfio pins. When using the device keep LIGHT and CONSISTENT pressure on the
 sensor. Otherwise you may crush the capillaries in your finger which results
 in bad or no results. A summary of the hardware connections are as follows: 
 SDA -> SDA
 SCL -> SCL
 RESET -> PIN 4
 MFIO -> PIN 5

 Author: Elias Santistevan
 Date: 8/2019
 SparkFun Electronics

 If you run into an error code check the following table to help diagnose your
 problem: 
 1 = Unavailable Command
 2 = Unavailable Function
 3 = Data Format Error
 4 = Input Value Error
 5 = Try Again
 255 = Error Unknown
*/

#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>

// Reset pin, MFIO pin
int resPin = 0;
int mfioPin = 2;

// Takes address, reset pin, and MFIO pin.
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); 

bioData body;  
// ^^^^^^^^^
// What's this!? This is a type (like int, byte, long) unique to the SparkFun
// Pulse Oximeter and Heart Rate Monitor. Unlike those other types it holds
// specific information on your heartrate and blood oxygen levels. BioData is
// actually a specific kind of type, known as a "struct". 
// You can choose another variable name other than "body", like "blood", or
// "readings", but I chose "body". Using this "body" varible in the 
// following way gives us access to the following data: 
// body.heartrate  - Heartrate
// body.confidence - Confidence in the heartrate value
// body.oxygen     - Blood oxygen level
// body.status     - Has a finger been sensed?


void setup()
{
    //Initialize serial communication
    Serial.begin(115200);
    
    //Initiate a 10-second countdown
    for (int i = 10; i > 0; i--)
    {
        Serial.print("[DEBUG] ");
        Serial.println(i);
        delay(1000);
    }
    Serial.println("[DEBUG] Countdown completed...");

    //Initialize I2C
    Wire.begin();

    //Initialize the MAX32664 biohub
    int result = bioHub.begin();
    if (result == 0)
    {
        // Zero errors!
        Serial.println("[DEBUG] Sensor started!");
    }
    else
    {
        Serial.println("[DEBUG] Could not communicate with the sensor!");
    }
    
    //Configure the sensor to output both raw data as well as calculated data
    Serial.println("[DEBUG] Configuring Sensor...."); 
    int error = bioHub.configSensorBpm(MODE_ONE);
    if(error == 0)
    { 
        // Zero errors!
        Serial.println("[DEBUG] Sensor configured.");
    }
    else 
    {
        Serial.println("[DEBUG] Error configuring sensor.");
        Serial.print("[DEBUG] Error: "); 
        Serial.println(error); 
    }

    // Data lags a bit behind the sensor, if you're finger is on the sensor when
    // it's being configured this delay will give some time for the data to catch
    // up. 
    Serial.println("[DEBUG] Loading up the buffer with data....");
    delay(4000);
}

void loop(){

    // Information from the readSensorBpm function will be saved to our "body" variable.  
    body = bioHub.readSensorBpm();

    Serial.print("[DATA]\t");
    Serial.print(body.irLed);
    Serial.print("\t");
    Serial.print(body.redLed);
    Serial.print("\t");
    Serial.print(body.heartRate); 
    Serial.print("\t");
    Serial.print(body.confidence); 
    Serial.print("\t");
    Serial.print(body.oxygen); 
    Serial.print("\t");
    Serial.print(body.status); 
    Serial.println("");

    /*
    Serial.print("IR LED: ");
    Serial.print(body.irLed);
    Serial.print(", Red LED: ");
    Serial.print(body.redLed);
    Serial.print(", Heartrate: ");
    Serial.print(body.heartRate); 
    Serial.print(", Confidence: ");
    Serial.print(body.confidence); 
    Serial.print(", Oxygen: ");
    Serial.print(body.oxygen); 
    Serial.print(", Status: ");
    Serial.print(body.status); 
    switch (body.status)
    {
        case 0:
            Serial.print(" (Success)");
            break;
        case 1:
            Serial.print(" (Not Ready)");
            break;
        case 2:
            Serial.print(" (Object Detected)");
            break;
        case 3:
            Serial.print(" (Finger Detected)");
            break;
    }
    Serial.println("");
    */
    
    // Slow it down or your heart rate will go up trying to keep up
    // with the flow of numbers
    delay(250); 
}
