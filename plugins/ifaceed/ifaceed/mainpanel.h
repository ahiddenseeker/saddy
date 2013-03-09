/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H


#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"
#include "templates/hstring.h"
#include "gui/spritewidget/qspritetablewidget.h"
#include <marshal/variant.h>
#pragma once


class IFaceEditor;
class AbstractScreenObject;
class MockSpriteTableWidget;

/*! A main window to edit level objects
 */
class MainPanel : public QMainWindow
{
	Q_OBJECT
private:
	/** Data for moving x
	 */
	float m_tmp_mov_x;
	/** Data for moving y
	 */
	float m_tmp_mov_y;
	/** State of main panel (temporary for slot)
	 */
	QString m_tmp_state;
public:
	/*! Constructor, which inits all of callbacks
	 */
	MainPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
	/*! Currently does nothing
	 */
	~MainPanel();

	/** Sets an editor to work with
		\param[in] editor editor
	 */
	inline void setEditor(IFaceEditor * editor) {  m_editor = editor; }
	
	/*! Synchronizes database with an editor
	 */
	void synchronizeDatabase();
	/*! Sets labels, that performed on mouse move
		\param[in] x x coordinates
		\param[in] y y coordinates
	 */
	void setMouseMovePosView(float x, float y);
	/*! Hints current editor state for highlighting
	 */
	void highlightState(const hst::string & hints);
	/*! Sets, whether adding or removing is enabled
		\param[in] enabled enabled stuff
	 */
	void setAddingEnabled(bool enabled);
	/*! Tries to set current propertit of active object, otherwise  - of selected
		if property exists
		\param[in] prop property name
		\param[in] v value
	 */
	template<typename T> void trySetProperty(const hst::string & prop, T v);
	inline Ui::MainPanelClass * myUI() { return &ui; }

	void updateObjectStats(AbstractScreenObject * o);
private:
	Ui::MainPanelClass ui;         //!< UI
	IFaceEditor    *   m_editor;   //!< Editor to work with
protected:
	/**
	 * An utility flag to prevent events, from self-changing 
	 */
	bool m_selfchanged;
	/**
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
    QSpriteTableWidget* m_spriteTableWidget; //!< Modified QTableWidget. Works with configs, groups, indexes
protected slots:
	void spriteSelected(QString config, QString group, int index);
	/**
	 * Adds new color for fonts in combo box
	 */
	void addNewFontColor();
	/**
	 * Adds new font size for font in combo box
	 */ 
	void addNewFontSize();
	/** Implementation of setMouseMovePosView
	 */
	void setMouseMovePosViewImpl();
	/** Implementation of highlightState
	 */
	void highlightStateImpl();
	/*! Adds a new font object
	 */
	void addFontObject();
	/*! Changes a font in data
		\param[in] s string
	 */ 
	void fontChanged(const QString & s);
	/*! Changed an angle in data
		\param[in] angle angle
	 */
	void angleChanged(double angle);
	/*! Whether color is changed
		\param[in] index of color
	 */
	void colorChanged(int index);
	/*! Whether size is changed
		\param[in] index of size
	 */
	void sizeChanged(int index);
	/*! Whether label object text changed
	 */
	void textChanged();
};

#endif // MAINPANEL_H
