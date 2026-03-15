mp40
{
	qer_editorimage textures/models/weapons/mp40/mp40.tga
//#if seperate_env
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcGen environmentmodel
//	}
//	{
//		map textures/models/weapons/mp40/mp40.tga
//		rgbGen lightingSpherical
//		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
//	}
//#else
//	{
//		map textures/common/carpetclip.tga
//		rgbGen lightingSpherical
//	}
//#endif
//}
	{
		map textures/models/weapons/mp40/mp40.tga
		rgbGen lightingSpherical
	}
}

mg42
{
	qer_editorimage textures/models/weapons/mg42/mg42.tga
	cull none
	{
		map textures/models/weapons/mg42/mg42.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}

static_mg42
{
	qer_editorimage textures/models/weapons/mg42/mg42.tga
	cull none
	{
		map textures/models/weapons/mg42/mg42.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}



P38
{
	qer_editorimage textures/models/weapons/P38/P38.tga
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcGen environmentmodel
//	}
	{
		map textures/models/weapons/P38/P38.tga
		rgbGen lightingSpherical
//		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

KAR98
{
	qer_editorimage textures/models/weapons/KAR98/KAR98.tga
	{
		map textures/models/weapons/KAR98/KAR98.tga
		rgbGen lightingSpherical
	}
}
//k98 scope texture

k98scope
{
	qer_editorimage textures/models/weapons/kar98/kar98scope.tga
	{
		map textures/models/weapons/kar98/kar98scope.tga
		rgbGen lightingSpherical
	}
}
///////////////////////////////////////
//potato masher grenade
///////////////////////////////////////
stielhandgranate
{
	qer_editorimage textures/models/weapons/p_masher/steilhangrenate.tga
	{
		map textures/models/weapons/p_masher/steilhangrenate.tga
		rgbGen lightingSpherical
	}
}
static_stielhandgranate
{
	qer_editorimage textures/models/weapons/p_masher/steilhangrenate.tga
	{
		map textures/models/weapons/p_masher/steilhangrenate.tga
		rgbGen vertex
	}
}

panzerschreck
{
	qer_editorimage textures/models/weapons/panzerschreck/pschreck.tga
	cull none
	{
		map textures/models/weapons/panzerschreck/pschreck.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
panzershell
{
	qer_editorimage textures/models/weapons/panzerschreck/pzrshell.tga
	{
		map textures/models/weapons/panzerschreck/pzrshell.tga
		rgbGen lightingSpherical
	}
}
mp44
{
	qer_editorimage textures/models/waepons/mp44/mp44.tga
	{
		map textures/models/weapons/mp44/mp44.tga
		rgbGen lightingSpherical
	}
}

mp44clip
{
	qer_editorimage textures/models/waepons/mp44/mp44clip.tga
	{
		map textures/models/weapons/mp44/mp44clip.tga
		rgbGen lightingSpherical
	}
}