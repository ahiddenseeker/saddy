#include "qttarget.h"

#include "../core/editor.h"

core::QtTarget::~QtTarget()
{

}


void core::QtTarget::receive(const sad::log::Message & message)
{
	if (m_enabled)
	{
		if (message.priority() == sad::log::FATAL)
		{
			this->critical(message.message());
		}
		if (message.priority() == sad::log::WARNING)
		{
			this->warning(message.message());
		}

	}
}

void core::QtTarget::critical(const sad::String & m)
{
	
	CLOSURE
	CLOSURE_DATA(core::Editor * ed; QString w;)
	CLOSURE_CODE(
		QMessageBox::critical(ed->panel(), "", w);
	)
	INITCLOSURE( CLSET(ed, m_editor); CLSET(w, m.data()); )
	SUBMITCLOSURE( m_editor->emitClosure );

}

void core::QtTarget::warning(const sad::String & m)
{
	
	CLOSURE
	CLOSURE_DATA(core::Editor * ed; QString w;)
	CLOSURE_CODE(
		QMessageBox::warning(ed->panel(), "", w);
	)
	INITCLOSURE( CLSET(ed, m_editor); CLSET(w, m.data()); )
	SUBMITCLOSURE( m_editor->emitClosure );

}