#include "sprite2dactions.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/sprite2d/changeoptions.h"
#include "../history/sprite2d/makebackground.h"
#include "../history/sprite2d/changeflipx.h"
#include "../history/sprite2d/changeflipy.h"

#include "../gui/scenenodeactions.h"

#include <sprite2d.h>
#include <geometry2d.h>

#include <QDebug>

// ===============================  PUBLIC METHODS ===============================

gui::Sprite2DActions::Sprite2DActions(QObject* parent) : QObject(parent), m_panel(NULL)
{

}

gui::Sprite2DActions::~Sprite2DActions()
{
	
}

void gui::Sprite2DActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::Sprite2DActions::panel() const
{
	return m_panel;
}

void gui::Sprite2DActions::cancelAddSprite()
{
	core::Shared* s = this->m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	sad::Renderer::ref()->lockRendering();
	s->setActiveObject(NULL);
	node->scene()->remove(node);
	sad::Renderer::ref()->unlockRendering();
	
	sad::hfsm::Machine* m = m_panel->editor()->machine();
	m->enterState(m->previousState());
	if (m->isInState("adding") || (m->isInState("selected") && s->selectedObject() == NULL))
	{
		m->enterState("idle");
	}
}

void gui::Sprite2DActions::moveCenterOfSprite(const sad::input::MouseMoveEvent & e)
{
	const sad::Point2D& p = e.pos2D();
	sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
	if (node)
	{
		static_cast<sad::Sprite2D*>(node)->setMiddle(p);
		this->m_panel->sceneNodeActions()->updateRegionForNode();
	}
}

void gui::Sprite2DActions::commitAdd(const sad::input::MousePressEvent& e)
{
	core::Shared* s = m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
	history::Command* c = new history::scenenodes::New(node);
	s->setActiveObject(NULL);
	s->setSelectedObject(node);
	m_panel->editor()->history()->add(c);
	c->commit(m_panel->editor());
	m_panel->editor()->machine()->enterState("selected");
}

void gui::Sprite2DActions::placeFirstPointForSprite(const sad::input::MousePressEvent& e)
{
	const sad::Point2D& p = e.pos2D();
	core::Shared* s = m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
	sprite->setArea(sad::Rect2D(p, p));
	sprite->setVisible(true);

	m_panel->editor()->shared()->toggleActiveBorder(true);

	m_panel->editor()->machine()->enterState("adding/sprite_diagonal/point_placed");
	m_panel->highlightState("Click, where you want lower point to be placed");

	m_panel->sceneNodeActions()->updateRegionForNode();
}

void gui::Sprite2DActions::moveLowerPointOfSprite(const sad::input::MouseMoveEvent & e)
{
	const sad::Point2D& p = e.pos2D();
	sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
	if (node)
	{
		sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(node);
		sad::Rect2D area = sprite->area();
		area = sad::Rect2D(p, area[3]);
		sprite->setArea(area);
		this->m_panel->sceneNodeActions()->updateRegionForNode();
	}
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::Sprite2DActions::add()
{
	bool valid = true;
	valid = valid && m_panel->currentScene() != NULL;
	valid = valid && m_panel->UI()->rtwSpriteSprite->selectedResourceName().exists();
	valid = valid && m_panel->editor()->machine()->isInState("adding") == false;
	if (valid)
	{
		m_panel->editor()->emitClosure(blocked_bind(
			m_panel->UI()->awSceneNodeAngle,
			&gui::anglewidget::AngleWidget::setValue,
			0
		));
		m_panel->editor()->emitClosure(blocked_bind(
			m_panel->UI()->clpSceneNodeColor,
			&gui::colorpicker::ColorPicker::setSelectedColor,
			QColor(255, 255, 255)
		));
		m_panel->editor()->emitClosure(blocked_bind(
			m_panel->UI()->cbSceneNodeVisible,
			&QCheckBox::setCheckState,
			Qt::Checked
		));
		m_panel->editor()->emitClosure(blocked_bind(
			m_panel->UI()->cbFlipX,
			&QCheckBox::setCheckState,
			Qt::Unchecked
		));
		m_panel->editor()->emitClosure(blocked_bind(
			m_panel->UI()->cbFlipY,
			&QCheckBox::setCheckState,
			Qt::Unchecked
		));

		m_panel->editor()->cleanupBeforeAdding();

		if (m_panel->UI()->rbPlaceAndRotate->isChecked())
		{
			this->addBySimplePlacing();
		}
		else
		{
			this->addByDiagonalPlacing();
		}
	}
}

void gui::Sprite2DActions::addBySimplePlacing()
{
	sad::Sprite2D* sprite = new sad::Sprite2D();
	sprite->setTreeName("");
	sprite->set(m_panel->UI()->rtwSpriteSprite->selectedResourceName().value());

	const sad::Settings & settings = sad::Renderer::ref()->settings();
	sprite->setMiddle(sad::Point2D(settings.width() / 2.0, settings.height() / 2.0));

	QString name = m_panel->UI()->txtObjectName->text();
	if (name.length())
	{
		sprite->setObjectName(name.toStdString());
	}

	m_panel->currentScene()->add(sprite);
	m_panel->editor()->shared()->setActiveObject(sprite);
	m_panel->editor()->shared()->toggleActiveBorder(true);

	m_panel->editor()->machine()->enterState("adding/sprite");
	m_panel->highlightState("Click, where you want sprite to be placed");

	m_panel->sceneNodeActions()->updateRegionForNode();
}

void gui::Sprite2DActions::addByDiagonalPlacing()
{
	sad::Sprite2D* sprite = new sad::Sprite2D();
	sprite->setTreeName("");
	sprite->set(m_panel->UI()->rtwSpriteSprite->selectedResourceName().value());

	const sad::Settings & settings = sad::Renderer::ref()->settings();
	sprite->setMiddle(sad::Point2D(settings.width() / 2.0, settings.height() / 2.0));

	QString name = m_panel->UI()->txtObjectName->text();
	if (name.length())
	{
		sprite->setObjectName(name.toStdString());
	}

	sprite->setVisible(false);

	m_panel->currentScene()->add(sprite);
	m_panel->editor()->shared()->setActiveObject(sprite);
	m_panel->editor()->shared()->toggleActiveBorder(false);

	m_panel->editor()->machine()->enterState("adding/sprite_diagonal");
	m_panel->highlightState("Click, where you want upper point to be placed");

	m_panel->sceneNodeActions()->updateRegionForNode();
}


void gui::Sprite2DActions::spriteOptionsChanged(sad::String s)
{
	if (m_panel->editor()->shared()->activeObject() != NULL)
	{
		sad::Renderer::ref()->lockRendering();
		m_panel->editor()->shared()->activeObject()->setProperty("options", s);
		m_panel->editor()->shared()->activeObject()->rendererChanged();
		m_panel->sceneNodeActions()->updateRegionForNode();
		sad::Renderer::ref()->unlockRendering();
	}
	else
	{
		sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
		if (node)
		{
			sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("options");
			sad::Maybe<sad::Rect2D> oldrect = node->getProperty<sad::Rect2D>("area");
			if (oldvalue.exists() && oldrect.exists())
			{
				if (oldvalue.value() != s)
				{
					sad::Renderer::ref()->lockRendering();
					node->setProperty("options", s);
					node->rendererChanged();
					m_panel->sceneNodeActions()->updateRegionForNode();
					sad::Renderer::ref()->unlockRendering();
					m_panel->editor()->history()->add(
						new history::sprite2d::ChangeOptions(
							node, 
							oldrect.value(), 
							oldvalue.value(),
							s
						)
					);
				}
			}
		}
	}
}

void gui::Sprite2DActions::makeBackground()
{
	const sad::Settings& settings = sad::Renderer::ref()->settings();
	sad::Rect2D newrect(
		sad::Point2D(0, 0),
		sad::Point2D(settings.width(), settings.height())
	);
	sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
	if (m_panel->editor()->shared()->activeObject() == NULL && node)
	{
		sad::Maybe<sad::Rect2D> oldrect = node->getProperty<sad::Rect2D>("area");
		sad::Maybe<float> oldangle = node->getProperty<float>("angle");
		sad::Maybe<unsigned int> oldlayer = node->getProperty<unsigned int>("layer");

		if (oldrect.exists() && oldangle.exists() && oldlayer.exists())
		{
			history::Command* c = new history::sprite2d::MakeBackground(
				node,
				oldrect.value(),
				oldangle.value(),
				oldlayer.value(),
				newrect,
				0,
				0
			);
			m_panel->editor()->history()->add(c);
			c->commit(m_panel->editor());
		}
	}
}

void gui::Sprite2DActions::flipXChanged(bool state)
{
	bool newvalue = state;
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
		m_panel->editor()->shared()->activeObject()->setProperty("flipx", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
			sad::Maybe<bool> oldvalue = node->getProperty<bool>("flipx");
			if (oldvalue.exists())
			{
				if (newvalue != oldvalue.value())
				{
					node->setProperty("flipx", newvalue);
					m_panel->editor()->history()->add(history::sprite2d::changeFlipX(
						node, 
						oldvalue.value(), 
						newvalue
					));
				}
			}
        }
    }
}

void gui::Sprite2DActions::flipYChanged(bool state)
{
	bool newvalue = state;
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
		m_panel->editor()->shared()->activeObject()->setProperty("flipy", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
			sad::Maybe<bool> oldvalue = node->getProperty<bool>("flipy");
			if (oldvalue.exists())
			{
				if (newvalue != oldvalue.value())
				{
					node->setProperty("flipy", newvalue);
					m_panel->editor()->history()->add(history::sprite2d::changeFlipY(
						node, 
						oldvalue.value(), 
						newvalue
					));
				}
			}
        }
    }
}
