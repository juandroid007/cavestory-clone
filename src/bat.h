#ifndef BAT_H_
#define BAT_H_

#include "units.h"
#include "rectangle.h"

#include <boost/shared_ptr.hpp>
#include <map>

struct Graphics;
struct Sprite;

class Bat {
	private:
		enum Facing {
			FIRST_FACING,
			LEFT = FIRST_FACING,
			RIGHT,
			LAST_FACING
		};

		struct SpriteState {
			SpriteState(Facing facing) : facing(facing) {}
			Facing facing;
		};

		friend bool operator < (const SpriteState& a, const SpriteState& b) {
			return a.facing < b.facing;
		}

		void initializeSprites(Graphics& graphics);
		void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
		SpriteState getSpriteState() const;

		const units::Game center_y_;

		units::Game x_, y_;

		units::Degrees flight_angle_;

		Facing facing_;

		std::map<SpriteState, boost::shared_ptr<Sprite> > sprites_;
	public:
		Bat(Graphics& graphics, units::Game x, units::Game y);

		void update(units::MS elapsed_time_ms, units::Game player_x);
		void draw(Graphics& graphics) const;

		Rectangle damageRectangle() const {
			return Rectangle(
					x_, y_ + units::kHalfTile,
					32, 0
					);
		}
};

#endif // BAT_H_