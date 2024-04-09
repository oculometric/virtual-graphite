#pragma once

struct OLVector3f; struct OLPalette;

float orderedFilterThreshold(int x, int y, unsigned int filter_size = 4);
float orderedDitherFloat(float f, unsigned int subdivisions, int x, int y, unsigned int filter_size = 4);
OLVector3f orderedDitherPalette(const OLVector3f& colour, const OLPalette& palette, int x, int y);