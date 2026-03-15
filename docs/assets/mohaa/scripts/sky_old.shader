textures/mohtest/test
{
	qer_editorimage textures/common/sky.tga
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	skyParms env/edensky 1024 -
	{
		map textures/sky/test_overlay.tga
		blendFunc GL_ONE GL_ONE
		tcMod scroll .01 .03
		tcMod scale 3 2
	}
}

test1
{
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	surfacelight 128
	{
		map textures/sky/inteldimclouds.tga
		tcMod scroll .1 .1
		tcMod scale 3 2
	}
	{
		map textures/sky/intelredclouds.tga
		blendFunc GL_ONE GL_ONE
		tcMod scroll .05 .05
		tcMod scale 3 3
	}
}

textures/we_cemetary_test/levelord_sky
{
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
//	cloudParms 512 half
//	sky env/cemsky
//	skyParms 1
	skyParms env/cemsky 512 -
	q3map_lightimage textures/sky/test_color.tga
	{
		map textures/sky/test_overlay.tga
		blendFunc GL_ONE GL_ONE
		rgbGen constant .5 .4 .2
		tcMod scroll .05 .05
		tcMod scale 3 3
	}
}

textures/sky/cliffsky
{
	qer_editorimage textures/common/sky.tga
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	skyParms half 512 -
	{
//		map textures/sky/inteldimclouds.tga
		map textures/sky/test_overlay.tga
		tcMod scroll .1 .1
		tcMod scale 3 2
	}
	{
//		map textures/sky/intelredclouds.tga
		map textures/sky/test_overlay.tga
		blendFunc GL_ONE GL_ONE
		tcMod scroll .05 .05
		tcMod scale 3 3
	}
}