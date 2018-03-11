#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "coordinates.h"
#include "polyline.h"


class route {
public:
    route() = default;
    route(const polyline& poly);

    coordinates& operator [](const int position);
    const coordinates& operator [](const int position) const;

    void addLast(const coordinates waypoint);
    coordinates removeLast();

    void add(const coordinates waypoint, const int position);
    void remove(const int position);

    const polyline generatePolyline() const;

private:
    const polyline generatePolylineForWaypoint(const coordinates& waypoint) const;
    const polyline generatePolylineForValue(const float value) const;

    const coordinates generateWaypointFromPolyline(const polyline& line) const;
    float generateValueFromPolyline(const polyline& line) const;

    std::vector<coordinates> waypoints;
};

#endif // ROUTE_H
