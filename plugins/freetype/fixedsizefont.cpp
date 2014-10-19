#include "fixedsizefont.h"
#include "towidechar.h"

#include <sadmutex.h>

#include <math.h>

#ifdef _MSC_VER
	#pragma warning(disable: 4996)
#endif

sad::freetype::FixedSizeFont::FixedSizeFont(
	FT_Library library, 
	FT_Face face, 
	unsigned int height
)
: m_on_gpu(false)
{
	requestSize(library, face, height);

	int ppem = face->size->metrics.y_ppem;
	double linespacinginpt = face->bbox.yMax - face->bbox.yMin;
	m_builtin_linespacing = ppem * ((float)(linespacinginpt) / face->units_per_EM);
	m_bearing_y = ppem * ((float)(face->bbox.yMax) / face->units_per_EM);	
	
	for(unsigned int i = 0; i < 256; i++)
	{
		m_glyphs[i] = new sad::freetype::Glyph(face, (unsigned char)i);
	}

	computeKerning(face);
}


sad::freetype::FixedSizeFont::~FixedSizeFont()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		delete m_glyphs[i];
	}
}

void sad::freetype::FixedSizeFont::uploadedTextures(sad::Vector<unsigned int> & textures)
{
	for(unsigned int i = 0; i < 256; i++)
	{
		if (m_glyphs[i]->Texture.IsOnGPU)
		{
			textures << m_glyphs[i]->Texture.Id;
		}
	}
}

void sad::freetype::FixedSizeFont::markTexturesAsUnloaded()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		m_glyphs[i]->Texture.IsOnGPU = false;
	}
}

static sad::Mutex sad_freetype_font_lock;

void sad::freetype::FixedSizeFont::render(
	const sad::String & s, 
	const sad::Point2D & p, 
	float ratio
)
{
	sad_freetype_font_lock.lock();

	if (!m_on_gpu)
	{
		for(unsigned int i = 0; i < 256; i++)
		{
			m_glyphs[i]->Texture.upload();
		}
		m_on_gpu = true;
	}

	sad::String tmp = s;
	tmp.removeAllOccurences("\r");
	sad::StringList list = tmp.split("\n");

	bool previous = false;
	unsigned char prevchar = 0;
	float xbegin = (float)(p.x());
	float curx = xbegin;
	float cury = (float)(p.y() - m_bearing_y);
	
	for(unsigned int i = 0; i < list.size(); i++)
	{
		for(unsigned int j = 0; j < list[i].size(); j++)
		{
			unsigned char curchar = list[i][j];
			if (previous)
			{
				curx += m_kerning_table[prevchar][curchar];
			}

			sad::freetype::Glyph * g = m_glyphs[curchar];
			g->render(curx, cury);

			curx += g->AdvanceX;
			prevchar = curchar;
			previous = true;
		}
		cury -= m_builtin_linespacing * ratio;
		curx = xbegin;
		previous = false;
	}

	sad_freetype_font_lock.unlock();
}


sad::Texture * sad::freetype::FixedSizeFont::renderToTexture(
	const sad::String & string,
	FT_Library library,
	FT_Face face,
	unsigned int height
)
{
	requestSize(library, face, height);

	sad::String tmp = string;
	tmp.removeAllOccurences("\r");
	tmp.removeAllOccurences("\n");

	sad::Size2D size = this->size(string, 1.0);

	sad::Texture * texture = new sad::Texture();
	texture->width() = (unsigned int)ceil(size.Width);
	texture->height() = (unsigned int)ceil(size.Height);
	texture->bpp() = 32;
	texture->vdata().resize(texture->width() * texture->height() * 4, 255);

	// Fill alpha byte with 0
	for(unsigned int i = 0; i < size.Height; i++)
	{
		for(unsigned int j = 0; j < size.Width; j++)
		{
			*(texture->pixel(i, j) + 3) = 0;
		}
	}

	sad::freetype::Glyph ** glyphs = new sad::freetype::Glyph*[tmp.size()];
	int y_max = -1; 
	for(unsigned int i = 0; i < tmp.size(); i++)
	{
		glyphs[i] = new sad::freetype::Glyph(face, tmp[i]);
		y_max = std::max(y_max, (int)(glyphs[i]->Height));
	}
	// Place glyphs
	bool previous = false;
	unsigned char prevchar = 0;
	unsigned int curx = 0;
	
	for(unsigned int i = 0; i < tmp.size(); i++)
	{
		unsigned char curchar = tmp[i];
		if (previous)
		{
			curx += (unsigned int)(m_kerning_table[prevchar][curchar]);
		}
		previous = true;	

		sad::freetype::Glyph * g = glyphs[i];
		sad::freetype::Texture& tex = g->Texture;

		unsigned int rows = (unsigned int)(g->TexCoordinateHeight * g->Texture.Height);
		unsigned int cols = (unsigned int)(g->TexCoordinateWidth  * g->Texture.Width);
		
		for(unsigned int ix = 0; ix < rows; ix++)
		{
			for(unsigned int iy = 0; iy < cols; iy++)
			{
				unsigned char pixel = g->Texture.Pixels[2 * (ix * (unsigned int)(g->Texture.Width)+ iy)];
				pixel = 255 - pixel;

				unsigned int posy = ix + y_max - (int)(g->Height);
				unsigned int posx = curx + iy;

				if (posy < texture->height() && posx < texture->width())
				{
					sad::uchar * tpixel = texture->pixel(posy, posx);
					tpixel[0] = pixel;				
					tpixel[1] = pixel;
					tpixel[2] = pixel;
					tpixel[3] = 255;
				}
			}
		}

		curx += (unsigned int)(g->AdvanceX);
		prevchar = curchar;
	}

	for(unsigned int i = 0; i < tmp.size(); i++)
	{
		delete glyphs[i];
	}
	delete glyphs;

	return texture;
}

sad::Size2D sad::freetype::FixedSizeFont::size(
	const sad::String & s, 
	float ratio
)
{
	sad::String tmp = s;
	tmp.removeAllOccurences("\r");
	sad::StringList list = tmp.split("\n");

	bool previous = false;
	unsigned char prevchar = 0;
	float curx = 0;
	float maxx = 0;
	for(unsigned int i = 0; i < list.size(); i++)
	{
		curx = 0;
		for(unsigned int j = 0; j < list[i].size(); j++)
		{
			unsigned char curchar = list[i][j];
			if (previous)
			{
				curx += m_kerning_table[prevchar][curchar];
			}

			sad::freetype::Glyph * g = m_glyphs[curchar];
			curx += g->AdvanceX;
			prevchar = curchar;
			previous = true;
		}
		maxx = std::max(maxx, curx);		
	}

	float maxy = 0 ;
	if (list.size() == 1)
	{
		if (fabs(maxx) > 0.001) 
		{
			maxy = m_builtin_linespacing; 
		}
	} 
	else
	{
		maxy = m_builtin_linespacing 
			+ (list.size() - 1) * m_builtin_linespacing * ratio;
	}

	return sad::Size2D(maxx, maxy);
}

void sad::freetype::FixedSizeFont::requestSize(
	FT_Library library,
	FT_Face face, 
	unsigned int height
)
{
	 double pixelheight = height;
	 double ptheight = pixelheight / 96.0 * 72.0;
	 int  pointheight = ((int)ceil(ptheight)) << 6;

	 FT_Size_RequestRec_ req;
	 req.type = FT_SIZE_REQUEST_TYPE_BBOX;
	 req.horiResolution = 96;
	 req.vertResolution = 96;
	 req.height = pointheight;
	 req.width = pointheight;
	   
	 FT_Request_Size(face, &req);
}

void sad::freetype::FixedSizeFont::computeKerning(FT_Face face)
{
	if (FT_HAS_KERNING(face))
	{
		FT_Vector kerning;
		for(unsigned int i = 0 ; i < 256; i++)
		{
			wchar_t c1 = sad::freetype::to_wide_char((unsigned int)i);
			unsigned int index1 = FT_Get_Char_Index(face, c1);
			for(unsigned int j = 0; j < 256; j++)
			{
				wchar_t c2 = sad::freetype::to_wide_char((unsigned int)j);
				unsigned int index2 = FT_Get_Char_Index(face, c2);
				FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);
				m_kerning_table[i][j] = (float)(kerning.x >> 6);
			}
		}
	}
	else
	{
		std::fill_n(&(m_kerning_table[0][0]), 256 * 256, 0.0f);
	}
}
