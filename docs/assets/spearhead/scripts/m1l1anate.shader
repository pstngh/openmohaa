textures/mohtest/omaha_set3_noshd
{
	qer_keyword m1
	qer_keyword sand
	qer_keyword flat
	qer_keyword floor
	surfaceparm dirt
	surfaceparm trans
	qer_editorimage textures/mohtest/omaha_set3_noshded.tga

	{
		map textures/mohtest/omaha_set3.tga
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
textures/algiers/grndset_2af_noshd
{
	qer_keyword gravel
	qer_keyword natural
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	surfaceparm trans
	qer_editorimage textures/algiers/grndset_2af_noshded.tga
	{
		map textures/algiers/grndset_2af.tga
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



textures/mohtest/omaha_set3ns_noshd
{
	qer_keyword m1
	qer_keyword sand
	qer_keyword flat
	qer_keyword floor
	surfaceparm trans
	surfaceparm nonsolid
	qer_editorimage textures/mohtest/omaha_set3ns_noshded.tga

	{
		map textures/mohtest/omaha_set3.tga
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
textures/algiers/grndset_2afns_noshd
{
	qer_keyword gravel
	qer_keyword natural
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	surfaceparm trans
	surfaceparm nonsolid
	qer_editorimage textures/algiers/grndset_2afns_noshded.tga
	{
		map textures/algiers/grndset_2af.tga
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

textures/algiers/cliffset2_sandtop2small_noshd
{
	qer_editorimage textures/algiers/cliffset2_sandtop2small_noshded.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm trans
	surfaceparm stone
	{
		map textures/algiers/cliffset2_sandtop2small.tga
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

textures/algiers/cliffset2_noshd
{
	qer_editorimage textures/algiers/cliffset2_noshded.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm trans
	surfaceparm stone
	{
		map textures/algiers/cliffset2.tga
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

textures/algiers/cliffset2nonDDStemp
{
	qer_editorimage textures/algiers/cliffset2nonDDStemp.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm stone
	{
		map textures/algiers/cliffset2nonDDStemp.tga
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
textures/algiers/cliffset2nonddstemp_noshd
{
	qer_editorimage textures/algiers/cliffset2_noshded.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm trans
	surfaceparm stone
	{
		map textures/algiers/cliffset2nonDDStemp.tga
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
textures/algiers/cliffset_1_noshd
{
	qer_editorimage textures/algiers/cliffset_1_noshd.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm trans
	surfaceparm stone
	{
		map textures/algiers/cliffset_1.tga
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


textures/algiers/desertrd_1ns
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm nonsolid        
	surfaceparm translucent
	qer_editorimage textures/algiers/desertrd_1ed.tga
	{
		map textures/algiers/desertrd_1.tga
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

textures/algiers/desertrd_512ns
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm nonsolid        
	surfaceparm translucent
	qer_editorimage textures/algiers/desertrd_512nsed.tga
	{
		map textures/algiers/desertrd_512.tga
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

