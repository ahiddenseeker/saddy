#include "spriteaddingstate.h"
#include "../ifaceeditor.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "../../objects/abstractscreenobject.h"
#include "../../mainpanel.h"
#include "../../history/newcommand.h"
#include "../../objects/screensprite.h"

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

void SimpleSpriteAddingState::enter()
{
	IFaceEditor * ed = this->editor();
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  ed->highlightState("Place a sprite"); 
				  p->setRegionParameters();  
				  p->setSpriteChangingEnabled(false);
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	o->moveCenterTo(hPointF(ev.x,ev.y));

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  ed->highlightState("Place a sprite"); 
				  p->setRegionParameters();  
				 )
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}

void SimpleSpriteAddingState::onWheel(const sad::Event & ev)
{
	float dangle = (ev.delta < 0)? (- ROTATION_ANGLE_STEP ) : ROTATION_ANGLE_STEP;
	IFaceEditor * ed = this->editor();
	MainPanel * p = ed->panel();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	float a = o->getProperty("angle")->get(ed->log())->get<float>(ed->log());
	a+=dangle;
	CLOSURE
	CLOSURE_DATA( MainPanel * p; float angle; )
	CLOSURE_CODE( p->myUI()->dblAngle->setValue(angle); p->setRegionParameters();   )
	INITCLOSURE( CLSET(p,p); CLSET(angle,a) )
	SUBMITCLOSURE( ed->emitClosure );
	o->getProperty("angle")->set(sad::Variant((float)a),ed->log());
}

void SimpleSpriteAddingState::onMouseDown(UNUSED const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o =	ed->behaviourSharedData()->activeObject();
	NewCommand * c = new NewCommand(ed->result(), o);
	ed->history()->add(c);
	c->commit(ed->log(), ed);
	ed->behaviourSharedData()->setActiveObject(NULL);
	ed->behaviourSharedData()->setSelectedObject(o);
	this->behaviour()->enterState("selected");
}

void SimpleSpriteAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		this->editor()->tryEraseObject();
	}
}

void SimpleSpriteAddingState::leave()
{
	IFaceEditor * ed = this->editor();
	this->shdata()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); p->setSpriteChangingEnabled(true); )
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}

void DiagonalSpriteAddingState::enter()
{
	m_substate = DSAS_INITIAL;
	IFaceEditor * ed = this->editor();
	ed->behaviourSharedData()->activeObject()->setProp<float>("angle", 0.0f, ed->log());
	ed->behaviourSharedData()->activeObject()->setProp<bool>("visibility", false, ed->log());
	ed->behaviourSharedData()->toggleActiveBorder(false);

	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; IFaceEditor * ed; )
	CLOSURE_CODE( p->setAddingEnabled(false); 
				  p->setAngleChangingEnabled(false); 
				  p->setSpriteChangingEnabled(false);
				  ed->highlightState("Place a first point");  
				)
	INITCLOSURE( CLSET(p,p); CLSET(ed,ed); )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseDown(const sad::Event & ev)
{
	DiagonalSpriteAddingSubState ss = m_substate;
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o = ed->behaviourSharedData()->activeObject();
	ScreenSprite * oo = static_cast<ScreenSprite *>(o);
	hPointF p(ev.x, ev.y);
	bool highlight = false;
	hst::string highlights;
	if (ss == DSAS_INITIAL)
	{
		ed->behaviourSharedData()->toggleActiveBorder(true);

		hRectF brect = oo->rect();
		hPointF ddelta = p - brect[3];
		for(int i = 0; i < 4; i++)
		{
			brect[i] += ddelta;
		}
		oo->setProp<bool>("visibility", true, ed->log());
		oo->setRotatedRectangle(brect, 0.0f);
		m_substate = DSAS_FIRSTCLICK;
		highlight = true;
		highlights = "Now place the bottom right point";
	}
	if (ss == DSAS_FIRSTCLICK)
	{
		this->onMouseMove(ev);
		NewCommand * c = new NewCommand(ed->result(), o);
		ed->history()->add(c);
		c->commit(ed->log(), ed);
		ed->behaviourSharedData()->setActiveObject(NULL);
		ed->behaviourSharedData()->setSelectedObject(o);
		this->behaviour()->enterState("selected");
	}
	CLOSURE
	CLOSURE_DATA( MainPanel * p; bool h; hst::string hs; )
	CLOSURE_CODE(
		p->setRegionParameters();   
		if (h)
			p->highlightState(hs);
	)
	INITCLOSURE( CLSET(p,ed->panel()); CLSET(h, highlight); CLSET(hs, highlights);  )
	SUBMITCLOSURE( ed->emitClosure );
}

void DiagonalSpriteAddingState::leave()
{
	IFaceEditor * ed = this->editor();
	ed->behaviourSharedData()->toggleActiveBorder(true);
	ed->behaviourSharedData()->setActiveObject(NULL);
	MainPanel * p = ed->panel();
	CLOSURE
	CLOSURE_DATA( MainPanel * p; )
	CLOSURE_CODE( p->setAddingEnabled(true); 
	              p->setAngleChangingEnabled(true); 
				  p->setSpriteChangingEnabled(true);
				)
	INITCLOSURE( CLSET(p,p) )
	SUBMITCLOSURE( ed->emitClosure );
}


void DiagonalSpriteAddingState::onMouseMove(const sad::Event & ev)
{
	IFaceEditor * ed = this->editor();
	AbstractScreenObject * o = ed->behaviourSharedData()->activeObject();
	ScreenSprite * oo = static_cast<ScreenSprite *>(o);
	if (m_substate == DSAS_FIRSTCLICK)
	{
		hPointF newpoint(ev.x, ev.y);
		hRectF nrect = oo->rect();
		nrect[1] = newpoint;
		nrect[0].setX(nrect[3].x());
		nrect[0].setY(nrect[1].y());
		nrect[2].setX(nrect[1].x());
		nrect[2].setY(nrect[3].y());
		oo->setRotatedRectangle(nrect, 0.0f);


		CLOSURE
		CLOSURE_DATA( MainPanel * p; )
		CLOSURE_CODE(p->setRegionParameters();   )
		INITCLOSURE( CLSET(p,ed->panel());  )
		SUBMITCLOSURE( ed->emitClosure );
	}
}

void DiagonalSpriteAddingState::onKeyDown(const sad::Event & ev)
{
	if (ev.key == KEY_ESC) 
	{
		this->editor()->tryEraseObject();
	}
}
