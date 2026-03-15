textures/barracks/mag1
{
	qer_keyword masked
	qer_keyword signs
	surfaceparm paper
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/barracks/mag1.tga
	cull none
	nopicmip
	{
		map textures/barracks/mag1.tga
		blendfunc blend
		depthWrite
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/barracks/pinup1
{
	qer_keyword masked
	qer_keyword signs
	surfaceparm paper
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/barracks/pinup1.tga
	cull none
	nopicmip
	{
		map textures/barracks/pinup1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/barracks/tentallies
{
	qer_keyword carpet
	qer_keyword special
	qer_keyword wall
	qer_keyword ceiling
	qer_keyword masked
	surfaceparm carpet
	surfaceparm fence
    qer_editorimage textures/barracks/tentallies.tga
	cull none
	{
		map textures/barracks/tentallies.tga
		alphaFunc GE128
		depthwrite
	nextbundle
		map $lightmap
	}
}