#include "db/database.h"

sad::db::Database::~Database()
{

}

sad::String sad::db::Database::save()
{
		return sad::String();	
}

void sad::db::Database::saveToFile(const sad::String& s)
{
			
}

void sad::db::Database::loadFromFile(const sad::String& name)
{
			
}

void sad::db::Database::load(const sad::String& text)
{
			
}

void sad::db::Database::addTable(const sad::String& name, db::Table* table)
{
			
}

void sad::db::Database::removeTable(const sad::String& name)
{
			
}

unsigned long long sad::db::Database::addMajor()
{
	return 0;	
}

bool sad::db::Database::save(unsigned long long id)
{
	return bool();	
}

bool sad::db::Database::save(unsigned long long id, const sad::String& name)
{
	return bool();	
}

sad::Vector<sad::db::Error*> sad::db::Database::load(const sad::String& name, unsigned long long& id)
{
	return sad::Vector<sad::db::Error*>();	
}

sad::db::Table* sad::db::Database::table(const sad::String& name)
{
	return NULL;	
}

sad::Hash<sad::String, sad::db::Table*>::iterator sad::db::Database::begin()
{
	return sad::Hash<sad::String, sad::db::Table*>::iterator();	
}

sad::Hash<sad::String, sad::db::Table*>::iterator sad::db::Database::end()
{
	return sad::Hash<sad::String, sad::db::Table*>::iterator();	
}

sad::db::ObjectFactory* sad::db::Database::factory()
{
	return m_factory;	
}

void sad::db::Database::setFactory(sad::db::ObjectFactory* f)
{
			
}

