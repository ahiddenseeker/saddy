#include "gui/table/unsignedintdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "qwwulongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::UnsignedIntDelegate::UnsignedIntDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedIntDelegate::~UnsignedIntDelegate()
{
	
}

void gui::table::UnsignedIntDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	unsigned int value = v.get<unsigned int>().value();
	this->setCurrentValue<unsigned int>(value);
	static_cast<QwwULongSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::UnsignedIntDelegate::widgetChanged(qulonglong i)
{
	unsigned int oldvalue = this->currentValue<unsigned int>();
	m_editor->history()->add(new history::database::ChangeProperty<unsigned int>(oldvalue, i, this));
	this->setCurrentValue<unsigned int>(i);
}

void gui::table::UnsignedIntDelegate::makeEditor()
{
	unsigned int value = this->currentValue<unsigned int>();
	QwwULongSpinBox * d = new QwwULongSpinBox();
	d->setValue((int)(value));
	d->setMinimum(0);
	d->setMaximum(UINT_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(qulonglong)), this, SLOT(widgetChanged(qulonglong)));	
}

void gui::table::UnsignedIntDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(qulonglong)));
}
