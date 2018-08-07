#include "map.h"
#include "sprite.h"
#include "graphics.h"
#include "game.h"
#include "rectangle.h"

using boost::shared_ptr;
using std::vector;

void Map::update(units::MS elapsed_time_ms) {
	for(size_t row = 0; row < tiles_.size(); row++) {
		for(size_t col = 0; col < tiles_[row].size(); col++) {
			if(tiles_[row][col].sprite) {
				tiles_[row][col].sprite->update(elapsed_time_ms);
			}
		}
	}
}

void Map::drawBackground(Graphics& graphics) const {
	backdrop_->draw(graphics);
	for(size_t row = 0; row < bkg_tiles_.size(); row++) {
		for(size_t col = 0; col < bkg_tiles_[row].size(); col++) {
			if(bkg_tiles_[row][col]) {
				bkg_tiles_[row][col]->draw(
						graphics,
						units::tileToGame(col), units::tileToGame(row));
			}
		}
	}
}

void Map::draw(Graphics& graphics) const {
	for(size_t row = 0; row < tiles_.size(); row++) {
		for(size_t col = 0; col < tiles_[row].size(); col++) {
			if(tiles_[row][col].sprite) {
				tiles_[row][col].sprite->draw(
						graphics,
						units::tileToGame(col), units::tileToGame(row));
			}
		}
	}
}

vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
	const units::Tile first_row = units::gameToTile(rectangle.top());
	const units::Tile last_row = units::gameToTile(rectangle.bottom());
	const units::Tile first_col = units::gameToTile(rectangle.left());
	const units::Tile last_col = units::gameToTile(rectangle.right());

	vector<CollisionTile> collision_tiles;

	for(units::Tile row = first_row; row <= last_row; row++) {
		for(units::Tile col = first_col; col <= last_col; col++) {
			collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
		}
	}

	return collision_tiles;
}

Map* Map::createTestMap(Graphics& graphics) {
	Map* map = new Map();

	map->backdrop_.reset(new FixedBackdrop("assets/bkBlue.bmp", graphics));

	const units::Tile num_rows = 15; // 15 * 32 = 480
	const units::Tile num_cols = 20; // 20 * 32 = 640

	// tiles_ && bkg_tiles_ are num_rows x num_cols in size
	map->tiles_ = vector<vector<Tile>>(
			num_rows, vector<Tile>(
				num_cols, Tile()
				)
			);
	map->bkg_tiles_ = vector<vector<shared_ptr<Sprite>>>(
			num_rows, vector<shared_ptr<Sprite>>(
				num_cols, shared_ptr<Sprite>()
				));

	shared_ptr<Sprite> sprite(new Sprite(
				graphics,
				"assets/PrtCave.bmp",
				units::tileToPixel(1), 0,
				units::tileToPixel(1), units::tileToPixel(1)
				));

	Tile tile(WALL_TILE, sprite);

	for(units::Tile col = 0; col < num_cols; col++) {
		for(units::Tile row = 11; row < num_rows; row++) {
			map->tiles_[row][col] = tile;
		}
	}

	map->tiles_[10][5] = tile;
	map->tiles_[10][7] = tile;
	map->tiles_[9][6] = tile;
	map->tiles_[10][6] = tile;
	map->tiles_[7][12] = tile;

	// Chains stuff
	shared_ptr<Sprite> chain_top(new Sprite(
				graphics,
				"assets/PrtCave.bmp",
				units::tileToPixel(11), units::tileToPixel(2),
				units::tileToPixel(1), units::tileToPixel(1)
				));
	shared_ptr<Sprite> chain_body(new Sprite(
				graphics,
				"assets/PrtCave.bmp",
				units::tileToPixel(12), units::tileToPixel(2),
				units::tileToPixel(1), units::tileToPixel(1)
				));
	shared_ptr<Sprite> chain_bottom(new Sprite(
				graphics,
				"assets/PrtCave.bmp",
				units::tileToPixel(13), units::tileToPixel(2),
				units::tileToPixel(1), units::tileToPixel(1)
				));

	map->bkg_tiles_[8][12] = chain_top;
	map->bkg_tiles_[9][12] = chain_body;
	map->bkg_tiles_[10][12] = chain_bottom;

	return map;
}
