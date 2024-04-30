#pragma once

#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"
#include "buffer.h"
#include "rendering.h"

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

	OLPointData raycast(const OLVector3f& origin, const OLVector3f& direction, bool cull_backfaces, float near_clip, float far_clip);
	void drawToBuffers(OLBuffer<float>* depth_buffer, OLBuffer<size_t>* index_buffer, OLBuffer<OLVector4<unsigned char>>* bary_buffer, OLRenderConfig* render_config);
	OLPointData closestPoint(const OLVector3f& point);
	bool readFromFile(const char* filename);
	void applyTransform(const OLMatrix4f& transform);

	~OLMesh();
};