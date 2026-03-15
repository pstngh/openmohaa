vsssource
{
	nopicmip
	surfaceparm nolightmap
//	spritegen parallel_oriented
	spritegen parallel
	noMerge
	cull twosided
	{
		clampmap textures/sprites/vsssource.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod rotate 20
		rgbGen vertex
		alphaGen vertex
	nextbundle
		map textures/sprites/vsssource2.tga
		tcMod rotate -20
	}
}

vsssource2
{
	nopicmip
	surfaceparm nolightmap
//	spritegen parallel_oriented
	spritegen parallel
	noMerge
	cull twosided
	{
		clampmap textures/sprites/vsssource.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod rotate -20
		rgbGen vertex
		alphaGen vertex
	nextbundle
		map textures/sprites/vsssource2.tga
		tcMod rotate 20
	}
}

corona_reg
{
	qer_editorimage textures/sprites/corona_reg.tga
	deformVertexes lightglow
	cull none
	{
		map textures/sprites/corona_reg.tga
		blendfunc add
	}
}
corona_red
{
	qer_editorimage textures/sprites/corona_red.tga
	deformVertexes lightglow
	cull none
	{
		map textures/sprites/corona_red.tga
		blendfunc add
	}
}

corona_util
{
	qer_editorimage textures/sprites/corona_util.tga
	deformVertexes lightglow
	cull none
	{
		map textures/sprites/corona_util.tga
		blendfunc add
	}
}

grenexp_flash
{
	spritegen parallel_oriented
	cull none
	{
		map textures/sprites/corona_reg.tga
		blendfunc GL_SRC_ALPHA GL_ONE
		rgbGen vertex
	}
}

cardboard_light
{
	spritegen oriented
	cull none
	{
		map textures/sprites/cardboard_light.tga
		blendfunc blend
		rgbGen vertex
	}
}

textures/sprites/metalshard_heavy
{
	spritegen oriented
	cull none
	{
		map textures/sprites/metalshard_heavy.tga
		blendfunc blend
		rgbGen vertex
	}
}

textures/sprites/metalshard_med
{
	spritegen oriented
	cull none
	{
		map textures/sprites/metalshard_med.tga
		blendfunc blend
		rgbGen vertex
	}
}

textures/sprites/metalshard_small
{
	spritegen oriented
	cull none
	{
		map textures/sprites/metalshard_small.tga
		blendfunc blend
		rgbGen vertex
	}
}

gren_boom
{
	spritegen parallel_oriented
	cull none
	{
		animmap 20 textures/effects/boom1.tga textures/effects/boom2.tga textures/effects/boom3.tga textures/effects/boom4.tga textures/effects/boom5.tga textures/effects/boom6.tga textures/effects/boom7.tga
		blendfunc add
	}
} 

air_explosion
{
	spriteGen parallel_oriented
        cull none
	{
                     animmap 10 textures/sprites/expl/airexp_001.tga textures/sprites/expl/airexp_002.tga textures/sprites/expl/airexp_003.tga textures/sprites/expl/airexp_004.tga textures/sprites/expl/airexp_005.tga textures/sprites/expl/airexp_006.tga textures/sprites/expl/airexp_007.tga textures/sprites/expl/airexp_008.tga textures/sprites/expl/airexp_009.tga textures/sprites/expl/airexp_010.tga 
       	        blendFunc add 
	}
} 


//_____________________________________________________Flame_________________________________________________//

fire_front
{
	qer_editorimage textures/effects/flamer_01.tga
	nopicmip
	nomipmaps
	deformVertexes autosprite
	cull none
	{
		animMapPhase 15 0 textures/effects/flamer_01.tga textures/effects/flamer_02.tga textures/effects/flamer_03.tga textures/effects/flamer_04.tga textures/effects/flamer_05.tga textures/effects/flamer_06.tga textures/effects/flamer_07.tga textures/effects/flamer_08.tga textures/effects/flamer_09.tga textures/effects/flamer_10.tga textures/effects/flamer_11.tga textures/effects/flamer_12.tga textures/effects/flamer_13.tga textures/effects/flamer_14.tga textures/effects/flamer_15.tga textures/effects/flamer_16.tga textures/effects/flamer_17.tga textures/effects/flamer_18.tga textures/effects/flamer_19.tga textures/effects/flamer_20.tga textures/effects/flamer_21.tga textures/effects/flamer_22.tga textures/effects/flamer_23.tga textures/effects/flamer_24.tga textures/effects/flamer_25.tga
		blendfunc add
	}
}

fire_middle
{
	qer_editorimage textures/effects/flamer_01.tga
	nopicmip
	nomipmaps
	deformVertexes autosprite
	cull none
	{
		animMapPhase 15 .2222222 textures/effects/flamer_01.tga textures/effects/flamer_02.tga textures/effects/flamer_03.tga textures/effects/flamer_04.tga textures/effects/flamer_05.tga textures/effects/flamer_06.tga textures/effects/flamer_07.tga textures/effects/flamer_08.tga textures/effects/flamer_09.tga textures/effects/flamer_10.tga textures/effects/flamer_11.tga textures/effects/flamer_12.tga textures/effects/flamer_13.tga textures/effects/flamer_14.tga textures/effects/flamer_15.tga textures/effects/flamer_16.tga textures/effects/flamer_17.tga textures/effects/flamer_18.tga textures/effects/flamer_19.tga textures/effects/flamer_20.tga textures/effects/flamer_21.tga textures/effects/flamer_22.tga textures/effects/flamer_23.tga textures/effects/flamer_24.tga textures/effects/flamer_25.tga
		blendfunc add
		tcmod scale -1 1
	}
}

fire_back
{
	qer_editorimage textures/effects/flamer_01.tga
	nopicmip
	nomipmaps
	deformVertexes autosprite
	cull none
	{
		animMapPhase 15 .5777778 textures/effects/flamer_01.tga textures/effects/flamer_02.tga textures/effects/flamer_03.tga textures/effects/flamer_04.tga textures/effects/flamer_05.tga textures/effects/flamer_06.tga textures/effects/flamer_07.tga textures/effects/flamer_08.tga textures/effects/flamer_09.tga textures/effects/flamer_10.tga textures/effects/flamer_11.tga textures/effects/flamer_12.tga textures/effects/flamer_13.tga textures/effects/flamer_14.tga textures/effects/flamer_15.tga textures/effects/flamer_16.tga textures/effects/flamer_17.tga textures/effects/flamer_18.tga textures/effects/flamer_19.tga textures/effects/flamer_20.tga textures/effects/flamer_21.tga textures/effects/flamer_22.tga textures/effects/flamer_23.tga textures/effects/flamer_24.tga textures/effects/flamer_25.tga
		blendfunc add
	}
} 

lampflame
{
	qer_editorimage textures/sprites/lamplight_1.tga
	nopicmip
	nomipmaps
	deformVertexes autosprite
	cull none
	{
		animMapPhase 15 0 textures/sprites/lamplight_1.tga textures/sprites/lamplight_2.tga textures/sprites/lamplight_3.tga textures/sprites/lamplight_4.tga textures/sprites/lamplight_5.tga textures/sprites/lamplight_6.tga textures/sprites/lamplight_7.tga 
		blendfunc add
	}
}


//_____________________________________________________Fakk2_________________________________________________//

// Fakk2 sprites below
frost
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap textures/sprites/smoke1-0000.tga textures/sprites/smoke1-0001.tga textures/sprites/smoke1-0002.tga textures/sprites/smoke1-0003.tga textures/sprites/smoke1-0004.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}


bubble
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/bubble.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

gasplant1
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/gasplant1.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}


bspray
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/bspray.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}


bloodeffect
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/meteorfire.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}



lochnar_pals
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/eyeglow.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}


fx_portal
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/fx_portal.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}



fakkportal
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/portal.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

fakkportal2
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/portal2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

soundwave
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/soundwave.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}



yellowspark
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/yellowspark.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

dustpoof
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/dustpoof.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

dustring
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/dustring.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

under_steam1
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/smoke3-0004.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

under_steam2
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/splashring.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

under_steam3
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/smoke3-0004.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}


under_swordeffect
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/splashring3.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

splashring
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/splashring.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

triangle
{
   cull twosided
   surfaceparm nolightmap
      {
      map $whiteimage
      rgbGen fromClient
      alphaGen fromClient
      }
}

flamesprite
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/flame.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

greenlight
{
   spritegen parallel_upright
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/greenlight.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

redbeam
{
   spritegen parallel_upright
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/redbeam.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

redbeam2
{
   spritegen parallel_upright
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/redbeam2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

bluebeam
{
   spritegen parallel_upright
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/bluebeam.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

lochnar
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
//      map textures/sprites/lochnar.tga
      map textures/sprites/lochnarbak2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

rain
{
   spritegen parallel_upright
   spritescale .5
   cull twosided
   surfaceparm nolightmap
   nomipmaps
      {
      map textures/sprites/rainbak.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//    rgbGen vertex
      }
}

oriented_rain
{
   spritegen oriented
   spritescale .5
   cull twosided
   surfaceparm nolightmap
   nomipmaps
      {
      map textures/sprites/rainbak.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      }
}

puff
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/puff.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

pee
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/pee.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

vsparkparticle
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/redparticle.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

eye
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/eye.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

milk
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/milk.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

spore
{
   nomipmaps
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/spore.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      //rgbGen vertex
      }
}

splash_y
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/rainy.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

splash_z
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/rainz.tga
      blendFunc GL_SRC_ALPHA GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

fruitglow
{
   spritegen parallel_oriented
   cull twosided
   spritescale 1.5
   surfaceparm nolightmap
      {
      map textures/sprites/fruitglow.tga
      blendFunc GL_ONE GL_ONE
      rgbGen wave sin .5 .5 0 .5
      alphaGen vertex
      }
}

crystal
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/crystal.tga
      blendFunc GL_ONE GL_ONE
      rgbGen wave sin .95 .1 0 2
//      rgbGen vertex
      alphaGen vertex
      }
}

crystal2
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/crystal2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}
crystalshard
{
   surfaceparm nolightmap
      {
      map models/items/crystalshard/crystalshard.tga
      }
}

mushglow
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/mushglow.tga
      blendFunc GL_ONE GL_ONE
      rgbGen wave sin .5 .2 0 .3
      alphaGen vertex
      }
}


eden_smokestack
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/smoke3-0000.tga textures/sprites/smoke3-0001.tga textures/sprites/smoke3-0002.tga textures/sprites/smoke3-0003.tga textures/sprites/smoke3-0004.tga
      blendFunc GL_ONE GL_ONE_MINUS_SRC_COLOR
      rgbGen vertex
      alphaGen vertex
      }
}

blacksmoke
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/smoke1-0000.tga textures/sprites/smoke1-0001.tga textures/sprites/smoke1-0002.tga textures/sprites/smoke1-0003.tga textures/sprites/smoke1-0004.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

greenblood2
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/greenblood2-0000.tga textures/sprites/greenblood2-0001.tga textures/sprites/greenblood2-0002.tga textures/sprites/greenblood2-0003.tga textures/sprites/greenblood2-0004.tga textures/sprites/greenblood2-0005.tga textures/sprites/greenblood2-0006.tga textures/sprites/greenblood2-0007.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

lightning1
{
   spritegen parallel_upright
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/lightning1-0000.tga textures/sprites/lightning1-0001.tga textures/sprites/lightning1-0002.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

graysmoke
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/smoke3-0000.tga textures/sprites/smoke3-0001.tga textures/sprites/smoke3-0002.tga textures/sprites/smoke3-0003.tga textures/sprites/smoke3-0004.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

graysmoke_gruff
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
   nomipmaps
      {
      animmap textures/sprites/smoke3-0000.tga textures/sprites/smoke3-0001.tga textures/sprites/smoke3-0002.tga textures/sprites/smoke3-0003.tga textures/sprites/smoke3-0004.tga
      blendFunc GL_ONE GL_ONE_MINUS_SRC_COLOR
      rgbGen vertex
      alphaGen vertex
      }
}

greensmoke
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap textures/sprites/smoke1-0000.tga textures/sprites/smoke1-0001.tga textures/sprites/smoke1-0002.tga textures/sprites/smoke1-0003.tga textures/sprites/smoke1-0004.tga
      blendFunc GL_ONE GL_ONE
      rgbGen constant 0 .2 0
      alphaGen vertex
      }
}

greensmoke2
{
//   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/gassy-smoke.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

greensmoke3
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/gassy-smoke2.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

whitesmoke
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/smoke1-0000.tga textures/sprites/smoke1-0001.tga textures/sprites/smoke1-0002.tga textures/sprites/smoke1-0003.tga textures/sprites/smoke1-0004.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

fall
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/fall.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

lochbloodsmoke
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      animmap textures/sprites/lochbloodsmoke-0000.tga textures/sprites/lochbloodsmoke-0001.tga textures/sprites/lochbloodsmoke-0002.tga textures/sprites/lochbloodsmoke-0003.tga textures/sprites/lochbloodsmoke-0004.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

fire1
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/fireball.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

fire2
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/fireball2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

fire3
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/fireball3.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

fire4
{
   cull twosided
   surfaceparm nolightmap
   sort additive
      {
      map textures/sprites/firepart2.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

fire4d
{
   cull twosided
   surfaceparm nolightmap
   sort additive
      {
      map textures/sprites/firepart2d.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

firesmoke
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/expsmoke01.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

volcanofire
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/volcanofire.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

volcanosmoke
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/volcanosmoke1.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

fire5
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      animmap 10 textures/sprites/fireframe1.tga textures/sprites/fireframe2.tga textures/sprites/fireframe3.tga textures/sprites/fireframe4.tga textures/sprites/fireframe5.tga textures/sprites/fireframe6.tga textures/sprites/fireframe7.tga textures/sprites/fireframe8.tga textures/sprites/fireframe9.tga textures/sprites/fireframe10.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

textures/aldiewater
{
   cull none
//   deform noiseanim 10
   tessSize 48
   surfaceparm nolightmap
   surfaceparm nonsolid
   surfaceparm trans
   surfaceparm water
   qer_editorimage textures/phook/water_grainy.tga
      {
      map textures/phook/water_grainy.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      tcMod turb 0.300000 0.300000 0.000000 0.100000
      }
}

cherry
{
    surfaceparm nolightmap
    subdivisions 4
    {
    map $whiteimage
    }

    {
    tcMod scale 4 4
    map "models/npc/gruff/ash.tga"
    blendFunc GL_DST_COLOR GL_ZERO
    rgbGen fromentity
    }
}

gruffglasses
{
   {
   map models/npc/gruff/gruffskin1.tga
   alphaGen constant 0.5
   rgbGen lightingGrid
   blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
   }
   {
	map textures/phook/envmap1.tga
	tcGen environment
	alphaGen constant .5
	rgbGen lightingGrid
	blendFunc GL_SRC_ALPHA GL_ONE
   }
}


blood
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/blood.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

blood2
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/blood2.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

blood3
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/blood3.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

blood4
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/blood4.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      alphaGen vertex
      }
}

droplet
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/droplet.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

under_droplet
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/under_droplet.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

dropletlarge
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/dropletlarge.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      }
}

weldsparks
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/weldspark.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

sparkcenter
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/sparkcenter.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

greenblood
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/greenblood.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

greenblood2
{
   spritegen parallel_oriented
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/greenblood2.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      }
}

bloodsplat
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
   spritescale 2.0
   sort decal
      {
      map textures/sprites/bloodsplat.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	alphaGen vertex
      }
}

bluesplat
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
   sort decal
//   spritescale 2.0
      {
      map textures/sprites/bloodsplat_blue.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	alphaGen vertex
      }
}

greensplat
{
   spritegen oriented
   cull twosided
   surfaceparm nolightmap
  sort decal
      {
      map textures/sprites/gspray.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	alphaGen vertex
      }
}

bluegreen
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/bluegreen.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

bluespark
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/bluespark.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

greenblue
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/greenblue.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

redyellow
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/redyellow.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }

}
white
{

   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/white.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }

}

spitblob
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/spitblob.tga
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      rgbGen vertex
      alphaGen vertex
      }
}

spitblob64
{
   spritegen parallel
   cull twosided
   surfaceparm nolightmap
      {
      map textures/sprites/spitblob64.tga
      blendFunc GL_ONE GL_ONE
      rgbGen vertex
      alphaGen vertex
      }
}

watersplash
{
	{
      clampmap textures/sprites/splash.tga
		blendFunc GL_ONE GL_ONE
		tcMod stretch sin .9 0.05 0 0.5
		rgbGen wave sin .7 .3 .25 .5
	}
}

dustdrop
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/dustdrop.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

corona_orange
{
	qer_editorimage textures/sprites/corona_orange.tga
	deformVertexes lightglow
	//cull none
	{
		map textures/sprites/corona_orange.tga
		blendfunc add
	}
}

water_g
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/water_g.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

explosed
{
	qer_editorimage textures/sprites/explosed.tga
	cull none
	{
		clampmap textures/sprites/explosed.tga
		blendfunc alphaadd
		tcmod rotate 40
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/explosed.tga
		tcmod rotate -40
	}
} 

fire_ring
{
	qer_editorimage textures/sprites/fire_ring.tga
	deformVertexes lightglow
	cull none
	spritegen oriented
	{
		map textures/sprites/fire_ring.tga
		blendfunc alphaadd
		
	}
} 

sharpflame
{
	qer_editorimage textures/sprites/sharpflame.tga
	deformVertexes lightglow
	nopicmip
	cull none
	spritegen parallel
	{
		clampmap textures/sprites/sharpflame.tga
		blendfunc alphaadd
		rgbGen vertex
		alphaGen vertex
		tcmod rotate -40
	nextbundle
		clampmap textures/sprites/sharpflame.tga
		tcmod rotate 40		
	}
} 

sharpflame2
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel
	noMerge
	cull none
	{
		map textures/sprites/sharpflame.tga
		blendFunc alphaadd
		rgbGen vertex
		alphaGen vertex	
		tcmod rotate -40
	nextbundle
		clampmap textures/sprites/sharpflame.tga
		tcmod rotate 40		
	}
}

smoke_ring
{
	qer_editorimage textures/sprites/smoke_ring.tga
	deformVertexes lightglow
	cull none
	spritegen oriented
	{
		map textures/sprites/smoke_ring.tga
		blendfunc alphaadd
		
	}
} 

flame_pj
{
	qer_editorimage textures/sprites/flame_pj.tga
	deformVertexes lightglow
	//cull none
	{
		map textures/sprites/flame_pj.tga
		blendfunc add
	}
}

smokeswirl
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
//	spritegen parallel
//	noMerge
	cull twosided
	{
		clampmap textures/sprites/smokeswirl.tga
		blendFunc blend
		tcMod rotate 5
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/smokeswirl.tga
		tcMod rotate -5
	}
} 

explosed2
{
	qer_editorimage textures/sprites/explosed2.tga
	nopicmip
	cull none
	spritegen parallel_oriented
	{
		clampmap textures/sprites/explosed2.tga
		blendfunc alphaadd
		tcMod rotate -40
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/explosed2.tga
		tcMod rotate 40
 	}
} 

water_drop1
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/water_drop1.tga
		blendfunc add
		rgbGen vertex
		alphaGen vertex
	}
} 

grass_piece
{
	cull none
	nopicmip
	spriteGen parallel_oriented
	{
		map textures/effects/bh_grass_piece.tga
		blendFunc blend
//		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen entity
		rgbGen vertex
	}
}

snowpuffers
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/snowpuffers.tga
		blendfunc alphaadd
		rgbGen vertex
		alphaGen vertex
	}
} 

fire_ring2
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel
	noMerge
	cull none
	{
		clampmap textures/sprites/fire_ring.tga
		blendFunc alphaadd
		tcMod rotate 20
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/fire_ring.tga
		tcMod rotate -20
	}
} 

senn_fire1
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel
	noMerge
	cull none

	{
		map textures/sprites/senn_fire1.tga
		blendFunc alphaadd
		rgbGen vertex
		alphaGen vertex	
	}
}

senn_fire2
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel
	noMerge
	cull none
	{
		map textures/sprites/senn_fire2.tga
		blendFunc alphaadd
		rgbGen vertex
		alphaGen vertex	
	}
}