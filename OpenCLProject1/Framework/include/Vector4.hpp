#ifndef BAS_VECTOR4_H
#define BAS_VECTOR4_H

#include <glew.h>

/**
	Represents a vector in 4D-space with x, y, z and w directions.
	Contains methods to handle different vector properties.
*/
class Vector4
{
public:
	Vector4( GLfloat const & _x, GLfloat const & _y, GLfloat const & _z, GLfloat const & _w );

	void operator=( Vector4 const & _vector );
	Vector4 operator+( Vector4 const & _vector ) const;
	Vector4 operator-( Vector4 const & _vector ) const;
	Vector4 operator*( GLfloat const & _scalar ) const;
	GLfloat operator*( Vector4 const & _vector ) const;
	GLfloat operator[]( GLuint const & _index ) const;

	GLfloat length() const;
	Vector4 normalize() const;

	Vector4 cross( Vector4 const & _vector ) const;

	GLfloat x() const;
	GLfloat y() const;
	GLfloat z() const;
	GLfloat w() const;

private:
	GLfloat m_x;	//!< The x direction of the vector.
	GLfloat m_y;	//!< The y direction of the vector.
	GLfloat m_z;	//!< The z direction of the vector.
	GLfloat m_w;	//!< The w direction of the vector.
};

#endif