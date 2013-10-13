#include <log/consoletarget.h>
#include <os/consoleimpl.h>
#include <3rdparty/format/format.h>

sad::log::ConsoleTarget::ConsoleTarget(const sad::String & format, int minpriority,  bool colored , bool allocate_console )
: m_console(new sad::os::ConsoleImpl())
{
	m_format = format;
	m_min_priority = minpriority;
	if (colored)	this->createColoredOutput(); 
	else            this->createNormalOutput();
	if (allocate_console) 
		m_console->createConsole();
}

void sad::log::ConsoleTarget::receive(const sad::log::Message & message)
{
	if (((int)message.priority()) < m_min_priority)
		return;
	std::string mesg = str(fmt::Format(m_format) 
						   << message.stime() 
						   << message.spriority()
						   << formatSubsystem(message)
						   << formatFileLine(message)
						   << message.message()
						  );
	m_console->setColorMode(m_coloring[message.priority()].p2(),  m_coloring[message.priority()].p1());
	m_console->print(mesg.c_str());
}

sad::log::ConsoleTarget::~ConsoleTarget()
{
	m_console->clearColorMode();
	delete m_console;
}


std::string sad::log::ConsoleTarget::formatSubsystem(const sad::log::Message & message)
{
	if (message.subsystem().length() == 0)
		return "";
	std::string result = message.subsystem().data();
	result += ": ";
	return result;
}

std::string sad::log::ConsoleTarget::formatFileLine(const sad::log::Message & message)
{
	if (message.fileline().length() == 0)
		return "";
	std::string result = "[";
	result += message.fileline().data();
	result += "] ";
	return result;
}

void sad::log::ConsoleTarget::createColoredOutput()
{
	m_coloring.insert(sad::log::FATAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_RED));
	m_coloring.insert(sad::log::CRITICAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_MAGENTA));
	m_coloring.insert(sad::log::WARNING, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_YELLOW));
	m_coloring.insert(sad::log::MESSAGE, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::WHITE));
	m_coloring.insert(sad::log::DEBUG, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_BLUE));
	m_coloring.insert(sad::log::USER, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::LIGHT_CYAN));
	m_coloring.insert(sad::log::SADDY_INTERNAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::DARK_MAGENTA));
}


void sad::log::ConsoleTarget::createNormalOutput()
{
	m_coloring.insert(sad::log::FATAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::CRITICAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::WARNING, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::MESSAGE, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::DEBUG, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::USER, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
	m_coloring.insert(sad::log::SADDY_INTERNAL, sad::Pair<sad::log::Color, sad::log::Color>(sad::log::NONE, sad::log::NONE));
}
