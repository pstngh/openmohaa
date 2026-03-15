textures/mohtest/window4
{
	qer_editorimage textures/mohtest/window4.tga
	cull disable
	{
		map textures/mohtest/window4.tga
		blendFunc blend
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
}

townwindow
{
	qer_editorimage textures/mohtest/window4.tga
	q3map_lightimage textures/mohtest/window4.tga
	cull disable
	{
		map textures/mohtest/window4.tga
		blendFunc blend
		rgbGen identity
	}
}