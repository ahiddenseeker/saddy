/*! \file   scene.h
\author HiddenSeeker
\brief  ����� ����������� ����� �����
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include <assert.h>
#pragma once

namespace sad
{

/*! ����� �������� ������� �����
*/
class BasicNode
{
	float m_x;    //!< �������
	float m_y;    //!< �������
protected:
	int   m_type; //!< ��� ������� (������������)
    
	BasicNode();
public:
	static int Type; //!< ����������� ���
	/*! ���
	*/
	int type() const;
	/*! ���������, ������� ����� ����������� ��� ���������� �������.
	*/
	virtual void render()=0;
	/*! ����������
	*/
	virtual ~BasicNode();
};

/*! ����� ������
*/
class Camera
{
 private:
	     float m_curX;
		 float m_curY;
		 float m_curZ;

		 float m_rotX;
		 float m_rotY;
		 float m_rotZ;
		 
		 float m_angle;
 public:
	     /*! Default constructed camera
		 */
	     Camera();
		 /*! Custom camera
		     \param x    x coord
			 \param y    y coord
			 \param z    zoom
			 \param angle angle
			 \param rx    rotation of x
			 \param ry    rotation of y
			 \param rz    rotation of z
		 */
         Camera(float x, float y, float z,float angle=0, float rx=0, float ry=0, float rz=0);

		 /*! Applies a camera
		 */
		 void apply();
		 
		 inline float & x() { return m_curX;}
         inline float & y() { return m_curY;}
         inline float & z() { return m_curZ;}

		 inline float & rotationX() { return m_rotX;}
         inline float & rotationY() { return m_rotY;}
         inline float & rotationZ() { return m_rotZ;}
         inline float & angle() { return m_angle;}

		 /*! Destructor
		 */
		 ~Camera();
};
/*! ����� �����
*/
class Scene
{
private:
	hst::vector<BasicNode *>   m_layers;                //!< ����, ������ ������� � �� �� ����������
	hst::hash<hst::string,unsigned long>  m_nodehash;   //!< ������� �� ����
	hst::vector<BasicNode *>   m_marked;                //!< ���������� ��� �������� �������
	hst::vector< hst::triplet<BasicNode *,hst::string,unsigned long > >   m_toadd;                 //!< ���������� ��� ���������� ������� 
	sad::Camera              m_camera;                  //!< ������� ������
public:
	sad::Camera   & camera();  //!< ������� ������

	Scene();    //!< ������ �����
	~Scene();   //!< ����������

	/*! ������� ��� �� �����
	*/
	void clear();
	/*! �������� ������� ��� �������� � ��������� �����
	    \param[in] what ��� ���� ����� �������
	*/
	void markForDeletion(BasicNode * what);
	/*! �������� ������� ��� ���������� � ��������� �����
	    \param[in] node  ������ 
      	\param[in] name  ���
	    \param[in] lay   ����
	*/
	void markForAddition(BasicNode * node, 
		                 const hst::string & name=hst::string(),
		                 unsigned long lay=(unsigned long)-1);

	/*! ��������� ������ � �����
	\param[in] node  ������ 
	\param[in] name  ���
	\param[in] lay   ����
	*/
	void add(
		BasicNode * node, 
		const hst::string & name=hst::string(),
		unsigned long lay=(unsigned long)-1
		);
	/*! ������� ������ �� �����
	\param[in] name ��� �������
	*/
	void remove(const hst::string & name);
	/*! �������� �����
	*/
	void render();
	/*! ��������� ������� �� �����. ���� �� �������, �� ������ NULL
	\param[in] name ���
	*/
	template<typename T> T * get(const hst::string & name); 
};

}

typedef sad::BasicNode BasicNode;
typedef sad::Scene     Scene;

//=================�������� ��� ��������� �����====================
template<typename T> T * sad::Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert(ind->type()==T::Type && "Inconsistent types!");
	return static_cast<T *>(ind);
}
