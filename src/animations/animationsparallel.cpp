#include "animations/animationsparallel.h"
#include "animations/animationsinstance.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::Parallel, sad::animations::Composite);

// ====================================== PUBLIC METHODS ======================================


sad::animations::Parallel::Parallel()
{

}

sad::animations::Parallel::~Parallel()
{

}

void sad::animations::Parallel::setState(sad::animations::Instance* i, double time)
{
    if (m_commands.contains(i->object()))
    {
        Commands& c  = m_commands[i->object()];
        for(size_t ii = 0; ii < m_links.size(); ii++)
        {
            sad::animations::Animation* animation = m_links[ii]->object(false);
            if (animation)
            {
                animation->setTime(this->time());
                i->setStateCommand(c[ii], false);
                animation->setState(i, time);
            }
        }
    }
}

void sad::animations::Parallel::setTime(double time)
{
    this->sad::animations::Animation::setTime(time);
    for(size_t ii = 0; ii < m_links.size(); ii++)
    {
        sad::animations::Animation* animation = m_links[ii]->object(false);
        if (animation)
        {
            animation->setTime(time);
        }
    }
}
