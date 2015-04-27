
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_heading()
{
   

char buf [30];
pxp_type pos;
float heading360;

font_ptr font = get_font(FontID::OSD_Charset);


	if ( osd_show_heading() == true)
	{
	   pos = get_osd_heading_position();

	   if ( font)
	   {
	       angle_type const heading = get_aircraft_heading();

	       heading360 = (float)heading.numeric_value();
	       if(heading360 < 0) heading360 = heading360 + 360.0;

	       sprintf(buf,"%3.0f%c",heading360,176);
		   draw_text(buf,pos,font);
	   }
	}

}
