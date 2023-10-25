#include "ReWire_OnlineCalculator.h"

ReWire_OnlineCalculator::ReWire_OnlineCalculator()
{
    SetCalculationType(ReWire_CalculationType::Calculation_NewestValue);
}

void ReWire_OnlineCalculator::SetCalculationType (uint8_t new_calculation_type)
{
    calculation_type = new_calculation_type;
    ResetCalculation();
}

uint8_t ReWire_OnlineCalculator::GetCalculationType ()
{
    return calculation_type;
}

void ReWire_OnlineCalculator::ResetCalculation ()
{
    a = NAN;
    b = NAN;
    n = 0;
}

void ReWire_OnlineCalculator::UpdateCalculation (float new_sample)
{
    float delta, delta2;

    switch (calculation_type)
    {
        case ReWire_CalculationType::Calculation_OldestValue:

            //Only store the value if no previous values have been stored
            if (n == 0)
            {
                a = new_sample;
                n++;
            }

            break;

        case ReWire_CalculationType::Calculation_NewestValue:

            //Always update the value to be the new sample value
            a = new_sample;
            break;

        case ReWire_CalculationType::Calculation_Min:
        case ReWire_CalculationType::Calculation_Max:

            if (n == 0)
            {
                a = new_sample;
                b = new_sample;
            }
            else
            {
                if (new_sample < a)
                {
                    a = new_sample;
                }

                if (new_sample > b)
                {
                    b = new_sample;
                }
            }

            n++;

            break;

        case ReWire_CalculationType::Calculation_Mean:
        case ReWire_CalculationType::Calculation_SquaredDistanceFromMean:
        case ReWire_CalculationType::Calculation_Variance:
        case ReWire_CalculationType::Calculation_StdDev:

            //For these calculations, "a" holds the mean,
            //and "b" holds the squared distance from the mean

            if (n == 0)
            {
                a = 0;
                b = 0;
            }

            //Increment the count
            n++;

            //Calculate the new mean
            delta = new_sample - a;
            a += delta / n;

            //Calculate the new squared distance from the mean
            delta2 = new_sample - a;
            b += delta * delta2;

            break;

        case ReWire_CalculationType::Calculation_Median:

            /* 
            * This is an implementation of the Feldman/Shavitt FAME algorithm
            * for estimating the median without having access to every sample
            * value. It maintains constant memory usage. 
            * 
            * Sources:
            * (1) https://stats.stackexchange.com/questions/103258/online-estimation-of-quartiles-without-storing-observations
            * (2) http://www.eng.tau.ac.il/~shavitt/courses/LargeG/streaming-median.pdf
            */

            if (n == 0)
            {
                a = new_sample;
                b = max(abs(a / 2.0f), 0.1);
            }
            else
            {
                if (a > new_sample)
                {
                    a -= b;
                }
                else if (a < new_sample)
                {
                    a += b;
                }

                if (abs(new_sample - a) < b)
                {
                    b /= 2.0f;
                }
            }

            break;   
    }
}

float ReWire_OnlineCalculator::GetCurrentValue ()
{
    return GetCurrentValue(calculation_type);
}

float ReWire_OnlineCalculator::GetCurrentValue (uint8_t temp_calculation_type)
{
    switch (temp_calculation_type)
    {
        case ReWire_CalculationType::Calculation_OldestValue:
        case ReWire_CalculationType::Calculation_NewestValue:
        case ReWire_CalculationType::Calculation_Min:
        case ReWire_CalculationType::Calculation_Mean: 
        case ReWire_CalculationType::Calculation_Median:       
            return a;

        case ReWire_CalculationType::Calculation_Max:
        case ReWire_CalculationType::Calculation_SquaredDistanceFromMean:
            return b;

        case ReWire_CalculationType::Calculation_Variance:
            return ((n > 0) ? (b / n) : 0);
        case ReWire_CalculationType::Calculation_StdDev:
            return ((n > 0) ? (sqrt(b / n)) : 0);
    }
}

void ReWire_OnlineCalculator::DebugOutputCurrentState ()
{
    Serial.print(", a = ");
    Serial.print(a);
    Serial.print(", b = ");
    Serial.print(b);
    Serial.print(", n = ");
    Serial.print(n);
    Serial.print(", stddev = ");
    Serial.print(GetCurrentValue(ReWire_CalculationType::Calculation_StdDev));
    Serial.println("");
}