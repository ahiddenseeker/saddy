#include "gui/table/unsignedlongdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "qwwulongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::UnsignedLongDelegate::UnsignedLongDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedLongDelegate::~UnsignedLongDelegate()
{
	
}

void gui::table::UnsignedLongDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	unsigned long value = v.get<unsigned long>().value();
	this->setCurrentValue<unsigned long>(value);
	static_cast<QwwULongSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::UnsignedLongDelegate::widgetChanged(qulonglong i)
{
	unsigned long oldvalue = this->currentValue<unsigned long>();
	m_editor->history()->add(new history::database::ChangeProperty<unsigned long>(oldvalue, i, this));
	this->setCurrentValue<unsigned long>(i);
}

void gui::table::UnsignedLongDelegate::makeEditor()
{
	unsigned long value = this->currentValue<unsigned long>();
	QwwULongSpinBox * d = new QwwULongSpinBox();
	d->setValue(value);
	d->setMinimum(0);
	d->setMaximum(ULONG_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(qulonglong)), this, SLOT(widgetChanged(qulonglong)));	
}

void gui::table::UnsignedLongDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(qulonglong)));
}
