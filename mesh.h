#pragma once

#include "vector2.h"
#include "vector3.h"

struct OLPointData
{
	OLVector3f position;
	float depth;
	OLVector2f uv;
	OLVector3f normal;
};

class OLMesh
{
private:
	size_t face_corners;
	OLVector3f* vertices;
	OLVector2f* texture_coordinates;
	OLVector3f* corner_normals;

	OLVector3f* face_normals;
	OLVector3f* ab_vectors;
	OLVector3f* ac_vectors;
	float* inv_determinants;

	size_t num_face_corners;
	size_t num_vertices;
public:
	OLMesh(const char* filename);

	OLPointData& raycast(const OLVector3f& origin, const OLVector3f& direction);
	OLPointData& closestPoint(const OLVector3f& point);

	~OLMesh();
};