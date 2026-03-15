textures/TAnatural/tallen_beam_01
{
	qer_keyword tallen_stuff
	surfaceparm alphashadow
	surfaceparm nonsolid
    qer_editorimage textures/light/beam_01.tga
	cull none
	nopicmip
	{
		map textures/light/beam_01.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 