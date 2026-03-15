// 2D shaders
gfx/2d/backtile
{
	nopicmip
	nomipmaps
	{
		noDepthTest
		map gfx/2d/backtile.tga
	}
}

gfx/fonts/facfont-20
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/facfont-20.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/marlett
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/marlett.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/verdana-12
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/verdana-12.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/verdana-14
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/verdana-14.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
		nodepthtest
	}
}

gfx/2d/arrow_right
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/arrow_right.tga
		blendFunc GL_ONE GL_ZERO
		rgbGen identity
	}
}

gfx/2d/arrow_left
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/arrow_left.tga
		blendFunc GL_ONE GL_ZERO
		rgbGen identity
	}
}

gfx/2d/blank
{
	nomipmaps
	{
		map gfx/2d/blank.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen oneMinusVertex
		rgbGen vertex
	}
}

gfx/2d/slowserver
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/slowserver.tga
		alphaFunc GT0
	}
}

////////////////////////////////////////
//extra fonts for game
////////////////////////////////////////

gfx/fonts/courier-16
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/courier-16.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/courier-18
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/courier-18.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/courier-20
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/courier-20.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/handle-16
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/handle-16.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/handle-18
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/handle-18.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/handle-22
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/handle-22.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

gfx/fonts/handle-23
{
	nopicmip
	nomipmaps
	{
		map gfx/fonts/handle-23.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}
