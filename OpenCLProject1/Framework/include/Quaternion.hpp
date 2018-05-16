#ifndef BAS_QUATERNION_H
#define BAS_QUATERNION_H

#include <glew.h>

class RadianAngle;
class DegreeAngle;
class Matrix4x4;
class Vector4;

/**
	A quaternion is used here to represent rotation
	of objects without large rounding errors.
	This class is only for rotations.
*/
class Quaternion
{
public:
	Quaternion();
	Quaternion( GLfloat const & _a, GLfloat const & _b,
		GLfloat const & _c, GLfloat const & _d );
	Quaternion( Vector4 const & _axis, DegreeAngle const & _angle );
	Quaternion( GLfloat const & _real, Vector4 const & _imaginary );
	Quaternion( Quaternion const & _quaternion );

	void operator=( Quaternion const & _quaternion );
	Quaternion operator*( Quaternion const & _quaternion ) const;

	GLfloat length() const;
	Quaternion normalize() const;
	Quaternion conjugate() const;
	Quaternion invert() const;
	Matrix4x4 rotationMatrix() const;
	Vector4 rotate( Vector4 const & _vector ) const;

	GLfloat a() const;
	GLfloat b() const;
	GLfloat c() const;
	GLfloat d() const;
	Vector4 imaginaryPart() const;

private:
	GLfloat m_a;	//!< The scalar value of the quaternion. The real part.
	GLfloat m_b;	//!< The first imaginary part.
	GLfloat m_c;	//!< The second imaginary part.
	GLfloat m_d;	//!< The third imaginary part.
};

#endif