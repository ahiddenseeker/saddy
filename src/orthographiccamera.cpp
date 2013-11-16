#include "orthographiccamera.h"
#include "renderer.h"
#include "log/log.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>														
#include <GL/glu.h>

sad::OrthographicCamera::OrthographicCamera()
: m_fetched(false), m_width(1), m_height(1)
{

}

sad::OrthographicCamera::OrthographicCamera(int width, int height)
: m_width(width), m_height(height), m_fetched(true)
{

}

void sad::OrthographicCamera::apply()
{
	if (!m_fetched)
	{
		m_fetched = true;
		m_width = Scene->renderer()->settings().width();
		m_height = Scene->renderer()->settings().height();
	}

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,m_width,0,m_height);
	glPopAttrib();

	this->sad::Camera::apply();
}

sad::OrthographicCamera::~OrthographicCamera()
{
}
