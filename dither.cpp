#include "dither.h"

#include <math.h>
#include <memory>

static const unsigned int filter_2_2[4] =
{
    0, 2,
    3, 1,
};

static const unsigned int filter_4_4[16] =
{
     0,  8,    2, 10,
    12,  4,   14,  6,

     3, 11,    1,  9,
    15,  7,   13,  5,
};

static const unsigned int filter_8_8[64] =
{
     0, 32,  8, 40,    2, 34, 10, 42,
    48, 16, 56, 24,   50, 18, 58, 26,
    12, 44,  4, 36,   14, 46,  6, 38,
    60, 28, 52, 20,   62, 30, 54, 22,

     3, 35, 11, 43,    1, 33,  9, 41,
    51, 19, 59, 27,   49, 17, 57, 25,
    15, 47,  7, 39,   13, 45,  5, 37,
    63, 31, 55, 23,   61, 29, 53, 21,
};

static const unsigned int filter_16_16[256] =
{
      0, 128,  32, 160,   8, 136,  40, 168,     2, 130,  34, 162,  10, 138,  42, 170,
    192,  64, 224,  96, 200,  72, 232, 104,   194,  66, 226,  98, 202,  74, 234, 106,
     48, 176,  16, 144,  56, 184,  24, 152,    50, 178,  18, 146,  58, 186,  26, 154,
    240, 112, 208,  80, 248, 120, 216,  88,   242, 114, 210,  82, 250, 122, 218,  90,
     12, 140,  44, 172,   4, 132,  36, 164,    14, 142,  46, 174,   6, 134,  38, 166,
    204,  76, 236, 108, 196,  68, 228, 100,   206,  78, 238, 110, 198,  70, 230, 102,
     60, 188,  28, 156,  52, 180,  20, 148,    62, 190,  30, 158,  54, 182,  22, 150,
    252, 124, 220,  92, 244, 116, 212,  84,   254, 126, 222,  94, 246, 118, 214,  86,

      3, 131,  35, 163,  11, 139,  43, 171,     1, 129,  33, 161,   9, 137,  41, 169,
    195,  67, 227,  99, 203,  75, 235, 107,   193,  65, 225,  97, 201,  73, 233, 105,
     51, 179,  19, 147,  59, 187,  27, 155,    49, 177,  17, 145,  57, 185,  25, 153,
    243, 115, 211,  83, 251, 123, 219,  91,   241, 113, 209,  81, 249, 121, 217,  89,
     15, 143,  47, 175,   7, 135,  39, 167,    13, 141,  45, 173,   5, 133,  37, 165,
    207,  79, 239, 111, 199,  71, 231, 103,   205,  77, 237, 109, 197,  69, 229, 101,
     63, 191,  31, 159,  55, 183,  23, 151,    61, 189,  29, 157,  53, 181,  21, 149,
    255, 127, 223,  95, 247, 119, 215,  87,   253, 125, 221,  93, 245, 117, 213,  85,
};

float orderedFilterThreshold(int x, int y, unsigned int filter_size)
{
    switch (filter_size)
    {
    case 2:
        return (float)(filter_2_2[((y % 2) * 2) + (x % 2)]) / 4.0f;
    case 4:
        return (float)(filter_4_4[((y % 4) * 4) + (x % 4)]) / 16.0f;
    case 8:
        return (float)(filter_8_8[((y % 8) * 8) + (x % 8)]) / 64.0f;
    case 16:
        return (float)(filter_16_16[((y % 16) * 16) + (x % 16)]) / 256.0f;
    }
    return 0.0f;
}

float orderedFilterThresholdCustom(int x, int y, unsigned int filter_size, unsigned int* filter)
{
    if (!filter) return 0.0f;
    return (float)(filter[((y % filter_size) * filter_size) + (x % filter_size)]) / (float)(filter_size * filter_size);
}

unsigned int* generateOrderedFilter(unsigned int filter_size)
{
    if (fmod(log2(filter_size), 1.0) > 0.0) return NULL;
    if (filter_size < 2) return NULL;
    if (filter_size > sqrt(UINT_MAX)) return NULL;

    unsigned int length = filter_size * filter_size;
    unsigned int* map = new unsigned int[length];

    map[0] = 0;

    unsigned int old_dimension = 1;
    unsigned int old_length = old_dimension * old_dimension;
    for (unsigned int dimension = 2; dimension <= filter_size; dimension *= 2)
    {
        // quadruple all previous values
        for (unsigned int index = 0; index < old_length; index++)
            map[index] *= 4;

        // spread out rows
        for (unsigned int row = old_dimension - 1; row > 0; row--)
        {
            memcpy(map + (row * dimension), map + (row * old_dimension), (sizeof(unsigned int) / sizeof(uint8_t)) * old_dimension);
        }

        // duplicate data
        unsigned int index_2 = 0 + old_dimension; // index into the top right block
        unsigned int index_3 = 0 + (old_length * 2); // index into the bottom left block
        unsigned int index_1 = 0 + (old_length * 2) + old_dimension; // index into the bottom right block
        for (unsigned int index_0 = 0; index_0 < old_length * 2; index_0++)
        {
            // offset values in different quadrants
            map[index_2] = map[index_0] + 2;
            map[index_3] = map[index_0] + 3;
            map[index_1] = map[index_0] + 1;

            if ((index_0 + 1) % old_dimension == 0)
            {
                index_0 += old_dimension;
                index_2 += old_dimension;
                index_3 += old_dimension;
                index_1 += old_dimension;
            }
            index_2++;
            index_3++;
            index_1++;
        }

        old_dimension *= 2;
        old_length *= 4;
    }

    return map;
}

float orderedDitherFloat(float f, unsigned int subdivisions, int x, int y, unsigned int filter_size)
{
    float scaled = f * (subdivisions - 1);
    float low = floor(scaled);

    return (low + ((scaled - low) > orderedFilterThreshold(x, y, filter_size))) / (subdivisions - 1);
}
