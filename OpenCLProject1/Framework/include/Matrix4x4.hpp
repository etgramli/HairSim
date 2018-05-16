#ifndef BAS_MATRIX_H
#define BAS_MATRIX_H

#include <glew.h>

class Vector4;
class DegreeAngle;

/**
	Represents a 4x4 matrix with float data type.
	Contains methods for matrix multiplication and
	to access single elements.
*/
class Matrix4x4
{
public:
	static Matrix4x4 identity();
	static Matrix4x4 translate( GLfloat const & _x,
								GLfloat const & _y,
								GLfloat const & _z );
	static Matrix4x4 rotate( Vector4 const & _rotationAxis,
							 DegreeAngle const & _angle );
	static Matrix4x4 scale( GLfloat const & _xScale,
							GLfloat const & _yScale,
							GLfloat const & _zScale );
	static Matrix4x4 orthographic( GLfloat const & _left,
								   GLfloat const & _right,
								   GLfloat const & _bottom,
								   GLfloat const & _top,
								   GLfloat const & _near,
								   GLfloat const & _far );
	static Matrix4x4 perspective( DegreeAngle const & _fieldOfView,
								  GLfloat const & _aspect,
								  GLfloat const & _near,
								  GLfloat const & _far );
	static Matrix4x4 lookAt( Vector4 const & _eye,
							 Vector4 const & _viewDirection,
							 Vector4 const & _upDirection );
	static Matrix4x4 frustum( GLfloat const & _left,
							  GLfloat const & _right,
							  GLfloat const & _bottom,
							  GLfloat const & _top,
							  GLfloat const & _near,
							  GLfloat const & _far );

	Matrix4x4();
	Matrix4x4( GLfloat const & _x0y0, GLfloat const & _x1y0, GLfloat const & _x2y0, GLfloat const & _x3y0,
			   GLfloat const & _x0y1, GLfloat const & _x1y1, GLfloat const & _x2y1, GLfloat const & _x3y1,
			   GLfloat const & _x0y2, GLfloat const & _x1y2, GLfloat const & _x2y2, GLfloat const & _x3y2,
			   GLfloat const & _x0y3, GLfloat const & _x1y3, GLfloat const & _x2y3, GLfloat const & _x3y3 );
	~Matrix4x4();

	Matrix4x4 operator*( Matrix4x4 const & _matrix ) const;
	Vector4 operator*( Vector4 const & _vector ) const;

	GLfloat determinant() const;
	Matrix4x4 inverse() const;
	Matrix4x4 transpose() const;

	GLfloat get( GLuint const & _x, GLuint const & _y ) const;
	GLfloat & getData();
	GLuint width() const;
	GLuint height() const;

private:
	GLfloat x0y0() const;
	GLfloat x1y0() const;
	GLfloat x2y0() const;
	GLfloat x3y0() const;

	GLfloat x0y1() const;
	GLfloat x1y1() const;
	GLfloat x2y1() const;
	GLfloat x3y1() const;

	GLfloat x0y2() const;
	GLfloat x1y2() const;
	GLfloat x2y2() const;
	GLfloat x3y2() const;

	GLfloat x0y3() const;
	GLfloat x1y3() const;
	GLfloat x2y3() const;
	GLfloat x3y3() const;

	GLuint toArrayIndex( GLuint const & _x, GLuint const & _y ) const;

	GLfloat m_elements[ 16 ];	//!< Contains the elements of the matrix.
};

#endif