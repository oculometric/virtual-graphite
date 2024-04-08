// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include <vector>
#include <iostream>

using namespace std;

#define FILTER_SIZE 4

float points_[FILTER_SIZE * FILTER_SIZE] =
{
    14,  1, 13,  2,
     4, 8,  6,  10,
    12,  3, 15,  5,
     7,  11,  0, 9,
};

float points__[FILTER_SIZE * FILTER_SIZE] =
{
     8,  2,  12, 4,
    12,  4, 8,  2,
    4,  12, 2,  8,
     2, 8,  4,  12,
};

float points[FILTER_SIZE * FILTER_SIZE] =
{
    2,  8,  1,  10,
    15,  7,  13,  5,
    0,  11,  3,  9,
    12,  4,  14,  6,
};


float filterValue(int x, int y)
{
    return points[((y % FILTER_SIZE) * FILTER_SIZE) + (x % FILTER_SIZE)] / (FILTER_SIZE * FILTER_SIZE);
}

#define DOWNSCALE 2

int main()
{
    InitWindow(1024, 1024, "vg");
    SetTargetFPS(60);

    Image img = LoadImage("demo.png");

    unsigned char* image_data = (unsigned char*)img.data;
    while (!WindowShouldClose())
    {
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

                float filter = filterValue(x / DOWNSCALE, y / DOWNSCALE);
                
                float magnitude = (0.2126 * col.r) + (0.7152 * col.g) + (0.0722 * col.b);
                unsigned char value = 255 * ((magnitude / 255.0f) >= filter);
                col.r = value;
                col.g = value;
                col.b = value;

                DrawPixel(x, y, col);
            }
        }
        
        EndDrawing();
    }

    return 0;
}