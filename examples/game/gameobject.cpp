#include "gameobject.h"
#include "game.h"

DECLARE_SOBJ_INHERITANCE(GameObject, sad::p2d::app::Object);

GameObject::GameObject(): sad::p2d::app::Object()
{
	m_hp = 1;
}

GameObject::~GameObject()
{
	for(size_t i = 0; i < m_guns.count(); i++)
	{
		delete m_guns[i];
	}
}

void GameObject::render()
{
	this->sad::p2d::app::Object::render();
	for(size_t i = 0; i < m_guns.count(); i++)
	{
		m_guns[i]->tryShoot();;
	}
}

int GameObject::hitPoints() const
{
    return m_hp;
}

void GameObject::incrementHP(int count)
{
    m_hp += count;
}

void GameObject::decrementHP(int count)
{
	m_hp -= count;

	// If hp less than zero - remove object
	if (m_hp <= 0 && this->game() != NULL)
	{
		this->game()->removeObject(this);
	}
}

Game * GameObject::game()
{
	if (m_app == NULL)
	{
		return NULL;
	}
	return static_cast<Game *>(m_app);
}

void GameObject::addGun(AbstractAutomaticGun * gun)
{
	m_guns << gun;
	gun->setObject(this);
}

