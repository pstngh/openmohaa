textures/choppywater/tehao_ChoppyWater
{
	qer_editorimage textures/choppywater/tehao_ChoppyWater.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
	surfaceParm nonsolid

	deformVertexes wave 20 sin 10 5 0 0.3
	deformVertexes wave 20 sin 10 5 1 0.2


	{
		map textures/water/opaque.tga
		blendfunc blend
		alphaGen constant .95
		rgbGen constant .85 .85 .85
	}
//	{
//		map textures/water/noise.tga
//		rgbGen identity
//		tcMod transform 1 0 0 1 0 0
//		tcMod scroll 0.01 -0.03
//
//  	nextbundle
//		map textures/water/noise2.tga
//		blendfunc add
//		rgbGen identity
//		tcMod transform 1 0 0 1 0 0
//		tcMod scroll 0.02 0
//	}

	{
		map textures/water/subpen_waves.tga
		blendfunc blend
		alphagen const .5
		rgbGen constant .85 .85 .85
		tcMod scroll 0 0.01
	}
	{
		map textures/water/water_shadows.tga
		tcMod scroll 0 0.07

	nextbundle
		map textures/water/water_shadows2.tga
		blendfunc filter
		rgbGen constant .85 .85 .85
		tcMod scroll 0.07 -0.07
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}


textures/choppywater/tehao_indoorWater
{
	qer_editorimage textures/choppywater/tehao_indoorWater.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
	surfaceParm nonsolid

	deformVertexes wave 5 sin 10 .7 0 0.4
	deformVertexes wave 5 sin 10 .7 1 0.3
	tessSize 128
	{
		map textures/water/opaque.tga
		blendfunc blend
		alphaGen constant .1
		//rgbGen entity
		rgbGen constant 1 .6 .6
	}
	{
		map textures/water/noise3.tga
		alphaGen constant .75
		rgbGen constant 1 .7 .6
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.01 -0.03
		tcMod scale 1.5 1.5
         nextbundle
		map textures/water/noise4.tga
		alphaGen constant .65
		blendfunc add
		rgbGen constant 1 .6 .6
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.02 0
		tcMod scale 1.5 1.5
	}
	{
		map textures/water/subpen_waves.tga
		blendfunc blend
		alphagen const .4
		//rgbGen entity
		rgbGen constant 1 .6 .6
		tcMod scroll 0 0.03
	}
	{
		map textures/water/water_shadows.tga
		tcMod scroll 0 0.07
	nextbundle
		map textures/water/water_shadows2.tga
		blendfunc filter
		tcMod scroll 0.07 -0.07
	}
	{
		map $lightmap
		rgbGen identity
		depthFunc equal
	}
}

