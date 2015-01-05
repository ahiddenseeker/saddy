/*! \file animationschangelist.h
    \author HiddenSeeker

    Describes a command, which changes list for any kind of animation
 */
#pragma once
#include "animationschangeproperty.h"

#include <sadvector.h>
#include <sadstring.h>

#include <QTextEdit>

namespace history
{

namespace animations
{
/*! A command, which changes font list for font list animation
 */
class ChangeList: public history::animations::ChangeProperty< sad::Vector<sad::String> >
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
		\param[in] propertyname a property for editing
		\param[in] view a view for editing
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeList(
		sad::animations::Animation* d, 
		const sad::String& propertyname,
		QTextEdit* view,
		const sad::Vector<sad::String>& oldvalue, 
		const sad::Vector<sad::String>& newvalue
	);
    /*! Erases link to an animation
      */
    virtual ~ChangeList();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Vector<sad::String>& value);
	/*! A view for text edit
	 */
	QTextEdit* m_view;
};

}

}
