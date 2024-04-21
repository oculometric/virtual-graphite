// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include <vector>
#include <iostream>
#include "camera.h"
#include "vector2.h"
#include "mesh.h"
#include "dither.h"
#include "buffer.h"

#define DOWNSCALE 2
#define NUM_CANDIDATES 4

using namespace std;

float randf()
{
    return (((float)rand() / (float)RAND_MAX) * 2.0f) - 1.0f;
}

void pointsGeneratingDemo()
{
    int num_points = 100;
    while (!WindowShouldClose())
    {
        cout << "generating points" << endl;
        srand(0);
        vector<Vector2> points;
        points.push_back(Vector2{ 0,0 });
        for (int p = 1; p < num_points; p++)
        {
            Vector2 best_candidate;
            float best_candidate_distance = -1;
            for (int c = 0; c < NUM_CANDIDATES; c++)
            {
                Vector2 candidate = Vector2{ randf(), randf() };
                float closest_existing_distance = 2.0f;
                for (Vector2 t : points)
                {
                    float dif = t.x - candidate.x;
                    float x_dif = dif * dif;
                    if (x_dif > closest_existing_distance) continue;
                    dif = t.y - candidate.y;
                    float y_dif = dif * dif;
                    if (y_dif > closest_existing_distance) continue;

                    float dist = x_dif + y_dif;
                    if (dist < closest_existing_distance)
                    {
                        closest_existing_distance = dist;
                    }
                }
                if (closest_existing_distance > best_candidate_distance)
                {
                    best_candidate = candidate;
                    best_candidate_distance = closest_existing_distance;
                }
            }
            points.push_back(best_candidate);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        float f_x = -1.0f;
        float f_y = -1.0f;
        cout << "drawing!" << endl;
        for (int y = 0; y < GetScreenHeight(); y++)
        {
            f_x = -1.0f;
            for (int x = 0; x < GetScreenWidth(); x++)
            {
                float value = 0.0f;
                for (Vector2 p : points)
                {
                    value = max(1.0f - ((((f_x - p.x) * (f_x - p.x)) + ((f_y - p.y) * (f_y - p.y))) * 50.0f), value);
                }
                unsigned char val = (unsigned char)(min(value, 1.0f) * 255);
                DrawPixel(x, y, Color{ val, val, val, 255 });
                f_x += 2.0f / GetScreenWidth();
            }
            f_y += 2.0f / GetScreenHeight();
        }

        for (Vector2 p : points)
        {
            DrawPixel((p.x + 1.0f) * (GetScreenWidth() / 2.0f), (p.y + 1.0f) * (GetScreenHeight() / 2.0f), WHITE);
        }
        EndDrawing();

        cout << "done" << endl;

        num_points += num_points/2;
    }
}

void depthBufferDemo()
{
    OLMatrix4f world_to_camera =
        OLCamera::projectionMatrix(0.01f, 10.0f, 90.0f, 1.0f)
        *
        ~OLObject::objectMatrix(OLVector3f{ -2,0,0 }, -OLVector3f OL_UP, OL_BACK, -OLVector3f OL_RIGHT, OLVector3f{ 1,1,1 });
    
    OLMesh demo_mesh("suzanne.obj");

    OLMesh screen_space_mesh (demo_mesh);
    screen_space_mesh.applyTransform(world_to_camera);

    OLBuffer<float> depth_buffer(1024, 1024);
    depth_buffer.fill(10.0f);
    screen_space_mesh.writeDepthBuffer(&depth_buffer, OLDepthWrite::LESS);
    
    while (!WindowShouldClose()) {
        BeginDrawing();

        OLVector4f sceen_space = { -1.0f, 1.0f, 1.0f, 1.0f };
        for (int y = 0; y < GetScreenHeight(); y++)
        {
            //sceen_space.x = -1.0f;
            for (int x = 0; x < GetScreenWidth(); x++)
            {
                float depth = max(min((depth_buffer.access(x, y) - 0.01f) / (10.0f - 0.01f), 1.0f), 0.0f);
                DrawPixel(x, y, Color{ (unsigned char)(depth * 255), (unsigned char)(depth * 255), (unsigned char)(depth * 255), 255 });
                /*OLVector4f world_space = world_to_camera * sceen_space;
                OLVector3f direction = norm(OLVector3f{ world_space.x, world_space.y, world_space.z });
                OLPointData data = demo_mesh.raycast(OLVector3f{ -2,0,0 }, direction, true, 0.01f, 10.0f);
                float remapped_depth = max(min((data.depth - 0.01f) / (10.0f - 0.01f), 1.0f), 0.0f);
                DrawPixel(x, y, Color{ (unsigned char)(remapped_depth * 255), (unsigned char)(remapped_depth * 255), (unsigned char)(remapped_depth * 255), 255 });
                sceen_space.x += 2.0f / GetScreenWidth();*/
            }
            //sceen_space.y -= 2.0f / GetScreenHeight();
        }

        EndDrawing();
    }
}

void orderedDitheringDemo()
{
    Image img = LoadImage("demo.png");

    unsigned int* kernel = generateOrderedFilter(32);

    unsigned char* image_data = (unsigned char*)img.data;
    unsigned int filter = 2;
    while (!WindowShouldClose())
    {
        cout << "filter " << filter << endl;
        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < GetScreenHeight(); y++)
        {
            for (int x = 0; x < GetScreenWidth(); x++)
            {
                int i_x = (x - (x % DOWNSCALE)) * (img.width / 1024);
                int i_y = (y - (y % DOWNSCALE)) * (img.height / 1024);
                int image_data_offset = ((i_y * img.width) + i_x) * 3;
                Color col = Color{ image_data[image_data_offset],image_data[image_data_offset + 1],image_data[image_data_offset + 2],255 };

                float magnitude = ((0.2126 * col.r) + (0.7152 * col.g) + (0.0722 * col.b)) / 255.0f;
                unsigned char value = 255 * orderedDitherFloat(magnitude, 32, x / DOWNSCALE, y / DOWNSCALE, filter);
                col.r = value;
                col.g = value;
                col.b = value;

                DrawPixel(x, y, col);
            }
        }
        
        EndDrawing();
        if (filter == 4) filter = 8;
        else if (filter == 2) filter = 4;
        else if (filter == 8) filter = 16;
        else filter = 2;
    }
}

int main()
{
    InitWindow(1024, 1024, "vg");
    SetTargetFPS(60);
    
    depthBufferDemo();

    return 0;
}