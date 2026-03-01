#include "pcg32.h"
#include <unordered_map>
#include <cstdint>

struct PCG32 {
    uint64_t state;
	uint64_t increment; //must be odd
};

// helper functions to split/join 64-bit state into two 32-bit parts for GameMaker

static inline uint32_t lo32(uint64_t x) {
    return static_cast<uint32_t>(x & 0xFFFFFFFFULL);
}

static inline uint32_t hi32(uint64_t x) {
    return static_cast<uint32_t>(x >> 32);
}

static inline uint64_t make_u64(uint32_t hi, uint32_t lo) {
    return (static_cast<uint64_t>(hi) << 32) | static_cast<uint64_t>(lo);
}


static std::unordered_map<int, PCG32> rngs;

static int next_handle = 1;

static uint32_t pcg32_step(PCG32& rng) {
    uint64_t oldstate = rng.state;
    rng.state = oldstate * 6364136223846793005ULL + rng.increment;

    uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);

    // rotate right by rot (0..31)
    return (xorshifted >> rot) | (xorshifted << ((-static_cast<int32_t>(rot)) & 31));
}

// create rng stream, return handle as double for GameMaker
GM_EXPORT double pcg32_create(double seed, double stream) {
    uint64_t seed_u = static_cast<uint64_t>(seed);
    uint64_t stream_u = static_cast<uint64_t>(stream);

    PCG32 rng{};
    rng.state = 0;
    rng.increment = (stream_u << 1u) | 1u; // must be odd

    // seeding sequence
    pcg32_step(rng);
    rng.state += seed_u;
    pcg32_step(rng);

    int handle = next_handle++;
    rngs[handle] = rng;
    return static_cast<double>(handle);
}

// return next u32 from stream
GM_EXPORT double pcg32_next_u32(double handle) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    return static_cast<double>(pcg32_step(it->second));
}

// destroy rng stream, or return 0 if not found
GM_EXPORT double pcg32_destroy(double handle) {
    int h = static_cast<int>(handle);
    rngs.erase(h);
    return 0.0;
}

// get current state as two doubles (lo and hi 32 bits), or 0 if not found
GM_EXPORT double pcg32_get_state_lo(double handle) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    return static_cast<double>(lo32(it->second.state));
}

GM_EXPORT double pcg32_get_state_hi(double handle) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    return static_cast<double>(hi32(it->second.state));
}

// get increment as two doubles (lo and hi 32 bits), or 0 if not found
GM_EXPORT double pcg32_get_inc_lo(double handle) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    return static_cast<double>(lo32(it->second.increment));
}

GM_EXPORT double pcg32_get_inc_hi(double handle) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    return static_cast<double>(hi32(it->second.increment));
}

// set state and increment from two doubles each (lo and hi 32 bits), return 1 on success or 0 if not found/invalid
GM_EXPORT double pcg32_set_state(
    double handle,
    double state_hi, double state_lo,
    double inc_hi, double inc_lo
) {
    int h = static_cast<int>(handle);
    auto it = rngs.find(h);
    if (it == rngs.end()) return 0.0;

    uint64_t new_state = make_u64(
        static_cast<uint32_t>(state_hi),
        static_cast<uint32_t>(state_lo)
    );

    uint64_t new_inc = make_u64(
        static_cast<uint32_t>(inc_hi),
        static_cast<uint32_t>(inc_lo)
    );

	// increment must remain odd for PCG32 to work correctly
    if ((new_inc & 1ULL) == 0ULL) return 0.0;

    it->second.state = new_state;
    it->second.increment = new_inc;

    return 1.0;
}