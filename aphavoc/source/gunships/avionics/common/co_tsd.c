// 
// 	 Enemy Engaged RAH-66 Comanche Versus KA-52 Hokum
// 	 Copyright (C) 2000 Empire Interactive (Europe) Ltd,
// 	 677 High Road, North Finchley, London N12 0DA
// 
// 	 Please see the document LICENSE.TXT for the full licence agreement
// 
// 2. LICENCE
//  2.1 	
//  	Subject to the provisions of this Agreement we now grant to you the 
//  	following rights in respect of the Source Code:
//   2.1.1 
//   	the non-exclusive right to Exploit  the Source Code and Executable 
//   	Code on any medium; and 
//   2.1.2 
//   	the non-exclusive right to create and distribute Derivative Works.
//  2.2 	
//  	Subject to the provisions of this Agreement we now grant you the
// 	following rights in respect of the Object Code:
//   2.2.1 
// 	the non-exclusive right to Exploit the Object Code on the same
// 	terms and conditions set out in clause 3, provided that any
// 	distribution is done so on the terms of this Agreement and is
// 	accompanied by the Source Code and Executable Code (as
// 	applicable).
// 
// 3. GENERAL OBLIGATIONS
//  3.1 
//  	In consideration of the licence granted in clause 2.1 you now agree:
//   3.1.1 
// 	that when you distribute the Source Code or Executable Code or
// 	any Derivative Works to Recipients you will also include the
// 	terms of this Agreement;
//   3.1.2 
// 	that when you make the Source Code, Executable Code or any
// 	Derivative Works ("Materials") available to download, you will
// 	ensure that Recipients must accept the terms of this Agreement
// 	before being allowed to download such Materials;
//   3.1.3 
// 	that by Exploiting the Source Code or Executable Code you may
// 	not impose any further restrictions on a Recipient's subsequent
// 	Exploitation of the Source Code or Executable Code other than
// 	those contained in the terms and conditions of this Agreement;
//   3.1.4 
// 	not (and not to allow any third party) to profit or make any
// 	charge for the Source Code, or Executable Code, any
// 	Exploitation of the Source Code or Executable Code, or for any
// 	Derivative Works;
//   3.1.5 
// 	not to place any restrictions on the operability of the Source 
// 	Code;
//   3.1.6 
// 	to attach prominent notices to any Derivative Works stating
// 	that you have changed the Source Code or Executable Code and to
// 	include the details anddate of such change; and
//   3.1.7 
//   	not to Exploit the Source Code or Executable Code otherwise than
// 	as expressly permitted by  this Agreement.
// 

//VJ 030429 TSD render mod
// common functions used by all choppers

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "project.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static rgb_colour
		terrain_colour[3][33];

static int 
	draw_large_mfd;

static int
	contour_spacing,
	contour_samples;
	
//VJ 030424 TSD render mod default to on
#define OPTIMISE_CONTOURS	1

#define OPTIMISE_PATHS		1


#if OPTIMISE_CONTOURS
static int
	num_contour_loops,
	num_contour_lines;
#endif


#define colour_base 32
#define colour_scale(a) min(colour_base,(int)(colour_base*a/elevation_factor)+1);			   	   			   

#define TSD_ASE_RANGE_2000		((float) 2000.0)
#define TSD_ASE_RANGE_5000		((float) 5000.0)
#define TSD_ASE_RANGE_10000	((float) 10000.0)
#define TSD_ASE_RANGE_25000	((float) 25000.0)

static rgb_colour
		dummy_colours[11];

#define MFD_COLOUR1 		  		(dummy_colours[0])
#define MFD_COLOUR2 		  		(dummy_colours[1])
#define MFD_COLOUR3 		  		(dummy_colours[2])
#define MFD_COLOUR4	  	  		(dummy_colours[3])
#define MFD_COLOUR5				(dummy_colours[4])
#define MFD_COLOUR6				(dummy_colours[5])
#define MFD_CONTOUR_COLOUR		(dummy_colours[6])
#define MFD_RIVER_COLOUR		(dummy_colours[7])
#define MFD_ROAD_COLOUR			(dummy_colours[8])
#define MFD_BACKGROUND_COLOUR	(dummy_colours[9])
#define clear_mfd_colour		(dummy_colours[10])

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void set_tsd_colours(void)
{
		set_rgb_colour (MFD_COLOUR1,   0, 0,  96, 255);  //dark blue
	   set_rgb_colour (MFD_COLOUR2,	32, 32,  164, 255); //light blue
		set_rgb_colour (MFD_COLOUR3, 220, 48,   0, 255); //bright red
		set_rgb_colour (MFD_COLOUR4, 148, 32,   0, 255);//dark red
		set_rgb_colour (MFD_COLOUR5,   0, 128,   192, 255);
		set_rgb_colour (MFD_COLOUR6,  255, 255,  0, 255);
	
		set_rgb_colour (MFD_CONTOUR_COLOUR,      48, 48,  48, 255);
		set_rgb_colour (MFD_RIVER_COLOUR,        50,  75, 225, 255);
		set_rgb_colour (MFD_ROAD_COLOUR,        255,255,96, 255);
		set_rgb_colour (MFD_ROAD_COLOUR,        248,248,248, 255);
		set_rgb_colour (clear_mfd_colour,        255,255,255,255);
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void reset_tsd_colours(void)
{
	set_rgb_colour (MFD_COLOUR1,              0, 255,   0, 255);
	set_rgb_colour (MFD_COLOUR2,              0, 200,   0, 255);
	set_rgb_colour (MFD_COLOUR3,              0, 176,   0, 255);
	set_rgb_colour (MFD_COLOUR4,              0, 151,   0, 255);
	set_rgb_colour (MFD_COLOUR5,              0, 128,   0, 255);
	set_rgb_colour (MFD_COLOUR6,             40,  68,  56, 255);

	set_rgb_colour (MFD_CONTOUR_COLOUR,     255, 100,   0, 255);
	set_rgb_colour (MFD_RIVER_COLOUR,        50,  75, 225, 255);
	set_rgb_colour (MFD_ROAD_COLOUR,        255, 200,   0, 255);
	set_rgb_colour (clear_mfd_colour,        255,255,255,0);
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Initialise_TSD_render_terrain(void)
{
	draw_large_mfd = TRUE;
	
	set_tsd_colours();
	
	set_rgb_colour(terrain_colour[0][0],100, 150, 240, 255); 
//colours as atlas: green yellow red white, strong	
	set_rgb_colour(terrain_colour[0][1],89,152,41,255);
	set_rgb_colour(terrain_colour[0][2],93,142,41,255);
	set_rgb_colour(terrain_colour[0][3],97,132,41,255);
	set_rgb_colour(terrain_colour[0][4],100,122,41,255);
	set_rgb_colour(terrain_colour[0][5],104,122,41,255);
	set_rgb_colour(terrain_colour[0][6],108,122,41,255);
	set_rgb_colour(terrain_colour[0][7],112,122,41,255);
	set_rgb_colour(terrain_colour[0][8],115,122,41,255);
	set_rgb_colour(terrain_colour[0][9],133,133,46,255);
	set_rgb_colour(terrain_colour[0][10],143,143,51,255);
	set_rgb_colour(terrain_colour[0][11],153,153,56,255);
	set_rgb_colour(terrain_colour[0][12],163,163,61,255);
	set_rgb_colour(terrain_colour[0][13],173,173,66,255);
	set_rgb_colour(terrain_colour[0][14],184,184,71,255);
	set_rgb_colour(terrain_colour[0][15],194,194,77,255);
	set_rgb_colour(terrain_colour[0][16],204,204,82,255);
	set_rgb_colour(terrain_colour[0][17],194,189,77,255);
	set_rgb_colour(terrain_colour[0][18],184,173,71,255);
	set_rgb_colour(terrain_colour[0][19],173,158,66,255);
	set_rgb_colour(terrain_colour[0][20],163,143,61,255);
	set_rgb_colour(terrain_colour[0][21],153,128,56,255);
	set_rgb_colour(terrain_colour[0][22],143,112,51,255);
	set_rgb_colour(terrain_colour[0][23],133,97,46,255);
	set_rgb_colour(terrain_colour[0][24],122,82,41,255);
	set_rgb_colour(terrain_colour[0][25],130,93,56,255);
	set_rgb_colour(terrain_colour[0][26],138,105,71,255);
	set_rgb_colour(terrain_colour[0][27],146,116,87,255);
	set_rgb_colour(terrain_colour[0][28],153,128,102,255);
	set_rgb_colour(terrain_colour[0][29],161,139,117,255);
	set_rgb_colour(terrain_colour[0][30],169,151,133,255);
	set_rgb_colour(terrain_colour[0][31],176,162,148,255);
	set_rgb_colour(terrain_colour[0][32],184,174,163,255);

//yellow to brown		
	set_rgb_colour(terrain_colour[1][0],100, 150, 240, 255); 
	set_rgb_colour(terrain_colour[1][1],254,230,175,255);
	set_rgb_colour(terrain_colour[1][2],254,231,170,255);
	set_rgb_colour(terrain_colour[1][3],254,232,165,255);
	set_rgb_colour(terrain_colour[1][4],254,231,161,255);
	set_rgb_colour(terrain_colour[1][5],255,231,156,255);
	set_rgb_colour(terrain_colour[1][6],255,229,153,255);
	set_rgb_colour(terrain_colour[1][7],255,228,149,255);
	set_rgb_colour(terrain_colour[1][8],255,225,145,255);
	set_rgb_colour(terrain_colour[1][9],255,222,142,255);
	set_rgb_colour(terrain_colour[1][10],255,219,139,255);
	set_rgb_colour(terrain_colour[1][11],255,215,136,255);
	set_rgb_colour(terrain_colour[1][12],255,211,133,255);
	set_rgb_colour(terrain_colour[1][13],255,206,129,255);
	set_rgb_colour(terrain_colour[1][14],255,201,126,255);
	set_rgb_colour(terrain_colour[1][15],255,195,123,255);
	set_rgb_colour(terrain_colour[1][16],253,189,120,255);
	set_rgb_colour(terrain_colour[1][17],251,183,116,255);
	set_rgb_colour(terrain_colour[1][18],247,176,112,255);
	set_rgb_colour(terrain_colour[1][19],243,168,108,255);
	set_rgb_colour(terrain_colour[1][20],238,161,103,255);
	set_rgb_colour(terrain_colour[1][21],232,152,99,255);
	set_rgb_colour(terrain_colour[1][22],225,144,94,255);
	set_rgb_colour(terrain_colour[1][23],217,135,88,255);
	set_rgb_colour(terrain_colour[1][24],208,126,82,255);
	set_rgb_colour(terrain_colour[1][25],198,117,75,255);
	set_rgb_colour(terrain_colour[1][26],186,107,68,255);
	set_rgb_colour(terrain_colour[1][27],173,97,61,255);
	set_rgb_colour(terrain_colour[1][28],159,86,52,255);
	set_rgb_colour(terrain_colour[1][29],143,76,43,255);
	set_rgb_colour(terrain_colour[1][30],125,65,34,255);
	set_rgb_colour(terrain_colour[1][31],106,53,23,255);
	set_rgb_colour(terrain_colour[1][32],86,42,12,255);	
	                                                  
//colours as atlas but washed
	set_rgb_colour(terrain_colour[2][32],90,59,27,255);
	set_rgb_colour(terrain_colour[2][31],105,59,27,255);
	set_rgb_colour(terrain_colour[2][30],119,61,28,255);
	set_rgb_colour(terrain_colour[2][29],131,65,30,255);
	set_rgb_colour(terrain_colour[2][28],141,70,34,255);
	set_rgb_colour(terrain_colour[2][27],150,77,39,255);
	set_rgb_colour(terrain_colour[2][26],157,85,44,255);
	set_rgb_colour(terrain_colour[2][25],163,94,50,255);
	set_rgb_colour(terrain_colour[2][24],169,102,57,255);
	set_rgb_colour(terrain_colour[2][23],173,111,63,255);
	set_rgb_colour(terrain_colour[2][22],176,119,70,255);
	set_rgb_colour(terrain_colour[2][21],179,127,76,255);
	set_rgb_colour(terrain_colour[2][20],182,134,81,255);
	set_rgb_colour(terrain_colour[2][19],184,139,85,255);
	set_rgb_colour(terrain_colour[2][18],186,143,89,255);
	set_rgb_colour(terrain_colour[2][17],188,144,91,255);
	set_rgb_colour(terrain_colour[2][16],191,144,91,255);
	set_rgb_colour(terrain_colour[2][15],207,159,102,255);
	set_rgb_colour(terrain_colour[2][14],214,169,110,255);
	set_rgb_colour(terrain_colour[2][13],217,177,119,255);
	set_rgb_colour(terrain_colour[2][12],216,183,128,255);
	set_rgb_colour(terrain_colour[2][11],212,188,136,255);
	set_rgb_colour(terrain_colour[2][10],205,192,143,255);
	set_rgb_colour(terrain_colour[2][9],194,194,149,255);
	set_rgb_colour(terrain_colour[2][8],181,194,154,255);
	set_rgb_colour(terrain_colour[2][7],166,193,156,255);
	set_rgb_colour(terrain_colour[2][6],149,191,157,255);
	set_rgb_colour(terrain_colour[2][5],130,188,155,255);
	set_rgb_colour(terrain_colour[2][4],110,183,150,255);
	set_rgb_colour(terrain_colour[2][3],88,177,142,255);
	set_rgb_colour(terrain_colour[2][2],65,170,130,255);
	set_rgb_colour(terrain_colour[2][1],42,162,114,255);
	set_rgb_colour(terrain_colour[2][0],100, 150, 240, 255); 

}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_contour_lines (vec3d *p1, vec3d *p2, vec3d *p3)
{
	int
		y,
		y1,
		y2,
		y3,
		dy,
		y_tmp,
		contour_samples_minus_one;

	float
		x1,
		z1,
		x2,
		z2,
		x3,
		z3,
		dx1,
		dz1,
		dx2,
		dz2,
		x_tmp,
		z_tmp,
		one_over_dy;

	x1 = p1->x;
	convert_float_to_int (p1->y, &y1);
	z1 = p1->z;

	x2 = p2->x;
	convert_float_to_int (p2->y, &y2);
	z2 = p2->z;

	x3 = p3->x;
	convert_float_to_int (p3->y, &y3);
	z3 = p3->z;

	//
	// ensure above sea level
	//

	y1 = max (y1, 0);

	y2 = max (y2, 0);

	y3 = max (y3, 0);

	//
	// scale height values for contour line spacing
	//

	y1 /= contour_spacing;
	y2 /= contour_spacing;
	y3 /= contour_spacing;

	contour_samples_minus_one = contour_samples - 1;

	//
	// check if flat
	//

	if ((y1 == y2) && (y2 == y3))
	{
		return;
	}

	//
	// sort height values (y1 = lowest, y3 = highest)
	//

	if (y1 > y3)
	{
		x_tmp = x1;
		y_tmp = y1;
		z_tmp = z1;

		x1 = x3;
		y1 = y3;
		z1 = z3;

		x3 = x_tmp;
		y3 = y_tmp;
		z3 = z_tmp;
	}

	if (y2 > y3)
	{
		x_tmp = x2;
		y_tmp = y2;
		z_tmp = z2;

		x2 = x3;
		y2 = y3;
		z2 = z3;

		x3 = x_tmp;
		y3 = y_tmp;
		z3 = z_tmp;
	}

	if (y1 > y2)
	{
		x_tmp = x1;
		y_tmp = y1;
		z_tmp = z1;

		x1 = x2;
		y1 = y2;
		z1 = z2;

		x2 = x_tmp;
		y2 = y_tmp;
		z2 = z_tmp;
	}

	//
	// draw contour lines
	//

	if (y1 == y2)
	{
		dy = y3 - y1;

		one_over_dy = 1.0 / (float) dy;

		dx1 = (x3 - x1) * one_over_dy;
		dz1 = (z3 - z1) * one_over_dy;
		dx2 = (x3 - x2) * one_over_dy;
		dz2 = (z3 - z2) * one_over_dy;

		y = contour_samples_minus_one - ((y1 - 1) % contour_samples);

		while (dy--)
		{
			#if OPTIMISE_CONTOURS

			num_contour_loops++;

			#endif

			if (y == 0)
			{
				#if OPTIMISE_CONTOURS

				num_contour_lines++;

				#endif

				y = contour_samples;

				draw_2d_line (x1, z1, x2, z2, MFD_CONTOUR_COLOUR);
			}

			y--;

			x1 += dx1;
			z1 += dz1;
			x2 += dx2;
			z2 += dz2;
		}
	}
	else if (y2 == y3)
	{
		dy = y2 - y1;

		one_over_dy = 1.0 / (float) dy;

		dx1 = (x2 - x1) * one_over_dy;
		dz1 = (z2 - z1) * one_over_dy;
		dx2 = (x3 - x1) * one_over_dy;
		dz2 = (z3 - z1) * one_over_dy;

		y = contour_samples_minus_one - ((y1 - 1) % contour_samples);

		x2 = x1;
		z2 = z1;

		while (dy--)
		{
			#if OPTIMISE_CONTOURS

			num_contour_loops++;

			#endif

			if (y == 0)
			{
				#if OPTIMISE_CONTOURS

				num_contour_lines++;

				#endif

				y = contour_samples;

				draw_2d_line (x1, z1, x2, z2, MFD_CONTOUR_COLOUR);

			}

			y--;

			x1 += dx1;
			z1 += dz1;
			x2 += dx2;
			z2 += dz2;
		}
	}
	else
	{
		dy = y3 - y1;

		one_over_dy = 1.0 / (float) dy;

		dx1 = (x3 - x1) * one_over_dy;
		dz1 = (z3 - z1) * one_over_dy;

		dy = y2 - y1;

		one_over_dy = 1.0 / (float) dy;

		dx2 = (x2 - x1) * one_over_dy;
		dz2 = (z2 - z1) * one_over_dy;

		y = contour_samples_minus_one - ((y1 - 1) % contour_samples);

		x2 = x1;
		z2 = z1;

		while (dy--)
		{
			#if OPTIMISE_CONTOURS

			num_contour_loops++;

			#endif

			if (y == 0)
			{
				#if OPTIMISE_CONTOURS

				num_contour_lines++;

				#endif

				y = contour_samples;

				draw_2d_line (x1, z1, x2, z2, MFD_CONTOUR_COLOUR);

			}

			y--;

			x1 += dx1;
			z1 += dz1;
			x2 += dx2;
			z2 += dz2;
		}

		dy = y3 - y2;

		one_over_dy = 1.0 / (float) dy;

		dx2 = (x3 - x2) * one_over_dy;
		dz2 = (z3 - z2) * one_over_dy;

		y = contour_samples_minus_one - ((y2 - 1) % contour_samples);

		while (dy--)
		{
			#if OPTIMISE_CONTOURS

			num_contour_loops++;

			#endif

			if (y == 0)
			{
				#if OPTIMISE_CONTOURS

				num_contour_lines++;

				#endif

				y = contour_samples;

				draw_2d_line (x1, z1, x2, z2, MFD_CONTOUR_COLOUR);
			}

			y--;

			x1 += dx1;
			z1 += dz1;
			x2 += dx2;
			z2 += dz2;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_path_lines
(
	int number_of_paths,
	contour_path *paths,
	vec3d *nodes,
	rgb_colour colour,
	int detail_level,
	float x_world_min,
	float z_world_min,
	float x_world_mid,
	float z_world_mid,
	float x_world_max,
	float z_world_max,
	float scale,
	int draw_thick_lines,
	int draw_large_mfd
)
{
	int
		this_path,
		outcode1,
		outcode2,
		index,
		step_size;

	#if OPTIMISE_PATHS

	int
		num_paths_traversed,
		num_lines_drawn;

	#endif

	float
		x1,
		z1,
		x2,
		z2;

	vec3d
		*sub_positions;

	ASSERT (number_of_paths > 0);

	ASSERT (paths);

	ASSERT (nodes);

	#if OPTIMISE_PATHS

	num_paths_traversed = 0;

	num_lines_drawn = 0;

	#endif

	step_size = max (detail_level << 3, 1);

	for (this_path = 0; this_path < number_of_paths; this_path++)
	{
		if
		(
			(paths[this_path].type != TEMP_TERRAIN_TYPE_COASTAL_RIVER) &&
			(paths[this_path].type != TEMP_TERRAIN_TYPE_LAKE) &&
			(paths[this_path].type != TEMP_TERRAIN_TYPE_OFFROAD)
		)
		{
			//
			// trivially reject path
			//

			outcode1 = 0;

			if (paths[this_path].xmin < x_world_min) outcode1 |= CLIP_LEFT;
			if (paths[this_path].xmin > x_world_max) outcode1 |= CLIP_RIGHT;
			if (paths[this_path].zmin < z_world_min) outcode1 |= CLIP_TOP;
			if (paths[this_path].zmin > z_world_max) outcode1 |= CLIP_BOTTOM;

			outcode2 = 0;

			if (paths[this_path].xmax < x_world_min) outcode2 |= CLIP_LEFT;
			if (paths[this_path].xmax > x_world_max) outcode2 |= CLIP_RIGHT;
			if (paths[this_path].zmax < z_world_min) outcode2 |= CLIP_TOP;
			if (paths[this_path].zmax > z_world_max) outcode2 |= CLIP_BOTTOM;

			if (!(outcode1 & outcode2))
			{
				#if OPTIMISE_PATHS

				num_paths_traversed++;

				#endif

				//
				// this also catches conditions where (count == 0)
				//

				if (paths[this_path].count <= step_size)
				{
					////////////////////////////////////////
					//
					// draw path as a single line
					//
					////////////////////////////////////////

					x1 = (nodes[paths[this_path].from].x - x_world_mid) * scale;
					z1 = (nodes[paths[this_path].from].z - z_world_mid) * scale;

					x2 = (nodes[paths[this_path].to].x - x_world_mid) * scale;
					z2 = (nodes[paths[this_path].to].z - z_world_mid) * scale;

					if (draw_thick_lines && draw_large_mfd)
					{
						draw_2d_half_thick_line (x1, z1, x2, z2, colour);
					}
					else
					{
						draw_2d_line (x1, z1, x2, z2, colour);
					}

					#if OPTIMISE_PATHS

					num_lines_drawn++;

					#endif
				}
				else
				{
					////////////////////////////////////////
					//
					// draw path with variable detail level
					//
					////////////////////////////////////////

					sub_positions = paths[this_path].points;

					index = step_size - 1;;

					//
					// draw line from start node to first sub-position
					//

					x1 = (nodes[paths[this_path].from].x - x_world_mid) * scale;
					z1 = (nodes[paths[this_path].from].z - z_world_mid) * scale;

					x2 = (sub_positions[index].x - x_world_mid) * scale;
					z2 = (sub_positions[index].z - z_world_mid) * scale;

					if (draw_thick_lines && draw_large_mfd)
					{
						draw_2d_half_thick_line (x1, z1, x2, z2, colour);
					}
					else
					{
						draw_2d_line (x1, z1, x2, z2, colour);
					}

					#if OPTIMISE_PATHS

					num_lines_drawn++;

					#endif

					//
					// draw sub-position lines
					//

					index += step_size;

					while (index < paths[this_path].count)
					{
						x1 = x2;
						z1 = z2;

						x2 = (sub_positions[index].x - x_world_mid) * scale;
						z2 = (sub_positions[index].z - z_world_mid) * scale;

						if (draw_thick_lines && draw_large_mfd)
						{
							draw_2d_half_thick_line (x1, z1, x2, z2, colour);
						}
						else
						{
							draw_2d_line (x1, z1, x2, z2, colour);
						}

						#if OPTIMISE_PATHS

						num_lines_drawn++;

						#endif

						index += step_size;
					}

					//
					// draw line from last sub-position to end node
					//

					x1 = x2;
					z1 = z2;

					x2 = (nodes[paths[this_path].to].x - x_world_mid) * scale;
					z2 = (nodes[paths[this_path].to].z - z_world_mid) * scale;

					if (draw_thick_lines && draw_large_mfd)
					{
						draw_2d_half_thick_line (x1, z1, x2, z2, colour);
					}
					else
					{
						draw_2d_line (x1, z1, x2, z2, colour);
					}

					#if OPTIMISE_PATHS

					num_lines_drawn++;

					#endif
				}
			}
		}
	}

	#if OPTIMISE_PATHS

	debug_filtered_log ("paths=%d, paths drawn=%d, lines drawn=%d", number_of_paths, num_paths_traversed, num_lines_drawn);

	#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void draw_tsd_terrain_map (env_2d *mfd_env, float y_translate, float range, float scale, vec3d *position, float heading)
{
	int
		x_index,
		z_index,
		row_add_on,
		x_min_index,
		z_min_index,
		x_max_index,
		z_max_index;

	float
		x_min,
		z_min,
		x_mid,
		z_mid,
		x_max,
		z_max,
		radius,
		distance,
		dx0,
		dz0,
		dx1,
		dz1,
		dx_grid,
		dz_grid,
		dx_start,
		dz_start,
		*this_row_ptr,
		*next_row_ptr,
		*this_row_start_ptr,
		*next_row_start_ptr;
		

		float 
			terrain_elev=0,
         elevation_factor=terrain_3d_map_maximum_height*0.97;
         
		rgb_colour 
			terrain_col;

      int step = 1;
      int stepmask;

	ASSERT (position);




	//
	// rotate map
	//

	set_2d_window_rotation (mfd_env, heading);

	//
	// get map centre position
	//

	distance = y_translate / scale;

	x_mid = position->x + (distance * sin (heading));
	z_mid = position->z + (distance * cos (heading));

	//
	// get map area (increased by one grid, where hypotenuse is approx (x + z))
	//

	radius = range + terrain_3d_simple_elevation_x_grid_size + terrain_3d_simple_elevation_z_grid_size;
	
	x_min = x_mid - radius;
	z_min = z_mid - radius;

	x_max = x_mid + radius;
	z_max = z_mid + radius;

	//
	// clip map area
	//

	x_min = max (x_min, MIN_MAP_X);
	z_min = max (z_min, MIN_MAP_Z);

	x_max = min (x_max, MAX_MAP_X);
	z_max = min (z_max, MAX_MAP_Z);


	//
	// get simple terrain map indices
	//

	convert_float_to_int ((x_min * terrain_3d_simple_elevation_x_grid_size_reciprocal), &x_min_index);
	convert_float_to_int ((z_min * terrain_3d_simple_elevation_z_grid_size_reciprocal), &z_min_index);

	convert_float_to_int ((x_max * terrain_3d_simple_elevation_x_grid_size_reciprocal), &x_max_index);
	convert_float_to_int ((z_max * terrain_3d_simple_elevation_z_grid_size_reciprocal), &z_max_index);

   if (range == TSD_ASE_RANGE_25000)
     step = 2;
   stepmask = ~(step - 1);

	//
	// scan map
	//

	x_min_index &= stepmask;
	z_min_index &= stepmask;

	x_max_index += step - 1;
	z_max_index += step - 1;

	x_max_index &= stepmask;
	z_max_index &= stepmask;

	if (x_max_index >= terrain_3d_simple_elevation_width)
	{
		x_max_index = terrain_3d_simple_elevation_width & stepmask;
	}

	if (z_max_index >= terrain_3d_simple_elevation_height)
	{
		z_max_index = terrain_3d_simple_elevation_height & stepmask;
	}

	if ((x_min_index < x_max_index) && (z_min_index < z_max_index))
	{		
		this_row_start_ptr = &terrain_3d_simple_elevation_grid[(z_min_index * terrain_3d_simple_elevation_width) + x_min_index];

		row_add_on = terrain_3d_simple_elevation_width * step;

		next_row_start_ptr = this_row_start_ptr + row_add_on;

		dx_start = ((((float) x_min_index) * terrain_3d_simple_elevation_x_grid_size) - x_mid) * scale;
		dz_start = ((((float) z_min_index) * terrain_3d_simple_elevation_z_grid_size) - z_mid) * scale;

		dx_grid = terrain_3d_simple_elevation_x_grid_size * (float) step * scale;
		dz_grid = terrain_3d_simple_elevation_z_grid_size * (float) step * scale;

		dz0 = dz_start;
		dz1 = dz_start + dz_grid;

		for (z_index = z_min_index; z_index < z_max_index; z_index += step)
		{			
			float mid_x, mid_z, mid_y;			

			dx0 = dx_start;
			dx1 = dx_start + dx_grid;

			mid_z = 0.5*(dz0+dz1);

			this_row_ptr = this_row_start_ptr;
			next_row_ptr = next_row_start_ptr;

			for (x_index = x_min_index; x_index < x_max_index; x_index += step)
			{
				int c = 0;		
				
				mid_x = 0.5*(dx0+dx1);
			   mid_y = (this_row_ptr[0]+this_row_ptr[1]+next_row_ptr[0]+next_row_ptr[1])/4.0;

			   terrain_elev = (this_row_ptr[0]+this_row_ptr[1]+mid_y)/3.0;			   
		   	c = colour_scale(terrain_elev);
			   if (terrain_elev > 0)
			   	terrain_col = terrain_colour[tsd_render_palette][c];			   	
			   else
			   	terrain_col = terrain_colour[tsd_render_palette][0];
			   draw_2d_filled_triangle (dx0,dz0,mid_x,mid_z,dx1,dz0,terrain_col);
			                                                                 
			   terrain_elev = (this_row_ptr[1]+next_row_ptr[1]+mid_y)/3.0;   
		   	c = colour_scale(terrain_elev);
			   if (terrain_elev > 0)
			   	terrain_col = terrain_colour[tsd_render_palette][c];			   	
			   else
			   	terrain_col = terrain_colour[tsd_render_palette][0];
			   draw_2d_filled_triangle (dx1,dz0,mid_x,mid_z,dx1,dz1,terrain_col);
                                                                          
			   terrain_elev = (this_row_ptr[0]+next_row_ptr[0]+mid_y)/3.0;   
		   	c = colour_scale(terrain_elev);
			   if (terrain_elev > 0)
			   	terrain_col = terrain_colour[tsd_render_palette][c];			   	
			   else
			   	terrain_col = terrain_colour[tsd_render_palette][0];
			   draw_2d_filled_triangle (dx0,dz0,dx0,dz1,mid_x,mid_z,terrain_col);
                                                                          
			   terrain_elev = (next_row_ptr[0]+next_row_ptr[1]+mid_y)/3.0;   
		   	c = colour_scale(terrain_elev);
			   if (terrain_elev > 0)
			   	terrain_col = terrain_colour[tsd_render_palette][c];			   	
			   else
			   	terrain_col = terrain_colour[tsd_render_palette][0];
			   draw_2d_filled_triangle (dx0,dz1,dx1,dz1,mid_x,mid_z,terrain_col);			   
			                                                                                       
				//                                  
				// next column                      
				//                                  
                                                
				dx0 += dx_grid;                     
				dx1 += dx_grid;                     
                                                
				this_row_ptr += step;                  
				next_row_ptr += step;                  
			}                                      
                                                
			//                                     
			// next row                            
			//                                     
                                                
			dz0 += dz_grid;                        
			dz1 += dz_grid;                        

			this_row_start_ptr += row_add_on;
			next_row_start_ptr += row_add_on;
		}
	}

	set_2d_window_rotation (mfd_env, 0.0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_tsd_contour_map (env_2d *mfd_env, float y_translate, float range, float scale, vec3d *position, float heading, int draw_large_mfd)
{
	int
		x_index,
		z_index,
		row_add_on,
		x_min_index,
		z_min_index,
		x_max_index,
		z_max_index,
		path_detail_level,
		contour_granularity,
		contour_granularity_mask;

	float
		x_min,
		z_min,
		x_mid,
		z_mid,
		x_max,
		z_max,
		radius,
		distance,
		dx0,
		dz0,
		dx1,
		dz1,
		dx_grid,
		dz_grid,
		dx_start,
		dz_start,
		*this_row_ptr,
		*next_row_ptr,
		*this_row_start_ptr,
		*next_row_start_ptr;

	vec3d
		p1,
		p2,
		p3;

	ASSERT (position);

	#if OPTIMISE_CONTOURS

	num_contour_loops = 0;

	num_contour_lines = 0;

	#endif

	if (tsd_render_mode == TSD_RENDER_CONTOUR_MODE)
	  reset_tsd_colours();
	else  
	  set_tsd_colours();

	//
	// rotate map
	//

	set_2d_window_rotation (mfd_env, heading);

	//
	// get map centre position
	//

	distance = y_translate / scale;

	x_mid = position->x + (distance * sin (heading));
	z_mid = position->z + (distance * cos (heading));

	//
	// get map area (increased by one grid, where hypotenuse is approx (x + z))
	//

	radius = range + terrain_3d_simple_elevation_x_grid_size + terrain_3d_simple_elevation_z_grid_size;

	x_min = x_mid - radius;
	z_min = z_mid - radius;

	x_max = x_mid + radius;
	z_max = z_mid + radius;

	//
	// clip map area
	//

	x_min = max (x_min, MIN_MAP_X);
	z_min = max (z_min, MIN_MAP_Z);

	x_max = min (x_max, MAX_MAP_X);
	z_max = min (z_max, MAX_MAP_Z);

	////////////////////////////////////////
	//
	// DRAW CONTOURS
	//
	////////////////////////////////////////

	//
	// get simple terrain map indices
	//

	convert_float_to_int ((x_min * terrain_3d_simple_elevation_x_grid_size_reciprocal), &x_min_index);
	convert_float_to_int ((z_min * terrain_3d_simple_elevation_z_grid_size_reciprocal), &z_min_index);

	convert_float_to_int ((x_max * terrain_3d_simple_elevation_x_grid_size_reciprocal), &x_max_index);
	convert_float_to_int ((z_max * terrain_3d_simple_elevation_z_grid_size_reciprocal), &z_max_index);

	////////////////////////////////////////
	//
	// contour_granularity: chunks the terrain grid
	// contour_samples    : culls the number of contour line samples
	// contour_spacing    : height between contour lines (meters)
	// path_detail_level  : 0 = highest detail (ie draw all sub-lines)
	//
	////////////////////////////////////////

	////////////////////////////////////////
	if (range == TSD_ASE_RANGE_2000)
	////////////////////////////////////////
	{
		contour_granularity = 1;

		contour_samples = 5;

		contour_spacing = 100;

		if (draw_large_mfd)
		{
			path_detail_level = 0;
		}
		else
		{
			path_detail_level = 2;
		}
	}
	////////////////////////////////////////
	else if (range == TSD_ASE_RANGE_5000)
	////////////////////////////////////////
	{
		if (draw_large_mfd)
		{
			contour_granularity = 1;

			contour_samples = 10;

			path_detail_level = 0;
		}
		else
		{
			contour_granularity = 2;

			contour_samples = 10;

			path_detail_level = 2;
		}

		contour_spacing = 100;
	}
	////////////////////////////////////////
	else if (range == TSD_ASE_RANGE_10000)
	////////////////////////////////////////
	{
		if (draw_large_mfd)
		{
			contour_granularity = 2;

			contour_samples = 10;

			path_detail_level = 1;
		}
		else
		{
			contour_granularity = 4;

			contour_samples = 20;

			path_detail_level = 3;
		}

		contour_spacing = 100;
	}
	
	////////////////////////////////////////
	else if (range == TSD_ASE_RANGE_25000)
	////////////////////////////////////////
	{
		if (draw_large_mfd)
		{
			contour_granularity = 4;

			contour_samples = 10;

			path_detail_level = 2;
		}
		else
		{
			contour_granularity = 8;

			contour_samples = 15;

			path_detail_level = 4;
		}

		contour_spacing = 250;
	}
	
	////////////////////////////////////////
	else
	///////////////////////////////////////
	{
		debug_fatal ("Unknown TSD/ASE range %.2f", range);
	}
	//VJ 030423 TSD render mod
	if (tsd_render_mode != TSD_RENDER_RELIEF_MODE)
	{	
	
		contour_spacing /= contour_samples;
	
		contour_granularity_mask = ~(contour_granularity - 1);
	
		//
		// scan map
		//
	
		x_min_index &= contour_granularity_mask;
		z_min_index &= contour_granularity_mask;
	
		x_max_index += contour_granularity - 1;
		z_max_index += contour_granularity - 1;
	
		x_max_index &= contour_granularity_mask;
		z_max_index &= contour_granularity_mask;
	
		if (x_max_index >= terrain_3d_simple_elevation_width)
		{
			x_max_index = terrain_3d_simple_elevation_width & contour_granularity_mask;
		}
	
		if (z_max_index >= terrain_3d_simple_elevation_height)
		{
			z_max_index = terrain_3d_simple_elevation_height & contour_granularity_mask;
		}
	
		if ((x_min_index < x_max_index) && (z_min_index < z_max_index))
		{		
			this_row_start_ptr = &terrain_3d_simple_elevation_grid[(z_min_index * terrain_3d_simple_elevation_width) + x_min_index];
	
			row_add_on = terrain_3d_simple_elevation_width * contour_granularity;
	
			next_row_start_ptr = this_row_start_ptr + row_add_on;
	
			dx_start = ((((float) x_min_index) * terrain_3d_simple_elevation_x_grid_size) - x_mid) * scale;
			dz_start = ((((float) z_min_index) * terrain_3d_simple_elevation_z_grid_size) - z_mid) * scale;
	
			dx_grid = terrain_3d_simple_elevation_x_grid_size * (float) contour_granularity * scale;
			dz_grid = terrain_3d_simple_elevation_z_grid_size * (float) contour_granularity * scale;
	
			dz0 = dz_start;
			dz1 = dz_start + dz_grid;
	
			for (z_index = z_min_index; z_index < z_max_index; z_index += contour_granularity)
			{			
				dx0 = dx_start;
				dx1 = dx_start + dx_grid;
	
				this_row_ptr = this_row_start_ptr;
				next_row_ptr = next_row_start_ptr;
	
				for (x_index = x_min_index; x_index < x_max_index; x_index += contour_granularity)
				{
					//
					// offset [z=0][x=0]
					//
	
					p1.x = dx0;
					p1.y = this_row_ptr[0];
					p1.z = dz0;
	
					//
					// offset [z=0][x=1]
					//
	
					p2.x = dx1;
					p2.y = this_row_ptr[contour_granularity];
					p2.z = dz0;
	
					//
					// offset [z=1][x=0]
					//
	
					p3.x = dx0;
					p3.y = next_row_ptr[0];
					p3.z = dz1;
	
	            
					draw_contour_lines (&p1, &p2, &p3);
	
					//
					// offset [z=1][x=1]
					//
	
					p1.x = dx1;
					p1.y = next_row_ptr[contour_granularity];
					p1.z = dz1;
	
	
					draw_contour_lines (&p1, &p2, &p3);
	
					//
					// next column
					//
	
					dx0 += dx_grid;
					dx1 += dx_grid;
	
					this_row_ptr += contour_granularity;
					next_row_ptr += contour_granularity;
				}
	
				//
				// next row
				//
	
				dz0 += dz_grid;
				dz1 += dz_grid;
	
				this_row_start_ptr += row_add_on;
				next_row_start_ptr += row_add_on;
			}
		}
	
		#if OPTIMISE_CONTOURS
	
		debug_filtered_log ("num_contour_loops = %d, num_contour_lines = %d", num_contour_loops, num_contour_lines);
	
		#endif
		
	}//tsd_render_mode != TSD_RENDER_RELIEF_MODE

   path_detail_level = max(0, path_detail_level-1);
   //VJ 030423 TSD render mod, increase path detail level

	////////////////////////////////////////
	//
	// DRAW RIVERS
	//
	////////////////////////////////////////

	draw_path_lines
	(
		contour_map_number_of_river_paths,
		contour_map_river_paths,
		contour_map_river_nodes,
		MFD_RIVER_COLOUR,
		path_detail_level,
		x_min,
		z_min,
		x_mid,
		z_mid,
		x_max,
		z_max,
		scale,
		draw_large_mfd,  //VJ 020426 draw roads thick when draw_large_mfd on
		draw_large_mfd
	);

	////////////////////////////////////////
	//
	// DRAW ROADS
	//
	////////////////////////////////////////

	draw_path_lines
	(
		contour_map_number_of_road_paths,
		contour_map_road_paths,
		contour_map_road_nodes,
		MFD_ROAD_COLOUR,
		path_detail_level,
		x_min,
		z_min,
		x_mid,
		z_mid,
		x_max,
		z_max,
		scale,
		//FALSE,
		TRUE, //VJ 030423 TSD render mod, roads were not very visible
		draw_large_mfd
	);

	//
	// reset window rotation
	//

	set_2d_window_rotation (mfd_env, 0.0);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////