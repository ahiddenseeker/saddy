#include "gui/resourcetreewidget/resourcetreewidget.h"
#include "gui/resourcetreewidget/resourcecache.h"
#include "gui/resourcetreewidget/celldelegate.h"
#include "gui/resourcetreewidget/cell.h"
#include "gui/resourcetreewidget/defaultimage.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QMessageBox>

#include <renderer.h>
#include <maybe.h>
#include <sadstring.h>

#include <texturemappedfont.h>

#include <QImage>

Q_DECLARE_METATYPE(sad::String);

gui::resourcetreewidget::ResourceTreeWidget::ResourceTreeWidget(QWidget * parent) 
: QWidget(parent), m_padding(6), m_tree_name("")
{
	m_tree_view = new QTreeWidget(parent);

	// Cleanup header of tree view
	QStringList headerLabels;
	headerLabels << "";
	m_tree_view->setHeaderLabels(headerLabels);
	m_tree_view->header()->close();

	m_element_view = new QTableWidget(parent);
	m_element_view->horizontalHeader()->hide();
	m_element_view->verticalHeader()->hide();
	m_element_view->setSelectionMode(QAbstractItemView::SingleSelection);
	m_element_view->setSelectionBehavior(QAbstractItemView::SelectItems);

	gui::resourcetreewidget::CellDelegate* mydelegate = new gui::resourcetreewidget::CellDelegate();
	mydelegate->setParent(m_element_view);
	mydelegate->setResourceTreeWidget(this);
	m_element_view->setItemDelegate(mydelegate);

	m_cache = new gui::resourcetreewidget::ResourceCache();
	m_cache->setParent(this);

	resizeWidgets(this->geometry());

	connect(m_tree_view, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(m_element_view, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(elementItemChanged(QTableWidgetItem*, QTableWidgetItem*)));
}

gui::resourcetreewidget::ResourceCache * gui::resourcetreewidget::ResourceTreeWidget::cache()
{
	return m_cache;
}

gui::resourcetreewidget::ResourceTreeWidget::~ResourceTreeWidget()
{
	delete m_tree_view;
	delete m_element_view;
}

double gui::resourcetreewidget::ResourceTreeWidget::padding() const
{
	return m_padding;
}

void gui::resourcetreewidget::ResourceTreeWidget::setPadding(double padding)
{
	m_padding = padding;
	resizeWidgets(this->geometry());
}

void gui::resourcetreewidget::ResourceTreeWidget::setTree(const QString & name)
{
	m_tree_name = name;
	updateTree();
}

const QString & gui::resourcetreewidget::ResourceTreeWidget::tree() const
{
	return m_tree_name;
}

void gui::resourcetreewidget::ResourceTreeWidget::setFilter(const QString & filter)
{
	m_filter = filter;
	updateTree();
}

const QString & gui::resourcetreewidget::ResourceTreeWidget::filter() const
{
	return m_filter;
}

void gui::resourcetreewidget::ResourceTreeWidget::updateTree()
{
	sad::Maybe<sad::String> path;
	QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
	if (items.size())
	{
		path =	this->selectedFolder(items[0]);
	}
	sad::Maybe<sad::String> resourcename;
	if (path.exists())
	{
		resourcename = this->selectedLocalPathToResource();
	}

	m_tree_view->clear();
	m_element_view->clear();

	QTreeWidgetItem * root = new QTreeWidgetItem(QStringList("/"));
	m_tree_view->addTopLevelItem(root);

	sad::resource::Tree * tree = sad::Renderer::ref()->tree(m_tree_name.toStdString());
	if (tree)
	{
		sad::resource::Folder * folderroot = tree->root();
		populateTree(root, folderroot);
	}
	tryRestoreSelection(path, resourcename);
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::pathToItemBySelection(
	const QString & name
)
{
	sad::Maybe<sad::String> result;
	QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
	if (items.count())
	{
		sad::String path = this->selectedFolder(items[0]).value();
		if (path.length() != 0)
		{
			path += "/";
			path += name.toStdString();
			result.setValue(path);
		} 
		else
		{
			result.setValue(name.toStdString());
		}
	}
	return result;
}

void gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName(
	const sad::String & name
)
{
	sad::StringList list = name.split("/");
	if (list.size() == 1)
	{
		this->tryRestoreSelection(sad::Maybe<sad::String>(""), sad::Maybe<sad::String>(name));
	}
	else
	{
		sad::String resourcename = list[list.size() - 1];
		list.removeAt(list.size() - 1);
		sad::String path = sad::join(list, "/");
		this->tryRestoreSelection(
			sad::Maybe<sad::String>(path), 
			sad::Maybe<sad::String>(resourcename)
		);
	}
}


sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedResourceName() const
{
	QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
	sad::Maybe<sad::String> result;
	if (items.size())
	{
		sad::Maybe<sad::String> path = this->selectedFolder(items[0]);
		if (path.exists())
		{
			sad::Maybe<sad::String> localresource = this->selectedLocalPathToResource();
			if (localresource.exists())
			{
				if (path.value().length() == 0)
				{
					result = localresource;
				}
				else
				{
					result.setValue(path.value() + "/" + localresource.value());
				}
			}
		}
	}
	return result;
}

sad::resource::Resource* gui::resourcetreewidget::ResourceTreeWidget::selectedResource() const
{
	sad::Maybe<sad::String> path = this->selectedResourceName();
	sad::resource::Resource * result = NULL;

	sad::resource::Tree * tree = sad::Renderer::ref()->tree(m_tree_name.toStdString());
	sad::resource::Folder * folder = tree->root();
	if (path.exists())
	{
		result = folder->resource(path.value());
	}
	return result;
}

void	gui::resourcetreewidget::ResourceTreeWidget::treeItemChanged(
	QTreeWidgetItem * current, 
	QTreeWidgetItem * previous
)
{
	if (current)
	{
		sad::Maybe<sad::String> v = selectedFolder(current);
		if (v.exists())
		{
			m_element_view->clear();
			sad::resource::Tree * tree = sad::Renderer::ref()->tree(m_tree_name.toStdString());
			sad::resource::Folder * folder = tree->root();
			if (v.value().length() != 0)
			{
				folder = tree->root()->folder(v.value());
			}
			sad::resource::ResourceIterator cur = folder->resourceListBegin();
			
			// Count rows in item
			unsigned int rows = 0;
			bool odd = false;
			QStringList list = m_filter.split("|");
			for(; cur != folder->resourceListEnd(); cur++)
			{
				bool shouldshowresource = true;
				if (list.count())
				{
					const sad::String & name = cur.value()->metaData()->name();
					shouldshowresource = list.indexOf(name.c_str()) != -1;
				}
				if (shouldshowresource)
				{
					if (odd)
					{
						rows += 1;
						odd = false;
					}
					else
					{
						odd = true;
					}
				}
			}
			if (odd)
			{
				rows += 1;
			}
			m_element_view->setRowCount(rows);
			m_element_view->setColumnCount(2);

			// Fill table
			int row = 0;
			int column = 0;
			cur = folder->resourceListBegin();
			for(; cur != folder->resourceListEnd(); cur++)
			{
				bool shouldshowitem = true;
				if (list.count())
				{
					shouldshowitem = list.indexOf(cur.value()->metaData()->name().c_str()) != -1;
				}
				if (shouldshowitem)
				{
					QTableWidgetItem * item = new QTableWidgetItem(cur.key().c_str());
					item->setFlags(item->flags() ^ Qt::ItemIsEditable);
					item->setSizeHint(QSize(
						gui::resourcetreewidget::Cell::Width, 
						gui::resourcetreewidget::Cell::Height
					));
					m_element_view->setItem(row, column, item);
					m_element_view->resizeColumnsToContents();
					m_element_view->resizeRowsToContents();
					if (column == 0)
					{
						column = 1;
					}
					else
					{
						column = 0;
						row += 1;
					}
				}
			}			
		}
	}
	else
	{
		m_element_view->clear();
	}
}

void gui::resourcetreewidget::ResourceTreeWidget::elementItemChanged(
	QTableWidgetItem * current, 
	QTableWidgetItem * previous
)
{
	if (current)
	{
		sad::String name = current->text().toStdString();
		QList<QTreeWidgetItem *> list = m_tree_view->selectedItems();
		if (list.count())
		{
			sad::Maybe<sad::String> path = this->selectedFolder(list[0]);
			if (path.exists())
			{
				sad::String result = path.value();
				if (result.length())
				{
					result += "/";
					result += name;
				}
				else
				{
					result = name;
				}
				emit selectionChanged(result);
			}
		}
	}
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedFolder(
	QTreeWidgetItem * item
) const
{
	sad::Maybe<sad::String> value;
	if (item)
	{
		QStringList treepath;
		while(item)
		{
			treepath << item->text(0);
			item = item->parent();
		}
		treepath.removeAt(treepath.size() - 1);
		std::reverse(treepath.begin(), treepath.end());
		sad::String path = treepath.join("/").toStdString();
		value.setValue(path);
	}
	return value;
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedLocalPathToResource() const
{
	QList<QTableWidgetItem *> elements = m_element_view->selectedItems();
	sad::Maybe<sad::String> result;
	if (elements.count())
	{
		result.setValue(elements[0]->text().toStdString());
	}
	return result;
}

void gui::resourcetreewidget::ResourceTreeWidget::tryRestoreSelection(
	const sad::Maybe<sad::String> & folder,
	const sad::Maybe<sad::String> & resourcelocal
)
{
	if (folder.exists())
	{
		sad::String pathvalue = folder.value();
		bool found = false;
		QTreeWidgetItem * current = NULL;
		if (pathvalue.length() == 0)
		{
			current = m_tree_view->topLevelItem(0);
			m_tree_view->setCurrentItem(current, 0);
			found = true;
		}
		else
		{
			sad::StringList list = pathvalue.split("/");
			current = m_tree_view->topLevelItem(0);
			for(int i = 0; i < list.count() && current != NULL; i++)
			{
				bool hasspecifieditem = false;
				for(int j = 0; j < current->childCount() && !hasspecifieditem; j++)
				{
				   if(current->child(j)->text(0) == list[i].c_str())
				   {
						current = current->child(j);
						hasspecifieditem = true;
				   }
				}
				if (!hasspecifieditem)
				{
					current = NULL;
				}
			}
			if (current)
			{
				m_tree_view->setCurrentItem(current, 0);
				found = true;
			}
		}

		if (found)
		{
			this->treeItemChanged(current, NULL);
			if (resourcelocal.exists())
			{
				// Try set selection for local
				QList<QTableWidgetItem *> items = m_element_view->findItems(resourcelocal.value().c_str(), Qt::MatchFixedString | Qt::MatchCaseSensitive);
				if (items.count())
				{
					QTableWidgetItem* item = items[0];
					bool b = m_element_view->blockSignals(true);
					m_element_view->setCurrentItem(item);
					m_element_view->blockSignals(b);
					m_element_view->update();
				}
			}
		}
	}
}

void gui::resourcetreewidget::ResourceTreeWidget::populateTree(
		QTreeWidgetItem * parentitem, 
		sad::resource::Folder * parentfolder
)
{
	sad::resource::FolderIterator it = parentfolder->folderListBegin();
	while(it != parentfolder->folderListEnd())
	{
		QTreeWidgetItem * item = new QTreeWidgetItem(QStringList(it.key().c_str()));
		parentitem->addChild(item);
		populateTree(item, it.value());
		++it;
	}
}

void gui::resourcetreewidget::ResourceTreeWidget::resizeEvent( QResizeEvent * e )
{
	QRect oldrect = this->geometry();
	QRect r(oldrect.x(), oldrect.y(), e->size().width(), e->size().height());
	resizeWidgets(r);
}

void gui::resourcetreewidget::ResourceTreeWidget::moveEvent( QMoveEvent * e )
{
	QRect oldrect = this->geometry();	
	QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
	resizeWidgets(r);	
}

void gui::resourcetreewidget::ResourceTreeWidget::resizeWidgets(const QRect & r)
{
	double halfsizenopad = r.width() / 2.0 - this->padding() / 2.0;
	m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, r.height());
	double element_view_x = r.x() + r.width() / 2.0 + this->padding() / 2.0;
	m_element_view->setGeometry(element_view_x, r.y(), halfsizenopad , r.height());
}
