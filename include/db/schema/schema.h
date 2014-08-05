/*! \file db/schema/schema.h
	\author HiddenSeeker
	
	Contains definition of class Schema, which defines a schema for all object.
 */
#pragma once
#include "../dbproperty.h"
#include "../dbobject.h"
#include "../../sadptrhash.h"
#include "../../sadvector.h"

namespace sad
{

namespace db
{

namespace schema
{

/*! \class Schema

	Defines a schema object for an object
 */
class Schema: public sad::db::Object  
{	
public:		
	/*! Creates new empty schema
		\param[in] parent a parent data for schema
	 */
	Schema(sad::db::schema::Schema* parent = NULL); 
	/*! This class can be inherited 
	 */
	virtual ~Schema();
	/*! Adds new property into schema
		\param[in] s a schema for a property
		\param[in] prop a property for a schema
		\return whether added successfully
	 */ 
	virtual bool add(const sad::String& s, sad::db::Property* prop);
	/*! Removes property from schema
		\param[in] s a schema
	 */
	virtual void remove(const sad::String & s);
	/*! Gets a property from schema
		\param[in] s a string
		\return  a property (NULL if not found)
	 */
	virtual sad::db::Property* getProperty(const sad::String& s) const;
	/*! Checks json value against schema
		\param[in] v a value for a schema
		\return a value for schema
	 */
	bool check(const picojson::value& v);
	/*! Loads an object for schema
		\param[in] o object, where data is stored
		\param[in] v a value for object
		\return whether it was successfull
	 */
	bool load(sad::db::Object * o, const picojson::value& v);
	/*! Saved linked object from a schema
		\param[in] linked a linked object
		\param[out] v a value, which will be filled with data from schema
	 */
	void save(sad::db::Object * linked, picojson::value & v);
	/*! Return parent schema
		\return parent schema
	 */
	const sad::Vector<sad::db::schema::Schema*>& parent() const;
	/*! Sets parent schema
		\param[in] parent parent schema
	 */
	void addParent(sad::db::schema::Schema* parent);
	/*! Returns own properties list
		\return own properties list
	 */
	const sad::Hash<sad::String, sad::db::Property*>& ownProperties() const;
protected: 
	/*! A parent schema for an object
	 */
	sad::Vector<sad::db::schema::Schema*> m_parent;
	/*! A properties, stored inside of schema
	 */
	sad::PtrHash<sad::String, sad::db::Property> m_properties;
};

}

}

}

#define SAD_DB_SCHEMA_START    struct  _____schema_initter { _____schema_initter() {
#define SAD_DB_SCHEMA_END      } }  _______schema_default_value;
