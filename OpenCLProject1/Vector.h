#pragma once

#include "CL\cl.hpp"

typedef cl_float3 cl_Vector;

class Vector
{
    float x, y, z;

public:
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector(): Vector(0.0f, 0.0f, 0.0f) {}
    ~Vector() {}

    Vector& operator+=(const Vector& vector);
    Vector& operator-=(const Vector& vector);

    float getX() const;

    float getY() const;

    float getZ() const;

    cl_float3 get() const;

    float length2() const;
    float length() const;

    void normalize();

    void add(const Vector vector);
    void add(const Vector * const vector);
    void add(const float scalar) {
        x += scalar;
        y += scalar;
        z += scalar;
    }
    void subtract(const float scalar) {
        add(-scalar);
    }

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
