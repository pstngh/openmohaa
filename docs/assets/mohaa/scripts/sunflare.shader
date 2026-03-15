textures/sprites/sunflare_main
//SED: Core of the sunsource
{
	surfaceparm nolightmap
	cull none
	{
		map textures/sprites/flare_main.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen fromClient
		rgbGen fromClient
		noDepthTest

	}
}

textures/sprites/corona
//SED: Core of the sunsource
{
	surfaceparm nolightmap
	cull none
	{
		map textures/sprites/flare_main.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen fromClient
		rgbGen fromClient
		noDepthTest
	}
}

textures/sprites/sunflare_middle
//SED: Middle distance from the sunsource
{
	surfaceparm nolightmap
	cull none
	{
		map textures/sprites/flare_orange2_darker_dots.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen fromClient
		rgbGen fromClient
		noDepthTest

	}
}

textures/sprites/sunflare_furthest
//SED: Furthest from the sunsource
{
	surfaceparm nolightmap
	cull none
	{
		map textures/sprites/orange_dot.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen fromClient
		rgbGen fromClient
		noDepthTest

	}
}

textures/jack/whitefade
//SED: Nasty screen overlay
{
	surfaceparm nolightmap
	cull none
	{
		map $whiteimage
		blendfunc GL_SRC_ALPHA GL_ONE
		alphaGen fromClient
		noDepthTest
	}
}