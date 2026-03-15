
textures/items_various/USCrate_01_
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_01_.tga
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

textures/items_various/USCrate_01_a
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_01_a.tga
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

textures/items_various/USCrate_01_b
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_01_b.tga
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

textures/items_various/USCrate_02_
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_02_.tga
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

textures/items_various/USCrate_02_a
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_02_a.tga
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

textures/items_various/USCrate_02_b
{
	qer_keyword wood
	qer_keyword ammo
	qer_keyword crate
	surfaceparm wood
	{
		map textures/items_various/USCrate_02_b.tga
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
/////////////////////tentcanvas
textures/items_various/it_e_2-1canvasUS01
{
	qer_keyword tent
	qer_keyword canvas
	surfaceparm carpet
	{
		map textures/items_various/it_e_2-1canvasUS01.tga
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

textures/items_various/sidney
{
	qer_editorimage textures/items_various/frame_sidney.tga
	
	{
		map textures/items_various/frame_sidney.tga
		///blendfunc blend
		///alphagen vertex
		///rgbgen vertex
		alphaFunc GE128
		depthWrite nextbundle
		map $lightmap
	}
} 

v2_new
{
	qer_editorimage textures/items_various/v2_new.tga
	{
		map textures/items_various/v2_new.tga
		//rgbGen static
		rgbgen lightingSpherical
	}
}