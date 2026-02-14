// test_convolver.ck
// Basic test script for ConvolVer 
// - loads IR
// - tests gain
// - tests mix (0-1) and wet/dry (dB)
// - tests block size configuration

ConvolVer conv => dac;

// 1. Configure
conv.blockSize(2048); 
<<< "Block size set to 2048" >>>;

// 2. Load source
SndBuf source => conv;
"CaveAuralization.wav" => source.read;
1 => source.loop;

// 3. Load IR
SndBuf irBuf => blackhole;
"IR_length2.wav" => irBuf.read;

// Copy samples
float irData[irBuf.samples()];
for(0=>int i; i<irBuf.samples(); i++) irBuf.valueAt(i) => irData[i];

// Load into ConvolVer
conv.load(irData);
<<< "IR Loaded. Length:", irData.size() >>>;

// 4. Test Controls
conv.gain(0.5);
<<< "Gain set to 0.5" >>>;

// Mix = 1.0 (Wet)
conv.mix(1.0);
conv.wet(0.0);
<<< "Mix: 1.0 (Full Wet)" >>>;
10::second => now;

// Mix = 0.5 (Blend)
conv.mix(0.5);
<<< "Mix: 0.5 (Blend)" >>>;
10::second => now;

// Mix = 0.0 (Dry)
conv.mix(0.0);
<<< "Mix: 0.0 (Dry Only)" >>>;
10::second => now;

<<< "Done" >>>;
