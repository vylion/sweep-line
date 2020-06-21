/*******************************************************************************************
*   
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <chrono>

#include "raylib.h"
#include "helpers.h"
#include "bst.h"

using time_point = std::chrono::steady_clock::time_point;

struct ColorSegment
{
    LineSegment l;
    Color c;
    uint id;
    std::set<uint> crossed_by;
};

struct SegmentPoint
{
    Point pos;
    uint sid; // Segment ID
    bool is_left;
    bool is_intersect = false;
    uint iid = 0;
};

void DrawColorSegment(const ColorSegment &cl)
{
    DrawLine(cl.l.first.x, cl.l.first.y, cl.l.second.x, cl.l.second.y, cl.c);
}

void DrawLines(const std::vector<ColorSegment> &lines)
{
    for (uint i = 0; i < lines.size(); ++i)
    {
        DrawColorSegment(lines[i]);
    }
}

void crossLines(ColorSegment &s, ColorSegment &t)
{
    Color c;
    classifyIntersection(s.l, t.l, c);

    s.c = c;
    t.c = darkenBy(c, 10);
}

void crossingNaive(std::vector<ColorSegment> &lines)
{
    bool crossed = false;

    for (uint i = 0; i < lines.size(); ++i)
    {
        for (uint j = i + 1; j < lines.size() && !crossed; ++j)
        {
            bool crossed = intersect(lines[i].l, lines[j].l);
            if (crossed)
            {
                lines[i].crossed_by.insert(j);
                lines[j].crossed_by.insert(i);
                lines[i].c = RED;
                lines[j].c = RED;
            }
        }
    }
}

void crossingSweep(std::vector<ColorSegment> &lines, const std::vector<SegmentPoint> &points)
{
    struct ComparePoints
    {
        bool operator()(const SegmentPoint &p, const SegmentPoint &q)
        {
            return p.pos.x > q.pos.x;
        }
    };

    std::priority_queue<SegmentPoint, std::vector<SegmentPoint>, ComparePoints> events(points.begin(), points.end(), ComparePoints());
    std::cout << "Queue is size: " << events.size() << std::endl;
    BST<SegmentPoint> tree;

    while (!events.empty())
    {
        events.pop();
    }
}

enum States
{
    INIT = 0,
    NAIVE,
    SWEEP
};

States operator++(States &s)
{
    if (s == SWEEP)
        return s;

    int si = static_cast<int>(s);
    return s = static_cast<States>(++si);
}

States operator--(States &s)
{
    if (s == INIT)
        return s;

    int si = static_cast<int>(s);
    return s = static_cast<States>(--si);
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 900;
    bool close = false;

    // Initialize random engine
    std::random_device rd;
    std::mt19937_64 gen(rd()); // Delete "_64" in a 32 bit system
    std::uniform_int_distribution<> distrx(0, screenWidth);
    std::uniform_int_distribution<> distry(0, screenHeight);
    std::uniform_int_distribution<> distrnols(50, 200);

    std::vector<ColorSegment> lines, naive_lines, sweep_lines;
    std::vector<SegmentPoint> points, sweep_points;

    const int nols = distrnols(gen); // Number of lines

    // Generate random lines
    for (uint i = 0; i < nols; ++i)
    {
        Point p, q;
        p = Point{distrx(gen), distry(gen)};
        q = Point{distrx(gen), distry(gen)};

        SegmentPoint sp, sq;

        bool sp_is_left = (p.x < q.x);
        sp = SegmentPoint{p, i, sp_is_left};
        sq = SegmentPoint{q, i, !sp_is_left};

        points.push_back(sp);
        points.push_back(sq);

        LineSegment l = LineSegment{p, q};
        //std::cout << "(" << l.first.x << "," << l.first.y << ") (" << l.second.x << "," << l.second.y << std::endl;
        ColorSegment cl = ColorSegment{l, BLACK};

        lines.push_back(cl);
    }

    // Copy initial lines vector for each algorithm
    naive_lines = lines;
    sweep_lines = lines;

    // Test naive algorithm
    time_point begin_naive = std::chrono::steady_clock::now();

    crossingNaive(naive_lines);

    time_point end_naive = std::chrono::steady_clock::now();

    // Test sweep algorithm
    time_point begin_sweep = std::chrono::steady_clock::now();

    crossingSweep(sweep_lines, sweep_points);

    time_point end_sweep = std::chrono::steady_clock::now();

    std::cout << "Experiment " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << " - " << nols << " lines" << std::endl;
    std::cout << "Naive elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_naive - begin_naive).count() << " ns" << std::endl;
    std::cout << "Sweep elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_sweep - begin_sweep).count() << " ns" << std::endl;

    States state = INIT;

    InitWindow(screenWidth, screenHeight, "raylib");

    SetTargetFPS(30); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose() and !close) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_RIGHT))
            ++state;
        else if (IsKeyPressed(KEY_LEFT))
            --state;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (state)
        {
        case INIT:
            DrawLines(lines);
            break;

        case NAIVE:
            DrawLines(naive_lines);
            break;

        case SWEEP:
            DrawLines(sweep_lines);
            break;

        default:
            break;
        }

        //DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}