
#include "helpers.h"

bool operator==(const Vector2 &v, const Vector2 &w)
{
    return v.x == w.x and v.y == w.y;
}

Vector2 operator-(const Vector2 &v, const Vector2 &w)
{
    return Vector2{v.x - w.x, v.y - w.y};
}

Vector2 operator+(const Vector2 &v, const Vector2 &w)
{
    return Vector2{v.x + w.x, v.y + w.y};
}

Color darkenBy(const Color &c, int amount)
{
    return Color{std::max(c.r - amount, 0), std::max(c.g - amount, 0), std::max(c.b - amount, 0)};
}

/* This fucntion tells us wether a loop pqr is clockwise, counterclockwise,
 * or if they're colinear */
Orientation orientation(const Point &p, const Point &q, const Point &r)
{
    Vector2 pq = p - q;
    Vector2 pr = p - r;

    float o = pq.x * pr.y - pr.x * pq.y;

    if (o > 0)
        return CW;
    else if (o < 0)
        return CCW;
    return CL;
}

bool sharedEndpoint(const LineSegment &s, const LineSegment &t)
{
    return s.first == t.first or s.second == t.second or s.first == t.second or s.second == t.second;
}

/* This function checks if a point p is inside a rectangle of diagonal t
 */
bool insideRectangle(const LineSegment &t, const Point &p)
{
    float x_min = std::min(t.first.x, t.second.x);
    float y_min = std::min(t.first.y, t.second.y);
    float x_max = std::max(t.first.x, t.second.x);
    float y_max = std::max(t.first.y, t.second.y);

    return ((p.x >= x_min and p.x <= x_max) and
            (p.y >= y_min and p.y <= y_max));
}

bool insideRectangle(const LineSegment &t, const LineSegment &s)
{
    return insideRectangle(t, s.first) and insideRectangle(t, s.second);
}

bool intersect(const LineSegment &s, const LineSegment &t)
{
    int t1 = orientation(s.first, s.second, t.first);
    int t2 = orientation(s.first, s.second, t.second);
    int s1 = orientation(t.first, t.second, s.first);
    int s2 = orientation(t.first, t.second, s.second);

    // They cross at a middle point
    if (t1 != t2 and s1 != s2)
        return true;

    // s.first lies inside t
    if (t1 == 0 and insideRectangle(t, s.first))
        return true;
    // s.second lies inside t
    if (t2 == 0 and insideRectangle(t, s.second))
        return true;
    // t.first lies inside s
    if (s1 == 0 and insideRectangle(s, t.first))
        return true;
    // t.second lies inside s
    if (s2 == 0 and insideRectangle(s, t.second))
        return true;

    return false;
}

void classifyIntersection(const LineSegment &s, const LineSegment &t, Color &colour, std::string &desc)
{
    colour = GRAY;
    desc = "No intersection";

    int t1 = orientation(s.first, s.second, t.first);
    int t2 = orientation(s.first, s.second, t.second);
    int s1 = orientation(t.first, t.second, s.first);
    int s2 = orientation(t.first, t.second, s.second);

    //Segment t has endpoints on either side of s
    if ((t1 < 0) != (t2 < 0))
    {
        //Segment s has endpoints on either side of t
        if ((s1 < 0) != (s2 < 0))
        {
            desc = "Intersection in a middle point (red)";
            colour = RED;
        }
        //Segment s has just one endpoint on the line over segment t
        else if ((s1 > 0) != (s2 > 0))
        {
            desc = "Endpoint on the intersection (green)";
            colour = GREEN;
        }
    }
    //Segment t has just one endpoint on the line over segment s
    else if ((t1 > 0) != (t2 > 0))
    {
        //Segment t and segment s share an endpoint
        if (sharedEndpoint(s, t))
        {
            desc = "Intersection at a shared endpoint (dark green)";
            colour = DARKGREEN;
        }
        //Segment s has endpoints on either side of t
        else if ((s1 < 0) != (s2 < 0))
        {
            desc = "Endpoint on the intersection (green)";
            colour = GREEN;
        }
    }
    //Segment t has both endpoints on the line over segment s
    else if ((t1 == 0) and (t2 == 0))
    {
        desc = " on a shared line";

        //Segment s and segment t share one endpoint
        if (sharedEndpoint(s, t))
        {
            //Segment s and t share both endpoints
            if (sharedEndpoint(s, t))
            {
                desc = "Full overlap (purple)";
                colour = PURPLE;
            }
            else
            {
                desc = "Intersection at a shared endpoint (pink)";
                colour = PINK;
            }
        }
        //Segment t has one endpoint inside the rectangle covering both endpoints of s
        //Since t's endpoints are already on the line over s, this means having it on s
        else if (insideRectangle(s, t))
        {
            desc = "Overlap (intersection)" + desc + " (dark blue)";
            colour = DARKBLUE;
        }
        //Segment t has both endpoints on the line over s, but none inside s
        else
        {
            desc = "No intersection, but" + desc + " (blue)";
            colour = BLUE;
        }
    }
}

void classifyIntersection(const LineSegment &s, const LineSegment &t, Color &colour)
{
    std::string unused;
    classifyIntersection(s, t, colour, unused);
}
