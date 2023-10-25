#ifndef __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H
#define __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H

#include <Arduino.h>
#include <Wire.h>
#include <ReWire_MAX32664.h>

#define REWIRE_PULSE_OXIMETER_VERSION_MAJOR         1
#define REWIRE_PULSE_OXIMETER_VERSION_MINOR         1

class ApplicationModel
{
    private:

        //These variables are for debug purposes only
        unsigned long next_debug_pulse = 0;
        unsigned long debug_pulse_period = 10000;

        //These variables store information about the time (in units of millis)
        unsigned long current_millis;
        unsigned long delta_millis;    
        unsigned long frame_count = 0;
        unsigned long sample_count = 0;

        //Boolean value to store the streaming state
        bool is_streaming_enabled = false;

        // Reset pin, MFIO pin
        int reset_pin = 0;
        int mfio_pin = 2;

        ReWire_MAX32664 max32664 = ReWire_MAX32664(&Wire, mfio_pin, reset_pin);  

        //Variables related to perfusion index
        float current_perfusion_index = 0;
        unsigned long last_perfusion_index_calculation_millis = 0;
        unsigned long perfusion_index_calculation_period = 1000;  

    public:

        ApplicationModel ();

        void Initialize ();
        void Loop ();

        void HandleIncomingSerialMessages ();
        void StreamPulseOximeterData ();

};



#endif /* __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H */
