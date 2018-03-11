#ifndef POLYLINE_H
#define POLYLINE_H

#include <string>


class polyline {
public:
    polyline() = default;
    polyline(const std::string& data);
    polyline(const char* data);

    const std::string& get() const;
    char& operator [](const int position);
    const char& operator [](const int position) const;

    void set(const std::string& data);
    void set(const char* data);

    polyline& append(const polyline& other);

    unsigned length() const;
private:
    std::string data;
};

#endif // POLYLINE_H
