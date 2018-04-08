#include "route.h"
#include <cmath>
#include <vector>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

static const double s_presision   = 100000.0;
static const int    s_chunkSize   = 5;
static const int    s_asciiOffset = 63;
static const int    s_5bitMask    = 0x1f; // 0b11111 = 31
static const int    s_6bitMask    = 0x20; // 0b100000 = 32

route::route(const QString& name, const QString& date): name(name), date(date) {
}

route::route(const QString &name, const QString &date, const polyline& poly):
    name(name), date(date) {

    size_t i = 0;
    std::string coords = poly.get();

    while (i < coords.size())
    {
        auto lat = decodePolylineForValue(coords, i);
        auto lon = decodePolylineForValue(coords, i);

        if (!waypoints.empty()) {
            const auto &prevPoint = waypoints.back();
            lat += prevPoint.getLatitude();
            lon += prevPoint.getLongitude();
        }
       waypoints.emplace_back(coordinates(round(lat * 1e5) / 1e5, round(lon * 1e5) / 1e5));
    };
}

void route::readFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QXmlStreamReader reader(file.readAll());
    while (!reader.atEnd() && !reader.hasError()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "name") {
                name = reader.readElementText();
            }
            if (reader.name() == "trkpt") {
               QXmlStreamAttributes attrib = reader.attributes();
               bool bOk = false;
               double longitude = attrib.value("lon").toDouble(&bOk);
               if (!bOk) {
                   throw std::invalid_argument("Долгота отсутствует");
               }
               if (fabs(longitude) > 180) {
                   throw std::invalid_argument("Долгота имеет некорректное значение");
               }
               double latitude = attrib.value("lat").toDouble(&bOk);
               if (!bOk) {
                   throw std::invalid_argument("Широта отсутствует");
               }
               if (fabs(latitude) > 90) {
                   throw std::invalid_argument("Широта имеет некорректное значение");
               }
               waypoints.push_back(coordinates(latitude, longitude));
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
    int32_t e5 = std::round(value * s_presision); // (2)

    e5 <<= 1;                                     // (4)

    if (value < 0) {
     e5 = ~e5;                                 // (5)
    }

    bool hasNextChunk = false;
    std::string result;

    // Split the value into 5-bit chunks and convert each of them to integer
    do {
        int32_t nextChunk = (e5 >> s_chunkSize); // (6), (7) - start from the left 5 bits.
        hasNextChunk = nextChunk > 0;

        int charVar = e5 & s_5bitMask;           // 5-bit mask (0b11111 == 31). Extract the left 5 bits.
        if (hasNextChunk) {
         charVar |= s_6bitMask;               // (8)
        }
        charVar += s_asciiOffset;                // (10)
        result += (char)charVar;                 // (11)

        e5 = nextChunk;
    } while (hasNextChunk);

    return polyline(result);
}

double route::decodePolylineForValue(const std::string& coords, size_t& i) const {
    int32_t result = 0;
    int shift = 0;
    char c = 0;
    do {
        c = coords.at(i++);
        c -= s_asciiOffset;      // (10)
        result |= (c & s_5bitMask) << shift;
        shift += s_chunkSize;    // (7)
    } while (c >= s_6bitMask);

    if (result & 1) {
        result = ~result;        // (5)
    }
    result >>= 1;                // (4)

    // Convert to decimal value.
    return result / s_presision; // (2)
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

bool route::operator ==(const route& other) const {
    return waypoints == other.waypoints;
}
