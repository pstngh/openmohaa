MC_gp_plowed_field
{
	qer_editorimage textures/MonteCassino_GroundPortal/plowed_field.jpg
	{
		map textures/MonteCassino_GroundPortal/plowed_field.jpg
		rgbGen constant 1.0 1.0 1.0
		blendfunc blend
	}
}

MC_gp_road
{
	qer_editorimage textures/MonteCassino_GroundPortal/road.jpg
	{
		map textures/MonteCassino_GroundPortal/road.jpg
		rgbGen constant 1.0 1.0 1.0
		blendfunc blend
	}
}

MC_gPortal_1
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_a.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_a.jpg
		rgbGen identity
	}
}
MC_gPortal_2
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_b.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_b.jpg
		rgbGen identity
	}
}
MC_gPortal_3
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_c.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_c.jpg
		rgbGen identity
	}
}
MC_gPortal_4
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_d.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_d.jpg
		rgbGen identity
	}
}
MC_gPortal_5
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_f.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_f.jpg
		rgbGen identity
	}
}
MC_gPortal_6
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_g.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_g.jpg
		rgbGen identity
	}
}
MC_gPortal_7
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_h.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_h.jpg
		rgbGen identity
	}
}
MC_gPortal_8
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_i.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_i.jpg
		rgbGen identity
	}
}
MC_gPortal_9
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_a.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_a.jpg
		rgbGen identity
	}
}
MC_gPortal_10
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_b.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_b.jpg
		rgbGen identity
	}
}
MC_gPortal_11
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_c.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_c.jpg
		rgbGen identity
	}
}
MC_gPortal_12
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_d.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_d.jpg
		rgbGen identity
	}
}
MC_gPortal_13
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_e.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_e.jpg
		rgbGen identity
	}
}
MC_gPortal_14
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_f.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_f.jpg
		rgbGen identity
	}
}
MC_gPortal_15
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_g.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_g.jpg
		rgbGen identity
	}
}
MC_gPortal_16
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_h.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_h.jpg
		rgbGen identity
	}
}
MC_gPortal_17
{
	qer_editorimage textures/MonteCassino_GroundPortal/t1l1_center_i.jpg
	{
		map textures/MonteCassino_GroundPortal/t1l1_center_i.jpg
		rgbGen identity
	}
}


//added for Cassino Grass
textures/MonteCassino_GroundPortal/cassino_grass
{
	qer_editorimage textures/MonteCassino_GroundPortal/cassino_grass.tga	
	qer_keyword vex
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		map textures/MonteCassino_GroundPortal/cassino_grass.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}
