#include "Matrix4x4.hpp"
#include "Vector4.hpp"
#include <sstream>
#include "OpenGLFrameworkException.hpp"
#include "Matrix3x3.hpp"
#include "Quaternion.hpp"
#include "DegreeAngle.hpp"

/**
	@return Returns the identity matrix.
*/
Matrix4x4 Matrix4x4::identity()
{
	return Matrix4x4();
}

/**
	Creates a translation matrix.
	The passed three values are the translations of the
	x, y and z directions.

	@param	_x	The translation in x direction.
	@param	_y	The translation in y direction.
	@param	_z	The translation in z direction.
	@return		Returns the result.
*/
Matrix4x4 Matrix4x4::translate( GLfloat const & _x,
								GLfloat const & _y,
								GLfloat const & _z )
{
	return Matrix4x4( 1.0f, 0.0f, 0.0f, _x,
					  0.0f, 1.0f, 0.0f, _y,
					  0.0f, 0.0f, 1.0f, _z,
					  0.0f, 0.0f, 0.0f, 1.0f );
}

/**
	Creates a rotation matrix using a quaternion.
	The quaternion uses the passed rotation axis and the angle.

	@param _rotationAxis The rotation axis.
	@param _angle The angle to rotate around the axis.
	@return Returns the rotation matrix.
*/
Matrix4x4 Matrix4x4::rotate( Vector4 const & _rotationAxis,
							 DegreeAngle const & _angle )
{
	Quaternion const quaternion( _rotationAxis, _angle );
	return quaternion.rotationMatrix();
}

/**
	Creates a scaling matrix.
	The passed values are the scaling factors for the
	x, y and z directions.

	@param	_xScale	The scaling in the x direction.
	@param	_yScale	The scaling in the y direction.
	@param	_zScale	The scaling in the z direction.
	@return			Returns the result.
*/
Matrix4x4 Matrix4x4::scale( GLfloat const & _xScale,
							GLfloat const & _yScale,
							GLfloat const & _zScale )
{
	return Matrix4x4( _xScale, 0.0f, 0.0f, 0.0f,
					  0.0f, _yScale, 0.0f, 0.0f,
					  0.0f, 0.0f, _zScale, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f );
}

/**
	Calculates a matrix for an orthographic camera projection.

	The method throws an OpenGLFrameworkException, if
	- _right <= _left
	- _top <= _bottom
	- _far <= _near

	@param	_left	The left clipping plane.
	@param	_right	The right clipping plane.
	@param	_bottom	The bottom clipping plane.
	@param	_top	The top clipping plane.
	@param	_near	The near clipping plane.
	@param	_far	The far clipping plane.
	@return			The orthogonal camera matrix.
*/
Matrix4x4 Matrix4x4::orthographic( GLfloat const & _left,
								   GLfloat const & _right,
								   GLfloat const & _bottom,
								   GLfloat const & _top,
								   GLfloat const & _near,
								   GLfloat const & _far )
{
	if( _right <= _left )
	{
		std::stringstream stream;
		stream << "Right clipping plane ( " << _right << " ) is <= ";
		stream << "left clipping plane ( " << _left << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _top <= _bottom )
	{
		std::stringstream stream;
		stream << "Top clipping plane ( " << _top << " ) is <= ";
		stream << "bottom clipping plane ( " << _bottom << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _far <= _near )
	{
		std::stringstream stream;
		stream << "Far clipping plane ( " << _far << " ) is <= ";
		stream << "near clipping plane ( " << _near << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else
	{
		GLfloat tx = -( _right + _left ) / ( _right - _left );
		GLfloat ty = -( _top + _bottom ) / ( _top - _bottom );
		GLfloat tz = -( _far + _near ) / ( _far - _near );

		GLfloat x0y0 = 2.0f / ( _right - _left );
		GLfloat x1y1 = 2.0f / ( _top - _bottom );
		GLfloat x2y2 = -2.0f / ( _far - _near );

		return Matrix4x4( x0y0, 0.0f, 0.0f, tx,
						  0.0f, x1y1, 0.0f, ty,
						  0.0f, 0.0f, x2y2, tz,
						  0.0f, 0.0f, 0.0f, 1.0f );
	}
}

/**
	Calculates a perspective projection matrix.

	The near and far clipping distance should always be positive.
	Additionally the near clipping distance must be > 0 and the near and far
	clipping values can not be swapped.
	Also the aspect must not be 0.

	The method throws an InvalidArgumentException, if
	- _aspect == 0.0f
	- _near < 0.0f
	- _far <= _near

	@param	_fieldOfView	Specifies the field of view angle, in degrees, in the y direction.
	@param	_aspect			The aspect ratio of the viewport width to height.
	@param	_near			The distance to the near clipping plane from the eye point.
	@param	_far			The distance to the far clipping plane from the eye point.
	@return					The result matrix.
*/
Matrix4x4 Matrix4x4::perspective( DegreeAngle const & _fieldOfView,
								  GLfloat const & _aspect,
								  GLfloat const & _near,
								  GLfloat const & _far )
{
	if( 0.0f == _aspect )
	{
		throw OpenGLFrameworkException( "The aspect ratio is 0.0f." );
	}

	else if( 0.0f > _near )
	{
		std::stringstream stream;
		stream << "Near clipping plane ( " << _near << " ) is negative.";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _far <= _near )
	{
		std::stringstream stream;
		stream << "Far clipping plane ( " << _far << " ) is <= ";
		stream << "near clipping plane ( " << _near << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else
	{
		GLfloat top = _near * std::tan( 0.5f * _fieldOfView.toRadian() );
		GLfloat right = top * _aspect;
		return frustum( -right, right, -top, top, _near, _far );
	}
}

/**
	Calculates a matrix for a camera view direction.
	Therefore the eye point of the camera is needed,
	the view direction and the up direction.

	The vectors doesn't need to be normalized.

	The method throws an InvalidArgumentException, if
	the view and up direction are linearly dependent.

	@param	_eye			The eye point of the camera.
	@param	_viewDirection	The view direction of the camera.
	@param	_upDirection	The up direction of the camera.
	@return					The result matrix.
*/
Matrix4x4 Matrix4x4::lookAt( Vector4 const & _eye,
							 Vector4 const & _viewDirection,
							 Vector4 const & _upDirection )
{
	Vector4 view = _viewDirection.normalize();
	Vector4 up = _upDirection.normalize();

	Vector4 horizontal = view.cross( up ).normalize();
	Vector4 orthogonalUp = horizontal.cross( view ).normalize();

	Matrix4x4 lookAt( horizontal.x(), horizontal.y(), horizontal.z(), 0.0f,
					  orthogonalUp.x(), orthogonalUp.y(), orthogonalUp.z(), 0.0f,
					  -view.x(), -view.y(), -view.z(), 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f );

	Matrix4x4 translate = Matrix4x4::translate( -_eye.x(), -_eye.y(), -_eye.z() );

	return lookAt * translate;
}

/**
	Describes a perspective matrix that produces a perspective projection.
	Alternative to the perspective(...) method.

	Throws an InvalidArgumentException, if
	- _right <= _left
	- _top <= _bottom
	- _far <= _near

	@param	_left	The left clipping plane on the near clipping plane.
	@param	_right	The right clipping plane on the near clipping plane.
	@param	_bottom	The bottom clipping plane on the near clipping plane.
	@param	_top	The top clipping plane on the near clipping plane.
	@param	_near	The distance from the eye point to the near clipping plane.
	@param	_far	The distance from the eye point to the far clipping plane.
	@return			Returns the created perspective projection matrix.
*/
Matrix4x4 Matrix4x4::frustum( GLfloat const & _left,
							  GLfloat const & _right,
							  GLfloat const & _bottom,
							  GLfloat const & _top,
							  GLfloat const & _near,
							  GLfloat const & _far )
{
	if( _right <= _left )
	{
		std::stringstream stream;
		stream << "Right clipping plane ( " << _right << " ) is <= ";
		stream << "left clipping plane ( " << _left << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _top <= _bottom )
	{
		std::stringstream stream;
		stream << "Top clipping plane ( " << _top << " ) is <= ";
		stream << "bottom clipping plane ( " << _bottom << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _far <= _near )
	{
		std::stringstream stream;
		stream << "Far clipping plane ( " << _far << " ) is <= ";
		stream << "near clipping plane ( " << _near << " ).";
		throw OpenGLFrameworkException( stream.str() );
	}

	else
	{
		GLfloat x0y0 = 2.0f * _near / ( _right - _left );
		GLfloat x1y1 = 2.0f * _near / ( _top - _bottom );

		GLfloat x2y0 = ( _right + _left ) / ( _right - _left );
		GLfloat x2y1 = ( _top + _bottom ) / ( _top - _bottom );
		GLfloat x2y2 = -( _far + _near ) / ( _far - _near );
		GLfloat x2y3 = -1.0f;

		GLfloat x3y2 = -( 2.0f * _far * _near ) / ( _far - _near );

		return Matrix4x4( x0y0, 0.0f, x2y0, 0.0f,
						  0.0f, x1y1, x2y1, 0.0f,
						  0.0f, 0.0f, x2y2, x3y2,
						  0.0f, 0.0f, x2y3, 0.0f );
	}
}

/**
	Default constructor.
	Initializes an identity matrix.
*/
Matrix4x4::Matrix4x4()
{
	for( int x = 0; x < 4; x++ )
	{
		for( int y = 0; y < 4; y++ )
		{
			GLuint index = toArrayIndex( x, y );
			if( x == y )
			{
				m_elements[ index ] = 1.0f;
			}

			else
			{
				m_elements[ index ] = 0.0f;
			}
		}
	}
}

/**
	Data constructor.
	Initializes each element in the matrix with the passed
	elements.

	@param	_x0y0	The value for the x = 0 and y = 0 position.
	@param	_x1y0	The value for the x = 1 and y = 0 position.
	@param	_x2y0	The value for the x = 2 and y = 0 position.
	@param	_x3y0	The value for the x = 3 and y = 0 position.
	@param	_x0y1	The value for the x = 0 and y = 1 position.
	@param	_x1y1	The value for the x = 1 and y = 1 position.
	@param	_x2y1	The value for the x = 2 and y = 1 position.
	@param	_x3y1	The value for the x = 3 and y = 1 position.
	@param	_x0y2	The value for the x = 0 and y = 2 position.
	@param	_x1y2	The value for the x = 1 and y = 2 position.
	@param	_x2y2	The value for the x = 2 and y = 2 position.
	@param	_x3y2	The value for the x = 3 and y = 2 position.
	@param	_x0y3	The value for the x = 0 and y = 3 position.
	@param	_x1y3	The value for the x = 1 and y = 3 position.
	@param	_x2y3	The value for the x = 2 and y = 3 position.
	@param	_x3y3	The value for the x = 3 and y = 3 position.
*/
Matrix4x4::Matrix4x4( GLfloat const & _x0y0, GLfloat const & _x1y0, GLfloat const & _x2y0, GLfloat const & _x3y0,
					  GLfloat const & _x0y1, GLfloat const & _x1y1, GLfloat const & _x2y1, GLfloat const & _x3y1,
					  GLfloat const & _x0y2, GLfloat const & _x1y2, GLfloat const & _x2y2, GLfloat const & _x3y2,
					  GLfloat const & _x0y3, GLfloat const & _x1y3, GLfloat const & _x2y3, GLfloat const & _x3y3 )
{
	m_elements[ 0 ] = _x0y0;
	m_elements[ 4 ] = _x0y1;
	m_elements[ 8 ] = _x0y2;
	m_elements[ 12 ] = _x0y3;
	m_elements[ 1 ] = _x1y0;
	m_elements[ 5 ] = _x1y1;
	m_elements[ 9 ] = _x1y2;
	m_elements[ 13 ] = _x1y3;
	m_elements[ 2 ] = _x2y0;
	m_elements[ 6 ] = _x2y1;
	m_elements[ 10 ] = _x2y2;
	m_elements[ 14 ] = _x2y3;
	m_elements[ 3 ] = _x3y0;
	m_elements[ 7 ] = _x3y1;
	m_elements[ 11 ] = _x3y2;
	m_elements[ 15 ] = _x3y3;
}

/**
Destructor.
Deallocates the elements of this matrix.
*/
Matrix4x4::~Matrix4x4()
{

}

/**
	Multiplication operator.
	Multiplies the passed matrix with this matrix and returns
	the result.

	@param	_matrix	The right hand matrix of the multiplication.
	@return			Returns the result matrix.
*/
Matrix4x4 Matrix4x4::operator*( Matrix4x4 const & _matrix ) const
{
	GLfloat newElements[ 16 ];

	for( GLuint y = 0; y < height(); y++ )
	{
		for( GLuint x = 0; x < width(); x++ )
		{
			GLuint index = toArrayIndex( x, y );
			newElements[ index ] = 0.0f;

			for( GLuint x2 = 0; x2 < width(); x2++ )
			{
				newElements[ index ] += get( x2, y ) * _matrix.get( x, x2 );
			}
		}
	}

	return Matrix4x4( newElements[ 0 ], newElements[ 1 ], newElements[ 2 ], newElements[ 3 ],
					  newElements[ 4 ], newElements[ 5 ], newElements[ 6 ], newElements[ 7 ],
					  newElements[ 8 ], newElements[ 9 ], newElements[ 10 ], newElements[ 11 ],
					  newElements[ 12 ], newElements[ 13 ], newElements[ 14 ], newElements[ 15 ] );
}

/**
	Multiplication operator.
	Multiplies the passed vector with this matrix and returns
	the result.

	@param	_vector	The vector of the multiplication.
	@return			The result vector.
*/
Vector4 Matrix4x4::operator*( Vector4 const & _vector ) const
{
	GLfloat newElements[ 4 ];

	for( GLuint y = 0; y < height(); y++ )
	{
		newElements[ y ] = 0.0f;

		for( GLuint x = 0; x < width(); x++ )
		{
			newElements[ y ] += get( x, y ) * _vector[ x ];
		}
	}

	return Vector4( newElements[ 0 ], newElements[ 1 ], newElements[ 2 ], newElements[ 3 ] );
}

/**
	Calculates the determinant of the matrix.

	@return	Returns the result.
*/
GLfloat Matrix4x4::determinant() const
{
	GLfloat d1 = ( x1y1() * x2y2() * x3y3() ) + ( x2y1() * x3y2() * x1y3() ) + ( x3y1() * x1y2() * x2y3() ) -
		( ( x3y1() * x2y2() * x1y3() ) + ( x1y1() * x3y2() * x2y3() ) + ( x2y1() * x1y2() * x3y3() ) );

	GLfloat d2 = ( x0y1() * x2y2() * x3y3() ) + ( x2y1() * x3y2() * x0y3() ) + ( x3y1() * x0y2() * x2y3() ) -
		( ( x3y1() * x2y2() * x0y3() ) + ( x0y1() * x3y2() * x2y3() ) + ( x2y1() * x0y2() * x3y3() ) );

	GLfloat d3 = ( x0y1() * x1y2() * x3y3() ) + ( x1y1() * x3y2() * x0y3() ) + ( x3y1() * x0y2() * x1y3() ) -
		( ( x3y1() * x1y2() * x0y3() ) + ( x0y1() * x3y2() * x1y3() ) + ( x1y1() * x0y2() * x3y3() ) );

	GLfloat d4 = ( x0y1() * x1y2() * x2y3() ) + ( x1y1() * x2y2() * x0y3() ) + ( x2y1() * x0y2() * x1y3() ) -
		( ( x2y1() * x1y2() * x0y3() ) + ( x0y1() * x2y2() * x1y3() ) + ( x1y1() * x0y2() * x2y3() ) );


	return x0y0() * d1 - x1y0() * d2 + x2y0() * d3 - x3y0() * d4;
}

/**
	Calculates the inverse of the matrix
	and returns the result.

	@return	The inverse of this matrix.
*/
Matrix4x4 Matrix4x4::inverse() const
{
	GLfloat d = determinant();

	if( 0.0f == d )
	{
		d = 1.0f;
	}

	Matrix3x3 matrix00( x1y1(), x2y1(), x3y1(), x1y2(), x2y2(), x3y2(), x1y3(), x2y3(), x3y3() );
	Matrix3x3 matrix10( x0y1(), x2y1(), x3y1(), x0y2(), x2y2(), x3y2(), x0y3(), x2y3(), x3y3() );
	Matrix3x3 matrix20( x0y1(), x1y1(), x3y1(), x0y2(), x1y2(), x3y2(), x0y3(), x1y3(), x3y3() );
	Matrix3x3 matrix30( x0y1(), x1y1(), x2y1(), x0y2(), x1y2(), x2y2(), x0y3(), x1y3(), x2y3() );

	Matrix3x3 matrix01( x1y0(), x2y0(), x3y0(), x1y2(), x2y2(), x3y2(), x1y3(), x2y3(), x3y3() );
	Matrix3x3 matrix11( x0y0(), x2y0(), x3y0(), x0y2(), x2y2(), x3y2(), x0y3(), x2y3(), x3y3() );
	Matrix3x3 matrix21( x0y0(), x1y0(), x3y0(), x0y2(), x1y2(), x3y2(), x0y3(), x1y3(), x3y3() );
	Matrix3x3 matrix31( x0y0(), x1y0(), x2y0(), x0y2(), x1y2(), x2y2(), x0y3(), x1y3(), x2y3() );

	Matrix3x3 matrix02( x1y0(), x2y0(), x3y0(), x1y1(), x2y1(), x3y1(), x1y3(), x2y3(), x3y3() );
	Matrix3x3 matrix12( x0y0(), x2y0(), x3y0(), x0y1(), x2y1(), x3y1(), x0y3(), x2y3(), x3y3() );
	Matrix3x3 matrix22( x0y0(), x1y0(), x3y0(), x0y1(), x1y1(), x3y1(), x0y3(), x1y3(), x3y3() );
	Matrix3x3 matrix32( x0y0(), x1y0(), x2y0(), x0y1(), x1y1(), x2y1(), x0y3(), x1y3(), x2y3() );

	Matrix3x3 matrix03( x1y0(), x2y0(), x3y0(), x1y1(), x2y1(), x3y1(), x1y2(), x2y2(), x3y2() );
	Matrix3x3 matrix13( x0y0(), x2y0(), x3y0(), x0y1(), x2y1(), x3y1(), x0y2(), x2y2(), x3y2() );
	Matrix3x3 matrix23( x0y0(), x1y0(), x3y0(), x0y1(), x1y1(), x3y1(), x0y2(), x1y2(), x3y2() );
	Matrix3x3 matrix33( x0y0(), x1y0(), x2y0(), x0y1(), x1y1(), x2y1(), x0y2(), x1y2(), x2y2() );

	return Matrix4x4( matrix00.determinant() / d, -matrix01.determinant() / d, matrix02.determinant() / d, -matrix03.determinant() / d,
					  -matrix10.determinant() / d, matrix11.determinant() / d, -matrix12.determinant() / d, matrix13.determinant() / d,
					  matrix20.determinant() / d, -matrix21.determinant() / d, matrix22.determinant() / d, -matrix23.determinant() / d,
					  -matrix30.determinant() / d, matrix31.determinant() / d, -matrix32.determinant() / d, matrix33.determinant() / d );
}

/**
	Calculates the transposed matrix and returns the result.

	@return	The transposed matrix of this matrix.
*/
Matrix4x4 Matrix4x4::transpose() const
{
	return Matrix4x4( x0y0(), x0y1(), x0y2(), x0y3(),
					  x1y0(), x1y1(), x1y2(), x1y3(),
					  x2y0(), x2y1(), x2y2(), x2y3(),
					  x3y0(), x3y1(), x3y2(), x3y3() );
}

/**
	Returns the element at the specified x and y coordinate.

	If x is >= width() or y is >= height() the method throws
	an OpenGLFrameworkException.

	@param	_x	The index of the needed col.
	@param	_y	The index of the needed row.
	@return		The needed element.
*/
GLfloat Matrix4x4::get( GLuint const & _x, GLuint const & _y ) const
{
	if( _x >= width() )
	{
		std::stringstream stream;
		stream << "GLuint x ( " << _x << " ) is out of range [ 0, ";
		stream << width() << " ].";
		throw OpenGLFrameworkException( stream.str() );
	}

	else if( _y >= height() )
	{
		std::stringstream stream;
		stream << "GLuint y ( " << _y << " ) is out of range [ 0, ";
		stream << height() << " ].";
		throw OpenGLFrameworkException( stream.str() );
	}

	else
	{
		GLuint index = toArrayIndex( _x, _y );
		return m_elements[ index ];
	}
}

GLfloat & Matrix4x4::getData()
{
	return m_elements[ 0 ];
}

/**
	@return		Returns the width of the matrix.
*/
GLuint Matrix4x4::width() const
{
	return 4;
}

/**
	@return		Returns the height of the matrix.
*/
GLuint Matrix4x4::height() const
{
	return 4;
}

/**
	Calculates the element index for the passed x and y position.

	@param	_x	The x index of the needed element.
	@param	_y	The y index of the needed element.
	@return		The array index to access the element.
*/
GLuint Matrix4x4::toArrayIndex( GLuint const & _x, GLuint const & _y ) const
{
	return _y * height() + _x;
}

/**
	@return	Returns the value at position x = 0 and y = 0.
*/
GLfloat Matrix4x4::x0y0() const
{
	return m_elements[ 0 ];
}

/**
	@return	Returns the value at position x = 1 and y = 0.
*/
GLfloat Matrix4x4::x1y0() const
{
	return m_elements[ 1 ];
}

/**
	@return	Returns the value at position x = 2 and y = 0.
*/
GLfloat Matrix4x4::x2y0() const
{
	return m_elements[ 2 ];
}

/**
	@return	Returns the value at position x = 3 and y = 0.
*/
GLfloat Matrix4x4::x3y0() const
{
	return m_elements[ 3 ];
}

/**
	@return	Returns the value at position x = 0 and y = 1.
*/
GLfloat Matrix4x4::x0y1() const
{
	return m_elements[ 4 ];
}

/**
	@return	Returns the value at position x = 1 and y = 1.
*/
GLfloat Matrix4x4::x1y1() const
{
	return m_elements[ 5 ];
}

/**
	@return	Returns the value at position x = 2 and y = 1.
*/
GLfloat Matrix4x4::x2y1() const
{
	return m_elements[ 6 ];
}

/**
	@return	Returns the value at position x = 3 and y = 1.
*/
GLfloat Matrix4x4::x3y1() const
{
	return m_elements[ 7 ];
}

/**
	@return	Returns the value at position x = 0 and y = 2.
*/
GLfloat Matrix4x4::x0y2() const
{
	return m_elements[ 8 ];
}

/**
	@return	Returns the value at position x = 1 and y = 2.
*/
GLfloat Matrix4x4::x1y2() const
{
	return m_elements[ 9 ];
}

/**
	@return	Returns the value at position x = 2 and y = 2.
*/
GLfloat Matrix4x4::x2y2() const
{
	return m_elements[ 10 ];
}

/**
	@return	Returns the value at position x = 3 and y = 2.
*/
GLfloat Matrix4x4::x3y2() const
{
	return m_elements[ 11 ];
}

/**
	@return	Returns the value at position x = 0 and y = 3.
*/
GLfloat Matrix4x4::x0y3() const
{
	return m_elements[ 12 ];
}

/**
	@return	Returns the value at position x = 1 and y = 3.
*/
GLfloat Matrix4x4::x1y3() const
{
	return m_elements[ 13 ];
}

/**
	@return	Returns the value at position x = 2 and y = 3.
*/
GLfloat Matrix4x4::x2y3() const
{
	return m_elements[ 14 ];
}

/**
	@return	Returns the value at position x = 3 and y = 3.
*/
GLfloat Matrix4x4::x3y3() const
{
	return m_elements[ 15 ];
}