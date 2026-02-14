# ConvolVer

**Type**: `UGen` | **Output**: Mono | **Input**: Mono

A high-performance, real-time FFT convolution engine for ChucK. Designed for efficiency and safety, `ConvolVer` performs partitioned convolution with automatic peak normalization to ensure unity gain.

## Features
- **Zero-Latency Feel**: Uses partitioned convolution (typical delay ~128 samples).
- **Safety**: Automatic **Peak Normalization** (defaults to 0.5 peak) on load to prevent dangerous volume spikes.
- **Efficiency**: SSE-optimized processing (via `sse2neon` on Apple Silicon).
- **Simplicity**: Single-method loading directly from float arrays.

## API Reference

### `int load( float[] ir )`
Loads an Impulse Response (IR) from a float array.
- **Parameters**: 
  - `ir`: An array of floats representing the impulse response audio data.
- **Returns**: `1` on success, `0` on failure.
- **Behavior**: 
  - The IR is automatically scanned for its peak amplitude.
  - It is normalized such that the peak amplitude is `0.5` (preserving -6dB headroom).
  - Internal buffers are reset (audio tail is cut).

### `float gain( float val )`
Sets the post-convolution output gain.
- **Parameters**:
  - `val`: Linear gain factor (default `1.0`).
- **Returns**: The new gain value.
- **Note**: This gain is applied *after* the internal normalization.

### `float gain()`
Gets the current output gain.
- **Returns**: Current linear gain.

### `int blockSize( int size )`
Sets the internal block size (partition size) for the convolution.
- **Parameters**:
  - `size`: The desired block size. Must be a power of 2 (e.g., 256, 512, 1024). Minimum is 32.
- **Returns**: The actual block size set (rounded up to nearest power of 2).
- **Note**: Larger block sizes (e.g., 4096, 8192) are recommended for very long IRs (e.g., > 10s) to improve performance. Default is 1024. This must be set *before* calling `load()` or you must reload the IR for it to take effect.

### `float mix( float val )`
Sets the dry/wet mix.
- **Parameters**:
  - `val`: Mix value (0.0 = Dry, 1.0 = Wet).
- **Returns**: The new mix value.

### `float wet( float db )`
Sets the wet signal trim in Decibels (dB). 
- **Parameters**:
  - `db`: Gain in dB (e.g., -3.0). 
- **Returns**: The new wet dB value.

### `float dry( float db )`
Sets the dry signal trim in Decibels (dB). 
- **Parameters**:
  - `db`: Gain in dB (e.g., -3.0). 
- **Returns**: The new dry dB value.

## Usage Example

```chuck
// 1. Instantiate
ConvolVer conv => dac;

// 2. Helper to load file into array
fun float[] getSamples(string path)
{
    SndBuf buf => blackhole;
    path => buf.read;
    float arr[buf.samples()];
    for(0=>int i; i<buf.samples(); i++) buf.valueAt(i) => arr[i];
    return arr;
}

// 3. Load IR
getSamples("my_reverb.wav") @=> float ir[];
conv.load(ir);

// 4. Play Source
SndBuf source => conv;
"drum_loop.wav" => source.read;
1 => source.loop;

// Mix
conv.mix(0.5);
conv.wet(-6.0); // Trim wet by 6dB
conv.dry(0.0);  // Dry at unity
 

while(true) 1::second => now;
```

## Compilation
>>>>>>> origin/main

### macOS
```bash
make osx
<<<<<<< HEAD
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
=======
sudo make install
```
### Linux
```bash
make linux
sudo make install
```
=======
# Chuck-Convolver
>>>>>>> 779fe3a7f942c29891b4a6f5afbf7bfbba33e473
>>>>>>> origin/main
