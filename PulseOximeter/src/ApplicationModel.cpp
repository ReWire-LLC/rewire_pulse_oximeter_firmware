#include <Arduino.h>
#include <ReWire_MAX32664.h>
#include "ApplicationModel.h"
#include "./utilities/ReWire_MCU_Serial_Number.h"
#include "./utilities/SupplementalAlgorithms.h"
#include "./utilities/ReWire_Memory.h"

ApplicationModel::ApplicationModel ()
{
    //empty
}

void ApplicationModel::Initialize ()
{
    //Initialize serial communication
    Serial.begin(115200);
    
    //Initiate a 10-second countdown
    /*
    for (int i = 10; i > 0; i--)
    {
        Serial.print("[DEBUG] ");
        Serial.println(i);
        delay(1000);
    }
    Serial.println("[DEBUG] Countdown completed...");
    */

    //Initialize I2C
    Serial.println("[DEBUG] Initializing I2C");
    Wire.begin();

    //Initialize the MAX32664 biohub
    Serial.println("[DEBUG] Initializing MAX32664");
    uint8_t device_mode;
    uint8_t result = max32664.Begin(device_mode);
    if (result == MAX32664_ReadStatusByteValue::SUCCESS && device_mode == MAX32664_DeviceOperatingMode::ApplicationMode)
    {
        //We successfully initialized communication with the sensor
        Serial.println("[DEBUG] Sensor started!");
    }
    else
    {
        //We were not able to communicate with the sensor
        Serial.println("[DEBUG] Could not communicate with the sensor!");
        while(1)
        {
            //empty
        }
    }
    
    //Configure the sensor to output both raw data as well as calculated data
    Serial.println("[DEBUG] Configuring Sensor....");

    result = max32664.ConfigureDevice_SensorAndAlgorithm();
    if(result == MAX32664_ReadStatusByteValue::SUCCESS)
    { 
        Serial.println("[DEBUG] Sensor configured.");
    }
    else 
    {
        Serial.println("[DEBUG] Error configuring sensor.");
        Serial.print("[DEBUG] Error: "); 
        Serial.println(result);
        while(1)
        {
            //empty
        } 
    }
}

void ApplicationModel::Loop ()
{
    //Update the time
    unsigned long t = millis();
    delta_millis = t - current_millis;
    current_millis = t;
    frame_count++;

    //Handle the debug pulse
    if (current_millis >= next_debug_pulse)
    {
        double elapsed_ms = current_millis - (current_millis - debug_pulse_period);

        double ms_per_frame = elapsed_ms / (double) frame_count;
        double frames_per_second = ((double)frame_count) / (elapsed_ms / 1000.0);

        double ms_per_sample = 0;
        double samples_per_second = 0;
        if(sample_count > 0)
        {
            ms_per_sample = elapsed_ms / (double) sample_count;
            samples_per_second = ((double)sample_count) / (elapsed_ms / 1000.0);
        }

        next_debug_pulse = current_millis + debug_pulse_period;
        Serial.print("[DEBUG] ");
        Serial.print("Application debug pulse (millis = ");
        Serial.print(current_millis);
        Serial.print(", ms/frame = ");
        Serial.print(ms_per_frame);
        Serial.print(", f/sec = ");
        Serial.print(frames_per_second);
        Serial.print(", ms/sample = ");
        Serial.print(ms_per_sample);
        Serial.print(", samples/sec = ");
        Serial.print(samples_per_second);        
        Serial.println(") ");

        ReWire_Memory::PrintMemoryMessage();

        frame_count = 0;
        sample_count = 0;
    }

    //Handle any incoming serial messages
    HandleIncomingSerialMessages();

    //Handle streaming of pulse oximetry data
    if (is_streaming_enabled)
    {
        StreamPulseOximeterData();
    }
}


void ApplicationModel::HandleIncomingSerialMessages ()
{
    //Check to see if any bytes are available on the serial line
    int bytes_available = Serial.available();
    if (bytes_available > 0)
    {
        //Create a buffer on the heap to hold the incoming bytes
        char *buffer = (char *) malloc(bytes_available);
        memset(buffer, 0, bytes_available);

        //Read the bytes
        Serial.readBytes(buffer, bytes_available);
        for (int i = 0; i < bytes_available; i++)
        {
            if (buffer[i] == 0xA)
            {
                buffer[i] = 0;
                break;
            }
        }

        //Store the bytes as an Arduino String object
        String command = String(buffer);

        //Free the buffer
        free(buffer);

        //Split the string into two components: the command and any parameters
        int idx_of_space = command.indexOf(' ');
        String cmd_parameter = "";
        if (idx_of_space >= 0)
        {
            cmd_parameter = command.substring(idx_of_space);
            cmd_parameter.trim();
        }

        //If the command starts with "name", then the user either wants to get or set
        //the unique device name
        if (command.startsWith("serial"))
        {
            String serial_number = ReWire_MCU_Serial_Number::as_String();
            Serial.print("[SERIAL] ");
            Serial.println(serial_number);
        }
        else if (command.startsWith("version"))
        {
            Serial.print("[VERSION] ");
            Serial.print(REWIRE_PULSE_OXIMETER_VERSION_MAJOR);
            Serial.print(".");
            Serial.print(REWIRE_PULSE_OXIMETER_VERSION_MINOR);
            Serial.println("");   
        }        
        else if (command.startsWith("stream"))
        {
            if (cmd_parameter.startsWith("on"))
            {
                is_streaming_enabled = true;
            }
            else if (cmd_parameter.startsWith("off"))
            {
                is_streaming_enabled = false;
            }
            else if (cmd_parameter.startsWith("?"))
            {
                Serial.print("[STREAM] ");
                if (is_streaming_enabled)
                {
                    Serial.println("on");
                }
                else
                {
                    Serial.println("off");
                }
            }
        }
    }
}

void ApplicationModel::StreamPulseOximeterData ()
{
    //In this function, we are following the steps outlined in Table 8 (section 3.2) of 
    //  the document "measuring-heart-rate-and-spo2-using-the-max32664a.pdf".
    //  Specifically, we are following the steps 2.1, 2.2, and 2.3 in the "READING SAMPLES"
    //  portion of Table 8.

    //Step 2.1: Read the sensor hub status byte
    uint8_t sensor_hub_status;
    uint8_t read_status = max32664.ReadSensorHubStatus(sensor_hub_status);

    //If the read was successful...
    if (read_status == MAX32664_ReadStatusByteValue::SUCCESS)
    {
        //If the DataRdyInt bit is set...
        //if (sensor_hub_status == 0x08)
        if(true)
        {
            //Step 2.2: Get the number of samples in the FIFO
            uint8_t num_available_samples;
            read_status = max32664.ReadNumberAvailableSamples(num_available_samples);

            //If the read was successful and if there are samples available to read...
            if (read_status == MAX32664_ReadStatusByteValue::SUCCESS && num_available_samples > 0)
            {
                sample_count += num_available_samples;

                //Step 2.3: Read the data stored in the FIFO
                for (int i = 0; i < num_available_samples; i++)
                {
                    MAX32664_Data current_sample;
                    read_status = max32664.ReadSample_SensorAndAlgorithm(current_sample);

                    //If the sample was successfully read...
                    if (read_status == MAX32664_ReadStatusByteValue::SUCCESS)
                    {
                        //Pass the data into the supplemental algorithm
                        supplemental_algorithms.AddSample(current_sample.ir);

                        //Calculate perfusion index (if it is time to do so)
                        if (current_millis >= (last_perfusion_index_calculation_millis + perfusion_index_calculation_period))
                        {
                            //Set the last time that PI was calculated to be the current time
                            last_perfusion_index_calculation_millis = current_millis;

                            //Calculate PI (perfusion index)
                            current_perfusion_index = supplemental_algorithms.CalculatePerfusionIndex();
                        }

                        //Output the sample data to over serial communication
                        Serial.print("[DATA]\t");
                        Serial.print(current_millis);
                        Serial.print("\t");
                        Serial.print(current_sample.ir);
                        Serial.print("\t");
                        Serial.print(current_sample.red);
                        Serial.print("\t");
                        Serial.print(current_sample.hr);
                        Serial.print("\t");
                        Serial.print(current_sample.hr_confidence);
                        Serial.print("\t");
                        Serial.print(current_sample.spo2);
                        Serial.print("\t");
                        Serial.print(current_sample.algorithm_state);
                        Serial.print("\t");
                        Serial.print(current_sample.algorithm_status);
                        Serial.print("\t");
                        Serial.print(current_sample.interbeat_interval);
                        Serial.print("\t");
                        Serial.print(current_perfusion_index);
                        Serial.println("");
                    }
                }
            }
        }
    }
}
