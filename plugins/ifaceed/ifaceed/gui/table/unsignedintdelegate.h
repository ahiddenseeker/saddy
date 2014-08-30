/*! \file unsignedintdelegate.h
	\author HiddenSeeker

	A delegate for property, which contains unsigned int values
 */
#pragma once
#include "delegate.h"
#include "../../history/propertychangecommand.h"
#include <QObject>

namespace gui
{
	
namespace table
{
/*! A delegate for editing values for unsigned int values
 */
class UnsignedIntDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
	/*! Constructs new delegate
	 */
	UnsignedIntDelegate();
	/*! Destroys delegate
	 */
	virtual ~UnsignedIntDelegate();
	/*! Sets a value for a delegate. Used by commands to reset
		\param[in] v a value
	 */
	virtual void set(const sad::db::Variant& v);
public slots:
	/*! When widget changed
		\param[in] i a widget value
	 */
	void widgetChanged(qulonglong i);
protected:
	/*! At this point delegate must create own editor, set it to table and connect delegate slots
		to it.
	 */
	virtual void makeEditor();
	/*! Disconnects slots  for own editor here
	 */ 
	virtual void disconnect();
};

}

}
