jeep
{
	qer_editorimage textures/models/vehicles/jeep/jeep.tga
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeep.tga
		rgbGen lightingSpherical
	}
}
jeeptredz
{
	qer_editorimage textures/models/vehicles/jeep/jeeptredz.tga
	noMerge
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeeptredz.tga
		//rgbGen lightingSpherical
		tcmod scroll fromEntity 0
	}
}
jeepwheelz
{
	qer_editorimage textures/models/vehicles/jeep/jeepwheelz.tga
	noMerge
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeepwheelz.tga
		tcmod rotate fromEntity 0 2
		// fromEntity
		//rgbGen lightingSpherical
	}
}
jeepwindow
{
	qer_editorimage textures/models/vehicles/jeep/jeepwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/jeep/jeepwindow.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

static_jeep
{
	qer_editorimage textures/models/vehicles/jeep/jeep.tga
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeep.tga
		rgbGen static
	}
}
static_jeeptredz
{
	qer_editorimage textures/models/vehicles/jeep/jeeptredz.tga
	noMerge
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeeptredz.tga
		//rgbGen lightingSpherical
		tcmod scroll fromEntity 0
	}
}
static_jeepwheelz
{
	qer_editorimage textures/models/vehicles/jeep/jeepwheelz.tga
	noMerge
	cull none	// Makes it double-sided, not sure if this is needed.
	{
		map textures/models/vehicles/jeep/jeepwheelz.tga
		tcmod rotate fromEntity 0 2
		// fromEntity
		//rgbGen lightingSpherical
	}
}
static_jeepwindow
{
	qer_editorimage textures/models/vehicles/jeep/jeepwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen static
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/jeep/jeepwindow.tga
		rgbGen static
		blendFunc blend
	}
}