#ifndef COLLISION_RECTANGLE_H_
#define COLLISION_RECTANGLE_H_

#include "rectangle.h"

class CollisionRectangle {
	public:
		virtual ~CollisionRectangle() = 0;

		virtual Rectangle boundingBox() const = 0;

		virtual Rectangle topCollision(units::Game x, units::Game y, units::Game delta) const = 0;

		virtual Rectangle bottomCollision(units::Game x, units::Game y, units::Game delta) const = 0;

		virtual Rectangle leftCollision(units::Game x, units::Game y, units::Game delta) const = 0;

		virtual Rectangle rightCollision(units::Game x, units::Game y, units::Game delta) const = 0;

		Rectangle collision(sides::SideType side, units::Game x, units::Game y, units::Game delta) const {
			if (side == sides::LEFT_SIDE)
				return leftCollision(x, y, delta);
			if (side == sides::RIGHT_SIDE)
				return rightCollision(x, y, delta);
			if (side == sides::TOP_SIDE)
				return topCollision(x, y, delta);
			return bottomCollision(x, y, delta);
		}
};

inline CollisionRectangle::~CollisionRectangle() {}

#endif // COLLISION_RECTANGLE_H_
