
textures/misc_outside/commontreeshadow
{
	qer_editorimage textures/models/natural/treeshadow1.tga
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm alphashadow
	surfaceparm nonsolid
	{
		map textures/models/natural/treeshadow1.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

textures/misc_outside/tallwintershadow
{
	qer_editorimage textures/models/natural/tree5ssprite.tga
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm alphashadow
	surfaceparm nonsolid
	{
		map textures/models/natural/tree5ssprite.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

