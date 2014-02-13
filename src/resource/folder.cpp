#include "resource/folder.h"
#include "resource/resource.h"

void sad::resource::free(const sad::resource::ResourceEntryList & list)
{
	for(size_t i = 0; i < list.size(); i++)
	{
		delete list[i].p2();
	}
}

sad::resource::Folder::Folder() : m_parent(NULL)
{

}

sad::resource::Folder::~Folder()
{

}

bool sad::resource::Folder::hasFolders() const
{
	return m_subfolders.count() != 0;
}

bool sad::resource::Folder::hasResources() const
{
	return m_resources.count() != 0;
}

bool sad::resource::Folder::addFolder(const sad::String& path, sad::resource::Folder* folder)
{
	sad::String foldername;
	sad::resource::Folder * parent = navigateParentFolder(path, true, foldername);
	if (parent == NULL)
	{
		return false;
	}
	if (parent->m_subfolders.contains(foldername) != NULL)
	{
		delete parent->m_subfolders[foldername];
	}
	parent->m_subfolders.insert(foldername, folder);
	folder->setParent(parent);
	return true;
}


bool sad::resource::Folder::addResources(const sad::resource::ResourceEntryList & list)
{
	bool result = true;
	for(int i = 0; i < list.size(); i++)
	{
		result = result &&	this->addResource(list[i].p1(), list[i].p2());
	}
	return result;
}

void sad::resource::Folder::replaceResources(const sad::resource::ResourceEntryList & list)
{
	for(int i = 0; i < list.size(); i++)
	{
		this->replaceResource(list[i].p1(), list[i].p2());
	}
}

void sad::resource::Folder::removeResources(
	const sad::resource::ResourceEntryList & list, 
	bool free
)
{
	for(int i = 0; i < list.size(); i++)
	{
		this->removeResource(list[i].p1(), free);
	}	
}

sad::Vector<sad::String> sad::resource::Folder::duplicatesBetween(
	const sad::resource::ResourceEntryList & list
)
{
	sad::Vector<sad::String> result;
	for(int i = 0; i < list.size(); i++)
	{
		if (this->resource(list[i].p1()) != NULL)
		{
			result << list[i].p1();
		}
	}
	return result;
}

bool sad::resource::Folder::addResource(const sad::String & path, sad::resource::Resource* r)
{
	sad::String resourcename;
	sad::resource::Folder * parent = navigateParentFolder(path, true, resourcename);
	if (parent == NULL)
	{
		return false;
	}
	if (parent->m_resources.contains(resourcename) != NULL)
	{
		delete parent->m_resources[resourcename];
	}
	parent->m_resources.insert(resourcename, r);
	r->setParentFolder(parent);
	return true;	
}

void sad::resource::Folder::removeFolder(const sad::String& path, bool free)
{
	sad::String foldername;
	sad::resource::Folder * parent = navigateParentFolder(path, false, foldername);
	if (parent == NULL)
	{
		return;
	}
	if (parent->m_subfolders.contains(foldername))
	{
		parent->setParent(NULL);
		if (free) 
		{
			delete parent->m_subfolders[foldername];
		}
		parent->m_subfolders.remove(foldername);
	}
}

void sad::resource::Folder::removeResource(const sad::String& path, bool free)
{
	sad::String resourcename;
	sad::resource::Folder * parent = navigateParentFolder(path, false, resourcename);
	if (parent == NULL)
	{
		return;
	}
	if (parent->m_resources.contains(resourcename))
	{
		parent->setParent(NULL);
		if (free) 
		{
			delete parent->m_resources[resourcename];
		}
		parent->m_resources.remove(resourcename);
	}
			
}

sad::resource::Folder* sad::resource::Folder::folder(const sad::String& path)
{
	sad::String foldername;
	resource::Folder * parent = this->navigateParentFolder(path, false, foldername);
	resource::Folder * result = NULL;
	if (parent)
	{
		if (parent->m_subfolders.contains(foldername))
		{
			result = parent->m_subfolders[foldername];
		}
	}
	return result;
}

sad::resource::Resource* sad::resource::Folder::resource(const sad::String& path)
{
	sad::String foldername;
	resource::Folder * parent = this->navigateParentFolder(path, false, foldername);
	resource::Resource * result = NULL;
	if (parent)
	{
		if (parent->m_resources.contains(foldername))
		{
			result = parent->m_resources[foldername];
		}
	}
	return result;	
}

void sad::resource::Folder::replaceResource(const sad::String& name, resource::Resource* r)
{
	sad::resource::Resource * old = resource(name);
	if (old)
	{
		old->replaceWith(r);
		this->removeResource(name, true);
	}
	this->addResource(name, r);
}

sad::resource::FolderIterator sad::resource::Folder::folderListBegin()
{
	return m_subfolders.begin();
}

sad::resource::FolderIterator sad::resource::Folder::folderListEnd()
{
	return m_subfolders.end();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceListBegin()
{
	return m_resources.begin();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceListEnd()
{
	return m_resources.end();
}

void sad::resource::Folder::setParent(sad::resource::Folder * folder)
{
	m_parent = folder;
}

sad::resource::Folder * sad::resource::Folder::parent() const
{
	return m_parent;
}

sad::Maybe<sad::String> sad::resource::Folder::find(sad::resource::Resource * r)
{
	sad::Maybe<sad::String> result;
	for(sad::resource::ResourceIterator it = this->resourceListBegin(); 
		it != this->resourceListEnd(); 
		it++)
	{
		if (it.value() == r)
		{
			result.setValue(it.key());
			return result;
		}
	}
	for(sad::resource::FolderIterator it = this->folderListBegin(); 
		it != this->folderListEnd(); 
		it++)
	{
		result = it.value()->find(r);
		if (result.exists())
		{
			result.setValue(it.key() + "/" + result.value());
			return result;
		}
	}
	return result;
}

sad::resource::ResourceEntryList sad::resource::Folder::copyAndClear()
{
	sad::resource::ResourceEntryList result;
	for(sad::resource::ResourceIterator it = this->resourceListBegin(); 
		it != this->resourceListEnd(); 
		it++)
	{
		result << sad::resource::ResourceEntry(it.key(), it.value());
	}
	m_resources.clear();
	for(sad::resource::FolderIterator it = this->folderListBegin(); 
		it != this->folderListEnd(); 
		it++)
	{
		sad::resource::ResourceEntryList tmp = it.value()->copyAndClear();
		sad::String prefix = it.key() + "/"; 
		for(size_t i = 0; i < tmp.size(); i++)
		{
			result <<  sad::resource::ResourceEntry(prefix + tmp[i].p1(), tmp[i].p2());
		}
	}
	return result;
}
sad::resource::Folder * sad::resource::Folder::navigateParentFolder(
		const sad::String & path, 
		bool create,
		sad::String & name
)
{
	if (path.size() == 0 )
		return NULL;
	if (path.getOccurences("/") > 1024)
		return NULL;
	sad::Vector<sad::String> splitpath = path.split("/");
	if (splitpath.size() == 0)
		return NULL;
	sad::resource::Folder * parent = this;
	for(int i = 0; i < (int)(splitpath.size()) - 1; i++)
	{
		if (parent->m_subfolders.contains(splitpath[i]) == NULL)
		{
			if (create)
			{
				parent->m_subfolders.insert(splitpath[i], new sad::resource::Folder());
			}
			else
			{
				return NULL;
			}
		}
		parent = parent->m_subfolders[splitpath[i]];
	}
	name = splitpath[splitpath.count() - 1];
	return parent;
}
