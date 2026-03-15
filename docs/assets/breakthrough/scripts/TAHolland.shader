textures/TAHolland/T1L1_road_ns_half
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm nonsolid
	qer_editorimage textures/wilderness/m3l3grass_set2.tga
	{
		map textures/wilderness/m3l3grass_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/TAHolland/T1L1_road_ns_trans
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm nonsolid
	qer_editorimage textures/wilderness/m3l3grass_bocroadt.tga
	{
		map textures/wilderness/m3l3grass_bocroadt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/TAHolland/T1L1_road_ns
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm nonsolid
	qer_editorimage textures/wilderness/m3l3grass_bocroad.tga
	{
		map textures/wilderness/m3l3grass_bocroad.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/TAHolland/fueltank2
{	
	qer_keyword Holland
	surfaceparm metal
	{
		map textures/TAHolland/fueltank2.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAHolland/greenwall_brickpitd
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/greenwall_brickpitd.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAHolland/hollbrick_flat1
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/hollbrick_flat1.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAHolland/hollbrick_flat2pitd
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/hollbrick_flat2pitd.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAHolland/whitewall_brick
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/whitewall_brick.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAHolland/pitted_brick
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/pitted_brick.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollwindow_dbl2
{
	qer_keyword Holland
	surfaceparm glass
	{
		map textures/TAHolland/hollwindow_dbl2.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollwindow_dbl1a
{
	qer_keyword Holland
	surfaceparm glass
	{
		map textures/TAHolland/hollwindow_dbl1a.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollwindow_f1
{
	qer_keyword Holland
	surfaceparm glass
	{
		map textures/TAHolland/hollwindow_f1.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/holldoor_1
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/TAHolland/holldoor_1.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollgardoor
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/TAHolland/hollgardoor.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollandroof
{
	qer_keyword Holland
	surfaceparm rock
	{
		map textures/TAHolland/hollandroof.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollwin_study
{
	qer_keyword Holland
	surfaceparm glass
	{
		map textures/TAHolland/hollwin_study.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/holl_road1
{
	qer_keyword Holland
	qer_keyword road
	surfaceparm rock
	{
		map textures/TAHolland/holl_road1.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/holl_walk1
{
	qer_keyword Holland
	qer_keyword road
	surfaceparm rock
	{
		map textures/TAHolland/holl_walk1.tga
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/holl_fence
{
	qer_keyword holland
	qer_keyword fence
	surfaceparm wood
	surfaceparm fence
	qer_editorimage textures/TAHolland/holl_fence.tga
	nomipmaps
	nopicmip
	cull none
	{
		ifCvar r_blendtrees 0
		map textures/TAHolland/holl_fence.tga
		depthwrite
		alphafunc GE128
	nextbundle
		map $lightmap
	}
	{
		ifCvarNot r_blendtrees 0
		map textures/TAHolland/holl_fence.tga
		depthwrite
		nocolorwrite
		alphaFunc GE_FOLIAGE1
	}
	{
		ifCvarNot r_blendtrees 0
		map textures/TAHolland/holl_fence.tga
		alphaFunc GT0
		blendfunc blend
	nextbundle
		map $lightmap
	}
}

textures/TAHolland/porchpanel_wht
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/TAHolland/porchpanel_wht.jpg
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/beverol
{
	qer_keyword Holland
	qer_keyword sign
	surfaceparm wood
	{
		map textures/TAHolland/beverol.jpg
	nextbundle
		map $lightmap
	}
}  

textures/TAHolland/hollwindow_f1night
{
	surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 100
	qer_keyword TA
	qer_keyword window
//	qer_keyword light
	qer_editorimage textures/TAHolland/hollwindow_f1lit.tga
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/TAHolland/hollwindow_f1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/TAHolland/hollwindow_f1.blend.tga
		blendfunc GL_ONE GL_ONE
	}
}  

textures/TAHolland/railway_road
{
	qer_keyword holland
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	qer_keyword TA
	surfaceparm dirt
	{
		map textures/TAHolland/railway_road.jpg
	nextbundle
		map $lightmap
	}
}  

textures/taholland/interiorwall_set2wtrim_A
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/taholland/interiorwall_set2wtrim_A.tga
	nextbundle
		map $lightmap
	}
}

textures/taholland/interiorwall_set3wtrim_A
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/taholland/interiorwall_set3wtrim_A.tga
	nextbundle
		map $lightmap
	}
}

textures/taholland/interiorwall_set2_A
{
	qer_keyword Holland
	surfaceparm wood
	{
		map textures/taholland/interiorwall_set2_A.tga
	nextbundle
		map $lightmap
	}
}
/////////////////////////// 
 
