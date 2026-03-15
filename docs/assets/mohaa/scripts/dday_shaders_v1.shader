// D-Day Specialty Shaders
textures/misc_outside/sf_deepbluesea
{
	qer_editorimage textures/misc_outside/ocean2.tga
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
//	deformVertexes wave 1000 sin 100 80 1 .3
	deformVertexes move 0 2 2 sin -3 1 0 .1
	deformVertexes move 0 2 2 sin 0 5 0 .05
//	deformVertexes bulge -6.2831853 16 .8
	deformVertexes bulge -6 6 .3
//	bulge = numberoftimesrepeatedpertexture amplitude frequency
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
		rgbGen const .95 .94 1
		alphaGen dot .1 .6
		tcMod scroll -.01 .002
		tcMod turb 0 .005 0 .008
		tcMod scale 10 9
	}
	{
		map textures/misc_outside/froth.tga
		blendFunc blend
		alphaGen dot .1 .6.
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


textures/beams/sf_shoregradient
{
//	qer_editorimage textures/beams/gradient_distance_rotated90right.tga
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_trans .4
	surfaceParm nolightmap
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
//	deformVertexes wave 1000 sin 100 80 1 .3
	deformVertexes move 0 2 2 sin -3 1 0 .1
//	deformVertexes move 0 2 2 sin 0 5 0 .05
//	deformVertexes bulge -6.2831853 16 .8
	deformVertexes bulge -6 6 .3
	{
//		map textures/beams/gradient_distance_rotated90right.tga
		map textures/beams/gradient_distance2.tga
//		blendFunc
		rgbGen const .4 .4 .4
//		tcMod scale .08 .01
		tcMod offset 1 0
	}
}

textures/misc_outside/sf_ddayocean
{
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	tessSize 512
//	deformVertexes move 0 2 2 sin -3 1 0 .1
//	deformVertexes bulge -6 6 .3
//	deformVertexes move 0 3 1 sin -3 1 0 .0143
	deformVertexes bulge -1.5 12 .1
	{
		map textures/misc_outside/ocean2.tga
           	blendFunc add
		rgbGen identity
		alphaGen const .7
//		tcMod scroll -.01 .002
		tcMod bulge -1.5 .02 .1 .5
//		tcMod turb 0 .005 0 .008
//		tcMod scale 10 9
		tcMod scale 8 4
		tcMod turb 0 .4 0 .008
	}
	{
		map textures/misc_outside/ocean1_no_alpha.tga
		blendFunc blend
		alphaGen sCoord 2 -.1 .3 .6
		tcMod turb 0 .01 0 .05
	nextbundle
		// I put the lightmap into a nextbundle so that it would get alpha'd
		// out by the alphaGen of this stage.
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
		// turb base amp phase freq
		tcMod turb 0 .01 .2 .01
		tcMod scale 7 7.5
	}
}

old_sf_ddayocean
{
	qer_editorimage textures/beams/gradient_distance2.tga
	qer_trans .4
	surfaceParm trans
	surfaceParm water
	surfaceParm noimpact
	cull none
	tessSize 128
//	deformVertexes wave 1000 sin 100 80 1 .3
	deformVertexes move 0 2 2 sin -3 1 0 .1
//	deformVertexes move 0 2 2 sin 0 5 0 .05
//	deformVertexes bulge -6.2831853 16 .8
	deformVertexes bulge -6 6 .3
//	bulge = numberoftimesrepeatedpertexture amplitude frequency
//	{
//		map textures/misc_outside/ocean2.tga
//            	blendFunc add
//		depthWrite
//		rgbGen identity
//		alphaGen dot .1 .6
//		tcMod scroll .01 0
//		tcMod scale 1 .5
//		tcMod turb 0 .08 0 .08
//	}
//	{
//		map textures/misc_outside/ocean2.tga
//           	blendFunc add
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
		alphaGen sCoord 2 .2
//		alphaGen dot .1 .6.
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