textures/misc_outside/bark_set1
{
	qer_keyword tree
	qer_keyword natural
	qer_keyword flat
	qer_keyword wood
	surfaceparm wood
	{
		map textures/misc_outside/bark_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bermudaagegrass_1
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/bermudaagegrass_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/nugrass_set2atrans
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/nugrass_set2atrans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

 textures/misc_outside/nugrass_set2atrans_corner
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/nugrass_set2atrans_corner.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 


textures/misc_outside/nugrass_set4topgrass
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/nugrass_set4topgrass.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocagegrass_1uf
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/bocagegrass_1uf.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocage_grass1a
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/bocage_grass1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocagegrass_1
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/bocagegrass_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_set1
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/creekbed_1
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/creekbed_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/fence1
{
	qer_editorimage textures/misc_outside/fence_set1m.tga
	qer_keyword wood
	qer_keyword masked
	qer_keyword door
	surfaceparm wood
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/misc_outside/fence_set1m.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/whiteboard_fence
{
	qer_keyword wood
	qer_keyword masked
	qer_keyword door
	surfaceparm wood
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/whiteboard_fence.tga
	{
		map textures/misc_outside/whiteboard_fence.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/whiteboard_fencedrk
{
	qer_keyword wood
	qer_keyword masked
	qer_keyword door
	surfaceparm wood
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/whiteboard_fencedrk.tga
	{
		map textures/misc_outside/whiteboard_fencedrk.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}
 
textures/misc_outside/grassmasked
{
	qer_keyword masked
	qer_keyword foliage


	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/grassmasked.tga
	{
		map textures/misc_outside/grassmasked.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/regpalm_shadow
{
	qer_keyword masked
	qer_keyword foliage
        qer_editorimage textures/models/natural/regpalmsprite.tga
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	nodraw
	{
		map textures/misc_outside/regpalm_shadow.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgetrm1full
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/hedgetrm1full.tga
	{
		map textures/misc_outside/hedgetrm1full.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerow
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/hedgerow.tga
	{
		map textures/misc_outside/hedgerow.tga
		depthWrite
		alphagen heightFade
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerow_backculled
{
	qer_keyword masked
	qer_keyword foliage
    qer_editorimage textures/misc_outside/hedgerow_culled.tga
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
    surfaceparm solid
	cull front
	nopicmip
	{
		map textures/misc_outside/hedgerow.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerowsml_backculled
{
	qer_keyword masked
	qer_keyword foliage
    qer_editorimage textures/misc_outside/hedgerowsml_culled.tga
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm solid
	cull front
	nopicmip
	{
		clampmap textures/misc_outside/hedgerowsml.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerowsml_backculled_noclamp
{
	qer_keyword masked
	qer_keyword foliage
     qer_editorimage textures/misc_outside/hedgerowsml_culled.tga
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm solid
	cull front
	nopicmip
	{
		map textures/misc_outside/hedgerowsml.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerowsml
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/misc_outside/hedgerowsml.tga
	cull none
	nopicmip
	{
		clampmap textures/misc_outside/hedgerowsml.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerowsml_noclamp
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/misc_outside/hedgerowsml.tga
	cull none
	nopicmip
	{
		map textures/misc_outside/hedgerowsml.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
} 

textures/misc_outside/hedgerowsml_end
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/misc_outside/hedgerowsml_end.tga
	cull none
	nopicmip
	{
		clampmap textures/misc_outside/hedgerowsml_end.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/weeds
{
	qer_keyword masked
	qer_keyword foliage
	surfaceparm grass
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/weeds.tga
	{
		map textures/misc_outside/weeds.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/fencerow_stone1
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword grass
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/fencerow_stone1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/rockwall3_greytrim
{
	qer_keyword wall
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall3_greytrim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/rockwall3_greytrimwin
{
	qer_keyword wall
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall3_greytrimwin.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/fencerow_stone1lng
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword grass
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/fencerow_stone1lng.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/sf_deepbluesea
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
	deformVertexes move 0 2 2 sin -3 1 0 .1
	deformVertexes move 0 2 2 sin 0 5 0 .05
	deformVertexes bulge -6 6 .3
//	deformVertexes wave 1000 sin 100 80 1 .3
//	deformVertexes bulge -6.2831853 16 .8
	//bulge = numberoftimesrepeatedpertexture amplitude frequency
	{
		map textures/misc_outside/ocean2.tga
		blendFunc blend
		depthWrite
		rgbGen identity
		alphaGen dot .1 .6
		tcMod scroll .01 0
		tcMod scale 9 7
		tcMod turb 0 .08 0 .08
	}
	{
		map textures/misc_outside/ocean2.tga
		blendFunc add
		depthWrite
		rgbGen identity
		rgbGen const (.95 .94 1)
		alphaGen dot .1 .6
		tcMod scroll -.01 .002
		tcMod turb 0 .005 0 .008
		tcMod scale 10 9
	}
	{
		map textures/misc_outside/froth.tga
		blendFunc blend
		alphaGen dot .1 .6
		tcMod scroll .01 .01
		tcMod turb 0 .01 0 .05
		tcMod scale 9 7
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}


textures/misc_outside/water_nodraw
{
	qer_editorimage textures/misc_outside/ocean1a.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	surfaceParm trans
	surfaceParm water
	surfaceParm nomarks
//	surfaceParm nodraw
	cull none
	{
		map textures/misc_outside/ocean1a.tga
		rgbGen identity
//		nextbundle
//		map $lightmap
	}

}

textures/beams/sf_shoregradient
{
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceParm nolightmap
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
	deformVertexes move 0 2 2 sin -3 1 0 .1
	deformVertexes bulge -6 6 .3
//	deformVertexes wave 1000 sin 100 80 1 .3
//	deformVertexes move 0 2 2 sin 0 5 0 .05
//	deformVertexes bulge -6.2831853 16 .8
	{
//		map textures/beams/gradient_distance_rotated90right.tga
		map textures/beams/gradient_distance2.tga
//		blendFunc
		rgbGen const (.4 .4 .4)
//		tcMod scale .08 .01
		tcMod offset 1 0
	}
}

textures/misc_outside/sf_ddayocean
{
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_keyword m2
	qer_trans .4
	//surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	tessSize 128
	deformVertexes bulge -1.5 12 .1
//	deformVertexes move 0 2 2 sin -3 1 0 .1
//	deformVertexes bulge -6 6 .3
//	deformVertexes move 0 3 1 sin -3 1 0 .0143
	{
		map textures/misc_outside/ocean2.tga
       //blendFunc add
		rgbGen identity
		//alphaGen const .7
//		tcMod scroll -.01 .002
		tcMod bulge -1.5 .02 .1 .5
		tcMod turb 0 .005 0 .008
		tcMod scale 10 9
	}
	{
		map textures/misc_outside/ocean1_no_alpha.tga
	//	blendFunc blend
		alphaGen sCoord 2 -.1 .3 .6
		tcMod turb 0 .01 0 .05
	nextbundle
		// I put the lightmap into a nextbundle so that it would get alpha'd out by the alphaGen of this stage.
		map $lightmap
		rgbGen Identity
	}
	{
		map textures/misc_outside/froth2.tga
		alphaGen oneMinusDot -1 1.1
	    blendfunc GL_SRC_ALPHA GL_ONE
//		tcMod bulge -1.5 .02 .1 .5
		tcMod scroll -.003 .005
		tcMod turb 0 .02 0 .01
		tcMod scale 10 10
	nextbundle
		map textures/misc_outside/froth2.tga
//		tcMod bulge -1.5 .02 .1 .5
		tcMod scroll -.001 -.007
//		turb base amp phase freq
		tcMod turb 0 .01 .2 .01
		tcMod scale 7 7.5
	}
}

textures/misc_outside/old_sf_ddayocean
{
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_keyword m2
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
//	deformVertexes move 0 2 2 sin 0 5 0 .05
//	deformVertexes bulge -6.2831853 16 .8
//	deformVertexes wave 1000 sin 100 80 1 .3
	deformVertexes move 0 2 2 sin -3 1 0 .1
	deformVertexes bulge -6 6 .3	// bulge = numberoftimesrepeatedpertexture amplitude frequency
//	{
//		map textures/misc_outside/ocean2.tga
//		blendFunc add
//		depthWrite
//		rgbGen identity
//		alphaGen dot .1 .6
//		tcMod scroll .01 0
//		tcMod scale 1 .5
//		tcMod turb 0 .08 0 .08
//	}
//	{
//		map textures/misc_outside/ocean2.tga
//		blendFunc add
//		depthWrite
//		rgbGen identity
//		rgbGen const (.95 .94 1)
//		alphaGen dot .1 .6
//		tcMod scroll -.01 .002
//		tcMod turb 0 .005 0 .008
//		tcMod scale 10 9
//	}
	{
		map textures/misc_outside/froth.tga
		alphaGen sCoord 2.0 .2
//		alphaGen dot .1 .6
		blendFunc blend
		tcMod scroll .01 .01
		tcMod turb 0 .01 0 .05
		tcMod scale 1 .5
	}
//	{
//		map textures/beams/gradient_distance2.tga
//		blendFunc blend
//	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//	}
}

textures/misc_outside/basicocean
{
	qer_editorimage textures/misc_outside/ocean1.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean1.tga
		blendFunc blend
		depthWrite
		rgbGen identity
		tcMod scroll .05  -.025
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

//22 X 62
textures/misc_outside/deepbluesea
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// spread sin base amp phase freq (wave/sec)
////	deformvertexes wave 1000 sin 0 40 0 .20
	deformvertexes flap t 10 sin 0 10 0 .10 0 10
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		rgbGen identityLighting
		tcMod scale 16 22//15 7
		tcMod scroll 0.01 .03
      nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -16 22//15 7
		tcMod scroll 0.01 0.04
	}
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
     		blendFunc add
     	   	tcMod scale .2 .5
		tcMod scroll 0 .005
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale .2 .5
		tcMod scroll 0 .01
	}
} 

textures/misc_outside/deepbluesea_runup
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// spread sin base amp phase freq (wave/sec)
////	deformvertexes wave 1000 sin 0 40 0 .20
	deformvertexes flap t 10 sin 0 3 0 .10 0 3
	{
		nopicmip	
		depthwrite
		map textures/misc_outside/oceandday1.tga
		blendFunc blend
		tcMod scale 16 22//15 7
		tcMod scroll 0.01 -0.2
      nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -16 22//15 7
        tcMod scroll 0.01 -0.25
	}
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
        blendFunc add
		//tcMod scale 32 22//15 7
        tcMod scale .2 .5
//		tcMod scroll 0 .005
		tcMod scroll 0 -0.00325
      nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale .2 .5
//		tcMod scroll 0 .01
		tcMod scroll 0 -0.0035
        //tcMod turb .1 .3 .2 .1
	}
} 

//johnfitz -- new version of ocean shader with stationary verts
textures/misc_outside/deepbluesea_runup_nowave
{
	qer_editorimage textures/misc_outside/deepbluesea_editor.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean

	qer_trans .4
	surfaceparm trans
	surfaceparm water
	surfaceparm nolightmap
	cull none

	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc blend
		tcMod scale 16 5
		tcMod scroll 0.01 -0.034
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -16 5
		tcMod scroll 0.01 -0.034
	}
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		tcMod scale 0.2 0.105
		tcMod scroll 0 -0.005
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 0.2 0.105
		tcMod scroll 0 -0.009
	}
}
//johnfitz

textures/misc_outside/deepbluesea_shoreline
{
// the deepbluesea_editor editor image is for easier aligning
//	qer_editorimage textures/misc_outside/ocean2.tga
	qer_editorimage textures/misc_outside/deepbluesea_editor.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean

	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// these first two layers are the fading continuation of the ocean
    {
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc blend
//		alphaGen tCoord 1.01 -0.01
		alphaGen tCoord 1.8 -0.01
		tcMod scale 16 5
		tcMod scroll 0.01 -0.034
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -16 5
		tcMod scroll 0.01 -0.034
	}
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
//		alphaGen tCoord 1.01 -0.01
		alphaGen tCoord 1.01 -0.5
		//tcMod scale 32 22//15 7
		tcMod scale 0.2 0.105
		tcMod scroll 0 -0.005
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 0.2 0.105
		tcMod scroll 0 -0.009
		//tcMod turb .1 .3 .2 .1
	}
// the next two layers combine into a single shore wash
    {
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc add
		rgbGen wave sin .15 .525 .35 -.04
		tcMod scale 8 1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .5 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 8 1.1	
		tcMod scroll -0.025 -0.025
    }
	{
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc add
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .325 -.04
		tcMod scale -8 1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .45 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -8 1.1	
		tcMod scroll -0.025 -0.025
    }
// the next two layers combine into a single shore wash
    {
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc add
		rgbGen wave sin .15 .525 .85 -.04
		tcMod scale 8 1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 0 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 8 1.1	
		tcMod scroll -0.025 -0.025
    }
	{
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc add
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .825 -.04
		tcMod scale -8 1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .95 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -8 1.1	
		tcMod scroll -0.025 -0.025
    }
}

textures/misc_outside/river
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
		surfaceparm nolightmap
	cull none
	//deformvertexes wave 30 sin 0 60 0 .1
      deformvertexes wave 30 sin 0 5 0 .2
	{
		map textures/misc_outside/ocean1a.tga
		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA 
		alphaGen lightingSpecular
		tcMod scroll .0 0.15
//		tcMod scale 2.50 2.50
	}
	{
		map textures/misc_outside/ocean2a.tga
		blendFunc add
//		tcMod scale .333 .33
		tcMod scroll .0 .10
//		tcMod turb 0 .2 0 .1
		tcMod scale 4 1
//		tcMod turb .1 .3 .2 .1
		tcMod scale .25 1
	nextbundle
		map textures/misc_outside/ocean2a.tga
		tcMod scale .55 .55
		tcMod scroll .0 .08
//		tcMod scroll -.03 -.05
	}
	
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/riversf
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
		surfaceparm nolightmap
	cull none
	//deformvertexes wave 30 sin 0 60 0 .1
      deformvertexes wave 30 sin 0 5 0 .2
	{
		map textures/misc_outside/froth.tga
		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
//        blendfunc blend 
//		alphaGen lightingSpecular
		tcMod scroll .0 0.15
//		tcMod scale 2.50 2.50
	}
	{
		map textures/misc_outside/ocean2.tga
		blendFunc blend
//		tcMod scale .333 .33
		tcMod scroll .0 .10
//		tcMod turb 0 .2 0 .1
		tcMod scale 4 1
//		tcMod turb .1 .3 .2 .1
		tcMod scale .25 1
	nextbundle
		map textures/misc_outside/ocean2.tga
		tcMod scale .55 .55
		tcMod scroll .0 .08
//		tcMod scroll -.03 -.05
	}
	
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/multiocean
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
//	surfaceparm nolightmap
	cull none
	deformvertexes wave 20 sin 0 40 0 .2
//	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean1.tga
		tcGen environment
		tcMod scale .222 .222
		tcMod scroll .003 .004
	}
	{
		map textures/misc_outside/froth1.tga
		blendFunc GL_ONE GL_ONE
		tcMod scale .255 .255
		tcMod turb .1 .3 .2 0
		tcMod scroll .003 .03
	nextbundle
		map textures/misc_outside/froth1.tga
		tcMod scale .039 .039
		tcMod scroll .02 .02
	}
	{
		map textures/misc_outside/froth1.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		alphaGen lightingSpecular
		tcMod scroll .07 -.04
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/largestream
{
	qer_editorimage textures/misc_outside/stream1.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword river
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .003 0 1
	nextbundle
		map textures/misc_outside/stream2fx.tga
		tcGen environment
		tcMod turb 0 .001 .3 4
	}
	{
		map textures/misc_outside/stream1.tga
		blendFunc blend
		rgbGen identity
		tcMod scroll .05 -.025
//		tcMod trans .5
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/misc_outside/strange_frothy_water
{
	qer_editorimage textures/misc_outside/stream1.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull none
	nopicmip
	//deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/ocean2.tga
		tcGen environment
		tcMod turb 0 .003 0 1
	nextbundle
		map textures/misc_outside/ocean1.tga
		tcGen environment
		tcMod turb 0 .001 .3 1
	}
	{
		map textures/misc_outside/ocean1.tga
		blendFunc GL_ONE GL_ONE
		blendFunc blend
		rgbGen identity
		tcMod scroll .05  -.025
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/misc_outside/hedgerowfence
{
	qer_keyword wood
	qer_keyword natural
	qer_keyword tree
	qer_keyword folliage
	qer_keyword m3
	qer_keyword wall
	surfaceparm wood
	{
		map textures/misc_outside/hedgerowfence.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nettingtest
{
	qer_editorimage textures/misc_outside/netting.tga
	qer_keyword special
	qer_keyword masked
	surfaceparm carpet
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/misc_outside/netting.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/windowgrate2
{
	qer_editorimage textures/misc_outside/iron_window_grate2.tga
	qer_keyword door
	qer_keyword metal
	qer_keyword masked
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nomipmaps
	nopicmip
	{
		map textures/misc_outside/iron_window_grate2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/windowgrate2a
{
	qer_editorimage textures/misc_outside/iron_window_grate2a.tga
	qer_keyword door
	qer_keyword masked
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/misc_outside/iron_window_grate2a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/blowingleaves
{
	qer_editorimage textures/misc_outside/leaves_blown.tga
	qer_keyword natural
	qer_keyword folliage
	qer_keyword masked
	surfaceparm glass
	surfaceparm nonsolid
	surfaceparm trans
	cull none
	nopicmip
	deformvertexes wave 30 sin 0 10 0 .2
	{
		map textures/misc_outside/leaves_blown.tga
		tcMod scale 2 2
		tcMod scroll 0 .25
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/nu_earth_set1
{
	qer_keyword natural
	qer_keyword flat
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/nu_earth_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_earth_set1berm
{
	qer_keyword grass
	qer_keyword natural
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/nu_earth_set1berm.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_earth_set2
{
	qer_keyword natural
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/nu_earth_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_earth_set2berm
{
	qer_keyword grass
	qer_keyword natural
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/nu_earth_set2berm.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_earth_set2edge
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/nu_earth_set2edge.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_grass_set1
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	surfaceparm grass
	{
		map textures/misc_outside/nu_grass_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_grass_set2
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	surfaceparm grass
	{
		map textures/misc_outside/nu_grass_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/rubblewithbrokenglass
{
	qer_editorimage textures/misc_outside/rubble2cwglass.tga
	qer_keyword flat
	qer_keyword natural
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/misc_outside/rubble2cwglass.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/misc_outside/tileroof1
{
	qer_keyword stone
	qer_keyword roof
	surfaceparm rock
	{
		map textures/misc_outside/tileroof1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/omahagrass_1
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/omahagrass_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocage_treerow1
{
	qer_keyword terrain
	qer_keyword wall
	qer_keyword m3
	qer_keyword folliage
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/bocage_treerow1.tga
	{
		map textures/misc_outside/bocage_treerow1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_grass_set2a_sp
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/nu_grass_set2a_sp.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/nu_grass_set2_sp
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		clampmap textures/misc_outside/nu_grass_set2_sp.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/nu_grass_set2_spdim
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		clampmap textures/misc_outside/nu_grass_set2_spdim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/steve_grass512_set2
{
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/steve_grass512_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/ncnu_grass_set2_sp
{
	qer_editorimage textures/misc_outside/nu_grass_set2_sp.tga
	qer_keyword terrain
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/nu_grass_set2_sp.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocage_treerow1rev
{
	qer_keyword masked
	qer_keyword terrain
	qer_keyword tree
	qer_keyword folliage
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	nopicmip
	cull none
	qer_editorimage textures/misc_outside/bocage_treerow1rev.tga
	{
		map textures/misc_outside/bocage_treerow1rev.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/secfence2
{
	qer_keyword masked
	qer_keyword wall
	qer_keyword rusted
	qer_keyword metal
	surfaceparm grill
	cull none
	nopicmip
	{
		map textures/misc_outside/secfence2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/bocroad_set1b
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroad_chopalt1
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_chopalt1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/concpath1_wgrass
{
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/misc_outside/concpath1_wgrass.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/concpath1a_wgrass
{
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/misc_outside/concpath1a_wgrass.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/grass_paths1
{
	qer_keyword road
	qer_keyword terrain
	qer_keyword floor
	qer_keyword grass
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/grass_paths1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/rockwall_2vine
{
	qer_keyword natural
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall_2vine.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/rockwall_heavy
{
	qer_keyword natural
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall_heavy.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocage_fencetop
{
	qer_keyword trim
	qer_keyword flat
	qer_keyword folliage
	qer_keyword natural
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/bocage_fencetop.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/damaged_siding_1
{
	qer_keyword masked
	qer_keyword wall
	qer_keyword wood
	surfaceparm wood
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/misc_outside/damaged_siding_1.tga
	{
		map textures/misc_outside/damaged_siding_1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/sandbagwall1tmp
{
	qer_keyword wall
	qer_keyword sand
	surfaceparm dirt
	{
		map textures/misc_outside/sandbagwall1tmp.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/sandbag_top
{
	qer_keyword wall
	qer_keyword sand
	surfaceparm dirt
	{
		map textures/misc_outside/sandbag_top.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/rockwall_s1
{
	qer_keyword terrain
	qer_keyword floor
	qer_keyword wall
	qer_keyword natural
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall_s1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/trans_1a_ce
{
	qer_keyword brick
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/trans_1a_ce.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocg_rockwall_moss
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/bocg_rockwall_moss.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocg_rockwall_vine
{
	qer_keyword wall
	qer_keyword folliage
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/bocg_rockwall_vine.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocageroad_putty
{
	qer_keyword floor
	qer_keyword gravel
	qer_keyword natural
	surfaceparm dirt
	{
		map textures/misc_outside/bocageroad_putty.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/mosswall
{
	qer_keyword floor
	qer_keyword wall
	qer_keyword natural
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/mosswall.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/moss_step_frontx
{
	qer_keyword trim
	qer_keyword stone
	qer_keyword rock
	qer_keyword natural
	qer_keyword floor
	surfaceparm rock
	{
		map textures/misc_outside/moss_step_frontx.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/moss_stepx
{
	qer_keyword stone
	qer_keyword rock
	qer_keyword natural
	qer_keyword floor
	surfaceparm rock
	{
		map textures/misc_outside/moss_stepx.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_set2
{
	qer_keyword grass
	qer_keyword gravel
	qer_keyword terrain
	qer_keyword road
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_set2rad
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword grass
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set2rad.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/nugrass_roadbrdr3
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword grass
	surfaceparm dirt
	{
		map textures/misc_outside/nugrass_roadbrdr3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_set3rad
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set3rad.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_full
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_full.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 
textures/misc_outside/bocroad_fullns
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
//	surfaceparm dirt
	surfaceparm nonsolid
	qer_editorimage textures/misc_outside/bocroad_full.tga
	{
		map textures/misc_outside/bocroad_full.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 
textures/misc_outside/bocroad_fullns_noshd
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
//	surfaceparm dirt
	surfaceparm nonsolid
	surfaceParm trans
	qer_editorimage textures/misc_outside/bocroad_full.tga
	{
		map textures/misc_outside/bocroad_full.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/hedgerow_transition
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword grass
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/hedgerow_transition.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/hedgetrans_grass
{
	qer_keyword terrain
	qer_keyword trim
	qer_keyword grass
	surfaceparm grass
	{
		map textures/misc_outside/hedgetrans_grass.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_set3_0
{
	qer_editorimage textures/misc_outside/bocroad_set3.tga
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword grass
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_0
{
	qer_editorimage textures/misc_outside/bocroad.tga
	qer_keyword gravel
	qer_keyword terrain
	qer_keyword road
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/rubbleroad_fill
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/rubbleroad_fill.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocage_stevereq
{
	qer_keyword road
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/bocage_stevereq.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/remagen_clifface1
{
	qer_keyword m6
	qer_keyword natural
	qer_keyword gravel
	qer_keyword rock
	qer_keyword wall
	qer_keyword terrain
	surfaceparm dirt
	{
		map textures/misc_outside/remagen_clifface1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/rock_cliffside_trans
{
	qer_keyword terrain
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt

	{
		map textures/misc_outside/rock_cliffside_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
textures/misc_outside/rock_cliffside_trans_noshd
{
	qer_keyword terrain
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	surfaceparm trans
	{
		map textures/misc_outside/rock_cliffside_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/misc_outside/remag_cliffside_trans
{
	qer_keyword m6
	qer_keyword terrain
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/remag_cliffside_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/stset_1flatsml_trans
{
	qer_keyword gravel
	qer_keyword stone
	qer_keyword road
	qer_keyword special
	qer_keyword terrain
	surfaceparm dirt
	{
		map textures/misc_outside/stset_1flatsml_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/garden_trough
{
	qer_keyword dirt
	qer_keyword brick
	qer_keyword stone
	qer_keyword gravel
	qer_keyword special
	qer_keyword trim
	{
		map textures/misc_outside/garden_trough.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/damaged_siding_1whole
{
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nomipmaps
	nopicmip
	qer_editorimage textures/misc_outside/damaged_siding_1whole.tga
	{
		map textures/misc_outside/damaged_siding_1whole.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/dryearth1_rd
{
	qer_keyword m1
	qer_keyword road
	qer_keyword gravel
	surfaceparm dirt
	{
		map textures/misc_outside/dryearth1_rd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/dryearth1_0
{
	qer_editorimage textures/misc_outside/dryearth1.tga
	qer_keyword grass
	qer_keyword flat
	qer_keyword m1
	surfaceparm grass
	{
		map textures/misc_outside/dryearth1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/treeline_center
{
	qer_editorimage textures/misc_outside/treeline_cntr.tga
	qer_keyword tree
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	//nopicmip
	{
		map textures/misc_outside/treeline_cntr.tga
		depthWrite
		alphagen heightFade 1000 1500
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

textures/misc_outside/furroughed_earth
{
	qer_editorimage textures/misc_outside/furroughed_earth1.tga
	qer_keyword dirt
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earth1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

// No lightmap on this, lightmaps don't look right on water.
textures/misc_outside/ocean2
{
	qer_keyword ocean
	qer_keyword liquid
	qer_keyword natural
	surfaceparm nolightmap
	{
		map textures/misc_outside/ocean2.tga
		rgbGen constant .5 .5 .5
	}
} 

textures/misc_outside/treeline_cntr_shadowed
{
	qer_editorimage textures/misc_outside/treeline_cntr_shadowed.tga
	qer_keyword tree
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	//nopicmip
	{
		map textures/misc_outside/treeline_cntr_shadowed.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

static_vane // Lightning Rod
{
	qer_editorimage textures/misc_outside/lightningrod.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/misc_outside/lightningrod.tga
		depthWrite
		alphaFunc GE128
		rgbGen static
	}
} 

textures/misc_outside/bocroad_set4
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword grass
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/algae2
{
	qer_editorimage textures/misc_outside/algae2.tga
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	//nopicmip
	{
		map textures/misc_outside/algae2.tga
		depthWrite
		blendfunc blend
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/algae2a
{
	qer_editorimage textures/misc_outside/algae2a.tga
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	//nopicmip
	{
		map textures/misc_outside/algae2a.tga
		depthWrite
		blendfunc blend
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/pond
{
	qer_editorimage textures/misc_outside/algae2a.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
		surfaceparm nolightmap
	cull none
	//deformvertexes wave 30 sin 0 60 0 .1
      deformvertexes wave 2 sin 0 1 0 .2
    {   
        map textures/misc_outside/ocean2.tga
        //tcGen environment2
    }
	{
		map textures/misc_outside/oceanbrite.tga
		blendFunc add
//		tcMod scale .333 .33
		tcMod scroll .01 .01
		tcMod turb 0 .2 0 0
//		tcMod scale 4 1
//		tcMod turb .1 .3 .2 .1
		tcMod scale .40 .50
	nextbundle
		map textures/misc_outside/oceanbrite.tga
		tcMod scale .55 .55
		tcMod scroll -.01 -.01
//		tcMod scroll -.03 -.05
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/canal_sludge
{
	qer_editorimage textures/misc_outside/canal_sludge.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
	surfaceparm nolightmap
	cull none
	//deformvertexes wave 30 sin 0 60 0 .1
        deformvertexes wave 2 sin 0 1 0 .2
        {
		map textures/misc_outside/ocean2_green.tga
		depthwrite
		blendfunc add
  		tcMod scroll 0.03 0.03
	nextbundle
		map textures/misc_outside/oceanbrite.tga
		tcMod scroll -0.02 -0.02
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}	

wplumelarge // Sprite version
{
	qer_editorimage textures/models/natural/waterplume_large.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/waterplume_large.tga
		depthWrite
		blendFunc blend
		rgbGen lightingGrid
	}
}

wplumesmall // Sprite version
{
	qer_editorimage textures/models/natural/waterplume.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/waterplume.tga
		depthWrite
		blendFunc blend
		rgbGen lightingGrid
	}
}

mortarhit // Sprite version
{
	qer_editorimage textures/models/natural/mortarhit.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/mortarhit.tga
		depthWrite
		blendFunc blend
		rgbGen lightingGrid
	}
} 

textures/misc_outside/bocroad_altrnate1
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroad_altrnate1.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/cobblestone2
{
	qer_keyword road
	qer_keyword natural
	qer_keyword stone
	surfaceparm rock
	{
	    map textures/misc_outside/cobblestone2.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/cobblestone2bridge
{
	qer_keyword road
	qer_keyword natural
	qer_keyword stone
	surfaceparm rock
	{
	    map textures/misc_outside/cobblestone2bridge.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroadtobridge
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroadtobridge.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroad4_trans2gravel
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroad4_trans2gravel.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/streetgravel
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/streetgravel.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroadadam
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroadadam.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroadadam2
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroadadam2.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/bocroad_fulladam
{
	qer_keyword dirt
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	surfaceparm dirt
	{
	    map textures/misc_outside/bocroad_fulladam.tga
	    depthWrite
	    rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

//22 X 62
textures/misc_outside/afrika_ocean
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// spread sin base amp phase freq (wave/sec)
////	deformvertexes wave 1000 sin 0 40 0 .20
	deformvertexes flap t 10 sin 0 5 0 .10 0 3
	{
		map textures/misc_outside/oceandday1.tga
		tcMod scale .5 .5
        tcMod scroll 0.01 .03
      nextbundle
		map textures/misc_outside/oceandday.tga
		tcMod scale .5 .5
        tcMod scroll -0.01 0.04
	}
	{
		map textures/misc_outside/oceandday1.tga
        blendFunc add
		tcMod scale 1 1
        tcMod scale 1.2 1.5
		tcMod scroll 0 .005
      nextbundle
		map textures/misc_outside/oceandday.tga
		tcMod scale 1.2 1.5
		tcMod scroll 0 .01
        //tcMod turb .1 .3 .2 .1
	}
} 

textures/misc_outside/afrika_shoreline
{
// the deepbluesea_editor editor image is for easier aligning
//	qer_editorimage textures/misc_outside/ocean2.tga
	qer_editorimage textures/misc_outside/deepbluesea_editor.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// these first two layers are the fading continuation of the ocean
    {
		map textures/misc_outside/oceandday1.tga
		blendFunc blend
		alphaGen tCoord 1.01 -0.01
		tcMod scale 7 2
		tcMod scroll 0.01 -0.034
	nextbundle
		map textures/misc_outside/oceandday.tga
		tcMod scale 7 2
		tcMod scroll -0.01 -0.034
	}
	{
		map textures/misc_outside/oceandday1.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		alphaGen tCoord 1.01 -0.01
		//tcMod scale 32 22//15 7
		tcMod scale 7 2
		tcMod scroll 0 -0.005
	nextbundle
		map textures/misc_outside/oceandday.tga
		tcMod scale 7 2
		tcMod scroll 0 -0.009
		//tcMod turb .1 .3 .2 .1
	}
// the next two layers combine into a single shore wash
    {
		map textures/misc_outside/wash2.tga
		blendFunc add
		rgbGen wave sin .15 .525 .35 -.045
		tcMod scale 7 2	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .5 -.045
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 0.3 0.5	
		tcMod scroll -0.025 -0.025
    }
	{
		map textures/misc_outside/wash2.tga
		blendFunc add
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .325 -.045
		tcMod scale 7 2	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .45 -.045
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 7 2	
		tcMod scroll -0.025 -0.025
    }
// the next two layers combine into a single shore wash
    {
		map textures/misc_outside/wash2.tga
		blendFunc add
		rgbGen wave sin .15 .525 .85 -.045
		tcMod scale 7 2	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 0 -.045
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 7 2	
		tcMod scroll -0.025 -0.025
    }
	{
		map textures/misc_outside/wash2.tga
		blendFunc add
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .825 -.045
		tcMod scale 7 2	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.725 -.3 .95 -.045
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 7 2	
		tcMod scroll -0.025 -0.025
    }
}

textures/misc_outside/mudpath_trans
{
	qer_keyword natural
	qer_keyword mud
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/misc_outside/mudpath_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/mudpath_trans2
{
	qer_keyword natural
	qer_keyword mud
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/misc_outside/mudpath_trans2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/rockwall2
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword grass
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/rockwall2_windowed
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword grass
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/rockwall2_windowed.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/fieldstone_wall
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword grass
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/fieldstone_wall.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/trench_mud
{
	qer_keyword natural
	qer_keyword mud
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/misc_outside/trench_mud.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/trench_mud_trans
{
	qer_keyword natural
	qer_keyword mud
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/misc_outside/trench_mud_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocagegrass_furrough
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/bocagegrass_furrough.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocroad_set4gouge
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/bocroad_set4gouge.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/furroughed_earthgouge
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earthgouge.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
  
textures/misc_outside/furroughed_earthgougeend
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earthgougeend.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
 
textures/misc_outside/furroughed_earthud
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earthud.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/furroughed_earth1ud 
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earth1ud.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/furroughed_earth1lr
{
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/misc_outside/furroughed_earth1lr.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/bocage_rockgrasstrans
{
	qer_keyword grass
	qer_keyword trim
	qer_keyword natural
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/bocage_rockgrasstrans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/subpen_water
{
	qer_editorimage textures/misc_outside/ocean2.tga

	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
	surfaceparm nolightmap		// needed?
	cull none
	deformvertexes wave 30 sin 0 5 0 .2
	//{
	//	map textures/misc_outside/ocean2.tga
	//	tcMod scroll .0 0.15
	//}
	{
		map textures/misc_outside/ocean5.tga
		//blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
		//alphaGen lightingSpecular
		tcMod scroll .01 .1
	}
	{
		map textures/misc_outside/ocean5.tga
		blendFunc add
		//alphaGen lightingSpecular
		tcMod scroll -.01 .05
		tcMod scale 2 2
	}


//	{
//		map textures/misc_outside/froth2.tga
//		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
//		alphaGen lightingSpecular
//		tcMod scroll .0 0.15
//	}
//	{
//		map textures/misc_outside/ocean2.tga
//		blendFunc add
//		tcMod scroll .0 .10
//		tcMod scale 4 1
//		tcMod scale .25 1
//	nextbundle
//		map textures/misc_outside/ocean2.tga
//		tcMod scale .55 .55
//		tcMod scroll .0 .08
//	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//		depthFunc equal
//	}
}

textures/misc_outside/subpen_water2
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	qer_trans .4
	surfaceparm trans
	surfaceparm water
	surfaceparm nolightmap
	cull none
	deformVertexes wave 30 sin 0 5 0 .2
	{
		map textures/misc_outside/ocean2.tga
		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA 
		alphaGen lightingSpecular
		tcMod scroll .0 0.15
	}
	{
		map textures/misc_outside/ocean2a.tga
		blendFunc add
		tcMod scroll .0 .025
		tcMod scale .25 .25
	nextbundle
		map textures/misc_outside/ocean2a.tga
		tcMod scale .125 .125
		tcMod scroll .0 .02
	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//		depthFunc equal
//	}
}

textures/misc_outside/subpen_water3
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword ocean
	qer_keyword liquid
	qer_keyword natural
	surfaceparm water
	{
		map textures/misc_outside/ocean2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/misc_outside/subpen_water4
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean
	surfaceparm trans
	surfaceparm water
	{
		map textures/misc_outside/ocean1b.tga
//		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/treeline_cntr_shadowed_senn
{
	qer_editorimage textures/misc_outside/treeline_cntr_shadoweds.tga
	qer_keyword tree
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	{
		map textures/misc_outside/treeline_cntr_shadowed.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/misc_outside/treeline_cntr_shadowed_z
{
	qer_editorimage textures/misc_outside/treeline_cntr_shadowed.tga
	qer_keyword tree
	qer_keyword foliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
	nomipmaps
	nopicmip
//	cull none
	{
		map textures/misc_outside/treeline_cntr_shadowed.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/misc_outside/riverbed
{
	qer_keyword river
	qer_keyword wall
	qer_keyword natural
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/riverbed.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/misc_outside/sewerbottom
{
	qer_keyword sewer
	qer_keyword rock
	surfaceparm rock
	{
		map textures/misc_outside/sewerbottom.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

////////////////////////////////////////
// north afrika lighthouse shoreline
////////////////////////////////////////

textures/misc_outside/northafrika_shoreline
{
// the deepbluesea_editor editor image is for easier aligning
//	qer_editorimage textures/misc_outside/ocean2.tga
	qer_editorimage textures/misc_outside/deepbluesea_editor.tga
	qer_keyword natural
	qer_keyword liquid
	qer_keyword ocean

	qer_trans .4
	surfaceparm trans
	surfaceparm water
    surfaceparm nolightmap
	cull none
// these first two layers are the fading continuation of the ocean
    {
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc blend
//		alphaGen tCoord 1.01 -0.01
//		alphaGen tCoord 1.8 -0.01
			alphaGen tCoord 1.1 -0.1
		tcMod scale 16 -5
		tcMod scroll 0.01 -0.034
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -16 -5
		tcMod scroll 0.01 -0.034
	}
	{
		nopicmip	
		map textures/misc_outside/oceandday1.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
//		alphaGen tCoord 1.01 -0.01
//		alphaGen tCoord 1.01 -0.5
			alphaGen tCoord 1.1 -0.1
		//tcMod scale 32 22//15 7
		tcMod scale 0.2 -0.105
		tcMod scroll 0 -0.005
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 0.2 -0.105
		tcMod scroll 0 -0.009
		//tcMod turb .1 .3 .2 .1
	}
// the next two layers combine into a single shore wash
    {
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc alphaadd
			alphaGen tCoord 1.1 -0.1
		rgbGen wave sin .15 .525 .35 -.04
		tcMod scale 3 -1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.775 -.3 .5 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 3 -1.1	
		tcMod scroll -0.025 -0.025
    }
	{
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc alphaadd
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .325 -.04
			alphaGen tCoord 1.1 -0.1
		tcMod scale -3 -1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.775 -.3 .45 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -3 -1.1	
		tcMod scroll -0.025 -0.025
    }
// the next two layers combine into a single shore wash
    {
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc alphaadd
			alphaGen tCoord 1.1 -0.1
		rgbGen wave sin .15 .525 .85 -.04
		tcMod scale 3 -1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.775 -.3 0 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale 3 -1.1	
		tcMod scroll -0.025 -0.025
    }
	{
		nopicmip	
		map textures/misc_outside/wash2.tga
		blendFunc alphaadd
			alphaGen tCoord 1.1 -0.1
		//blendFunc GL_ONE GL_SRC_ALPHA
		rgbGen wave sin .15 .525 .825 -.04
		tcMod scale -3 -1.1	
		tcMod scroll 0.01 .0
		tcMod wavetrant  sin 0.775 -.3 .95 -.04
	nextbundle
		map textures/misc_outside/oceandday1.tga
		tcMod scale -3 -1.1	
		tcMod scroll -0.025 -0.025
    }
}

