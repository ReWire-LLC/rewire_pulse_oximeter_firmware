#ifndef __REWIRE_CALCULATION_TYPE_H
#define __REWIRE_CALCULATION_TYPE_H

enum ReWire_CalculationType
{
    Calculation_NewestValue,
    Calculation_OldestValue,    
    Calculation_Min,
    Calculation_Max,    
    Calculation_Mean,
    Calculation_SquaredDistanceFromMean,
    Calculation_Variance,
    Calculation_StdDev,
    Calculation_Median
};

namespace ReWire_CalculationType_Converter
{
    extern const char *CalculationTypeNames[];

    const char *ConvertCalculationTypeToName (ReWire_CalculationType calc_type);
};

#endif /* __REWIRE_CALCULATION_TYPE_H */
