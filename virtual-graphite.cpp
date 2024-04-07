// virtual-graphite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

#include <set>
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
        srand(0);
        set<Vector2> points;
        points.insert(Vector2{ 0,0 });
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
            points.insert(best_candidate);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (Vector2 p : points) DrawCircle(((p.x + 1.0f) / 2.0f) * 1024, ((p.y + 1.0f) / 2.0f) * 1024, 1.0f, WHITE);
        EndDrawing();

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
