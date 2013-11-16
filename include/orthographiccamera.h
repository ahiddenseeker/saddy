/*! \file   orthographiccamera.h
    \author HiddenSeeker
    
	An orthographic camera, which sets of orthographic projection, so all viewed objects will be withing match
	a rectangle with specified width and height
*/
#pragma once
#include "camera.h"
#include <cstring>

namespace sad
{

class Renderer;
/*! \class OrthographicCamera

	An orthographic camera, which sets of orthographic projection, so all viewed objects will be withing match
	a rectangle with specified width and height
 */
class OrthographicCamera: public sad::Camera
{
public:
	/*! Creates a camera, with rectangle width and height, taken from 
		window settings of renderer.
	 */
	OrthographicCamera();
	/*! Creates a camera, width and height of orthographic projections, 
		so all viwed objects will be within
		width and specified height
		\param[in]  width of bounding rectangle
		\param[in]  height of bounding rectangle
	 */
	OrthographicCamera(int width, int height);
	/*! Applies an orthographic projection matrices, using gluOrtho2D function, clearing
		another matrices. After that applies matrix transformation
	 */
	virtual void apply();
	/*! Kept for purpose of inheritance
	 */
	virtual ~OrthographicCamera();
private:
	/*! Whether width and height is already set in camera
	 */
	bool m_fetched;  
	int  m_width;    //!< Width or viewed maximal X coordinate 
	int  m_height;   //!< Height  or viewed maximal Y coordinate 
};

}
