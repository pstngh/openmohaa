textures/das_boot/brassclock
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	nopicmip
	{
		map textures/das_boot/clockenv.tga
		tcGen environment
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
	{
		clampmap textures/das_boot/brassclock.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		clampmap textures/das_boot/hourhand.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod rotate .0083 225
	}
	{
		clampmap textures/das_boot/minutehand.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod rotate .1 45
	}
}

textures/test/test
{
	qer_editorimage textures/clock/stnclkface.tga
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	PolygonOffset
	nopicmip
	nomipmaps
	{
		clampmap textures/clock/stnclkface.tga
		depthwrite
		alphafunc GE128
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
	{
		clampmap textures/clock/stnclkhour.tga
		alphafunc GE128
		tcMod rotate .008333333 180
	}
	{
		clampmap textures/clock/stnclkminute.tga
		alphafunc GE128
		tcMod rotate .1 45
	}
}
textures/test/window
{
cull none
	{
		map textures/test/window_env.tga
		tcgen environment
		alphagen const .1
		blendFunc blend
	}
	{
		map textures/test/window.tga
		blendFunc blend
	   nextbundle
		map $lightmap
	}
}

textures/test/spr512_shot1
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/test/spr512_shot1.tga
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

textures/test/spr512_shot2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/test/spr512_shot2.tga
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

textures/test/sandtest1
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/test/sandtest1.tga
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

textures/test/sandtest2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm gravel
	{
		map textures/test/sandtest2.tga
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

textures/test/omaha_pjspick9
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm gravel
	{
		clampmapy textures/test/omaha_pjspick9.tga
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

textures/test/omaha_pjspick8
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		clampmapy textures/test/omaha_pjspick8.tga
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

textures/test/omaha_pjspick7
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick7.tga
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

textures/test/omaha_pjspick6
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick6.tga
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

textures/test/omaha_pjspick5
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick5.tga
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

textures/test/omaha_pjspick4
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick4.tga
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

textures/test/omaha_pjspick_sline
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick_sline.tga
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

textures/test/omaha_pjspick3
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick3.tga
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

textures/test/omaha_pjspick2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick2.tga
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

textures/test/omaha_pjspick1
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_pjspick1.tga
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

textures/test/omaha_set5blast2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5blast2.tga
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

textures/test/omaha_set5brd1
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5brd1.tga
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

textures/test/omahasandtrans5
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omahasandtrans5.tga
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

textures/test/omaha_set5_shingle
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5_shingle.tga
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

textures/test/omaha_set5mix
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5mix.tga
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

textures/test/omaha_set5_shoreline
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5_shoreline.tga
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

textures/test/omaha_set5
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5.tga
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

textures/test/nu_earth_set5blast1
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/nu_earth_set5blast1.tga
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

textures/test/omaha_set5_covered
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5_covered.tga
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

textures/test/omaha_set5mixhvy
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm sand
	{
		map textures/test/omaha_set5mixhvy.tga
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

static_planetmap
{
	qer_editorimage textures/test/planetmap.tga
	{
		map textures/test/planetmap.tga
		rgbGen lightingSpherical
		//rgbGen static

	}
}

static_rockwallmodel
{
	qer_editorimage textures/test/rockwall.tga
	{
		map textures/test/rockwall.tga
		rgbGen static
	}
}


static_naziflag1
{
	
	surfaceparm monsterclip
	surfaceparm playerclip
    qer_editorimage textures/models/submodels/naziflag1.tga
	//surfaceparm alphashadow
	//surfaceparm carpet
	cull none
       // deformvertexes flap s 16 sin 0 2 5 .8 0 2
       // deformvertexes flap s 16 sin 0 1 2 .4 0 2
	deformVertexes flap s 128 sin .5 6 1   -1 0 2
	deformVertexes flap s 64 sin   1 3 0.5 -1 0 1.5
     //deformVertexes flap t 64 sin   1 3 0.5 1 0 1.5

	{
		map textures/models/submodels/naziflag1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

