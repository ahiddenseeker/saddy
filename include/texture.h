/*! \file    texture.h
\author  Myself;Yourself
\brief   ���, ��� ��������� � �������� ����������� �����
*/
#include "primitives/hcolor.h"
#include "templates/hstring.h"
#include "templates/hwstring.h"
#include "templates/hlvector.hpp"
#pragma once


namespace sad
{
	static unsigned int globalID=0;

	/*! ����� ��������
	*/
	class Texture
	{
	public:
		/*! ��������� ����� ���������������
		*/
		enum Mode
		{
			BORDER_REPEAT,
			BORDER_CLAMP,
			COMBINE_REPLACE,
			COMBINE_ADD,
			COMBINE_MODULATE
		};

	private:

		hst::vector<Uint8> m_data;   //!< ���� ��������
		Uint8              m_filter; //!< ������ ����������
		Uint8              m_bpp;    //!< ����� ����� �� ������
		unsigned int       m_width;  //!< ������
		unsigned int       m_height; //!< ������
		unsigned int       m_id;     //!< ID �������� � �����������
		Mode               m_mode;   //!< ����� ������

	public:

		/*! ������� ������ ��������
		*/
		Texture();
		/*! ����������
		*/
		~Texture();
		/*! ������ �������
		*/
		void buildMipMaps();
		/*! �������� �� �����, � ����������� �� ����������
		\param[in] filename ��� �����
		*/
		bool load(const hst::string & filename);
		/*! �������� �� ����� BMP
		\param[in] filename ��� �����
		*/
		bool loadBMP(const hst::string & filename);
		/*! �������� �� ����� TGA
		\param[in] filename ��� �����
		*/
		bool loadTGA(const hst::string & filename);
		/*! �������� �� ����� TGA
		\param[in] filename ��� �����
		*/
		bool loadTGA(const hst::wstring & filename);
		/*! �������� �� ����� PNG
		\param[in] filename ��� �����
		*/
		bool loadPNG(const hst::string & filename);
		/*! �������� �� ����� PNG
		\param[in] filename ��� �����
		*/
		bool loadPNG(const hst::wstring & filename);
		/*! �������� ������������������ �������� ������� TGA
		*/
		void loadDefaultTGATexture();
	};
}