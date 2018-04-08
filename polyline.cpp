#include "polyline.h"


polyline::polyline(const std::string& data): data(data) {
}

polyline::polyline(const char* data): data(data) {
}

const std::string& polyline::get() const {
    return data;
}

char& polyline::operator [](const int position) {
    return data[position];
}

const char& polyline::operator [](const int position) const {
    return data[position];
}

void polyline::set(const std::string& data) {
    this->data = data;
}

void polyline::set(const char* data) {
    this->data = data;
}

polyline& polyline::append(const polyline& other) {
    data.append(other.data);
    return *this;
}

unsigned polyline::length() const {
    return data.length();
}

bool polyline::operator ==(const polyline& other) const {
    return data == other.data;
}
