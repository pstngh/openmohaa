tracer
{
    surfaceparm nolightmap
	cull none
	q3map_surfacelight 2000
	{
		map textures/beams/tracer.tga
		nofog
		alphaGen vertex
		rgbGen vertex
		//tcmod scroll 5 0
		tcmod scroll 10 0
		nextbundle
		clampmap textures/beams/tracergradient.tga
		//tcmod scroll 5 0
		blendFunc add
	}
}	

// FAKK2 stuff that should be removed
emap1
{
	{
	map textures/sprites/emap1.tga
	blendFunc GL_ONE GL_ONE
	rgbGen vertex
	}
}

beam_darkblue
{
	cull none
	{
		map textures/sprites/beam_blue.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

beam_white
{
	nomipmaps
	{
		map textures/sprites/beam_white2.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod scroll -2 0
	}
}

beam_darkblue2
{
	cull none
	{
		map textures/sprites/beam_blue3.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
//		tcMod scroll 2 0
	}
}

beam_green
{
	cull none
	{
		map textures/sprites/beam_green.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		tcMod scroll -3 0
	}
}