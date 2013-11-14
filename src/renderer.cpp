#include "renderer.h"

#include "texturemanager.h"
#include "input.h"
#include "fontmanager.h"
#include "texturemanager.h"
#include "scene.h"
#include "window.h"
#include "glcontext.h"

sad::Renderer * sad::Renderer::m_instance = NULL;

sad::Renderer::Renderer()
: 
m_running(false),
m_fps(75),
m_input_manager(new sad::Input()),
m_font_manager(new sad::FontManager()),
m_texture_manager(new sad::TextureManager()),
m_scene(new sad::Scene()),
m_new_scene(NULL),
m_window(new sad::Window()),
m_context(new sad::GLContext())
{
	m_scene->setRenderer(this);
	m_window->setRenderer(this);
}

sad::Renderer::~Renderer(void)
{
	if (m_scene)
		delete m_scene;
	delete m_input_manager;
	delete m_font_manager;
	delete m_texture_manager;
	delete m_window;
	delete m_context;
}

void sad::Renderer::setScene(Scene * scene)
{
	if (this->running())
	{
		m_new_scene = scene;
	} 
	else
	{
		delete m_scene;
		m_scene = scene;
	}
	scene->setRenderer(this);
}

sad::Scene* sad::Renderer::scene() const
{
	return m_scene;
}


bool sad::Renderer::init(const sad::Settings& _settings)
{
	SL_INTERNAL_SCOPE("sad::Renderer::init", (*this));
	m_glsettings = _settings;
	m_window->setCreationSize(m_glsettings.width(), m_glsettings.height());

	// TODO: Remove it, when testing lazy loads
	if (running() || hasValidContext())
	{
		return;
	}

	bool success = m_window->create();
	if (success)
	{
		success =  m_context->createFor(m_window);
		if (!success)
		{
			SL_LOCAL_FATAL("Failed to create context\n",*this);
			m_window->destroy();
		} 
		else
		{
			m_window->initialize();
		}
	}
	if (!success) 
	{ 
		SL_LOCAL_FATAL("Cannot create window\n",*this);
	}
	return true;
}

bool sad::Renderer::run()
{
	SL_INTERNAL_SCOPE("sad::Renderer::run()", *this);
 

	// Try to create window if needed
	bool success = false;
	if (m_window->valid() == false)
	{
		success = m_window->create();
		if (!success)
		{
			SL_LOCAL_FATAL("Cannot create window\n",*this);
		}
	} 
	else
	{
		success = true;
	}

	// Try to create context if needed
	if (m_context->valid() == false && success)
	{
		success =  m_context->createFor(m_window);
		if (!success)
		{
			SL_LOCAL_FATAL("Failed to create OpenGL context\n",*this);
			m_window->destroy();
		}
		else
		{
			m_window->initialize();
		}
	}

 
	if (success)
	{
		mainLoop();
		m_context->destroy();
		m_window->destroy();
	}

	return success;
}

void sad::Renderer::quit()
{
	m_running = false;
	if (m_window->valid())
	{
		m_window->close();
	}
}

sad::Renderer* sad::Renderer::ref()
{
	if (sad::Renderer::m_instance == NULL)
	{
		sad::Renderer::m_instance = new sad::Renderer();
		atexit(sad::Renderer::destroyInstance);
	}
	return sad::Renderer::m_instance;
}

double sad::Renderer::fps() const
{
	return m_fps;
}

void sad::Renderer::setWindowTitle(const sad::String & s)
{
	m_window->setTitle(s);
}

void sad::Renderer::makeFixedSize()
{
	m_window->makeFixedSize();
}

void sad::Renderer::makeResizeable()
{
	m_window->makeResizeable();
}


void sad::Renderer::toggleFullscreen()
{
	if (m_window->fullscreen())
	{
		m_window->leaveFullscreen();
	} 
	else
	{
		m_window->enterFullscreen();
	}
}

bool sad::Renderer::running()
{
	return m_running;
}

bool sad::Renderer::hasValidContext()
{
	return m_window->valid() && m_context->valid();
}


sad::FontManager * sad::Renderer::fonts()
{
	return m_font_manager;
}

sad::TextureManager * sad::Renderer::textures()
{
	return m_texture_manager;
}

sad::Input  * sad::Renderer::controls()
{
	return m_input_manager;
}

sad::log::Log * sad::Renderer::log()
{
	return &m_log;
}

sad::Window * sad::Renderer::window()
{
	return m_window;
}

sad::GLContext * sad::Renderer::context()
{
	return m_context;
}

void sad::Renderer::destroyInstance()
{
	delete  sad::Renderer::m_instance;
}

void sad::Renderer::setFPS(double fps) 
{
	// Reject sudden raises or drops for frameate
	m_fps =  fps * 0.8 + m_fps * 0.2;
}



void sad::Renderer::reshape(int width, int height)
{
  SL_INTERNAL_SCOPE(fmt::Format("sad::Renderer::reshape({0}, {1})") << width << height, *this);

  if (width == 0) {
	  width = 1;
  }
  
  glViewport (0, 0, width, height);				// ����������������� ViewPort (������� ���������)
  glMatrixMode (GL_PROJECTION);										// �������� ������� ��������
  glLoadIdentity ();													// ���������� � �� ���������
  gluPerspective (m_glsettings.fov(), 
			    (GLfloat)(width)/(GLfloat)(height),		      	// Calculate The Aspect Ratio Of The Window
			     m_glsettings.znear(), 
		             m_glsettings.zfar());		
  glMatrixMode (GL_MODELVIEW);										// �������� ������� �������
  glLoadIdentity ();													// ���������� � �� ���������
  this->controls()->  postResize(sad::ResizeEvent( 
								 m_window.height,m_window.width,height,width
	                             ));
  m_window.width=width;
  m_window.height=height;
}

#ifndef GL_GENERATE_MIPMAP_HINT
	#define GL_GENERATE_MIPMAP_HINT           0x8192
#endif














//Getting a black background with all params
bool sad::Renderer::initGLRendering()
{
    SL_INTERNAL_SCOPE("sad::Renderer::initGLRendering()", *this);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f); //Fill a black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	const char * version=(const char *)glGetString(GL_VERSION);
	if (version!=NULL)
	{
		SL_LOCAL_INTERNAL(sad::String("running OpenGL ")+sad::String(version), *this);
		if (version[0]>'1' || version[2] >='4')
			glHint(GL_GENERATE_MIPMAP_HINT,GL_NICEST);
	}

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	
	reshape(m_glsettings.width(),m_glsettings.height());
	
	glFlush();
	return true;
}














