/*! \file db/save,h
	\author HiddenSeeker
	
	Describes a save operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "../sadstring.h"
#include "../sadrect.h"
#include "../sadcolor.h"
#include "../sadsize.h"

namespace sad
{

namespace db
{
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
template<typename _Type>
inline picojson::value Save<_Type>::perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	if (sad::db::TypeName<_Type>::isSadObject())
	{
		picojson::value v;
		reinterpret_cast<sad::Object *>(ptr)->save(v);
		return v;
	}
	throw sad::db::NotImplemented("sad::db::Save<_Type>::perform");
	//return picojson::value();
}

/*! Specification for saving bool values
 */
template<>
class Save<bool>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	return picojson::value(*reinterpret_cast<bool*>(ptr));
}

};

/*! Generates specification for saving to type via casting pointer to it
 */
#define SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE( TYPENAME ) \
template<>                                                     \
class Save<  TYPENAME >                                        \
{                                                              \
public:                                                        \
static inline picojson::value perform(void * ptr)              \
{                                                              \
	if (!ptr)                                                  \
		throw sad::db::InvalidPointer();                       \
	return picojson::value((double)(*( TYPENAME *)ptr));       \
}                                                              \
};

SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(signed char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned char)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(short)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned short)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(int)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned int)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(long long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(unsigned long long)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(float)
SPECIFY_SAVE_AS_CASTING_TO_DOUBLE_FOR_TYPE(double)

/*! Specification for saving string values
 */
template<>
class Save<std::string>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	return picojson::value(*reinterpret_cast<std::string*>(ptr));
}

};
/*! Specification for saving string values
 */
template<>
class Save<sad::String>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	return picojson::value(static_cast<sad::String>(*reinterpret_cast<sad::String*>(ptr)));
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Point2D & p = *(reinterpret_cast<sad::Point2D *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("x", picojson::value(static_cast<double>(p.x())));
	v.insert("y", picojson::value(static_cast<double>(p.y())));
	return v;
}

};

/*! Specification for saving point values
 */
template<>
class Save<sad::Point2I>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Point2I & p = *(reinterpret_cast<sad::Point2I *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("x", picojson::value(static_cast<double>(p.x())));
	v.insert("y", picojson::value(static_cast<double>(p.y())));
	return v;
}

};

/*! Specification for saving rectangle values
 */
template<>
class Save<sad::Rect2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Rect2D & p = *(reinterpret_cast<sad::Rect2D *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("p1", sad::db::Save<sad::Point2D>::perform(&(p[0])));
	v.insert("p2", sad::db::Save<sad::Point2D>::perform(&(p[1])));
	v.insert("p3", sad::db::Save<sad::Point2D>::perform(&(p[2])));
	v.insert("p4", sad::db::Save<sad::Point2D>::perform(&(p[3])));
	return v;
}

};

/*! Specification for saving color values
 */
template<>
class Save<sad::Color>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Color & p = *(reinterpret_cast<sad::Color *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("r", picojson::value(static_cast<double>(p.r())));
	v.insert("g", picojson::value(static_cast<double>(p.g())));
	v.insert("b", picojson::value(static_cast<double>(p.b())));
	return v;
}

};

/*! Specification for saving values of type color with alpha-channel
 */
template<>
class Save<sad::AColor>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::AColor & p = *(reinterpret_cast<sad::AColor *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("r", picojson::value(static_cast<double>(p.r())));
	v.insert("g", picojson::value(static_cast<double>(p.g())));
	v.insert("b", picojson::value(static_cast<double>(p.b())));
	v.insert("a", picojson::value(static_cast<double>(p.a())));
	return v;
}

};

/*! Specification for saving values of type vector of vectors of color with alpha-channel
 */
template<>
class Save<sad::Vector<sad::Vector<sad::AColor> > >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::Vector<sad::AColor> > & p = *(reinterpret_cast<sad::Vector<sad::Vector<sad::AColor> > *>(ptr));
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		picojson::value tmp(picojson::array_type, false);
		for(size_t j = 0; j < p[i].size(); j++)
		{
			tmp.push_back(sad::db::Save<sad::AColor>::perform(&(p[i][j])));
		}
		v.push_back(tmp);
	}
	return v;
}

};

/*! Specification for saving size values
 */
template<>
class Save<sad::Size2D>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Size2D & p = *(reinterpret_cast<sad::Size2D *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("width", picojson::value(static_cast<double>(p.Width)));
	v.insert("height", picojson::value(static_cast<double>(p.Height)));
	return v;
}

};

/*! Specification for saving size values
 */
template<>
class Save<sad::Size2I>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::Size2I & p = *(reinterpret_cast<sad::Size2I *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("width", picojson::value(static_cast<double>(p.Width)));
	v.insert("height", picojson::value(static_cast<double>(p.Height)));
	return v;
}

};

/*! Specification for saving values of type vector of 2-dimensional points
 */
template<>
class Save<sad::Vector<sad::Point2D> >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::Point2D> & p = *(reinterpret_cast<sad::Vector<sad::Point2D> *>(ptr));
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		v.push_back(sad::db::Save<sad::Point2D>::perform(&(p[i])));
	}
	return v;
}

};

/*! Specification for saving dialogue phrase values
 */
template<>
class Save<sad::dialogue::Phrase>
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	const sad::dialogue::Phrase & p = *(reinterpret_cast<sad::dialogue::Phrase *>(ptr));
	picojson::value v(picojson::object_type, false);
	v.insert("name", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.actorName()))));
	v.insert("portrait", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.actorPortrait()))));
	v.insert("phrase", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.phrase()))));
	double duration = p.duration();
	v.insert("duration", sad::db::Save<double>::perform(&duration));
	v.insert("viewhint", sad::db::Save<sad::String>::perform(const_cast<sad::String*>(&(p.viewHint()))));
	return v;
}

};

/*! Specification for saving values of type vector of phrases
 */
template<>
class Save<sad::Vector<sad::dialogue::Phrase*> >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::dialogue::Phrase*> & p = *(reinterpret_cast<sad::Vector<sad::dialogue::Phrase*> *>(ptr));
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		v.push_back(sad::db::Save<sad::dialogue::Phrase>::perform(p[i]));
	}
	return v;
}

};

/*! Specification for saving values of type vector of unsigned long long
 */
template<>
class Save<sad::Vector<unsigned long long> >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<unsigned long long> & p = *(reinterpret_cast<sad::Vector<unsigned long long> *>(ptr));
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		v.push_back(sad::db::Save<unsigned long long>::perform(&(p[i])));
	}
	return v;
}

};

/*! Specification for saving values of type vector of unsigned long long
 */
template<>
class Save<sad::Vector<sad::String> >
{
public:
/*! Saves a value of specified type
	\param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
	if (!ptr)
		throw sad::db::InvalidPointer();
	sad::Vector<sad::String> & p = *(reinterpret_cast<sad::Vector<sad::String> *>(ptr));
	picojson::value v(picojson::array_type, false);
	for(size_t i = 0; i < p.size(); i++)
	{
		v.push_back(sad::db::Save<sad::String>::perform(&(p[i])));
	}
	return v;
}

};

}

}
