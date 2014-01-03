/*! \file controls.h
	\author HiddenSeeker

	A basic implementation for controls, which hold a event handlers, which handle queued events
 */
#pragma once
#include "events.h"
#include "handlers.h"
#include "handlerconditions.h"
#include "../sadptrvector.h"
#include "../temporarilyimmutablecontainer.h"

namespace sad
{

namespace input
{

/*! An immutable base container for controls, used as hints for IDE and other parts
 */
typedef sad::TemporarilyImmutableContainerWithHeterogeneousCommands<
	sad::Pair<sad::input::HandlerTypeAndConditions, sad::input::AbstractHandler*>,
	sad::input::AbstractHandler*
> ControlsImmutableBase;

/*! A controls, which hold a event handlers, which handle queued events.
	
	Note that controls DOES NOT provide type-checking of events and handlers.
	This is done, due to perfomance issues, caused by typecasting.
 */
class Controls
: public sad::input::ControlsImmutableBase
{
public:
	/*! Creates new empty controls with callbacks
	 */
	Controls();
	/*! Controls takes ownership for all of conditions and handler. 
		It's frees memory from them, when destroyed
	 */ 
	~Controls();
	/*! Adds new handler and conditions for it invocation. Enqueues it to queue, it cannot
		be added now
		\param[in] tac defines both type for events, received by handler
		\param[in] h  handler, which will receive events
	 */
	void add(const sad::input::HandlerTypeAndConditions & tac, sad::input::AbstractHandler * h);
	/*! Adds new handler with conditions
		\param[in] tac type and conditions
		\param[in] f function
	 */
	template<
		typename _EventType
	>
	inline void add(const sad::input::HandlerTypeAndConditions & tac, void (*f)(const _EventType &))
	{
		add(tac, new sad::input::FreeFunctionHandler<_EventType>(f));
	}
	/*! Adds new handler with no conditions
		\param[in] f function
	 */
	template<
		typename _EventType
	>
	inline void add(void (*f)(const _EventType &))
	{
		sad::input::HandlerTypeAndConditions tac;
		tac.set1(sad::input::EnumValueForEventType<_EventType>::Type);
		add(tac, 
			new sad::input::FreeFunctionHandler<_EventType>(f)
		   );
	}
	/*! Adds new handler with conditions
		\param[in] tac type and conditions
		\param[in] f function
	 */
	inline void add(const sad::input::HandlerTypeAndConditions & tac, void (*f)())
	{
		add(tac, new sad::input::VoidFreeFunctionHandler(f));
	}
	/*! Adds new handler as a method call for conditions
		\param[in] tac type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f a method, which will handle event
	 */
	template<
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		void (_MethodObject::*f)()
	)
	{
		add(tac, new sad::input::VoidMethodHandler<_Object, void (_MethodObject::*)()>(o, f));
	}
	/*! Adds new handler as a method call for conditions
		\param[in] tac type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f a method, which will handle event
	 */
	template<
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		void (_MethodObject::*f)() const
	)
	{
		add(tac, new sad::input::VoidMethodHandler<_Object, void (_MethodObject::*)() const>(o, f));
	}
	/*! Adds new handler as a method call for conditions
		\param[in] tac type and conditions
		\param[in] o object, which callback will be called on
		\param[in] c a method, which will handle event
	 */
	template<
		typename _EventType,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		void (_MethodObject::*c)(const _EventType &)
	)
	{
		add(tac, new sad::input::MethodHandler<_EventType, _Object, void (_MethodObject::*)(const _EventType &)>(o, c));
	}
	/*! Adds new handler as a method call for conditions
		\param[in] tac type and conditions
		\param[in] o object, which callback will be called on
		\param[in] c a method, which will handle event
	 */
	template<
		typename _EventType,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		void (_MethodObject::*c)(const _EventType &) const
	)
	{
		add(tac, new sad::input::MethodHandler<_EventType, _Object, void (_MethodObject::*)(const _EventType &) const>(o, c));
	}

	/*! Adds new handler as a method call with no conditions. 
		\param[in] o object, which callback will be called on
		\param[in] c a method, which will handle event
	 */
	template<
		typename _EventType,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		_Object * o, 
		void (_MethodObject::*c)(const _EventType &) const
	)
	{
		sad::input::HandlerTypeAndConditions tac;
		tac.set1(sad::input::EnumValueForEventType<_EventType>::Type);
		add(tac, new sad::input::MethodHandler<_EventType, _Object, void (_MethodObject::*)(const _EventType &) const>(o, c));
	}
	/*! Adds new handler as a method call with no conditions. 
		\param[in] o object, which callback will be called on
		\param[in] c a method, which will handle event
	 */
	template<
		typename _EventType,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		_Object * o, 
		void (_MethodObject::*c)(const _EventType &)
	)
	{
		sad::input::HandlerTypeAndConditions tac;
		tac.set1(sad::input::EnumValueForEventType<_EventType>::Type);
		add(tac, new sad::input::MethodHandler<_EventType, _Object, void (_MethodObject::*)(const _EventType &)>(o, c));
	}

	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] tac a type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _EventType,
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)(const _EventType &)
	)
	{
		add(tac, new sad::input::CompositeHandler<_EventType, _Object, _FMethod, void (_MethodObject::*)(const _EventType &)>(o, f, g));
	}
	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] tac a type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _EventType,
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)(const _EventType &) const
	)
	{
		add(tac, new sad::input::CompositeHandler<_EventType, _Object, _FMethod, void (_MethodObject::*)(const _EventType &) const>(o, f, g));
	}	
	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] tac a type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)()
	)
	{
		add(tac, new sad::input::VoidCompositeHandler<_Object, _FMethod, void (_MethodObject::*)()>(o, f, g));
	}
	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] tac a type and conditions
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		const sad::input::HandlerTypeAndConditions & tac, 
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)() const
	)
	{
		add(tac, new sad::input::VoidCompositeHandler<_Object, _FMethod, void (_MethodObject::*)() const>(o, f, g));
	}

	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _EventType,
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)(const _EventType &) 
	)
	{
		sad::input::HandlerTypeAndConditions tac;
		tac.set1(sad::input::EnumValueForEventType<_EventType>::Type);
		add(tac, new sad::input::CompositeHandler<_EventType, _Object, _FMethod, void (_MethodObject::*)(const _EventType &)>(o, f, g));
	}	
	/*! Adds new handler as a composition of methods, applied sequentially to object. 
		\param[in] o object, which callback will be called on
		\param[in] f first composition method
		\param[in] g second composition method
	 */
	template<
		typename _EventType,
		typename _FMethod,
		typename _Object,
		typename _MethodObject
	>
	inline void add(
		_Object * o, 
		_FMethod f,
		void (_MethodObject::*g)(const _EventType &) const
	)
	{
		sad::input::HandlerTypeAndConditions tac;
		tac.set1(sad::input::EnumValueForEventType<_EventType>::Type);
		add(tac, new sad::input::CompositeHandler<_EventType, _Object, _FMethod, void (_MethodObject::*)(const _EventType &) const>(o, f, g));
	}	
	/*! Posts event to controls, running callbacks, determined by event type,  with argument of event e
		\param[in] type type of event, which is run
		\param[in] e event which is being emited
	 */
	void postEvent(EventType type, const sad::input::AbstractEvent & e);
	/*! Must be called at start of new receiving session to make sure, that controls changing
		of handlers is locked and we can't hurt it by messing with them. Also it add queued events
		to make sure, nothing gets from container.
	 */
	void startReceivingEvents();
	/*! Must be called, when all window events are processes to make sure, that all controls changing
		is processed and nothing gets killed in between.
	 */
	void finishRecevingEvents();
	/*! An amount of delta, that should be picked in mouse wheel event in X11,
		since X11 does not support it natively.
		\param[in] delta sensivity
	 */
	void setWheelTickSensivity(double delta);
	/*! Returns amount of delta, that should be picked in mouse wheel event in X11,
		since X11 does not support it natively.
		\return sensivity value
	 */
	double wheelTickSensivity() const;
	/*! Sets sensivity (in ms) for double click in X11, since it does not support it natively
		\param[in] sensivity a sensivity for double click recognized
	 */
	void setDoubleClickSensivity(double sensivity);
	/*! Returns sensivity (in ms) for double click in X11, since it does not support it natively
		\return sensivity for double click
	 */
	double doubleClickSensivity() const;
protected:
	/*! A both handle and conditions, stored in controls
	 */
	typedef sad::Pair<sad::input::HandlerConditionsList, sad::input::AbstractHandler *> HandlerAndConditions;
	/*! A shortcut for list of handlers
	 */
	typedef sad::Vector<HandlerAndConditions>  HandlerList;
	/*! A handlers, attached to a event types
	 */
	HandlerList m_handlers[SAD_INPUT_EVENTTYPE_COUNT];
	/*! An amount of delta, that should be picked in mouse wheel event in X11,
		since X11 does not support it natively
	 */
	double m_wheelticksensivity;
	/*! A sensivity for double click as amount of time between two clicks, needed to compose them
		as double click
	 */
	double m_doubleclicksensivity;
	/*! Immediately clears all handlers, removing all previous bindings
	 */
	virtual void clearNow();
	/*! Adds immediately pair of type and handler to controls
		\param[in] o a specified pair
	 */
	virtual void addNow(
		sad::Pair<
			sad::input::HandlerTypeAndConditions, 
			sad::input::AbstractHandler*
		> o
	);
	/*! Removes a handler from registered controls, freeing all of it's memory from handler
		and it's conditions
		\param[in] o an abstract handler, to be removed
	 */
	void removeNow(sad::input::AbstractHandler* o);
private:
	/*! This object is non-copyable, this is not implemented
		\param[in] o other controls object
	 */
	Controls(const sad::input::Controls & o);
	/*! This object is non-copyable, this is not implemented
		\param[in] o other controls object
		\return self-reference
	 */
	Controls& operator=(const sad::input::Controls & o);
	/*! Frees memory, from both handler and conditions, setting pair values to zero
		\param[in] o handler and conditions
	 */
	static void freeHandlerAndConditions(HandlerAndConditions & o);
	/*! Invokes a conditions and tries to invoke a handlers, if they are met
		\param[in] o handler and conditions
		\param[in] e event
	 */
	static void tryInvokeHandler(const HandlerAndConditions & o, const sad::input::AbstractEvent & e);
};

}

}
