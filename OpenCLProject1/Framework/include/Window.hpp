#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glew.h>
#include <glfw3.h>
#include <string>
#include "Camera.hpp"

/**
	Implementation for a graphical context representing a
	whole window.
*/
class Window
{
public:
	static Window * getInstance();

	~Window();

	void open( std::string const & _title,
			   GLuint const & _pixelWidth,
			   GLuint const & _pixelHeight );
	GLboolean isOpen() const;
	void close();

	void setEyePoint( Vector4 const & _eyePoint );

	GLuint getWidth() const;
	GLuint getHeight() const;

	void setActive();
	void swapBuffer();

	Camera getCamera() const;

private:
	static void handleKeyEvent( GLFWwindow * _window,
								GLint _key,
								GLint _scancode,
								GLint _action,
								GLint _mods );
	static void handleResizeEvent( GLFWwindow * _window,
								   int _width,
								   int _height );
	static void handleFramebufferResizeEvent( GLFWwindow * _window,
											  int _width,
											  int _height );

	void handleKeyEvent( GLint const & _key,
						 GLint const & _action,
						 GLint const & _mods );
	void handleResizeEvent( GLuint const & _width,
							GLuint const & _height );
	void handleFramebufferResizeEvent( GLsizei const & _width,
									   GLsizei const & _height );

	Window();

	static Window * m_instance;	//!< The window instance.
	Camera m_camera;			//!< The camera instance.
	GLFWwindow * m_window;		//!< The glfw window instance. Can be null before initialization.
};

#endif