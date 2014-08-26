#include <QFontComboBox>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

#include "ifaceeditor.h"

#include <freetype/font.h>

#include <label.h>
#include <fontmanager.h>
#include <renderer.h>

#include <pipeline/pipelinetask.h>
#include <pipeline/pipeline.h>

#include <db/load.h>
#include <db/save.h>

#include <resource/tree.h>

#include "core/fonttemplatesdatabase.h"
#include "core/editorbehaviour.h"
#include "core/saddythread.h"
#include "core/xmlconfigloader.h"

#include "../objects/screentemplate.h"
#include "../objects/screenlabel.h"
#include "../objects/screensprite.h"

#include "states/idlestate.h"
#include "states/labeladdingstate.h"
#include "states/spriteaddingstate.h"
#include "states/selectedstate.h"

#include "../history/propertychangecommand.h"
#include "../history/deletecommand.h"

#include "objectborders.h"
#include "objectxmlreader.h"
#include "sceneaddingtask.h"

#include "typeconverters/save.h"
#include "typeconverters/load.h"
#include "typeconverters/qcolortosadcolor.h"
#include "typeconverters/qcolortosadacolor.h"
#include "typeconverters/qstringtosadstring.h"
#include "typeconverters/qlistqlistqcolortosadvectorsadvectoracolor.h"
#include "typeconverters/sadcolortoqcolor.h"
#include "typeconverters/sadacolortoqcolor.h"
#include "typeconverters/sadstringtoqstring.h"
#include "typeconverters/sadvectorsadvectoracolortoqlistqlistqcolor.h"





IFaceEditor::IFaceEditor() : m_icons("editor_icons")
{
	m_qttarget = new core::QtTarget(this);
	sad::Renderer::ref()->log()->addTarget(m_qttarget);
	m_cmdargs = NULL;
	m_initmutex = new sad::Mutex();
	m_saddywaitmutex = new sad::Mutex();
	m_renderthread = new core::SaddyThread(this);
	m_waitforqt = false;
	m_waitforsaddy = false;
	m_qtapp = NULL;
	m_history = new EditorHistory();

	m_handling_event = false;
	sad::log::FileTarget * fh = new sad::log::FileTarget("{0}: [{1}] {3}{2}{4}", sad::log::MESSAGE);
	fh->open("user.txt");
	sad::log::Log::ref()->addTarget(fh);
	m_target = new sad::log::FileTarget();
	m_target->open("full.txt");
	sad::log::Log::ref()->addTarget(m_target);

	m_db = NULL;
	m_counter = 0;
	m_result = new ScreenTemplate();
	m_selection_border = NULL;

	core::EditorBehaviour * behaviour = new core::EditorBehaviour(this,"idle");
	behaviour->addState("idle", new IdleState());
	behaviour->addState("label_adding", new LabelAddingState());
	behaviour->addState("selected", new SelectedState());
	behaviour->addState("sprite_adding_simple", new SimpleSpriteAddingState());
	behaviour->addState("sprite_adding_diagonal", new DiagonalSpriteAddingState());
	this->behaviours().insert("main", behaviour);

	// Fill conversion table with converters
	sad::db::ConversionTable::ref()->add(
		"sad::Color", 
		"QColor", 
		new core::typeconverters::SadColorToQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::AColor", 
		"QColor", 
		new core::typeconverters::SadAColorToQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::String", 
		"QString", 
		new core::typeconverters::SadStringToQString()
	);
	sad::db::ConversionTable::ref()->add(
		"sad::Vector<sad::Vector<sad::AColor> >", 
		"QList<QList<QColor> >", 
		new core::typeconverters::SadVectorSadVectorToAColorToQListQListQColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QColor", 
		"sad::Color", 		
		new core::typeconverters::QColorToSadColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QColor", 		
		"sad::AColor", 
		new core::typeconverters::QColorToSadAColor()
	);
	sad::db::ConversionTable::ref()->add(
		"QString",
		"sad::String",  
		new core::typeconverters::QStringToSadString()
	);
	sad::db::ConversionTable::ref()->add(
		"QList<QList<QColor> >", 
		"sad::Vector<sad::Vector<sad::AColor> >", 
		new core::typeconverters::QListQListQColorToSadVectorSadVectorToAColor()
	);

}
IFaceEditor::~IFaceEditor()
{
	delete m_db;
	delete m_result;
	for (sad::Hash<sad::String, core::EditorBehaviour*>::iterator it =m_behaviours.begin();it!=m_behaviours.end();it++)
	{
		delete it.value();
	}
	delete m_cmdoptions;
	delete m_saddywaitmutex;
	delete m_qtapp;
	delete m_initmutex;
	delete m_renderthread;
	delete m_cmdargs;
	delete m_history;
	delete m_behavioursharedata;
}

void IFaceEditor::setDatabase(FontTemplateDatabase * db)
{
	delete m_db;
	m_db = db;
}

void IFaceEditor::initSaddyRendererOptions()
{
	sad::Renderer::ref()->init(sad::Settings(WINDOW_WIDTH, WINDOW_HEIGHT, false));
	sad::Renderer::ref()->setWindowTitle("Saddy Interface Editor");
	sad::Renderer::ref()->makeFixedSize();
	this->assertSaddyInit(true);
}

QMainWindow * IFaceEditor::createQtWindow()
{
	
	return new MainPanel();
}

MainPanel * IFaceEditor::panel()
{
	return static_cast<MainPanel*>(this->qtWindow());
}


void IFaceEditor::quit()
{
	sad::Renderer::ref()->quit();
}

sad::cli::Parser * IFaceEditor::createOptionParser()
{
	sad::cli::Parser * r = new sad::cli::Parser();
	r->addSingleValuedOption("ifaceconfig");
	r->addFlag("debug");
	return r;
}


// A future result of loading database
class DBLoadingTaskFuture
{
 protected:
	 bool m_result;   //!< Result of computation
	 bool m_computed; //!< Computed result
 public:
	 inline DBLoadingTaskFuture()
	 {
		 m_result = false;
		 m_computed = false;
	 }

	 inline void setResult(bool result) 
	 {
		 m_result = result;
		 m_computed = true;
	 }

	 inline bool result() 
	 {
		 while (!m_computed) {}
		 return m_result;
	 }
};


// An aynchronous task used to loading some sprite database. Because loading must be performed in
// a rendering thread, because no OpenGL context is available to other threads
class DBLoadingTask: public sad::pipeline::AbstractTask
{
 protected:
	 FontTemplatesMaps * m_maps; //!< Maps data
	 FontTemplateDatabase * m_db;  //!< Database for loading
	 DBLoadingTaskFuture * m_future; //!< Future for computing
	 sad::log::Log * m_log;    //!< Logger for logging data
 public:
	 /** Constructs new tasks
	  */
	 inline DBLoadingTask(FontTemplatesMaps * maps, 
						  FontTemplateDatabase * db, 
						  DBLoadingTaskFuture * f, 
						  sad::log::Log * log)
	 {
		 m_maps = maps;
		 m_db = db;
		 m_future = f;
		 m_log = log;
	 }
	 // Loads a db
	 virtual void _process()
	 {
	    bool data = m_db->load(*m_maps, m_log);
		m_future->setResult(data);
	 }
	 virtual ~DBLoadingTask()
	 {
	 }
};


void IFaceEditor::reportResourceLoadingErrors(
		sad::Vector<sad::resource::Error *> & errors,
		const sad::String& configname
)
{
	sad::String errorlist = sad::resource::format(errors);
	sad::String resultmessage = "There was errors while loading ";
	resultmessage += configname;
	resultmessage += ":\n";
	resultmessage += errorlist;
	sad::util::free(errors);
	SL_FATAL(resultmessage);
	QTimer::singleShot(0, this->panel(), SLOT(close()));
}

void IFaceEditor::onFullAppStart()
{
	if (this->parsedArgs()->single("ifaceconfig").value().length() == 0)
	{
		SL_WARNING("Config file is not specified. You must choose it now");
		QString config = QFileDialog::getOpenFileName(this->panel(),"Choose a config file",QString(),
													  ("Configs (*.json)"));
		if (config.length() == 0) 
		{
			SL_FATAL("Config file is not specified. Quitting...");
			QTimer::singleShot(0, this->panel(), SLOT(close()));
			return;
		} 
		else 
		{
			this->parsedArgs()->setSingleValuedOption("ifaceconfig", config.toStdString().c_str());
		}
	}
	bool success = true;
	sad::resource::Tree * resourcetree = new sad::resource::Tree();
	resourcetree->factory()->registerResource<sad::freetype::Font>();
	resourcetree->setStoreLinks(true);
	
	sad::Renderer * renderer = sad::Renderer::ref();
	renderer->addTree("resources", resourcetree);
	sad::Vector<sad::resource::Error *> errors = resourcetree->loadFromFile("resources/resources.json");
	if (errors.count())
	{
		reportResourceLoadingErrors(errors, "resources.json");
		return;
	}

	// Load first stage - a maps of handling all of data
	sad::String configname = this->parsedArgs()->single("ifaceconfig").value();
	renderer->tree()->factory()->registerResource<sad::freetype::Font>();
	errors = renderer->tree()->loadFromFile(configname);
	if (errors.count())
	{
		reportResourceLoadingErrors(errors, "config");
		success = false;
	}
	
	if (success) {
		this->panel()->setEditor(this);
		this->panel()->synchronizeDatabase();
				
		class IFaceMouseMoveHandler 
		: public sad::input::AbstractHandlerForType<sad::input::MouseMoveEvent>
		{
		 public:
			IFaceMouseMoveHandler(IFaceEditor * ed) : m_editor(ed)
			{
			}
			virtual void invoke(const sad::input::MouseMoveEvent & ev)
			{
				if (m_editor) 
				{
					MainPanel * p = m_editor->panel();
					p->setMouseMovePosView(ev.pos2D().x(),ev.pos2D().y());
				}
				m_editor->currentBehaviour()->onMouseMove(ev);
			}
		 protected:
		   IFaceEditor * m_editor;
		} * handler = new IFaceMouseMoveHandler(this);
		class IFaceKeyDownHandler
		: public sad::input::AbstractHandlerForType<sad::input::KeyPressEvent>
		{
		 public:
			IFaceKeyDownHandler(IFaceEditor * ed) : m_editor(ed)
			{
			}
			void commitInEditor()
			{
				CLOSURE
				CLOSURE_DATA(IFaceEditor * e;)
				CLOSURE_CODE(e->history()->commit(e); )
				INITCLOSURE( CLSET(e, m_editor); );
				SUBMITCLOSURE( m_editor->emitClosure );
			}
			void rollbackInEditor()
			{
				IFaceEditor * ed = static_cast<IFaceEditor*>(m_editor);
				CLOSURE
				CLOSURE_DATA(IFaceEditor * e;)
				CLOSURE_CODE(e->history()->rollback(e); )
				INITCLOSURE( CLSET(e, m_editor); );
				SUBMITCLOSURE( m_editor->emitClosure );
			}
			virtual void invoke(const sad::input::KeyPressEvent & ev)
			{
				bool handled = false;
				if (m_editor) 
				{
					if (m_editor->behaviourSharedData()->activeObject() == NULL
						&& m_editor->behaviourSharedData()->selectedObject() == NULL)
					{
						if (ev.Key == sad::Esc)
						{
							handled = true;
							m_editor->quit();
						}
					}
					if (ev.Key == sad::Z && ev.CtrlHeld)
					{
						handled = true;
						this->rollbackInEditor();
					}
					if (ev.Key == sad::R && ev.CtrlHeld)
					{
						handled = true;
						this->commitInEditor();
					}
				}
				if (!handled)
					m_editor->currentBehaviour()->onKeyDown(ev);
			}
		protected:
		   IFaceEditor * m_editor;
		} * kbdhandler = new IFaceKeyDownHandler(this);


		sad::input::Controls * c = sad::Renderer::ref()->controls();
		c->add(*sad::input::ET_MouseMove, handler);
		c->add(*sad::input::ET_KeyPress, kbdhandler);		
		c->add(*sad::input::ET_KeyRelease, this, &IFaceEditor::currentBehaviour, &core::EditorBehaviour::onKeyUp);
		c->add(*sad::input::ET_MouseWheel, this, &IFaceEditor::currentBehaviour, &core::EditorBehaviour::onWheel);
		c->add(*sad::input::ET_MousePress, this, &IFaceEditor::currentBehaviour, &core::EditorBehaviour::onMouseDown);
		c->add(*sad::input::ET_MouseRelease, this, &IFaceEditor::currentBehaviour, &core::EditorBehaviour::onMouseUp);

		m_selection_border = new SelectedObjectBorder(this->shdata());
		sad::Renderer::ref()->pipeline()->append( m_selection_border );
		sad::Renderer::ref()->pipeline()->appendProcess(this, &IFaceEditor::tryRenderActiveObject);
		sad::Renderer::ref()->pipeline()->append( new ActiveObjectBorder(this->shdata()) );

		this->setBehaviour("main");
		this->highlightState("Idle");

	}
}

FontTemplateDatabase * IFaceEditor::database()
{
	return m_db;
}

void IFaceEditor::highlightState(const sad::String & hint)
{
	this->panel()->highlightState(hint);
}


void IFaceEditor::tryRenderActiveObject()
{
	AbstractScreenObject * o =	this->behaviourSharedData()->activeObject();
	if (o)
		o->render();
}
void IFaceEditor::tryEraseObject()
{
	sad::String state = this->currentBehaviour()->state(); 
	if (state == "label_adding" 
		|| state == "sprite_adding_simple" 
		|| state == "sprite_adding_diagonal")
	{
		AbstractScreenObject * o =	this->behaviourSharedData()->activeObject();
		delete o;
		this->behaviourSharedData()->setActiveObject(NULL);
		this->currentBehaviour()->cancelState();
	}
	if (state == "selected")
	{
		AbstractScreenObject * o =	this->behaviourSharedData()->selectedObject();
		this->behaviourSharedData()->setSelectedObject(NULL);
		DeleteCommand * cmd = new DeleteCommand(this->result(), o);
		this->history()->add(cmd);
		cmd->commit(this);
		this->currentBehaviour()->enterState("idle");
	}
}

void IFaceEditor::submitEvent(UNUSED const sad::String & eventType,UNUSED const sad::db::Variant & v)
{
	CLOSURE
	CLOSURE_DATA( IFaceEditor * me; )
	CLOSURE_CODE( 
		SL_SCOPE("IFaceEditor::submitEvent()::closure");
		if (me->m_handling_event)
			return;
		me->m_handling_event = true;
		me->panel()->updateList(); 
		if (me->behaviourSharedData()->selectedObject() != NULL )
		{
			if (me->behaviourSharedData()->activeObject() == NULL)
			{
				SL_SCOPE("IFaceEditor::submitEvent()::closure::callUpdateObjectStats()");
				me->panel()->updateObjectStats(me->behaviourSharedData()->selectedObject());
			}
			// Remove order, if selected removed
			sad::log::Log* lg = sad::log::Log::ref();
			if (me->shdata()->selectedObject()->prop<bool>("activity",lg) == false
			   )
			{
				SL_SCOPE("IFaceEditor::submitEvent()::closure::fixingSelected()");
				if (me->currentBehaviour()->state() == "selected")
				{
					SL_DEBUG("Entering idle state");
					me->currentBehaviour()->enterState("idle");
				}
				SL_DEBUG("Unselecting object to null");
				me->shdata()->setSelectedObject(NULL);
			}
		}
		me->m_handling_event = false;
	)
	INITCLOSURE ( CLSET(me, this) )
	SUBMITCLOSURE( this->emitClosure );
}


core::EditorBehaviourSharedData * IFaceEditor::createBehaviourData()
{
	IFaceSharedData * e = new IFaceSharedData();
	e->setEditor(this);
	return e;
}

IFaceSharedData * IFaceEditor::shdata()
{
	return static_cast<IFaceSharedData *>(this->behaviourSharedData());
}

void IFaceEditor::appendRotationCommand()
{
	float new_angle = 0.0f;
	float old_angle = 0.0f;
	AbstractScreenObject * o = NULL;
	sad::log::Log* lg = sad::log::Log::ref();
	this->shdata()->getAndDismissRotationCommand(o, new_angle, old_angle);
	this->history()->add(new PropertyChangeCommand<float>(o, "angle", new_angle, old_angle, lg));
}


void IFaceEditor::reload()
{
   SL_SCOPE("IFaceEditor::reload()");
   // 1. Load maps
   FontTemplatesMaps * maps =  new FontTemplatesMaps(); 
   sad::String filename = this->parsedArgs()->single("ifaceconfig").value();
   sad::log::Log* lg = sad::log::Log::ref();
   if (maps->load(filename.data(), lg) 
	   == false) {
		// 2. If map loading failed, stop right there
	    // 2.1. Report   error
	    delete maps;
		SL_WARNING(str(fmt::Print("Map file \"{0}\": loading failed") << filename.data()));
		return;
   }
   // 3. Load texture database
   m_counter++;
   FontTemplateDatabase * db = new FontTemplateDatabase(&m_counter);		
   DBLoadingTaskFuture * future = new DBLoadingTaskFuture();
   DBLoadingTask * task = new DBLoadingTask(maps,db,future, lg);
   sad::Renderer::ref()->lockRendering();
   sad::Renderer::ref()->pipeline()->prepend(task);
   sad::Renderer::ref()->unlockRendering();
   if (future->result() == false) {
		// 3.1. If loading failed, report error	 
	    delete maps;
		delete future;
		delete db;
		sad::String filename = this->parsedArgs()->single("ifaceconfig").value();
		SL_WARNING(str(fmt::Print("Map file \"{0}\": loading font and templates failed") << filename));
		return;
   }
   // At this point we need only database, db to
   // check compliance with other objects
   delete maps;
   delete future;
   // 4. Check, whether all scene needed data in DB
   bool allobjectsvalid = true;
   // Container of errors
   sad::Vector<sad::String> errors;
   AbstractScreenObject * it = this->result()->templateBegin();	
   while (it)
   {
		allobjectsvalid = allobjectsvalid && it->isValid(db, &errors);
		it = this->result()->templateNext();
   }
   if (!allobjectsvalid)
   {
	   // 4.1. If failed, report error
	   sad::String errorsasstring = "Not all objects are valid:\n";
       for(unsigned int i = 0; i < errors.count(); i++)
	   {
			errorsasstring << errors[i];
	   }
	   SL_WARNING(errorsasstring);
	   delete db;
	   return;
   }
   // 5. Reload scene data for db
   it = this->result()->templateBegin();	
   while (it)
   {
	    it->tryReload(db);
		it = this->result()->templateNext();
   }
   // 6. Remove old DB
   this->setDatabase(db);
   /*
	  7. Reload fonts in UI 
	  8. Reload sprites in UI
    */
   this->panel()->synchronizeDatabase();
   /**	 
	 9. Toggle selected object, if in selected state. This for resetting object data
   */
   if (this->currentBehaviour()->state() == "selected")
   {
   	   this->panel()->updateObjectStats(this->behaviourSharedData()->selectedObject());
   }
}


void IFaceEditor::save()
{
	QString filename = QFileDialog::getSaveFileName(
		NULL, 
		"Please specify save file name",
		"",
		QString("XML (*.xml)")
		);
	if (filename.length())
	{
		ObjectXMLWriter w(filename, "screentemplate");
		if(w.write(this->result()) == false)
		{
			SL_WARNING(QString("Cannot write into file \"") + filename + "\"");
		}
	}
}

void IFaceEditor::load()
{
	QString filename = QFileDialog::getOpenFileName(
		NULL, 
		"Please specify save file name",
		"",
		QString("XML (*.xml)")
		);
	if (filename.length())
	{
		ScreenObjectXMLReader  r(filename);
		ScreenTemplate * e = new ScreenTemplate();
		if (r.read(e) == false)
		{
			delete e;
			SL_WARNING(QString("Cannot load  file \"") + filename + "\"");
			return;
		}
		// We must save our uids, to force it to work with container 
		// Also, we check when all objects are valid and can be loaded from db
		AbstractScreenObject * it = e->templateBegin();
		bool allobjectsarevalid = true;
		sad::Vector<sad::Pair<sad::String, AbstractScreenObject *> > m_pairs;
		sad::log::Log* lg = sad::log::Log::ref();
		while(it)
		{
			allobjectsarevalid = allobjectsarevalid && it->isValid(this->database());
			// We cannot perform ::setUid here, because it will broke some iterator
			sad::String uid = it->prop<sad::String>("uid", lg);
			m_pairs << sad::Pair<sad::String, AbstractScreenObject *>(uid, it);
			// This must be done, because we data added via
			// HashBasedSerializableContainer::add, which don't care
			// to some reference-counting, but real container cares about it
			// in destructor, so we need to increment counter
			// to make object survive death of his container or scene
			it->addRef();
			it = e->templateNext();
		}
        for(unsigned int i = 0; i < m_pairs.count(); i++)
		{
			e->setUid(m_pairs[i].p2(), m_pairs[i].p1());
		}
		if (!allobjectsarevalid)
		{
			delete e;
			SL_WARNING(QString("Not all objects can be mapped to database, aborting"));
			return;
		}
		// Reload an objects
		it = e->templateBegin();
		while(it)
		{
			it->initializeGraphicAfterLoad(this->database());
			it = e->templateNext();
		}
		
		// Clear history
		this->history()->clear();
		// Clear result
		delete m_result;
		m_result = e;
		// Perform cleanup data
		this->currentBehaviour()->enterState("idle");
		sad::Renderer::ref()->lockRendering();
		if (this->scene()->objectCount() != 0)
			this->scene()->clear();
		// Add post-render task, which adds a sorted results when scene is empty and dies
		sad::Renderer::ref()->pipeline()->append(new SceneAddingTask(e, this->scene()));
		sad::Renderer::ref()->unlockRendering();
	}
}

sad::cli::Parser * IFaceEditor::parsedArgs() const
{
	return m_cmdoptions;
}

void IFaceEditor::init(int argc,char ** argv)
{
	// Create dependent behaviour data
	m_behavioursharedata = this->createBehaviourData();
	// Firstly we create an arguments and application
	// to strip all of Qt's options, which wouldn't break a parser, after this work
	m_cmdargs = new sad::cli::Args(argc, argv);
	this->m_qtapp = new QApplication(this->m_cmdargs->count(),
									(this->m_cmdargs->arguments()));

	m_cmdoptions = this->createOptionParser();
	m_cmdoptions->parse(argc, (const char **)argv);
	
	this->assertSaddyInit(true);
	this->m_waitforsaddy = true;
	
	
	// This thread also runs ALL of event loops
	m_waitforsaddy = true;
	m_renderthread->start();
	this->waitForSaddyThread();
	if (this->saddyInitSuccessfull()) {
		this->initQtActions();
		m_waitforsaddy = true;
		this->awakeSaddyThread();
		this->waitForSaddyThread();
		this->runQtEventLoop();
	}
	m_renderthread->wait();

}
void IFaceEditor::waitForSaddyThread()
{
	while(this->shouldMainThreadWaitForSaddy());
}

void IFaceEditor::initSaddyActions() 
{
	this->initSaddyRendererOptions();
}

void IFaceEditor::initQtActions() 
{
	this->m_mainwindow = this->createQtWindow();
	this->bindQtSlots();
	this->awakeSaddyThread();
}


void IFaceEditor::bindQtSlots()
{
}

void IFaceEditor::runQtEventLoop()
{
	if (this->m_mainwindow) 
	{
		if (this->m_qtapp) 
		{
			QObject::connect(this->m_qtapp,SIGNAL(lastWindowClosed()),this,SLOT(qtQuitSlot()));
		}
		this->m_mainwindow->show();
	}

	if (this->m_qtapp) 
	{
		QObject::connect(this, SIGNAL(closureArrived(sad::ClosureBasic*)), this, SLOT(onClosureArrived(sad::ClosureBasic*)) );
		m_qttarget->enable();
		QTimer::singleShot(0,this,SLOT(onFullAppStart()));
		this->m_qtapp->exec();
		m_qttarget->disable();
	}
}

void IFaceEditor::runSaddyEventLoop() 
{
	m_quit_reason = core::QR_NOTSET;
	sad::Renderer::ref()->controls()->add(*sad::input::ET_Quit, this, &IFaceEditor::onSaddyWindowDestroySlot);
	sad::Renderer::ref()->run();
	// Quit reason can be set by main thread, when window is closed
	if (m_quit_reason == core::QR_NOTSET)
		this->saddyQuitSlot();
}

void IFaceEditor::onSaddyWindowDestroySlot()
{
	this->onSaddyWindowDestroy();
}

void IFaceEditor::initDefaultSaddyOptions()
{
	sad::Settings sett(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	sad::Renderer::ref()->init(sett);
	this->m_scene = new sad::Scene();
	sad::Renderer::ref()->setScene(this->m_scene);
	sad::Renderer::ref()->makeFixedSize();
	// Try to load default icons
	QString a = QApplication::applicationDirPath();
	a = QDir(a).filePath(ICONS_XML);
	XMLConfigLoader * loader = new XMLConfigLoader(a);
	m_icons.setLoader(loader);
	bool loaded =  (m_icons.reload() == sad::SCR_OK);
	SL_DEBUG(QString("Loading icons from %1").arg(a));
	if (!loaded) 
	{
		SL_FATAL(QString("Can\'t load %1").arg(a));
	}
	this->assertSaddyInit(loaded);
}

sad::Sprite2DConfig & IFaceEditor::icons()
{
	return m_icons;
}

void IFaceEditor::saddyQuitSlot()
{
	if (m_quit_reason == core::QR_NOTSET) {
		m_quit_reason = core::QR_SADDY;
		QTimer::singleShot(0,this,SLOT(onQuitActions()));
	}
}
void IFaceEditor::qtQuitSlot()
{
	if (m_quit_reason == core::QR_NOTSET) {
		m_quit_reason = core::QR_QTWINDOW;
		this->onQuitActions();
	}
}
void IFaceEditor::onQuitActions()
{
	this->onQtWindowDestroy();
	if (m_quit_reason == core::QR_SADDY) {
		this->m_mainwindow->close();
	}
	if (m_quit_reason == core::QR_QTWINDOW) {
		sad::Renderer::ref()->quit();
	}
	this->quitSaddyActions();
	this->quitQtActions();
}

void IFaceEditor::eraseBehaviour()
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	m_current_behaviour.clear();
}

void IFaceEditor::setBehaviour(const sad::String & name)
{
	if (m_current_behaviour.length())
	{
		m_behaviours[m_current_behaviour]->deactivate();
	}
	if (behaviours().contains(name))
	{
		m_current_behaviour = name;
		m_behaviours[m_current_behaviour]->activate();
	}
	else 
	{
		SL_DEBUG(QString("Can\'t find editor behaviour, named %1").arg(name.data()));
	}
}

core::EditorBehaviour nullBehaviour(NULL,"");

core::EditorBehaviour * IFaceEditor::currentBehaviour()
{
	if (m_current_behaviour.length())
	{
		return m_behaviours[m_current_behaviour];
	}
	return &nullBehaviour;
}

void IFaceEditor::onClosureArrived(sad::ClosureBasic * closure)
{
	closure->run();
	delete closure;
}


void IFaceEditor::emitClosure(sad::ClosureBasic * closure)
{
	emit closureArrived(closure);
}

void IFaceEditor::onSaddyWindowDestroy()
{

}

void IFaceEditor::quitQtActions()
{

}
void IFaceEditor::quitSaddyActions()
{

}

void IFaceEditor::onQtWindowDestroy()
{

}
