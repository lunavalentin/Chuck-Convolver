# ConvolVer

A simple, efficient convolution Chugin for ChucK, based on `FFTConvolver`.

## Descriptions

ConvolVer performs partitioned FFT convolution, allowing for low-latency convolution of audio signals with impulse responses (IRs). It is suitable for real-time applications such as reverb, cabinet simulation, and more.

## Usage

```chuck
// Instantiate
ConvolVer conv => dac;

// Load an impulse response (array of floats)
[1.0, 0.5, 0.25] @=> float ir[];
conv.load(ir);

// Controls
conv.gain(1.0);       // Master output gain
conv.mix(0.5);        // 0.0 = Dry, 1.0 = Wet
conv.wet(0.0);        // Wet level in dB
conv.dry(0.0);        // Dry level in dB
conv.blockSize(256);  // Internal FFT block size (power of 2, min 32)
conv.normalize(1);    // 1 = Enable peak normalization (default), 0 = Disable
```

### API

*   `int load(float[] ir)`: Loads an impulse response. Returns 1 on success, 0 on failure.
*   `float gain(float val)`: Sets the master output gain.
*   `float mix(float val)`: Sets the mix between dry and wet signals (0.0 - 1.0).
*   `float wet(float db)`: Sets the wet signal level in dB.
*   `float dry(float db)`: Sets the dry signal level in dB.
*   `int blockSize(int val)`: Sets the internal block size for partitioned convolution. Must be a power of 2 >= 32. Smaller blocks = lower latency but higher CPU usage.
*   `int normalize(int toggle)`: Enables (1) or disables (0) automatic peak normalization of the IR upon loading. Default is enabled (1).

## Build Instructions

### macOS
```bash
make osx
```

### Linux
```bash
make linux
```

### Windows
Open `ConvolVer.vcxproj` in Visual Studio and build.
Alternatively, use MSBuild:
```powershell
MSBuild ConvolVer.vcxproj /p:Configuration=Release
```

## Build & Run Example

```bash
# Install dependency if missing
brew install sse2neon

# Build (macOS)
make osx

# Run the shipped example (uses the produced ConvolVer.chug)
chuck --chugin:ConvolVer.chug example_convolver_drops.ck
```

## Troubleshooting

- **Missing `sse2neon.h` during build**: install via Homebrew with `brew install sse2neon` and ensure `/opt/homebrew/include` is available to the compiler (the included `makefile.mac` adds `-I/opt/homebrew/include` when needed).
- **Which file to pass to `chuck`**: the built artifact is `ConvolVer.chug` — use `--chugin:ConvolVer.chug` when running examples.
- **Targeting a specific architecture**: edit `ARCHS` in `makefile.mac` (default builds a universal binary for `x86_64` and `arm64`).

## License

MIT License. See source files for details.
