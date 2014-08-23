#include "screensprite.h"

#include "../core/fonttemplatesdatabase.h"
#include "../core/spritedatabase.h"

#include <geometry2d.h>
#include <3rdparty/format/format.h>

#include <db/load.h>
#include <db/save.h>

#include <QString>

DECLARE_SOBJ_INHERITANCE(ScreenSprite, AbstractScreenObject);


ScreenSprite::ScreenSprite()
: m_color(255, 255, 255), 
m_alpha(0),
m_index(0), 
m_rect(sad::Rect2D(sad::Point2D(0,0), sad::Point2D(128,128))),
m_angle(0.0f)
{
	m_observer = NULL;
	
	this->addProperty("color",new sad::db::Field<ScreenSprite, sad::Color>(&ScreenSprite::m_color));
	this->addProperty("alpha",new sad::db::Field<ScreenSprite, int>(&ScreenSprite::m_alpha));

	this->addProperty("config",new sad::db::Field<ScreenSprite, sad::String>(&ScreenSprite::m_config));
	this->addProperty("group" ,new sad::db::Field<ScreenSprite, sad::String>(&ScreenSprite::m_group));
	this->addProperty("index" ,new sad::db::Field<ScreenSprite, int>(&ScreenSprite::m_index));
	
	sad::db::Field<ScreenSprite, sad::Rect2D> * hr = new sad::db::Field<ScreenSprite, sad::Rect2D>(&ScreenSprite::m_rect);
	this->addProperty("rect" , hr);
	sad::db::Field<ScreenSprite, float> * fr =  new sad::db::Field<ScreenSprite, float>(&ScreenSprite::m_angle);
	this->addProperty("angle", fr);
}


ScreenSprite::~ScreenSprite()
{
	delete m_observer;
}

sad::String ScreenSprite::typeName()
{
	return "ScreenSprite";
}



void ScreenSprite::_render()
{
	if (m_observer)
	{	
		m_observer->sprite()->setColor(
			sad::AColor(m_color.r(), m_color.g(), m_color.b(), (sad::uchar)m_alpha)
		);
		m_observer->sprite()->render();
	}
}


sad::String ScreenSprite::_description()
{
	QString a = QString("sprite(%1,%2,%3) at (%4,%5)");
	sad::Point2D  p = this->region()[0];
	a = a.arg(m_config.data())
		 .arg(m_group.data())
		 .arg(QString::number(m_index))
		 .arg(QString::number(p.x()))
 		 .arg(QString::number(p.y()));
	return a.toStdString().c_str();
}


sad::Rect2D ScreenSprite::region()
{
	sad::Rect2D rd = m_rect;
	sad::rotate(rd, m_angle);
	return  rd;	
}

void ScreenSprite::moveCenterTo(const sad::Point2D & point)
{
	if (m_observer)
	{
		sad::Rect2D  hregion = this->region();
 		sad::Point2D middle = (hregion[0] + hregion[2]) / 2; 
		sad::Point2D delta = point - middle;
		m_observer->sprite()->moveBy(delta);
		sad::moveBy(delta, m_rect);
	}
}





bool ScreenSprite::isWithin(const sad::Point2D & p)
{
	sad::Rect2D r = this->region();
	return sad::isWithin(p, r);
}


bool ScreenSprite::isValid(FontTemplateDatabase * db, sad::Vector<sad::String> * errors)
{

	SpriteDatabase & d = db->sprites();
	bool ok = false;
	if (d.configs().contains(m_config.data()))
	{
		if (d.configs()[m_config.data()].contains(m_group.data()))
		{
			ok = d.configs()[m_config.data()][m_group.data()].contains(m_index);
		}
	}
	if (ok == false && errors != NULL)
	{
		(*errors) << str(fmt::Print("Sprite template (\"{0},{1},{2}\") is absent in database\n") 
					  << m_config.data()
					  << m_group.data()
					  << m_index
				     ).c_str();
	}
	return ok;
}



bool ScreenSprite::tryReload(FontTemplateDatabase * db)
{
	sad::Point2D  point(0,0);
	sad::Point2D  size(0,0);
    //bool changesize = false;
	if (m_observer)
	{
		point = m_observer->sprite()->middle();
	}

	delete m_observer;
	m_observer = NULL;
	if (isValid(db))
	{
		this->reloadNoSize(db);
		this->moveCenterTo(point);
		m_observer->sprite()->rotate(m_angle);
		return true;
	}
	return false;
}

void ScreenSprite::reloadNoSize(FontTemplateDatabase * db)
{
	sad::Sprite2DConfig * cfg = db->sprites().hconfigs()[m_config];
	sad::Sprite2DConfigObserver * obs = new sad::Sprite2DConfigObserver(m_group, m_index, cfg);
	obs->createSprite(sad::Point2D(0,0));
	m_observer = obs;
	m_rect = this->m_observer->sprite()->renderableArea();
}

void ScreenSprite::setRotatedRectangle(const sad::Rect2D & rotatedrectangle, float angle)
{
	float mangle = -1 * angle;
	m_rect = rotatedrectangle;
	sad::rotate(m_rect, mangle);	
	m_angle = angle;
	sad::Sprite2DConfigObserver * o = this->observer();
	if (o)
	{
		this->observer()->sprite()->setRenderableArea(rotatedrectangle);
	}
}

void ScreenSprite::initializeGraphicAfterLoad(FontTemplateDatabase * db)
{
	sad::Sprite2DConfig * cfg = db->sprites().hconfigs()[m_config];
	sad::Sprite2DConfigObserver * obs = new sad::Sprite2DConfigObserver(m_group, m_index, cfg);
	obs->createSprite(sad::Point2D(0,0));
	m_observer = obs;
	obs->sprite()->setRenderableArea(m_rect);
	obs->sprite()->rotate(m_angle);
}
