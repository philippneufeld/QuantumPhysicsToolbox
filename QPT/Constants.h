// Philipp Neufeld, 2023

#ifndef QPT_CONSTANTS_H_
#define QPT_CONSTANTS_H_

// Includes
#include "Platform.h"

namespace QPT {

// General constants
constexpr static double Ln2_v = 0.69314718056;
constexpr static double Pi_v = 3.14159265358979323846;
constexpr static double TwoPi_v = 2 * Pi_v;

// Physical constants
constexpr static double SpeedOfLight_v = 2.99792458e8;              // m / s
constexpr static double BoltzmannConstant_v = 1.38064852e-23;       // J / K
constexpr static double VacuumPermittivity_v = 8.8541878128e-12;    // F / m
constexpr static double PlanckConstant_v = 6.62607004e-34;          // J s
constexpr static double ReducedPlanckConstant_v = 1.054571817e-34;  // J * s
constexpr static double AtomicMassUnit_v = 1.66053906660e-27;       // kg
constexpr static double ElementaryCharge_v = 1.602176462e-19;       // C
constexpr static double ElectronMass_v = 9.1093837015e-31;          // kg
constexpr static double BohrRadius_v = 0.5291772083e-10;            // m
constexpr static double Debye_v = 3.335640952e-30;                  // C*m
constexpr static double RydbergConstant_v = 10973731.568;           // m^-1
constexpr static double RydbergEnergy_v = 2.17987236110353e-18;     // J
constexpr static double FineStructureConstant_v = 0.0072973525693;  // 1

// Unit conversions
constexpr static double EnergyInverseCm_v =
    100 * PlanckConstant_v * SpeedOfLight_v;                   // cm^-1 -> J
constexpr static double EnergyGHz_v = 1e9 * PlanckConstant_v;  // cm^-1 -> J

}  // namespace QPT

#endif  // !QPT_CONSTANTS_H_
