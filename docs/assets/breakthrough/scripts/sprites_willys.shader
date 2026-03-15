willys_dust
{
	nopicmip
	surfaceparm nolightmap
   	spritegen parallel
	noMerge
   	cull twosided
   	surfaceparm nolightmap
      {
		map textures/sprites/willys_dust_plume.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen constant .6 .36 .25
     		alphaGen vertex
      }
}
willys_dirt
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	//noMerge
	cull twosided
	surfaceparm nolightmap
      {
		map textures/sprites/willys_dirt_plume.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen constant .5 .5 .3
		alphaGen vertex
      }
}