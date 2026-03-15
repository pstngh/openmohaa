textures/choppywater/subpen_opaque_chop
{
	qer_editorimage textures/choppywater/subpen_opaque_chop.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
	surfaceParm nonsolid

	deformVertexes wave 64 sin 8 3.5 0 0.3
	deformVertexes wave 64 sin 8 3.5 1 0.2

	{
		map textures/water/opaque.tga
		blendfunc blend
	}
	{
		map textures/water/noise.tga
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.01 -0.13
         nextbundle
		map textures/water/noise2.tga
		blendfunc add
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.12 0
	}

	{
		map textures/water/subpen_waves.tga
		blendfunc blend
		alphagen const 0.5
		tcMod scroll 0 0.01
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
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

