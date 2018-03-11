#include "coordinates.h"


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
    return result;
}

const coordinates coordinates::operator +(const coordinates& other) const {
    coordinates result(latitude + other.latitude, longitude + other.longitude);
    return result;
}
