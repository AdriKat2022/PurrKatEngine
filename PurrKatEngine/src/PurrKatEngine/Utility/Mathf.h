#pragma once

#define MATHF_CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define MATHF_CLAMP_01(x) ((x) < 0 ? 0 : ((x) > 1 ? 1 : (x)))

class Mathf
{
public:
    
};
