
textures/misc_outside/nu_grass_set2a_spns
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	qer_editorimage textures/misc_outside/nu_grass_set2a_spns.tga
	surfaceparm nonsolid
	surfaceparm trans
	{
		map textures/misc_outside/nu_grass_set2a_sp.tga
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
textures/misc_outside/nu_grass_set2a_sp_noshd
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	surfaceparm trans
	qer_editorimage textures/misc_outside/nu_grass_set2a_sp_noshd.tga
	{
		map textures/misc_outside/nu_grass_set2a_sp.tga
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
textures/misc_outside/bocroad_fullns
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
//	surfaceparm dirt
	surfaceparm nonsolid
	surfaceparm trans
	qer_editorimage textures/misc_outside/bocroad_full.tga
	{
		map textures/misc_outside/bocroad_full.tga
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
textures/misc_outside/bocroad_fullns_noshd
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
//	surfaceparm dirt
	surfaceparm nonsolid
	surfaceParm trans
	qer_editorimage textures/misc_outside/bocroad_fullns_noshd.tga
	{
		map textures/misc_outside/bocroad_full.tga
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

textures/misc_outside/rock_cliffside_trans_noshd
{
	qer_keyword terrain
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	surfaceparm trans
	qer_editorimage textures/misc_outside/rock_cliffside_trans_noshd.tga
	{
		map textures/misc_outside/rock_cliffside_trans.tga
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
//noclampmap no clip version of the above
textures/blasted/grassbrdr_set4_blastnclmpns
{
	qer_keyword terrain
	qer_keyword m3
	qer_keyword dirt
	surfaceParm nonsolid
	surfaceparm trans
	qer_editorimage textures/blasted/grassbrdr_set4_blastns.tga
	{
		map textures/blasted/grassbrdr_set4_blast.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 
textures/misc_outside/streetgravelns_noshd
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	surfaceparm trans
	qer_editorimage textures/misc_outside/streetgravelns_noshd.tga

	{
	    map textures/misc_outside/streetgravel.tga
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

// Can be placed in levels as a normal texture
textures/models/items/rubblebase_noshd
{
	surfaceparm trans
	qer_editorimage textures/models/items/rubblebase.tga
	{
		map textures/models/items/rubblebase.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

textures/misc_outside/bocroad_fulladamns_noshd
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	surfaceparm nonsolid
	surfaceparm trans
	qer_editorimage textures/misc_outside/bocroad_fulladam.tga  
	{
	    map textures/misc_outside/bocroad_fulladam.tga
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

 
