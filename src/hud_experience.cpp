#include "hud_experience.h"
#include "number_spr.h"

namespace {
	const units::Game kDrawY = 3 * units::kHalfTile;

	const units::Game kLevelNumberDrawX = 3 * units::kHalfTile;

	const units::Tile kLevelDrawX = 1;

	const units::Tile kLevelSourceX = 5;
	const units::Game kLevelSourceY = 160;

	const units::Tile kLevelSourceWidth = 1;
	const units::Game kLevelSourceHeight = units::kHalfTile;

	const units::Game kExperienceBarDrawX = 5 * units::kHalfTile;

	const units::Game kExperienceBarSourceX = 0;
	const units::Game kExperienceBarSourceY = 9 * units::kHalfTile;

	const units::Game kExperienceBarSourceWidth = 5 * units::kHalfTile;
	const units::Game kExperienceBarSourceHeight = units::kHalfTile;

	const units::Game kFlashSourceX = 5 * units::kHalfTile;
	const units::Game kFlashSourceY = 5;

	const units::Tile kFillSourceX = 0;
	const units::Tile kFillSourceY = 5;

	const units::Tile kMaxSourceX = 5 * units::kHalfTile;
	const units::Tile kMaxSourceY = 9 * units::kHalfTile;

	const units::MS kFlashTime = 800;
	const units::MS kFlashPeriod = 40;

	const std::string kSpriteName("TextBox");
}

ExperienceHUD::ExperienceHUD(Graphics& graphics) :
	experience_bar_sprite_(graphics, kSpriteName,
			units::gameToPixel(kExperienceBarSourceX),
			units::gameToPixel(kExperienceBarSourceY),
			units::gameToPixel(kExperienceBarSourceWidth),
			units::gameToPixel(kExperienceBarSourceHeight)),
	level_sprite_(graphics, kSpriteName,
			units::tileToPixel(kLevelSourceX),
			units::gameToPixel(kLevelSourceY),
			units::tileToPixel(kLevelSourceWidth),
			units::gameToPixel(kLevelSourceHeight)),
	flash_sprite_(graphics, kSpriteName,
			units::gameToPixel(kFlashSourceX),
			units::tileToPixel(kFlashSourceY),
			units::gameToPixel(kExperienceBarSourceWidth),
			units::gameToPixel(kExperienceBarSourceHeight)),
	max_sprite_(graphics, kSpriteName,
			units::gameToPixel(kMaxSourceX),
			units::gameToPixel(kMaxSourceY),
			units::gameToPixel(kExperienceBarSourceWidth),
			units::gameToPixel(kExperienceBarSourceHeight)),
	fill_sprite_(graphics, kSpriteName,
			units::tileToPixel(kFillSourceX),
			units::tileToPixel(kFillSourceY),
			units::gameToPixel(kExperienceBarSourceWidth),
			0,
			units::gameToPixel(kExperienceBarSourceHeight)),
	flash_timer_(kFlashTime)
{}

void ExperienceHUD::draw(Graphics& graphics,
		units::GunLevel gun_level,
		units::GunExperience current_experience,
		units::GunExperience level_experience) {
	level_sprite_.draw(graphics, units::tileToGame(kLevelDrawX), kDrawY, NULL);
	NumberSpr::HUDNumber(graphics, gun_level, 2).draw(graphics, NULL, kLevelNumberDrawX, kDrawY);

	experience_bar_sprite_.draw(graphics, kExperienceBarDrawX, kDrawY, NULL);

	if (current_experience < level_experience) {
		fill_sprite_.set_percentage_width(current_experience * 1.0f / level_experience);
		fill_sprite_.draw(graphics, kExperienceBarDrawX, kDrawY, NULL);
	} else {
		max_sprite_.draw(graphics, kExperienceBarDrawX, kDrawY, NULL);
	}

	if (flash_timer_.active() && flash_timer_.current_time() / kFlashPeriod % 2 == 0)
		flash_sprite_.draw(graphics, kExperienceBarDrawX, kDrawY, NULL);
}
