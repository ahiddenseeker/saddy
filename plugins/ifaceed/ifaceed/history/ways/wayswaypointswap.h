/*! \file wayswaypointswap.h
 *  \author HiddenSeeker
 *
 * Describes a simple command for swapping points
 */
#pragma once
#include "../command.h"
#include <p2d/app/way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::WayPointSwap
 *   A simple command for swapping points in way
 */
class WayPointSwap: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
		\param[in] pos1 first position
		\param[in] pos2 second position
     */
    WayPointSwap(sad::p2d::app::Way* w, int pos1, int pos2);
    /*! Could be inherited
     */
    virtual ~WayPointSwap();
    /*! Swaps waypoints
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Swaps waypoints
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current selected way
     */
    sad::p2d::app::Way* m_way;
    /*! A first index for a point
	 */
	int m_pos1;
	/*! A second index for a point
	 */
	int m_pos2;
	/*! Tries to update ui for swapped entries
		\param[in] ob editor
	 */
	void tryUpdateUI(core::Editor* ob);
};

}

}
