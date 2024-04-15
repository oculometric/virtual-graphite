#pragma once

#include "vector3.h"
struct OLPalette;

/**
 * get the filter value for a given filter size. the returned value is in the 0-1 range
 * 
 * @param x horizontal offset within the filter, does not need to be limited to the filter size
 * @param y vertical offset within the filter, does not need to be limited to the filter size
 * @param filter_size size of the kernel to sample from
 * 
 * @returns float in 0-1 range
 * **/
float orderedFilterThreshold(int x, int y, unsigned int filter_size = 4);

/**
 * sample the filter value from a custom filter provided. the filter should be an 
 * array of length `filter_size * filter_size` of unsigned integers ranging from 
 * zero to `(filter_size * filter_size) - 1` inclusive
 * 
 * @param x horizontal offset within the filter, does not need to be limited to the filter size
 * @param y vertical offset within the filter, does not need to be limited to the filter size
 * @param filter_size width and height of the filter
 * 
 * @returns float in 0-1 range
 * **/
float orderedFilterThresholdCustom(int x, int y, unsigned int filter_size, unsigned int* filter);

/**
 * construct a filter using the threshold map algorithm to generate a Bayer matrix for 
 * ordered dithering, which can be passed into `orderedFilterThresholdCustom`
 * 
 * @param filter_size width and height of the filter. must be a power of two
 * 
 * @returns pointer to newly allocated array of size `filter_size * filter_size`, or
 * NULL if the `filter_size` was not a power of two
 * **/
unsigned int* generateOrderedFilter(unsigned int filter_size);

/**
 * dither a single floating point value on a given pixel offset. `f` is dithered between the two
 * closest subdivisions, according to a given filter pattern and offset. supported filter sizes are 2, 4, 8, 16
 * 
 * @param f float to be dithered, in 0-1 range
 * @param subdivisions number of discrete possible values in the output
 * @param x horizontal position of the pixel, does not need to be limited to the filter size
 * @param y vertical position of the pixel, does not need to be limited to the filter size
 * @param filter_size size of the kernel used to dither the float. larger kernels have a greater range of effective values
 * 
 * @returns float rounded to the nearest subdivision, with dithering applied
 * **/
float orderedDitherFloat(float f, unsigned int subdivisions, int x, int y, unsigned int filter_size = 4);


OLVector3f orderedDitherPalette(const OLVector3f& colour, const OLPalette& palette, int x, int y);