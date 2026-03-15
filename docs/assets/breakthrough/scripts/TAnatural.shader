textures/TAnatural/river_muddy
{
	qer_editorimage textures/TAnatural/riverswill.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword Holland
      //qer_trans .3
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	//deformvertexes wave 30 sin 0 10 0 .2
        {
		map textures/TAnatural/riverswill.tga
		blendFunc GL_ONE GL_ONE
		rgbGen identity
		tcMod scale .5 .5
		tcMod scroll .05 .05
	}
	{
		map textures/TAnatural/riverswill_additive.tga
                blendfunc add
		tcMod scale .6 .6
		tcMod scroll .07 .07
	nextbundle
		map textures/TAnatural/riverswill_additive.tga
		tcMod scale .6 .6
		tcMod scroll 0 .07
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
} 

///////////////////////////////////////////////////////
//                GRASS MODEL TEXTURE 		 //////
///////////////////////////////////////////////////////
grassclod
{
	qer_editorimage textures/TAnatural/grassclod.tga
	nomipmaps
	cull none
//	deformvertexes flap t 24 sin 4 4 0 1 1 0
	deformVertexes autoSprite
	{
//		ifcvar r_blendtrees 0
		clampmap textures/models/natural/grassclod.tga
		depthwrite
		alphafunc GE128
//		alphagen distfade 1250 512
		rgbgen vertex
	}
//
//	{
//		ifcvarnot r_blendtrees 0
//		clampmap textures/models/natural/grassclod.tga
//		depthwrite
//		nocolorwrite
///		alphafunc GE_FOLIAGE1
//		alphagen distfade 2000 1500
//	}
//	{
//		ifCvarNot r_blendtrees 0
//		clampmap textures/models/natural/grassclod.tga
//		alphafunc GT0
///		depthWrite
//		alphagen distfade 1250 128
//		rgbgen lightinggrid
//	}

} 

/////////////////////////////////
/// overhead fog plane for t2l1//
/////////////////////////////////

textures/TAnatural/t2fogplane
{
	qer_editorimage textures/TAnatural/t2fogplane.tga
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull front
	{
		map textures/TAnatural/t2fogplane.tga
		blendFunc blend
		depthWrite
		rgbGen constant .7 .7 .8
//		alphaGen dot .1 .6
		alphaGen const 0.25
		tcMod scroll .08 0
		tcMod scale 1 1
		tcMod turb 0 .04 0.04 0.04
	}
}

TAwintershrub
{
	qer_editorimage textures/TAnatural/tawintershrub.tga
	nomipmaps
	cull none
	deformvertexes flap t 24 sin 4 4 0 1 1 0
	{
		ifcvar r_blendtrees 0
		clampmap textures/TAnatural/tawintershrub.tga
		depthwrite
		alphafunc GE128
		alphagen distfade 2000 1500
		rgbgen lightinggrid
	}
	{
		ifcvarnot r_blendtrees 0
		clampmap textures/TAnatural/tawintershrub.tga
		depthwrite
		nocolorwrite
		alphafunc GE_FOLIAGE1
		alphagen distfade 2000 1500
	}
	{
		ifCvarNot r_blendtrees 0
		clampmap textures/TAnatural/tawintershrub.tga
		alphafunc GT0
		blendfunc blend
		alphagen distfade 2000 1500
		rgbgen lightinggrid
	}
}  

TAwintershrubfill
{
	qer_editorimage textures/TAnatural/tawintershrubfill.tga
	nomipmaps
	cull none
	deformvertexes flap t 24 sin 4 4 0 1 1 0
	{
		ifcvar r_blendtrees 0
		clampmap textures/TAnatural/tawintershrubfill.tga
		depthwrite
		alphafunc GE128
		alphagen distfade 2000 1500
		rgbgen lightinggrid
	}
	{
		ifcvarnot r_blendtrees 0
		clampmap textures/TAnatural/tawintershrubfill.tga
		depthwrite
		nocolorwrite
		alphafunc GE_FOLIAGE1
		alphagen distfade 2000 1500
	}
	{
		ifCvarNot r_blendtrees 0
		clampmap textures/TAnatural/tawintershrubfill.tga
		alphafunc GT0
		blendfunc blend
		alphagen distfade 2000 1500
		rgbgen lightinggrid
	}
}  

  
/////////////////////////////////
// Tree2winter  //tall winter////
/////////////////////////////////

static_tree2_1winter //trunk
{
	qer_editorimage textures/TAnatural/tree2_1winter.tga
	{
		map textures/TAnatural/tree2_1winter.tga
		alphaGen distFade 1000 600
		rgbgen lightingSpherical
	}
}
static_tree2_2winter //vertacle cross plains
{
	qer_editorimage textures/TAnatural/tree2_2winter.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/TAnatural/tree2_2winter.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbgen lightingSpherical
	}
}
static_tree2_3winter //fill plains
{
	qer_editorimage textures/TAnatural/tree2_3winter.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/TAnatural/tree2_3winter.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		rgbgen lightingSpherical
	}
}
static_tree2_4winter //horizontal plains
{
	qer_editorimage textures/TAnatural/tree2_4winter.tga
		nomipmaps
	cull none
//	check into these, not that important, get the rest looking right 1st
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/TAnatural/tree2_4winter.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 512
		rgbgen lightingSpherical
	}
}
static_tree2wintersprite //birch sprite
{
	qer_editorimage textures/TAnatural/tree2wintersprite.tga
	//	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/TAnatural/tree2wintersprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1200 1200
		rgbgen lightingSpherical
	}
}  

textures/TAnatural/ta_hedgerow
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/TAnatural/ta_hedgerow.tga
	cull none
	nopicmip
	{
		map textures/TAnatural/ta_hedgerow.tga
		depthWrite
		alphagen heightFade 750 1000
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}  

textures/TAnatural/ta_hedgerowend
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/TAnatural/ta_hedgerowend.tga
	cull none
	nopicmip
	{
		map textures/TAnatural/ta_hedgerowend.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}  

textures/TAnatural/ta_hedgerowendL
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/TAnatural/ta_hedgerowendL.tga
	cull none
	nopicmip
	{
		map textures/TAnatural/ta_hedgerowendL.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}  

textures/TAnatural/ta_hedgerowendR
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/TAnatural/ta_hedgerowendR.tga
	cull none
	nopicmip
	{
		map textures/TAnatural/ta_hedgerowendR.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}  

textures/TAnatural/m3l3grass_1test
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/TAnatural/m3l3grass_1test.jpg
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/m3l3grass_trench
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/m3l3grass_trench.jpg
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/m3l3grass_shed
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/m3l3grass_shed.jpg
	nextbundle
		map $lightmap
	}
}

textures/TAnatural/m3l3grass_trnchend
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/m3l3grass_trnchend.jpg
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/furroughed_c
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_c.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/furroughed_lr
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_lr.tga
	nextbundle
		map $lightmap
	}
}

textures/TAnatural/furroughed_base
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_base.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/furroughed_tb
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_tb.tga
	nextbundle
		map $lightmap
	}
} 

textures/TAnatural/furroughed_new01
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_new01.tga
	nextbundle
		map $lightmap
	}
}

textures/TAnatural/furroughed_new02
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/TAnatural/furroughed_new02.tga
	nextbundle
		map $lightmap
	}
}

textures/wilderness/m3l3grass_bocroad
{
	qer_keyword grass
	qer_keyword test
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/wilderness/m3l3grass_bocroad_new.tga
	nextbundle
		map $lightmap
	}
}
