wake
{
	qer_editorimage textures/misc_outside/wake.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
        {
		map textures/misc_outside/oceandday.tga
		//blendFunc GL_ONE GL_ONE
		rgbGen identity
		tcMod scroll .2  .7
	}
	{
		map textures/misc_outside/oceandday1.tga
                blendfunc add
		tcMod scroll 0 .9
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scroll 0 .5
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
