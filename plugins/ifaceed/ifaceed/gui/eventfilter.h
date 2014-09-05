/*!\file eventfilter.h
 * \author HiddenSeeker
 *
 * Describes an event filter, used to react
 * to global hot keys in application, like
 * pressing Ctrl+Z, Ctrl+R, Ctrl+<number> and
 * Ctrl+F1 and Ctrl+F2 in future
 */
#pragma once
#include <QObject>
#include <QKeyEvent>

class MainPanel;

namespace gui
{

/*!
 * \class EventFilter
 * An eventfilter used to react
 * to global hot keys in application, like
 * pressing Ctrl+Z, Ctrl+R, Ctrl+<number> and
 * Ctrl+F1 and Ctrl+F2 in future
 */
class EventFilter : public QObject
{
    Q_OBJECT
public:
    /*! Creates new event filter
     */
    explicit EventFilter(QObject *parent = 0);
    /*!
     * Sets panel for event filter
     * \param panel new panel
     */
    void setPanel(MainPanel* panel);
    /*!
     * Handled events for filter
     * \param[in] Object
     * \param[in] Event
     * \return whether event is handled
     */
    virtual bool eventFilter(QObject *o, QEvent *e);
protected:
    /*!
     * A linked panel to event filter
     */
    MainPanel* m_panel;
};

}
