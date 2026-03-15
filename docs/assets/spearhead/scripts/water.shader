textures/water/canal_water
{
	qer_editorimage textures/water/ocean_waves_med.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceParm noimpact
	{
		map textures/water/noise.tga
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.01 -0.13
	}
	{
		map textures/water/noise2.tga
		blendfunc add
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.12 0
	}
	{
		map textures/water/noise_almost_white.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod transform 1 0 0 1 0 0
		tcMod scroll 0.003 0.1
	}
	{
		map textures/water/noise_almost_white2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod scroll 0 0.02
	}
	{
		map textures/water/ocean_waves_combo.tga
		blendfunc add
		tcMod scroll 0 0.02
	}
	{
		map textures/water/ocean_waves_cont_light.tga
		blendfunc filter
		tcMod scale 0.1 0.1
		tcMod scroll 0 0.02
	}
	{
		map textures/water/ocean_waves_cont_light2.tga
		blendfunc filter
		tcMod scale 0.1 0.1
		tcMod scroll 0.02 -0.02
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/water/subpen_water_clear
{
	qer_editorimage textures/water/ocean_waves_med_clear.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
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
		map textures/water/ocean_waves_combo.tga
		blendfunc blend
		alphagen const 0.5
		tcMod scroll 0 0.02
	}
	{
		map textures/water/ocean_waves_cont_light.tga

		tcMod scale 0.1 0.1
		tcMod scroll 0 0.02
	nextbundle
		map textures/water/ocean_waves_cont_light2.tga
		blendfunc filter
		tcMod scale 0.1 0.1
		tcMod scroll 0.02 -0.02
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/water/subpen_clear
{
	qer_editorimage textures/water/subpen_clear.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
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

textures/water/subpen_transition
{
	qer_editorimage textures/water/subpen_transition.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
	{
		map textures/water/clear_transition.tga
		blendfunc blend
//		tcmod scale 0.3 0.3
		tcmod transform 0.85 0 0 0.85 0.1 0.1
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
//		tcmod transform 0.1 0 0 0.1 1 0
	nextbundle
		map textures/water/water_shadows2.tga
		blendfunc filter
		tcMod scroll 0.07 -0.07
//		tcmod transform 0.1 0 0 0.1 1 0
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/water/subpen_opaque
{
	qer_editorimage textures/water/subpen_opaque.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword subpen
	qer_keyword add-on
	qer_keyword ocean
	surfaceParm water
	surfaceparm trans
	surfaceParm noimpact
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

