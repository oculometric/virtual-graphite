// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib-5.0_win64_msvc16/include/raylib.h"

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
        std::cout << "generating points" << endl;
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
        std::cout << "drawing!" << endl;
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

        std::cout << "done" << endl;

        num_points += num_points/2;
    }
}

void depthBufferDemo()
{
    OLMatrix4f camera_to_view = OLCamera::projectionMatrix(0.01f, 10.0f, 90.0f, 1.0f);
    OLMatrix4f world_to_camera = ~OLObject::objectMatrix(OLVector3f{ -2,0,2 }, -OLVector3f OL_UP, norm(OLVector3f{ 1,0,1 }), norm(OLVector3f{ -1,0,1 }), OLVector3f{ 1,1,1 });
    OLMatrix4f world_to_view = camera_to_view * world_to_camera;
    OLMesh demo_mesh("suzanne.obj");

    OLBuffer<float> depth_buffer(1024, 1024);

    OLMatrix4f model_to_world;
    float rotation_z = 0.0f;

    Image screen_img;
    screen_img.width = 1024;
    screen_img.height = 1024;
    screen_img.format = PixelFormat::PIXELFORMAT_UNCOMPRESSED_R32;
    screen_img.mipmaps = 1;
    screen_img.data = depth_buffer.getBufferAddress();

    BeginDrawing();
    Texture2D screen_tex = LoadTextureFromImage(screen_img);
    EndDrawing();

    while (!WindowShouldClose()) {
        model_to_world = OLObject::objectMatrix(OL_ZERO, OLVector3f{ cos(rotation_z), -sin(rotation_z),0 }, OLVector3f{ sin(rotation_z), cos(rotation_z), 0 }, OL_BACK, OL_ONE);
        OLMatrix4f model_to_view = world_to_view * model_to_world;
        OLMesh screen_space_mesh(demo_mesh);
        screen_space_mesh.applyTransform(model_to_view);
        depth_buffer.fill(10.0f);
        screen_space_mesh.writeDepthBuffer(&depth_buffer, OLDepthWrite::LESS);
        for (unsigned int i = 0; i < depth_buffer.getLength(); i++) depth_buffer.unsafeAccess(i) /= 10.0f;

        BeginDrawing();

        UnloadTexture(screen_tex);
        screen_tex = LoadTextureFromImage(screen_img);
        DrawCircle(100, 100, 24, RED);
        DrawTexture(screen_tex, 0, 0, WHITE);

        DrawFPS(0, 0);

        EndDrawing();
        
        rotation_z += (2.0f / 180.0f) * PI;
    }
    UnloadTexture(screen_tex);

}

void orderedDitheringDemo()
{
    Image img = LoadImage("demo.png");

    unsigned int* kernel = generateOrderedFilter(32);

    unsigned char* image_data = (unsigned char*)img.data;
    unsigned int filter = 2;
    while (!WindowShouldClose())
    {
        std::cout << "filter " << filter << endl;
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