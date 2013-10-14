/*! \file log.h
	\author HiddenSeeker
	
	Describes a basic log and log parameters
 */
#pragma once
#include "priority.h"
#include "stringcaster.h"
#include "color.h"
#include "logmessage.h"
#include "filetarget.h"
#include "consoletarget.h"
#include "logscope.h"

#include "../sadvector.h"
#include "../marshal/actioncontext.h"
#include "../sadmutex.h"




namespace sad
{

namespace log
{
	
/*! Log class takes all the front work, building a messages 
	and broadcasting them to all targets.

	If you want to register some events, writing it to log, you should use 
	this class.
 */
class Log: public ActionContext
{
public:
	/*! Broadcasts a message to all targets
		\param[in] m message
	 */
	virtual void broadcast(const sad::log::Message & m);
	/*! Adds a target to a list of targets. A messages, created with 
		sad::log::Log::broadcast() will be broadcasted to this target.
		\param[in] t target 
	 */
	virtual sad::log::Log & addTarget(sad::log::Target * t);
	/*! Removes a target to a list of targets. A messages, created with 
		sad::log::Log::broadcast() will be broadcasted to this target.
			
		Note, that memory from target is not freed. You must delete it manually.

		\param[in] t target 
	 */
	virtual sad::log::Log & removeTarget(sad::log::Target * t);
	/*! Broadcasts a message of priority FATAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	template<typename T> 
	void fatal(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::FATAL, file, line);
	}
	/*! Broadcasts a message of priority CRITICAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	template<typename T> 
	void critical(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::CRITICAL, file, line);
	}
	/*! Broadcasts a message of priority WARNING to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void warning(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::WARNING, file, line);
	}
	/*! Broadcasts a message of priority MESSAGE to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void message(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::MESSAGE, file, line);
	}
	/*! Broadcasts a message of priority DEBUG to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void debug(const T & mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(mesg, sad::log::DEBUG, file, line);
	}
	/*! Broadcasts a message of user priority to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
		\param[in] user a user priority name
	 */		
	template<typename T> 
	void user(
		const T & mesg, 
		const char * file = NULL, 
		int line = 0, 
		const sad::String & user =  sad::String()
	)
	{
		_createAndBroadcast(mesg, sad::log::USER, file, line, user);
	}
	/*! Broadcasts a message of SADDY_INTERNAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void saddyInternal(
		const T & mesg, 
		const char * file = NULL, 
		int line = 0
	)
	{
		_createAndBroadcast(mesg, sad::log::SADDY_INTERNAL, file, line);
	}
	/*! Broadcasts a message of priority FATAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	void fatal(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::FATAL, file, line);
	}
	/*! Broadcasts a message of priority CRITICAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */
	void critical(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::CRITICAL, file, line);
	}
	/*! Broadcasts a message of priority WARNING to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void warning(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::WARNING, file, line);
	}
	/*! Broadcasts a message of priority MESSAGE to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void message(const char * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::MESSAGE, file, line);
	}
	/*! Broadcasts a message of priority DEBUG to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	void debug(const char  * mesg, const char * file = NULL, int line = 0)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::DEBUG, file, line);
	}
	/*! Broadcasts a message of user priority to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
		\param[in] user a user priority name
	 */		
	void user(const char *  mesg, const char * file = NULL, int line = 0, const sad::String & user =  sad::String())
	{
		_createAndBroadcast(sad::String(mesg), sad::log::USER, file, line, user);
	}
	/*! Broadcasts a message of SADDY_INTERNAL to all targets
		\param[in] mesg message
		\param[in] file source file name, where message was created
		\param[in] line source file line, where message was created
	 */		
	template<typename T> 
	void saddyInternal(
		const char * mesg, 
		const char * file = NULL, 
		int line = 0
	)
	{
		_createAndBroadcast(sad::String(mesg), sad::log::SADDY_INTERNAL, file, line);
	}
	/*! Pushes current subsystem name into stack of subsystem names
		\param[in] str string
	 */
	virtual void pushAction(const sad::String & str);
	/*! Pushes current subsystem name into stack of subsystem names
		\param[in] str string
		\param[in] file a name of source file, where entering to subsystem occured
		\param[in] line a source file line, where entering 
	 */
	virtual void pushAction(const sad::String & str, const char * file, int line);
	/*! Pops last subsystem name from the stack of subsystem names
	 */
	virtual void popAction();
	/*! Frees memory from all targets
	 */
	virtual ~Log();
	/*! Returns a global renderer's log instance
	 */
	static Log * ref();
	/*! Creates new empty log
	 */
	inline Log() 
	{
		m_internal_mode = false;
	}
	/*! Sets internal mode. If true, on ::pushAction, log will generate internal messages
		\param[in] mode a new internal mode
	 */
	inline void setInternalMode(bool mode)
	{
		m_internal_mode = mode;
	}
protected:
	/*! Locks, for making this compatible with multithreading applications
	 */
	::sad::Mutex m_lock;
	/*! Determines, whether log is in internal mode. If internal mode, 
		sad::log::Log::pushAction creates an internal messaage
	 */ 
	bool        m_internal_mode;
	/*! A vector of targets, for broadcasting file
	 */
	sad::Vector<sad::log::Target *> m_targets;
	/*! Returns a current subsystem
		\return name of current subsystem
	 */
	virtual sad::String subsystem();
	/*! Creates new message and broadcasts them to all contained targets
		\param[in] mesg text message, that is being logged
		\param[in] priority  a priority for message
		\param[in] file      a name of file, where message was emitted
		\param[in] line		 a lnumber of line, where message was emitted
		\param[in] upriority 
	 */
	virtual void createAndBroadcast(
		const sad::String & mesg, 
		sad::log::Priority priority,
		const char * file = NULL, 
		int line = 0,
		const sad::String & upriority = sad::String()
	);
	/*! Creates new message, converts it to string 
		and broadcasts them to all contained targets
		\param[in] mesg       message, that is being logged
		\param[in] priority  a priority for message
		\param[in] file      a name of file, where message was emitted
		\param[in] line		 a lnumber of line, where message was emitted
		\param[in] upriority 
	 */
	template<typename T>
	void _createAndBroadcast(
		const T & mesg, 
		sad::log::Priority priority,
		const char * file = NULL, 
		int line = 0,
		const sad::String & upriority = sad::String()
	)
	{
		createAndBroadcast(sad::log::StringCaster<T>::cast(mesg), priority, file, line, upriority);
	}
};

}

}

/*! Creates new message in global log with message X and priority FATAL
 */
#define SL_FATAL(X) sad::log::Log::ref()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority CRITICAL
 */
#define SL_CRITICAL(X) sad::log::Log::ref()->critical(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority WARNING
 */
#define SL_WARNING(X) sad::log::Log::ref()->warning(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority MESSAGE
 */
#define SL_MESSAGE(X) sad::log::Log::ref()->message(X, __FILE__, __LINE__)
/*! Creates new message in global log with message X and priority DEBUG
 */
#define SL_DEBUG(X) sad::log::Log::ref()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in global log with message X and priority TYPE
 */
#define SL_USER(X, TYPE) sad::log::Log::ref()->user(X, __FILE__, __LINE__, TYPE)
/*! Creates new message in global log with message X and priority SADDY_INTERNAL
 */
#define SL_INTERNAL(X) sad::log::Log::ref()->saddyInternal(X, __FILE__, __LINE__)

/*! Creates new message in local log R with message X and priority FATAL
 */
#define SL_LOCAL_FATAL(X,R)    (R).log()->fatal(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority CRITICAL
 */
#define SL_LOCAL_CRITICAL(X,R) (R).log()->critical(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority WARNING
 */
#define SL_LOCAL_WARNING(X,R)  (R).log()->warning(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority MESSAGE
 */
#define SL_LOCAL_MESSAGE(X,R)  (R).log()->message(X, __FILE__, __LINE__)
/*! Creates new message in local log R with message X and priority DEBUG
 */
#define SL_LOCAL_DEBUG(X,R)    (R).log()->debug(X, __FILE__, __LINE__)
/*! Creates new user message in local log R with message X and priority TYPE
 */
#define SL_LOCAL_USER(X, R, TYPE)  (R).log()->user(X, __FILE__, __LINE__, TYPE)
/*! Creates new message in local log R with message X and priority SADDY_INTERNAL
 */
#define SL_LOCAL_INTERNAL(X, R) (R).log()->saddyInternal(X, __FILE__, __LINE__)
