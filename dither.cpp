#include "dither.h"

#include <math.h>

static const float filter_4_4[16] =
{
    2,  8,  1,  10,
    15,  7,  13,  5,
    0,  11,  3,  9,
    12,  4,  14,  6,
};

static const float filter_2_2[4] =
{
    0, 2,
    3, 1,
};

static const float filter_8_8_[64] =
{
    63, 31, 63, 31, 63, 31, 63, 31,
    31, 63, 31, 63, 31, 63, 31, 63,
    63, 31, 63, 31, 63, 31, 63, 31,
    31, 63, 31, 63, 31, 63, 31, 63,
    63, 31, 63, 31, 63, 31, 63, 31,
    31, 63, 31, 63, 31, 63, 31, 63,
    63, 31, 63, 31, 63, 31, 63, 31,
    31, 63, 31, 63, 31, 63, 31, 63,
};

static const float filter_8_8[64] =
{
    51, 15, 63, 23, 55,  3, 59, 31,
    19, 43,  7, 39, 27, 47, 11, 35,
    59, 31, 55, 15, 63, 23, 51,  3,
    11, 35, 19, 47,  7, 39, 27, 43,
    55,  3, 59, 31, 51, 15, 63, 23,
    27, 47, 11, 35, 19, 43,  7, 39,
    63, 23, 51,  3, 59, 31, 55, 15,
     7, 39, 27, 43, 11, 35, 19, 47,
};

float orderedFilterThreshold(int x, int y, unsigned int filter_size)
{
    switch (filter_size)
    {
    case 2:
        return filter_2_2[((y % 2) * 2) + (x % 2)] / 4;
    case 4:
        return filter_4_4[((y % 4) * 4) + (x % 4)] / 16;
    case 8:
        return filter_8_8[((y % 8) * 8) + (x % 8)] / 64;
    }
    return 0.0f;
}

float orderedDitherFloat(float f, unsigned int subdivisions, int x, int y, unsigned int filter_size)
{
    float scaled = f * subdivisions;
    float low = floor(scaled);

    return (low + ((scaled - low) > orderedFilterThreshold(x, y, filter_size))) / subdivisions;
}
