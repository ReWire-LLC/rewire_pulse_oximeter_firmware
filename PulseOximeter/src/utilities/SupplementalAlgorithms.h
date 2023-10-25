#ifndef __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H
#define __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H

#include <Arduino.h>
#include "ReWire_OnlineCalculator.h"

#define REWIRE_SUPP_ALGO_SIGNAL_LENGTH              512
#define REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF         256
#define REWIRE_SUPP_ALGO_PEAKS_LENGTH               64
#define REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF          32

class SupplementalAlgorithms
{
    public:

        SupplementalAlgorithms();

        void AddSample (uint32_t sample);
        float CalculatePerfusionIndex ();

    private:

        float get_current_mean ();
        float get_current_stddev ();
        void update_peaks_and_troughs (uint32_t sample);
        void add_peak (uint32_t sample);
        void add_trough (uint32_t sample);
        float get_current_peak_mean ();
        float get_current_trough_mean ();

        bool is_current_segment_positive = true;
        uint32_t current_segment_peak_or_trough = 0;

        uint32_t recent_signal[REWIRE_SUPP_ALGO_SIGNAL_LENGTH];
        uint32_t recent_signal_mean[REWIRE_SUPP_ALGO_SIGNAL_LENGTH];
        uint32_t current_signal_position = 0;

        uint32_t recent_signal_peaks[REWIRE_SUPP_ALGO_PEAKS_LENGTH];
        uint32_t recent_signal_troughs[REWIRE_SUPP_ALGO_PEAKS_LENGTH];
        uint32_t current_peaks_position = 0;
        uint32_t current_troughs_position = 0;

        ReWire_OnlineCalculator recent_signal_stats[2];
        ReWire_OnlineCalculator recent_peaks_stats[2];
        ReWire_OnlineCalculator recent_troughs_stats[2];
};

extern SupplementalAlgorithms supplemental_algorithms;

#endif /* __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H */
