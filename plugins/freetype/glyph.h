/*! \file glyph.h
	\author HiddenSeeker

	Describes a glyph as a bitmap and it's metric
 */
#pragma once
#include "texture.h"

#include <maybe.h>

namespace sad
{

namespace freetype
{

class Glyph
{
public:
	/*! A texture as a bitmap to be used, when rendering
	 */
	sad::freetype::Texture Texture;
	/*! An  index of char, which can be passed as index of char in freetype
	 */
	unsigned int  Index;
	/*! Bitmap width
	 */
	float  Width;
	/*! Bitmap height
	 */
	float  Height;
	/*! A width in texture coordinates
	 */
	float TexCoordinateWidth;
	/*! A height in texture coordinates
	 */
	float TexCoordinateHeight;
	/*! A vertical bearing  as
		distance from baseline to top point of glyph
	 */ 
	float BearingY;
	/*! A vertical distance from baseline to bottom point
		of glyph
	 */
	float Descender;
	/*! How long should pen move, after glyph was rendered
	 */
	float AdvanceX;

	/*! Maximum Y of bounding box
	 */
	int YMax;
	/*! Minimum Y of bounding box
	 */
	int YMin;

	/*! Default glyph fills with zeroes
	 */
	Glyph();

	/*! Creates a new glyph for specified character, building a texture for it
		\param[in] face a face
		\param[in] c a character, which is stored in glyph
	 */
	Glyph(FT_Face face, unsigned char c);
	
	/*! Renders a glyph at specified baseline position. Note, that this
		is BASELINE position. 
		\param[in] x X coordinate position
		\param[in] y Y coordinate position
	 */
	void render(float x, float y);
	/*! Tries to get a glyph for a face and char c
		\param[in] face face to be used
		\param[in] c character
		\param[out] index index of character in list
		\return glyph value
	 */
	static sad::Maybe<FT_Glyph> glyph(FT_Face face, unsigned char c, unsigned int & index);
private:
	/*! Sets all metrics of glyph from specified freetype glyph
		\param[in] face  a global face
		\param[in] glyph a freetype glyph
	 */
	void makeGlyph(FT_Face face, FT_Glyph glyph);
	/*! Makes empty glyph, if no glyph data available
	 */
	void makeEmptyGlyph();
};

}

}
