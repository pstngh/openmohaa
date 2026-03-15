textures/interior/ceiling_mural1
{
	{
		map textures/special/goldenv.tga
		tcGen environment
	}
	{
		map textures/interior/ceiling_mural1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/special/silverenv
{
	qer_keyword special
	{
		map textures/special/silverenv.tga
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