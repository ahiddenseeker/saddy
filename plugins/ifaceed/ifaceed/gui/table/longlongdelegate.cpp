#include "gui/table/longlongdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "qwwlongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::LongLongDelegate::LongLongDelegate() : gui::table::Delegate()
{

}

gui::table::LongLongDelegate::~LongLongDelegate()
{
	
}

void gui::table::LongLongDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	long value = v.get<long long>().value();
	this->setCurrentValue<long long>(value);
	static_cast<QwwLongSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::LongLongDelegate::widgetChanged(qlonglong i)
{
	long long oldvalue = this->currentValue<long long>();
	m_editor->history()->add(new history::database::ChangeProperty<long long>(oldvalue, i, this));
	this->setCurrentValue<long long>(i);
}

void gui::table::LongLongDelegate::makeEditor()
{
	long long value = this->currentValue<long long>();
	QwwLongSpinBox * d = new QwwLongSpinBox();
	d->setValue(value);
	d->setMinimum(LLONG_MIN);
	d->setMaximum(LLONG_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(qlonglong)), this, SLOT(widgetChanged(qlonglong)));	
}

void gui::table::LongLongDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(qlonglong)));
}
