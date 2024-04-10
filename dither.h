#pragma once

struct OLVector3f; struct OLPalette;

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