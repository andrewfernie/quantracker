
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "symbology.hpp"
#include "osd.hpp"
#include "angle_limits.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_nav()
{
   

char buf [30];
pxp_type pos;

font_ptr font = get_font(FontID::OSD_Charset);


	if ( osd_show_nav_wp() == true)
	{
	   pos = get_osd_nav_wp_position();

	   if (font)
	   {
		 size_type font_size = get_size(font);

         uint8_t const mode = get_custom_mode();

         if(mode == 3)          // mode=3=Auto
         {
			 if(get_num_wp_loaded() > 0)
			 {
				sprintf(buf,"%2d/%02d",get_current_wp_index(),get_num_wp_loaded());
				draw_text(buf,pos,font);
			 }
			 else
			 {
				 sprintf(buf,"wp:%2d",get_current_wp_index());
				 draw_text(buf,pos,font);
			 }

			 double dist = get_current_wp_dist().numeric_value();

			 angle_type const wp_bearing = get_current_wp_bearing();

		     angle_type const heading = get_aircraft_heading();

             pos.y = pos.y - (font_size.y);

		     bitmap_ptr wp_arrow = get_bitmap(BitmapID::wp_arrow);
		     if (wp_arrow) {
		        size_type const vect = get_size(wp_arrow) / 2;
		        draw_bitmap(wp_arrow, pos, vect, heading - wp_bearing);
		     }

             pos.x = pos.x + (2*font_size.x);

	         sprintf(buf,"%4.0fm",dist);
	         draw_text(buf,pos,font);

         }
	   }
	}
}
