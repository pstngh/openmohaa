opel
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen lightingSpherical
	}
}
opel_cull
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen lightingSpherical
	}
}
opelhub
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhub.tga
	noMerge
	{
		map textures/models/vehicles/OpelTruck/opelhub.tga
		rgbGen lightingSpherical
		tcmod rotate 5 0 -20
	}
}
opelhub_norotate
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhub.tga
	{
		map textures/models/vehicles/OpelTruck/opelhub.tga
		rgbGen lightingSpherical
	}
}
opelgreen
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelgreen.tga
	{
		map textures/models/vehicles/OpelTruck/opelgreen.tga
		rgbGen lightingSpherical
	}
}
opel_cullgreen
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelgreen.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opelgreen.tga
		rgbGen lightingSpherical
	}
}
opelgreen_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelgreen_d.tga
	{
		map textures/models/vehicles/OpelTruck/opelgreen_d.tga
		rgbGen lightingSpherical
	}
}
opel_cullgreen_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelgreen_d.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opelgreen_d.tga
		rgbGen lightingSpherical
	}
}

opelhubgreen
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhubgreen.tga
	noMerge
	{
		map textures/models/vehicles/OpelTruck/opelhubgreen.tga
		rgbGen lightingSpherical
		tcmod rotate 5 0 -20
	}
}
opelhubgreen_norotate
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhubgreen.tga
	{
		map textures/models/vehicles/OpelTruck/opelhubgreen.tga
		rgbGen lightingSpherical
	}
}
opeltread
{
	qer_editorimage textures/models/vehicles/OpelTruck/opeltread.tga
	noMerge
	{
		map textures/models/vehicles/OpelTruck/opeltread.tga
		rgbGen lightingSpherical
		tcmod scroll 0.2 0
	}
}
opeltread_norotate
{
	qer_editorimage textures/models/vehicles/OpelTruck/opeltread.tga
	{
		map textures/models/vehicles/OpelTruck/opeltread.tga
		rgbGen lightingSpherical
	}
}
opelwindows
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelwindows.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		Blendfunc blend
//	}
	{
		map textures/models/vehicles/OpelTruck/opelwindows.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//grid lighting opel truck
/////////////////////////////////////////////////////////////////////////////////////
opel_grid
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen lightingGrid
	}
}
opel_cull_grid
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen lightingGrid
	}
}
opelhub_grid
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhub.tga
	{
		map textures/models/vehicles/OpelTruck/opelhub.tga
		rgbGen lightingGrid
	}
}
opeltread_grid
{
	qer_editorimage textures/models/vehicles/OpelTruck/opeltread.tga
	{
		map textures/models/vehicles/OpelTruck/opeltread.tga
		rgbGen lightingGrid
	}
}
opelwindows_grid
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelwindows.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingGrid
//		tcgen environmentmodel
//		Blendfunc blend
//	}
	{
		map textures/models/vehicles/OpelTruck/opelwindows.tga
		rgbGen lightingGrid
		blendFunc blend
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//static opel truck
/////////////////////////////////////////////////////////////////////////////////////

static_opel
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen vertex
	}
}
static_opel_cull
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opel.tga
		rgbGen vertex
	}
}
static_opelhub
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhub.tga
	{
		map textures/models/vehicles/OpelTruck/opelhub.tga
		rgbGen vertex
	}
}
static_opeltread
{
	qer_editorimage textures/models/vehicles/OpelTruck/opeltread.tga
	{
		map textures/models/vehicles/OpelTruck/opeltread.tga
		rgbGen vertex
	}
}
static_opelwindows
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelwindows.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen vertex
//		tcgen environmentmodel
//		Blendfunc blend
//	}
	{
		map textures/models/vehicles/OpelTruck/opelwindows.tga
		rgbGen vertex
		blendFunc blend
	}
}

opellights
{
	qer_editorimage textures/models/items/beam.tga
	{
		map textures/common/airdust.tga
		tcmod scroll 0 0.15
	nextbundle
		clampmap textures/models/items/beam.tga
		rgbGen dot 0 1.5
		blendfunc add	
	}
}
opellightflare
{
	qer_editorimage textures/sprites/corona_util.tga
	{
		map textures/sprites/corona_util.tga
		rgbGen dot -1 1.5
		blendfunc add	
	}
}

static_opelengine
{
	qer_editorimage textures/models/vehicles/OpelTruck/engine.tga
	{
		map textures/models/vehicles/OpelTruck/engine.tga
		rgbGen vertex
	}
}
///////////////////////////////////////////////////////////////
//damaged opel
///////////////////////////////////////////////////////////////
opelengine
{
	qer_editorimage textures/models/vehicles/OpelTruck/engine.tga
	{
		map textures/models/vehicles/OpelTruck/engine.tga
		rgbGen lightingSpherical
	}
}
opel_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel_d.tga
	{
		map textures/models/vehicles/OpelTruck/opel_d.tga
		rgbGen lightingSpherical
	}
}
opel_cull_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opel_d.tga
	cull none
	{
		map textures/models/vehicles/OpelTruck/opel_d.tga
		rgbGen lightingSpherical
	}
}
opelhub_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelhub_d.tga
	{
		map textures/models/vehicles/OpelTruck/opelhub_d.tga
		rgbGen lightingSpherical
	}
}

opelwindows_d
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelwindows_d.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		Blendfunc blend
//	}
	{
		map textures/models/vehicles/OpelTruck/opelwindows_d.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

opelwindows_d2
{
	qer_editorimage textures/models/vehicles/OpelTruck/opelwindows_d2.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		Blendfunc blend
//	}
	{
		map textures/models/vehicles/OpelTruck/opelwindows_d2.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}
