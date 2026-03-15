textures/mohmenu/dmloading/mp_unterseite.tga
{
	nomipmaps
	nopicmip
	cull none
	force32bit
	surfaceparm nolightmap

		{
		clampMap textures/mohmenu/dmloading/mp_unterseite.tga
		//blendfunc gl_one_minus_src_alpha gl_src_alpha
		}
}

textures/jon/ex_pool
{
	qer_editorimage textures/misc_outside/ocean1.tga
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
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


