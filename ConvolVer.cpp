//-----------------------------------------------------------------------------
// ConvolVer: Simple Convolution Chugin
//-----------------------------------------------------------------------------

#include "chugin.h"
#include "FFTConvolver.h"
#include <vector>
#include <cmath>
#include <algorithm>

CK_DLL_CTOR(convolver_ctor);
CK_DLL_DTOR(convolver_dtor);
CK_DLL_TICK(convolver_tick);

CK_DLL_MFUN(convolver_load);
CK_DLL_MFUN(convolver_gain);
CK_DLL_MFUN(convolver_mix);
CK_DLL_MFUN(convolver_wet_db);
CK_DLL_MFUN(convolver_dry_db);
CK_DLL_MFUN(convolver_blockSize);
<<<<<<< HEAD
CK_DLL_MFUN(convolver_normalize);
=======
>>>>>>> origin/main

t_CKINT convolver_data_offset = 0;

class ConvolVer
{
private:
    float _SR;
    size_t _blocksize = 128; 
    fftconvolver::FFTConvolver _convolver;
    
    std::vector<float> _input_buffer;
    std::vector<float> _output_buffer;
    size_t _idx;
    
    float _gain;

    float _mix; // 0-1
    float _wet_db; // dB
    float _dry_db; // dB
    
    // Calculated Linear Values
    float _wet_linear;
    float _dry_linear;
    size_t _userBlockSize;

<<<<<<< HEAD
    bool _normalize;

public:
    ConvolVer(float fs) : _SR(fs), _idx(0), _gain(1.0f), _mix(1.0f), _wet_db(0.0f), _dry_db(0.0f), _userBlockSize(1024), _normalize(true)
=======
public:
    ConvolVer(float fs) : _SR(fs), _idx(0), _gain(1.0f), _mix(1.0f), _wet_db(0.0f), _dry_db(0.0f), _userBlockSize(1024)
>>>>>>> origin/main
    {
        updateLinearGains();
        _blocksize = _userBlockSize;
        _input_buffer.resize(_blocksize, 0.0f);
        _output_buffer.resize(_blocksize, 0.0f);
    }

    ~ConvolVer() {}

    int load(std::vector<float>& samples)
    {
        // Safety: Check for NaN/Inf
        for (float f : samples) {
            if (std::isnan(f) || std::isinf(f)) return 0;
        }
        
        // Safety: Peak Normalize to 0.3 (Conservative default)
        // Convolution adds a lot of energy. 
<<<<<<< HEAD
        if (_normalize) {
            float max_val = 0.0f;
            for (float f : samples) {
                if (std::abs(f) > max_val) max_val = std::abs(f);
            }

            if (max_val > 0.000001f) {
                float scale = 0.5f / max_val; 
                // Apply scale
                for (size_t i=0; i<samples.size(); ++i) samples[i] *= scale;
            }
=======
        float max_val = 0.0f;
        for (float f : samples) {
            if (std::abs(f) > max_val) max_val = std::abs(f);
        }

        if (max_val > 0.000001f) {
            float scale = 0.5f / max_val; 
            // Apply scale
            for (size_t i=0; i<samples.size(); ++i) samples[i] *= scale;
>>>>>>> origin/main
        }

        _convolver.reset();
        _blocksize = _userBlockSize;
        _input_buffer.resize(_blocksize, 0.0f);
        _output_buffer.resize(_blocksize, 0.0f);
        bool success = _convolver.init(_blocksize, samples.data(), samples.size());
        
        // Reset buffer state
        std::fill(_input_buffer.begin(), _input_buffer.end(), 0.0f);
        std::fill(_output_buffer.begin(), _output_buffer.end(), 0.0f);
        _idx = 0;

        return success ? 1 : 0;
    }

    void setGain(float g) { _gain = g; }
    float getGain() { return _gain; }

    // Helper to update linear gains
    void updateLinearGains() {
        // DB to Linear: pow(10, db/20)
        float w_lin = std::pow(10.0f, _wet_db / 20.0f);
        float d_lin = std::pow(10.0f, _dry_db / 20.0f);
        
        _wet_linear = w_lin * _mix;
        _dry_linear = d_lin * (1.0f - _mix);
    }

    void setMix(float m) { 
        _mix = m; 
        if(_mix < 0.0f) _mix = 0.0f;
        if(_mix > 1.0f) _mix = 1.0f;
        updateLinearGains();
    }
    float getMix() { return _mix; }
    
    void setWetDb(float db) { 
        _wet_db = db; 
        updateLinearGains();
    }
    float getWetDb() { return _wet_db; }
    
    void setDryDb(float db) { 
        _dry_db = db; 
        updateLinearGains();
    }
    float getDryDb() { return _dry_db; }

    int setBlockSize(int size) {
        if (size < 32) size = 32; // Minimum safety
        // Ensure power of 2
        int p2 = 1;
        while (p2 < size) p2 <<= 1;
        _userBlockSize = p2;
        return _userBlockSize;
    }
    int getBlockSize() { return (int)_userBlockSize; }

<<<<<<< HEAD
    void setNormalize(int n) { _normalize = (n != 0); }
    int getNormalize() { return _normalize ? 1 : 0; }

=======
>>>>>>> origin/main
    SAMPLE tick(SAMPLE in)
    {
        _input_buffer[_idx] = in;
        
        SAMPLE out = (_output_buffer[_idx] * _wet_linear) + (in * _dry_linear);
        out *= _gain;
        
        _idx++;
        if (_idx >= _blocksize) {
            _convolver.process(_input_buffer.data(), _output_buffer.data(), _blocksize);
            _idx = 0;
        }
        
        return out;
    }
};

CK_DLL_QUERY(ConvolVer)
{
    QUERY->setname(QUERY, "ConvolVer");
    QUERY->begin_class(QUERY, "ConvolVer", "UGen");
    
    QUERY->add_ctor(QUERY, convolver_ctor);
    QUERY->add_dtor(QUERY, convolver_dtor);
    QUERY->add_ugen_func(QUERY, convolver_tick, NULL, 1, 1);

    QUERY->add_mfun(QUERY, convolver_load, "int", "load");
    QUERY->add_arg(QUERY, "float[]", "ir");

    QUERY->add_mfun(QUERY, convolver_gain, "float", "gain");
    QUERY->add_arg(QUERY, "float", "val");

    QUERY->add_mfun(QUERY, convolver_mix, "float", "mix");
    QUERY->add_arg(QUERY, "float", "val");

    QUERY->add_mfun(QUERY, convolver_wet_db, "float", "wet");
    QUERY->add_arg(QUERY, "float", "db");

    QUERY->add_mfun(QUERY, convolver_dry_db, "float", "dry");
    QUERY->add_arg(QUERY, "float", "db");

    QUERY->add_mfun(QUERY, convolver_blockSize, "int", "blockSize");
    QUERY->add_arg(QUERY, "int", "val");

<<<<<<< HEAD
    QUERY->add_mfun(QUERY, convolver_normalize, "int", "normalize");
    QUERY->add_arg(QUERY, "int", "toggle");

=======
>>>>>>> origin/main
    convolver_data_offset = QUERY->add_mvar(QUERY, "int", "@c_data", false);
    QUERY->end_class(QUERY);
    return TRUE;
}

CK_DLL_CTOR(convolver_ctor)
{
    OBJ_MEMBER_INT(SELF, convolver_data_offset) = 0;
    ConvolVer *obj = new ConvolVer(API->vm->srate(VM));
    OBJ_MEMBER_INT(SELF, convolver_data_offset) = (t_CKINT)obj;
}

CK_DLL_DTOR(convolver_dtor)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    CK_SAFE_DELETE(obj);
    OBJ_MEMBER_INT(SELF, convolver_data_offset) = 0;
}

CK_DLL_TICK(convolver_tick)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    if(obj) *out = obj->tick(in);
    return TRUE;
}

CK_DLL_MFUN(convolver_load)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    Chuck_ArrayFloat *arr = (Chuck_ArrayFloat *)GET_NEXT_OBJECT(ARGS);
    if (!arr) { RETURN->v_int = 0; return; }
    
    int len = API->object->array_float_size(arr);
    std::vector<float> samps(len);
    for(int i=0; i<len; i++) samps[i] = API->object->array_float_get_idx(arr, i);
    
    RETURN->v_int = obj->load(samps);
}

CK_DLL_MFUN(convolver_gain)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    float g = GET_NEXT_FLOAT(ARGS);
    obj->setGain(g);
    RETURN->v_float = g;
}

CK_DLL_MFUN(convolver_mix)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    float v = GET_NEXT_FLOAT(ARGS);
    obj->setMix(v);
    RETURN->v_float = v;
}

CK_DLL_MFUN(convolver_wet_db)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    float v = GET_NEXT_FLOAT(ARGS);
    obj->setWetDb(v);
    RETURN->v_float = v;
}

CK_DLL_MFUN(convolver_dry_db)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    float v = GET_NEXT_FLOAT(ARGS);
    obj->setDryDb(v);
    RETURN->v_float = v;
}

CK_DLL_MFUN(convolver_blockSize)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    int v = GET_NEXT_INT(ARGS);
    RETURN->v_int = obj->setBlockSize(v);
}
<<<<<<< HEAD

CK_DLL_MFUN(convolver_normalize)
{
    ConvolVer *obj = (ConvolVer *)OBJ_MEMBER_INT(SELF, convolver_data_offset);
    int v = GET_NEXT_INT(ARGS);
    obj->setNormalize(v);
    RETURN->v_int = v;
}
=======
>>>>>>> origin/main
