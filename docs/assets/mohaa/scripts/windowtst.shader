textures/mohtest/windowtst
{
	{
		map textures/common/reflection1.tga
		tcGen environment
	}
	{
		map textures/mohtest/windowtst.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohtest/window2th
{
	{
		map textures/common/reflection1.tga
		tcGen environment
	}
	{
		map textures/mohtest/window2th.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohtest/window3
{
	{
		map textures/common/reflection1.tga
		tcGen environment
	}
	{
		map textures/mohtest/window3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}