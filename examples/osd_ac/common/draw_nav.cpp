
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

			 float bearing = get_current_wp_bearing().numeric_value();
			 float bearing360 = limit_360(bearing);

		     angle_type const heading = get_aircraft_heading();
		     float heading360 = limit_360((float)heading.numeric_value());

		     bearing360 = limit_plus_minus_180(bearing360 - heading360);

			 pos.x = pos.x - 4*(font_size.x);
			 pos.y = pos.y - (font_size.y);

			 sprintf(buf,"%4.0fm/%4.0f%c",dist,(double)bearing360,176);
			 draw_text(buf,pos,font);
         }
	   }
	}
}
