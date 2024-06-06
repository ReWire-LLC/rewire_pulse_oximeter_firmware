#ifndef __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H
#define __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H

#include <Arduino.h>
#include "ReWire_OnlineCalculator.h"

#define REWIRE_SUPP_ALGO_SIGNAL_LENGTH              512
#define REWIRE_SUPP_ALGO_SIGNAL_LENGTH_HALF         256
#define REWIRE_SUPP_ALGO_PEAKS_LENGTH               64
#define REWIRE_SUPP_ALGO_PEAKS_LENGTH_HALF          32

#define REWIRE_SUPP_ALGO_SAMPLING_RATE_FLOAT        100.0f
#define REWIRE_SUPP_ALGO_CONST_A                    -45.060f
#define REWIRE_SUPP_ALGO_CONST_B                    30.354f
#define REWIRE_SUPP_ALGO_CONST_C                    94.845f

class SupplementalAlgorithms
{
    public:

        SupplementalAlgorithms();

        void AddSample (uint32_t ir_sample, uint32_t red_sample);
        float CalculatePerfusionIndex ();
        float CalculateBeatsPerMinute ();
        float CalculateSpO2 ();

    private:

        float get_current_min ();
        float get_current_max ();
        float get_current_mean ();
        float get_current_stddev ();
        void update_peaks_and_troughs (uint32_t sample);
        void add_peak (uint32_t sample);
        void add_trough (uint32_t sample);
        float get_current_peak_mean ();
        float get_current_trough_mean ();
        float get_current_mean_peak_distance ();

        float get_current_red_max ();
        float get_current_red_min ();
        float get_current_red_mean ();

        bool is_current_segment_positive = true;
        uint32_t current_segment_peak_or_trough = 0;

        uint32_t recent_ir_signal[REWIRE_SUPP_ALGO_SIGNAL_LENGTH];
        uint32_t recent_ir_signal_mean[REWIRE_SUPP_ALGO_SIGNAL_LENGTH];
        uint32_t current_signal_position = 0;

        uint32_t recent_ir_signal_peaks[REWIRE_SUPP_ALGO_PEAKS_LENGTH];
        uint32_t recent_ir_signal_troughs[REWIRE_SUPP_ALGO_PEAKS_LENGTH];
        uint32_t current_peaks_position = 0;
        uint32_t current_troughs_position = 0;

        ReWire_OnlineCalculator recent_ir_signal_mean_stats[2];
        ReWire_OnlineCalculator recent_ir_signal_min_max_stats[2];
        ReWire_OnlineCalculator recent_ir_peaks_stats[2];
        ReWire_OnlineCalculator recent_ir_troughs_stats[2];

        ReWire_OnlineCalculator recent_ir_peak_distance_stats[2];
        uint32_t samples_since_last_peak = 0;

        ReWire_OnlineCalculator recent_red_maximum_stats[2];
        ReWire_OnlineCalculator recent_red_minimum_stats[2];
        ReWire_OnlineCalculator recent_red_mean_stats[2];
};

extern SupplementalAlgorithms supplemental_algorithms;

#endif /* __REWIRE_PULSE_OXIMETER_SUPPLEMTNAL_ALGORITHMS_H */
