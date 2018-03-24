#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <string>
#include "coordinates.h"
#include "polyline.h"
#include <QString>


class route {
public:
    route() = default;
    route(const QString& name);
    route(const polyline& poly);

    coordinates& operator [](const int position);
    const coordinates& operator [](const int position) const;

    void addLast(const coordinates waypoint);
    coordinates removeLast();

    void add(const coordinates waypoint, const int position);
    void remove(const int position);

    const polyline generatePolyline() const;

    void setName(const QString& name);
    QString getName() const;

    size_t getNumberOfPoints() const;
    float length() const;

private:
    const polyline generatePolylineForWaypoint(const coordinates& waypoint) const;
    const polyline generatePolylineForValue(const float value) const;

    const coordinates generateWaypointFromPolyline(const polyline& line) const;
    float generateValueFromPolyline(const polyline& line) const;

    QString name;
    std::vector<coordinates> waypoints;
};

#endif // ROUTE_H
