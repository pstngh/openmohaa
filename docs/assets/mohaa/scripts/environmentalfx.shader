textures/misc_outside/blowingleaves
{
	qer_editorimage textures/misc_outside/leaves_blown.tga
	surfaceparm nonsolid
	surfaceparm trans
	cull none
	deformvertexes wave 30 sin 0 10 0 .2
	nopicmip
	{
		map textures/misc_outside/leaves_blown.tga
		tcMod scale 2 2
		tcMod scroll 0 .25
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

// Boon: Removed this as it is also defined in german.shader
//textures/German/nazibanner1
//{
//	qer_editorimage textures/german/nazbannr1.tga
//	surfaceparm nonsolid
//	surfaceparm trans
//	cull none
//	deformvertexes wave 30 sin 0 10 0 .2
//	nopicmip
//	{
//		map textures/German/nazbannr1.tga
//		blendFunc blend
//		alphaFunc GE128
//		depthWrite
//		rgbGen identity
//	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//		depthFunc equal
//	}
//}

//textures/central_europe_winter/snowfield1
//{
//	qer_editorimage textures/central_europe_winter/snowfield.tga
//	{
//		map textures/central_europe_winter/snowfx.tga
//     		tcMod parallax -.001 -.001
//	}
//	{
//		map textures/central_europe_winter/snowfield.tga
//		blendFunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
//		rgbGen identity
//	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//	}
//}

textures/central_europe_winter/jhpjwinterwall1_sparkley
{
	qer_editorimage textures/central_europe_winter/jhpj_winterwall1sprk.tga
	nopicmip
	{
		map textures/central_europe_winter/snowfx3.tga
		tcMod parallax .002 .002
		tcMod scale 15 15
	nextbundle
		map textures/central_europe_winter/snowfx2.tga
		tcMod scale 2 2
	}
	{
		map $whiteimage
		rgbGen constant .2 .2 .2
		blendFunc add
	}
	{
		// This stage lightens up the non-sparkly bits
		map textures/central_europe_winter/jhpj_winterwall1sprk.tga
//		rgbGen const .0625 .0954 .1616
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
//		tcMod scale .4 .5
	nextbundle
		map $lightmap
	}
}


ripple
{
	cull none
	spriteGen oriented
	{
		map textures/sprites/ripple.tga
		blendFunc GL_ONE GL_ONE
//		alphaGen vertex
		rgbGen vertex
	}
}

textures/windowflash
{
	nopicmip
	qer_editorimage textures/mohtest/windowflash.tga
	qer_keyword mackey
	surfaceparm nonsolid
	surfaceParm nolightmap

	polygonoffset
	{
		map textures/mohtest/windowflash.tga
		rgbgen const 0.7 0.7 0.7

//		rgbgen const 0.82 0.82 0.82
		//alphagen const 0.5
		blendfunc add
	}
}

textures/windowflash2
{
	nopicmip
	qer_editorimage textures/mohtest/windowflash2.tga
	qer_keyword mackey
	surfaceparm nonsolid
	surfaceParm nolightmap

	polygonoffset
	{
		map textures/mohtest/windowflash2.tga
		rgbgen const 0.7 0.7 0.7

//		rgbgen const 0.82 0.82 0.82
		//alphagen const 0.5
		blendfunc add
	}
}


textures/mohtest/window4masked2
{
	qer_editorimage textures/mohtest/window4msk2.tga
	qer_keyword mackey
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm nomarks
	surfaceparm wood
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm trans
	cull none
	nopicmip
	{
		map textures/mohtest/window4msk2.tga
		alphaFunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}
}
