/*! \file   handlerconditions.h
	\author HiddenSeeker

	A conditions for running event handlers is specified here. 
 */
#pragma once
#include "events.h"
#include "../sadvector.h"
#include "../sadpair.h"
#include "../maybe.h"

namespace sad
{

namespace input
{

class AbstractHanderCondition
{
public:
	/*! Checks a condition against abstract event
		\param[in] e event
		\return whether we should  run an event
	 */
	virtual bool check(const sad::input::AbstractEvent & e) = 0;
	/*! Clones a condition
		\return clone a condition
	 */
	virtual sad::input::AbstractHanderCondition * clone() = 0;
	/*! You can inherit condition for implementing your very own conditions
	 */
	virtual ~AbstractHanderCondition();
};

/*! A handler conditions as a list of conditions
 */
typedef sad::Vector<sad::input::AbstractHanderCondition *> 
	HandlerConditionsList;


/*! A class for both type and conditions.
	Here we introduce a small implementation hack, which allows to reduce count of constructor of handlers.

	Note, that you can create memory leak, by creating it outside of handler specification part (a call to
	sad:input::Controls::add) and not freeing a handler conditions list.
 */
class HandlerTypeAndConditions: public sad::Pair<sad::input::EventType, sad::input::HandlerConditionsList>
{
public:
	/*! A conversion hack for putting event type to handler insertion
		\param[in] t type of event
	 */
	inline HandlerTypeAndConditions(sad::input::EventType t) 
	: sad::Pair<sad::input::EventType, sad::input::HandlerConditionsList>(
		t, 
		sad::input::HandlerConditionsList()
	)
	{
	}
	/*! A parametric cnstructor for options
		\param[in] t type of event
		\param[in] m a list of handler conditions
	 */
	inline HandlerTypeAndConditions(sad::input::EventType t, const  sad::input::HandlerConditionsList & m ) 
	: sad::Pair<sad::input::EventType, sad::input::HandlerConditionsList>(
		t, 
		m
	)
	{
	}
	/*! Adds new condition into list, preserving constant parts
		\param[in] constants
	 */
	inline const HandlerTypeAndConditions & operator<<(sad::input::AbstractHanderCondition * c) const
	{
		HandlerTypeAndConditions & me = const_cast<sad::input::HandlerTypeAndConditions &>(*this);
		me._2() << c;
		return *this;
	}

};


}

}

/*! Converts event type to type and conditions, allowing to write something like 
	*t | Condition(a) | Condition(b)
	\param[in] t type
	\return type and conditions
 */
inline sad::input::HandlerTypeAndConditions operator*(sad::input::EventType t)
{
	return sad::input::HandlerTypeAndConditions(t);
}
