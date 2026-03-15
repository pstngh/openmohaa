// Civilian Car
car
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car1.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car1.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Static Civilian Car
static_car
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car1.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car1.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Civilian Car 2
car2
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car2.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car2.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Static Civilian Car 2
static_car2
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car2.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car2.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Civilian Car 3
car3
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car3.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car3.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Static Civilian Car 3
static_car3
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/car3.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/car/car3.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

// Civilian Car Window
carwindow
{
	qer_editorimage textures/models/vehicles/car/carwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carwindow.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

// Static Civilian Car Window
static_carwindow
{
	qer_editorimage textures/models/vehicles/car/carwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carwindow.tga
		rgbGen vertex
		blendFunc blend
	}
}

// Rusted Civilian Car
carrusted
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/carrusted.tga
	{
		map textures/models/VEHICLES/car/carrusted.tga
		rgbGen lightingSpherical
	}
}

// Static Rusted Civilian Car
static_carrusted
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/carrusted.tga
	{
		map textures/models/VEHICLES/car/carrusted.tga
		rgbGen vertex
	}
}

// Rusted Civilian Car Window
carwindowR
{
	qer_editorimage textures/models/vehicles/car/carWindowR.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carWindowR.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

// Static Rusted Civilian Car Window
static_carwindowR
{
	qer_editorimage textures/models/vehicles/car/carWindowR.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carWindowR.tga
		rgbGen vertex
		blendFunc blend
	}
}

// Burned Civilian Car
carburnt
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/carBurnt.tga
	{
		map textures/models/VEHICLES/car/carBurnt.tga
		rgbGen lightingSpherical
	}
}

// Static Burned Civilian Car
static_carburnt
{
	cull none
	qer_editorimage textures/models/VEHICLES/car/carBurnt.tga
	{
		map textures/models/VEHICLES/car/carBurnt.tga
		rgbGen vertex
	}
}

// Burned Civilian Car Window
carwindowB
{
	qer_editorimage textures/models/vehicles/car/carWindowB.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carWindowB.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

// Static Burned Civilian Car Window
static_carwindowB
{
	qer_editorimage textures/models/vehicles/car/carWindowB.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		alphaGen const 0.05
		blendFunc blend
	}
	{
		map textures/models/vehicles/car/carWindowB.tga
		rgbGen vertex
		blendFunc blend
	}
}