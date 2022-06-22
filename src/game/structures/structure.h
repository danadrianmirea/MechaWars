#pragma once

#include "raylib.h"

#include "structure.fwd.h"
#include "../../assets_manager.h"
#include "../../entity.h"
#include "../../map.h"

class Structure : public Entity
{
protected:
	Texture texture;
	Color color = WHITE;

	void _update_dest_rect() override;

	Map* map;
public:
	Structure( Map* _map ) : Structure( 0, 0, 1, 1, _map ) {};
	Structure( int x, int y, Map* _map ) : Structure( x, y, 1, 1, _map ) {};
	Structure( int x, int y, int w, int h, Map* _map );

	~Structure() override;

	void render() override;

	/*void set_pos( const int x, const int y ) override;
	void set_size( const int w, const int h ) override;*/

	Color get_color() { return color; }

	void reserve_pos();
	void unreserve_pos();

	virtual void on_selected() {};
	virtual void on_unselected() {};
};

