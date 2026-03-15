gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/bigchars.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//blendFunc GL_ONE GL_ZERO
		//rgbgen identity
		rgbgen vertex
	}
}

console
{
	nopicmip
	nomipmaps
   
        {
		map gfx/2d/BattleCry.tga
		blendfunc filter
                //blendFunc GL_ONE GL_ZERO
		//blendfunc blend
		//alphagen wave sin 0 10 0 10.2
		//alphafunc gt0
                //tcMod scroll .02  0
                //tcmod scale 2 1
	}

        {
                map gfx/misc/console01.tga
                blendFunc add
                tcMod turb 0 .1 0 .1
                tcMod scale 2 1
                tcmod scroll 0.2  .1
		
	} 

}

ColorBlock
{
	cull none
	nopicmip
	nomipmaps
	{
		map $whiteimage
		rgbgen vertex
		depthfunc always
	}
}

crosshair
{
	nopicmip
	nomipmaps
	{
		clampmap gfx/2d/crosshair.tga
		blendfunc blend
		alphafunc ge128
		tcMod rotate 80
	}
//	{
//		clampmap gfx/2d/crosshair.tga
//		blendfunc add
//		alphafunc ge128
//		tcMod rotate -80
//	}
}

mouse
{
	nopicmip
	nomipmaps
	{
		clampmap gfx/2d/mouse_cursor.tga
		blendfunc blend
		alphafunc ge128
	}
}
