#pragma once

#include "CL\cl.hpp"

class Vector
{
    float x,y,z;

public:
    Vector(float x, float y, float z) : x(x), y(y), z(z) {};
    Vector(): Vector(0.0f, 0.0f, 0.0f) {}
    ~Vector();

    Vector& operator+=(const Vector& vector) {
        this->add(vector);
        return *this;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    float getZ() const {
        return z;
    }

    cl_float3 get() const {
        cl_float3 value = {x,y,z};
        return value;
    }

    float length2() const {
        return pow(x,2) + pow(y,2) + pow(z,2);
    }
    float length() const {
        return sqrt(length2());
    }

    void normalize() {
        const float length = this->length();
        this->x /= length;
        this->y /= length;
        this->z /= length;
    }

    void add(const Vector vector) {
        this->x += vector.getX();
        this->y += vector.getY();
        this->z += vector.getZ();
    }
    void add(const Vector * const vector) {
        this->add(*vector);
    }
};


inline Vector operator+(Vector lhs, const Vector &rhs) {
    lhs += rhs;
    return lhs;
}
