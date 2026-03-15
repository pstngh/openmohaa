textures/light/test
{
	surfaceparm nomarks
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/test.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/test_1_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/jh_rustwin1lit
{
	qer_editorimage textures/algiers/jh_rustwin1.tga
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/Algiers/jh_rustwin1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_rustwin1_blend.tga
		blendfunc GL_ONE GL_ONE
	}
} 

textures/light/jh_rustwin1_blend
{
	qer_editorimage textures/light/jh_rustwin1_blend.tga
	surfaceparm nomarks
	surfaceparm glass
	{
		map textures/light/jh_rustwin1_blend.tga
		rgbGen identity
	}
}

textures/light/ihwin1a_framelit
{
	qer_editorimage textures/light/ihwin1a_framelit.tga
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/ihwin1a_framelit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/ihwin1a_framelit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/window2
{
	qer_editorimage textures/light/window_02.tga
	qer_keyword utility
	surfaceparm nomarks
	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/window_02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/window_02_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/window3
{
	qer_editorimage textures/light/window_03.tga
	qer_keyword utility
	surfaceparm nomarks
	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/window_03.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/window_03_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/ce_window1night
{
	qer_keyword utility
	surfaceparm nomarks
	q3map_surfacelight 1500
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/ce_window1night.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/ce_window1night_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/window4_interiorlit
{
	qer_keyword utility
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 1500
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/window4_interiorlit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/window4_interiorlit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/window4_exteriorlit
{
	qer_keyword utility
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 1500
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/window4_exteriorlit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/window4_exteriorlit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
} 

textures/light/jh_natestahwin1lit
{
	qer_editorimage textures/light/jh_natestahwin1_blend.tga
	qer_keyword utility
	surfaceparm nomarks
	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/general_structure/jh_natestahwin1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_natestahwin1_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}
textures/light/jh_cathwin1
{
	qer_editorimage textures/central_europe/jh_cathwin1.tga
	qer_keyword utility
	surfaceparm nomarks
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/central_europe/jh_cathwin1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_cathwin1_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/jh_window2pj_lit
{
	qer_editorimage textures/mohcommon/jh_window2pj_lit.tga
	surfaceparm nomarks
	q3map_surfacelight 100
	surfaceparm glass
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/mohcommon/jh_window2pj_lit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_window2pj_lit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/jh_window3pj_lit
{
	qer_editorimage textures/mohcommon/jh_window3pj_lit.tga
	surfaceparm nomarks
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/mohcommon/jh_window3pj_lit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_window3pj_lit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
} 

textures/light/jh_window1pj_lit
{
	qer_editorimage textures/light/jh_window1pj_lit.tga
	surfaceparm nomarks
	q3map_surfacelight 3000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/jh_window1pj_lit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jh_window1pj_lit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
} 

textures/light/jhwin1a_framelit
{
	qer_editorimage textures/light/jhwin1a_framelit.tga
	surfaceparm nomarks
	q3map_surfacelight 4000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/jhwin1a_framelit.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/jhwin1a_framelit_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

// Globe light post
globe
{
	qer_editorimage textures/models/lights/globe.tga
	{
		map textures/models/lights/globe.tga
		rgbGen lightingSpherical
	}
}

g_post
{
	qer_editorimage textures/models/lights/g_post.tga
	{
		map textures/models/lights/g_post.tga
		rgbGen lightingSpherical
	}
}

// Static Globe light post
static_globe
{
	qer_editorimage textures/models/lights/globe.tga
	{
		map textures/models/lights/globe.tga
		rgbgen lightingSpherical
	}
	{
		map textures/models/lights/globe.tga
		blendfunc blend
                rgbGen identity
	}
}

static_g_post
{
	qer_editorimage textures/models/lights/g_post.tga
	{
		map textures/models/lights/g_post.tga
		rgbgen lightingSpherical
	}
}


textures/light/ornmntl_lampshade
{
	qer_editorimage textures/interior/ornmntl_lampshade.tga
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
	// light 1
	// test light
//	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/interior/ornmntl_lampshade.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/ornmntl_lampshade_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

// Globe light post winter
globe_winter
{
	qer_editorimage textures/models/lights/globe_winter.tga
	{
		map textures/models/lights/globe_winter.tga
		rgbGen lightingSpherical
	}
}

g_post_winter
{
	qer_editorimage textures/models/lights/g_post_winter.tga
	{
		map textures/models/lights/g_post_winter.tga
		rgbGen lightingSpherical
	}
}

static_globe_winter
{
	qer_editorimage textures/models/lights/globe_winter.tga
	{
		map textures/models/lights/globe_winter.tga
		rgbgen lightingSpherical
	} 
        {
		map textures/models/lights/globe_winter.tga
		blendfunc blend
                rgbGen identity
	}
}

static_g_post_winter
{
	qer_editorimage textures/models/lights/g_post_winter.tga
	{
		map textures/models/lights/g_post_winter.tga
		rgbgen lightingSpherical
	}
}


// Hanging wall lamp
hanger
{
	qer_editorimage textures/models/lights/hanger.tga
	{
		map textures/models/lights/hanger.tga
		rgbGen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

lantern
{
	qer_editorimage textures/models/lights/lantern.tga
	{
		map textures/models/lights/lantern.tga
		rgbGen lightingSpherical
	}
}

h_cross
{
	cull none
	qer_editorimage textures/models/lights/hanger.tga
	{
		map textures/models/lights/hanger.tga
		rgbGen lightingSpherical
	}
}

static_hanger
{
	qer_editorimage textures/models/lights/hanger.tga
	{
		map textures/models/lights/hanger.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

static_h_cross
{
	cull none
	qer_editorimage textures/models/lights/hanger.tga
	{
		map textures/models/lights/hanger.tga
		rgbgen lightingSpherical
	}
}


// Hanging wall lamp winter
hanger_snow
{
	qer_editorimage textures/models/lights/hanger_snow.tga
	{
		map textures/models/lights/hanger_snow.tga
		rgbGen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

lantern_snow
{
	qer_editorimage textures/models/lights/lantern_snow.tga
	{
		map textures/models/lights/lantern_snow.tga
		rgbGen lightingSpherical
	}
}

h_cross_snow
{
	cull none
	qer_editorimage textures/models/lights/hanger_snow.tga
	{
		map textures/models/lights/hanger_snow.tga
		rgbGen lightingSpherical
	}
}

static_hanger_snow
{
	qer_editorimage textures/models/lights/hanger_snow.tga
	{
		map textures/models/lights/hanger_snow.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

static_lantern_snow
{
	qer_editorimage textures/models/lights/lantern_snow.tga

	{
		map textures/models/lights/lantern_snow.tga
		rgbgen lightingSpherical
	}
        {
		map textures/models/lights/lantern_snow.tga
                blendfunc blend
		rgbGen identity
	}
}

static_h_cross_snow
{
	cull none
	qer_editorimage textures/models/lights/hanger_snow.tga
	{
		map textures/models/lights/hanger_snow.tga
		rgbgen lightingSpherical
	}
}


// Tri pole light
support
{
	qer_editorimage textures/models/lights/support.tga
	{
		map textures/models/lights/support.tga
		rgbGen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

t_post
{
	qer_editorimage textures/models/lights/t_post.tga
	{
		map textures/models/lights/t_post.tga
		rgbGen lightingSpherical
	}
}

s_cross
{
	cull none
	qer_editorimage textures/models/lights/support.tga
	{
		map textures/models/lights/support.tga
		rgbGen lightingSpherical
	}
}

// Tri pole light winter
support_snow
{
	qer_editorimage textures/models/lights/support_snow.tga
	{
		map textures/models/lights/support_snow.tga
		rgbGen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

t_post_winter
{
	qer_editorimage textures/models/lights/t_post_winter.tga
	{
		map textures/models/lights/t_post_winter.tga
		rgbGen lightingSpherical
	}
}

s_cross_winter
{
	qer_editorimage textures/models/lights/support_snow.tga
	cull none
	{
		map textures/models/lights/support_snow.tga
		rgbGen lightingSpherical
	}
}

static_support_snow
{
	qer_editorimage textures/models/lights/support_snow.tga
	{
		map textures/models/lights/support_snow.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

static_t_post_winter
{
	qer_editorimage textures/models/lights/t_post_winter.tga
	{
		map textures/models/lights/t_post_winter.tga
		rgbgen lightingSpherical
	}
}

static_s_cross_winter
{
	qer_editorimage textures/models/lights/support_snow.tga
	cull none
	{
		map textures/models/lights/support_snow.tga
		rgbgen lightingSpherical
	}
}



// Lightbulb

//bulb
//{
//	qer_editorimage textures/models/lights/bulb.tga
//	{
//		map textures/models/lights/bulb.tga
//		rgbgen lightingSpherical
//	}
//        {
//		map textures/models/lights/bulb.tga
//		blendfunc blend
//                rgbGen identity
//	}
//}

static_bulb
{
	qer_editorimage textures/models/lights/bulb.tga
	{
		map textures/models/lights/bulb.tga
		rgbgen lightingSpherical
		depthwrite
	} 
      {
		map textures/models/lights/bulb.tga
		blendfunc blend
                rgbGen identity
	}
}

bulb
{
	qer_editorimage textures/models/lights/bulb.tga
	{
		map textures/models/lights/bulb.tga
//		rgbGen spherical
		rgbGen lightingspherical  //Radek
		depthwrite
	} 
      {
		map textures/models/lights/bulb.tga
		blendfunc blend
                rgbGen identity
	}
}


// Lightbulb cover
bulbcover
{
	qer_editorimage textures/models/lights/bulbcover.tga
	cull none
	{
		map textures/models/lights/bulbcover.tga
		rgbGen lightingGrid
	}
}

static_bulbcover
{
	qer_editorimage textures/models/lights/bulbcover.tga
	cull none
	{
		map textures/models/lights/bulbcover.tga
		rgbgen lightingSpherical
	}
}


// Moroccan lamp
desertlamp
{
	qer_editorimage textures/models/lights/desertlamp.tga
	cull none
	{
		map textures/models/lights/desertlamp.tga
		rgbGen lightingGrid
		alphafunc ge128
		depthwrite
	}
}

static_desertlamp
{
	qer_editorimage textures/models/lights/desertlamp.tga
	cull none
	{
		map textures/models/lights/desertlamp.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
        {
		map textures/models/lights/desertlamp.tga
		blendfunc blend
                rgbGen identity
	}
}

cagelight
{
	qer_editorimage textures/models/lights/cagelight.tga
	cull none
	{
		map textures/models/lights/cagelight.tga
		rgbGen lightingGrid
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight.tga
		blendfunc blend
                rgbGen identity
	}
} 
cagelight-red
{
	qer_editorimage textures/models/lights/cagelight-red.tga
	cull none
	{
		map textures/models/lights/cagelight-red.tga
		//rgbGen lightingGrid
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight-red.tga
		blendfunc blend
                rgbGen lightingSpherical
	}
} 
cagelight-red-lit
{
	qer_editorimage textures/models/lights/cagelight-red-lit.tga
	cull none
	{
		map textures/models/lights/cagelight-red-lit.tga
		rgbGen lightingGrid
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight-red-lit.tga
		blendfunc blend
                rgbGen identity
	}
} 
cagelight-green
{
	qer_editorimage textures/models/lights/cagelight-green.tga
	cull none
	{
		map textures/models/lights/cagelight-green.tga
		rgbGen lightingGrid
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight-green.tga
		blendfunc blend
                rgbGen identity
	}
} 
static_cagelight
{
	qer_editorimage textures/models/lights/cagelight.tga
	cull none
	{
		map textures/models/lights/cagelight.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight.tga
		blendfunc blend
                rgbGen identity
	}
} 

static_cagelight_red
{
	qer_editorimage textures/models/lights/cagelight_red.tga
	cull none
	{
		map textures/models/lights/cagelight_red.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
	{
        	map textures/models/lights/cagelight_red.tga
		blendfunc blend
                rgbGen identity
	}
} 

// Lightbulb cover
static_bulbcover_one
{
	qer_editorimage textures/models/lights/bulbcover_one.tga
	{
		map textures/models/lights/bulbcover_one.tga
		rgbgen lightingSpherical
	}
}
static_bulbcover_two
{
	qer_editorimage textures/models/lights/bulbcover_two.tga
	{
		map textures/models/lights/bulbcover_two.tga
		rgbGen identity
	}
}

//German lantern
static_glantern
{
	qer_editorimage textures/models/lights/glantern.tga
	{
		map textures/models/lights/glantern.tga
		rgbgen lightingSpherical
	}
}
static_glanterncull
{
	qer_editorimage textures/models/lights/glantern.tga
	cull none
	{
		map textures/models/lights/glantern.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
} 

//German lantern
glantern
{
	qer_editorimage textures/models/lights/glantern.tga
	{
		map textures/models/lights/glantern.tga
		rgbGen lightingspherical
	}
}
glanternglass
{
	qer_editorimage textures/models/lights/glantern.tga
	{
		map textures/models/lights/glantern.tga
		rgbGen identity
		alphagen oneminusdot 0.2 1.2
		blendfunc blend
	}
}
glanterncull
{
	qer_editorimage textures/models/lights/glantern.tga
	cull none
	{
		map textures/models/lights/glantern.tga
		rgbGen lightingspherical
		alphafunc ge128
		depthwrite
	}
} 

///////////////////////////////////////////////////////////////justin///////////////////
shattered_globe
{
	qer_editorimage textures/models/lights/globe_shattered.tga
	{
		map textures/models/lights/globe_shattered.tga
                blendFunc blend
		rgbGen lightingSpherical
	}
} 

broken_lantern
{
	qer_editorimage textures/models/lights/lantern_broken.tga
	{
		map textures/models/lights/lantern_broken.tga
                blendFunc blend
		rgbGen lightingSpherical
	}
} 

////////////////////////////////////////////
//lit warehouse windows
////////////////////////////////////////////

textures/light/warewindowlit1
{
	qer_editorimage textures/light/warewindowlit1.tga
	surfaceparm nomarks
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/warewindowlit1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/warewindowlit1_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/warewindowlit2
{
	qer_editorimage textures/light/warewindowlit2.tga
	surfaceparm nomarks
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/warewindowlit2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/warewindowlit2_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/light/warewindowlit_nolight
{
	qer_editorimage textures/light/warewindowlit2.tga
	surfaceparm nomarks
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/warewindowlit2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/warewindowlit2_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/warewindowlit3
{
	qer_editorimage textures/light/warewindowlit3.tga
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/light/warewindowlit3.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/light/warewindowlit3_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/light/ambientwhite
{
	qer_editorimage textures/common/white.tga
	surfaceparm nodamage
	qer_keyword utility
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm nomarks
	surfaceparm trans
	q3map_surfacelight 2000
	{
		map $whiteimage
		rgbgen lightingSpherical
	}
}

static_lantern
{
	qer_editorimage textures/models/lights/lantern.tga
	{
		map textures/models/lights/lantern.tga
		rgbGen lightingSpherical
		
	}
        {
		map textures/models/lights/lantern.tga
		blendfunc blend
              rgbGen identity
	}
}

static_support
{
	qer_editorimage textures/models/lights/support.tga
	{
		map textures/models/lights/support.tga
		rgbgen lightingSpherical
		alphafunc ge128
		depthwrite
	}
}

static_s_cross
{
	cull none
	qer_editorimage textures/models/lights/support.tga
	{
		map textures/models/lights/support.tga
		rgbgen lightingSpherical
	}
}

static_t_post
{
	qer_editorimage textures/models/lights/t_post.tga
	{
		map textures/models/lights/t_post.tga
		rgbgen lightingSpherical
	}
}

bulbcover_three
{
	qer_editorimage textures/models/lights/bulbcover_three.tga
	{
		map textures/models/lights/bulbcover_three.tga
		rgbgen lightingSpherical
	}
}
bulbcover_four
{
        cull none
	qer_editorimage textures/models/lights/bulbcover_four.tga
	{
		map textures/models/lights/bulbcover_four.tga
		rgbgen lightingSpherical
	}
}

Candleholder
{
	qer_editorimage textures/models/lights/Candleholder/Candleholder.tga
	surfaceparm nomarks
	{
		map $lightmap
	}
	{
		map textures/models/lights/Candleholder/Candleholder.tga
		rgbGen vertex
	}
	{
		map textures/models/lights/Candleholder/Candleholder_glow.tga
		blendFunc add
	}
}


Candle_Standing
{
	qer_editorimage textures/models/lights/Candle_Standing/Candle_Standing.tga
	{
		map $lightmap
	}
	{
		map textures/models/lights/Candle_Standing/Candle_Standing.tga
		rgbGen vertex
	}
	{
		map textures/models/lights/Candle_Standing/Candle_glow.tga
		blendFunc add
	}
}

Candle_Standing_T
{
	qer_editorimage textures/models/lights/Candle_Standing/Candle_Standing_T.tga
	{
		map $lightmap
	}
	{
		map textures/models/lights/Candle_Standing/Candle_Standing_T.tga
		rgbGen vertex
	}
	{
		map textures/models/lights/Candle_Standing/Candle_Tglow.tga
		blendFunc add
	}
}

Candle_Standing_S
{
	qer_editorimage textures/models/lights/Candle_Standing/Candle_Standing_S.tga
	{
		map $lightmap
	}
	{
		map textures/models/lights/Candle_Standing/Candle_Standing_S.tga
		rgbgen vertex
	}
	{
		map textures/models/lights/Candle_Standing/Candle_Sglow.tga
		blendFunc add
	}
}

Spilled_Wax
{
	qer_editorimage textures/models/lights/Candle_Standing/Spilled_Wax.tga
	{
		map textures/models/lights/Candle_Standing/Spilled_Wax.tga
		rgbgen lightingSpherical
		alphafunc ge128
	}
}

Candle_Fell
{
	qer_editorimage textures/models/lights/Candle_Standing/Candle_Fell.tga
	{
		map textures/models/lights/Candle_Standing/Candle_Fell.tga
		rgbgen lightingSpherical
	}
}

WallLight
{
	cull none
	qer_editorimage textures/models/lights/WallLight/WallLight.tga
	{
		map textures/models/lights/WallLight/WallLight.tga
		rgbgen lightingSpherical
	}
}

StandingLight
{
	qer_editorimage textures/models/lights/StandingLight/StandingLight.tga
	q3map_lightimage textures/models/lights/StandingLight/StandingLight_glow.tga
	surfaceparm nomarks
	q3map_surfaceLight 10000
	light 1
	{
		map $lightmap
	}
	{
		map textures/models/lights/StandingLight/StandingLight.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen vertex
	}
	{
 		map textures/models/lights/StandingLight/StandingLight_glow.tga
		blendFunc add
	}
}

StandingLight_Off
{
	qer_editorimage textures/models/lights/StandingLight/StandingLight_Off.tga
	{
		map textures/models/lights/StandingLight/StandingLight_Off.tga
		rgbgen lightingSpherical
	}
}


textures/standinglightemitter
{
	qer_editorimage textures/models/lights/StandingLight/StandingLight.tga
	surfaceparm nonsolid
//	surfaceparm nodraw
//	(this will turnoff the surface lights)
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfaceLight 10000
}

