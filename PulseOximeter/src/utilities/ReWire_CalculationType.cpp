#include "ReWire_CalculationType.h"

namespace ReWire_CalculationType_Converter
{
    const char *CalculationTypeNames[] = {
        "Calculation_NewestValue",
        "Calculation_OldestValue",    
        "Calculation_Min",
        "Calculation_Max",    
        "Calculation_Mean",
        "Calculation_SquaredDistanceFromMean",
        "Calculation_Variance",
        "Calculation_StdDev",
        "Calculation_Median"
    };

    const char *ConvertCalculationTypeToName (ReWire_CalculationType calc_type)
    {
        return CalculationTypeNames[calc_type];
    }
};