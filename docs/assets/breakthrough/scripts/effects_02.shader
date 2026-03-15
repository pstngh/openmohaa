leaf_01
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		map textures/effects/leaf01.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
		rgbGen vertex
	}
}


mine_01
{
	surfaceparm metal
	{
		map textures/mines/mine_01.tga
		depthWrite
		rgbGen identity
nextbundle
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

mine_02
{
	surfaceparm metal
	{
		map textures/mines/mine_02.tga
		depthWrite
		rgbGen identity
 nextbundle
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


sm_explosion_01
{
	spriteGen parallel_oriented
	deformVertexes autoSprite
	cull none
	nopicmip
	nomipmaps
	{
		animmaponce 23 textures/sprites/sm_explosion/sm_expl0001.tga textures/sprites/sm_explosion/sm_expl0002.tga textures/sprites/sm_explosion/sm_expl0003.tga textures/sprites/sm_explosion/sm_expl0004.tga textures/sprites/sm_explosion/sm_expl0005.tga textures/sprites/sm_explosion/sm_expl0006.tga textures/sprites/sm_explosion/sm_expl0007.tga textures/sprites/sm_explosion/sm_expl0008.tga textures/sprites/sm_explosion/sm_expl0009.tga textures/sprites/sm_explosion/sm_expl0010.tga textures/sprites/sm_explosion/sm_expl0011.tga textures/sprites/sm_explosion/sm_expl0012.tga textures/sprites/sm_explosion/sm_expl0013.tga textures/sprites/sm_explosion/sm_expl0014.tga textures/sprites/sm_explosion/sm_expl0015.tga textures/sprites/sm_explosion/sm_expl0016.tga textures/sprites/sm_explosion/sm_expl0017.tga textures/sprites/sm_explosion/sm_expl0018.tga textures/sprites/sm_explosion/sm_expl0019.tga textures/sprites/sm_explosion/sm_expl0020.tga textures/sprites/sm_explosion/sm_expl0021.tga textures/sprites/sm_explosion/sm_expl0022.tga textures/sprites/sm_explosion/sm_expl0023.tga textures/sprites/sm_explosion/sm_expl0024.tga textures/sprites/sm_explosion/sm_expl0025.tga textures/sprites/sm_explosion/sm_expl0026.tga textures/sprites/sm_explosion/sm_expl0027.tga 
		blendFunc alphaadd
		rgbGen constant 1 .7 .4
	}
}


sm_explosion_02
{
	spriteGen parallel_oriented
	deformVertexes autoSprite
	cull none
	nopicmip
	nomipmaps
	{
		animMapPhase 25 .25 textures/sprites/sm_explosion/sm_expl0001.tga textures/sprites/sm_explosion/sm_expl0002.tga textures/sprites/sm_explosion/sm_expl0003.tga textures/sprites/sm_explosion/sm_expl0004.tga textures/sprites/sm_explosion/sm_expl0005.tga textures/sprites/sm_explosion/sm_expl0006.tga textures/sprites/sm_explosion/sm_expl0007.tga textures/sprites/sm_explosion/sm_expl0008.tga textures/sprites/sm_explosion/sm_expl0009.tga textures/sprites/sm_explosion/sm_expl0010.tga textures/sprites/sm_explosion/sm_expl0011.tga textures/sprites/sm_explosion/sm_expl0012.tga textures/sprites/sm_explosion/sm_expl0013.tga textures/sprites/sm_explosion/sm_expl0014.tga textures/sprites/sm_explosion/sm_expl0015.tga textures/sprites/sm_explosion/sm_expl0016.tga textures/sprites/sm_explosion/sm_expl0017.tga textures/sprites/sm_explosion/sm_expl0018.tga textures/sprites/sm_explosion/sm_expl0019.tga textures/sprites/sm_explosion/sm_expl0020.tga textures/sprites/sm_explosion/sm_expl0021.tga textures/sprites/sm_explosion/sm_expl0022.tga textures/sprites/sm_explosion/sm_expl0023.tga textures/sprites/sm_explosion/sm_expl0024.tga textures/sprites/sm_explosion/sm_expl0025.tga textures/sprites/sm_explosion/sm_expl0026.tga textures/sprites/sm_explosion/sm_expl0027.tga 
		blendFunc alphaadd
		rgbGen constant 1 .6 .3
		tcmod scale -1 1
	}
}


sm_explosion_03
{
	spriteGen parallel_oriented
	deformVertexes autoSprite
	cull none
	nopicmip
	nomipmaps
	{
		animMapPhase 23 .35 textures/sprites/sm_explosion/sm_expl0001.tga textures/sprites/sm_explosion/sm_expl0002.tga textures/sprites/sm_explosion/sm_expl0003.tga textures/sprites/sm_explosion/sm_expl0004.tga textures/sprites/sm_explosion/sm_expl0005.tga textures/sprites/sm_explosion/sm_expl0006.tga textures/sprites/sm_explosion/sm_expl0007.tga textures/sprites/sm_explosion/sm_expl0008.tga textures/sprites/sm_explosion/sm_expl0009.tga textures/sprites/sm_explosion/sm_expl0010.tga textures/sprites/sm_explosion/sm_expl0011.tga textures/sprites/sm_explosion/sm_expl0012.tga textures/sprites/sm_explosion/sm_expl0013.tga textures/sprites/sm_explosion/sm_expl0014.tga textures/sprites/sm_explosion/sm_expl0015.tga textures/sprites/sm_explosion/sm_expl0016.tga textures/sprites/sm_explosion/sm_expl0017.tga textures/sprites/sm_explosion/sm_expl0018.tga textures/sprites/sm_explosion/sm_expl0019.tga textures/sprites/sm_explosion/sm_expl0020.tga textures/sprites/sm_explosion/sm_expl0021.tga textures/sprites/sm_explosion/sm_expl0022.tga textures/sprites/sm_explosion/sm_expl0023.tga textures/sprites/sm_explosion/sm_expl0024.tga textures/sprites/sm_explosion/sm_expl0025.tga textures/sprites/sm_explosion/sm_expl0026.tga textures/sprites/sm_explosion/sm_expl0027.tga  
		blendFunc alphaadd
		rgbGen constant 1 .5 .3
		tcmod scale .75 .75
	}
}


tehaoSmoke_dark
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehaoSmoke_dark.tga
		blendfunc blend
		rgbGen vertex
	}
}



tehaoSmoke_light
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehaoSmoke_light.tga
		blendfunc blend
		rgbGen vertex
	}
}
tehaoSmoke_light_up
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/tehao/tehaoSmoke_light.tga
		blendfunc blend
		rgbGen vertex
	}
}
BridgeCharge_up
{
	cull none
	sort nearest
	spriteGen parallel_upright
	{
		clampmap textures/fx/bridge/bridgeCharge.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}
tehaoSmoke_mix
{
	spriteGen parallel_oriented
	nopicmip
	surfaceparm nolightmap
	noMerge
	cull twosided
	{
		clampmap textures/fx/tehao/tehaoSmoke_light02.tga
		blendfunc blend
		rgbGen identity
		tcMod rotate 40
	nextbundle
		clampmap textures/fx/tehao/tehaoSmoke_light03.tga
		rgbGen identity
		tcMod rotate -40
	}
}


Transparency
{
		surfaceparm nolightmap
	{
		map textures/fx/tehao/transparency.tga
		blendfunc blend
		alphaGen constant 0
	}
}


tehaoRipple
{
	spriteGen oriented
	cull none
	{
		map textures/fx/tehao/tehaoRipple_02.tga
		blendFunc GL_SRC_ALPHA GL_ONE 
		alphaGen vertex
		rgbGen vertex
	}
}



tehaoRipple_sub
{
	spriteGen oriented
	cull none
	{
		map textures/fx/tehao/tehaoRipple_01.tga
		blendFunc GL_SRC_ALPHA GL_ONE 
		alphaGen vertex
		rgbGen vertex
	}
}



waterPuddle
{
	surfaceparm nolightmap
	cull none
	{
		map textures/fx/tehao/waterPuddle.tga
		blendFunc blend
		alphaGen dot .2 .5
		rgbGen vertex
	}
}


waterDrop_01
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		map textures/fx/tehao/tehao_waterDrop1.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}


waterDrop_02
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehao_waterDrop2.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}


waterDrop_03
{
	cull none
	sort nearest
	spriteGen parallel
	{
		clampmap textures/fx/tehao/tehao_waterDrop3.tga
		blendfunc blend
		rgbGen vertex
	}
}

waterDrop_04
{
	cull none
	sort nearest
	spriteGen parallel
	{
		clampmap textures/fx/tehao/tehao_waterDrop3.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

waterDrop_05
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		map textures/fx/tehao/tehao_waterDrop1.tga
		blendFunc alphaadd
		alphaGen vertex
		rgbGen vertex
	}
}


waterStreak
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	surfaceparm water
	surfaceparm nolightmap
	{
		clampmap textures/fx/tehao/streaky.tga
		blendfunc blend
		rgbGen vertex
	}
}


waterMist
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehaoMist.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}



coronaRed_animated
{
	deformVertexes lightglow
	cull none
	{
		//animmapPhase 20 .8 textures/fx/tehao/coronaRed_animated/coronaRed_animated0000.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0001.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0002.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0003.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0004.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0005.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0006.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0007.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0008.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0009.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0010.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0011.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0012.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0013.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0014.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0015.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0016.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0017.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0018.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0019.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0020.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0021.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0022.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0023.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0024.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0025.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0025.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0026.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0027.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0028.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0029.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0030.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0031.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0032.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0033.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0034.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0035.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0036.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0037.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0038.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0039.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0040.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0041.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0042.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0043.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0044.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0045.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0046.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0047.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0048.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0049.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0050.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0051.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0052.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0053.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0054.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0055.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0056.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0057.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0058.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0059.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0060.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0061.tga textures/fx/tehao/coronaRed_animated/coronaRed_animated0062.tga 
		map textures/sprites/corona_red.tga
		blendFunc alphaadd
		alphaGen wave sin .5 .5 0 .5
	}
}


flicker_light
{
	polygonoffset
	{
		map textures/decals/red_flash.tga
		blendFunc alphaadd
		alphaGen wave sin .5 1 0 8
		rgbGen vertex
	}
}


rollingRocks
{
	cull none
	spriteGen parallel_oriented
	{
		animMap 45 textures/fx/tehao/rocks_animated/rock0000.tga textures/fx/tehao/rocks_animated/rock0001.tga textures/fx/tehao/rocks_animated/rock0002.tga textures/fx/tehao/rocks_animated/rock0003.tga textures/fx/tehao/rocks_animated/rock0004.tga textures/fx/tehao/rocks_animated/rock0005.tga textures/fx/tehao/rocks_animated/rock0006.tga textures/fx/tehao/rocks_animated/rock0007.tga textures/fx/tehao/rocks_animated/rock0008.tga textures/fx/tehao/rocks_animated/rock0009.tga textures/fx/tehao/rocks_animated/rock0010.tga textures/fx/tehao/rocks_animated/rock0011.tga textures/fx/tehao/rocks_animated/rock0012.tga textures/fx/tehao/rocks_animated/rock0013.tga textures/fx/tehao/rocks_animated/rock0014.tga textures/fx/tehao/rocks_animated/rock0015.tga textures/fx/tehao/rocks_animated/rock0016.tga textures/fx/tehao/rocks_animated/rock0017.tga textures/fx/tehao/rocks_animated/rock0018.tga textures/fx/tehao/rocks_animated/rock0019.tga
		blendFunc blend
	}
}


tehaoDebris_01
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		map textures/fx/tehao/debris_01.tga
		blendFunc blend
		rgbGen vertex
		tcMod rotate 100
	}
}


tehao_candelLight_01
{
	spritegen parallel_oriented
	cull none
	sort nearest
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		animmapPhase 30 0 textures/fx/tehao/candelLight_02/candleLight0000.tga textures/fx/tehao/candelLight_02/candleLight0001.tga textures/fx/tehao/candelLight_02/candleLight0002.tga textures/fx/tehao/candelLight_02/candleLight0003.tga textures/fx/tehao/candelLight_02/candleLight0004.tga textures/fx/tehao/candelLight_02/candleLight0005.tga textures/fx/tehao/candelLight_02/candleLight0006.tga textures/fx/tehao/candelLight_02/candleLight0007.tga textures/fx/tehao/candelLight_02/candleLight0008.tga textures/fx/tehao/candelLight_02/candleLight0009.tga textures/fx/tehao/candelLight_02/candleLight0010.tga textures/fx/tehao/candelLight_02/candleLight0011.tga textures/fx/tehao/candelLight_02/candleLight0012.tga textures/fx/tehao/candelLight_02/candleLight0013.tga textures/fx/tehao/candelLight_02/candleLight0014.tga textures/fx/tehao/candelLight_02/candleLight0015.tga textures/fx/tehao/candelLight_02/candleLight0016.tga textures/fx/tehao/candelLight_02/candleLight0017.tga textures/fx/tehao/candelLight_02/candleLight0018.tga textures/fx/tehao/candelLight_02/candleLight0019.tga textures/fx/tehao/candelLight_02/candleLight0020.tga textures/fx/tehao/candelLight_02/candleLight0021.tga textures/fx/tehao/candelLight_02/candleLight0022.tga textures/fx/tehao/candelLight_02/candleLight0023.tga textures/fx/tehao/candelLight_02/candleLight0024.tga textures/fx/tehao/candelLight_02/candleLight0025.tga textures/fx/tehao/candelLight_02/candleLight0026.tga textures/fx/tehao/candelLight_02/candleLight0027.tga textures/fx/tehao/candelLight_02/candleLight0028.tga textures/fx/tehao/candelLight_02/candleLight0029.tga textures/fx/tehao/candelLight_02/candleLight0030.tga textures/fx/tehao/candelLight_02/candleLight0031.tga textures/fx/tehao/candelLight_02/candleLight0032.tga textures/fx/tehao/candelLight_02/candleLight0033.tga textures/fx/tehao/candelLight_02/candleLight0034.tga textures/fx/tehao/candelLight_02/candleLight0035.tga textures/fx/tehao/candelLight_02/candleLight0036.tga textures/fx/tehao/candelLight_02/candleLight0037.tga textures/fx/tehao/candelLight_02/candleLight0038.tga textures/fx/tehao/candelLight_02/candleLight0039.tga textures/fx/tehao/candelLight_02/candleLight0040.tga textures/fx/tehao/candelLight_02/candleLight0041.tga textures/fx/tehao/candelLight_02/candleLight0042.tga textures/fx/tehao/candelLight_02/candleLight0043.tga textures/fx/tehao/candelLight_02/candleLight0044.tga textures/fx/tehao/candelLight_02/candleLight0045.tga textures/fx/tehao/candelLight_02/candleLight0046.tga textures/fx/tehao/candelLight_02/candleLight0047.tga textures/fx/tehao/candelLight_02/candleLight0048.tga textures/fx/tehao/candelLight_02/candleLight0049.tga textures/fx/tehao/candelLight_02/candleLight0050.tga textures/fx/tehao/candelLight_02/candleLight0051.tga textures/fx/tehao/candelLight_02/candleLight0052.tga textures/fx/tehao/candelLight_02/candleLight0053.tga textures/fx/tehao/candelLight_02/candleLight0054.tga textures/fx/tehao/candelLight_02/candleLight0055.tga textures/fx/tehao/candelLight_02/candleLight0056.tga textures/fx/tehao/candelLight_02/candleLight0057.tga textures/fx/tehao/candelLight_02/candleLight0058.tga textures/fx/tehao/candelLight_02/candleLight0059.tga textures/fx/tehao/candelLight_02/candleLight0060.tga textures/fx/tehao/candelLight_02/candleLight0061.tga textures/fx/tehao/candelLight_02/candleLight0062.tga textures/fx/tehao/candelLight_02/candleLight0063.tga
		blendfunc alphaadd
		rgbGen vertex
	 }
}

tehao_candelLight_02
{
	spritegen parallel_oriented
	cull none
	sort nearest
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		animmapPhase 30 .3 textures/fx/tehao/candelLight_02/candleLight0000.tga textures/fx/tehao/candelLight_02/candleLight0001.tga textures/fx/tehao/candelLight_02/candleLight0002.tga textures/fx/tehao/candelLight_02/candleLight0003.tga textures/fx/tehao/candelLight_02/candleLight0004.tga textures/fx/tehao/candelLight_02/candleLight0005.tga textures/fx/tehao/candelLight_02/candleLight0006.tga textures/fx/tehao/candelLight_02/candleLight0007.tga textures/fx/tehao/candelLight_02/candleLight0008.tga textures/fx/tehao/candelLight_02/candleLight0009.tga textures/fx/tehao/candelLight_02/candleLight0010.tga textures/fx/tehao/candelLight_02/candleLight0011.tga textures/fx/tehao/candelLight_02/candleLight0012.tga textures/fx/tehao/candelLight_02/candleLight0013.tga textures/fx/tehao/candelLight_02/candleLight0014.tga textures/fx/tehao/candelLight_02/candleLight0015.tga textures/fx/tehao/candelLight_02/candleLight0016.tga textures/fx/tehao/candelLight_02/candleLight0017.tga textures/fx/tehao/candelLight_02/candleLight0018.tga textures/fx/tehao/candelLight_02/candleLight0019.tga textures/fx/tehao/candelLight_02/candleLight0020.tga textures/fx/tehao/candelLight_02/candleLight0021.tga textures/fx/tehao/candelLight_02/candleLight0022.tga textures/fx/tehao/candelLight_02/candleLight0023.tga textures/fx/tehao/candelLight_02/candleLight0024.tga textures/fx/tehao/candelLight_02/candleLight0025.tga textures/fx/tehao/candelLight_02/candleLight0026.tga textures/fx/tehao/candelLight_02/candleLight0027.tga textures/fx/tehao/candelLight_02/candleLight0028.tga textures/fx/tehao/candelLight_02/candleLight0029.tga textures/fx/tehao/candelLight_02/candleLight0030.tga textures/fx/tehao/candelLight_02/candleLight0031.tga textures/fx/tehao/candelLight_02/candleLight0032.tga textures/fx/tehao/candelLight_02/candleLight0033.tga textures/fx/tehao/candelLight_02/candleLight0034.tga textures/fx/tehao/candelLight_02/candleLight0035.tga textures/fx/tehao/candelLight_02/candleLight0036.tga textures/fx/tehao/candelLight_02/candleLight0037.tga textures/fx/tehao/candelLight_02/candleLight0038.tga textures/fx/tehao/candelLight_02/candleLight0039.tga textures/fx/tehao/candelLight_02/candleLight0040.tga textures/fx/tehao/candelLight_02/candleLight0041.tga textures/fx/tehao/candelLight_02/candleLight0042.tga textures/fx/tehao/candelLight_02/candleLight0043.tga textures/fx/tehao/candelLight_02/candleLight0044.tga textures/fx/tehao/candelLight_02/candleLight0045.tga textures/fx/tehao/candelLight_02/candleLight0046.tga textures/fx/tehao/candelLight_02/candleLight0047.tga textures/fx/tehao/candelLight_02/candleLight0048.tga textures/fx/tehao/candelLight_02/candleLight0049.tga textures/fx/tehao/candelLight_02/candleLight0050.tga textures/fx/tehao/candelLight_02/candleLight0051.tga textures/fx/tehao/candelLight_02/candleLight0052.tga textures/fx/tehao/candelLight_02/candleLight0053.tga textures/fx/tehao/candelLight_02/candleLight0054.tga textures/fx/tehao/candelLight_02/candleLight0055.tga textures/fx/tehao/candelLight_02/candleLight0056.tga textures/fx/tehao/candelLight_02/candleLight0057.tga textures/fx/tehao/candelLight_02/candleLight0058.tga textures/fx/tehao/candelLight_02/candleLight0059.tga textures/fx/tehao/candelLight_02/candleLight0060.tga textures/fx/tehao/candelLight_02/candleLight0061.tga textures/fx/tehao/candelLight_02/candleLight0062.tga textures/fx/tehao/candelLight_02/candleLight0063.tga
		blendfunc alphaadd
		rgbGen vertex
	 }
}

tehao_candelLight_03
{
	spritegen parallel_oriented
	cull none
	sort nearest
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		animmapPhase 30 .7 textures/fx/tehao/candelLight_02/candleLight0000.tga textures/fx/tehao/candelLight_02/candleLight0001.tga textures/fx/tehao/candelLight_02/candleLight0002.tga textures/fx/tehao/candelLight_02/candleLight0003.tga textures/fx/tehao/candelLight_02/candleLight0004.tga textures/fx/tehao/candelLight_02/candleLight0005.tga textures/fx/tehao/candelLight_02/candleLight0006.tga textures/fx/tehao/candelLight_02/candleLight0007.tga textures/fx/tehao/candelLight_02/candleLight0008.tga textures/fx/tehao/candelLight_02/candleLight0009.tga textures/fx/tehao/candelLight_02/candleLight0010.tga textures/fx/tehao/candelLight_02/candleLight0011.tga textures/fx/tehao/candelLight_02/candleLight0012.tga textures/fx/tehao/candelLight_02/candleLight0013.tga textures/fx/tehao/candelLight_02/candleLight0014.tga textures/fx/tehao/candelLight_02/candleLight0015.tga textures/fx/tehao/candelLight_02/candleLight0016.tga textures/fx/tehao/candelLight_02/candleLight0017.tga textures/fx/tehao/candelLight_02/candleLight0018.tga textures/fx/tehao/candelLight_02/candleLight0019.tga textures/fx/tehao/candelLight_02/candleLight0020.tga textures/fx/tehao/candelLight_02/candleLight0021.tga textures/fx/tehao/candelLight_02/candleLight0022.tga textures/fx/tehao/candelLight_02/candleLight0023.tga textures/fx/tehao/candelLight_02/candleLight0024.tga textures/fx/tehao/candelLight_02/candleLight0025.tga textures/fx/tehao/candelLight_02/candleLight0026.tga textures/fx/tehao/candelLight_02/candleLight0027.tga textures/fx/tehao/candelLight_02/candleLight0028.tga textures/fx/tehao/candelLight_02/candleLight0029.tga textures/fx/tehao/candelLight_02/candleLight0030.tga textures/fx/tehao/candelLight_02/candleLight0031.tga textures/fx/tehao/candelLight_02/candleLight0032.tga textures/fx/tehao/candelLight_02/candleLight0033.tga textures/fx/tehao/candelLight_02/candleLight0034.tga textures/fx/tehao/candelLight_02/candleLight0035.tga textures/fx/tehao/candelLight_02/candleLight0036.tga textures/fx/tehao/candelLight_02/candleLight0037.tga textures/fx/tehao/candelLight_02/candleLight0038.tga textures/fx/tehao/candelLight_02/candleLight0039.tga textures/fx/tehao/candelLight_02/candleLight0040.tga textures/fx/tehao/candelLight_02/candleLight0041.tga textures/fx/tehao/candelLight_02/candleLight0042.tga textures/fx/tehao/candelLight_02/candleLight0043.tga textures/fx/tehao/candelLight_02/candleLight0044.tga textures/fx/tehao/candelLight_02/candleLight0045.tga textures/fx/tehao/candelLight_02/candleLight0046.tga textures/fx/tehao/candelLight_02/candleLight0047.tga textures/fx/tehao/candelLight_02/candleLight0048.tga textures/fx/tehao/candelLight_02/candleLight0049.tga textures/fx/tehao/candelLight_02/candleLight0050.tga textures/fx/tehao/candelLight_02/candleLight0051.tga textures/fx/tehao/candelLight_02/candleLight0052.tga textures/fx/tehao/candelLight_02/candleLight0053.tga textures/fx/tehao/candelLight_02/candleLight0054.tga textures/fx/tehao/candelLight_02/candleLight0055.tga textures/fx/tehao/candelLight_02/candleLight0056.tga textures/fx/tehao/candelLight_02/candleLight0057.tga textures/fx/tehao/candelLight_02/candleLight0058.tga textures/fx/tehao/candelLight_02/candleLight0059.tga textures/fx/tehao/candelLight_02/candleLight0060.tga textures/fx/tehao/candelLight_02/candleLight0061.tga textures/fx/tehao/candelLight_02/candleLight0062.tga textures/fx/tehao/candelLight_02/candleLight0063.tga
		blendfunc alphaadd
		rgbGen vertex
	 }
}



corona_brokenLight01
{
	deformVertexes lightglow
	cull none
	{
		animMap 30 textures/fx/tehao/corona_brokenLight/corona_brokenLight0001.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0002.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0003.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0004.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0005.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0006.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0007.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0008.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0009.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0010.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0011.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0012.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0013.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0014.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0015.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0016.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0017.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0018.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0019.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0020.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0021.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0022.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0023.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0024.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0025.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0025.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0026.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0027.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0028.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0029.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0030.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0031.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0032.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0033.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0034.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0035.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0036.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0037.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0038.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0039.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0040.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0041.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0042.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0043.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0044.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0045.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0046.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0047.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0048.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0049.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0050.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0051.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0052.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0053.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0054.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0055.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0056.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0057.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0058.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0059.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0060.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0061.tga textures/fx/tehao/corona_brokenLight/corona_brokenLight0062.tga 
		blendFunc add
		tcMod rotate 4
	}
}

cinematicExplosion_02
{
	spriteGen parallel_upright
	cull none
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		animmap 20 textures/effects/bidsh01.tga textures/effects/bidsh02.tga textures/effects/bidsh03.tga textures/effects/bidsh04.tga textures/effects/bidsh05.tga textures/effects/bidsh06.tga textures/effects/bidsh07.tga textures/effects/bidsh08.tga textures/effects/bidsh09.tga textures/effects/bidsh10.tga textures/effects/bidsh11.tga textures/effects/bidsh12.tga textures/effects/bidsh13.tga textures/effects/bidsh14.tga textures/effects/bidsh15.tga textures/effects/bidsh16.tga textures/effects/bidsh17.tga textures/effects/bidsh18.tga textures/effects/bidsh19.tga
		blendfunc alphaadd
	}
}


cinematicExplosion_03
{
	spriteGen parallel_upright
	cull none
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		animmap 15 textures/effects/hit01.tga textures/effects/hit02.tga textures/effects/hit03.tga textures/effects/hit04.tga textures/effects/hit05.tga textures/effects/hit06.tga textures/effects/hit07.tga textures/effects/hit08.tga textures/effects/hit09.tga textures/effects/hit10.tga textures/effects/hit11.tga textures/effects/hit12.tga textures/effects/hit13.tga textures/effects/hit14.tga textures/effects/hit15.tga textures/effects/hit16.tga textures/effects/hit17.tga textures/effects/hit18.tga textures/effects/hit19.tga
		blendfunc alphaadd
		rgbGen identity
	}
}

cinematic_sparks
{
	spriteGen parallel_oriented
	cull none
	{
		animMap 17 textures/effects/gnats0000.tga textures/effects/gnats0001.tga textures/effects/gnats0002.tga textures/effects/gnats0003.tga textures/effects/gnats0004.tga textures/effects/gnats0005.tga 
		blendfunc alphaadd
		rgbGen vertex
	}
}

feather_01
{
	spriteGen parallel_oriented
	cull none
	{
		map textures/fx/tehao/feather01.tga
		rgbGen vertex
		blendFunc blend
	}
}

feather_02
{
	spriteGen oriented
	cull none
	{
		map textures/fx/tehao/feather02.tga
		rgbGen vertex
		blendFunc blend 
	}
}

smokeTrail_fire
{
	spriteGen parallel_oriented
	cull none
	{
		clampmap textures/fx/C47_exp/C47_fire_01.tga
		blendfunc alphaadd
		rgbGen identity
		alphaGen wave sin .5 1 0 10
		tcMod rotate -230
	}
}

burst
{
	cull none
	spritegen parallel
	{
		clampmap textures/fx/tehao/burst.tga
		blendfunc alphaadd
		rgbGen const 1 .7 .4
		tcMod rotate 90
	}
}

shootSparks
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/generalSpark_01.tga
		blendfunc alphaadd
		rgbGen wave sin .5 .8 0 10
	}
}











