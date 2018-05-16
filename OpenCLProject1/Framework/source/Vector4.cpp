#include "Vector4.hpp"
#include <sstream>
#include "OpenGLFrameworkException.hpp"

/**
	Constructor.
	Initializes each direction of the vector with the
	passed values.

	@param	_x	The value for the x direction.
	@param	_y	The value for the y direction.
	@param	_z	The value for the z direction.
	@param	_w	The value for the w direction.
*/
Vector4::Vector4( GLfloat const & _x, GLfloat const & _y, GLfloat const & _z, GLfloat const & _w ) :
	m_x( _x ),
	m_y( _y ),
	m_z( _z ),
	m_w( _w )
{

}

/**
	Assignment-operator.
	Copies the values of the passed vector to this vector.

	@param	_vector	The vector to copy.
*/
void Vector4::operator=( Vector4 const & _vector )
{
	m_x = _vector.x();
	m_y = _vector.y();
	m_z = _vector.z();
	m_w = _vector.w();
}

/**
	Translates this vector by the passed vector.

	The method only translates points with w = 1.

	@param _vector The translation vector.
	@return Returns the translated vector.
*/
Vector4 Vector4::operator+( Vector4 const & _vector ) const
{
	if( 1.0f == w() && 0.0f == _vector.w() )
	{
		return Vector4( x() + _vector.x(), y() + _vector.y(), z() + _vector.z(), 1.0f );
	}

	else
	{
		return *this;
	}
}

/**
	Minus-operator.
	Subtracts the passed vector from this vector and
	returns the result.

	@param	_vector	The vector to subtract.
	@return			The subtraction result.
*/
Vector4 Vector4::operator-( Vector4 const & _vector ) const
{
	GLfloat newX = x() - _vector.x();
	GLfloat newY = y() - _vector.y();
	GLfloat newZ = z() - _vector.z();
	GLfloat newW = w() - _vector.w();
	return Vector4( newX, newY, newZ, newW );
}

/**
	Multiplication-operator.
	Calculates a scalar multiplication by
	multiplying this vector with the passed scalar value.

	@param	_scalar	The scalar value for the calculation.
*/
Vector4 Vector4::operator*( GLfloat const & _scalar ) const
{
	GLfloat newX = x() * _scalar;
	GLfloat newY = y() * _scalar;
	GLfloat newZ = z() * _scalar;
	GLfloat newW = w() * _scalar;
	return Vector4( newX, newY, newZ, newW );
}

/**
	Multiplication-operator.
	Calculates the dot product with this and the passed vector.

	@param	_vector	The right hand vector of the dot product.
*/
GLfloat Vector4::operator*( Vector4 const & _vector ) const
{
	return x() * _vector.x() +
		   y() * _vector.y() +
		   z() * _vector.z() +
		   w() * _vector.w();
}

/**
	Brace operator.
	Returns the value of the needed dimension.
	For performance, use x(), y(), z() or w() method instead.

	If the index is not in range [ 0, 3 ], the method throws
	an OpenGLFrameworkException.

	@param	_index	The index of the needed direction.
	@return			Returns the value of the needed direction.
*/
GLfloat Vector4::operator[]( GLuint const & _index ) const
{
	switch( _index )
	{
	case 0:
		return x();

	case 1:
		return y();

	case 2:
		return z();

	case 3:
		return w();

	default:
		std::stringstream stream;
		stream << "Index ( " << _index << " ) is out of range [ 0, 3 ].";
		throw OpenGLFrameworkException( stream.str() );
	}
}

/**
	Returns the length of the vector.

	@return	The length of the vector.
*/
GLfloat Vector4::length() const
{
	return std::sqrt( x() * x() + y() * y() + z() * z() + w() * w() );
}

/**
	Normalizes the vector to length one and
	returns the result.

	@return		The normalized vector.
*/
Vector4 Vector4::normalize() const
{
	GLfloat currentLength = length();
	return Vector4( x() / currentLength,
					y() / currentLength,
					z() / currentLength,
					w() / currentLength );
}

/**
	Calculates the 3D vector product between this and the passed vector.
	The homogen coordinate is set to this w() coordinate.

	@param _vector The right-hand vector.
	@return Returns the result.
*/
Vector4 Vector4::cross( Vector4 const & _vector ) const
{
	GLfloat newX = y() * _vector.z() - z() * _vector.y();
	GLfloat newY = z() * _vector.x() - x() * _vector.z();
	GLfloat newZ = x() * _vector.y() - y() * _vector.x();
	return Vector4( newX, newY, newZ, w() );
}

/**
@return	Returns the x direction of the vector.
*/
GLfloat Vector4::x() const
{
	return m_x;
}

/**
@return	Returns the x direction of the vector.
*/
GLfloat Vector4::y() const
{
	return m_y;
}

/**
@return	Returns the x direction of the vector.
*/
GLfloat Vector4::z() const
{
	return m_z;
}

/**
@return	Returns the x direction of the vector.
*/
GLfloat Vector4::w() const
{
	return m_w;
}
