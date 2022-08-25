#pragma once

#include "raylib.h"

#include "world_entity.fwd.h"
#include "map.h"
#include "teams.h"
#include "ui/ui_button.hpp"
#include "ui/ui_tile_cursor.h"

struct DamageInfo
{
	int damage = 0;
	std::weak_ptr<WorldEntity> attacker;

	bool is_explosion = false;
};

class WorldEntity : public Entity
{
protected:
	void _update_dest_rect() override;

	Texture texture;
	Color color = WHITE;

	TEAM team_id;
	Rectangle team_quad;

	int max_health = 100;
	int health = 100;

	Shader hit_shader;
	float hit_time = .25f;
	bool is_currently_hit = false;

	std::weak_ptr<Map> map;
	std::weak_ptr<UITileCursor> selecting_cursor; //  tile cursor which is currently selecting the world entity
	std::vector<std::weak_ptr<UIButton>> buttons;

	const int MARGIN = 3;
public:
	WorldEntity( std::weak_ptr<Map> map ) : WorldEntity( 0, 0, 1, 1, map ) {};
	WorldEntity( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, 1, 1, map ) {};
	WorldEntity( int x, int y, int w, int h, std::weak_ptr<Map> map );

	void init() override;
	void render() override;
	void render_hud() override;

	void safe_destroy() override;

	Color get_color() { return color; }

	void reserve_pos();
	void unreserve_pos();

	void take_damage( DamageInfo info );
	virtual void on_take_damage( DamageInfo info ) {};

	void set_selecting_cursor( std::weak_ptr<UITileCursor> cursor ) { selecting_cursor = cursor; }
	void unset_selecting_cursor() { selecting_cursor.reset(); }
	std::weak_ptr<UITileCursor> get_selecting_cursor() { return selecting_cursor; }
	bool is_selected() { return !selecting_cursor.expired(); }

	void select( std::weak_ptr<UITileCursor> cursor );
	void unselect();

	virtual void on_selected() {};
	virtual void on_unselected() {};
	virtual void on_right_click_selected() {};

	void set_team( TEAM id )
	{
		team_id = id;
		color = get_team_color( team_id );
	}
	TEAM get_team() { return team_id; }

	template <typename... Args>
	std::shared_ptr<UIButton> create_button( Args... args )
	{
		auto button = GameManager::create<UIButton>( args... );

		//button->set_size( button_size, button_size );
		button->set_color( color );

		buttons.push_back( button );
		return button;
	}
	void clear_buttons();
	void perform_layout();

	std::weak_ptr<Map> get_map() { return map; }
};

