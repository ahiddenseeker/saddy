#include "db/dbobject.h"


sad::db::Object::Object() : m_table(NULL), MajorId(0), MinorId(0)
{

}

sad::db::Object::~Object()
{

}

picojson::value sad::db::Object::save()
{
	return picojson::value();	
}

bool sad::db::Object::load(const picojson::value& v)
{
	return false;
}

sad::db::Table* sad::db::Object::table() const
{
	return m_table;
}

void sad::db::Object::setTable(sad::db::Table* t)
{
	m_table = t;
}

sad::db::schema::Schema * sad::db::Object::schema() const
{
	return NULL;
}

