#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include <glew.h>

class Vector3;

/**
	Represents a square matrix in three dimensional space.
*/
class Matrix3x3
{
public:
	Matrix3x3( GLfloat const & _x0y0, GLfloat const & _x1y0, GLfloat const & _x2y0,
			   GLfloat const & _x0y1, GLfloat const & _x1y1, GLfloat const & _x2y1,
			   GLfloat const & _x0y2, GLfloat const & _x1y2, GLfloat const & _x2y2 );
	~Matrix3x3();

	GLfloat determinant() const;

	GLfloat x0y0() const;
	GLfloat x1y0() const;
	GLfloat x2y0() const;

	GLfloat x0y1() const;
	GLfloat x1y1() const;
	GLfloat x2y1() const;

	GLfloat x0y2() const;
	GLfloat x1y2() const;
	GLfloat x2y2() const;

private:
	GLuint toArrayIndex( GLuint const & _x, GLuint const & _y ) const;

	GLfloat m_elements[ 9 ];	//!< Contains the elements of the matrix.
};

#endif