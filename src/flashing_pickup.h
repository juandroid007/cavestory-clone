#ifndef FLASHING_PICKUP_H_
#define FLASHING_PICKUP_H_

#include "pickup.h"
#include "sprite.h"
#include "timer.h"

#include <memory>

class FlashingPickup : public Pickup {
	private:
		FlashingPickup(
				Graphics& graphics,
				units::Game center_x, units::Game center_y,
				units::Tile source_x, units::Tile source_y, 
				const Rectangle& rectangle,
				const int value,
				const PickupType type);

		Sprite sprite_, flash_sprite_, dissipating_sprite_;

		units::Game x_, y_;

		Timer timer_;

		units::MS flash_period_;

		const Rectangle& rectangle_;

		const int value_;

		const PickupType type_;
	public:
		Rectangle collisionRectangle() const;

		void draw(Graphics& graphics);
		bool update(units::MS elapsed_time, const Map& map);

		int value() const { return value_; }
		PickupType type() const { return type_; }

		static std::shared_ptr<Pickup> heartPickup(
				Graphics& graphics,
				units::Game center_x, units::Game center_y);

		static std::shared_ptr<Pickup> multiHeartPickup(
				Graphics& graphics,
				units::Game center_x, units::Game center_y);
};
#endif // FLASHING_PICKUP_H_
