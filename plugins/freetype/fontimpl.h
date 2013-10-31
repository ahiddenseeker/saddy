/*! \file   fontimpl.h
	\author HiddenSeeker

	An implementation of freetype font
 */
#pragma once
#include <sadptrhash.h>
#include "texture.h"

#include <sadstring.h>
#include <sadsize.h>
#include <sadpoint.h>

namespace sad
{

namespace freetype
{

class FixedSizeFont;


class FontImpl
{
public:
	/*! Creates new font implementation
	 */
	FontImpl();
	/*! Frees all resources from implementation
	 */
	~FontImpl();
	/*! Loads a font
	    \param[in] filename a path to TTF or other files, to be loaded via freetype
	 */
	virtual bool load(const sad::String & filename);
	/*! Renders a string on screen
	    \param[in] str string
		\param[in] p   upper-left point in viewport coordinates
		\param[in] ratio a ratio for line spacing
	 */
	virtual void render(
		const sad::String & str,
		const sad::Point2D & p,
		float ratio
	); 
	/*! Sets a size for font implementation
		\param[in] size a new size
	 */
	void setSize(unsigned int size);
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\param[in] ratio a ratio for line-spacing
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str, float ratio);	
	/*! Returns a built-in line spacing
		\return line spacing
	 */
	float builtinLineSpacing() const;
protected:
	/*! Creates and returns current size for data
		\param[in] size a new size for data
		\return call list for rendering
	 */
	sad::freetype::FixedSizeFont * fontForSize(unsigned int size) const;
	/*! A freetype library descriptor
	 */
	FT_Library m_library;
	/*! A used freetype face
	 */
	FT_Face    m_face;
	/*! A call lists as map from type to data 
	 */
	typedef sad::PtrHash<unsigned int, sad::freetype::FixedSizeFont> SizeMap;
	/*! A cache for lists, which renders a string
	 */
	SizeMap m_size_cache;
	/*! Cleans all resources, freeing all data
	 */
	void cleanup();
	/*! A cached size
	 */
	unsigned int m_cached_size;
	/*! A cached current font
	 */
	sad::freetype::FixedSizeFont * m_font;
	/*! A first creating time, when font is created
	 */
	bool m_created_now;
};

}

}
