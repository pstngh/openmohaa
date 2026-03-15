textures/clock/remag_clk
{
	qer_editorimage textures/clock/stnclkface.tga
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	PolygonOffset
	nomipmaps
//	nopicmip
	{
		clampmap textures/clock/stnclkface.tga
		depthwrite
		alphafunc GE128
	}
	{
		clampmap textures/clock/stnclkhour.tga
		alphafunc GE128
		tcMod rotate .008333333 180
	}
	{
		clampmap textures/clock/stnclkminute.tga
		alphafunc GE128
		tcMod rotate .1 45
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}

/////////////////////////////////////////////////////
// static clock face 1
/////////////////////////////////////////////////////

static_clockface1
{
	qer_editorimage textures/clock/clockface1.tga
	nomipmaps
	nopicmip
	{
		clampmap textures/clock/clockface1.tga
		depthwrite
	}
	///////////////////////////////////////////
	///hourhand
	///////////////////////////////////////////
	{
		clampmap textures/clock/hourhand.tga
		alphafunc GE128
		tcMod rotate .008333333 180
	}
	///////////////////////////////////////////
	///minutehand
	///////////////////////////////////////////
	{
		clampmap textures/clock/minutehand.tga
		alphafunc GE128
		tcMod rotate .1 45
	}
	{
		map $lightmap
		rgbGen static
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}

/////////////////////////////////////
// clock housing 1
/////////////////////////////////////

static_clockhousing
{
	cull none
	qer_editorimage textures/clock/clockhousing.tga
	{
		map textures/clock/clockhousing.tga
		rgbGen static
	}
}

/////////////////////////////////////
// roman numeral clockface
/////////////////////////////////////

static_clockface2
{
	qer_editorimage textures/clock/clockface_roman.tga
	nomipmaps
	nopicmip
	{
		clampmap textures/clock/clockface_roman.tga
		depthwrite
	}
	///////////////////////////////////////////
	///hourhand
	///////////////////////////////////////////
	{
		clampmap textures/clock/hourhand.tga
		alphafunc GE128
		tcMod rotate .008333333 90
	}
	///////////////////////////////////////////
	///minutehand
	///////////////////////////////////////////
	{
		clampmap textures/clock/minutehand.tga
		alphafunc GE128
		tcMod rotate .1 0
	}
	{
		map $lightmap
		rgbGen static
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}

/////////////////////////////////////
// black clock housing
/////////////////////////////////////

static_blackclock
{
	cull none
	qer_editorimage textures/clock/blackframe.tga
	{
		map textures/clock/blackframe.tga
		rgbGen static
	}
}

//////////////////////////////////
//wirecage
//////////////////////////////////

static_wirecage
{
	nopicmip
	nomipmaps
	qer_editorimage textures/clock/wiregrate2.tga
	{
		map textures/clock/wiregrate2.tga
		alphaFunc GE128
		rgbGen static
	}
}

/////////////////////////////////////
// aluminum side
/////////////////////////////////////

static_aluminumclock
{
	cull none
	qer_editorimage textures/clock/aluminumside.tga
	{
		map textures/clock/aluminumside.tga
		rgbGen static
	}
}


/////////////////////////////////////
//black clock housing with inner lip
/////////////////////////////////////

static_blackclock2
{
	cull none
	qer_editorimage textures/clock/blackframe2.tga
	{
		map textures/clock/blackframe2.tga
		rgbGen static
	}
}

/////////////////////////////////////
//brass case for subclock
/////////////////////////////////////

static_brassclock
{
	cull none
	qer_editorimage textures/clock/brassframe2.tga
	{
		map textures/clock/brassframe2.tga
		rgbGen static
	}
}

/////////////////////////////////////////////////////
// uboat clock face
/////////////////////////////////////////////////////

static_subclock
{
	qer_editorimage textures/clock/subclockface.tga
	nomipmaps
	nopicmip
	{
		clampmap textures/clock/subclockface.tga
		depthwrite
	}
	///////////////////////////////////////////
	///hourhand
	///////////////////////////////////////////
	{
		clampmap textures/clock/subhour.tga
		alphafunc GE128
		tcMod rotate .008333333 180
	}
	///////////////////////////////////////////
	///minutehand
	///////////////////////////////////////////
	{
		clampmap textures/clock/subminute.tga
		alphafunc GE128
		tcMod rotate .1 45
	}
	{
		map $lightmap
		rgbGen static
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}
