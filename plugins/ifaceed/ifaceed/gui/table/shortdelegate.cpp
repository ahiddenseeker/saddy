#include "gui/table/shortdelegate.h"

#include "core/editor.h"

#include "history/database/propertychanged.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

gui::table::ShortDelegate::ShortDelegate() : gui::table::Delegate()
{

}

gui::table::ShortDelegate::~ShortDelegate()
{
	
}

void gui::table::ShortDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	short value = v.get<short>().value();
	this->setCurrentValue<short>(value);
	static_cast<QSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::ShortDelegate::widgetChanged(int i)
{
	short oldvalue = this->currentValue<short>();
	m_editor->history()->add(new history::database::PropertyChanged<short>(oldvalue, i, this));
	this->setCurrentValue<short>(i);
}

void gui::table::ShortDelegate::makeEditor()
{
	short value = this->currentValue<short>();
	QSpinBox * d = new QSpinBox();
	d->setValue((int)(value));
	d->setMinimum(SHRT_MIN);
	d->setMaximum(SHRT_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::ShortDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(int)));
}