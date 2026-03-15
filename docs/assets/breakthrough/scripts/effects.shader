//=====================================================================================
// Bullet hit - Wood

bh_wood_puff
{
	deformVertexes autoSprite2
	cull none
	{
		animmap 20 textures/effects/woodpuff1.tga  textures/effects/woodpuff2.tga  textures/effects/woodpuff3.tga textures/effects/woodpuff4.tga textures/effects/woodpuff5.tga textures/effects/woodpuff6.tga textures/effects/woodpuff7.tga
		rgbGen lightingGrid
		blendfunc blend
	}
}

bh_wood_puff_simple
{
	deformVertexes autoSprite2
	cull none
	{
		map textures/effects/woodpuff1.tga
		rgbGen fromEntity
		alphaGen fromEntity
		blendfunc blend
	}
}

bh_wood_piece
{
	cull none
	{
		map textures/effects/bh_wood_piece.tga
		blendFunc blend
		alphaGen entity
		rgbGen fromEntity
	}
}

//=====================================================================================
// Bullet hit - Stone

bh_stone_puff
{
	deformVertexes autoSprite2
	cull none
	{
		animmap 20 textures/effects/rockpuff1.tga  textures/effects/rockpuff2.tga  textures/effects/rockpuff3.tga textures/effects/rockpuff4.tga textures/effects/rockpuff5.tga textures/effects/rockpuff6.tga textures/effects/rockpuff7.tga
		rgbGen lightingGrid
		blendfunc blend
	}
}

bh_stone_piece
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bh_stone_piece.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Bullet hit - Metal

bh_metal_fastpiece
{
	cull none
	{
		map textures/effects/bh_metal_fastpiece.tga
		blendFunc add
		alphaGen entity
		rgbGen entity
//		rgbGen identitylighting
	}
}

bh_metal_spark
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		map textures/effects/metalspark.tga
		blendFunc GL_ONE GL_ONE
		alphaGen entity
//		rgbGen entity
		rgbGen identity
	}
}

//=====================================================================================
// Bullet hit - Dirt

bh_dirt_piece
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bh_dirt_piece.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

bh_mud_splot
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/mudsplot.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Bullet hit - Foliage

bh_foliage_leaf
{
	cull none
	{
		clampmap models/fx/leaf/leaf.tga
		blendFunc blend
		alphaGen entity
		rgbGen lightingGrid
	}
}

//=====================================================================================
// Bullet hit - Grass

bh_grass_piece
{
	nopicmip
	cull none
	{
		map textures/effects/bh_grass_piece.tga
		blendFunc blend
//		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen entity
		rgbGen lightingGrid
	}
}

//=====================================================================================
// Bullet hit - Water

bh_water_drop
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bh_water_drop.tga
//		blendFunc blend
//		alphaGen entity
//		blendFunc add
//		rgbGen lightingGrid
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen vertex
		rgbGen vertex
	}
}

water_trail_bubble
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bh_water_drop.tga
//		blendFunc blend
//		alphaGen entity
		blendFunc add
//		rgbGen lightingGrid
		alphaGen vertex
		rgbGen vertex
	}
}


bh_water_ring
{
	spriteGen oriented
	cull none
	{
		map textures/effects/bh_water_ring.tga
//		blendFunc GL_ONE GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen vertex
		rgbGen vertex
	}
}

water_ring_moving
{
	spriteGen oriented
	cull none
	{
		map textures/effects/water_ring_moving.tga
//		blendFunc GL_ONE GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen vertex
		rgbGen vertex
	}
}

water_wake
{
	spriteGen oriented
	cull none
	{
		clampmap textures/effects/water_wake.tga
//		blendFunc GL_ONE GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Bullet hit - Ocean

bh_ocean_plume
{
	spritegen parallel_upright
	cull none
	{
		animmap 10 textures/sprites/waterplume1.tga textures/sprites/waterplume2.tga textures/sprites/waterplume3.tga textures/sprites/waterplume4.tga textures/sprites/waterplume5.tga textures/sprites/waterplume6.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

water_splash_drop
{
	cull none
	{
		clampmap textures/effects/water_splash_drop.tga
		blendFunc blend
		alphaGen entity
		rgbGen lightingGrid
	}
}

oceanspray
{
	spritegen parallel_oriented
	cull none
	{
		clampmap textures/effects/oceanspray.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Bullet hit - Sand

bh_sand
{
	spritegen parallel_upright
	cull none
	{
		map textures/effects/sandplume1.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Bullet hit - Snow


bh_snow_puff
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bh_snow_puff.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// Muzzle flashes

muzmodel
{
	sort nearest
	cull none
	{
		map models/fx/muzflash/flashnode1.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphagen vertex
	}
}

muzsprite
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	spriteScale .3
	{
		map models/fx/muzflash/flashnode1.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen vertex
		//rgbGen vertex
	}
}

thompsonsmg_spriteflash
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	spriteScale .7
	{
		map models/fx/muzflash/thompsonsmg_spriteflash.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
	}
}

thompsonsmg_sideflash
{
	sort nearest
	cull none
	{
		map models/fx/muzflash/thompsonsmg_sideflash.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
	}
}

mg42_spriteflash
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	spriteScale .7
	{
		map models/fx/muzflash/mg42_spriteflash.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
	}
}

mg42_sideflash
{
	sort nearest
	cull none
	{
		map models/fx/muzflash/mg42_sideflash.tga
		//blendFunc GL_ONE GL_ONE
		blendFunc alphaadd
		alphaGen vertex
	}
}

mg42_starflash
{
	sort nearest
	cull none
	{
		map models/fx/muzflash/muzflash2.tga
		//blendFunc GL_ONE GL_ONE
		blendFunc alphaadd
		alphaGen vertex
	}
}

//=====================================================================================
// Ammo shell casing shaders

shellmetal
{
	cull none
	{
		map models/ammo/shell/shell.tga
		rgbGen lightingGrid
//		alphaGen entity
	}

//	{
//		map textures/effects/shell_envmap.tga
//		blendFunc GL_DST_COLOR GL_ONE
//		tcGen environment
//	}

} 

shotgun_shell
{
	cull none
	{
		map models/ammo/shell/shotgun_shell.tga
		rgbGen lightingGrid
//		alphaGen entity
	}

//	{
//		map textures/effects/shell_envmap.tga
//		blendFunc GL_DST_COLOR GL_ONE
//		tcGen environment
//	}

}

//=====================================================================================
// Grenade explosions

gren_explosion
{
	nomipmaps
	cull none
	spriteGen parallel_oriented
//	sort additive
//	spriteScale 1.5
//	surfaceparm nolightmap
	{
//		animmap 20 textures/sprites/expl/expl0002.tga textures/sprites/expl/expl0003.tga textures/sprites/expl/expl0004.tga textures/sprites/expl/expl0005.tga textures/sprites/expl/expl0006.tga textures/sprites/expl/expl0007.tga textures/sprites/expl/expl0008.tga textures/sprites/expl/expl0009.tga textures/sprites/expl/expl0010.tga textures/sprites/expl/expl0011.tga textures/sprites/expl/expl0012.tga textures/sprites/expl/expl0013.tga textures/sprites/expl/expl0014.tga textures/sprites/expl/expl0015.tga textures/sprites/expl/expl0016.tga textures/sprites/expl/expl0017.tga textures/sprites/expl/expl0018.tga textures/sprites/expl/expl0019.tga textures/sprites/expl/expl0020.tga textures/sprites/expl/expl0021.tga
		animmap 20 textures/sprites/expl/expl0002.tga textures/sprites/expl/expl0003.tga textures/sprites/expl/expl0004.tga textures/sprites/expl/expl0005.tga textures/sprites/expl/expl0006.tga textures/sprites/expl/expl0007.tga textures/sprites/expl/expl0008.tga textures/sprites/expl/expl0009.tga textures/sprites/expl/expl0010.tga textures/sprites/expl/expl0011.tga textures/sprites/expl/expl0012.tga textures/sprites/expl/expl0013.tga textures/sprites/expl/expl0014.tga textures/sprites/expl/expl0015.tga textures/sprites/expl/expl0016.tga

		blendFunc add
		rgbGen vertex
	}
} 


//=====================================================================================
// mortar stuff

dirtplume
{
	nomipmaps
	nopicmip
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/effects/dirtplume.tga
//		clampmap textures/effects/dirtystuff.tga
		alphafunc ge128
//		blendfunc blend
		alphaGen vertex
		rgbGen vertex
		depthwrite
nextbundle
		map textures/effects/dirtnoise.tga
		tcmod rotate 360
	}
}

dirtcloud
{
	nomipmaps
	nopicmip
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/effects/dirtplume.tga
//		tcmod rotate 120
//		alphafunc ge128
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

mortar_dirthit
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/models/natural/mortarhit2.tga
		alphafunc ge128
		alphaGen vertex
		rgbGen vertex

nextbundle
		map textures/models/natural/mortar_noise.tga
		tcmod scroll 0 -.1
		tcmod scale 8 16
	}
}

mortar_dirthit2
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/models/natural/motarhit.tga
//		alphafunc ge128
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
//		depthwrite
nextbundle
		map textures/effects/sandplume1.tga
		tcmod scale 2 2
	}
}


waterplume
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/effects/h2o_hit.tga
//		alphafunc ge128
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
//		depthwrite
	}
}

spritely_water
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/models/natural/waterplume_large.tga
//		blendfunc gl_src_alpha gl_one
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

grndblst1
{
	cull none
	spritegen parallel
	sort 256
	{
		clampmap textures/effects/grndblst1.tga
		blendfunc GL_SRC_ALPHA GL_ONE
		alphaGen vertex
		rgbGen vertex
	//	nodepthtest
	}
}

//=====================================================================================

textures/effects/bang
{
	cull none
//	nomipmaps
	spriteGen parallel_oriented
	{
//		animmap 15 textures/effects/bang01.tga textures/effects/bang02.tga textures/effects/bang03.tga textures/effects/bang04.tga textures/effects/bang05.tga textures/effects/bang06.tga textures/effects/bang07.tga textures/effects/bang08.tga textures/effects/bang09.tga textures/effects/bang10.tga textures/effects/bang11.tga textures/effects/bang12.tga textures/effects/bang13.tga textures/effects/bang14.tga textures/effects/bang15.tga textures/effects/bang16.tga textures/effects/bang17.tga textures/effects/bang18.tga textures/effects/bang19.tga
////		alphafunc ge128
//		blendfunc add
////		alphaGen vertex
//		rgbGen const 0.5 0.5 0.5
//		depthwrite

//		animmap 20 textures/effects/hit01.tga textures/effects/hit02.tga textures/effects/hit03.tga textures/effects/hit04.tga textures/effects/hit05.tga textures/effects/hit06.tga textures/effects/hit07.tga textures/effects/hit08.tga textures/effects/hit09.tga textures/effects/hit10.tga textures/effects/hit11.tga textures/effects/hit12.tga textures/effects/hit13.tga textures/effects/hit14.tga textures/effects/hit15.tga textures/effects/hit16.tga textures/effects/hit17.tga textures/effects/hit18.tga textures/effects/hit19.tga
		animmap 20 textures/effects/bidsh01.tga textures/effects/bidsh02.tga textures/effects/bidsh03.tga textures/effects/bidsh04.tga textures/effects/bidsh05.tga textures/effects/bidsh06.tga textures/effects/bidsh07.tga textures/effects/bidsh08.tga textures/effects/bidsh09.tga textures/effects/bidsh10.tga textures/effects/bidsh11.tga textures/effects/bidsh12.tga textures/effects/bidsh13.tga textures/effects/bidsh14.tga textures/effects/bidsh15.tga textures/effects/bidsh16.tga textures/effects/bidsh17.tga textures/effects/bidsh18.tga textures/effects/bidsh19.tga
//		map textures/effects/hit09.tga
//		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		blendfunc add
//		alphaGen wave inversesawtooth -1 3 0 0.95
//		rgbGen colorwave 1 1 1 inversesawtooth -1 3 0 0.95
	}
}

textures/effects/woodsplinters
{
	cull none
	spriteGen oriented
	{
		clampmap textures/effects/woodsplinters.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//=====================================================================================
// water stuff

//// Water splish splash
//water_splash
//{
//	cull none
//	{
//		map textures/effects/water_splash.tga
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		alphaGen entity
//	}
//}
//
//water_spray
//{
//	cull none
//	{
//		map textures/effects/water_spray.tga
//		blendFunc GL_ONE GL_ONE
//		alphaGen vertex
//		rgbGen vertex
//	}
//}

water_drop1
{
	cull none
	spriteGen oriented
	{
		map textures/effects/water_drop1.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
		rgbGen vertex
	}
}

//water_splashring
//{
//	cull none
//	spriteGen oriented
//	{
//		map textures/effects/water_splashring.tga
//		blendFunc GL_ONE GL_ONE
//		alphaGen vertex
//		rgbGen vertex
//	}
//}

water_splash2
  {
	spriteGen oriented
 	cull none
 	{
 		map textures/effects/water_splash2.tga
 		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 		alphaGen vertex
		rgbgen vertex
 	}
  }

//=====================================================================================
// Barrel Effects

barrel_water_long
{
	cull none
	{
		map textures/effects/barrel_oil_long.tga
		blendFunc add
		alphaGen vertex
//		rgbGen vertex
		rgbGen lightingGrid
	}
}

barrel_water_splat
{
	polygonOffset
	{
		map textures/effects/barrel_oil_splat.tga
		blendFunc add
		rgbGen vertex
		alphaGen vertex
	}
}

barrel_water_drop
{
	spriteGen parallel_oriented
	{
		map textures/effects/barrel_oil_splat.tga
		blendFunc add
		rgbGen vertex
		alphaGen vertex
	}
}
 
//---------------------------------------------------------------------//

barrel_oil_long
{
	cull none
	{
		map textures/effects/barrel_oil_long.tga
		blendFunc blend
		alphaGen vertex
//		rgbGen vertex
		rgbGen lightingGrid
	}
}

barrel_oil_splat
{
	polygonOffset
	{
		map textures/effects/barrel_oil_splat.tga
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

barrel_oil_drop
{
	spriteGen parallel_oriented
	{
		map textures/effects/barrel_oil_splat.tga
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

// Drip Effect
textures/drip
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	qer_editorimage textures/weather/drip.tga
	deformVertexes autoSprite2
	cull none
	{
		map textures/weather/drip.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .2 .2 .2
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 


// Rain Effect
textures/rain
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	qer_editorimage textures/weather/rain.tga
	//deformVertexes autoSprite2
	cull none
	{
		map textures/weather/rain.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .2 .2 .2
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow0
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow1.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow1
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow2.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow2
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow3.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow3
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow4.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow4
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow5.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow5
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow6.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow6
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow7.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow7
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow8.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow8
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow9.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow9
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow10.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow10
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow11.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/snow11
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/snow12.tga
		//blendfunc blend
		blendfunc add
		rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 




//__________________________________________Fallout Effect_________________
// Snow Effect
textures/fallout0
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1.tga
		//map textures/weather/snow1.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//blendfunc add
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout1
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1_burning.tga
		//map textures/weather/snow2.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//blendfunc add
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout2
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1.tga
		//map textures/weather/snow3.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//blendfunc add
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout3
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_cluster.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout4
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_cluster.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout5
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout6
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout7
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_1.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout8
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_2.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout9
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_2.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//blendfunc add
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout10
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_3.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

// Snow Effect
textures/fallout11
{
	nomipmaps
	nopicmip
	qer_keyword 3dk
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	deformVertexes autoSprite
	cull none
	{
		map textures/weather/fallout_3.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
		//rgbgen constant .5 .5 .5
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 


//_____________________________________________________SeaGull_________________________________________________//

seagull
{
       cull none
       nomipmaps
       surfaceparm nonsolid
       qer_keyword bird
       surfaceparm trans
       surfaceparm nomarks
       surfaceparm noimpact
       qer_editorimage textures/special/gull.tga
	deformVertexes autoSprite2
	cull none
{
		map textures/special/gull.tga
		//blendfunc blend
		alpahFunc GE128
		rgbgen constant .2 .2 .2
		//alphaGen constant 0.8
		//alphaGen distFade 512 256
		//tcmod scroll 0 -5
	}
} 

bodyhitpuff
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/bodyhitpuff.tga
		blendFunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//_____________________________________________________Flame_________________________________________________//

simple_flame
{
	nopicmip
	nomipmaps
	spritegen parallel_upright
	cull none
	{
//		animmap 10 textures/effects/aFlame_01.tga textures/effects/aFlame_02.tga textures/effects/aFlame_03.tga textures/effects/aFlame_04.tga textures/effects/aFlame_05.tga textures/effects/aFlame_06.tga textures/effects/aFlame_07.tga textures/effects/aFlame_08.tga textures/effects/aFlame_03.tga
		animmap 15 textures/effects/flamer_01.tga textures/effects/flamer_02.tga textures/effects/flamer_03.tga textures/effects/flamer_04.tga textures/effects/flamer_05.tga textures/effects/flamer_06.tga textures/effects/flamer_07.tga textures/effects/flamer_08.tga textures/effects/flamer_09.tga textures/effects/flamer_10.tga textures/effects/flamer_11.tga textures/effects/flamer_12.tga textures/effects/flamer_13.tga textures/effects/flamer_14.tga textures/effects/flamer_15.tga textures/effects/flamer_16.tga textures/effects/flamer_17.tga textures/effects/flamer_18.tga textures/effects/flamer_19.tga textures/effects/flamer_20.tga textures/effects/flamer_21.tga textures/effects/flamer_22.tga textures/effects/flamer_23.tga textures/effects/flamer_24.tga textures/effects/flamer_25.tga
		blendfunc add
	}
} 

simple_flame_pj
{
	nopicmip
	nomipmaps
	spritegen parallel_oriented
	cull none
	{
//		animmap 10 textures/effects/aFlame_01.tga textures/effects/aFlame_02.tga textures/effects/aFlame_03.tga textures/effects/aFlame_04.tga textures/effects/aFlame_05.tga textures/effects/aFlame_06.tga textures/effects/aFlame_07.tga textures/effects/aFlame_08.tga textures/effects/aFlame_03.tga
		animmap 15 textures/effects/flamer_01.tga textures/effects/flamer_02.tga textures/effects/flamer_03.tga textures/effects/flamer_04.tga textures/effects/flamer_05.tga textures/effects/flamer_06.tga textures/effects/flamer_07.tga textures/effects/flamer_08.tga textures/effects/flamer_09.tga textures/effects/flamer_10.tga textures/effects/flamer_11.tga textures/effects/flamer_12.tga textures/effects/flamer_13.tga textures/effects/flamer_14.tga textures/effects/flamer_15.tga textures/effects/flamer_16.tga textures/effects/flamer_17.tga textures/effects/flamer_18.tga textures/effects/flamer_19.tga textures/effects/flamer_20.tga textures/effects/flamer_21.tga textures/effects/flamer_22.tga textures/effects/flamer_23.tga textures/effects/flamer_24.tga textures/effects/flamer_25.tga
		blendfunc add
	}
} 
  
//_____________________________________________________________________________________________________________//

stonechip
{
	spritegen parallel_oriented
	cull none
	{
		map textures/effects/stonechip.tga
		alphaFunc GE128
		alphaGen vertex
		rgbGen vertex
	}
}
 
//_____________________________________________________________________________________________________________//

barrel_gas_expl
{
	spritegen parallel_oriented
	cull none
	{
		animmap 30 textures/effects/grndcooker01.tga textures/effects/grndcooker02.tga textures/effects/grndcooker03.tga textures/effects/grndcooker04.tga textures/effects/grndcooker05.tga textures/effects/grndcooker06.tga textures/effects/grndcooker07.tga textures/effects/grndcooker08.tga textures/effects/grndcooker09.tga textures/effects/grndcooker10.tga textures/effects/grndcooker11.tga textures/effects/grndcooker12.tga textures/effects/grndcooker13.tga textures/effects/grndcooker14.tga textures/effects/grndcooker15.tga textures/effects/grndcooker16.tga textures/effects/grndcooker17.tga
		blendfunc add
	}
}

plane_bomb_expl
{
	spritegen parallel_oriented
	cull none
	{
		animmap 30 textures/effects/mushroom01.tga textures/effects/mushroom02.tga textures/effects/mushroom03.tga textures/effects/mushroom04.tga textures/effects/mushroom05.tga textures/effects/mushroom06.tga textures/effects/mushroom07.tga textures/effects/mushroom08.tga textures/effects/mushroom09.tga textures/effects/mushroom10.tga textures/effects/mushroom11.tga textures/effects/mushroom12.tga textures/effects/mushroom13.tga textures/effects/mushroom14.tga textures/effects/mushroom15.tga
		blendfunc add
	}
}

fire_ring
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
//	spritegen parallel
	noMerge
	cull twosided
	{
		clampmap textures/effects/fire_ring.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		tcMod rotate 60
	//	rgbGen vertex
	//	alphaGen vertex
	nextbundle
		clampmap textures/effects/fire_ring.tga
		tcMod rotate -60
	}
}
//dead us bodies-------------------------------------------------------------------------------------------

textures/effects/usdead1a // Sprite version
{
    qer_editorimage textures/effects/usdead1a.tga
    surfaceparm nonsolid
	surfaceparm translucent

	
    {
		map textures/effects/usdead1a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

    

textures/effects/usdead1b // Sprite version
{
	qer_editorimage textures/effects/usdead1b.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead1b.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 



textures/effects/usdead1c // Sprite version
{
	qer_editorimage textures/effects/usdead1c.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead1c.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead2a // Sprite version
{
	qer_editorimage textures/effects/usdead2a.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead2a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead2b // Sprite version
{
	qer_editorimage textures/effects/usdead2b.tga
	surfaceparm nonsolid
	surfaceparm translucent
	{
		map textures/effects/usdead2b.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead2c // Sprite version
{
	qer_editorimage textures/effects/usdead2c.tga
	surfaceparm nonsolid
	surfaceparm translucent

	{
		map textures/effects/usdead2c.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead3a // Sprite version
{
	qer_editorimage textures/effects/usdead3a.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead3a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead3b // Sprite version
{
	qer_editorimage textures/effects/usdead3b.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead3b.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 


textures/effects/usdead3c // Sprite version
{
	qer_editorimage textures/effects/usdead3c.tga
	surfaceparm nonsolid
	surfaceparm translucent

	
	{
		map textures/effects/usdead3c.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

////BACK TO OUR GOODIES!!!!!!!!!!//

hall_purger
{
	cull none
	nopicmip
	nomipmaps
	spriteGen parallel_oriented
	{
		animmap 13 textures/effects/ball001.tga textures/effects/ball002.tga textures/effects/ball003.tga textures/effects/ball004.tga textures/effects/ball005.tga textures/effects/ball006.tga textures/effects/ball007.tga textures/effects/ball008.tga textures/effects/ball009.tga textures/effects/ball010.tga textures/effects/ball011.tga textures/effects/ball012.tga textures/effects/ball013.tga 
            blendfunc add
	}
}

hot_slag
{
	cull none
	nopicmip
	nomipmaps
	spriteGen parallel_oriented

	{
		map textures/effects/slag_hot.tga
		blendfunc GL_SRC_ALPHA GL_ONE
		alphaGen vertex
	}
} 

glass_small
{
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  
      {
            map textures/effects/glass_small.tga
            blendfunc blend
	 	//depthWrite
      }
}

glass_medium
{
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  
      {
            map textures/effects/glass_medium.tga
            blendfunc blend
		//depthWrite
      }
} 

glass_large
{
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  
      {
            map textures/effects/glass_large.tga
            blendfunc blend
		//depthWrite
      }
}

//=====================================================================================
// Hallfires

fireball_pj
{
	cull none
	spriteGen parallel_oriented
	{
//		animmap 20 textures/sprites/expl/expl0002.tga textures/sprites/expl/expl0003.tga textures/sprites/expl/expl0004.tga textures/sprites/expl/expl0005.tga textures/sprites/expl/expl0006.tga textures/sprites/expl/expl0007.tga textures/sprites/expl/expl0008.tga textures/sprites/expl/expl0009.tga textures/sprites/expl/expl0010.tga textures/sprites/expl/expl0011.tga textures/sprites/expl/expl0012.tga textures/sprites/expl/expl0013.tga textures/sprites/expl/expl0014.tga textures/sprites/expl/expl0015.tga textures/sprites/expl/expl0016.tga textures/sprites/expl/expl0017.tga textures/sprites/expl/expl0018.tga textures/sprites/expl/expl0019.tga textures/sprites/expl/expl0020.tga textures/sprites/expl/expl0021.tga
		animmap 15 textures/sprites/expl/expl0002.tga textures/sprites/expl/expl0003.tga textures/sprites/expl/expl0004.tga textures/sprites/expl/expl0005.tga textures/sprites/expl/expl0006.tga textures/sprites/expl/expl0007.tga textures/sprites/expl/expl0008.tga textures/sprites/expl/expl0009.tga textures/sprites/expl/expl0010.tga textures/sprites/expl/expl0011.tga textures/sprites/expl/expl0012.tga textures/sprites/expl/expl0013.tga textures/sprites/expl/expl0014.tga textures/sprites/expl/expl0015.tga textures/sprites/expl/expl0016.tga

		blendfunc alphaadd
	//	rgbGen vertex
	}
}

//=====================================================================================
// electrix

electric
{
	cull none
        qer_editorimage textures/effects/lectricarc_sml1.tga
	spriteGen parallel_oriented
	{
		animmap 15 textures/effects/lectricarc_sml1.tga textures/effects/lectricarc_sml2.tga textures/effects/lectricarc_sml3.tga
                blendfunc alphaadd
        //        rgbGen vertex
	}
} 

electric_spark
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		map textures/effects/sparky1.tga
		blendFunc GL_ONE GL_ONE
		alphaGen entity
		rgbGen identity
	}
}

sparkslag
{
	spritegen parallel_oriented
	cull none
	surfaceparm nolightmap
	{
		map textures/effects/sparkslag.tga
		blendFunc GL_ONE GL_ONE
		alphaGen entity
		rgbGen identity
	}
} 

//WOOD DEBRIS n Stuff////////////////////////////////////

//Wood_latice for debris effects
wood_latice
{
	qer_editorimage textures/effects/wood_latice.tga
	{
		map textures/effects/wood_latice.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
} 

wood_laticecull
{
	qer_editorimage textures/effects/wood_latice.tga
	cull none
	{
		map textures/effects/wood_latice.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
} 

// wood_brace
wood_brace
{
	qer_editorimage textures/effects/woodbeams.tga

	{
		map textures/effects/woodbeams.tga
		rgbGen lightingSpherical
	}
}

// woodbeams
woodbeams
{
	qer_editorimage textures/effects/woodbeams.tga
        cull none
	{
		map textures/effects/woodbeams.tga
		rgbGen lightingSpherical
	}
} 

// window_pane
window_pane
{
	qer_editorimage textures/effects/wood_latice.tga

	{
		map textures/effects/wood_latice.tga
		rgbGen lightingSpherical
	}
} 

// wood_flap
wood_flap
{
	qer_editorimage textures/effects/woodbeams.tga

	{
		map textures/effects/woodbeams.tga
		rgbGen lightingSpherical
	}
} 

//a stick and a block connected
stickblock
{
	qer_editorimage textures/effects/woodbeams.tga

	{
		map textures/effects/woodbeams.tga
		rgbGen lightingSpherical
	}
} 

//CONCRETE AND STONE GOODNESS FOR YOUR DEMOLITION PLEASURE//

//a huge slab of concrete with a tude
concrete1
{
	qer_editorimage textures/effects/woodbeams.tga

	{
		map textures/effects/concrete1.tga
		rgbGen lightingSpherical
	}
} 

//concrete2
concrete2
{
	qer_editorimage textures/effects/concrete2.tga

	{
		map textures/effects/concrete2.tga
		rgbGen lightingSpherical
	}
} 

//a block o crete
blockcrete
{
	qer_editorimage textures/effects/blockcrete.tga

	{
		map textures/effects/blockcrete.tga
		rgbGen lightingSpherical
	}
} 

//concrete with rebars
rebarblock
{
	qer_editorimage textures/effects/rebarblock.tga
	{
		map textures/effects/rebarblock.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
} 

rebarblockcull
{
	qer_editorimage textures/effects/rebarblock.tga
	cull none
	{
		map textures/effects/rebarblock.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
} 

//Herewith starts the metal chunks and rust salsa
heavy_pipe
{
	qer_editorimage textures/effects/heavy_pipe.tga
	{
		map textures/effects/heavy_pipe.tga
		rgbGen lightingSpherical
	}
} 

metal_section
{
	qer_editorimage textures/effects/metal_section.tga
	{
		map textures/effects/metal_section.tga
		rgbGen lightingSpherical
	}
} 

bolt_part
{
	qer_editorimage textures/effects/bolt_part.tga
	{
		map textures/effects/bolt_part.tga
		rgbGen lightingSpherical
	}
} 

ibeam_piece
{
	qer_editorimage textures/effects/ibeam_piece.tga
	{
		map textures/effects/ibeam_piece.tga
		rgbGen lightingSpherical
	}
} 

bp_debris1
{	
	cull none
	qer_editorimage textures/effects/bp_debris1.tga
	{
		map textures/effects/bp_debris1.tga
		rgbGen lightingSpherical
	}
} 

bp_debris2
{	
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
	qer_editorimage textures/effects/bp_debris2.tga  
      {
            map textures/effects/bp_debris2.tga
            alphafunc GE128
	    depthWrite
	nextbundle
	    rgbGen lightingSpherical
      }
} 

bp_debris2g
{	
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
	qer_editorimage textures/effects/bp_debris2g.tga  
      {
            map textures/effects/bp_debris2g.tga
            alphafunc GE128
	    depthWrite
	nextbundle
	    rgbGen lightingSpherical
      }
} 

bp_debris3
{	
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  	qer_editorimage textures/effects/bp_debris3.tga
      {
            map textures/effects/bp_debris3.tga
            alphafunc GE128 
	    depthWrite
	nextbundle
	    rgbGen lightingSpherical
      }
} 

bp_debris3g
{	
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  	qer_editorimage textures/effects/bp_debris3g.tga
      {
            map textures/effects/bp_debris3g.tga
            alphafunc GE128 
	    depthWrite
	nextbundle
	    rgbGen lightingSpherical
      }
} 

//Some Natural Goodness in the form of insects...
moth
{
	cull none
	nopicmip
	nomipmaps
	spritegen oriented
  
      {
            map textures/effects/moth.tga
            blendfunc blend 
		depthWrite
      }
} 

firefly
{
	cull none
	nopicmip
	nomipmaps
	spriteGen parallel_oriented

	{
		map textures/effects/firefly.tga
		depthWrite
		blendfunc add
	}
} 

newboom1
{
	cull none
        qer_editorimage textures/effects/boom1.tga
	spriteGen parallel_oriented
	{
		animmap 30 textures/effects/boom1.tga textures/effects/boom2.tga textures/effects/boom3.tga textures/effects/boom4.tga textures/effects/boom5.tga textures/effects/boom6.tga textures/effects/boom7.tga
        blendfunc alphaadd
        //rgbGen vertex
	}
} 

kingtiger_starflash
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	spriteScale .7
	{
		map models/fx/muzflash/muzflash2.tga
		blendFunc GL_ONE GL_ONE
		alphaGen vertex
	}
}

tracer_fake
{
        surfaceparm nolightmap
	spriteGen oriented
	cull none
	q3map_surfacelight 2000
	{
		map textures/beams/smalltracer.tga
		alphaGen vertex
		rgbGen vertex
		tcmod scroll 0 10
		nextbundle
		clampmap textures/beams/smalltracergradient.tga
		blendFunc add
	}
}	

//=====================================================================================

//Bursts of flak

flak_flash
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	{
		map textures/fx/flak/flakBlast_Flash01.tga
		blendFunc blend
		alphaGen vertex
	}
}

flak_cloud01
{
	spriteGen parallel_oriented
	cull none
	{
		map textures/fx/flak/flakBlast_Cloud01.tga
		alphaGen vertex
		rgbGen vertex
		blendFunc blend
	}
}

flak_cloud03
{
	spriteGen parallel_oriented
	cull none
	{
		map textures/fx/flak/flakBlast_Cloud03.tga
		alphaGen vertex
		rgbGen vertex
		blendFunc blend
	}

	{
		map textures/fx/flak/flakBlast_Cloud02.tga
		alphaGen vertex
		blendFunc blend
		alphaGen wave triangle 0 0.65 .5 0.5
	}
}

flak_cloud04
{
	spriteGen parallel_oriented
	cull none
	{
		map textures/fx/flak/flakBlast_Cloud03.tga
		alphaGen vertex
		rgbGen vertex
		blendFunc blend
	}

	{
		map textures/fx/flak/flakBlast_Cloud02.tga
		alphaGen vertex
		blendFunc blend
		alphaGen wave triangle 0 0.8 .3 .34
	}
}

flakFlash_skyBox
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	{
		map textures/fx/flak/Flak_skyBox_fire.tga
		blendfunc alphaadd
		alphaGen vertex
	}
}

flakSmoke_skyBox
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	{
		map textures/fx/flak/Flak_skyBox_smoke.tga
		blendFunc blend
		alphaGen vertex
	}
}

//=====================================================================================

//mortarImpacts_snow

mortar_snowHit
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/sprites/snow_mortarHit.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 -.5
		tcmod scale 3 1.5
	}
}

mortar_dirtHit
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/sprites/dirt_mortarHit.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 -.5
		tcmod scale 3 1.5
	}
}


snowclump
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/snowclump.tga
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
	}
} 

mortar_snowPlume
{
	nomipmaps
	nopicmip
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/snowplume.tga
		alphafunc ge128
		alphaGen vertex
		rgbGen vertex
		depthwrite
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod rotate -360
	}
}

mortar_dirtChunks
{
	nomipmaps
	nopicmip
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/mortar_stoneParticle.tga
		alphafunc ge128
		alphaGen vertex
		rgbGen vertex
		depthwrite
	}
}

//=====================================================================================

//C47 collision - T1l1
           

C47_fire2smoke
{
	spriteGen parallel_oriented
	{
		clampmap textures/fx/C47_exp/C47_smoke_02.tga
		blendFunc blend
		rgbGen vertex
		tcmod rotate -50
		alphaGen wave triangle 1 .5 0 .6
		
	}

	{
		clampmap textures/fx/C47_exp/C47_fire_02.tga
		blendfunc blend
		rgbGen vertex
		tcmod rotate -50
	}
}

C47_fire2smokeCCW
{
	spriteGen parallel_oriented
	{
		clampmap textures/fx/C47_exp/C47_smoke_02.tga
		blendFunc blend
		rgbGen vertex
		tcmod rotate -50
		alphaGen wave triangle 1 .5 0 .6
		
	}

	{
		clampmap textures/fx/C47_exp/C47_fire_02.tga
		blendfunc blend
		rgbGen vertex
		tcmod rotate -50
	}
}

C47skyTrain_smoke2
{
	spriteGen parallel_oriented
	{
		clampmap textures/fx/C47_exp/C47_smoke_02.tga
		blendfunc blend
		rgbGen vertex
		tcmod rotate -10
	}
}

C47skyTrain_smoke3
{
	spriteGen parallel_oriented
	cull none
	{
		clampmap textures/fx/C47_exp/C47_smoke_02.tga
		blendfunc blend
		rgbGen vertex
		tcmod rotate 10
	}
}

C47skyTrain_fiery
{
	spriteGen parallel_oriented
	sort nearest
	{
		clampmap textures/fx/C47_exp/C47_fire_02.tga
		blendfunc alphaadd
		rgbGen vertex
//		tcmod rotate 30
	}
}

C47skyTrain_fieryCCW
{
	spriteGen parallel_oriented
	{
		clampmap textures/fx/C47_exp/C47_fire_01.tga
		blendfunc alphaadd
		rgbGen vertex
		tcmod rotate -90
	}
}

C47skyTrain_burst1
{
	spriteGen parallel_oriented
	sort nearest
	{
		clampmap textures/fx/C47_exp/C47_burst_01.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================
//Boat ride in 1_3

waterWake
{
	cull none
	sort nearest
	spriteGen oriented
	{
		clampmap textures/fx/water/waterWake.tga
		blendfunc blend
		rgbGen vertex
	}
}

waterRing
{
	cull none
	sort nearest
	spriteGen oriented
	{
		clampmap textures/fx/water/waterRing.tga
		blendfunc blend
		rgbGen vertex
	}
}

waterDrop
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/water/waterDrop.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}
