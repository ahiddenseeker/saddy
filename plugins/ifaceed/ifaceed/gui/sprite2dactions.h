/*! \file sprite2dactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to sprited
 */
#pragma once
#include <QObject>

#include <input/events.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to sprites
 */	
class Sprite2DActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new actions for sprites
		\param[in] parent a parent object
	 */
	Sprite2DActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~Sprite2DActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
	/*! Cancels adding sprite to scene
	 */
	void cancelAddSprite();
	/*! Moves center of sprite to a position
		\param[in] e a sprite
	 */
	void moveCenterOfSprite(const sad::input::MouseMoveEvent & e);
	/*! Commits adding a sprite
		\param[in] e event
	 */
	void commitAdd(const sad::input::MousePressEvent& e);
public slots:
	/*! Starts placing sprites
	 */
	void add();
	/*! Adds new sprite to a scene, if all parameters in form are valid
	 */
	void addBySimplePlacing();
	/*! Adds new sprite to a scene by diagonal placing
	 */
	void addByDiagonalPlacing();
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
