#ifndef __REWIRE_ONLINE_CALCULATOR_H
#define __REWIRE_ONLINE_CALCULATOR_H

#include <Arduino.h>
#include <math.h>
#include "ReWire_CalculationType.h"

class ReWire_OnlineCalculator
{
    private:
        uint8_t calculation_type;
        float a, b, n;

    public:

        ReWire_OnlineCalculator();

        void SetCalculationType (uint8_t new_calculation_type);
        uint8_t GetCalculationType ();

        void ResetCalculation ();
        void UpdateCalculation (float new_sample);
        float GetCurrentValue ();
        float GetCurrentValue (uint8_t temp_calculation_type);

        void DebugOutputCurrentState ();
};

#endif /* __REWIRE_ONLINE_CALCULATOR_H */