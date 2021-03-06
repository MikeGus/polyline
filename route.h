#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <string>
#include "coordinates.h"
#include "polyline.h"
#include <QString>
#include "visitor.h"


class route {
public:
    route() = default;

    route(const QString& name, const QString& date);

    route(const QString& name, const QString& date, const polyline& poly);

    void readFromFile(const QString& filename);

    coordinates& operator [](const int position);

    const coordinates& operator [](const int position) const;

    void addLast(const coordinates waypoint);

    coordinates removeLast();

    void add(const coordinates waypoint, const int position);

    void remove(const int position);

    const polyline generatePolyline() const;

    void setName(const QString& name);

    QString getName() const;

    void setDate(const QString& date);

    QString getDate() const;

    size_t getNumberOfPoints() const;

    float length() const;

    bool operator ==(const route& other) const;

    void accept(visitor& v);

private:
    const polyline generatePolylineForWaypoint(const coordinates& waypoint) const;

    const polyline generatePolylineForValue(const float value) const;

    double decodePolylineForValue(const std::string& value, size_t& i) const;

    QString name;
    QString date;
    std::vector<coordinates> waypoints;
};

#endif // ROUTE_H
