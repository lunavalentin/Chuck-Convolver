// test_convolver_long.ck
// Test script for ConvolVer with long IRs and new controls (Mix, Wet/Dry dB)

ConvolVer conv => dac;

// 1. Set Block Size
conv.blockSize(8192);
<<< "Block Size set to:", 8192 >>>;

// 2. Load Source
SndBuf source => conv;
"CaveAuralization.wav" => source.read;
1 => source.loop;

// 3. Load Long IR
SndBuf irBuf => blackhole;
"IR_length18.wav" => irBuf.read;

if(irBuf.samples() == 0) {
    <<< "Error: Could not load IR file!" >>>;
    me.exit();
}

// Copy samples
float irData[irBuf.samples()];
for(0=>int i; i<irBuf.samples(); i++) irBuf.valueAt(i) => irData[i];

// Load into ConvolVer
conv.load(irData);
<<< "Long IR Loaded. Length:", irData.size() >>>;

// 4. Test Mix and Wet/Dry dB
<<< "Testing Mix and dB controls..." >>>;

// Case 1: Full Wet (Mix=1.0, Wet=0dB, Dry=0dB)
conv.mix(1.0);
conv.wet(0.0); // 0 dB
conv.dry(0.0); // 0 dB (ignored when mix=1)
<<< "Mix: 1.0 (Full Wet), Wet: 0dB" >>>;
2::second => now;

// Case 2: 50% Mix (Mix=0.5)
conv.mix(0.5);
<<< "Mix: 0.5 (Blend)" >>>;
2::second => now;

// Case 3: Dry Only (Mix=0.0)
conv.mix(0.0);
<<< "Mix: 0.0 (Dry Only)" >>>;
2::second => now;

// Case 4: Wet Trim (Mix=1.0, Wet=-6dB)
conv.mix(1.0);
conv.wet(-6.0);
<<< "Mix: 1.0, Wet: -6dB" >>>;
2::second => now;

// Case 5: Dry Trim (Mix=0.0, Dry=-6dB)
conv.mix(0.0);
conv.dry(-6.0);
<<< "Mix: 0.0, Dry: -6dB" >>>;
2::second => now;

// Reset
conv.mix(1.0);
conv.wet(0.0);
conv.dry(0.0);

// 5. Run for stability
<<< "Running for 5 seconds to check stability..." >>>;
5::second => now;

<<< "Test Complete." >>>;
