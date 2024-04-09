// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include <vector>
#include <iostream>

#include "dither.h"

using namespace std;

#define DOWNSCALE 2

int main()
{
    InitWindow(1024, 1024, "vg");
    SetTargetFPS(1);

    Image img = LoadImage("demo.png");

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
                unsigned char value = 255 * orderedDitherFloat(magnitude, 32, x / DOWNSCALE, y / DOWNSCALE, 8);
                col.r = value;
                col.g = value;
                col.b = value;

                DrawPixel(x, y, col);
            }
        }
        
        EndDrawing();
        if (filter == 4) filter = 8;
        else if (filter == 2) filter = 4;
        else filter = 2;
    }

    return 0;
}