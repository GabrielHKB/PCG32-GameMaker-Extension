# PCG32 RNG Extension for GameMaker

A deterministic random number generator for GameMaker based on the PCG32 algorithm.

This extension provides independent RNG streams, deterministic seeding, and the ability to save and restore RNG state, which makes it useful for procedural generation, reproducible simulations, roguelikes, and debugging.

The core RNG is implemented in C++ and exposed to GameMaker through a wrapper API.

This project was partly inspired by

# Features

- High-quality PCG32 pseudo-random number generator

- Independent RNG streams

- Deterministic seeds

- Save and restore RNG state

Lightweight GameMaker wrapper functions

Suitable for procedural generation, AI randomness, loot systems, etc.

# Installation

Download the repository or release package.

In GameMaker select:

Tools → Import Local Package

Import:

PCG32_GM.yymps

The extension and wrapper scripts will be added to your project automatically.

# Basic Usage

## Create a new RNG stream:

var rng = rng_create(12345, 1);

## Generate a random float:

var value = rng_float(rng);

## Generate a number in a range:

var damage = rng_range(rng, 10, 20);

## Generate a random integer:

var roll = rng_int(rng, 1, 6);

## Chance check:

if (rng_chance(rng, 0.25))
{
    show_debug_message("Success!");
}

## Destroy the RNG when finished:

rng_destroy(rng);


# RNG Streams

Each RNG stream is independent. This prevents randomness in one system from affecting another.

Example:

rng_world = rng_create(seed, 1);
rng_loot  = rng_create(seed, 2);
rng_ai    = rng_create(seed, 3);

This ensures that AI randomness (ie. attack selection) will not influence loot drops or world generation.

## Saving and Restoring RNG State

The extension allows you to capture the exact RNG state and restore it later.

## Capture the state:

var snap = rng_snapshot(rng);

## Restore it later:

rng_restore(rng, snap);

This is useful for:

- deterministic debugging

- replays

- save/load systems

- rollback systems

| Function | Description |
|----------|-------------|
| `rng_create(seed, stream)` | Create a new RNG stream |
| `rng_destroy(handle)` | Destroy an RNG stream |
| `rng_next(handle)` | Raw 32-bit random integer |
| `rng_float(handle)` | Random float in `[0,1)` |
| `rng_range(handle, min, max)` | Random float in range |
| `rng_int(handle, min, max)` | Random integer in range |
| `rng_chance(handle, probability)` | Probability check |
| `rng_snapshot(handle)` | Capture RNG state |
| `rng_restore(handle, snapshot)` | Restore RNG state |
