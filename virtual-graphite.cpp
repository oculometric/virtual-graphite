// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include "camera.h"
#include "vector2.h"
#include "mesh.h"

using namespace std;

int main()
{
    InitWindow(256, 256, "oculib");
    SetTargetFPS(4);

    OLMatrix4f world_to_camera =
        ~(OLCamera::projectionMatrix(0.01f, 10.0f, 90.0f, 1.0f)
        *
            ~OLObject::objectMatrix(OLVector3f{ 0,0,0 }, -OLVector3f OL_UP, OL_BACK, -OLVector3f OL_RIGHT, OLVector3f{ 1,1,1 }));
    
    OLMesh demo_mesh("suzanne.obj");
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        OLVector4f sceen_space = { -1.0f, 1.0f, 1.0f, 1.0f };
        for (int y = 0; y < GetScreenHeight(); y++)
        {
            sceen_space.x = -1.0f;
            for (int x = 0; x < GetScreenWidth(); x++)
            {
                OLVector4f world_space = world_to_camera * sceen_space;
                OLVector3f direction = norm(OLVector3f{ world_space.x, world_space.y, world_space.z });
                OLPointData data = demo_mesh.raycast(OLVector3f{ -2,0,0 }, direction, true, 0.01f, 10.0f);
                float remapped_depth = max(min((data.depth - 0.01f) / (10.0f - 0.01f), 1.0f), 0.0f);
                DrawPixel(x, y, Color{ (unsigned char)(remapped_depth * 255), (unsigned char)(remapped_depth * 255), (unsigned char)(remapped_depth * 255), 255 });
                sceen_space.x += 2.0f / GetScreenWidth();
            }
            sceen_space.y -= 2.0f / GetScreenHeight();
        }

        EndDrawing();

        cout << "frame rendered" << endl;
    }

    return 0;
}