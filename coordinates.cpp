#include "coordinates.h"
#include <cmath>

coordinates::coordinates(const float latitude, const float longitude):
    latitude(latitude), longitude(longitude) {
}

float coordinates::getLatitude() const {
    return this->latitude;
}

float coordinates::getLongitude() const {
    return this->longitude;
}

void coordinates::setLatitude(const float latitude) {
    this->latitude = latitude;
}

void coordinates::setLongitude(const float longitude) {
    this->longitude = longitude;
}

const coordinates coordinates::operator -(const coordinates& other) const {
    coordinates result(latitude - other.latitude, longitude - other.longitude);
    if (result.latitude < - 90) {
        result.latitude += 180;
    } else if (result.latitude > 90) {
        result.latitude -= 180;
    } else if (result.longitude < - 180) {
        result.longitude += 360;
    } else if (result.longitude > 180) {
        result.longitude -= 360;
    }
    return result;
}

const coordinates coordinates::operator +(const coordinates& other) const {
    coordinates result(latitude + other.latitude, longitude + other.longitude);
    return result;
}

float coordinates::distance(const coordinates& other) const {
    int R = 6371000; // metres
    float latThis = latitude / 180 * M_PI;
    float latOther = other.latitude / 180 * M_PI;

    float dLat = (latitude - other.latitude) / 180 * M_PI;
    float dLon = (longitude - other.longitude) / 180 * M_PI;

    float a = sin(dLat / 2) * sin(dLat / 2) +
            cos(latThis) * cos(latOther) *
            sin(dLon / 2) * sin(dLon / 2);

    float c = 2 * atan2(sqrt(a), sqrt(1-a));

    return R * c;
}
