/*! \file primitiverenderer.h
	\author HiddenSeeker

	A special part of renderer, which renders lines and rectangles
 */
#pragma once
#include "sadrect.h"
#include "sadcolor.h"

namespace sad
{

/*! \class PrimitiveRenderer
	
	A special part of renderer, which response to line rendering and rectangles
 */
class PrimitiveRenderer
{
public:
	/*! Constructs default primitive renderer
	 */
	PrimitiveRenderer();
	/*! You can override primitive render to render own primitives in a way, you like it
	 */
	virtual ~PrimitiveRenderer();
	/*! Renders a line with specified color
		\param[in] p1 a first point of renderer
		\param[in] p2 a second point of renderer
		\param[in] c  a color, with which line should be rendered
	 */
	virtual void line(
		const sad::Point2D & p1,
		const sad::Point2D & p2,
		const sad::AColor & c
	);
	/*! Renders a rectangle with specified color
		\param[in] r rectangle to be rendered
		\param[in] c a color, with which line should be rendered
	 */
	virtual void rectangle(
		const sad::Rect2D & r,
		const sad::AColor & c
	);
};

}
