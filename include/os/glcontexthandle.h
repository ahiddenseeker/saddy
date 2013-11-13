/*! \file glcontexthandle.h
	\author HiddenSeeker

	Contains declaration of OpenGL context handle
 */
#pragma once
#include "windowhandles.h"


namespace sad
{

namespace os
{

/*! A handle to OpenGL context, useful for approaches with
	system hacks
 */
class GLContextHandle
{
public:
#ifdef WIN32
	HGLRC Context;
#endif

#ifdef X11
	GLXContext Context;
#endif

	/*! Creates default invalid handle
	 */
	inline GLContextHandle()
	{
		cleanup();
	}

	/*! Cleans up handle
	 */
	void cleanup()
	{
#ifdef WIN32
		Context = NULL;
#endif

#ifdef X11
		Context = NULL;
#endif
	}
	/*! Checks, whether handle is valid
		\return whether is valid
	 */
	bool valid() const
	{
#ifdef WIN32
		return Context != NULL;
#endif

#ifdef X11
		return Context != NULL;
#endif
	}
};

}

}

