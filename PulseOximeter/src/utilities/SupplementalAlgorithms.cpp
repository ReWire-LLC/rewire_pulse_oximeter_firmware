#include "SupplementalAlgorithms.h"
#include "ReWire_CalculationType.h"
#include <math.h>

SupplementalAlgorithms::SupplementalAlgorithms()
{
    recent_signal_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_signal_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_peaks_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_peaks_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_troughs_stats[0].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
    recent_troughs_stats[1].SetCalculationType(ReWire_CalculationType::Calculation_Mean);
}

float SupplementalAlgorithms::get_current_mean ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_signal_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_signal_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_stddev ()
{
    if (current_signal_position < REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        return recent_signal_stats[0].GetCurrentValue(ReWire_CalculationType::Calculation_StdDev);
    }
    else
    {
        return recent_signal_stats[1].GetCurrentValue(ReWire_CalculationType::Calculation_StdDev);
    }
}

float SupplementalAlgorithms::get_current_peak_mean ()
{
    if (current_peaks_position < REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        return recent_peaks_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_peaks_stats[1].GetCurrentValue();
    }
}

float SupplementalAlgorithms::get_current_trough_mean ()
{
    if (current_troughs_position < REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        return recent_troughs_stats[0].GetCurrentValue();
    }
    else
    {
        return recent_troughs_stats[1].GetCurrentValue();
    }
}

void SupplementalAlgorithms::add_peak (uint32_t sample)
{
    //Save the sample as a peak in the "recent_signal_peaks" array
    recent_signal_peaks[current_peaks_position] = sample;

    //Update the peak stats
    recent_peaks_stats[0].UpdateCalculation(sample);
    recent_peaks_stats[1].UpdateCalculation(sample);

    //Update the "current position" in the ring-buffers
    current_peaks_position++;
    if (current_peaks_position >= REWIRE_SUPP_ALGO_PEAKS_LENGTH)
    {
        current_peaks_position = 0;
        recent_peaks_stats[1].ResetCalculation();
    }
    else if (current_peaks_position == REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        recent_peaks_stats[0].ResetCalculation();
    }    
}

void SupplementalAlgorithms::add_trough (uint32_t sample)
{
    //Save the sample as a peak in the "recent_signal_peaks" array
    recent_signal_troughs[current_troughs_position] = sample;

    //Update the peak stats
    recent_troughs_stats[0].UpdateCalculation(sample);
    recent_troughs_stats[1].UpdateCalculation(sample);

    //Update the "current position" in the ring-buffers
    current_troughs_position++;
    if (current_troughs_position >= REWIRE_SUPP_ALGO_PEAKS_LENGTH)
    {
        current_troughs_position = 0;
        recent_troughs_stats[1].ResetCalculation();
    }
    else if (current_troughs_position == REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF)
    {
        recent_troughs_stats[0].ResetCalculation();
    }    
}

void SupplementalAlgorithms::update_peaks_and_troughs (uint32_t sample)
{
    //Update the peaks/troughs
    if (is_current_segment_positive)
    {
        //If the new sample is less than the signal mean
        if (sample < recent_signal_mean[current_signal_position])
        {
            //This segment is completed

            //Set the new segment to be a negative segment, leading towards a trough
            is_current_segment_positive = false;

            //Check to see if the current segment peak is far enough away from the mean to be counted
            //as a legitimate peak
            float current_stddev = get_current_stddev();
            if (current_stddev != 0)
            {
                float z_score = (current_segment_peak_or_trough - recent_signal_mean[current_signal_position]) / current_stddev;
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
        if (sample > recent_signal_mean[current_signal_position])
        {
            //This segment is completed

            //Set the new segment to be a positive segment, leading towards a peak
            is_current_segment_positive = true;

            //Check to see if the current segment peak is far enough away from the mean to be counted
            //as a legitimate peak
            float current_stddev = get_current_stddev();
            if (current_stddev != 0)
            {
                float z_score = (current_segment_peak_or_trough - recent_signal_mean[current_signal_position]) / current_stddev;
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

void SupplementalAlgorithms::AddSample (uint32_t sample)
{
    //Place the new sample into the "recent_signal" ring-buffer
    recent_signal[current_signal_position] = sample;
    
    //Update our calculations for mean and standard deviation of the recent signal
    recent_signal_stats[0].UpdateCalculation(sample);
    recent_signal_stats[1].UpdateCalculation(sample);

    //Update the "recent_signal_mean" ring-buffer
    recent_signal_mean[current_signal_position] = get_current_mean();

    //Update the peaks and troughs
    update_peaks_and_troughs(sample);

    //Update the "current position" in the ring-buffers
    current_signal_position++;
    if (current_signal_position >= REWIRE_SUPP_ALGO_SIGNAL_LENGTH)
    {
        current_signal_position = 0;
        recent_signal_stats[1].ResetCalculation();
    }
    else if (current_signal_position == REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF)
    {
        recent_signal_stats[0].ResetCalculation();
    }
}

float SupplementalAlgorithms::CalculatePerfusionIndex ()
{
    //The following sources were used to determine the calculation for the perfusion index:
    //1. https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3417976/
    //2. https://www.analog.com/en/technical-articles/guidelines-for-spo2-measurement--maxim-integrated.html
    //3. https://dsp.stackexchange.com/questions/46615/calculate-spo%E2%82%82-value-from-raw-fingertip-plethysmography-ppg
    //4. https://www.ti.com/lit/an/slaa655/slaa655.pdf

    float dc_component = get_current_mean();
    float ac_component = get_current_peak_mean() - get_current_trough_mean();
    float pi = 0;
    if (dc_component != 0)
    {
        pi = (ac_component / dc_component) * 100.0f;    
    }
    
    return pi;
}


SupplementalAlgorithms supplemental_algorithms;
