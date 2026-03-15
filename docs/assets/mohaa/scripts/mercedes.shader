// Mercedes
mercedes
{
	qer_editorimage textures/models/vehicles/mercedes/mercedes.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcGen environmentmodel
	}
	{
		map textures/models/vehicles/mercedes/mercedes.tga
		rgbGen lightingSpherical
		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}
mercedestread
{
	qer_editorimage textures/models/vehicles/mercedes/mtread.tga
	{
		map textures/models/vehicles/mercedes/mtread.tga
		rgbGen lightingSpherical
	}
}
mercedeshub
{
	qer_editorimage textures/models/vehicles/mercedes/mhub.tga
	{
		map textures/models/vehicles/mercedes/mhub.tga
		rgbGen lightingSpherical
	}
}
mercedes_window
{
	qer_editorimage textures/models/vehicles/mercedes/mercedesw.tga
	cull none
	{
		map textures/models/vehicles/mercedes/mercedesw.tga
		rgbGen lightingSpherical
		blendFunc blend
		depthWrite
	}
}

static_mercedes
{
	qer_editorimage textures/models/vehicles/mercedes/mercedes.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcGen environmentmodel
	}
	{
		map textures/models/vehicles/mercedes/mercedes.tga
		rgbGen vertex
		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}
static_mercedes_window
{
	qer_editorimage textures/models/vehicles/mercedes/mercedesw.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/mercedes/mercedesw.tga
		rgbGen vertex
		blendFunc blend
	}
}
static_mercedestread
{
	qer_editorimage textures/models/vehicles/mercedes/mtread.tga
	{
		map textures/models/vehicles/mercedes/mtread.tga
		rgbGen vertex
	}
}
static_mercedeshub
{
	qer_editorimage textures/models/vehicles/mercedes/mhub.tga
	{
		map textures/models/vehicles/mercedes/mhub.tga
		rgbGen vertex
	}
}
