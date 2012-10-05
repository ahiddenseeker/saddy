/** \file   ifaceeditor.h
	\author HiddenSeeker

	Defines a main interface editor class
 */
#include "../editorcore/editor.h"
#include "../mainpanel.h"
#include "ifacecmdoptions.h"

class FontTemplateDatabase;

class IFaceEditor: public Editor
{
	Q_OBJECT
 public:
	/** Constructs editor with empty db
	 */
	IFaceEditor();
	/** Frees memory from db
	 */
	~IFaceEditor();
	/** Returns a database fwith all of resources
	 */
	FontTemplateDatabase * database();
 protected:
	/**	Counter for loading all of dbs
	 */
    int m_counter;
	/** A template database with fonts
	 */
	FontTemplateDatabase * m_db;
	/** Sets a database for templates
		\param[in] db database
	 */
	void setDatabase(FontTemplateDatabase * db);
	/** Creates a parser to parse command options
      */
    virtual CommandLineOptions * createOptionParser();
	/** Sets my options and my data
	 */
	void initSaddyRendererOptions();
	/** Returns new main window
	    \return MainPanel
	 */
	QMainWindow * createQtWindow();
	/** Returns editor qt window
		\return MainPanel
	 */
	MainPanel * panel();
	/** Creates a new Qt slots for working
	  */
    void bindQtSlots();
	 /** REIMPLEMENT. the action, when user closes window of qt
	   */
	void onQtWindowDestroy();
	/** REIMPLEMENT. the action, when user closes window of qt
	  */
	void onSaddyWindowDestroy();
	/** REIMPLEMENT this function to do work, when qt window quit 
	  */
	void quitQtActions();
	/** REIMPLEMENT this function to do work, when saddy window quit 
	  */
	void quitSaddyActions();
	/** Returns parsed options
		\return parsed options
	  */
	inline IFaceCmdOptions * cmdLineOptions()
	{
		return static_cast<IFaceCmdOptions *>(this->commandLineOptions());
	}
	/** A function, which is called, when app fully started from main start
	  */
	virtual void onFullAppStart();
 public:
	/** Quits an editor
	 */
	void quit(const sad::Event & ev = sad::Event());
	/** Highlights a state of editor in GUI. Override this for custom output
		\param[in] hint hint of state
	  */
	virtual void highlightState(const hst::string & hint);
};

/** A special handler for method
 */
typedef HandlerFor<IFaceEditor>::Method<void (IFaceEditor::*)(const sad::Event &o),
										sad::Event
										> IFaceEditorHandler;