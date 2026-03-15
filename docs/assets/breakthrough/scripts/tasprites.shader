smokegrenade
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	cull twosided
	{
		clampmap textures/sprites/hrpuffnstuff.tga
		blendFunc blend
		tcMod rotate 16
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/hrpuffnstuff.tga
		tcMod rotate -14 77
	}
}

smokegrenade_allied
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	cull twosided
	{
		clampmap textures/sprites/hrpuffnstuff_allied.tga
		blendFunc blend
		tcMod rotate 16
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/hrpuffnstuff_allied.tga
		tcMod rotate -14 77
	}
}

smokegrenade_blue
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	cull twosided
	{
		clampmap textures/sprites/hrpuffnstuff_blue.tga
		blendFunc blend
		tcMod rotate 16
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/hrpuffnstuff_blue.tga
		tcMod rotate -14 77
	}
}
smokegrenade_german
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	cull twosided
	{
		clampmap textures/sprites/hrpuffnstuff_german.tga
		blendFunc blend
		tcMod rotate 16
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/hrpuffnstuff_german.tga
		tcMod rotate -14 77
	}
}

smokegrenade_soviet
{
	nopicmip
	surfaceparm nolightmap
	spritegen parallel_oriented
	cull twosided
	{
		clampmap textures/sprites/hrpuffnstuff_soviet.tga
		blendFunc blend
		tcMod rotate 16
		rgbGen vertex
		alphaGen vertex
	nextbundle
		clampmap textures/sprites/hrpuffnstuff_soviet.tga
		tcMod rotate -14 77
	}
}

corona_util_colorable
{
	qer_editorimage textures/sprites/corona_util.tga
	deformVertexes lightglow
	cull none
	{
		map textures/sprites/corona_util.tga
		rgbGen vertex
		alphaGen vertex
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}

//=====================================================================================
// mortar stuff

snowPlume
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
		tcmod rotate 360
	}
}

snowCloud
{
	nomipmaps
	nopicmip
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/sprites/snowplume.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

mortar_snowHit1
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/sprites/snow_motarhit.tga
		alphafunc ge128
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 -.1
		tcmod scale 8 16
	}
}

mortar_snowHit2
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/sprites/snow_motarhit.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/effects/sandplume1.tga
		tcmod scale 2 2
	}
}

snowClump
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

//=====================================================================================

//Train Sparks

trainSpark_01
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/Train/trainSpark_01.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

//=====================================================================================
//Bomb exploding in the snow.

//snow and smoke trailing blast debris
snowPlume_01
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bomb_exp/snowTrail_01.tga
		blendfunc blend
		rgbGen vertex
	}
}

dirtDebris_01
{
	cull none
	deformVertexes autoSprite2
	{
		clampmap textures/fx/bomb_exp/dirtySnowball_01.tga
		blendfunc blend
		rgbGen vertex
	}
}

//Initial blast
bombBlast_snow01
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/bomb_exp/bombBlast_snow02.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 .7
		tcmod scale 5 2
	}
}

bombBlast_dirt01
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/bomb_exp/bombBlast_dirt01.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 .75
		tcmod scale 5 2
	}
}

//Big Blast -dirt-

bombBlast_bigDirt
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/bomb_exp/BombBlast_bigDirt.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 -.5
		tcmod scale 3 2
	}
}

thrownStone1
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/dirtBall1.tga
		blendfunc blend
		rgbGen vertex
	}
}

thrownStone2
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/concrete_02.tga
		blendfunc blend
		rgbGen vertex
	}
}

thrownStone3
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/dirtBall2.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================

//Big bridge detonation

waterBlast01
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/effects/h2o_hit.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

//Big Splash

bigSplash
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/Bridge/bigSplash.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 -.5
		tcmod scale 3 2
	}
}

//Charges going off

BridgeCharge
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bridge/bridgeCharge.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

fallingDirt
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bridge/fallingDirt.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================
//Tree Shattering from a Mortar Strike

Splinter1
{
	cull none
	spriteGen oriented
	{
		clampmap textures/fx/treeShatter/Splinter1.tga
		blendfunc blend
		rgbGen vertex
	}
}

Splinter2
{
	cull none
	spriteGen oriented
	{
		clampmap textures/fx/treeShatter/Splinter2.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================

//Grenade redo

metalDebris_01
{
	cull none
	deformVertexes autoSprite2
	{
		clampmap textures/fx/grenade/metalDebris_01.tga
		blendfunc blend
		rgbGen vertex
	}
}

grenadeFire
{
	sort nearest
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/grenade/explosion1.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

//=====================================================================================
//Tank's muzzle fire

muzzleBlast_01
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/muzzleBlast_01.tga
		blendfunc blend
		rgbGen vertex
	}
}

generalSpark_01
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/generalSpark_01.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

generalFireball_01
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bridge/bridgeCharge.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

muzzleCharge
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bridge/bridgeCharge.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

muzzleCharge_K5
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bridge/bridgeCharge.tga
		blendfunc alphaadd
		rgbGen vertex
		nofog
	}
}

muzzleCharge_02
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/muzzleBlast_02.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

//=====================================================================================
//Strafe

strafe_snowHit1
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/Strafe/strafe_snowHit1.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	}
}

strafe_snowHit2
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/Strafe/strafe_snowHit2.tga
		blendfunc blend
		alphaGen vertex
		rgbGen vertex
	nextbundle
		map textures/sprites/mortarsnow_noise.tga
		tcmod scroll 0 2
		tcmod scale 5 2
	}
}

snowPlume_02
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/strafe/snowTrail_02.tga
		blendfunc blend
		rgbGen vertex
	}
}
//=====================================================================================
//Strafe

//smoke from engine
engineSmoke_01
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/supplyTruck/engineSmoke.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================

deathSod_01
{
	cull none
	deformVertexes autoSprite2
	{
		clampmap textures/fx/tank/grassChunk.tga
		blendfunc blend
		rgbGen vertex
	}
}

concreteDebris_01
{
	cull none
	deformVertexes autoSprite2
	{
		clampmap textures/fx/tank/concrete_01.tga
		blendfunc blend
		rgbGen const .1 .1 .1
	}
}

puffyStuff_dark
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehaoSmoke_dark.tga
		blendfunc blend
		rgbGen vertex
	}
}

puffyStuff_light
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tehao/tehaoSmoke_light.tga
		blendfunc blend
		rgbGen vertex
	}
}

//=====================================================================================
//Fire

campFire_01
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/sustainedFire/flame1.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

campFire_02
{
	cull none
	spriteGen parallel_upright
	{
		clampmap textures/fx/sustainedFire/flame2.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

campFire_03
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/sustainedFire/flame3.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

emberBlast
{
	cull none
	sort nearest
	spriteGen parallel_oriented
	{
		clampmap textures/fx/tank/Explosion3.tga
		blendfunc alphaadd
		rgbGen vertex
	}
}

snowPlume_K5
{
	cull none
	spriteGen parallel_oriented
	{
		clampmap textures/fx/bomb_exp/snowTrail_01.tga
		blendfunc blend
		rgbGen vertex
		nofog
	}
}
