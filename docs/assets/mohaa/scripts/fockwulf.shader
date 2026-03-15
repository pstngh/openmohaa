// Fockwulf airplane
fockwulf
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwulf.tga
	{
		map textures/models/vehicles/fockwulf/fockwulf.tga
		rgbGen lightingSpherical
	}
}

///destroyed fockwulf

fockwulf_d
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwulf_d.tga
	{
		map textures/models/vehicles/fockwulf/fockwulf_d.tga
		rgbGen lightingSpherical
	}
}

fockwindow_d
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwindow_d.tga
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
		map textures/models/vehicles/fockwulf/fockwindow_d.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}
// Fockwulf airplane in flight
fockwulffly
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwulffly.tga
	{
		map textures/models/vehicles/fockwulf/fockwulffly.tga
		rgbGen lightingSpherical
	}
}

// Fockwulf airplane in flight window
fockwindowfly
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwindow_flat.tga
	{
		map textures/models/vehicles/fockwulf/fockwindow_flat.tga
		rgbGen lightingSpherical
	}
}

// Fockwulf window
fockwindow
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwindow.tga
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
		map textures/models/vehicles/fockwulf/fockwindow.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}
//propellar
prop
{
	qer_editorimage textures/models/vehicles/fockwulf/prop.tga
	cull none
//	{
//		//map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		//alphaGen const 0.3
//		blendFunc blend
//	}
	{
		clampmap textures/models/vehicles/fockwulf/prop.tga
//		rgbGen lightingSpherical
		tcmod rotate 5000
		blendFunc blend
	}
}
// Fockwulf airplane
static_fockwulf
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwulf.tga
	{
		map textures/models/vehicles/fockwulf/fockwulf.tga
		rgbGen vertex
	}
}

// Fockwulf window
static_fockwindow
{
	qer_editorimage textures/models/vehicles/fockwulf/fockwindow.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		//alphaGen const 0.1
		//blendFunc add
		Blendfunc blend
	}
	{
		map textures/models/vehicles/fockwulf/fockwindow.tga
		rgbGen vertex
		blendFunc blend
	}
}
//propellar
static_prop
{
	qer_editorimage textures/models/vehicles/fockwulf/prop.tga
	cull none
//	{
//		//map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		//alphaGen const 0.3
//		blendFunc blend
//	}
	{
		clampmap textures/models/vehicles/fockwulf/prop.tga
//		rgbGen lightingSpherical
		tcmod rotate 30
		blendFunc blend
	}
}

