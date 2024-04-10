#include "mesh.h"

#include <fstream>
#include <iostream>
#include <string>

struct OLFaceCornerInfo { uint32_t vert; uint32_t uv; uint32_t vn; };

OLFaceCornerInfo splitFaceCorner(std::string str)
{
    OLFaceCornerInfo fci = { 0,0,0 };
    size_t first_break_ind = str.find('/');
    if (first_break_ind == std::string::npos) return fci;
    fci.vert = stoi(str.substr(0, first_break_ind)) - 1;
    size_t second_break_ind = str.find('/', first_break_ind + 1);
    if (second_break_ind != first_break_ind + 1)
        fci.uv = stoi(str.substr(first_break_ind + 1, second_break_ind - first_break_ind)) - 1;
    fci.vn = stoi(str.substr(second_break_ind + 1, str.find('/', second_break_ind + 1) - second_break_ind)) - 1;

    return fci;
}

OLPointData& OLMesh::raycast(const OLVector3f& origin, const OLVector3f& direction, bool cull_backfaces, float near_clip, float far_clip)
{
    OLPointData hit_data{ {}, far_clip, {}, {} };

    size_t num_triangles = num_face_corners / 3;
    for (size_t t_ind = 0; t_ind < num_triangles; t_ind++)
    {
        OLVector3f n = face_normals[t_ind];
        float dot = n ^ direction;
        if (dot > 0.0f && cull_backfaces) continue;
        if (dot == 0.0f) continue;

        OLVector3f va = vertices[face_corners[(t_ind * 3) + 0]];
        float dist = ((va - origin) ^ n) / dot;
        if (dist < near_clip || dist > hit_data.depth) continue;

        OLVector3f p = (direction * dist) + origin;
        OLVector3f vap = p - va;
        float da = vap ^ edge_vectors[(t_ind * 2) + 0];
        float db = vap ^ edge_vectors[(t_ind * 2) + 1];

        float dabac = edge_dots[(t_ind * 3) + 1];
        float inv_denom = inv_denominators[t_ind];

        float v = ((edge_dots[(t_ind * 3) + 2] * da) - (dabac * db)) * inv_denom;
        if (v < 0 || v > 1) continue;
        float w = ((edge_dots[(t_ind * 3) + 0] * db) - (dabac * da)) * inv_denom;
        if (w < 0 || w > 1) continue;
        float u = 1 - v - w;
        if (u < 0 || u > 1) continue;

        hit_data.depth = dist;
        hit_data.normal = n;
        hit_data.position = p;
        // TODO: calcualte UV
    }

    return hit_data;
}

bool OLMesh::readFromFile(const char* filename)
{
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) return false;

    deallocateBuffers();

    // prepass to count vertices and triangles
    uint16_t found_vertices = 0;
    uint16_t found_triangles = 0;
    uint16_t found_uvs = 0;
    uint16_t found_vnorms = 0;

    std::string line;
    while (getline(file, line))
    {
        if (line.length() < 2) continue;
        if (line[0] == '#') continue;
        if (line[0] == 'v' && line[1] == ' ')
        {
            found_vertices++;
        }
        if (line[0] == 'f' && line[1] == ' ')
        {
            found_triangles++;
        }
        if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            found_uvs++;
        }
        if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            found_vnorms++;
        }
    }

    num_face_corners = found_triangles * 3;
    num_vertices = found_vertices;

    face_corners = new size_t[num_face_corners];
    vertices = new OLVector3f[num_vertices];
    texture_coordinates = new OLVector2f[num_face_corners];
    corner_normals = new OLVector3f[num_face_corners];

    OLVector2f* uvs_temp = NULL;
    OLVector3f* vns_temp = NULL;

    if (found_uvs != 0)
        uvs_temp = new OLVector2f[found_uvs];
    if (found_vnorms != 0)
        vns_temp = new OLVector3f[found_vnorms];

    // process the file, excluding triangles
    file.clear();
    file.seekg(0, std::ios::beg);

    int v_ind = 0;
    int vt_ind = 0;
    int vn_ind = 0;
    int f_ind = 0;
    std::string type;
    OLVector3f tmp3;
    OLVector2f tmp2;

    while (!file.eof())
    {
        file >> type;
        if (type == "v")
        {
            // read vertex position
            file >> tmp3.x;
            file >> tmp3.y;
            file >> tmp3.z;
            vertices[v_ind] = tmp3;
            v_ind++;
        }
        else if (type == "vn")
        {
            // read vertex normal
            file >> tmp3.x;
            file >> tmp3.y;
            file >> tmp3.z;
            if (vn_ind >= found_vnorms) std::cout << "shit";
            vns_temp[vn_ind] = tmp3;
            vn_ind++;
        }
        else if (type == "vt")
        {
            // read vertex uv
            file >> tmp2.x;
            file >> tmp2.y;
            uvs_temp[vt_ind] = tmp2;
            vt_ind++;
        }
        // skip to next line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // iterate the file again, processing triangles now. TODO: support non-tri faces
    std::string v_inds;
    OLFaceCornerInfo fci;
    file.clear();
    file.seekg(0, std::ios::beg);
    while (!file.eof())
    {
        file >> type;
        if (type == "f")
        {
            if (f_ind >= num_face_corners)
                break;
            // read triangle

            // first vert
            file >> v_inds;
            fci = splitFaceCorner(v_inds);

            if (fci.vert < found_vertices)
                face_corners[f_ind] = fci.vert;
            else
                face_corners[f_ind] = 0;
            if (fci.uv < found_uvs && found_uvs > 0)
                texture_coordinates[f_ind] = uvs_temp[fci.uv];
            if (fci.vn < found_vnorms && found_vnorms > 0)
                corner_normals[f_ind] = vns_temp[fci.vn];
            f_ind++;

            // second vert
            file >> v_inds;
            fci = splitFaceCorner(v_inds);

            if (fci.vert < found_vertices)
                face_corners[f_ind] = fci.vert;
            else
                face_corners[f_ind] = 0;
            if (fci.uv < found_uvs && found_uvs > 0)
                texture_coordinates[f_ind] = uvs_temp[fci.uv];
            if (fci.vn < found_vnorms && found_vnorms > 0)
                corner_normals[f_ind] = vns_temp[fci.vn];
            f_ind++;

            // third vert
            file >> v_inds;
            fci = splitFaceCorner(v_inds);

            if (fci.vert < found_vertices)
                face_corners[f_ind] = fci.vert;
            else
                face_corners[f_ind] = 0;
            if (fci.uv < found_uvs && found_uvs > 0)
                texture_coordinates[f_ind] = uvs_temp[fci.uv];
            if (fci.vn < found_vnorms && found_vnorms > 0)
                corner_normals[f_ind] = vns_temp[fci.vn];
            f_ind++;
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // clean up
    file.close();

    if (found_uvs > 0) delete[] uvs_temp;
    if (found_vnorms > 0) delete[] vns_temp;

    updateSecondaryData();
}

OLMesh::~OLMesh()
{
    deallocateBuffers();
}

void OLMesh::updateSecondaryData()
{
	if (face_normals) delete face_normals;
	if (edge_vectors) delete edge_vectors;
	if (edge_dots) delete edge_dots;
	if (inv_denominators) delete inv_denominators;

	size_t num_triangles = num_face_corners / 3;

	face_normals = new OLVector3f[num_triangles];
	edge_vectors = new OLVector3f[num_triangles * 2];
	edge_dots = new float[num_triangles * 3];
	inv_denominators = new float[num_triangles];

	for (size_t i = 0; i < num_triangles; i++)
	{
		OLVector3f va = vertices[face_corners[(i * 3) + 0]];
		OLVector3f vb = vertices[face_corners[(i * 3) + 1]];
		OLVector3f vc = vertices[face_corners[(i * 3) + 2]];

		OLVector3f vab = vb - va;
		OLVector3f vac = vc - va;
		edge_vectors[(i * 2) + 0] = vab;
		edge_vectors[(i * 2) + 1] = vac;

		OLVector3f n = norm(vab % vac);
		face_normals[i] = n;

		if (corner_normals[(i * 3) + 0] == OLVector3f{ 0,0,0 }) corner_normals[(i * 3) + 0] = n;
		if (corner_normals[(i * 3) + 1] == OLVector3f{ 0,0,0 }) corner_normals[(i * 3) + 1] = n;
		if (corner_normals[(i * 3) + 2] == OLVector3f{ 0,0,0 }) corner_normals[(i * 3) + 2] = n;

		float dabab = vab ^ vab;
		float dabac = vab ^ vac;
		float dacac = vac ^ vac;
		edge_dots[(i * 3) + 0] = dabab;
		edge_dots[(i * 3) + 1] = dabac;
		edge_dots[(i * 3) + 2] = dacac;

		inv_denominators[i] = 1.0f / ((dabab * dacac) - (dabac * dabac));
	}
}

void OLMesh::deallocateBuffers()
{
    if (face_corners) { delete face_corners; face_corners = NULL; }
    if (vertices) { delete vertices; vertices = NULL; }
    if (texture_coordinates) { delete texture_coordinates; texture_coordinates = NULL; }
    if (corner_normals) { delete corner_normals; corner_normals = NULL; }
    if (face_normals) { delete face_normals; face_normals = NULL; }
    if (edge_vectors) { delete edge_vectors; edge_vectors = NULL; }
    if (edge_dots) { delete edge_dots; edge_dots = NULL; }
    if (inv_denominators) { delete inv_denominators; inv_denominators = NULL; }
}

OLMesh::OLMesh(const char* filename)
{
    face_corners = NULL;
    vertices = NULL;
    texture_coordinates = NULL;
    corner_normals = NULL;
    face_normals = NULL;
    edge_vectors = NULL;
    edge_dots = NULL;
    inv_denominators = NULL;

    num_face_corners = 0;
    num_vertices = 0;

    readFromFile(filename);
}
