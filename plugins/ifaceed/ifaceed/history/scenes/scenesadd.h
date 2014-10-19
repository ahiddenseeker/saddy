/*! \file scenesadd.h
	\author HiddenSeeker

	Describes a command, used to add an scene to scenes list
 */
#pragma once
#include "../command.h"
#include <scene.h>


namespace history
{

namespace scenes
{

/*! A command to add scene to scene list
 */
class Add: public history::Command
{
public:
	/*! Adds a scene, which was added. Command links to scene
		\param[in] s a scene
	 */
	Add(sad::Scene * s);
	/*! Deletes reference to a scene
	 */
	~Add();
	/*! Applies changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void commit(core::Editor * ob = NULL);
	/*! Reverts changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! Describes an added scene
	 */
	sad::Scene* m_scene;
};

}

}
