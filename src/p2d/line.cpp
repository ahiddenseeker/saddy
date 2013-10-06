#include "p2d/line.h"
#include "primitives/matrix2x2.h"
#include "log/log.h"
#include <algorithm>

DECLARE_SOBJ_INHERITANCE_WITH_INDEX(sad::p2d::Line, sad::p2d::CollisionShape, 2);


sad::p2d::CollisionShape * sad::p2d::Line::clone(int count) const
{
	sad::p2d::Line * b = new sad::p2d::Line[count]();
	std::fill_n(b, count, *this);
	return b;
}


sad::p2d::Point sad::p2d::Line::center() const
{
	sad::p2d::Point r = m_c.p1();
	r += m_c.p2();
	r /= 2.0;
	return r;
}

void sad::p2d::Line::rotate(double angle)
{
	sad::p2d::Point center = this->center();	
	sad::p2d::Matrix2x2 m = sad::p2d::Matrix2x2::counterclockwise(angle);
	sad::p2d::Point x1 = (this->m_c.p1() - center )* m + center;
	sad::p2d::Point x2 = (this->m_c.p2() - center )* m + center;
	setCutter(sad::p2d::Cutter2D(x1 , x2) );
}

void sad::p2d::Line::move(const sad::p2d::Vector & d)
{
	m_c._1() += d;
	m_c._2() += d;
}

sad::p2d::ConvexHull sad::p2d::Line::toHull() const
{
	sad::Vector<sad::p2d::Point> set;
	set << m_c.p1();
	set << m_c.p2();

	return sad::p2d::ConvexHull(set);
}

sad::p2d::Cutter1D sad::p2d::Line::project(const p2d::Axle & a) const
{
	double a2 = sad::p2d::scalar(a, a);
	double p1 = sad::p2d::scalar(m_c.p1(), a) / a2;
	double p2 = sad::p2d::scalar(m_c.p2(), a) / a2;
	if (p1 > p2)
		std::swap(p1, p2);
	return sad::p2d::Cutter1D(p1, p2);
}

sad::Vector<sad::p2d::Point> sad::p2d::Line::points() const
{
	sad::Vector<sad::p2d::Point> result;
	result << m_c.p1();
	result << m_c.p2();
	return result;
}



size_t sad::p2d::Line::sizeOfType() const
{
	return sizeof(p2d::Line);
}

void sad::p2d::Line::populatePoints(sad::Vector<p2d::Point> & v) const
{
	v << m_c.p1();
	v << m_c.p2();
}

void sad::p2d::Line::normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n)
{
	n = m_c.p2() - m_c.p1();
	sad::p2d::mutableUnit(n);
	sad::p2d::mutableNormalizedOrtho(n, p2d::OVI_DEG_90);
}



hst::string sad::p2d::Line::dump() const
{
	return str(fmt::Format("Line at ({0}, {1}) - ({2}, {3})")
							<< m_c.p1().x() << m_c.p1().y()
							<< m_c.p2().x() << m_c.p2().y()
			  );
}

