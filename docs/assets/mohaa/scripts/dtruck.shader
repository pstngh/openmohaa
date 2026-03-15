//////////////////////////////////////////////////////////////////////////////////////////////
//Civilian delivery truck1                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////

Dtruck1
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck1.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck1.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

static_Dtruck1
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck1.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck1.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//Civilian delivery truck2                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////

Dtruck2
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck2.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck2.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

static_Dtruck2
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck2.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck2.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//Civilian delivery truck3                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////

Dtruck3
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck3.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck3.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

static_Dtruck3
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/Dtruck3.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/Dtruck3.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//Civilian delivery truck all burnt up                                                                  //
/////////////////////////////////////////////////////////////////////////////////////////////////

Dtruck_burnt
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/DTruckBurnt.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/DTruckBurnt.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}

static_dtruck_burnt
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/DTruckBurnt.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/DTruckBurnt.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//Civilian delivery truck rusted                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////

Dtruck_rusted
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/DTruckRusted.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/DTruckRusted.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}


static_dtruck_rusted
{
	cull none
	qer_editorimage textures/models/VEHICLES/Dtruck/DTruckRusted.tga
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
	}
	{
		map textures/models/VEHICLES/Dtruck/DTruckRusted.tga
		rgbGen vertex
		blendfunc gl_one_minus_src_alpha gl_src_alpha 
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//window                                                                              //
////////////////////////////////////////////////////////////////////////////////////////

Dtruckwindow
{
	qer_editorimage textures/models/vehicles/Dtruck/Dtruckwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.3
		blendFunc blend
	}
	{
		map textures/models/vehicles/Dtruck/Dtruckwindow.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

static_Dtruckwindow
{
	qer_editorimage textures/models/vehicles/Dtruck/Dtruckwindow.tga
	cull none
	{
		map textures/common/reflection1.tga
		rgbGen vertex
		tcgen environmentmodel
		alphaGen const 0.3
		blendFunc blend
	}
	{
		map textures/models/vehicles/Dtruck/Dtruckwindow.tga
		rgbGen vertex
		blendFunc blend
	}
}
////////////////////////////////////////////////////////////////////////////////////////
// broken windows                                                                              //
////////////////////////////////////////////////////////////////////////////////////////
DtruckwindowB
{
	qer_editorimage textures/models/vehicles/Dtruck/DTruckWindowB.tga
	cull none
//	{
		//map textures/common/reflection1.tga
		//rgbGen lightingSpherical
		//tcgen environmentmodel
		//alphaGen const 0.3
		//blendFunc blend
//	}
	{
		map textures/models/vehicles/Dtruck/DTruckWindowB.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

static_dtruckwindowb
{
	qer_editorimage textures/models/vehicles/Dtruck/DTruckWindowB.tga
	cull none
//	{
		//map textures/common/reflection1.tga
		//rgbGen vertex
		//tcgen environmentmodel
		//alphaGen const 0.3
		//blendFunc blend
//	}
	{
		map textures/models/vehicles/Dtruck/DTruckWindowB.tga
		rgbGen vertex
		blendFunc blend
	}
}