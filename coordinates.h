#ifndef COORDINATES_H
#define COORDINATES_H


class coordinates {
public:
    coordinates(const float latitude=0.0, const float longitude=0.0);

    float getLatitude() const;

    float getLongitude() const;

    void setLatitude(const float latitude);

    void setLongitude(const float longitude);

    const coordinates operator -(const coordinates& other) const;

    const coordinates operator +(const coordinates& other) const;

    float distance(const coordinates& other) const;

    bool operator ==(const coordinates& other) const;

private:
    double latitude;
    double longitude;
};

#endif // COORDINATES_H
