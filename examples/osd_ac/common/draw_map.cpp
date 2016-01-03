#include <cstdio>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/get_distance.hpp> 
#include "osd.hpp"
#include "symbology.hpp"

using namespace quan::uav::osd;

/*

 #define MAP_MARKER_SIZE 0.05

 #define	MAP_AREA_FRACTION	0.50			// Area within overall screen area used by the map. By using less than 100% we leave a few lines atthe top and bottom for text
 #define	ASPECT_RATIO		0.75			// X axis caling needed to make square pixels

 uint16_t horizon_last_xs, horizon_last_xe, horizon_last_ys, horizon_last_ye;
 uint8_t horizon_init = false;
 float	mapScale = 0.005;
 float	map_max_xy = 200.0;
 uint8_t	barActive = false;
 uint8_t	current_text_mode;
 uint8_t	oldNumWPLoaded;
 float	oldLat, oldLong;
 float   oldYaw;
 float   oldMapScale;
 uint8_t firstPassMap = true;

 */

void draw_map()
{

    if (osd_show_map() == true)
    {
        /*

        pxp_type pos = get_osd_map_position();

        float home_x, home_y;
        float tmp_x, tmp_y;
        float lat1, lat2, long1, long2, bearing, dist;
        float wp1_x, wp1_y, wp2_x, wp2_y;
        float map_range;
        char text_string[10];
        char text_string2[10];
        uint8_t needToRedrawMap = false;

        uint16_t x1i, y1i, xlj, ylj;
        uint8_t i;


        map_max_xy = 1.0;	// Initialize low so that we can capture the maximum

        // draw the home box

        if (osd_symbol_active (SYMBOLOGY_MODE_MAP))
        {
            // Decide if we need to redraw

            // First, if this is the first pass through the map page
            if (firstPassMap)
            {
                needToRedrawMap = true;
            }
            firstPassMap = false;

            // or, if we have more waypoints
            if (oldNumWPLoaded != numWPLoaded)
            {
                needToRedrawMap = true;
            }

            // or, if the aircraft position has moved more than the equivalent of two pixels on the screen
            lat1 = ToRad((float) global_gps_raw_int_data.lat / 1e7);
            long1 = ToRad((float) global_gps_raw_int_data.lon / 1e7);
            lat_lon_to_x_y(METERS_SELECT, lat1, long1, oldLat, oldLong, &tmp_x,
                    &tmp_y);
            osd_get_scaled_xy(tmp_x, tmp_y, &x1i, &y1i);
            osd_get_scaled_xy(0, 0, &xlj, &ylj);
            if ((abs(x1i - xlj) >= 2) || (abs(y1i - ylj) >= 2))
            {
                needToRedrawMap = true;
            }

            // or, if the map scale has changed
            if (oldMapScale != mapScale)
            {
                needToRedrawMap = true;
            }

            // or, if the aircraft yaw angle has changed by more than 5 degrees
            if (fabs(global_attitude_data.yaw - oldYaw) > (5 * DEG_TO_RAD))
            {
                needToRedrawMap = true;
            }

            if (needToRedrawMap)
            {
                osd_clear_map_area();
                oldLat = lat1;
                oldLong = long1;
                oldYaw = global_attitude_data.yaw;
                oldMapScale = mapScale;
                needToRedrawMap = false;

                // draw the center marker
                osd_drawline_scaled(-MAP_MARKER_SIZE, 0.0, MAP_MARKER_SIZE, 0.0,
                        DRAW_LINE_WHITE, DRAW_LINE_WIDTH_WIDE);
                osd_drawline_scaled(0.0, -MAP_MARKER_SIZE, 0.0, MAP_MARKER_SIZE,
                        DRAW_LINE_WHITE, DRAW_LINE_WIDTH_WIDE);

                osd_set_cursor_pos(SCREEN_SIZE_Y_CHAR - 1, 0, 11);

                map_range = 1.0 / mapScale;
                strcpy(text_string, "Scale ");
                dtostrf(map_range, 4, 0, text_string2);
                strcat(text_string, text_string2);
                strcat(text_string, "m");

                osd_draw_text(text_string, OSD_TEXT_CURRENT);

                if (DATA_AVAILABLE(MSG_GPS_RAW_FIX)
                        && DATA_AVAILABLE(MSG_HOMEWP) && homeIsSet)
                {
                    lat2 = ToRad(wpList[homeWPIndex].lat / 1e7);
                    long2 = ToRad(wpList[homeWPIndex].lng / 1e7);

                    // find the xy co-ordinates for the home marker relative to the current position
                    lat_lon_to_x_y(METERS_SELECT, lat1, long1, lat2, long2,
                            &tmp_x, &tmp_y);

                    // Keep track of maximum values to allow auto map scaling (in next pass through)
                    map_max_xy = max(map_max_xy, fabs(tmp_x));
                    map_max_xy = max(map_max_xy, fabs(tmp_y));

                    // rotate the home co-ordinates so that it is relative to the a/c heading, and scale down
                    rotate_x_y(global_attitude_data.yaw, tmp_x * mapScale,
                            tmp_y * mapScale, &home_x, &home_y);
                    home_x = constrain(home_x, -1.0, 1.0);
                    home_y = constrain(home_y, -1.0, 1.0);

                    // draw the home marker
                    osd_draw_box_scaled(home_x, home_y, MAP_MARKER_SIZE);

                    if (osd_symbol_active (VECTORHOME_ENABLE))
                    {
                        osd_drawline_scaled(0.0, 0.0, home_x, home_y,
                                DRAW_LINE_WHITE, DRAW_LINE_WIDTH_WIDE);
                    }

                    if (osd_symbol_active (ROUTE_ENABLE))
                    {
                        if (numWPLoaded > 1)
                        {
                            for (i = 0; i <= numWPLoaded; i++)
                            {
                                lat2 = ToRad(wpList[i].lat / 1e7);
                                long2 = ToRad(wpList[i].lng / 1e7);

                                // find the xy co-ordinates for the home marker relative to the current position
                                lat_lon_to_x_y(METERS_SELECT, lat1, long1, lat2,
                                        long2, &tmp_x, &tmp_y);

                                // Keep track of maximum values to allow auto map scaling (in next pass through)
                                map_max_xy = max(map_max_xy, fabs(tmp_x));
                                map_max_xy = max(map_max_xy, fabs(tmp_y));

                                // rotate the home co-ordinates so that it is relative to the a/c heading, and scale down
                                rotate_x_y(global_attitude_data.yaw,
                                        tmp_x * mapScale, tmp_y * mapScale,
                                        &wp2_x, &wp2_y);

                                wp2_x = constrain(wp2_x, -1.0, 1.0);
                                wp2_y = constrain(wp2_y, -1.0, 1.0);

                                if (i > 0)
                                {
                                    osd_draw_diamond_scaled(wp2_x, wp2_y,
                                            MAP_MARKER_SIZE);
                                    osd_drawline_scaled(wp1_x, wp1_y, wp2_x,
                                            wp2_y, DRAW_LINE_WHITE,
                                            DRAW_LINE_WIDTH_WIDE);
                                }

                                wp1_x = wp2_x;
                                wp1_y = wp2_y;
                            }
                            osd_drawline_scaled(wp1_x, wp1_y, home_x, home_y,
                                    DRAW_LINE_WHITE, DRAW_LINE_WIDTH_WIDE);
                        }

                    }

                    if (osd_symbol_active (RUNWAY_ENABLE))
                    {

                        // find the xy co-ordinates for the first runway end relative to the current position
                        lat_lon_to_x_y(METERS_SELECT, lat1, long1,
                                ToRad(RUNWAY_LAT_1), ToRad(RUNWAY_LON_1),
                                &tmp_x, &tmp_y);

                        // rotate the home co-ordinates so that it is relative to the a/c heading, and scale down
                        rotate_x_y(global_attitude_data.yaw, tmp_x, tmp_y,
                                &wp1_x, &wp1_y);

                        // find the xy co-ordinates for the second runway end relative to the current position
                        lat_lon_to_x_y(METERS_SELECT, lat1, long1,
                                ToRad(RUNWAY_LAT_2), ToRad(RUNWAY_LON_2),
                                &tmp_x, &tmp_y);

                        // rotate the home co-ordinates so that it is relative to the a/c heading, and scale down
                        rotate_x_y(global_attitude_data.yaw, tmp_x, tmp_y,
                                &wp2_x, &wp2_y);

                        // Only draw the runway if it is within two kilometers in X and Y
                        if ((fabs(wp1_x) < 2000.0) && (fabs(wp1_y) < 2000.0)
                                && (fabs(wp2_x) < 2000.0)
                                && (fabs(wp2_y) < 2000.0))
                        {
                            // Keep track of maximum values to allow auto map scaling (in next pass through)
                            map_max_xy = max(map_max_xy, fabs(wp1_x));
                            map_max_xy = max(map_max_xy, fabs(wp1_y));
                            map_max_xy = max(map_max_xy, fabs(wp2_x));
                            map_max_xy = max(map_max_xy, fabs(wp2_y));

                            // Convert to current scaling
                            wp1_x = wp1_x * mapScale;
                            wp1_y = wp1_y * mapScale;
                            wp2_x = wp2_x * mapScale;
                            wp2_y = wp2_y * mapScale;

                            // Draw if within current scaling range
                            if ((fabs(wp1_x) < 1.0) && (fabs(wp1_y) < 1.0)
                                    && (fabs(wp2_x) < 1.0)
                                    && (fabs(wp2_y) < 1.0))
                            {
                                osd_drawline_scaled(wp1_x, wp1_y, wp2_x, wp2_y,
                                        DRAW_LINE_WHITE, DRAW_LINE_WIDTH_WIDE);
                            }
                        }
                    }
                }

                if (map_max_xy > 5000)
                {
                    mapScale = 0.0001;
                }
                else if (map_max_xy > 2000)
                {
                    mapScale = 0.0002;
                }
                else if (map_max_xy > 1500)
                {
                    mapScale = 0.0005;
                }
                else if (map_max_xy > 1000)
                {
                    mapScale = 0.0006666667;
                }
                else if (map_max_xy > 750)
                {
                    mapScale = 0.001;
                }
                else if (map_max_xy > 500)
                {
                    mapScale = 0.00133333;
                }
                else if (map_max_xy > 400)
                {
                    mapScale = 0.002;
                }
                else if (map_max_xy > 300)
                {
                    mapScale = 0.0025;
                }
                else if (map_max_xy > 200)
                {
                    mapScale = 0.0033333;
                }
                else if (map_max_xy > 100)
                {
                    mapScale = 0.005;
                }
                else
                {
                    map_max_xy = 0.01;
                }
            }
        }
        else
        {
            firstPassMap = true;
        }

        oldNumWPLoaded = numWPLoaded;

*/
    }
}
