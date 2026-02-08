# Changelog: ConvRev vs ConvolVer

A detailed comparison of the original `ConvRev` chugin and the new `ConvolVer` implementation.

## 1. Critical Safety Fixes
- **Normalization**: 
  - **ConvRev**: No normalization. Loading a standard IR resulted in massive gain (+60dB+), risking speaker damage.
  - **ConvolVer**: Implements **Automatic Peak Normalization** (defaulting to 0.5). Output level is always safe regardless of IR length.
- **Thread Safety**:
  - **ConvRev**: Spawns a new `std::thread` **every audio block** (~344 times/second). This causes massive CPU spikes and dropouts.
  - **ConvolVer**: Eliminates per-block thread spawning, running efficiently on the audio thread (or a persistent worker in future versions) without allocation glitches.

## 2. API & Usability
- **Loading IRs**:
  - **ConvRev**: Required a slow ChucK `for` loop to set every sample individually (`cr.coeff(i, val)`), crossing the C++/ChucK boundary thousands of times.
  - **ConvolVer**: Adds `load(float[] array)`, allowing instant loading of entire IRs from `SndBuf.samples()`.
- **Return Types**:
  - **ConvRev**: Buggy `float` return types (returning integers for float values).
  - **ConvolVer**: Fixed return types for gain and status values.

## 3. Performance
- **SIMD / Vectorization**:
  - **ConvRev**: MacOS makefile disabled SSE optimizations.
  - **ConvolVer**: Explicitly enables `FFTCONVOLVER_USE_SSE` and includes `sse2neon.h` for fully vectorized performance on both Intel and Apple Silicon Macs.

## 4. Stability
- **Buffer Management**:
  - **ConvRev**: Re-allocated buffers in `init()` without proper locking, causing potential crashes if called during playback.
  - **ConvolVer**: Uses safer state resets and buffer management.

## 5. New Features (v1.1)
- **Wet/Dry Control**: 
  - Added `.mix(float)` (0-1) for easy blending. 
  - Added `.wet(float)` and `.dry(float)` taking **dB** values for precise trimming.
- **Configurable Block Size**:
  - Added `.blockSize(int)` to optimize partitioned convolution for different IR lengths.
  - Fixes performance issues with very long IRs (> 5s) by allowing larger partition sizes (e.g., 4096 or 8192).

## Summary
`ConvolVer` is a production-ready rewrite of `ConvRev` that prioritizes safety (unity gain), efficiency (no allocation in audio path), and usability (array-based loading).
