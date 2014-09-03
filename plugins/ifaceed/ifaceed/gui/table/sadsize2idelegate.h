/*! \file sadsize2idelegate.h
    \author HiddenSeeker

    A delegate for property, which contains sad::Size2I values
 */
#pragma once
#include "delegate.h"

#include "../../history/propertychangecommand.h"

#include "../tuplewidget/int64tuplewidget.h"

#include <QObject>

#include <sadpoint.h>

namespace gui
{

namespace table
{
/*! A delegate for editing values for sad::Size2I values
 */
class SadSize2IDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
    /*! Constructs new delegate
     */
    SadSize2IDelegate();
    /*! Destroys delegate
     */
    virtual ~SadSize2IDelegate();
    /*! Sets a value for a delegate. Used by commands to reset
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v);
public slots:
    /*! When widget changed
        \param[in] f a first value
        \param[in] s a second value
     */
    void widgetChanged(qlonglong f, qlonglong s);
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
