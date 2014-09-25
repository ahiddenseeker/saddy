/*! \file customobjectchangeproperty.h
	\author HiddenSeeker

	Describes a command, which defines changing property of a custom object
 */
#pragma once
#include "../history/scenenodes/scenenodeschangeproperty.h"

#include "../../mainpanel.h"

namespace history
{
	
namespace customobject
{

/*! Describes a command, which defines changing property of a custom object
 */
template<typename T>
class ChangeProperty: public history::scenenodes::ChangeProperty<T>
{
public:
	 /*! Constructs new command for node
        \param[in] d a node
        \param[in] property a property name
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangeProperty(
        sad::SceneNode* d,
        const sad::String& property,
        const T& oldvalue,
        const T& newvalue
    )
    :  history::scenenodes::ChangeProperty<T>(d, property, oldvalue, newvalue)
    {
        
    }
	/*! Can be inherited
	 */
	virtual ~ChangeProperty()
    {
	    
    }
protected:
	 /*! Updates a main panel to current value
		 \param[in] e editor
		 \param[in] value a value for element
	  */
	 void updateUI(core::Editor* e, const T& value)
	 {
		 e->panel()->updateCustomObjectPropertyValue(
			 this->history::scenenodes::ChangeProperty<T>::m_node,
			 this->history::scenenodes::ChangeProperty<T>::m_property,
			 sad::db::Variant(value)
		);
	 }
};

}

}
