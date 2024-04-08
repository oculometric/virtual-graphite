#pragma once

#include "matrix4.h"
#include "vector3.h"
#include "object.h"

class OLCamera : public OLObject
{
private:

public:
	static OLMatrix4f projectionMatrix(float near_clip_plane, float far_clip_plane, float horizontal_fov_degrees, float aspect_ratio);
};