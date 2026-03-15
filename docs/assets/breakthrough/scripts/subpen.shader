//MP_Druckkammern_TOW Sky Shader

textures/mp_subpen/subpen
{
	qer_editorimage textures/mp_subpen/subpen_ed.tga	
	qer_keyword common
	qer_keyword sky
	qer_keyword vexar
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	skyParms env/subpen 512 -
	{
		map textures/mp_subpen/vexar_clouds_1.tga
		blendFunc blend
		tcMod scroll .005 .005
		tcMod scale 5 5
	}

	{
		map textures/mp_subpen/vexar_clouds_3.tga
		blendFunc blend
		tcMod scroll .001 .001
		tcMod scale 6 6
	}

}

//New upper_hull shaders
static_miscpieces_ta
{
	cull none
	qer_editorimage textures/models/submodels/miscpieces.tga
	{
		map textures/models/submodels/miscpieces.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}
static_subrailing_ta
{
	cull none
	qer_editorimage textures/models/submodels/subrailing.tga
	{
		map textures/models/submodels/subrailing.tga
		alphaFunc GE128
		rgbGen lightingSpherical
	}
}

// Generator
generator_top5
{
	qer_editorimage textures/models/static/generator/generator_top5.jpg
	{
		map textures/models/static/generator/generator_top5.jpg
		rgbGen lightingSpherical
	}
}
generator_side1
{
	qer_editorimage textures/models/static/generator/generator_side1.jpg
	{
		map textures/models/static/generator/generator_side1.jpg
		rgbGen lightingSpherical
	}
}
generator_top
{
	qer_editorimage textures/models/static/generator/generator_top.jpg
	{
		map textures/models/static/generator/generator_top.jpg
		rgbGen lightingSpherical
	}
}
generator_vents
{
	qer_editorimage textures/models/static/generator/generator_vents.tga
	cull none
	{
		map textures/models/static/generator/generator_vents.tga
		depthwrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
generator_railing
{
	qer_editorimage textures/models/static/generator/generator_railing.tga
	nopicmip
	{
		map textures/models/static/generator/generator_railing.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
generator_trim1
{
	qer_editorimage textures/models/static/generator/generator_trim1.jpg
	{
		map textures/models/static/generator/generator_trim1.jpg
		rgbGen lightingSpherical
	}
}
generator_side2
{
	qer_editorimage textures/models/static/generator/generator_side2.jpg
	{
		map textures/models/static/generator/generator_side2.jpg
		rgbGen lightingSpherical
	}
}
generator_top3
{
	qer_editorimage textures/models/static/generator/generator_top3.jpg
	{
		map textures/models/static/generator/generator_top3.jpg
		rgbGen lightingSpherical
	}
}
generator_nurnies
{
	qer_editorimage textures/models/static/generator/generator_nurnies.jpg
	{
		map textures/models/static/generator/generator_nurnies.jpg
		rgbGen lightingSpherical
	}
}
generator_trim3
{
	qer_editorimage textures/models/static/generator/generator_side3.jpg
	{
		map textures/models/static/generator/generator_side3.jpg
		rgbGen lightingSpherical
	}
}
generator_side3a
{
	qer_editorimage textures/models/static/generator/generator_side3a.jpg
	{
		map textures/models/static/generator/generator_side3a.jpg
		rgbGen lightingSpherical
	}
}
generator_side5
{
	qer_editorimage textures/models/static/generator/generator_side5.jpg
	{
		map textures/models/static/generator/generator_side5.jpg
		rgbGen lightingSpherical
	}
}
generator_decking
{
	qer_editorimage textures/models/static/generator/generator_decking.jpg
	{
		map textures/models/static/generator/generator_decking.jpg
		rgbGen lightingSpherical
	}
}
generator_side1_seam
{
	qer_editorimage textures/models/static/generator/generator_side1_seam.jpg
	{
		map textures/models/static/generator/generator_side1_seam.jpg
		rgbGen lightingSpherical
	}
}
generator_vents2
{
	qer_editorimage textures/models/static/generator/generator_vents2.tga
	{
		map textures/models/static/generator/generator_vents2.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
generator_inards
{
	qer_editorimage textures/models/static/generator/generator_inards.jpg
	{
		map textures/models/static/generator/generator_inards.jpg
		rgbGen lightingSpherical
	}
}
generator_ladder
{
	qer_editorimage textures/models/static/generator/generator_ladder.tga
	{
		map textures/models/static/generator/generator_ladder.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
generator_railing2
{
	qer_editorimage textures/models/static/generator/generator_railing2.tga
	{
		map textures/models/static/generator/generator_railing2.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
generator_top6
{
	qer_editorimage textures/models/static/generator/generator_top2.jpg
	{
		map textures/models/static/generator/generator_top2.jpg
		rgbGen lightingSpherical
	}
}





textures/mp_subpen/subpen/fuel_pipe
{
	qer_editorimage textures/models/static/fuel_tanks/fuel_pipe.jpg
	qer_keyword subpen
	qer_keyword metal
	surfaceparm metal	
	{
		map textures/models/static/fuel_tanks/fuel_pipe.jpg
		depthWrite
		rgbGen identity
	}
	
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/mp_subpen/subpen/fuel_panels
{
	qer_editorimage textures/models/static/fuel_tanks/fuel_panels.jpg
	qer_keyword subpen
	qer_keyword metal
	surfaceparm metal	
	{
		map textures/models/static/fuel_tanks/fuel_panels.jpg
		depthWrite
		rgbGen identity
	}
	
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/mp_subpen/subpen/fuel_connect
{
	qer_editorimage textures/models/static/fuel_tanks/fuel_connect.jpg
	qer_keyword subpen
	qer_keyword metal
	surfaceparm metal
	{
		map textures/models/static/fuel_tanks/fuel_connect.jpg
		depthWrite
		rgbGen identity
	}
	
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/mp_subpen/subpen/fuel_connect_edge
{
	qer_editorimage textures/models/static/fuel_tanks/fuel_connect_edge.jpg
	qer_keyword subpen
	qer_keyword metal
	surfaceparm metal
	{
		map textures/models/static/fuel_tanks/fuel_connect_edge.jpg
		depthWrite
		rgbGen identity
	}
	
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
