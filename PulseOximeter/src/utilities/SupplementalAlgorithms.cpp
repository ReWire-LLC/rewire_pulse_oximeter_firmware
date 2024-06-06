#include "SupplementalAlgorithms.h"
#include "ReWire_CalculationType.h"
#include <math.h>

SupplementalAlgorithms::SupplementalAlgorithms()
{
    recent_ir_signal_mean_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_signal_mean_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_signal_min_max_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Min);
    recent_ir_signal_min_max_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Min);
    recent_ir_peaks_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_peaks_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_troughs_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_troughs_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);

    recent_ir_peak_distance_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_ir_peak_distance_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);

    recent_red_maximum_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Max);
    recent_red_maximum_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Max);
    recent_red_minimum_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Min);
    recent_red_minimum_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Min);
    recent_red_mean_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_red_mean_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
}

float SupplementalAlgorithms::get_current_min ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_ir_signal_min_max_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_ir_signal_min_max_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_max ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_ir_signal_min_max_stats[0].GetCurrentValue(ReWire_CalculationType::Calculation_Max);
    }
    else
    {
        return recent_ir_signal_min_max_stats[1].GetCurrentValue(ReWire_CalculationType::Calculation_Max);
    }
}

float SupplementalAlgorithms::get_current_mean ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_ir_signal_mean_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_ir_signal_mean_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_stddev ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_ir_signal_mean_stats[0].GetCurrentValue(ReWire_CalculationType::Calculation_StdDev);
    }
    else
    {
        return recent_ir_signal_mean_stats[1].GetCurrentValue(ReWire_CalculationType::Calculation_StdDev);
    }
}

float SupplementalAlgorithms::get_current_peak_mean ()
{
    if (current_peaks_position < REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        return recent_ir_peaks_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_ir_peaks_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_trough_mean ()
{
    if (current_troughs_position < REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        return recent_ir_troughs_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_ir_troughs_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_mean_peak_distance ()
{
    if (current_peaks_position < REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        return recent_ir_peak_distance_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_ir_peak_distance_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_red_max ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_red_maximum_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_red_maximum_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_red_min ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_red_minimum_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_red_minimum_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_red_mean ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_red_mean_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_red_mean_stats[1].GetCurrentValue();
    }
}

void SupplementalAlgorithms::add_peak (uint32_t sample)
{
    //Save the sample as a peak in the "recent_ir_signal_peaks" array
    recent_ir_signal_peaks[current_peaks_position] = sample;

    //Update the peak stats
    recent_ir_peaks_stats[0].UpdateCalculation(sample);
    recent_ir_peaks_stats[1].UpdateCalculation(sample);

    //Update the "peak distance" stats
    recent_ir_peak_distance_stats[0].UpdateCalculation(samples_since_last_peak);
    recent_ir_peak_distance_stats[1].UpdateCalculation(samples_since_last_peak);    

    //Reset the "samples since last peak" to now be 0
    samples_since_last_peak = 0;

    //Update the "current position" in the ring-buffers
    current_peaks_position++;
    if (current_peaks_position >= REWIRE_SUPP_ALGO_PEAKS_LENGTH)
    {
        current_peaks_position = 0;
        recent_ir_peaks_stats[1].ResetCalculation();
        recent_ir_peak_distance_stats[1].ResetCalculation();
    }
    else if (current_peaks_position == REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        recent_ir_peaks_stats[0].ResetCalculation();
        recent_ir_peak_distance_stats[0].ResetCalculation();
    }
}

void SupplementalAlgorithms::add_trough (uint32_t sample)
{
    //Save the sample as a peak in the "recent_ir_signal_troughs" array
    recent_ir_signal_troughs[current_troughs_position] = sample;

    //Update the peak stats
    recent_ir_troughs_stats[0].UpdateCalculation(sample);
    recent_ir_troughs_stats[1].UpdateCalculation(sample);

    //Update the "current position" in the ring-buffers
    current_troughs_position++;
    if (current_troughs_position >= REWIRE_SUPP_ALGO_PEAKS_LENGTH)
    {
        current_troughs_position = 0;
        recent_ir_troughs_stats[1].ResetCalculation();
    }
    else if (current_troughs_position == REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        recent_ir_troughs_stats[0].ResetCalculation();
    }    
}

void SupplementalAlgorithms::update_peaks_and_troughs (uint32_t sample)
{
    //Increment the number of samples since the last "peak" was found
    samples_since_last_peak++;

    //Update the peaks/troughs
    if (is_current_segment_positive)
    {
        //If the new sample is less than the signal mean
        if (sample < recent_ir_signal_mean[current_signal_position])
        {
            //This segment is completed

            //Set the new segment to be a negative segment, leading towards a trough
            is_current_segment_positive = false;

            //Check to see if the current segment peak is far enough away from the mean to be counted
            //as a legitimate peak
            float current_stddev = get_current_stddev();
            if (current_stddev != 0)
            {
                float z_score = (current_segment_peak_or_trough - recent_ir_signal_mean[current_signal_position]) / current_stddev;
                if (abs(z_score) > 1)
                {
                    add_peak(current_segment_peak_or_trough);
                }
            }

            //Set the current segment peak/trough initial value to be the value of the current sample
            current_segment_peak_or_trough = sample;
        }
        else
        {
            //This segment is ongoing

            //Check to see if the new sample is greater than any previous value
            if (sample > current_segment_peak_or_trough)
            {
                current_segment_peak_or_trough = sample;
            }
        }
    }
    else
    {
        //If the new sample is greater than the signal mean
        if (sample > recent_ir_signal_mean[current_signal_position])
        {
            //This segment is completed

            //Set the new segment to be a positive segment, leading towards a peak
            is_current_segment_positive = true;

            //Check to see if the current segment peak is far enough away from the mean to be counted
            //as a legitimate peak
            float current_stddev = get_current_stddev();
            if (current_stddev != 0)
            {
                float z_score = (current_segment_peak_or_trough - recent_ir_signal_mean[current_signal_position]) / current_stddev;
                if (abs(z_score) > 1)
                {
                    add_trough(current_segment_peak_or_trough);
                }            
            }

            //Set the current segment peak/trough initial value to be the value of the current sample
            current_segment_peak_or_trough = sample;
        }
        else
        {
            //This segment is ongoing
            
            //Check to see if the new sample is less than any previous value
            if (sample < current_segment_peak_or_trough)
            {
                current_segment_peak_or_trough = sample;
            }
        }
    }
}

void SupplementalAlgorithms::AddSample (uint32_t ir_sample, uint32_t red_sample)
{
    //Place the new sample into the "recent_ir_signal" ring-buffer
    recent_ir_signal[current_signal_position] = ir_sample;
    
    //Update our calculations for mean and standard deviation of the recent signal
    recent_ir_signal_mean_stats[0].UpdateCalculation(ir_sample);
    recent_ir_signal_mean_stats[1].UpdateCalculation(ir_sample);
    recent_ir_signal_min_max_stats[0].UpdateCalculation(ir_sample);
    recent_ir_signal_min_max_stats[1].UpdateCalculation(ir_sample);

    //Update the "recent_ir_signal_mean" ring-buffer
    recent_ir_signal_mean[current_signal_position] = get_current_mean();

    //Update the peaks and troughs
    update_peaks_and_troughs(ir_sample);

    //Update the red signal stats
    recent_red_maximum_stats[0].UpdateCalculation(red_sample);
    recent_red_maximum_stats[1].UpdateCalculation(red_sample);
    recent_red_minimum_stats[0].UpdateCalculation(red_sample);
    recent_red_minimum_stats[1].UpdateCalculation(red_sample);
    recent_red_mean_stats[0].UpdateCalculation(red_sample);
    recent_red_mean_stats[1].UpdateCalculation(red_sample);

    //Update the "current position" in the ring-buffers
    current_signal_position++;
    if (current_signal_position >= REWIRE_SUPP_ALGO_SIGNAL_LENGTH)
    {
        current_signal_position = 0;
        recent_ir_signal_mean_stats[1].ResetCalculation();
        recent_ir_signal_min_max_stats[1].ResetCalculation();

        recent_red_maximum_stats[1].ResetCalculation();
        recent_red_minimum_stats[1].ResetCalculation();
        recent_red_mean_stats[1].ResetCalculation();
    }
    else if (current_signal_position == REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        recent_ir_signal_mean_stats[0].ResetCalculation();
        recent_ir_signal_min_max_stats[0].ResetCalculation();

        recent_red_maximum_stats[0].ResetCalculation();
        recent_red_minimum_stats[0].ResetCalculation();
        recent_red_mean_stats[0].ResetCalculation();        
    }

    //Debug output
    /*
    Serial.print(ir_sample);
    Serial.print(" ");
    Serial.print(red_sample);
    Serial.print(" ");
    Serial.print(get_current_mean());
    Serial.print(" ");
    Serial.print(get_current_max());
    Serial.print(" ");
    Serial.print(get_current_min());
    Serial.print(" ");
    Serial.print(get_current_red_mean());
    Serial.print(" ");
    Serial.print(get_current_red_max());
    Serial.print(" ");
    Serial.print(get_current_red_min());
    Serial.println("");
    */
}

float SupplementalAlgorithms::CalculatePerfusionIndex ()
{
    //The following sources were used to determine the calculation for the perfusion index:
    //1. https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3417976/
    //2. https://www.analog.com/en/technical-articles/guidelines-for-spo2-measurement--maxim-integrated.html
    //3. https://dsp.stackexchange.com/questions/46615/calculate-spo%E2%82%82-value-from-raw-fingertip-plethysmography-ppg
    //4. https://www.ti.com/lit/an/slaa655/slaa655.pdf

    float dc_component = get_current_mean();
    float ac_component = get_current_max() - get_current_min();
    float pi = 0;
    if (dc_component != 0)
    {
        pi = abs((ac_component / dc_component) * 100.0f);    
    }
    
    return pi;
}

float SupplementalAlgorithms::CalculateBeatsPerMinute ()
{
    //Heart rate (beats per minute) is calculated as:
    //  BPM = (Sampling Rate / Distance between peaks) * 60
    //  The sampling rate from the MAX30101 is currently 100 samples per second

    float peak_distance = get_current_mean_peak_distance();
    if (peak_distance > 0)
    {
        float bpm = (REWIRE_SUPP_ALGO_SAMPLING_RATE_FLOAT / peak_distance) * 60.0f;
        return round(bpm);
    }
    else
    {
        return 0;
    }
}

float SupplementalAlgorithms::CalculateSpO2 ()
{
    // SpO2 (blood-oxygen concentration) is calculated as:
    //
    //  SpO2 = aR^2 + bR + c
    //
    // The default values for a, b, and c as defined in the Maxim documentation for the MAX30101 are:
    //  
    //  a = 1.5958422
    //  b = -34.6596622
    //  c = 112.6898759
    //
    // However, the SparkFun library uses the following values for a, b and c:
    //
    //  a = -45.060
    //  b = 30.354
    //  c = 94.845
    //
    // The Maxim documentation states that their default values were "without the optical shield", but the
    // MAX30101 contains an optical shield on it. I don't know if that optical shield affects the calibration values
    // at all.
    //
    // Anyway, after some testing, it seems like the SparkFun values work well, so I have chosen to use those values
    // for the time being. These values can be revisited in the future if it is necessary to do so.
    //
    // The value for R is calculated as follows:
    //
    //  R = (AC_red / DC_red) / (AC_ir / DC_ir)
    //
    //  The above equation for R can be re-written as the following:
    //
    //  R = (AC_red * DC_ir) / (AC_ir * DC_red)
    //
    //  Re-writing the equation like so eliminates some of the division operations, which could speed up the computation.
    //
    //See the following article:
    //  https://www.analog.com/en/technical-articles/guidelines-for-spo2-measurement--maxim-integrated.html

    float DC_ir = get_current_mean();
    float AC_ir = get_current_max() - get_current_min();
    float DC_red = get_current_red_mean();
    float AC_red = get_current_red_max() - get_current_red_min();

    if (DC_red > 0 && DC_ir > 0)
    {
        float R = (AC_red * DC_ir) / (AC_ir * DC_red);
        float SpO2 = (REWIRE_SUPP_ALGO_CONST_A * pow(R, 2)) + (REWIRE_SUPP_ALGO_CONST_B * R) + REWIRE_SUPP_ALGO_CONST_C;

        //Debug output
        /*
        Serial.print("DC_ir = ");
        Serial.print(DC_ir);
        Serial.print(", AC_ir = ");
        Serial.print(AC_ir);
        Serial.print(", DC_red = ");
        Serial.print(DC_red);
        Serial.print(", AC_red = ");
        Serial.print(AC_red);
        Serial.print(", AC_red / DC_red = ");
        Serial.print(AC_red / DC_red);
        Serial.print(", AC_ir / DC_ir = ");
        Serial.print(AC_ir / DC_ir);
        Serial.print(", R = ");
        Serial.println(R);
        */

        return SpO2;
    }
    else
    {
        return 0;
    }
}


SupplementalAlgorithms supplemental_algorithms;
