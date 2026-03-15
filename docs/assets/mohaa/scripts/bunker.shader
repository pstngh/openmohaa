static_vent_valve
{
	qer_editorimage textures/models/bunker/vent_valve.tga
	{
		map textures/models/bunker/vent_valve.tga
		rgbGen vertex
		//rgbGen static
		//rgbGen lightingSpherical
	}
}

switch1
{
	qer_editorimage textures/models/bunker/switch1.tga
	{
		map textures/models/bunker/switch1.tga
		rgbGen lightingSpherical
	}
}

switch1_pulse
{
	qer_editorimage textures/models/bunker/switch1.tga
	{
		map textures/models/bunker/switch1.tga
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

static_switch1
{
	qer_editorimage textures/models/bunker/switch1.tga
	{
		map textures/models/bunker/switch1.tga
		rgbGen vertex
	}
}

static_switch2
{
	qer_editorimage textures/models/bunker/switch2.tga
	{
		map textures/models/bunker/switch2.tga
		rgbGen vertex
		//rgbGen static
		//rgbGen lightingSpherical
	}
}

filter_thing
{
	qer_editorimage textures/models/bunker/gas_filter.tga
	{
		map textures/models/bunker/gas_filter.tga
		rgbGen static
	}
}
// switch1 pulsating
pulsating_switch1
{
	qer_editorimage textures/models/bunker/switch1.tga
	{
		map textures/models/bunker/switch1.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

/////////////////////////////////////////////////
//bunker lights
/////////////////////////////////////////////////

/////////////////////////
//lightbulb
/////////////////////////

static_lightbulb
{
	qer_editorimage textures/models/bunker/lightbulb.tga
	{
		map textures/models/bunker/lightbulb.tga
		rgbGen static
	}
}
	
/////////////////////////
//domelight
/////////////////////////

static_domelight
{
	qer_editorimage textures/models/bunker/fixture1.tga
	{
		map textures/models/bunker/fixture1.tga
		rgbGen static
	}
}

///////////////////////////
//environment mapped glass
///////////////////////////

static_glass
{
//cull none
	{
		map textures/test/window_env.tga
		tcgen environmentmodel
		alphagen const .1
		blendFunc blend
	      rgbGen static
	}
}

////////////////////////////
//wall cage light
////////////////////////////

static_wallcage
{
	qer_editorimage textures/models/bunker/fixture2.tga
	{
		map textures/models/bunker/fixture2.tga
		rgbGen static
	}
}

////////////////////////////
//wire cage
////////////////////////////

static_wirecage2
{
	qer_editorimage textures/models/bunker/wirecage.tga
	{
		map textures/models/bunker/wirecage.tga
		alphaFunc GE128
		rgbGen static
	}
}
