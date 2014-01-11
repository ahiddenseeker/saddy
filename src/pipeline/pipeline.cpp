#include "pipeline/pipeline.h"
#include <cassert>

sad::pipeline::Pipeline::Pipeline()
{

}

bool sad::pipeline::Pipeline::contains(const sad::String & mark)
{
	return findByMark(mark).p1() != NULL;
}

sad::pipeline::Step * sad::pipeline::Pipeline::insertStep(
		sad::pipeline::PipelineInsertionType type, 
		sad::pipeline::Step * step
	)
{
	assert(
		type == sad::pipeline::PIT_BEGIN 
	 || type == sad::pipeline::PIT_END
	 || type == sad::pipeline::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION
	 || type == sad::pipeline::PIT_SYSTEM_AFTER_LAST_USER_ACTION 
	);
	sad::pipeline::PipelineInsertionData data;
	data.set1(type);
	data.set3(step);
	add(data);
	return step;
}

sad::pipeline::Step *  sad::pipeline::Pipeline::insertStep(
		sad::pipeline::PipelineInsertionType type, 
		sad::String mark,
		sad::pipeline::Step * step
	)
{
	sad::pipeline::PipelineInsertionData data;
	data.set1(type);
	data._2().setValue(mark);
	data.set3(step);
	add(data);
	return step;
}


void sad::pipeline::Pipeline::run()
{
	this->performQueuedActions();
	this->lockChanges();

	invokeSteps(m_system_steps_before_user);
	invokeSteps(m_user_steps);
	invokeSteps(m_system_steps_after_user);

	this->unlockChanges();
	this->performQueuedActions();
}

sad::pipeline::Step * sad::pipeline::Pipeline::append(sad::pipeline::Step * step)
{
	step->setSource(sad::pipeline::ST_USER);
	this->insertStep(sad::pipeline::PIT_END, step);
	return step;
}

sad::pipeline::Step * sad::pipeline::Pipeline::prepend(sad::pipeline::Step * step)
{
	step->setSource(sad::pipeline::ST_USER);
	this->insertStep(sad::pipeline::PIT_BEGIN, step);
	return step;	
}

sad::pipeline::Pipeline::~Pipeline()
{
	sad::pipeline::Pipeline::StepsList * lists[3] = {
		&m_system_steps_before_user,
		&m_user_steps,
		&m_system_steps_after_user
	};
	for(int i = 0; i < 3; i++)
	{
		sad::pipeline::Pipeline::clearSteps(lists[i]);
	}
}

void sad::pipeline::Pipeline::add(const sad::pipeline::PipelineInsertionData & o)
{
	if (o.p1() == sad::pipeline::PIT_END && o.p3()->source() == sad::pipeline::ST_USER)
	{
		addNow(o);
	}
	else
	{
		this->sad::TemporarilyImmutableContainerWithHeterogeneousCommands<
			sad::pipeline::PipelineInsertionData, 
			sad::pipeline::Step *
		>::add(o);
	}
}

void sad::pipeline::Pipeline::invokeSteps(sad::pipeline::Pipeline::StepsList & steps)
{
	for(unsigned int i = 0; i < steps.size(); i++)
	{
		steps[i]->process();
		if (steps[i]->shouldBeDestroyedAfterProcessing()) 
		{
			delete steps[i];
			steps.removeAt(i);
			--i;
		}
	}
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByMark(StepsList* steps, const sad::String & mark)
{
	for(unsigned int i = 0; i < steps->size(); i++) 
	{
		sad::pipeline::Step* step= (*steps)[i];
		if (step->mark().exists())
		{
			if (step->mark().value() == mark)
			{
				return sad::pipeline::Pipeline::StepListPosition(steps, i);
			}
		}
	}
	return sad::pipeline::Pipeline::StepListPosition(NULL, 0);
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByStep(StepsList* steps, sad::pipeline::Step* step)
{
	for(unsigned int i = 0; i < steps->size(); i++) 
	{
		sad::pipeline::Step* pstep= (*steps)[i];
		if (step == pstep)
		{
			return sad::pipeline::Pipeline::StepListPosition(steps, i);
		}
	}
	return sad::pipeline::Pipeline::StepListPosition(NULL, 0);
}

sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByMark(const sad::String & mark)
{
	sad::pipeline::Pipeline::StepsList * lists[3] = {
		&m_system_steps_before_user,
		&m_user_steps,
		&m_system_steps_after_user
	};
	sad::pipeline::Pipeline::StepListPosition pos(NULL, 0);
	for(int i = 0; i < 3 && pos.p1() == NULL; i++)
	{
		pos = sad::pipeline::Pipeline::findByMark(lists[i], mark);
	}
	return pos;
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByStep(sad::pipeline::Step* step)
{
	sad::pipeline::Pipeline::StepsList * lists[3] = {
		&m_system_steps_before_user,
		&m_user_steps,
		&m_system_steps_after_user
	};
	sad::pipeline::Pipeline::StepListPosition pos(NULL, 0);
	for(int i = 0; i < 3 && pos.p1() == NULL; i++)
	{
		pos = sad::pipeline::Pipeline::findByStep(lists[i], step);
	}
	return pos;
}

void sad::pipeline::Pipeline::clearSteps(StepsList* steps)
{
	for(unsigned int i = 0; i < steps->size(); i++)
	{
		delete (*steps)[i];
	}
	steps->clear();
}

void sad::pipeline::Pipeline::addNow(PipelineInsertionData o)
{
	switch(o.p1())
	{
		case sad::pipeline::PIT_BEGIN: 
		{
			if (o.p3()->source() == sad::pipeline::ST_SYSTEM)
				m_system_steps_before_user.insert(o.p3(), 0);
			else
				m_user_steps.insert(o.p3(), 0);
			break;
		}
		case sad::pipeline::PIT_END:
		{
			if (o.p3()->source() == sad::pipeline::ST_SYSTEM)
				m_system_steps_after_user << o.p3();
			else
				m_user_steps << o.p3();
			break;
		}
		case sad::pipeline::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION:
		{
			m_system_steps_before_user << o.p3();
			break;
		}
		case sad::pipeline::PIT_SYSTEM_AFTER_LAST_USER_ACTION:
		{
			m_system_steps_after_user << o.p3();
			break;
		}
		case sad::pipeline::PIT_BEFORE:
		{
			StepListPosition pos = findByMark(o.p2().value());
			StepsList * list = pos.p1();
			size_t position = pos.p2();
			if (list != NULL)
			{
				list->insert(o.p3(), position);
			}
			break;
		}
		case sad::pipeline::PIT_AFTER:
		{
			StepListPosition pos = findByMark(o.p2().value());
			StepsList * list = pos.p1();
			size_t position = pos.p2();
			if (list != NULL)
			{
				if (list->size() - 1 == position)
				{
					(*list) << o.p3();
				} 
				else
				{
					list->insert(o.p3(), position + 1);
				}
			}
			break;
		}
	};
}

void sad::pipeline::Pipeline::removeNow(sad::pipeline::Step * o)
{
	StepListPosition pos = findByStep(o);
	StepsList * list = pos.p1();
	size_t position = pos.p2();
	if (list != NULL)
	{
		list->removeAt(position);
	}
}


void sad::pipeline::Pipeline::clearNow()
{
	sad::pipeline::Pipeline::clearSteps(&m_user_steps);
}


void sad::pipeline::Pipeline::appendStateTransition(
	sad::hfsm::Machine * machine, 
	const sad::String & state
)
{
	append(new sad::hfsm::MachineStateChangeTask(machine, state) );
}

