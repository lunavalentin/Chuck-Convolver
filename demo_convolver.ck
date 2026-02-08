// demo_convolver.ck
// - demonstrates loading IR
// - demonstrates setting block size for long IRs
// - demonstrates mix (0-1) and wet/dry (dB)

ConvolVer conv => dac;

// 1. Set Block Size
// For longer IRs (e.g. > 5s), use a larger block size (e.g. 4096 or 8192)
// This must be done BEFORE loading the IR (or reload after)
conv.blockSize(4096);

// 2. Load Source
SndBuf source => conv;
"CaveAuralization.wav" => source.read;
1 => source.loop;

// 3. Load IR
SndBuf irBuf => blackhole;
"IR_length4.wav" => irBuf.read;

// Copy samples
float irData[irBuf.samples()];
for(0=>int i; i<irBuf.samples(); i++) irBuf.valueAt(i) => irData[i];

// Load into ConvolVer
conv.load(irData);
<<< "IR Loaded. Length:", irData.size() >>>;

// 4. Mix
conv.gain(0.8);

// Full Wet
conv.mix(1.0);
conv.wet(-3.0); // -3 dB trim
conv.dry(-96.0); // silence

<<< "Playing... Mix=1.0, Wet=-3dB" >>>;
eon => now;
