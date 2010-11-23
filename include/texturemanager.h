/*!  \file    texturemanager.h
\author  Unknown
\brief   ���������� ����������
*/
#include "texture.h"
#include "templates/hhash.hpp"
#pragma once

namespace sad
{
	/*! ���������� ���������� ������������� �����.
	� ��� ����� ������ ��-��� ������� ������������� ����� ��.
	*/
	class TextureManager
	{
	private:
		hst::hash<hst::string,Texture *> m_data; //!< ������ � ���������

		static TextureManager * m_instance;      //!< �������

		TextureManager();
		TextureManager(const TextureManager &);
		TextureManager & operator=(const TextureManager &);
		static void freeInstance();
	public:
		/*! ���������� ������� ������� ���������
		*/
		static TextureManager * instance();
		/*! ����������
		*/
		~TextureManager();
		/*! ���������� ������� �������� �� �����, ���� ����, ����� NULL
		\param[in] name ���
		*/
		Texture *  get(const hst::string & name);
		/*! ��������� �������� � ������
		\param[in] name ���
		\param[in] tex  ��������
		*/
		void load(const hst::string & name, Texture * tex);
		/*! ��������� ��������
		\param[in] name ���
		*/
		void unload(const hst::string & name);
	};

}
