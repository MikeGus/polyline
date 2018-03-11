#include "route.h"
#include <cmath>


route::route(const polyline& poly) {
    unsigned waypointPolyLength = 12;
    unsigned waypointNumber = poly.length() / waypointPolyLength;
    coordinates previous;

    unsigned startIndex = 0;
    for (unsigned i = 0; i < waypointNumber; ++i) {
        char bufferCharPolyline[13];
        bufferCharPolyline[waypointPolyLength] = 0;
        for (unsigned i = startIndex; i < startIndex + waypointPolyLength; ++i) {
            bufferCharPolyline[i] = poly[i];
        }
        polyline bufferPolyline(bufferCharPolyline);
        coordinates current(generateWaypointFromPolyline(bufferPolyline));
        waypoints.push_back(current - previous);
        previous = current;
    }
}

coordinates& route::operator [](const int position) {
    return waypoints[position];
}

const coordinates& route::operator [](const int position) const {
    return waypoints[position];
}

void route::addLast(const coordinates waypoint) {
    waypoints.push_back(waypoint);
}

coordinates route::removeLast() {
    coordinates result(waypoints.back());
    waypoints.pop_back();
    return result;
}

void route::add(const coordinates waypoint, const int position) {
    auto iterator(waypoints.cbegin());
    std::advance(iterator, position);
    waypoints.insert(iterator, waypoint);
}

void route::remove(const int position) {
    auto iterator(waypoints.cbegin());
    std::advance(iterator, position);
    waypoints.erase(iterator);
}

const polyline route::generatePolyline() const {
    polyline result;
    if (waypoints.empty()) {
        return result;
    }

    auto iterator(waypoints.begin());
    result.append(generatePolylineForWaypoint(*iterator));
    coordinates previous(*iterator);

    iterator++;
    while (iterator != waypoints.end()) {
        result.append(generatePolylineForWaypoint(*iterator - previous));
    }

    return result;
}

const polyline route::generatePolylineForWaypoint(const coordinates &waypoint) const {
    polyline latitude(generatePolylineForValue(waypoint.getLatitude()));
    polyline longitude(generatePolylineForValue(waypoint.getLongitude()));
    return latitude.append(longitude);
}

const polyline route::generatePolylineForValue(const float value) const {
    int resultValue = value * 1e5 + 0.5;
    bool negative = resultValue < 0;
    if (negative) {
        resultValue ^= 1;
        resultValue += 1;
    }
    resultValue <<= 1;
    if (negative) {
        resultValue ^= 1;
    }
    char mask = 31;
    char bitmap[7];
    bitmap[6] = 0;
    for (int i = 0; i < 6; ++i) {
        bitmap[5 - i] = resultValue & mask;
        resultValue >>= 5;
    }
    for (int i = 0; i < 5; ++i) {
        bitmap[i] += 0x20;
        bitmap[i] += 63;
    }

    polyline result(bitmap);
    return result;
}

const coordinates route::generateWaypointFromPolyline(const polyline& line) const {

    char str[7];
    str[6] = 0;
    for (int i = 0; i < 6; ++i) {
        str[i] = line[i];
    }
    polyline latitude(str);
    for (int i = 0; i < 6; ++i) {
        str[i] = line[i];
    }
    polyline longitude(str);

    coordinates result(generateValueFromPolyline(latitude),
                       generateValueFromPolyline(longitude));

    return result;
}

float route::generateValueFromPolyline(const polyline& line) const {
    char bitmap[7];
    bitmap[6] = 0;
    for (int i = 0; i < 6; ++i) {
        bitmap[i] = line[i];
    }
    for (int i = 0; i < 6; ++i) {
        bitmap[i] -= 63;
    }
    for (int i = 0; i < 5; ++i) {
        bitmap[i] -= 0x20;
    }

    int value = 0;
    for (int i = 0; i < 6; ++i) {
        value += bitmap[5 - i] * pow(32, i);
    }

    bool negative = false;
    if (value % 2) {
        value ^= 1;
        negative = true;
    }

    value >>= 1;

    if (negative) {
        unsigned mask = pow(2, 31);
        value |= mask;
        value -= 1;
        value ^= 1;
    }

    return value / 1e5;
}
