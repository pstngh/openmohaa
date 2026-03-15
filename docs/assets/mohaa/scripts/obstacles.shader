// Hedgehog
hedgehog
{
	qer_editorimage textures/models/items/hedgehog.tga
	cull none
	{
		map textures/models/items/hedgehog.tga
		rgbGen lightingGrid
	}
}

// Teller minepost
minepole
{
	qer_editorimage textures/models/items/minepole.tga
	{
		map textures/models/items/minepole.tga
		rgbGen lightingGrid
	}
}

minepole2
{
	qer_editorimage textures/models/items/minepole.tga
	cull none
	{
		map textures/models/items/minepole.tga
		rgbGen lightingGrid
	}
}

// Ramp
ramp
{
	qer_editorimage textures/models/items/ramp.tga
	{
		map textures/models/items/ramp.tga
		rgbGen lightingGrid
	}
}

// Barb wire posts
bwirepost
{
	qer_editorimage textures/models/items/bwirepost.tga
	{
		map textures/models/items/bwirepost.tga
		rgbGen lightingGrid
	}
}

// Barb wire
bspindle
{
	qer_editorimage textures/models/items/bspindle.tga
	cull none
	nopicmip
	nomipmaps
	{
		clampmap textures/models/items/bspindle.tga
		blendFunc blend
//		alphaFunc GT0
//		alphaGen distFade 2048 3072
		alphaGen distFade 256 3072
		rgbGen lightingGrid
	}
}

bplane
{
	qer_editorimage textures/models/items/bplane.tga
	cull none
	nopicmip
	nomipmaps
	{
		clampmap textures/models/items/bplane.tga
		blendFunc blend
//		alphaFunc GT0
//		alphaGen distFade 2048 3072
		alphaGen distFade 256 3072
		rgbGen lightingGrid
	}
}

// Barb wire Pulsing
bspindle_pulse
{
	qer_editorimage textures/models/items/bspindle.tga
	cull none
	nopicmip
	nomipmaps
	{
		clampmap textures/models/items/bspindle.tga
		blendFunc blend
//		alphaFunc GT0
//		alphaGen distFade 2048 3072
		alphaGen distFade 256 3072
		rgbGen lightingGrid
	}
	{ 
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

bplane_pulse
{
	qer_editorimage textures/models/items/bplane.tga
	cull none
	nopicmip
	nomipmaps
	{
		clampmap textures/models/items/bplane.tga
		blendFunc blend
//		alphaFunc GT0
//		alphaGen distFade 2048 3072
		alphaGen distFade 256 3072
		rgbGen lightingGrid


	}
	{
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Large rock winter
lrock1
{
	qer_editorimage textures/models/items/rockwinter1.tga
	{
		map textures/models/items/rockwinter1.tga
		rgbGen lightingGrid
	}
}

static_lrock1
{
	qer_editorimage textures/models/items/rockwinter1.tga
	{
		map textures/models/items/rockwinter1.tga
		rgbGen vertex
	}
}

// Medium rock winter
mrock1
{
	qer_editorimage textures/models/items/mrock1.tga
	{
		map textures/models/items/mrock1.tga
		rgbGen lightingGrid
	}
}

static_mrock1
{
	qer_editorimage textures/models/items/mrock1.tga
	{
		map textures/models/items/mrock1.tga
		rgbGen vertex
	}
}

// Haystack
static_haystack
{
	qer_editorimage textures/models/items/haystack.tga
	{
		map textures/models/items/haystack.tga
		rgbGen vertex
	}
}

// Big pile of rubble
static_lrubble1
{
	qer_editorimage textures/mohtest/rubble2c.tga
	{
		map textures/mohtest/rubble2c.tga
		rgbGen vertex
	}
}

static_boards
{
	qer_editorimage textures/interior/trenchwall2.tga
	{
		map textures/interior/trenchwall2.tga
		rgbGen vertex
	}
}

// Small pile of rubble
static_srubble1
{
	qer_editorimage textures/models/items/rubblebase.tga
	{
		map textures/models/items/rubblebase.tga
		rgbGen vertex
	}
}

// Can be placed in levels as a normal texture
textures/models/items/rubblebase
{
	{
		map textures/models/items/rubblebase.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

static_bricks
{
	qer_editorimage textures/models/items/rubbleblocks.tga
	{
		map textures/models/items/rubbleblocks.tga
		rgbGen vertex
	}
}