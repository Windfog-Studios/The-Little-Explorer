#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

#define COLLIDER_OFFSET 320

struct Collider;

// ----------------------------------------------------
struct MapLayer {
	p2SString name = "No name";
	uint width = 0u;
	uint height = 0u;
	uint* tile_gid = nullptr;
	inline uint Get(int x, int y) const { if (tile_gid >0) return x + y * width; };
	~MapLayer() { RELEASE(tile_gid); }
};
// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetRect(int tile_id);
	p2SString			name = "No name";
	int					firstgid =0 ;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;
};

struct ObjectGroup {
	p2SString name = "No name";
	SDL_Rect* object;
	Collider** collider;
	uint size = 0u;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ObjectGroup*> objectgroups;
	int					player_initial_x = 0;
	int					player_initial_y = 0;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	int GetXPosition(int x) { return x * data.tile_width; }
	int GetYPosition(int y) { return y * data.tile_height;}

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	void Reset_Level();

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectgroup);

public:

	MapData data;
	int map_offset = 10;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	int level_1_player_x;
	int level_1_player_y;
	int level_2_player_x;
	int level_2_player_y;

};

#endif // __j1MAP_H__