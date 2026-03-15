textures/flags/nazi1
{
	qer_keyword special
	surfaceparm nonsolid
	qer_editorimage textures/flags/nazi1.tga
	surfaceparm alphashadow
	cull none
	//deformvertexes flap t 10 sin 1 3 50 .08 0 10
	{
		map textures/flags/nazi1.tga
		//depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
        }
}

//////////////////////////////////////
//nazibanner and brass eagle top models
//////////////////////////////////////

static_flagtop2
{
	cull none
	qer_editorimage textures/models/flags/flagtop.tga
	{
		map textures/models/flags/flagtop.tga
		blendfunc blend
		depthwrite
		//alphafunc GE128
		rgbGen static
	}
}

static_flagtop
{
	qer_editorimage textures/models/flags/flagtop.tga
	{
		map textures/models/flags/flagtop.tga
		blendfunc blend
		//alphafunc GE128
		rgbGen static
	}
}
static_nazibanner1
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/nazibanner.tga
	//surfaceparm alphashadow
	cull none
	//deformvertexes flap t 1 sin 1 1 50 .08 0 2
	deformvertexes flap t 1 sin 0 1 0 .10 0 2
	{
		map textures/models/flags/nazibanner.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}

static_nazibanner1a
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/nazibanner.tga
	//surfaceparm alphashadow
	cull none
	//deformvertexes flap t 1 sin 1 1 50 .08 0 2
	deformvertexes flap t 1 sin 0 1 0 .35 0 6
	{
		map textures/models/flags/nazibanner.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}


static_nazibanner1b
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/nazibanner.tga
	//surfaceparm alphashadow
	cull none
	//deformvertexes flap t 1 sin 1 1 50 .08 0 2
	deformvertexes flap t 1 sin 0 1 0.75 .36 0 6
	{
		map textures/models/flags/nazibanner.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}
static_nazibanner2
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/nazibanner2.tga
	//surfaceparm alphashadow
	cull none
	//deformvertexes flap t 1 sin 1 1 50 .08 0 2
	deformvertexes flap t 1 sin 0 1 0 .10 0 2
	{
		map textures/models/flags/nazibanner2.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}

static_kriegsflag
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/kriegsmarine.tga
	//surfaceparm alphashadow
	cull none
	deformVertexes flap s 128 sin .5 6 1   -1 0 2
	deformVertexes flap s 64 sin   1 3 0.5 -1 0 1.5
	{
		map textures/models/flags/kriegsmarine.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}

cranebanner
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/naziflag1.tga
	//surfaceparm alphashadow
	cull none
	deformvertexes flap t 1 sin 0 .5 0 .20 0 1
	{
		map textures/models/flags/naziflag1.tga
		rgbgen lightingSpherical
      }
}

static_naziflag2
{
	//qer_keyword special
	//surfaceparm nonsolid
	qer_editorimage textures/models/flags/naziflag2.tga
	//surfaceparm alphashadow
	cull none
	deformVertexes flap s 128 sin .5 6 1   -1 0 2
	deformVertexes flap s 64 sin   1 3 0.5 -1 0 1.5
	{
		map textures/models/flags/naziflag2.tga
		//depthWrite
		//alphaFunc GE128
		rgbgen static
        }
}
