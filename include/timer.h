/*! \file timer.h
	\author HiddenSeeker

	Contains crossplatform implementation for a timer
 */
#pragma once

namespace sad
{

namespace os
{
class TimerImpl;
}

/*! A crossplatform implementation for a timer
 */
class Timer
{
public:
	/*! Creates new timer and tries to start it as soon as possible
	 */
	Timer();
	/*! Copies another timer state
		\param[in] o other timer
	 */
	Timer(const sad::Timer & o);
	/*! Copies another timer state
		\param[in] o other timer
		\return self-reference
	 */
	sad::Timer& operator=(const sad::Timer & o);
	/*! Starts a timer
	 */
	void start();
	/*! Stops a timer. After stopping, getting elapsed time is possible
		via sad::Timer::elapsed()
	 */
	void stop();
	/*! Returns elapsed time, between calls of sad::Timer::start() and
		sad::Timer::stop() using high-perfomance counter, if possible. For
		some chipsets could fall to wallclock time, since correct information
		won't be available
	 */
	double elapsed() const;
protected:
	/*! An implementation of a timer
	 */
	sad::os::TimerImpl * m_dptr;
};

}
