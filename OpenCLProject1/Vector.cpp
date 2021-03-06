#include "Vector.h"


Vector& Vector::operator+=(const Vector& vector) {
    this->add(vector);
    return *this;
}


Vector& Vector::operator-=(const Vector& vector) {
    this->x -= vector.getX();
    this->y -= vector.getY();
    this->z -= vector.getZ();
    return *this;
}

Vector& Vector::operator*=(const float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

float Vector::operator*(const Vector& vector) {
	return this->x * vector.getX() + this->y * vector.getY() + this->z * vector.getZ();
}

bool Vector::operator!=(const Vector &other) {
    return !(*this == other);
}

bool Vector::operator==(const Vector& other) {
    return x == other.getX() && y == other.getY() && z == other.getZ();
}


float Vector::getX() const {
    return x;
}

float Vector::getY() const {
    return y;
}

float Vector::getZ() const {
    return z;
}

cl_float3 Vector::get() const {
    cl_float3 value = {x,y,z};
    return value;
}

float Vector::length2() const {
    return pow(x,2) + pow(y,2) + pow(z,2);
}
float Vector::length() const {
    return sqrt(length2());
}

void Vector::normalize() {
    const float length = this->length();
    this->x /= length;
    this->y /= length;
    this->z /= length;
}

void Vector::add(const Vector vector) {
    this->x += vector.getX();
    this->y += vector.getY();
    this->z += vector.getZ();
}

void Vector::add(const Vector * const vector) {
    this->add(*vector);
}

void Vector::add(const float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
}

void Vector::subtract(const float scalar) {
    add(-scalar);
}

cl_Vector Vector::getClData() const {
    cl_float3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}
