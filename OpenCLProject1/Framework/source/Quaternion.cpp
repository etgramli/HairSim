#include "Quaternion.hpp"
#include "Matrix4x4.hpp"
#include "Vector4.hpp"
#include "DegreeAngle.hpp"
#include <algorithm>

/**
	Default constructor.
	Initializes the quaternion with a neutral
	rotation.
*/
Quaternion::Quaternion() :
	m_a( 0.0f ),
	m_b( 0.0f ),
	m_c( 0.0f ),
	m_d( 0.0f )
{

}

/**
	Constructor.
	Initializes all four values.

	@param	_a	First quaternion value.
	@param	_b	First quaternion value.
	@param	_c	First quaternion value.
	@param	_d	First quaternion value.
*/
Quaternion::Quaternion( GLfloat const & _a, GLfloat const & _b,
						GLfloat const & _c, GLfloat const & _d ) :
	m_a( _a ),
	m_b( _b ),
	m_c( _c ),
	m_d( _d )
{

}

/**
	Rotation constructor.
	Initializes the quaternion with a rotation defined by
	the passed rotation axis and the passed degree angle.

	@param	_axis	The axis to rotate around.
	@param	_angle	The degree angle to rotate.
*/
Quaternion::Quaternion( Vector4 const & _axis, DegreeAngle const & _angle ) :
	m_a( std::cos( _angle.toRadian() / 2.0f ) ),
	m_b( 0.0f ),
	m_c( 0.0f ),
	m_d( 0.0f )
{
	Vector4 norm = _axis.normalize();
	m_b = norm.x() * std::sin( _angle.toRadian() / 2.0f );
	m_c = norm.y() * std::sin( _angle.toRadian() / 2.0f );
	m_d = norm.z() * std::sin( _angle.toRadian() / 2.0f );
}

/**
	Constructor.
	Initializes the real part with the float value and each imaginary
	part with the vector components.

	@param	_real		The value for the real quaternion.
	@param	_imaginary	The vector with all imaginary parts.
*/
Quaternion::Quaternion( GLfloat const & _real, Vector4 const & _imaginary ) :
	m_a( _real ),
	m_b( _imaginary.x() ),
	m_c( _imaginary.y() ),
	m_d( _imaginary.z() )
{

}

/**
	Copy constructor.
	Copies the passed quaternion.

	@param	_quaternion	The quaternion to copy.
*/
Quaternion::Quaternion( Quaternion const & _quaternion ) :
	m_a( _quaternion.a() ),
	m_b( _quaternion.b() ),
	m_c( _quaternion.c() ),
	m_d( _quaternion.d() )
{

}

/**
	Assignment operator.
	Copies the passed quaternion.

	@param	_quaternion	The quaternion to copy.
*/
void Quaternion::operator=( Quaternion const & _quaternion )
{
	m_a = _quaternion.a();
	m_b = _quaternion.b();
	m_c = _quaternion.c();
	m_d = _quaternion.d();
}

/**
	Multiplication operator.
	Multiplies this quaternion with the passed one.

	@param	_quaternion	The quaternion to multiply with.
	@return				The result quaternion.
*/
Quaternion Quaternion::operator*( Quaternion const & _quaternion ) const
{
	GLfloat newA = a() * _quaternion.a() - b() * _quaternion.b() -
		c() * _quaternion.c() - d() * _quaternion.d();

	GLfloat newB = a() * _quaternion.b() + b() * _quaternion.a() +
		c() * _quaternion.d() - d() * _quaternion.c();

	GLfloat newC = a() * _quaternion.c() - b() * _quaternion.d() +
		c() * _quaternion.a() + d() * _quaternion.b();

	GLfloat newD = a() * _quaternion.d() + b() * _quaternion.c() -
		c() * _quaternion.b() + d() * _quaternion.a();

	return Quaternion( newA, newB, newC, newD );
}

/**
	@return	Returns the length of the quaternion.
*/
GLfloat Quaternion::length() const
{
	return std::sqrt( a() * a() + b() * b() + c() * c() + d() * d() );
}

/**
	Normalizes the quaternion by dividing each element with the
	length. Does not modify the current quaternion.
	Creates a new one.

	If the quaternion has a length of 0,
	this quaternion will be returned.

	@return		The normalized quaternion.
*/
Quaternion Quaternion::normalize() const
{
	if( 0.0f == length() )
	{
		return Quaternion( *this );
	}

	else
	{
		GLfloat norm = length();
		GLfloat newA = a() / norm;
		GLfloat newB = b() / norm;
		GLfloat newC = c() / norm;
		GLfloat newD = d() / norm;

		return Quaternion( newA, newB, newC, newD );
	}
}

/**
	Conjugates the quaternion by inverting the
	imaginary part of the quaternion.

	@return	Returns the result.
*/
Quaternion Quaternion::conjugate() const
{
	return Quaternion( a(), -b(), -c(), -d() );
}

/**
	Inverts the quaternion.
	The inverse quaternion is the normalized and conjugated
	quaternion.

	@return	Returns the result.
*/
Quaternion Quaternion::invert() const
{
	Quaternion normalized = normalize();
	return normalized.conjugate();
}

/**
	Returns the rotation matrix defined by the quaternion.

	@return	Rotation matrix.
*/
Matrix4x4 Quaternion::rotationMatrix() const
{
	Quaternion normalized = normalize();
	GLfloat a = normalized.a();
	GLfloat b = normalized.b();
	GLfloat c = normalized.c();
	GLfloat d = normalized.d();

	//Remember: aYX
	GLfloat a11 = 1.0f - 2.0f * ( c * c + d * d );
	GLfloat a12 = 2.0f * ( b * c - a * d );
	GLfloat a13 = 2.0f * ( b * d + a * c );

	GLfloat a21 = 2.0f * ( b * c + a * d );
	GLfloat a22 = 1.0f - 2.0f * ( d * d + b * b );
	GLfloat a23 = 2.0f * ( c * d - a * b );

	GLfloat a31 = 2.0f * ( b * d - a * c );
	GLfloat a32 = 2.0f * ( c * d + a * b );
	GLfloat a33 = 1.0f - 2.0f * ( b * b + c * c );

	return Matrix4x4( a11, a12, a13, 0.0f,
					  a21, a22, a23, 0.0f,
					  a31, a32, a33, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f );
}

/**
	Rotates the passed vector with this quaternion and
	returns the result.

	@param	_vector	The vector to rotate.
*/
Vector4 Quaternion::rotate( Vector4 const & _vector ) const
{
	Quaternion q = normalize();
	Quaternion vectorQuaternion( 0.0f, _vector );

	Quaternion result = q * vectorQuaternion * q.conjugate();

	return result.imaginaryPart();
}

/**
	@return	Returns the first quaternion value.
*/
GLfloat Quaternion::a() const
{
	return m_a;
}

/**
	@return	Returns the second quaternion value.
*/
GLfloat Quaternion::b() const
{
	return m_b;
}

/**
	@return	Returns the third quaternion value.
*/
GLfloat Quaternion::c() const
{
	return m_c;
}

/**
	@return	Returns the fourth quaternion value.
*/
GLfloat Quaternion::d() const
{
	return m_d;
}

/**
	@return	Returns the imaginary part of the quaternion.
*/
Vector4 Quaternion::imaginaryPart() const
{
	return Vector4( b(), c(), d(), 0.0f );
}