#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#pragma warning(disable: 4101)
#include <stdio.h>
#include <3rdparty/tpunit++/tpunit++.hpp>
#include <sadvector.h>
#include <p2d/findcontactpoints.h>
#include <geometry2d.h>
#pragma warning(pop)

/*!
 * Tests finding contact points between reactangle and bound
 */
struct TestFindContactPointsRtoB : tpunit::TestFixture
{
 public:
   TestFindContactPointsRtoB() : tpunit::TestFixture(
	   TEST(TestFindContactPointsRtoB::test1),
	   TEST(TestFindContactPointsRtoB::test2),
	   TEST(TestFindContactPointsRtoB::test3),
	   TEST(TestFindContactPointsRtoB::test4),
	   TEST(TestFindContactPointsRtoB::test5),
	   TEST(TestFindContactPointsRtoB::test6),
	   TEST(TestFindContactPointsRtoB::test7),
	   TEST(TestFindContactPointsRtoB::test8),
	   TEST(TestFindContactPointsRtoB::test9),
	   TEST(TestFindContactPointsRtoB::test10),
	   TEST(TestFindContactPointsRtoB::test11),
	   TEST(TestFindContactPointsRtoB::test12)
   ) {}

   void test1()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(-2);
	   r2.setType(sad::p2d::BT_LEFT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test2()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(-1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_RIGHT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }

   void test3()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, -1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_UP);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test4()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, 1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(0);
	   r2.setType(sad::p2d::BT_DOWN);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   EXPECT_THROW(m.invoke(s1, v1, s2, v2), sad::p2d::CannotDetermineContactPoints);
   }


   void test5()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, 1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(-2);
	   r2.setType(sad::p2d::BT_LEFT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.size() == 0);
   }


   void test6()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, 1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_RIGHT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.size() == 0);
   }

   void test7()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_UP);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.size() == 0);
   }


   void test8()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(0);
	   r2.setType(sad::p2d::BT_DOWN);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE(pairs.size() == 0);
   }

   void test9()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(-1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(-2);
	   r2.setType(sad::p2d::BT_LEFT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 1, -2, 1) );
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 2, -2, 2) );
   }

   void test10()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(1, 0);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_RIGHT);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 1, 3, 1) );
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 2, 3, 2) );
   }

   void test11()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, 1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(3);
	   r2.setType(sad::p2d::BT_UP);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 2, 1, 3) );
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 2, 2, 3) );
   }

   void test12()
   {
	   sad::p2d::Rectangle r1;
	   sad::p2d::Bound r2;
	   sad::p2d::Vector v1(0, -1);
	   sad::p2d::Vector v2(0, 0);
	   r1.setRect(
			sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(1, 2),
				   sad::Point2D(2, 2), sad::Point2D(2, 1))	  
	   );
	   r2.setPosition(-3);
	   r2.setType(sad::p2d::BT_DOWN);
	   sad::p2d::FindContactPoints m;
	   sad::p2d::CollisionShape * s1 = &r1;
	   sad::p2d::CollisionShape * s2 = &r2;
	   sad::p2d::SetOfPointsPair pairs = m.invoke(s1, v1, s2, v2);
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 1, 1, 1, -3) );
	   ASSERT_TRUE( sad::p2d::hasPair(pairs, 2, 1, 2, -3) );
   }

}  test_find_contact_points_rtob; 