#include "animations/animationsoptionlist.h"
#include "animations/animationsinstance.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"

#include "sprite2d.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <sprite2d.h>

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::OptionList, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::OptionList::OptionList()
{
    m_creators.pushProperty<sad::String>("options", "options");
    m_creators.pushProperty<sad::Rect2D>("texturecoordinates", "texturecoordinates");
}

sad::animations::OptionList::~OptionList()
{
	
}

static sad::db::schema::Schema* AnimationOptionListSchema = NULL;

sad::db::schema::Schema* sad::animations::OptionList::basicSchema()
{
    if (AnimationOptionListSchema == NULL)
    {
        AnimationOptionListSchema = new sad::db::schema::Schema();
        AnimationOptionListSchema->addParent(sad::animations::Animation::basicSchema());

        AnimationOptionListSchema->add(
            "list",
			new sad::db::MethodPair<sad::animations::OptionList, sad::Vector<sad::String> >(
				&sad::animations::OptionList::list,
                &sad::animations::OptionList::setList
            )
        );
		        
        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationOptionListSchema);
    }
    return AnimationOptionListSchema;
}

sad::db::schema::Schema* sad::animations::OptionList::schema() const
{
    return sad::animations::OptionList::basicSchema();
}


bool sad::animations::OptionList::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<sad::Vector<sad::String> > list = picojson::to_type<sad::Vector<sad::String> >(
														picojson::get_property(v, "list")
													  );
		bool result = list.exists();
		if (result)
		{
			setList(list.value());
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::OptionList::setList(const sad::Vector<sad::String>& list)
{
	m_list = list;
    m_inner_valid = m_list.size() != 0;
    this->updateValidFlag();
}

const sad::Vector<sad::String> & sad::animations::OptionList::list() const
{
	return m_list;
}


void sad::animations::OptionList::setState(sad::animations::Instance* i, double time)
{
	double value = static_cast<double>(m_list.size()) * time / m_time;
	unsigned int kvalue = static_cast<unsigned int>(value);
	if (kvalue < m_list.size())
	{
        i->stateCommandAs<sad::String>()->call(m_list[kvalue]);
	}
}


sad::animations::setstate::AbstractSetStateCommand* sad::animations::OptionList::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c = NULL;
        if (o->isInstanceOf("sad::Sprite2D"))
        {
            void (sad::Sprite2D::*f)(const sad::String&) = &sad::Sprite2D::set;
            c = sad::animations::setstate::make(
                    o,
                    f
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setOptions
                    );
            }
            else
            {
                c = new sad::animations::setstate::SetProperty<sad::String>(o, "options");
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::String>();
}


bool sad::animations::OptionList::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::String>("options").exists();
    }
    return result;
}
