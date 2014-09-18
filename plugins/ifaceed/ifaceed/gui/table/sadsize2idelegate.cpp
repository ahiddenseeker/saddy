#include "gui/table/sadsize2idelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::SadSize2IDelegate::SadSize2IDelegate() : gui::table::Delegate()
{

}

gui::table::SadSize2IDelegate::~SadSize2IDelegate()
{

}

void gui::table::SadSize2IDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    sad::Size2I value = v.get<sad::Size2I>().value();
    this->setCurrentValue<sad::Size2I>(value);
    static_cast<gui::tuplewidget::Int64TupleWidget*>(m_my_widget)->setValue(value.Width, value.Height);
    m_my_widget->blockSignals(b);
}

void gui::table::SadSize2IDelegate::widgetChanged(qlonglong f, qlonglong s)
{
    sad::Size2I oldvalue = this->currentValue<sad::Size2I>();
    sad::Size2I newvalue(f, s);
    m_editor->history()->add(new history::database::ChangeProperty<sad::Size2I>(oldvalue, newvalue, this));
    this->setCurrentValue<sad::Size2I>(newvalue);
}

void gui::table::SadSize2IDelegate::makeEditor()
{
    sad::Size2I value = this->currentValue<sad::Size2I>();
    gui::tuplewidget::Int64TupleWidget * d = new gui::tuplewidget::Int64TupleWidget();
    d->setLabel("w:", "h:");
    d->setValue(value.Width, value.Height);
    d->setRange(0, UINT_MAX);
    m_my_widget = d;
    this->insertToTable();
    m_widget->setRowHeight(m_row, d->rowHeight(0) * 2 + 4);
    QObject::connect(d, SIGNAL(valueChanged(qlonglong, qlonglong)), this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

void gui::table::SadSize2IDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

