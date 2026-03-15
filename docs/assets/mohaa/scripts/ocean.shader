textures/misc_outside/basicocean
{
	qer_editorimage textures/misc_outside/ocean1.tga
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean1.tga
		blendFunc blend
		depthWrite
		rgbGen identity
		tcMod scroll .05  -.025
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/deepbluesea_test
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_trans .4
	surfaceparm trans
	surfaceparm water
//	surfaceparm nolightmap
	cull none
	deformvertexes wave 30 sin 0 60 0 .1
//	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean1.tga
		tcGen environment
//		map $whiteimage
//		blendFunc blend
//		alphaGen oneMinusDot
//		vrgbGen constant 0 .25 .75
	}
	{
		map textures/misc_outside/ocean1.tga
		blendFunc GL_ONE GL_ONE
//		tcMod scale .333 .33
		tcMod scale .33 .33
//		tcMod turb 0 .2 0 .1
		tcMod scale 4 1
		tcMod turb .1 .3 .2 .1
		tcMod scale .25 1
	nextbundle
		map textures/misc_outside/ocean2.tga
		tcMod scale .55 .55
		tcMod scroll -.03 -.06
//		tcMod scroll -.03 -.05
	}
	{
		map textures/misc_outside/ocean1.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen lightingSpecular
		tcMod scroll .07 -.04
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/multiocean
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_trans .4
	surfaceparm trans
	surfaceparm water
//	surfaceparm nolightmap
	cull none
	deformvertexes wave 20 sin 0 40 0 .2
//	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean1.tga
		tcGen environment
		tcMod scale .222 .222
		tcMod scroll .003 .004
	}
	{
		map textures/misc_outside/froth1.tga
		blendFunc GL_ONE GL_ONE
		tcMod scale .255 .255
		tcMod turb .1 .3 .2 0
		tcMod scroll .003 .03
	nextbundle
		map textures/misc_outside/froth1.tga
		tcMod scale .039 .039
		tcMod scroll .02 .02
	}
	{
		map textures/misc_outside/froth1.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen lightingSpecular
		tcMod scroll .07 -.04
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/largestream
{
	qer_editorimage textures/misc_outside/stream1.tga
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .003 0 1
	nextbundle
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .001 .3 4
	}
	{
		map textures/misc_outside/stream1.tga
		blendFunc blend
		rgbGen identity
		tcMod scroll .05  -.025
//		tcMod trans .5
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/misc_outside/strange_frothy_water
{
	qer_editorimage textures/misc_outside/stream1.tga
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .003 0 1
	nextbundle
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .001 .3 4
	}
	{
		map textures/misc_outside/stream1.tga
		blendFunc GL_ONE GL_ONE
//		blendFunc blend
		rgbGen identity
		tcMod scroll .05  -.025
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/special/swill
{
	qer_editorimage textures/special/swill.tga
//	qer_trans .4
	surfaceparm trans
	surfaceparm water
	surfaceparm nolightmap
	cull none
//	deformvertexes wave 20 sin 0 20 0 .1
	{
		map textures/special/swill.tga
//		tcMod scale .33 .33
//		tcMod scale 4 1
//		tcMod turb .1 .3 .2 .1
		tcMod scroll .03 0
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}