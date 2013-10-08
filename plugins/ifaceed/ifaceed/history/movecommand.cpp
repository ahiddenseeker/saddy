#include "movecommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include "../editorcore/editor.h"
#include <unused.h>

MoveCommand::MoveCommand(AbstractScreenObject * object, const sad::Point2D & oldp, const sad::Point2D & newp)
{
	SL_SCOPE("MoveCommand::MoveCommand");
	m_object = object;
	m_old_point = oldp;
	m_new_point = newp;
}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::commit(UNUSED  ActionContext *c, CommandChangeObserver * ob )
{
	m_object->moveCenterTo(m_new_point);
	ob->submitEvent("MoveCommand::commit", sad::Variant(0));
}

void MoveCommand::rollback(UNUSED ActionContext *c, CommandChangeObserver * ob)
{
	m_object->moveCenterTo(m_old_point);
	ob->submitEvent("MoveCommand::rollback", sad::Variant(0));
}


ResizeCommand::ResizeCommand(AbstractScreenObject * object,
                             const sad::Rect2D & oldrect,
                             const sad::Rect2D & nr, float a)
{
	SL_SCOPE("ResizeCommand::ResizeCommand");
	m_object = object;
    m_old_rect = oldrect;
	m_new_rect = nr;
	m_angle = a;
}

ResizeCommand::~ResizeCommand()
{

}

void ResizeCommand::commit(UNUSED ActionContext *c, CommandChangeObserver * ob )
{
	m_object->setRotatedRectangle(m_new_rect, m_angle);
	ob->submitEvent("ResizeCommand::commit", sad::Variant(0));
}

void ResizeCommand::rollback(UNUSED  ActionContext *c, CommandChangeObserver * ob)
{
	m_object->setRotatedRectangle(m_old_rect, m_angle);
	ob->submitEvent("ResizeCommand::rollback", sad::Variant(0));
}


MakeBackgroundCommand::MakeBackgroundCommand(AbstractScreenObject * object)
{
	SL_SCOPE("MakeBackgroundCommand::MakeBackgroundCommand");
	m_o = object;
	m_layer = m_o->prop<unsigned int>("layer", sad::Log::ref());
	m_rect = m_o->prop<sad::Rect2D>("rect", sad::Log::ref());
	m_angle =  m_o->prop<float>("angle", sad::Log::ref());
}

void MakeBackgroundCommand::commit(UNUSED ActionContext *c, CommandChangeObserver * ob )
{
	m_o->setProp<unsigned int>("layer", 0, sad::Log::ref());
	m_o->setProp<float>("angle", 0.0f, sad::Log::ref());
	m_o->setProp<sad::Rect2D>("rect", sad::Rect2D(sad::Point2D(0,0), sad::Point2D(WINDOW_WIDTH, WINDOW_HEIGHT)), sad::Log::ref());	
	ob->submitEvent("MakeBackgroundCommand::commit", sad::Variant(0));
}

void MakeBackgroundCommand::rollback(UNUSED ActionContext *c, CommandChangeObserver * ob )
{
	m_o->setProp<sad::Rect2D>("rect", m_rect, sad::Log::ref());	
	m_o->setProp<float>("angle", m_angle, sad::Log::ref());	
	m_o->setProp<unsigned int>("layer", m_layer, sad::Log::ref());	
	
	ob->submitEvent("MakeBackgroundCommand::rollback", sad::Variant(0));
}


MakeBackgroundCommand::~MakeBackgroundCommand()
{

}


