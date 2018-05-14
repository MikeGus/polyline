#ifndef COORDINATES_H
#define COORDINATES_H


class coordinates {
public:
    coordinates(const float latitude=0.0, const float longitude=0.0, const float height=0.0);

    double getLatitude() const;

    double getLongitude() const;

    double getHeight() const;

    void setLatitude(const float latitude);

    void setLongitude(const float longitude);

    void setHeight(const float height);

    const coordinates operator -(const coordinates& other) const;

    const coordinates operator +(const coordinates& other) const;

    float distance(const coordinates& other) const;

    bool operator ==(const coordinates& other) const;

private:
    double latitude;
    double longitude;
    double height;
};

#endif // COORDINATES_H
