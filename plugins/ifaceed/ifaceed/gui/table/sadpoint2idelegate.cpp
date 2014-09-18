#include "gui/table/sadpoint2idelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::SadPoint2IDelegate::SadPoint2IDelegate() : gui::table::Delegate()
{

}

gui::table::SadPoint2IDelegate::~SadPoint2IDelegate()
{

}

void gui::table::SadPoint2IDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    sad::Point2I value = v.get<sad::Point2I>().value();
    this->setCurrentValue<sad::Point2I>(value);
    static_cast<gui::tuplewidget::Int64TupleWidget*>(m_my_widget)->setValue(value.x(), value.y());
    m_my_widget->blockSignals(b);
}

void gui::table::SadPoint2IDelegate::widgetChanged(qlonglong f, qlonglong s)
{
    sad::Point2I oldvalue = this->currentValue<sad::Point2I>();
    sad::Point2I newvalue(f, s);
    m_editor->history()->add(new history::database::ChangeProperty<sad::Point2I>(oldvalue, newvalue, this));
    this->setCurrentValue<sad::Point2I>(newvalue);
}

void gui::table::SadPoint2IDelegate::makeEditor()
{
    sad::Point2I value = this->currentValue<sad::Point2I>();
    gui::tuplewidget::Int64TupleWidget * d = new gui::tuplewidget::Int64TupleWidget();
    d->setLabel("x:", "y:");
    d->setValue(value.x(), value.y());
    d->setRange(INT_MIN, INT_MAX);
    m_my_widget = d;
    this->insertToTable();
    m_widget->setRowHeight(m_row, d->rowHeight(0) * 2 + 4);
    QObject::connect(d, SIGNAL(valueChanged(qlonglong, qlonglong)), this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

void gui::table::SadPoint2IDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

