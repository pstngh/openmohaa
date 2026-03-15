textures/blasted/rebar_ww2_nofence
{
	qer_keyword pipe
	qer_keyword metal
	qer_keyword masked
	surfaceparm metal

  	qer_editorimage textures/blasted/rebar_ww2.tga
	cull none
	nopicmip

	{
		map textures/blasted/rebar_ww2.tga
		alphaFunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}

}

textures/general_industrial/ibeam_vert_nocullnone
{
	qer_keyword trim
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	qer_editorimage textures/general_industrial/ibeam_vert.tga
	{
		map textures/general_industrial/ibeam_vert.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}