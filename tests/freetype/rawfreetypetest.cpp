#include "helper.h"
#include <math.h>

/*!
 * Tests used workflow with freetype, using operations with raw freetype
 * A mistake here identificates, that here are major problems and freetype
 * implementation has changed
 */
struct RawFreetypeTest : tpunit::TestFixture
{
public:
   
   RawFreetypeTest() : tpunit::TestFixture(
	   TEST(RawFreetypeTest::test)
   ) {}
   
   void test()
   {
	   FT_Library library;
	   FT_Face    face;
		
	   FT_Init_FreeType(&library);

	   ASSERT_TRUE( FT_New_Face(library, SOURCE_FONT, 0, &face) == 0 );

	   double pixelheight = 72.0;
	   double ptheight = pixelheight / 96.0 * 72.0;
	   int  pointheight = ((int)ceil(ptheight)) << 6;

	   FT_Size_RequestRec_ req;
	   req.type = FT_SIZE_REQUEST_TYPE_BBOX;
	   req.horiResolution = 96;
	   req.vertResolution = 96;
	   req.height = pointheight;
	   req.width = pointheight;
	   
	   FT_Request_Size(face, &req);
	   //FT_Set_Char_Size(face, pointheight, pointheight, 96, 96);

	   int ppem = face->size->metrics.y_ppem;
	   double  height = ppem * ((double)(face->height) / face->units_per_EM);
	   double  ascender = ppem * ((double)(face->ascender) / face->units_per_EM);
	   double  descender = ppem * ((double)(face->descender) / face->units_per_EM);

	   double mybaseheight = ppem * ((double)(face->ascender - face->descender + face->height) /  face->units_per_EM);

	   double totalheight = ascender - descender;

	   FT_Load_Glyph( face, FT_Get_Char_Index( face, '(' ), FT_LOAD_DEFAULT);
	   FT_Glyph glyph = NULL;
	   FT_Get_Glyph(face->glyph, &glyph);
	   FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	   FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	   FT_Bitmap & bitmap = bitmap_glyph->bitmap;

	   for(int i = bitmap.rows - 1; i > -1; i--)
	   {
		   for(int j = 0; j < bitmap.width; j++)
		   {
			   unsigned char pixel = bitmap.buffer[i * bitmap.width + j];
			   if (pixel == 0) 
			   {
				   printf(" ");
			   } 
			   else
			   {
				   printf("1", pixel);
			   }
		   }
		   printf("\n");
	   }
	   FT_Done_Face(face);
	   FT_Done_FreeType(library);
   }

} _raw_freetype_test;