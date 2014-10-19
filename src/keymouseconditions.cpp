#include "keymouseconditions.h"

bool sad::KeyHoldCondition::check(const sad::input::AbstractEvent & e)
{
	const sad::input::KeyEvent & ke = static_cast<const sad::input::KeyEvent &>(e);
	return ke.Key == m_key;
}

sad::input::AbstractHanderCondition * sad::KeyHoldCondition::clone()
{
	return new sad::KeyHoldCondition(m_key);
}


sad::SpecialKeyHoldCondition::SpecialKeyHoldCondition(sad::SpecialKey key) 
: m_key(key)
{

}

bool sad::SpecialKeyHoldCondition::check(const sad::input::AbstractEvent & e)
{
	const sad::input::KeyEvent & ke = static_cast<const sad::input::KeyEvent &>(e);
	sad::input::KeyEvent & k = const_cast<sad::input::KeyEvent &>(ke);
	bool result = false;
	switch(m_key)
	{
		case sad::HoldsControl: result = k.CtrlHeld; break;
		case sad::HoldsAlt: result = k.AltHeld; break;
		case sad::HoldsShift: result = k.ShiftHeld; break;
	}
	return result;
}

sad::input::AbstractHanderCondition* sad::SpecialKeyHoldCondition::clone()
{
	 sad::SpecialKeyHoldCondition* result =  new sad::SpecialKeyHoldCondition(m_key);
	 return result;
}

bool sad::MouseButtonHoldCondition::check(const sad::input::AbstractEvent & e)
{
	const sad::input::MouseEvent & ke = static_cast<const sad::input::MouseEvent &>(e);
	return ke.Button == m_button;
}

sad::input::AbstractHanderCondition * sad::MouseButtonHoldCondition::clone()
{
	return new sad::MouseButtonHoldCondition(m_button);
}
