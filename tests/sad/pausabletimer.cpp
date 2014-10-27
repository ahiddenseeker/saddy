#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <pausabletimer.h>
#include <sadsleep.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



/*!
 * Tests semaphore logic
 */
struct SadPausableTimer : tpunit::TestFixture
{
public:

	SadPausableTimer() : tpunit::TestFixture(
		TEST(SadPausableTimer::test)
	) {}

   void test()
   {
	   sad::PausableTimer t;
	   t.start();
	   sad::sleep(200);
	   t.pause();
	   sad::sleep(200);
	   t.resume();
	   sad::sleep(200);
	   double beforestop = t.elapsed();
	   t.stop();
	   sad::sleep(200);	
	   double afterstop = t.elapsed();

	   ASSERT_TRUE(beforestop >= 400.0 && beforestop <= 600.0);
	   ASSERT_TRUE(afterstop >= 400.0 && afterstop <= 600.0);
   }

} _pausable_timer_test;
