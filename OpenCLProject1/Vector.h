#pragma once

#include "CL\cl.hpp"

typedef cl_float3 cl_Vector;

class Vector {
private:
    float x, y, z;

public:
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector(): Vector(0.0f, 0.0f, 0.0f) {}

    Vector& operator+=(const Vector& vector);
    Vector& operator-=(const Vector& vector);
    Vector& operator*=(const float scalar);
	float operator*(const Vector &vector);
    bool operator!=(const Vector& other);
    bool operator==(const Vector& other);

    float getX() const;
    float getY() const;
    float getZ() const;

    cl_float3 get() const;

    float length2() const;
    float length() const;

    void normalize();

    void add(const Vector vector);
    void add(const Vector * const vector);
    void add(const float scalar);
    void subtract(const float scalar);

    cl_Vector getClData();
};

inline Vector operator+(Vector lhs, const Vector &rhs) {
    lhs += rhs;
    return lhs;
}

inline Vector operator-(Vector lhs, const Vector &rhs) {
    lhs -= rhs;
    return lhs;
}

inline Vector operator*(Vector lhs, const float scalar) {
    lhs *= scalar;
    return lhs;
}
