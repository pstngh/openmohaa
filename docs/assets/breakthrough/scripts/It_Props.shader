It_P_Streetsign
{
	qer_editorimage textures/models/static/Streetsign/It_P_Streetsign.tga
	
	{
		map textures/models/static/Streetsign/It_P_Streetsign.tga	
		rgbGen lightingSpherical
	}
}

It_P_Medicbox
{
	qer_editorimage textures/models/static/Medicbox/It_P_Medicbox.tga
	
	{
		map textures/models/static/Medicbox/It_P_Medicbox.tga	
		rgbGen lightingSpherical
	}
}

It_P_Storesign1
{
	qer_editorimage textures/models/static/Storesign/It_P_Storesign1.tga
	
	{
		map textures/models/static/Storesign/It_P_Storesign1.tga	
		rgbGen lightingSpherical
	}
}

It_P_Storesign2
{
	qer_editorimage textures/models/static/Storesign/It_P_Storesign2.tga
	
	{
		map textures/models/static/Storesign/It_P_Storesign2.tga	
		rgbGen lightingSpherical
	}
}

It_P_Storesign1a
{
	qer_editorimage textures/models/static/Storesign/It_P_Storesign1a.tga
	
	{
		map textures/models/static/Storesign/It_P_Storesign1a.tga	
		rgbGen lightingSpherical
	}
}

It_P_Mannequin
{
	qer_editorimage textures/models/static/Mannequin/It_P_Mannequin.tga
	
	{
		map textures/models/static/Mannequin/It_P_Mannequin.tga	
		rgbGen lightingSpherical
	}
}

It_P_statue
{
	qer_editorimage textures/models/static/It_P_statue/It_P_statue.tga
	
	{
		map textures/models/static/It_P_statue/It_P_statue.tga
		rgbGen lightingSpherical
	}
}

It_P_fountain
{
	qer_editorimage textures/models/static/It_P_statue/It_P_fountain.tga
	
	{
		map textures/models/static/It_P_statue/It_P_fountain.tga
		rgbGen lightingSpherical
	}
}

Af_P_stretcher
{
	cull none
	qer_editorimage textures/models/static/Stretcher/Af_P_stretcher.tga
	
	{
		map textures/models/static/Stretcher/Af_P_stretcher.tga
		rgbGen lightingSpherical
	}
}
It_P_K5Schematics
{
	cull none	
	qer_editorimage textures/models/static/K5Schematics/It_P_K5Schematics.tga
	
	{
		map textures/models/static/K5Schematics/It_P_K5Schematics.tga
		rgbGen lightingSpherical
	}
}
It_P_Anziomap
{
	
	qer_editorimage textures/models/items/Anziomap/It_P_Anziomap.tga
	
	{
		map textures/models/items/Anziomap/It_P_Anziomap.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}
It_P_Anziomapnopulse
{
	
	qer_editorimage textures/models/items/Anziomap/It_P_Anziomap.tga
	cull none
	{
		map textures/models/items/Anziomap/It_P_Anziomap.tga
		rgbGen lightingSpherical
	}
}
It_P_HayBale
{
	
	qer_editorimage textures/models/static/HayBale/It_P_HayBale.tga
	
	{
		map textures/models/static/HayBale/It_P_HayBale.tga
		rgbGen lightingSpherical
	}
}
It_P_RadarStation
{
	cull none
	qer_editorimage textures/models/static/RadarStation/Radar_Dark.tga
	
	{
		map textures/models/static/RadarStation/Radar_Dark.tga
		rgbGen lightingSpherical
	}
}