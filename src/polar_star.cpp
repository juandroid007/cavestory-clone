#include "polar_star.h"
#include "sprite.h"

namespace {
	const int kPolarStarIndex = 2; // 0-based indexing
	const units::Game kGunWidth = 3 * units::kHalfTile;
	const units::Game kGunHeight = 2 * units::kHalfTile;

	const units::Tile kHorizontalOffset = 0;
	const units::Tile kUpOffset = 2;
	const units::Tile kDownOffset = 4;

	const units::Tile kLeftOffset = 0;
	const units::Tile kRightOffset = 1;

	const std::string kSpritePath("Arms");

	// Nozzle Offsets
	const units::Game kNozzleHorizontalY = 23;
	const units::Game kNozzleHorizontalLeftX = 10;
	const units::Game kNozzleHorizontalRightX = 38;
	const units::Game kNozzleUpY = 4;
	const units::Game kNozzleUpLeftX = 27;
	const units::Game kNozzleUpRightX = 21;
	const units::Game kNozzleDownY = 28;
	const units::Game kNozzleDownLeftX = 29;
	const units::Game kNozzleDownRightX = 19;
	// Projectile Sprite
	const units::Tile kProjectileSourceY = 2;
	const units::Tile kHorizontalProjectileSourceX = 8;
	const units::Tile kVerticalProjectileSourceX = 9;

	// Projectile Velocity
	const units::Velocity kProjectileSpeed = 0.6f;
	const units::Game kProjectileMaxOffset = 7 * units::kHalfTile;
}

PolarStar::PolarStar(Graphics& graphics) {
	initializeSprites(graphics);
}

void PolarStar::updateProjectiles(units::MS elapsed_time_ms) {
	if (projectile_a_) {
		if (!projectile_a_->update(elapsed_time_ms)) {
			projectile_a_.reset();
		}
	}
	if (projectile_b_) {
		if (!projectile_b_->update(elapsed_time_ms)) {
			projectile_b_.reset();
		}
	}
}

void PolarStar::draw(
		Graphics& graphics,
		HorizontalFacing horizontal_facing, VerticalFacing vertical_facing,
		bool gun_up,
		units::Game player_x, units::Game player_y) {
	units::Game x = gun_x(horizontal_facing, player_x);
	units::Game y = gun_y(vertical_facing, gun_up, player_y);

	sprite_map_[std::make_tuple(horizontal_facing, vertical_facing)]->draw(graphics, x, y);

	if (projectile_a_)
		projectile_a_->draw(graphics);
	if (projectile_b_)
		projectile_b_->draw(graphics);
}

units::Game PolarStar::gun_y(VerticalFacing vertical_facing, bool gun_up, units::Game player_y) const {
	units::Game gun_y = player_y;

	if(vertical_facing == UP)
		gun_y -= units::kHalfTile / 2;

	if(vertical_facing == DOWN)
		gun_y += units::kHalfTile / 2;
	if(gun_up)
		gun_y -= 2.0f;

	return gun_y;

}

void PolarStar::startFire(units::Game player_x, units::Game player_y,
		HorizontalFacing horizontal_facing,
		VerticalFacing vertical_facing,
		bool gun_up) {
	if (projectile_a_ && projectile_b_) return;

	units::Game bullet_x = gun_x(horizontal_facing, player_x) - units::kHalfTile;
	units::Game bullet_y = gun_y(vertical_facing, gun_up, player_y) - units::kHalfTile;

	switch (vertical_facing) {
		case HORIZONTAL:
			bullet_y += kNozzleHorizontalY;
			if (horizontal_facing == LEFT) {
				bullet_x += kNozzleHorizontalLeftX;
			} else {
				bullet_x += kNozzleHorizontalRightX;
			}
			break;
		case UP:
			bullet_y += kNozzleUpY;
			if (horizontal_facing == LEFT) {
				bullet_x += kNozzleUpLeftX;
			} else {
				bullet_x += kNozzleUpRightX;
			}
			break;
		case DOWN:
			bullet_y += kNozzleDownY;
			if (horizontal_facing == LEFT) {
				bullet_x += kNozzleDownLeftX;
			} else {
				bullet_x += kNozzleDownRightX;
			}
			break;
		default: break;
	}

	// Create our projectile using bullet_x, bullet_y
	if (!projectile_a_) {
		projectile_a_.reset(new Projectile(
					vertical_facing == HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
					horizontal_facing, vertical_facing,
					bullet_x, bullet_y));
	} else if (!projectile_b_) {
		projectile_b_.reset(new Projectile(
					vertical_facing == HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
					horizontal_facing, vertical_facing,
					bullet_x, bullet_y));
	}
}

void PolarStar::initializeSprites(Graphics& graphics) {
	horizontal_projectile_.reset(new Sprite(
				graphics, "Bullet",
				units::tileToPixel(kHorizontalProjectileSourceX), units::tileToPixel(kProjectileSourceY),
				units::tileToPixel(1), units::tileToPixel(1)));
	vertical_projectile_.reset(new Sprite(
				graphics, "Bullet",
				units::tileToPixel(kVerticalProjectileSourceX), units::tileToPixel(kProjectileSourceY),
				units::tileToPixel(1), units::tileToPixel(1)));

	ENUM_FOREACH(h_facing, HORIZONTAL_FACING) {
		ENUM_FOREACH(v_facing, VERTICAL_FACING) {
			initializeSprite(graphics, std::make_tuple((HorizontalFacing)h_facing,
						(VerticalFacing)v_facing));
		}
	}
}

void PolarStar::initializeSprite(Graphics& graphics, const SpriteState& sprite_state) {
	units::Tile tile_y = sprite_state.horizontal_facing(sprite_state) == LEFT ? kLeftOffset : kRightOffset;
	switch(sprite_state.vertical_facing(sprite_state)) {
		case HORIZONTAL:
			tile_y += kHorizontalOffset;
			break;
		case UP:
			tile_y += kUpOffset;
			break;
		case DOWN:
			tile_y += kDownOffset;
			break;
		case LAST_VERTICAL_FACING: break;
	}

	sprite_map_[sprite_state] = std::shared_ptr<Sprite>(new Sprite(graphics, kSpritePath,
				units::gameToPixel(kPolarStarIndex * kGunWidth), units::tileToPixel(tile_y),
				units::gameToPixel(kGunWidth), units::gameToPixel(kGunHeight)));
}

PolarStar::Projectile::Projectile(std::shared_ptr<Sprite> sprite,
		HorizontalFacing horizontal_direction,
		VerticalFacing vertical_direction,
		units::Game x, units::Game y) :
	sprite_(sprite),
	horizontal_direction_(horizontal_direction),
	vertical_direction_(vertical_direction),
	x_(x), y_(y),
	offset_(0)
{}

bool PolarStar::Projectile::update(units::MS elapsed_time_ms) {
	offset_ += kProjectileSpeed * elapsed_time_ms;

	return offset_ < kProjectileMaxOffset;
}

void PolarStar::Projectile::draw(Graphics& graphics) {
	units::Game x = x_;
	units::Game y = y_;

	switch (vertical_direction_) {
		case HORIZONTAL:
			x += horizontal_direction_ == LEFT ? -offset_ : offset_;
			break;
		case UP:
			y -= offset_;
			break;
		case DOWN:
			y += offset_;
			break;
		default: break;
	}

	sprite_->draw(graphics, x, y);
} 
