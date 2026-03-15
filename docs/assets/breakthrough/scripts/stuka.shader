// Stuka airplane
stuka
{
	qer_editorimage textures/models/vehicles/stuka/stuka.tga
	{
		map textures/models/vehicles/stuka/stuka.tga
		rgbGen lightingSpherical
	}
}

// Stuka window
stukawindow
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow.tga
		rgbGen lightingSpherical
		blendFunc blend
		depthWrite
	}
}

static_stuka
{
	qer_editorimage textures/models/vehicles/stuka/stuka.tga
	{
	map textures/models/vehicles/stuka/stuka.tga
		rgbGen lightingSpherical
	}
}

static_stukawindow
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow.tga
		rgbGen vertex
		blendFunc blend
		depthWrite

	}
}
// Stuka airplane desert camoflauge
stuka_desert
{
	qer_editorimage textures/models/vehicles/stuka/stuka_desert.tga
	{
		map textures/models/vehicles/stuka/stuka_desert.tga
		rgbGen lightingSpherical
	}
}

// Stuka window desert camo
stukawindow_desert
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow_desert.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow_desert.tga
		rgbGen lightingSpherical
		blendFunc blend
		depthWrite
	}
}

// Stuka airplane desert camoflauge
static_stuka_desert
{
	qer_editorimage textures/models/vehicles/stuka/stuka_desert.tga
	{
	map textures/models/vehicles/stuka/stuka_desert.tga
		rgbGen lightingSpherical
	}
}

// Stuka window desert camo
static_stukawindow_desert
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow_desert.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow_desert.tga
		rgbGen lightingSpherical
		blendFunc blend
		depthWrite
	}
}
////////////////////////////////////////////////////////////////////
//destroyed stukas
////////////////////////////////////////////////////////////////////

stukawindow_d
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow_d.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow_d.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
		blendFunc blend
		depthWrite
	}
}
stuka_d
{
	qer_editorimage textures/models/vehicles/stuka/stuka_d.tga
	{
		map textures/models/vehicles/stuka/stuka_d.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
stuka_dcull
{
	qer_editorimage textures/models/vehicles/stuka/stuka_d.tga
	cull none
	{
		map textures/models/vehicles/stuka/stuka_d.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
stukawindow_d_d 
{
	qer_editorimage textures/models/vehicles/stuka/stukawindow_d_d.tga
	cull none
	{
		map textures/models/vehicles/stuka/stukawindow_d_d.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
		blendFunc blend
		depthWrite
	}
}

stukadesrt_d
{
	qer_editorimage textures/models/vehicles/stuka/stuka_desert_d.tga
	{
		map textures/models/vehicles/stuka/stuka_desert_d.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
stukadesrt_dcull
{
	qer_editorimage textures/models/vehicles/stuka/stuka_desert_d.tga
	cull none
	{
		map textures/models/vehicles/stuka/stuka_desert_d.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}

//new stuka skin... added by Caiphus
It_V_stuka
{
	qer_editorimage textures/models/vehicles/It_V_stuka/It_V_stuka.tga
	{
		map textures/models/vehicles/It_V_stuka/It_V_stuka.tga
		rgbGen lightingSpherical
	}
}

It_V_stuka_d
{
	qer_editorimage textures/models/vehicles/It_V_stuka/It_V_stuka_d.tga
	{
		map textures/models/vehicles/It_V_stuka/It_V_stuka_d.tga
		rgbGen lightingSpherical
	}
}

It_V_stukaWin
{
	qer_editorimage textures/models/vehicles/It_V_stuka/It_V_stukaWin
	{
		map textures/models/vehicles/It_V_stuka/It_V_stukaWin.tga
		rgbGen lightingSpherical
	}
}
