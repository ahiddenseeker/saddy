/*! \file   renderer.cpp
\author FreakyBlast

\brief  Definition of Renderer.

In this file OpenGL function has been used obviously.
*/


#include "renderer.h"
#ifdef WIN32
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif


sad::Renderer::~Renderer(void)
{
	
}

bool sad::Renderer::init(const sad::Settings& _settings)
{
 m_glsettings.setWidthScreen(_settings.width());
 m_glsettings.setHeightScreen(_settings.height());
 m_glsettings.setIsFullscreen(_settings.isFullscreen());
 m_glsettings.setBPP(_settings.bpp());
 m_glsettings.setDepth(_settings.depth());
 m_glsettings.setFoV(_settings.fov());
 m_glsettings.setZNear(_settings.znear());
 m_glsettings.setZFar(_settings.zfar());
 
 return true;
}


void sad::Renderer::reshape(int width, int height)
{
 glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// ����������������� ViewPort (������� ���������)
 glMatrixMode (GL_PROJECTION);										// �������� ������� ��������
 glLoadIdentity ();													// ���������� � �� ���������
 gluPerspective (m_glsettings.fov(), 
			    (GLfloat)(width)/(GLfloat)(height),		      	// Calculate The Aspect Ratio Of The Window
			     m_glsettings.znear(), 
				 m_glsettings.zfar());		
 glMatrixMode (GL_MODELVIEW);										// �������� ������� �������
 glLoadIdentity ();													// ���������� � �� ���������
}


sad::Renderer& sad::Renderer::instance()
{
 static Renderer aloneRenderer;
 return aloneRenderer;
}


void sad::Renderer::mainLoop()
{
 int frames=0;
 bool isMessagePumpActive;
 MSG msg;

 m_running = true;											// Program Looping Is Set To TRUE
 m_window.active=true;
 Renderer::setTimer();


 while (m_running)											// Loop Until WM_QUIT Is Received
 {
  Renderer::instance().setTimer();					
  // Check For Window Messages
  if (PeekMessage (&msg, m_window.hWND, 0, 0, PM_REMOVE) != 0)
  {
	 // Check For WM_QUIT Message
	 if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
	 {
	  TranslateMessage(&msg);				// Translate The Message
	  DispatchMessage(&msg);						// If Not, Dispatch The Message
	 }
	 else											// Otherwise (If Message Is WM_QUIT)
	 {
		m_running = false;				// Terminate The Message Loop
	 }
   }
   else												// If There Are No Messages
   {
	  // Process Application Loop
	  frames++;
	  if (Renderer::instance().elapsedInMSeconds() >= 1000)
	  {m_fps = frames;frames=0;Renderer::instance().setTimer();}
	  
	  if (m_window.active)
	     update();
   }
  }
 m_window.active=false;
 this->releaseWindow();
}



void sad::Renderer::run()
{
 if (createWindow())
 {
		hst::log::inst()->owrite(hst::string("Renderer: started rendering cycles\n"));
	    mainLoop();
 }
 else
	 hst::log::inst()->owrite(hst::string("Can't create window\n"));
 
 hst::log::inst()->save("log.txt");
}	

LRESULT CALLBACK sad::Renderer::WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_CLOSE)
	{
     	    sad::Renderer::instance().m_running=false;					
			return 0;													
	}
	if (uMsg==WM_KEYDOWN)
	{
		if (wParam==VK_ESCAPE)
		{
			instance().quit();
		}
		char af[1000];
		GetKeyNameText(lParam,af,100);
		if (af[0]=='F')
		{
			instance().toggleFullscreen();
		}
		hst::log::inst()->owrite(hst::string(af));
		return 0;
	}
	if (uMsg==WM_SIZE)
	{
		if (wParam==SIZE_MINIMIZED)
		{
			instance().m_window.active=false;
		}
		else
		{
			instance().m_window.active=true;
			instance().reshape(LOWORD (lParam), HIWORD (lParam));
		}
	}
	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}




void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  if (m_running)
  {
   this->releaseWindow();
   this->m_window.fullscreen=!this->m_window.fullscreen;
   this->createWindow();
  }
  else
  {
	  this->m_window.fullscreen=!this->m_window.fullscreen;
  }
}

bool sad::Renderer::changeScreenResolution(int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
 DEVMODE dmScreenSettings;											// Device Mode
 ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
 dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
 dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
 dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
 dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
 dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
 if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
 {
	return false;													// Display Change Failed, Return False
 }
 return true;														// Display Change Was Successful, Return True
}






//Getting a black background with all params
bool sad::Renderer::initGLRendering()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f); //Fill a black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return true;
}

void sad::Renderer::mapToOGL(int x,int y,float & px,float & py,float & pz)
{
	GLint     viewport[4];
	GLdouble  modelview[16];
	GLdouble  projection[16];

	GLfloat winx=0,winy=0,winz=0;
	GLdouble result[3];
	
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT,viewport);

	winx=(float)x;
	winy=(float)(viewport[3])-(float)(y);

	glReadPixels(x,(int)winy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winz);

	gluUnProject(winx,winy,winz,modelview,projection,viewport,result,result+1,result+2);

	px=(float)(result[0]);
	py=(float)(result[1]);
	pz=(float)(result[2]);
}