#pragma once
#include <cstdint>

#ifdef _WIN32
#define GM_EXPORT extern "C" __declspec(dllexport)
#else
#define GM_EXPORT extern "C"
#endif


// Creation / Destruction

GM_EXPORT double pcg32_create(double seed, double stream);
GM_EXPORT double pcg32_next_u32(double handle);
GM_EXPORT double pcg32_destroy(double handle);

// Get State

GM_EXPORT double pcg32_get_state_hi(double handle);
GM_EXPORT double pcg32_get_state_lo(double handle);


// Get Increment (Stream)

GM_EXPORT double pcg32_get_inc_hi(double handle);
GM_EXPORT double pcg32_get_inc_lo(double handle);

// Set Full State

GM_EXPORT double pcg32_set_state(
    double handle,
    double state_hi, double state_lo,
    double inc_hi, double inc_lo
);