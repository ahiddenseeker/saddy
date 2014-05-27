/*! \file defaultimage.h
	\author HiddenSeeker

	Contains data for default image for resource cache
 */
#pragma once

#include <texture.h>

namespace resourcetreewidget
{

/*! A container class for data, for default image of ResourceTreeWidget
 */
class DefaultImage
{
public:
	/*! A height of default image
	 */
	static size_t Height;
	/*! A width of default image
	 */
	static size_t Width;
	/*! A data for default image
	 */
	static sad::uchar Data[22500];
};

}