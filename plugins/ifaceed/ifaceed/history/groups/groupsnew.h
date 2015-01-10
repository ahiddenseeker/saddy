/*! \file groupsnew.h
	\author HiddenSeeker

	Describes a command, which performs creating new animation group
 */
#pragma once
#include "../command.h"

#include <animations/animationsgroup.h>

namespace history
{
	
namespace groups
{
/*! A command, which performs creating new animation group
 */
class New: public history::Command
{
public:
	 /*! Constructs new command for node
		\param[in] d a node
	  */
	 New(sad::animations::Group* d);
	 /*! Erases link to a node
	  */
	 virtual ~New();
	 /*! Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(core::Editor * ob = NULL);
	 /*! Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! A node, which must be added to an item
	 */
	sad::animations::Group* m_node;
};

}

}
