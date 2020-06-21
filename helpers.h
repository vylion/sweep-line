
#ifndef HELPERS_H

#include <utility>
#include <string>
#include <random>
#include <time.h>

#include "raymath.h"

typedef Vector2 Point;
typedef std::pair<Point, Point> LineSegment;

enum Orientation
{
    CW = 1,
    CCW = -1,
    CL = 0
};

bool operator==(const Vector2 &v, const Vector2 &w);
Vector2 operator-(const Vector2 &v, const Vector2 &w);
Vector2 operator+(const Vector2 &v, const Vector2 &w);
Color darkenBy(const Color &c, int amount);
Orientation orientation(const Point &p, const Point &q, const Point &r);
bool sharedEndpoint(const LineSegment &s, const LineSegment &t);
bool insideRectangle(const LineSegment &t, const Point &p);
bool insideRectangle(const LineSegment &t, const LineSegment &s);
bool intersect(const LineSegment &s, const LineSegment &t);
void classifyIntersection(const LineSegment &s, const LineSegment &t, Color &colour, std::string &desc);
void classifyIntersection(const LineSegment &s, const LineSegment &t, Color &colour);

#endif // !HELPERS_H
