#ifndef __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H
#define __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H

#include <Arduino.h>
#include <Wire.h>
#include <ReWire_MAX32664.h>

class ApplicationModel
{
    private:

        //These variables are for debug purposes only
        unsigned long next_debug_pulse = 0;
        unsigned long debug_pulse_period = 10000;

        //These variables store information about the time (in units of millis)
        unsigned long current_millis;
        unsigned long delta_millis;    

        //Boolean value to store the streaming state
        bool is_streaming_enabled = false;

        // Reset pin, MFIO pin
        int reset_pin = 0;
        int mfio_pin = 2;

        ReWire_MAX32664 max32664 = ReWire_MAX32664(&Wire, mfio_pin, reset_pin);    

    public:

        ApplicationModel ();

        void Initialize ();
        void Loop ();

        void HandleIncomingSerialMessages ();
        void StreamPulseOximeterData ();

};



#endif /* __REWIRE_PULSE_OXIMETER_APPLICATION_MODEL_H */
