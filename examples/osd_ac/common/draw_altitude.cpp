#include <cstdint>
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/osd/features_api.hpp>
//#include <quan/uav/osd/home_position_is_set.hpp>
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;
using quan::length;

// todo add a home_position_is_set function

void draw_altitude()
{
   pxp_type pos;

   length::m alt;

   int alt_m;
   size_type vect;
   bitmap_ptr bitmap ;
   font_ptr font ;
   char buf[30];

   if ( osd_show_altitude() == true)
   {
	   position_type const aircraft_position = get_aircraft_position();
	   position_type const home_position = get_home_position();

	   alt = aircraft_position.alt - home_position.alt;
	   pos = get_osd_altitude_position();

	   vect= {0,0};

	   bitmap = get_bitmap(BitmapID::altitude_symbol);
	   if (bitmap) {
		   vect = get_size(bitmap) / 2;
		   draw_bitmap(bitmap,pos);
	   }
	   alt_m = static_cast<int> (alt.numeric_value() + 0.5f);

	   font = get_font(FontID::OSD_Charset);
	   if ( font){
		  sprintf( buf,"%4d%c",alt_m,141);
		  draw_text(buf,pos + pxp_type{vect.x,0},font);
	   }
   }



	if ( osd_show_baro_altitude() == true)
	{
	   alt = get_baro_alt();

	   pos = get_osd_baro_altitude_position();

	   vect= {0,0};

	   bitmap = get_bitmap(BitmapID::altitude_symbol);
	   if (bitmap) {
		   vect = get_size(bitmap) / 2;
		   draw_bitmap(bitmap,pos);
	   }

	   alt_m = static_cast<int> (alt.numeric_value() + 0.5f);

	   font = get_font(FontID::OSD_Charset);
	   if ( font){
		  sprintf( buf,"%4d%c",alt_m,141);
		  draw_text(buf,pos + pxp_type{vect.x,0},font);
	   }
	}

	if ( osd_show_altitude_bar() == true) {
		pos = get_osd_altitude_bar_position();

		alt = get_baro_alt();
		float baro_alt = alt.numeric_value();

		float scale = osd_get_altitude_bar_scale();
		if(scale <= 0.0) scale = 1.0;

		if(baro_alt <= 0.0f)
		   draw_box(pos, pos + pxp_type{BARO_ALT_BAR_WIDTH, BARO_ALT_BAR_HEIGHT},colour_type::white, false);
		else if(baro_alt <= scale) {
	       draw_box(pos, pos + pxp_type{BARO_ALT_BAR_WIDTH, BARO_ALT_BAR_HEIGHT},colour_type::white, false);

		   int32_t bar_height = (int32_t)(baro_alt / scale * BARO_ALT_BAR_HEIGHT);
		   draw_box(pos, pos + pxp_type{BARO_ALT_BAR_WIDTH, bar_height},colour_type::white, true);
		}
	 }

}

