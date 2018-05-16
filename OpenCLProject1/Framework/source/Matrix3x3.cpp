#include "Matrix3x3.hpp"
#include <sstream>

/**
	Element constructor.
	Initializes all elements of the matrix with the passed values.

	@param	_x0y0	The value for the x = 0 and y = 0 position.
	@param	_x1y0	The value for the x = 1 and y = 0 position.
	@param	_x2y0	The value for the x = 2 and y = 0 position.
	@param	_x0y1	The value for the x = 0 and y = 1 position.
	@param	_x1y1	The value for the x = 1 and y = 1 position.
	@param	_x2y1	The value for the x = 2 and y = 1 position.
	@param	_x0y2	The value for the x = 0 and y = 2 position.
	@param	_x1y2	The value for the x = 1 and y = 2 position.
	@param	_x2y2	The value for the x = 2 and y = 2 position.
*/
Matrix3x3::Matrix3x3( GLfloat const & _x0y0, GLfloat const & _x1y0, GLfloat const & _x2y0,
					  GLfloat const & _x0y1, GLfloat const & _x1y1, GLfloat const & _x2y1,
					  GLfloat const & _x0y2, GLfloat const & _x1y2, GLfloat const & _x2y2 )
{
	m_elements[ 0 ] = _x0y0;
	m_elements[ 3 ] = _x0y1;
	m_elements[ 6 ] = _x0y2;
	m_elements[ 1 ] = _x1y0;
	m_elements[ 4 ] = _x1y1;
	m_elements[ 7 ] = _x1y2;
	m_elements[ 2 ] = _x2y0;
	m_elements[ 5 ] = _x2y1;
	m_elements[ 8 ] = _x2y2;
}

/**
	Destructor.
	Deallocates the elements of the matrix.
*/
Matrix3x3::~Matrix3x3()
{

}
/**
	Calculates the determinant of the matrix.

	@return	Returns the result.
*/
GLfloat Matrix3x3::determinant() const
{
	return x0y0() * x1y1() * x2y2() + x1y0() * x2y1() * x0y2() + x2y0() * x0y1() * x1y2() -
		x0y2() * x1y1() * x2y0() - x1y2() * x2y1() * x0y0() - x2y2() * x0y1() * x1y0();
}

/**
	@return	Returns the element at the position x = 0, y = 0.
*/
GLfloat Matrix3x3::x0y0() const
{
	return m_elements[ 0 ];
}

/**
	@return	Returns the element at the position x = 1, y = 0.
*/
GLfloat Matrix3x3::x1y0() const
{
	return m_elements[ 1 ];
}

/**
	@return	Returns the element at the position x = 2, y = 0.
*/
GLfloat Matrix3x3::x2y0() const
{
	return m_elements[ 2 ];
}

/**
	@return	Returns the element at the position x = 0, y = 1.
*/
GLfloat Matrix3x3::x0y1() const
{
	return m_elements[ 3 ];
}

/**
	@return	Returns the element at the position x = 1, y = 1.
*/
GLfloat Matrix3x3::x1y1() const
{
	return m_elements[ 4 ];
}

/**
	@return	Returns the element at the position x = 2, y = 1.
*/
GLfloat Matrix3x3::x2y1() const
{
	return m_elements[ 5 ];
}

/**
	@return	Returns the element at the position x = 0, y = 2.
*/
GLfloat Matrix3x3::x0y2() const
{
	return m_elements[ 6 ];
}

/**
	@return	Returns the element at the position x = 1, y = 2.
*/
GLfloat Matrix3x3::x1y2() const
{
	return m_elements[ 7 ];
}

/**
	@return	Returns the element at the position x = 2, y = 2.
*/
GLfloat Matrix3x3::x2y2() const
{
	return m_elements[ 8 ];
}

/**
	Calculates the element index for the passed x and y position.

	@param	_x	The x index of the needed element.
	@param	_y	The y index of the needed element.
	@return		The array index to access the element.
*/
GLuint Matrix3x3::toArrayIndex( GLuint const & _x, GLuint const & _y ) const
{
	return _y * 3 + _x;
}