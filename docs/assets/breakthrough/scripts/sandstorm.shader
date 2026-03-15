//Sandstorm


sandstorm01
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		clampmap textures/effects/steam_blur.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
		tcMod rotate 50
	}
}


sandstorm02
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		clampmap textures/effects/steam_blur.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen vertex
		rgbGen vertex
	}
}


sandstorm_gust
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		clampmap textures/fx/tehao/tehaoSandGust.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}


sandstorm_animB
{
	spritegen parallel_oriented
	nomipmaps
	cull none
	force32bit
	surfaceparm nolightmap
	surfaceparm noimpact
	{
		animmap 15 textures/sprites/sandB0001.tga textures/sprites/sandB0002.tga textures/sprites/sandB0003.tga textures/sprites/sandB0004.tga textures/sprites/sandB0005.tga textures/sprites/sandB0006.tga textures/sprites/sandB0007.tga textures/sprites/sandB0008.tga textures/sprites/sandB0009.tga textures/sprites/sandB0010.tga textures/sprites/sandB0011.tga textures/sprites/sandB0012.tga textures/sprites/sandB0013.tga textures/sprites/sandB0014.tga textures/sprites/sandB0015.tga textures/sprites/sandB0016.tga textures/sprites/sandB0017.tga textures/sprites/sandB0018.tga textures/sprites/sandB0019.tga textures/sprites/sandB0020.tga textures/sprites/sandB0021.tga textures/sprites/sandB0022.tga textures/sprites/sandB0023.tga textures/sprites/sandB0024.tga textures/sprites/sandB0025.tga textures/sprites/sandB0026.tga textures/sprites/sandB0027.tga textures/sprites/sandB0028.tga textures/sprites/sandB0029.tga textures/sprites/sandB0030.tga 
      	blendFunc blend
		rgbgen constant .75 .75 .75
	}
}


sandstorm_fadeIn
{
	spritegen parallel_oriented
	nomipmaps
	cull none
	force32bit
	surfaceparm nolightmap
	surfaceparm noimpact
	{
		animmap 15 textures/sprites/sandB0001.tga textures/sprites/sandB0002.tga textures/sprites/sandB0003.tga textures/sprites/sandB0004.tga textures/sprites/sandB0005.tga textures/sprites/sandB0006.tga textures/sprites/sandB0007.tga textures/sprites/sandB0008.tga textures/sprites/sandB0009.tga textures/sprites/sandB0010.tga textures/sprites/sandB0011.tga textures/sprites/sandB0012.tga textures/sprites/sandB0013.tga textures/sprites/sandB0014.tga textures/sprites/sandB0015.tga textures/sprites/sandB0016.tga textures/sprites/sandB0017.tga textures/sprites/sandB0018.tga textures/sprites/sandB0019.tga textures/sprites/sandB0020.tga textures/sprites/sandB0021.tga textures/sprites/sandB0022.tga textures/sprites/sandB0023.tga textures/sprites/sandB0024.tga textures/sprites/sandB0025.tga textures/sprites/sandB0026.tga textures/sprites/sandB0027.tga textures/sprites/sandB0028.tga textures/sprites/sandB0029.tga textures/sprites/sandB0030.tga 
      		blendFunc blend
		rgbgen constant .75 .75 .75
		alphaGen entity
	}
}


sandstorm_fadeOut
{
	spritegen parallel_oriented
	nomipmaps
	cull none
	force32bit
	surfaceparm nolightmap
	surfaceparm noimpact
	{
		animmap 15 textures/sprites/sandB0001.tga textures/sprites/sandB0002.tga textures/sprites/sandB0003.tga textures/sprites/sandB0004.tga textures/sprites/sandB0005.tga textures/sprites/sandB0006.tga textures/sprites/sandB0007.tga textures/sprites/sandB0008.tga textures/sprites/sandB0009.tga textures/sprites/sandB0010.tga textures/sprites/sandB0011.tga textures/sprites/sandB0012.tga textures/sprites/sandB0013.tga textures/sprites/sandB0014.tga textures/sprites/sandB0015.tga textures/sprites/sandB0016.tga textures/sprites/sandB0017.tga textures/sprites/sandB0018.tga textures/sprites/sandB0019.tga textures/sprites/sandB0020.tga textures/sprites/sandB0021.tga textures/sprites/sandB0022.tga textures/sprites/sandB0023.tga textures/sprites/sandB0024.tga textures/sprites/sandB0025.tga textures/sprites/sandB0026.tga textures/sprites/sandB0027.tga textures/sprites/sandB0028.tga textures/sprites/sandB0029.tga textures/sprites/sandB0030.tga 
      		blendFunc blend
		rgbgen constant .75 .75 .75
		alphaGen entity
	}
}


textures/sprites/sandstorm_door
{
	qer_editorimage textures/fx/doorSand01.tga
	spritegen parallel_oriented
	nomipmaps
	cull none
	force32bit
	surfaceparm nolightmap
	{
		map textures/fx/doorSand01.tga
		blendFunc blend
		tcMod turb 0 .05 0 .3
		}
	{
		map textures/fx/doorSand02.tga
		blendFunc blend
		tcMod turb 0 .035 .25 .5
		}
}


textures/fx/sandstorm_sky
{
	qer_editorimage textures/fx/sand01.tga
	nomipmaps
	cull front
	force32bit
	surfaceparm nolightmap
	surfaceparm noimpact	

	{
		map textures/fx/sand01.tga
		blendFunc blend
		tcMod scroll .1 .1
		tcMod scale 5 5
		}

	{
		map textures/fx/sand02.tga
		blendFunc blend
		tcMod scroll -.05 -.05
		tcMod scale 6 6
		}
}




