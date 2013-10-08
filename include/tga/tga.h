/*! \file tga.h
	Contains the loader of texture of tga format.
*/
#include "types.h"
#include "texture.h"
#include <vector>
#include <string.h>
#include <wchar.h>
#pragma once


namespace tga
{
	/*! Contains data of texture.
	 */
	class Info
	{
	public:
		std::vector<sad::uchar> & m_TGA_data;		/*!< Texture's bits.								  */
		sad::uchar                m_TGA_filter;	/*!< Filtration metod.									  */
		sad::uchar                m_TGA_bpp;		/*!< Bits per pixel.									  */
		unsigned int         m_TGA_width;		/*!< Width of the image.							  */
		unsigned int         m_TGA_height;	/*!< Height of the image.								  */
		unsigned int	     m_TGA_imageSize;	/*!< Size of texture image.							  */
		bool                 m_horzflip;      /*!< Whether we must flip horizontally                  */
		bool                 m_vertflip;      /*!< Whether we must flip vertically                    */

		inline Info(std::vector<sad::uchar> * data) : m_TGA_data(*data) {}
	};

	/*! Contains the header of Targa textures format (TGA)								(Total: 18 byte)
	 */
	struct TGAHeader
	{
		unsigned char  idLength;			/*!< Length of ID field.						(1 byte) */
		unsigned char  colorMapType;		/*!< Type of color map.							(1 byte) */
		unsigned char  imageType;			/*!< type of image.								(1 byte) */

		unsigned short colorMapOrigin;		/*!< First entry of color map.					(2 byte) */
		unsigned short colorMapSize;		/*!< Length of color map.						(2 byte) */
		unsigned char  colorMapEntrySize;	/*!< Size of color map.							(1 byte) */

		unsigned short XOrigin;				/*!< Horizontal coordinate of image's origin.	(2 byte) */
		unsigned short YOrigin;				/*!< Vertical coordinate of image's origin.		(2 byte) */
		unsigned short width;				/*!< Width of the image.						(2 byte) */
		unsigned short height;				/*!< Height of the image.						(2 byte) */
		unsigned char  bitsPerPix;			/*!< Bits per pixel.							(1 byte) */
		unsigned char  imageDescriptor;		/*!< Bits of image descriptor.					(1 byte) */
	};

	/*! Types of image.
	 */
	const char TGA_NO_DATA		  = 0;		/*!< Image doesn't contains any data.					 */
	const char TGA_COLORMAP		  = 1;		/*!< Color map without RLE compression.					 */
	const char TGA_TRUECOLOR	  = 2;		/*!< RGB image without RLE compression.					 */
	const char TGA_MONOCHROME	  = 3;		/*!< Monochrome image without RLE compression.			 */
	const char TGA_COLORMAP_RLE   = 9;		/*!< Color map with RLE compression.					 */
	const char TGA_TRUECOLOR_RLE  = 10;		/*!< RGB image with RLE compression.					 */
	const char TGA_MONOCHROME_RLE = 11;		/*!< Monochrome image with RLE compression.				 */

	/*! Reads data from TGA file.
	 */
	bool readTGA(FILE *handler, tga::Info & data);

	/*! Returns the header of TGA texture.
	 \param[in] buf pointer to bytes array of TGA header.
	 \return header object.
	 */
	TGAHeader getHeader(sad::uchar *buf);

	/*! Loads uncompressed type of TGA texture.
	\param[out] data reference to the object of texture's data.
	\param[in] hFile pointer to handler of file.
	 */
	bool loadUnCompressed(tga::Info & data, FILE *hFile);

	/*! Loads compressed type of TGA texture.
	\param[out] data reference to the object of texture's data.
	\param[in] hFile pointer to handler of file.
	 */
	bool loadCompressed(tga::Info & data, FILE *hFile);
}

namespace sad
{
class TGATextureLoader: public TextureLoader
{
public:
	/*! Loads a texture
		\param[in] file
		\param[in] texture
	 */
	virtual bool load(FILE * file, sad::Texture * texture);

	~TGATextureLoader();
};

}