#include "animationprocess.h"

#include <animations/animationsanimations.h>
#include <renderer.h>

#include "../closuremethodcall.h"
#include "../core/editor.h"
#include "../core/shared.h"
#include "../mainpanel.h"

// ============================ PUBLIC METHODS ============================

gui::AnimationProcess::AnimationProcess() : m_editor(NULL), m_instance(NULL)
{
	m_timer.setSingleShot(false);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
}

gui::AnimationProcess::~AnimationProcess()
{
	if (m_instance)
	{
		m_instance->delRef();
	}
}

void gui::AnimationProcess::start(sad::animations::Instance* i)
{
	if (m_editor)
	{
		if (m_editor->shared()->isAnyKindOfAnimationIsRunning() == false)
		{
			m_editor->shared()->setAnimationIsRunning(true);
			m_editor->panel()->toggleAnimationPropertiesEditable(false);

			m_instance = i;
			m_instance->addRef();

			m_timer.setSingleShot(false);
			m_timer.setInterval(gui::AnimationProcess::TIMEOUT);

			sad::Renderer::ref()->animations()->add(m_instance);

			m_timer.start();
		}
	}
}

void gui::AnimationProcess::stop()
{
	m_mutex.lock();
	if (m_instance != NULL)
	{
		if (m_editor->shared()->isAnyKindOfAnimationIsRunning())
		{
			m_editor->shared()->setAnimationIsRunning(false);
			m_editor->emitClosure( bind(m_editor->panel(), &MainPanel::toggleAnimationPropertiesEditable, true) );

			if (m_instance->finished() == false)
			{
				sad::Renderer::ref()->lockRendering();
				sad::Renderer::ref()->animations()->remove(m_instance);
				m_instance->cancel(sad::Renderer::ref()->animations());
				sad::Renderer::ref()->unlockRendering();
			}

			m_instance->delRef();
			m_instance = NULL;

			m_editor->emitClosure( bind(&m_timer, &QTimer::stop) );
		}
	}
	m_mutex.unlock();
}

void  gui::AnimationProcess::setEditor(core::Editor* e)
{
	m_editor = e;
}

const int gui::AnimationProcess::TIMEOUT = 100;

// ========================== PUBLIC SLOTS METHODS ==========================

void gui::AnimationProcess::timerExpired()
{
    if (m_editor && m_instance)
    {
		if (m_editor->shared()->isAnyKindOfAnimationIsRunning())
		{
			if (m_instance)
			{
				if (m_instance->finished())
				{
					this->stop();					
				}
			}
		}
    }
}

