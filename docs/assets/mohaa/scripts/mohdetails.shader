textures/general_structure/concretewall1
{
	qer_editorimage	textures/general_structure/jh_conc1.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/general_structure/jh_conc1.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/general_structure/concretewall2
{
	qer_editorimage	textures/general_structure/jh_conc2.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/general_structure/jh_conc2.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/general_structure/adobewalltall_1
{
	qer_editorimage	textures/general_structure/jh_adobwalltall1.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/general_structure/jh_adobwalltall1.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/general_structure/adobewalltall_1a
{
	qer_editorimage	textures/general_structure/jh_adobwalltall1a.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/general_structure/jh_adobwalltall1a.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/lightplaster1
{
	qer_editorimage	textures/algiers/plasterwal2.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/algiers/plasterwal2.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/lightplaster2
{
	qer_editorimage	textures/algiers/plasterwal2a.tga
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/algiers/plasterwal2a.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/detail3
{
	qer_editorimage	textures/mohcommon/detail3.tga
	qer_keyword special
	{
		map textures/mohcommon/detail3.tga
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