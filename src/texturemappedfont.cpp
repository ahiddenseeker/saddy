#include "texturemappedfont.h"
#include "texturemanager.h"
#include "renderer.h"

sad::TextureMappedFont::TextureMappedFont() : sad::Font(), m_texture(NULL),
m_builtin_linespacing(0), m_size_ratio(1.0)
{

}

sad::TextureMappedFont::~TextureMappedFont()
{

}

sad::Size2D sad::TextureMappedFont::size(const sad::String & str)
{
	sad::Size2D result;
	// If loading was failed, return (0, 0) as size
	if (m_texture == NULL)
		return result;
	
	// A font can have various sizes, so we need ratio to compute new width
	// of glyph
	double height = m_builtin_linespacing * m_size_ratio;

	// Lets break string into lines
	sad::String tmp = str;
	tmp.removeAllOccurences("\r");
	sad::StringList lines = str.split("\n");
	
	// Compute result
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		double linewidth = 0;
		sad::String & string = lines[i];
		for(unsigned int j = 0; j < string.length(); j++)
		{
			unsigned char c = *reinterpret_cast<unsigned char*>(&(string[j]));
			linewidth += m_leftbearings[c] * m_size_ratio;
			linewidth += m_sizes[c].Width * m_size_ratio;
			linewidth += m_rightbearings[c] * m_size_ratio;
		}
		result.Width = std::max(linewidth, result.Width);
	}

	if (lines.size() <= 1)
	{
		result.Height = (lines.size() == 1) ? height : 0;
	}
	else
	{
		result.Height = height + height * (lines.size() - 1) * m_linespacing_ratio; 
	}
	return result;
}


void  sad::TextureMappedFont::render(const sad::String & str,const sad::Point2D & p)
{
	// If loading was failed, do nothing
	if (m_texture == NULL)
		return;

	double x = p.x();
	double y = p.y();

	
	sad::String string = str;
	string.removeAllOccurences("\r");

	
	m_texture->bind();
	glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT);
	
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->setCurrentColor();

	unsigned int glyphheight = 0;
	unsigned int glyphwidth = 0;
	glBegin(GL_QUADS);
	for(unsigned int i = 0;  i < string.length(); i++)
	{
		unsigned char glyphchar = *reinterpret_cast<unsigned char*>(&(string[i]));
		sad::Rect2D & glyph = m_glyphs[ glyphchar ];

		if (string[i] != '\n' && string[i] != '\r')
		{
			x += m_leftbearings[ string[i] ] * m_size_ratio;
			glyphheight = m_sizes[glyphchar].Height * m_size_ratio;
			glyphwidth = m_sizes[glyphchar].Width * m_size_ratio; 

			glTexCoord2f(glyph[0].x(), glyph[0].y()); 
 			glVertex2f(x, y);

			glTexCoord2f(glyph[1].x(), glyph[1].y()); 
 			glVertex2f(x + glyphwidth, y);

			glTexCoord2f(glyph[2].x(), glyph[2].y()); 
 			glVertex2f(x + glyphwidth, y - glyphheight);

			glTexCoord2f(glyph[3].x(), glyph[3].y()); 
 			glVertex2f(x, y - glyphheight);
		}
		if (string[i] != '\n')
		{
			x += glyphwidth;
			x += m_rightbearings[ string[i] ] * m_size_ratio;
		}
		else
		{
			x = p.x();
			y -= m_builtin_linespacing * m_size_ratio * m_linespacing_ratio;
		}
	}
	glEnd();
	this->restoreColor();
	glPopAttrib();
}

bool sad::TextureMappedFont::load(const sad::String & filename, sad::Renderer * r)
{
	return this->load(filename + ".png", filename + ".cfg", r);
}

bool sad::TextureMappedFont::load(
	const sad::String & texturefilename,  
	const sad::String & configfilename,
	sad::Renderer * r
)
{
	// If renderer is null, set renderer to global
	if (r == NULL)
		r = sad::Renderer::ref();

	// Trying to load a textures
	m_texture = new sad::Texture;
	r->textures()->add(texturefilename, m_texture);
	if (!m_texture->load(texturefilename, r))
	{
		r->textures()->remove(texturefilename);
		return false;
	}
	
	bool result = true;
	FILE * fl = fopen(configfilename.data(), "rt");
	if (fl)
	{
		// Count is the first line of format file
		int count = 0;

		// If failed to read file,result is false
		int integralspacing = 0;
		fscanf(fl, "%d", &integralspacing);
		m_builtin_linespacing = (double)integralspacing;
		if (ferror(fl)) 
		{
			result = false;
		}

		// Try to load each glyph
		unsigned int x1 = 0 , y1 = 0, x2 = 0, y2 = 0;
		unsigned char c = 0;
		for (int i = 0; i < 256 && result; i++ )
		{
			fscanf(fl,
				   "%u %u %u %u %d %d\n", 
				   &x1, 
				   &y1, 
				   &x2, 
				   &y2, 
				   m_leftbearings + i, 
				   m_rightbearings + i
				  );
			if (ferror(fl))
			{
				result = false;
			}

			m_sizes[i].Width = abs((int)(x2 - x1));
			m_sizes[i].Height = abs((int)(y2 - y1));
			
			sad::Rect2D rect(
				sad::Point2D(
					(double)x1 / m_texture->width(), 
					(double)y1 / m_texture->height()
				),
				sad::Point2D(
					(double)x2 / m_texture->width(), 
					(double)y2 / m_texture->height()				
				)
			);
			m_glyphs[i] = rect; 
		}
		fclose(fl);
	} 
	else
	{
		result = false;
	}

	// If failed to load, remove texture
	if (result == false)
	{	
		r->textures()->remove(texturefilename);
	} 
	else
	{
		m_size_ratio = (float)(this->sad::Font::size()) / m_builtin_linespacing;
	}
	return result;
}

float sad::TextureMappedFont::builtinLineSpacing() const
{
	return m_builtin_linespacing * m_size_ratio;
}

void sad::TextureMappedFont::setSize(unsigned int size)
{
	m_size_ratio = (float)size / m_builtin_linespacing;
}
