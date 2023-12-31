#pragma once
#include "Object.h"

#define PI 3.14

namespace Objects {

	class Circle : public Object {
	public:
		Circle(void);
		Circle(const Point2d& p, double r);

		virtual ~Circle(void) = default;

	public:
		virtual void draw(WDraw& wDraw) const override;
		virtual BoundyBox getBoundyBox() const override;
		virtual double length() const override;

		virtual void pack(Provider::DataProvider* dataprov) const override;
		virtual void unpack(Provider::DataProvider* dataprov) override;

	protected:
		Point2d m_p = Point2d(0, 0);
		double m_dr = 2;
	};

}