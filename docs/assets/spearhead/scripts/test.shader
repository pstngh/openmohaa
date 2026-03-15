tree_destroy_leaf
{
	spriteGen oriented
	cull none
	{
		clampmap textures/test/tree_destroy/leaf.tga
		alphafunc GE128
		alphaGen vertex
		rgbGen lightingSpherical
	}
}

textures/mp_subpen/fuel_support
{
	qer_editorimage textures/models/static/fuel_tanks/fuel_support.tga
	{
		map textures/models/static/fuel_tanks/fuel_support.tga
		alphafunc GE128
//		rgbGen lightingSpherical
	}
}
