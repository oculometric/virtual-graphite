#pragma once

#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"
#include "buffer.h"

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
	size_t* face_corners;
	OLVector3f* vertices;
	OLVector2f* texture_coordinates;
	OLVector3f* corner_normals;

	OLVector3f* face_normals;
	OLVector3f* edge_vectors;
	float* edge_dots;
	float* inv_denominators;

	size_t num_face_corners;
	size_t num_vertices;

	void updateSecondaryData();
	void deallocateBuffers();
public:
	OLMesh(const char* filename);
	OLMesh(const OLMesh& other);
	
	OLMesh(const OLMesh&& other) = delete;
	void operator=(const OLMesh& other) = delete;
	void operator=(const OLMesh&& other) = delete;

	OLPointData& raycast(const OLVector3f& origin, const OLVector3f& direction, bool cull_backfaces, float near_clip, float far_clip);
	OLPointData& closestPoint(const OLVector3f& point);
	void writeDepthBuffer(OLBuffer<float>* buffer, OLDepthWrite mode);
	bool readFromFile(const char* filename);
	void applyTransform(const OLMatrix4f& transform);

	~OLMesh();
};