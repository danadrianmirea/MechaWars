#include "ui_tile_cursor.h"

#include "../world_entity.h"

UITileCursor::UITileCursor( Map* _map ) : UIBase(), applied_pos( { 0, 0 } )
{
	map = _map;

	texture = AssetsManager::get_or_load_texture( "assets/textures/ui/tile_cursor.png" );
	quad = Rectangle { 0, 0, (float) texture.width, (float) texture.height };
}

bool UITileCursor::unhandled_mouse_click( int mouse_button, bool is_pressed )
{
	//  selection
	if ( mouse_button == MOUSE_BUTTON_LEFT && is_pressed )
	{
		if ( hovered_structure )
		{
			if ( !( hovered_structure == selected_structure ) )
			{
				if ( selected_structure )
					selected_structure->on_unselected();

				selected_structure = hovered_structure;
				selected_structure->on_selected();
				//printf( "%d selected\n", hovered_structure->get_id() );

				should_update_pos = true;
			}
		}
		else if ( selected_structure )
		{
			selected_structure->on_unselected();
			selected_structure = nullptr;
			//printf( "unselected structure!\n" );

			should_update_pos = true;
		}
	}
	
	//printf( "UITileCursor: %d, %d\n", mouse_button, is_pressed );
	return false;
}

void UITileCursor::update( float dt )
{
	//  update mouse pos
	Int2 tile_mouse_pos = GameCamera::get_current()->get_tile_mouse_pos();
	if ( should_update_pos || !( tile_mouse_pos == applied_pos ) )
	{
		//printf( "update (%d;%d)\n", tile_mouse_pos.x, tile_mouse_pos.y );
		applied_pos = tile_mouse_pos;

		if ( map->has_structure_at( applied_pos.x, applied_pos.y ) )
		{
			WorldEntity* structure = map->get_structure_at_pos( applied_pos.x, applied_pos.y );
			hovered_structure = structure;

			//  apply structure info
			if ( should_update_pos || !selected_structure )
			{
				pos = structure->get_pos();
				size = structure->get_size();
				color = structure->get_color();
			}
		
			//printf( "%d\n", structure->get_id() );
		}
		else
		{
			if ( should_update_pos || !selected_structure )
			{
				pos = applied_pos;

				size.x = 1;
				size.y = 1;
				color = WHITE;
			}
			hovered_structure = nullptr;
		}
	}

	should_update_pos = false;
}

const int offset = 1;
void UITileCursor::render()
{
	//  top-left
	DrawTextureEx( texture, Vector2 { (float) pos.x * Map::TILE_SIZE - offset, (float) pos.y * Map::TILE_SIZE - offset }, 0.0f, 1.0f, color );
	//  top-right
	DrawTextureEx( texture, Vector2 { (float) ( pos.x + size.x ) * Map::TILE_SIZE + offset, (float) pos.y * Map::TILE_SIZE - offset }, 90.0f, 1.0f, color );
	//  bottom-left
	DrawTextureEx( texture, Vector2 { (float) pos.x * Map::TILE_SIZE - offset, (float) ( pos.y + size.y ) * Map::TILE_SIZE + offset }, 270.0f, 1.0f, color );
	//  bottom-right
	DrawTextureEx( texture, Vector2 { (float) ( pos.x + size.x ) * Map::TILE_SIZE + offset, (float) ( pos.y + size.y ) * Map::TILE_SIZE + offset }, 180.0f, 1.0f, color );
}