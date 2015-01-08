#include "scenesclear.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

#include <renderer.h>

#include <algorithm>

Q_DECLARE_METATYPE(sad::db::Object*)

history::scenes::Clear::Clear(sad::Scene * s) : m_scene(s)
{
	m_scene->addRef();
	m_nodes = m_scene->objects();
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->active() == false)
		{
			m_nodes.removeAt(i);
			--i;
		}
		else
		{
			m_nodes[i]->addRef();
		}
	}
}

history::scenes::Clear::~Clear()
{
	m_scene->delRef();
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->delRef();
	}
}	

struct ___sortorder
{
	bool operator()(const sad::Pair<sad::SceneNode*, int>& p1, const sad::Pair<sad::SceneNode*, int>& p2)
	{
		return p1.p2() > p2.p2();
	}
};

void history::scenes::Clear::set(
	const sad::Vector< sad::Pair<sad::SceneNode*, int> >& positions,
	const sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> >& dependent
)
{
	m_positions = positions;
	___sortorder a;
	// Sort position descending
	std::sort(m_positions.begin(), m_positions.end(), a);
	m_dependent = dependent;
}


void history::scenes::Clear::commit(core::Editor * ob)
{
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->setActive(false);
	}
	if (ob)
	{
		if (ob->panel()->currentScene() == m_scene)
		{
			ob->emitClosure( bind(ob->panel()->UI()->lstSceneObjects, &QListWidget::clear) );
		}
		for(size_t i = 0; i < m_nodes.size(); i++)
		{
			if (ob->shared()->selectedObject() == m_nodes[i])
			{
				ob->shared()->setSelectedObject(NULL);
				if (ob->machine()->isInState("selected"))
				{
					ob->machine()->enterState("idle");
				}
			}
		}

		for(size_t i = 0; i < m_positions.size(); i++)
		{
			ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::removeItem, m_positions[i].p2()));
		}

		for(size_t i = 0; i < m_dependent.size(); i++)
		{
			m_dependent[i].p1()->setObjectId(0);
			if (m_dependent[i].p1() == ob->shared()->selectedInstance())
			{
				ob->emitClosure( blocked_bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, 0));
			}
		}
	}
}

void history::scenes::Clear::rollback(core::Editor * ob)
{
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->setActive(true);
	}
	if (ob)
	{
		if (ob->panel()->currentScene() == m_scene)
		{
			for(size_t i = 0; i < m_nodes.size(); i++)
			{
				ob->emitClosure( bind(ob->panel(), &MainPanel::addSceneNodeToSceneNodeList, m_nodes[i]) );
			}
		}
		for(int i = m_positions.size() - 1; i > -1; i--)
		{
			QVariant v;
			v.setValue(static_cast<sad::db::Object*>(m_positions[i].p1()));

			QString name = ob->panel()->fullNameForNode(m_positions[i].p1());
			int pos = m_positions[i].p2();

			void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;
			ob->emitClosure( bind(
				ob->panel()->UI()->cmbAnimationInstanceObject, 
				f, 
				pos,
				name,
				v
			));			
		}

		for(int i = 0; i < m_dependent.size(); i++)
		{
			m_dependent[i].p1()->setObjectId(m_dependent[i].p2());
			if (m_dependent[i].p1() == ob->shared()->selectedInstance())
			{
				int pos = -1;
				for(size_t j = 0; (j < m_positions.size()) && pos == -1; j++)
				{
					if (m_positions[j].p1()->MajorId == m_dependent[i].p2())
					{
						pos = m_positions[j].p2();
					}
				}
				if (pos != -1)
				{
					ob->emitClosure( blocked_bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, pos));
				}
			}
		}
	}
}
