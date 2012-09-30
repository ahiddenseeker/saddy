/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"


class IFaceEditor;

class MockSpriteTableWidget;

/*! A main window to edit level objects
 */
class MainPanel : public QMainWindow
{
	Q_OBJECT
private:
	/** A sprite table data
	 */
	MockSpriteTableWidget * m_sprite_table;
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

	/** Synchronizes database with an editor
	 */
	void synchronizeDatabase();
private:
	Ui::MainPanelClass ui;         //!< UI
	IFaceEditor    *   m_editor;   //!< Editor to work with
protected:
	/**
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
protected slots:
	/**
	 * Adds new color for fonts in combo box
	 */
	void addNewFontColor();
	/**
	 * Adds new font size for font in combo box
	 */ 
	void addNewFontSize();
};

#endif // MAINPANEL_H
