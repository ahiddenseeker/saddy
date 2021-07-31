/*!  \file animationschangeeasingproperty.h
 *   
 *
 *   Defines a command type, which describes change of property
 *   of easing function in animation
 */
#pragma once

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>

#include "../command.h"

#include "../../core/editor.h"


namespace history
{

namespace animations
{

/*! A command, which describes changing property of animation
 */
template<
    typename T
>
class ChangeEasingProperty: public history::Command
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] setter a setter for property
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeEasingProperty(
        sad::animations::Animation* d,
        void (sad::animations::easing::Function::*setter)(T),
        const T& old_value,
        const T& new_value
    )
    : m_animation(d), m_setter(setter), m_old_value(old_value), m_new_value(new_value)
    {
        m_animation->addRef();
    }
    /*! Erases link to an animation
      */
    virtual ~ChangeEasingProperty()
    {
        m_animation->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override
    {
        sad::Renderer::ref()->lockRendering();
        ((m_animation->easing())->*m_setter)(m_new_value);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_new_value);
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override
    {
        sad::Renderer::ref()->lockRendering();        
        ((m_animation->easing())->*m_setter)(m_old_value);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_old_value);
    }
protected:
    /*! A changed animation
     */
    sad::animations::Animation* m_animation;
    /*!
     * A setter for property
     */
    void (sad::animations::easing::Function::*m_setter)(T);
    /*! An  old value for font property of node
     */
    T m_old_value;
    /*! A new value for font property of node
     */
    T m_new_value;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const T& value)
    {
        if (e->shared()->selectedAnimation() == m_animation)
        {
            this->updateUI(e, value);
        }
    }
    /*!
     * Descendants must re-implement this method to make UI update
     * actually happen
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const T& value) = 0;
};

}

}
