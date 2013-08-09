#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/collides1d.h"
#pragma warning(pop)


/*!
 * Tests object logic
 */
struct Collides1DTest : tpunit::TestFixture
{
 public:
   Collides1DTest() : tpunit::TestFixture(
	   TEST(Collides1DTest::test)	   
   ) {}
   
   void test()
   {
	   ASSERT_FALSE( p2d::collides1D(0.0, 0.5, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(0.0, 1.0, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(0.0, 1.5, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(0.0, 2.0, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(0.0, 3.0, 1.0, 2.0) );

	   ASSERT_TRUE( p2d::collides1D(1.0, 1.0, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(2.0, 2.0, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(1.0, 2.0, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(1.0, 1.5, 1.0, 2.0) );
	   ASSERT_TRUE( p2d::collides1D(1.0, 2.5, 1.0, 2.0) );

	   ASSERT_TRUE( p2d::collides1D(2.0, 2.8, 1.0, 2.0) );
	   ASSERT_FALSE( p2d::collides1D(2.1, 2.8, 1.0, 2.0) );
   }

} test1;
