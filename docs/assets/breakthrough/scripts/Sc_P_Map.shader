Sc_P_Map
{
	qer_editorimage textures/models/static/Map/Sc_P_Map.tga
	{
		map textures/models/static/Map/Sc_P_Map.tga	
		rgbGen lightingSpherical
	}
}
Sc_P_MapPuls
{
	qer_editorimage textures/models/static/Map/Sc_P_Map.tga
	{
		map textures/models/static/Map/Sc_P_Map.tga	
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

