// Example: ConvolVer with Droplets
// IR: IR_length3.4_20250226_005613.wav

// 1. Setup ConvolVer
ConvolVer conv => dac;
conv.mix(0.8); // 20% Wet

// 2. Load IR
"IR_length3.4_20250226_005613.wav" => string irPath;
// Helper to load file into array
fun float[] getSamples(string path)
{
    SndBuf buf => blackhole;
    path => buf.read;
    if(buf.samples() == 0) {
        <<< "Error: could not load IR:", path >>>;
        return [0.0];
    }
    float arr[buf.samples()];
    for(0=>int i; i<buf.samples(); i++) buf.valueAt(i) => arr[i];
    return arr;
}

getSamples(irPath) @=> float ir[];
conv.load(ir);
<<< "IR Loaded:", ir.size(), "samples" >>>;

// 3. Setup Source (from Chukos-BeepPlocDrops.ck)
SinOsc d => ADSR e => conv; // Route to reverb
e.set(10::ms, 50::ms, 0.5, 20::ms);
0.2 => d.gain;

// 4. Loop for ~1 minute
now + 1::minute => time stopTime;

while(now < stopTime)
{
    Math.random2(500, 2000) => d.freq;
    e.keyOn();
    62::ms => now;
    e.keyOff();
    e.releaseTime() => now;
    
    // Randomize wait slightly
    Math.random2f(0.8, 1.2)::second => now;
}

<<< "Done." >>>;
