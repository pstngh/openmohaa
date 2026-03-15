// p47 airplane
p47
{
	qer_editorimage textures/models/vehicles/p47/p47.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/vehicles/p47/p47.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

// p47 airplane in flight
p47fly
{
	qer_editorimage textures/models/vehicles/p47/p47fly.tga
	{
		map textures/models/vehicles/p47/p47fly.tga
		rgbGen lightingSpherical
	}
}

// p47 airplane in flight window
p47flywindow
{
	qer_editorimage textures/models/vehicles/p47/p47window_flat.tga
	{
		map textures/models/vehicles/p47/p47window_flat.tga
		rgbGen lightingSpherical
	}
}

// p47 window
p47window
{
	qer_editorimage textures/models/vehicles/p47/p47window.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		//alphaGen const 0.1
		//blendFunc add
		Blendfunc blend
	}
	{
		map textures/models/vehicles/p47/p47window.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

// p47 airplane
static_p47
{
	qer_editorimage textures/models/vehicles/p47/p47.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/vehicles/p47/p47.tga
		rgbGen vertex
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

// p47 window
static_p47window
{
	qer_editorimage textures/models/vehicles/p47/p47window.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		//alphaGen const 0.1
		//blendFunc add
		Blendfunc blend
	}
	{
		map textures/models/vehicles/p47/p47window.tga
		rgbGen vertex
		blendFunc blend
	}
}
