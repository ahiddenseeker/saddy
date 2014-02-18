#include "resource/tree.h"
#include "resource/physicalfile.h"

#include "renderer.h"

#include "3rdparty/picojson/picojson.h"
#include "3rdparty/picojson/valuetotype.h"

#include "util/free.h"
#include "util/fs.h"

#include <fstream>

sad::resource::Tree::Tree(sad::Renderer * r)
: 
m_renderer(r), 
m_root(new sad::resource::Folder()), 
m_factory(new sad::resource::Factory()),
m_storelinks(false)
{
	if (r == NULL)
	{
		m_renderer = sad::Renderer::ref();
	}
}

sad::resource::Tree::~Tree()
{
	delete m_root;
	delete m_factory;
}


sad::Vector<sad::resource::Error*> sad::resource::Tree::loadFromString(const sad::String & string)
{
	sad::Vector<sad::resource::Error*> errors;
	picojson::value v = picojson::parse_string(string);
	if (picojson::get_last_error().size() == 0 && v.is<picojson::array>())
	{
		// Check new root errors
		sad::resource::Folder * newroot = new sad::resource::Folder();
		sad::Vector<sad::resource::PhysicalFile *> newfiles;

		// Try load data to temporary containers
		picojson::array & resourcelist = v.get<picojson::array>();		
		for(int i = 0 ; i < resourcelist.size() && errors.size() != 0; i++)
		{
			sad::Maybe<sad::String>  maybetype = picojson::to_type<sad::String>(
				picojson::get_property(resourcelist[i], "type")
			);
			sad::Maybe<sad::String>  maybename = picojson::to_type<sad::String>(
				picojson::get_property(resourcelist[i], "filename")
			);
			if (maybetype.exists() && maybename.exists())
			{
				sad::Maybe<sad::String>  mayberesourcename = picojson::to_type<sad::String>(
							picojson::get_property(resourcelist[i], "name")
						);
				errors << load(
					maybetype.value(), 
					maybename.value(), 
					mayberesourcename, 
					newroot, 
					resourcelist[i], 
					newfiles
				);
			} 
			else
			{
				errors << new sad::resource::MalformedResourceEntry(resourcelist[i]);
			}
		}

		// Try to copy resources to a core
		if (errors.size() == 0)
		{
			sad::resource::ResourceEntryList list = newroot->copyAndClear();
			errors << this->duplicatesToErrors(m_root->duplicatesBetween(list));
			if (errors.size() == 0)
			{
				m_root->addResources(list);
				m_files << newfiles;
				delete newroot;
			}
			else
			{
				sad::resource::free(list);
			}
		}
		
		if (errors.size() != 0)
		{
			delete newroot;
			util::free(newfiles);
		}
	}
	else
	{
		if (picojson::get_last_error().size() == 0)
		{
			errors << new  sad::resource::MalformedResourceEntry(v);
		}
		else
		{
			errors << new sad::resource::JSONParseError();
		}
	}
	return errors;
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::loadFromFile(const sad::String& string)
{
	std::ifstream stream(string.c_str());
	if (stream.good())
	{
		std::string alldata(
			(std::istreambuf_iterator<char>(stream)), 
			std::istreambuf_iterator<char>()
		);
		return loadFromString(string);
	}
	else
	{
		if (util::isAbsolutePath(string) == false)
		{
			sad::String path = util::concatPaths(m_renderer->executablePath(), string);
			stream.open(path.c_str());
			if (stream.good())
			{
				std::string alldata(
					(std::istreambuf_iterator<char>(stream)), 
					 std::istreambuf_iterator<char>()
					);
				return loadFromString(string);
			}
		}
	}
	sad::Vector<sad::resource::Error*> result;
	result << new sad::resource::FileLoadError(string);
	return result;
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::load(
		const sad::String& typehint, 
		const sad::String& filename, 
		const sad::Maybe<sad::String>& resourcename
)
{
	return load(
		typehint, 
		filename, 
		resourcename, 
		m_root, 
		picojson::value(picojson::null_type, false), 
		m_files
	);
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::load(
	const sad::String& typehint, 
	const sad::String& filename, 
	const sad::Maybe<sad::String>& resourcename,
	sad::resource::Folder * store,
	const picojson::value & v,
	sad::Vector<sad::resource::PhysicalFile *> & files
)
{
	sad::resource::Folder * temporary = new sad::resource::Folder();
	sad::Vector<sad::resource::Error*> errors;
	
	// Fill picojson value if needed
 	picojson::value resourcedescription = v;
	if (resourcedescription.is<picojson::object>() == false)
	{
		resourcedescription = picojson::object();
		resourcedescription.insert("type", picojson::value(typehint));
		resourcedescription.insert("filename", picojson::value(filename));
		if (resourcename.exists())
		{
			resourcedescription.insert("name", picojson::value(resourcename.value()));
		}
	}

	// Try create physical file
	sad::resource::PhysicalFile * file = m_factory->fileByType(typehint);
	// First of all file could not be created sometimes
	if (file)
	{
		file->setTree(this);
		file->setName(filename);
		sad::resource::Resource  * resource = m_factory->create(typehint);
		// Sometimes a resource takes care of loading itself, otherwise it could be done
		// via file
		if (resource)
		{
			resource->setFactoryName(typehint);
			bool ok = resource->tryLoad(*file, m_renderer, resourcedescription , m_storelinks);
			// Sometimes loading of resource could fail
			if (ok)
			{
				file->add(resource);
				resource->setPhysicalFile(file);
				// Whend appending a resource, resource could be anonymous
				if (resourcename.exists())
				{
					// Sometimes resource could be duplicated
					if (temporary->resource(resourcename.value()) != NULL)
					{
						errors << new sad::resource::ResourceAlreadyExists(resourcename.value());
						delete resource;
					}
					else
					{
						resource->setName(resourcename.value());
						temporary->addResource(resourcename.value(), resource);
					}
				}
				else
				{
					errors << new sad::resource::AnonymousResource(filename);
					delete resource;
				}
			}
			else
			{
				sad::String fileerrorname = filename;
				if (resourcename.exists())
				{
					fileerrorname = resourcename.value();
				}
				errors << new sad::resource::ResourceLoadError(fileerrorname);
			}

		}
		else
		{
			 errors << file->load(temporary);
		}
	}
	else
	{
		errors << new sad::resource::UnregisteredFileType(typehint);
	}

	// Cleanup all state or try to copy all data, if errors not found 
	// Errors still can persist due to duplication of resources in store and temporary
	if (errors.size() == 0)
	{
		// Checks whether resources already exists in stored place
		sad::resource::ResourceEntryList list = temporary->copyAndClear();
		errors << this->duplicatesToErrors( store->duplicatesBetween(list) );

		// If everything is ok, append all elements, otherwise free lists
		if (errors.size() == 0)
		{
			files << file;
			store->addResources(list);
			delete temporary;
		}
		else
		{
			sad::resource::free(list);
		}
	}

	// If some errors exists, destroy temporary
	if (errors.size() != 0)
	{
		if (file)
			delete file;
		delete temporary;
	}
	return errors;
}

bool sad::resource::Tree::unload(const sad::String& file)
{
	sad::resource::PhysicalFile * f = this->file(file);
	if (f)
	{
		sad::Vector<sad::resource::Resource*>  list = f->resources();
		bool referenced = false;
		for(int i = 0; i < list.size(); i++)
		{
			referenced = referenced || list[i]->referenced();
		}
		if (referenced)
			return false;
		for(int i = 0; i < list.size(); i++)
		{
			sad::Maybe<sad::String> path = this->root()->find(list[i]);
			if (path.exists())
			{
				this->root()->removeResource(path.value(), true);
			} 
			else
			{
				delete list[i];
			}
		}
		m_files.removeAll(f);
		delete f;
		return true;
	}
	return false;
}

sad::resource::Folder* sad::resource::Tree::root() const
{
	return m_root;
}

sad::resource::PhysicalFile* sad::resource::Tree::file(const sad::String& name)
{
	for(int i = 0; i < m_files.size(); i++)
	{
		if (m_files[i]->name() == name)
			return m_files[i];
	}
	return NULL;
}

const sad::Vector<sad::resource::PhysicalFile*>& sad::resource::Tree::files() const
{
	return m_files;
}

sad::resource::Factory* sad::resource::Tree::factory()
{
	return m_factory;
}

void sad::resource::Tree::setFactory(sad::resource::Factory* factory)
{
	delete m_factory;
	m_factory = factory;
}

void sad::resource::Tree::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::resource::Tree::renderer() const
{
	return m_renderer;
}

bool sad::resource::Tree::shouldStoreLinks() const
{
	return m_storelinks;
}

void sad::resource::Tree::setStoreLinks(bool store)
{
	m_storelinks = store;
}

sad::Vector<sad::resource::Error *> sad::resource::Tree::duplicatesToErrors(
		const sad::Vector<sad::String> & l
)
{
	sad::Vector<sad::resource::Error *> result;
	for(size_t i = 0; i < l.size(); i++)
	{
		result << new sad::resource::ResourceAlreadyExists(l[i]);
	}
	return result;
}
