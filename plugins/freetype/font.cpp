#include <freetype/font.h>
#include "fontimpl.h"
#include <log/log.h>


sad::freetype::Font::Font()
: m_dptr(new sad::freetype::FontImpl())
{

}

bool sad::freetype::Font::load(const sad::String & filename)
{
	return m_dptr->load(filename);
}

void sad::freetype::Font::render(const sad::String & str,const sad::Point2D & p)
{
	m_dptr->setSize(m_size);

	this->setCurrentColor();
	m_dptr->render(str, p);
	this->restoreColor();
}

bool sad::freetype::Font::builtinLineSpacing() const
{
	return m_dptr->builtinLineSpacing();
}

sad::Size2D sad::freetype::Font::size(const sad::String & str)
{
	m_dptr->setSize(m_size);
	return m_dptr->size(str);
}

sad::freetype::Font::~Font()
{
	delete m_dptr;
}

// Uncommend to enable glyph rendering debug
// #define FREETYPE_GLYPH_DEBUG

#ifdef FREETYPE_GLYPH_DEBUG

static unsigned const int render_rect_indices_count = 4;

static unsigned int render_rect_indices[render_rect_indices_count][2] = 
{
	{0, 1},
	{1, 2},
	{2, 3},
	{3, 0}
};
/*! Renders rectangle
	\param[in] x1 X coordinate of first point
	\param[in] y1 Y coordinate of first point
	\param[in] x2 X coordinate of second point
	\param[in] y2 Y coordinate of second point
	\param[in] c  ciolor
 */
void debug_render_rect(double x1, double y1, double x2, double y2, const sad::Color & c)
{
	sad::Rect2D r(x1, y1, x2, y2);

	glDisable(GL_TEXTURE_2D);
		
	glBegin(GL_LINES);
	glColor3ub(c.r(), c.g(),  c.b());

	for(int i = 0; i < render_rect_indices_count; i++)
	{
		const sad::Point2D & p1 = r[render_rect_indices[i][0]];
		const sad::Point2D & p2 = r[render_rect_indices[i][1]];

		glVertex2f((float)(p1.x()), (float)(p1.y()));
		glVertex2f((float)(p2.x()), (float)(p2.y()));
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

#endif
