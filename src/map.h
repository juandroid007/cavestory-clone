#ifndef MAP_H_
#define MAP_H_

#include "backdrop.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

struct Graphics;
struct Sprite;
struct Rectangle;

class Map {
	public:
		enum TileType {
			AIR_TILE,
			WALL_TILE
		};

		class CollisionTile {
			public:
				CollisionTile(int row, int col, TileType tile_type) :
					row(row), col(col), tile_type(tile_type) {}

				int row, col;

				TileType tile_type;

		};

		static Map* createTestMap(Graphics& graphics);

		std::vector<CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;

		void update(int elapsed_time_ms);
		void drawBackground(Graphics& graphics) const;
		void draw(Graphics& graphics) const;
	private:
		class Tile {
			public:
				Tile(TileType tile_type = AIR_TILE,
						boost::shared_ptr<Sprite> sprite = boost::shared_ptr<Sprite>()) :
					tile_type(tile_type),
					sprite(sprite) {}

				TileType tile_type;
				boost::shared_ptr<Sprite> sprite;
		};

		boost::scoped_ptr<Backdrop> backdrop_;

		std::vector<std::vector<Tile>> tiles_;

};

#endif // MAP_H_
