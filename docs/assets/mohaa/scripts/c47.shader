// C47
c47
{
	qer_editorimage textures/models/vehicles/c47/c47.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/vehicles/c47/c47.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

static_c47
{
	qer_editorimage textures/models/vehicles/c47/c47.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/vehicles/c47/c47.tga
		rgbGen vertex
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

c47prop
{
	qer_editorimage textures/models/vehicles/c47/c47prop.tga
	cull none
	{
		clampmap textures/models/vehicles/c47/c47prop.tga
		tcmod rotate 5000
		blendFunc blend
	}
}

c47fly
{
	qer_editorimage textures/models/vehicles/c47/c47fly.tga
	{
		map textures/models/vehicles/c47/c47fly.tga
		rgbGen lightingSpherical
	}
}
