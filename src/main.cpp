//#include "stdafx.h"
#include "renderer.h"
#include "input.h"
#include "testnode.h"
#include "fontnode.h"
#include "texturemanager.h"
#include "fontmanager.h"
#include "movitem.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

void rend_quit(const sad::Event & o)
{
	sad::Renderer::instance().quit();
}
void rend_toggle(const sad::Event & o)
{
	sad::Renderer::instance().toggleFullscreen();
}

#include<math.h>
#include<time.h>

#ifdef WIN32
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
	sad::Renderer::instance().init(sad::Settings(640,480,false));
	
	
	sad::TMFont * fnt=new sad::TMFont;
	bool res=fnt->load("examples/times_red.PNG","examples/times_red.CFG");
    if (res)
	{
		sad::FontManager::add(fnt,"times_red");
	}
	else
	{
		delete fnt;
	}
	sad::Texture * tex=new sad::Texture();
	hst::string  testString("examples/times_red.PNG");
	bool result=tex->loadPNG(testString);
	sad::TextureManager::instance()->load("TEST",tex);
    sad::Texture * test=sad::TextureManager::instance()->get("TEST");
	test->setAlpha(32);
	srand(time(NULL));

	sad::Scene * sc= new sad::Scene();


	
	sc->add( new TestNode(test,(float)rand()/RAND_MAX*0.2f - 0.1f,(float)rand()/RAND_MAX*0.2f - 0.1f,0.06f) );
	sc->add( new PlayerBullet(217.0f, 0.2f, 0.15f, 0.5f, test, 0.0f, 0.0f, 0.5f, 0.5f) );
	if (res)
        	sc->add( new TestFontNode(fnt));

	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("My new sad window");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);	
	//Here must be an initialization of engine, and running it

							 
	
	test->buildMipMaps();
	test->enable();
	sad::Renderer::instance().run();

	return 0;
}

