/*! \file   sprite.h
    \author HiddenSeeker

	Contains a definition of simple 3D Sprite
*/
#include "../include/scene.h"
#include "../include/texture.h"
#include "../include/primitives/hpoint.h"
#pragma once


namespace s3d
{
/*! typedef hst::point<hst::D3,float> point;
*  
*/
 typedef hst::point<hst::D3,float> point;
 
 }
 
/*! Class of simple quad that rotates
*/
class Sprite: public sad::BasicNode
{
 private:
	     float        m_tex_coord[8];    //!< Texture coordinates
		 s3d::point   m_rect[4];         //!< Rectangle
		 
		 sad::Texture * m_tex;           //!< Associated texture
		 /*! Rotates one point around other point
			 \param[in]   p       rotating point
			 \param[in]   pivot   pivot point
			 \param[in]   alpha   alpha angle of rotation on XY axis
			 \param[in]   theta   theta angle of rotation on YZ axis
		 */
		 void rotate(
		             s3d::point & p, 
					 const s3d::point & pivot, 
					 double alpha, 
					 double theta 
					);
 public:
	     static int Type;                //!< Static type
          /*! Creates a simple node
		      \param[in] tex        texture
			  \param[in] rect       rectangle, with point directed clockwise
			  \param[in] tex_coord  texture coordinates
		  */
		 Sprite(
		        sad::Texture * tex,
		        s3d::point rect[4], 
				float * tex_coord=NULL
			   );
		  /*! Creates a simple node
		      \param[in] tex        texture
			  \param[in] rect       rectangle, with point directed clockwise
			  \param[in] tex_coord  texture coordinates
		  */
		 Sprite(
		        sad::Texture * tex,
		        s3d::point rect[4], 
				int * tex_coord
			   );
		  /*! Sets a new texture coordinate
		      \param[in] ind index
			  \param[in] x   x coord
			  \param[in] y   y coord.
		  */
		  void setTexCoords(int ind, float x ,float y);
		  /*! Sets a new texture coordinate
		      \param[in] ind index
			  \param[in] x   x coord
			  \param[in] y   y coord.
		  */
		  void setTexCoords(int ind, int x ,int y);
		  
		  /*! Returns a texture coordinates
		  */
		  const float * texCoords() const;
		  /*! Returns a point reference
		      \param[in] n number of point
			  \return reference to point
		  */
		  s3d::point & point(int n);
		  /*! Calculates a middle point
		      \return middle point
		  */
		  s3d::point middle() const;
		  /*! Moves a points by following vector
			  \param[in] p point
		  */
		  void moveBy(const s3d::point & p);
		  /*! Moves a sprite center to a point
		      \param[in] p point
		  */
		  void moveTo(const s3d::point & p);
		  /*! Rotates a sprite around the pivot in three dimensions
		      \param[in]  pivot pivot point
		      \param[in]  alpha angle of rotation on XY axis
			  \param[in]  theta angle of rotation on YZ axis
		  */
		  void rotate(const s3d::point & pivot, double alpha, double theta);
		  /*! Rotates a sprite around his middle point in three dimensions
		      \param[in]  alpha angle of rotation on XY axis
			  \param[in]  theta angle of rotation on YZ axis
		  */
		  void rotate(double alpha, double theta);
		  /*! Renders it
		  */
		  virtual void render();
		  /*! Destructs it
		  */
		  virtual ~Sprite();
};
