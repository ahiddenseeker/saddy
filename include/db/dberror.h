/*! \file db/dberror.h
	\author HiddenSeeker
	
	Contains definition of class sad::db::Error.

	This is a basic class for all db-related errors
 */
#pragma once
#pragma once
#include <stdexcept>
#include "../sadstring.h"
#include "../3rdparty/format/format.h"
#include "../3rdparty/picojson/picojson.h"
#include "../object.h"

namespace sad
{

namespace db
{

/*! \class Error

	A basic class for all db-related errors
 */
class Error:public sad::Object, public std::runtime_error  
{	
SAD_OBJECT
public:	
	/*! Constructs new error
		\param[in] message a error message
	 */
	inline Error(const std::string & message) : std::runtime_error(message)
	{
		
	}
	/*! This class can be inherited 
	 */
	virtual ~Error();
};

/*! \class NotImplemented

	Defines that method is not implemented
 */
class NotImplemented: public sad::db::Error
{
SAD_OBJECT
public:
	/*! Formats error
		\param[in] method name of method
		\return error string
	 */
	inline static sad::String format_error(const sad::String & method)
	{
		return fmt::str(fmt::Format("Method \"{0}\" is not implemented") << method);
	}

	/*! Constructs a error
		\param[in] method a name of method
	 */
	inline NotImplemented(const sad::String & method)
	: sad::db::Error(sad::db::NotImplemented::format_error(method)), m_method(method)
	{
		
	}

	/*! This class can be inherited 
	 */
	virtual ~NotImplemented();
protected:
	/*! A name of file.
	 */
	sad::String m_method;	
};

}

}
