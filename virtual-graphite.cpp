// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include <vector>
#include <iostream>

using namespace std;

inline float distance(const Vector2& a, const Vector2& b)
{
    return ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
}

inline bool operator<(const Vector2& a, const Vector2& b)
{
    if (a.x < b.x)
        return true;
    else if (a.x > b.x)
        return false;
    else
    {
        if (a.y < b.y) return true;
        else return false;
    }
}

#define NUM_CANDIDATES 5
const float RAND_DIVISOR = 1.0f / RAND_MAX;

inline float randf()
{
    return ((float)(rand() << 1) * RAND_DIVISOR) - 1.0f;
}

int main()
{
    InitWindow(1024, 1024, "vg");
    SetTargetFPS(60);
    
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
                //cout << f_x << " : " << x << endl;
                float value = 0.0f;
                for (Vector2 p : points)
                {
                    value = max(1.0f - (distance(Vector2{ f_x, f_y }, p) * 50.0f), value);
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

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
