/*! \file  include/font.h
	\author HiddenSeeker

	Defines a basic class, for all renderable fonts in Saddy engine
 */
#pragma once
#include "sadpoint.h"
#include "sadsize.h"
#include "sadstring.h"
#include "sadcolor.h"

#include "resource/resource.h"

namespace sad
{

/*! A basic class for font, which renders all fonts in Saddy engine
 */ 
class Font: public sad::resource::Resource
{
SAD_OBJECT
public:
	/*! Constructs new font with size of 14px
	 */
	Font();
	/*! Renders a string on screen
	    \param[in] str string
		\param[in] p   upper-left point in viewport coordinates
	 */
	virtual void render(const sad::String & str,const sad::Point2D & p) = 0; 
	/*! Sets a color, which label will be rendered with, if label is rendered,
		using this font
		\param[in] c color of font
	 */
	virtual void setColor(const sad::AColor & c);
	/*! Returns a color, which label will be rendered with, if label is rendered,
		using this font
		\return color
	 */
	virtual sad::AColor color() const; 
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str) = 0;
	/*! Returns size of font in pixels
		\return size of font in pixels
	 */
	inline unsigned int size() const
	{
		return m_size;
	}
	/*! Sets size of font in pixels
		\param[in] size size of font in points
	 */
	virtual void setSize(unsigned int size);
	/*! Returns a builtin line spacing
		\return line spacing
	 */
	virtual float builtinLineSpacing() const;
	/*! Sets size of font in points
		\param[in] size size of font in points
	 */
	inline void setSizeInPoints(unsigned int size)
	{
		setSize((unsigned int)(size * 1.333));
	}
	/*! Returns size of font in points
		\return size of points
	 */
	inline unsigned int sizeInPoints() const
	{
		return (unsigned int)(m_size / 1.333);
	}
	/*! Returns a line spacing ratio, relative to built-in
		\return line spacing ratio
	 */
	inline float lineSpacingRatio() const
	{
		return m_linespacing_ratio;
	}
	/*! Sets line spacing ratio, relative to built-in
		\param[in] ratio a new ratio
	 */
	inline void setLineSpacingRatio(float ratio)
	{
		m_linespacing_ratio = ratio;
	}
	/*! Returns a line spacing
		\return a new line spacing
	 */
	inline float lineSpacing() const
	{
		return m_linespacing_ratio * this->builtinLineSpacing();
	}
	/*! Sets a new line spacing
		\param[in] size a size of line spacing
	 */
	inline void setLineSpacing(float size)
	{
		m_linespacing_ratio = size / this->builtinLineSpacing();
	}
	/*! Kept for purpose of inheritance
	 */
	virtual ~Font();
protected:
	/*! Size of font in pixels
	 */
	unsigned int m_size;
	/*! A linespacing ratio for font
	 */
	float m_linespacing_ratio;
	/*! A color of font
	 */
	sad::AColor m_color;
	/*! A buffer, used by fonts to store current color
	 */
	int m_current_color_buffer[4]; 
	/*! Sets current OpenGL state rendering color to an inner color
	 */
	void setCurrentColor();
	/*! Restores OpenGL state rendering color
	 */
	void restoreColor();
};

}
