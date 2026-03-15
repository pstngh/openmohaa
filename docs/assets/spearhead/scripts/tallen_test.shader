
textures/ta_berlin/window3brokenalpha
{
	qer_editorimage textures/ta_berlin/window3frame.tga
	qer_keyword broken
	qer_keyword window
	qer_keyword glass
	surfaceparm fence
	surfaceparm patch	
	surfaceparm wood
	surfaceparm castshadow
	surfaceparm alphashadow
	surfaceparm nonsolid

//	cull none
	{
		map textures/test/window_env.tga
		tcgen environment
		alphagen const .2
		blendFunc blend
		alphaFunc GT0
	nextbundle
		map textures/ta_berlin/window3broken.tga
	}
	{
		map textures/ta_berlin/window3frame.tga
//		blendFunc blend
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}
