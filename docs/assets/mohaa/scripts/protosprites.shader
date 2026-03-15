crosshair
{
	spriteGen oriented
//	spriteGen parallel_oriented
	cull none   
	nomipmaps
	{
		map textures/sprites/crosshair.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

fgrenexplosion
{
	surfaceparm nolightmap
	spriteGen parallel_oriented
	spriteScale 1.5
        sort additive
 	cull none
	{
		animmap 20 textures/sprites/expl/expl0002.tga textures/sprites/expl/expl0003.tga textures/sprites/expl/expl0004.tga textures/sprites/expl/expl0005.tga textures/sprites/expl/expl0006.tga textures/sprites/expl/expl0007.tga textures/sprites/expl/expl0008.tga textures/sprites/expl/expl0009.tga textures/sprites/expl/expl0010.tga textures/sprites/expl/expl0011.tga textures/sprites/expl/expl0012.tga textures/sprites/expl/expl0013.tga textures/sprites/expl/expl0014.tga textures/sprites/expl/expl0015.tga textures/sprites/expl/expl0016.tga textures/sprites/expl/expl0017.tga textures/sprites/expl/expl0018.tga textures/sprites/expl/expl0019.tga textures/sprites/expl/expl0020.tga textures/sprites/expl/expl0021.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
//		alphaGen vertex
	}
}

fgrenshock
{
	spriteGen parallel_oriented
	cull none
	{
		map textures/sprites/shockwave2.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}