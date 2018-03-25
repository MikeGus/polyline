#include "route.h"
#include <cmath>
#include <vector>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

route::route(const QString& name, const QString& date): name(name), date(date) {
}

route::route(const QString &name, const QString &date, const polyline& poly):
    name(name), date(date) {
    waypoints.clear();

    int len = poly.length();
    int index = 0;

    float lat = 0;
    float lng = 0;

    while (index < len) {
        char b;
        int shift = 0;
        int result = 0;
        do {
            b = poly[index++] - 63;
            result |= (b & 0x1f) << shift;
            shift += 5;
        } while (b >= 0x20);
        float dlat = ((result & 1) ? ~(result >> 1) : (result >> 1));
        lat += dlat;

        shift = 0;
        result = 0;
        do {
            b = poly[index++] - 63;
            result |= (b & 0x1f) << shift;
            shift += 5;
        } while (b >= 0x20);

        float dlng = ((result & 1) ? ~(result >> 1) : (result >> 1));
        lng += dlng;

        coordinates point(lat * 1e-5, lng * 1e-5);
        waypoints.push_back(point);
    }
}

void route::readFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QXmlStreamReader reader(&file);
    if (reader.readNextStartElement()) {
        while (reader.readNextStartElement()) {
            if (reader.name() == "trk") {
                while (reader.readNextStartElement()) {
                    if (reader.name() == "name") {
                        name = reader.readElementText();
                    } else if (reader.name() == "desc") {
                        QString len = reader.readElementText();
                        len.truncate(len.indexOf(" "));
                    }
                    else if (reader.name() == "trkseg") {
                        while (!reader.isEndDocument()) {
                            reader.readNext();
                            if (reader.isStartElement()) {
                                if (reader.name().toString() == "trkpt") {
                                    double lat = reader.attributes().value("lat").toDouble();
                                    double lon = reader.attributes().value("lon").toDouble();
                                    coordinates newCoordinate(lat, lon);
                                    waypoints.push_back(newCoordinate);
                                }
                            }
                            else {
                                reader.readNext();
                            }
                        }
                    }
                    else {
                        reader.skipCurrentElement();
                    }
                }
            }
            else {
                reader.skipCurrentElement();
            }
        }
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
    ++iterator;

    while (iterator != waypoints.end()) {
        result.append(generatePolylineForWaypoint(*iterator - previous));
        previous = *iterator;
        ++iterator;
    }

    return result;
}

const polyline route::generatePolylineForWaypoint(const coordinates &waypoint) const {
    polyline latitude(generatePolylineForValue(waypoint.getLatitude()));
    polyline longitude(generatePolylineForValue(waypoint.getLongitude()));
    return latitude.append(longitude);
}

const polyline route::generatePolylineForValue(const float value) const {

    char result[6];

    int val = value * 1e5 + 0.5;

    bool isNeg = val < 0;
    val <<= 1;

    if (isNeg) {
        val = ~val;
    }

    unsigned count = 0;

    do {
      result[count] = val & 0x1f;
      val >>= 5;

      if (val) {
          result[count] |= 0x20;
      }

      result[count] += 63;

      ++count;
    } while (val);

    result[count] = 0;
    polyline resPoly = polyline(result);

    return resPoly;
}

float route::length() const {
    if (waypoints.size() < 2) {
        return 0;
    }
    float general = 0;
    auto itFirst = waypoints.cbegin();
    for (auto itSecond = itFirst + 1; itSecond != waypoints.cend();
         ++itFirst, ++itSecond) {
        general += (*itSecond).distance(*itFirst);
    }

    return general / 1000;
}

void route::setName(const QString& name) {
    this->name = QString(name);
}

QString route::getName() const {
    return name;
}

void route::setDate(const QString& date) {
    this->date = date;
}

QString route::getDate() const {
    return date;
}

size_t route::getNumberOfPoints() const {
    return waypoints.size();
}
