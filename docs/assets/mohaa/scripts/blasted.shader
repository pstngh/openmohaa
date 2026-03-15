textures/blasted/jh_blstfrm1
{
	qer_keyword window
	qer_keyword damaged
	qer_keyword concrete
	qer_keyword masked
	surfaceParm stone
	surfaceParm trans
	cull none
	sort banner
	nopicmip
	{
		map textures/blasted/jh_blstfrm1.tga
		alphaFunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/blasted/jh_concwallwin1
{
	qer_keyword wall
	qer_keyword window
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/blasted/jh_concwinwall1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/blasted/jh_concwallwin1a
{
	qer_keyword window
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/blasted/jh_concwallwin1a.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/blasted/jh_concwallwin1b
{
	qer_keyword window
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/blasted/jh_concwallwin1b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/blasted/jh_concwallwin1c
{
	qer_keyword window
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin1c.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin1d
{
	qer_keyword window
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin1d.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2
{
	qer_keyword window
	qer_keyword wall
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/blasted/jh_concwallwin2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		blendFunc filter
		rgbGen identity
		depthwrite
	}
}

textures/blasted/jh_concwallwin2a
{
	qer_keyword window
	qer_keyword wall
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2a.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2b
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2b.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2c
{
	qer_keyword damaged
	qer_keyword wall
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2c.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2d
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2d.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2e
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2e.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2f
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2f.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2g
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2g.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2h
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2h.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2i
{
	qer_keyword window
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/blasted/jh_concwallwin2i.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/blasted/jh_concwallwin2j
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2j.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/jh_concwallwin2k
{
	qer_keyword wall
	qer_keyword damaged
	qer_keyword concrete
	surfaceParm plaster
	{
		map textures/blasted/jh_concwallwin2k.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/dday_bunker_wall1bdamsml
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm stone
	{
		map textures/BLASTED/dday_bunker_wall1bdamsml.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/dday_bunker_wall1bsml
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm stone
	{
		map textures/BLASTED/dday_bunker_wall1bsml.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/dday_bunker_wall1cdamsml
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm stone
	{
		map textures/BLASTED/dday_bunker_wall1cdamsml.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/dday_bunker_wall1crkdsml
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm stone
	{
		map textures/BLASTED/dday_bunker_wall1crkdsml.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/dday_bunker_wall1csml
{
	qer_keyword wall
	qer_keyword concrete
	surfaceParm stone
	{
		map textures/BLASTED/dday_bunker_wall1csml.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/rebar_ww2
{
	qer_keyword pipe
	qer_keyword metal
	qer_keyword masked
	surfaceParm metal
	surfaceParm trans
    surfaceParm fence
    qer_editorimage textures/blasted/rebar_ww2.tga
	cull none
	nopicmip
	{
		map textures/blasted/rebar_ww2.tga
		alphaFunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}
}

// Omaha beach scene top-away from beach
textures/blasted/grassbrdr_set3_damage
{
	qer_keyword terrain
	qer_keyword m3
	qer_keyword dirt
	surfaceParm dirt
	{
		clampmap textures/blasted/grassbrdr_set3_damage.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/blasted/grassbrdr_set3_damage_noclamp
{
	qer_editorimage textures/blasted/grassbrdr_set3_damage.tga
	qer_keyword terrain
	qer_keyword m3
	qer_keyword dirt
	surfaceParm dirt
	{
		map textures/blasted/grassbrdr_set3_damage.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

// Omaha beach scene beachside-rubble2cc
textures/blasted/grassbrdr_set3_damage2
{
	qer_keyword terrain
	qer_keyword m2
	qer_keyword dirt
	surfaceParm dirt
	{
		clampmap textures/blasted/grassbrdr_set3_damage2.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

textures/blasted/grassbrdr_set4_damage2
{
	qer_keyword terrain
	qer_keyword m2
	qer_keyword dirt
	surfaceParm dirt
	{
		clampmap textures/blasted/grassbrdr_set4_damage2.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}
 
//used in bocage
textures/blasted/grassbrdr_set4_blast
{
	qer_keyword terrain
	qer_keyword m3
	qer_keyword dirt
	surfaceParm dirt
	{
		clampmap textures/blasted/grassbrdr_set4_blast.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

//this is the non clampmapped version of the above
textures/blasted/grassbrdr_set4_blastnclmp
{
	qer_keyword terrain
	qer_keyword m3
	qer_keyword dirt
	surfaceParm dirt
	{
		map textures/blasted/grassbrdr_set4_blast.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 



textures/blasted/bunkerdoor2
{
	qer_keyword door
	qer_keyword metal
	surfaceParm metal
	{
		map textures/blasted/bunkerdoor2.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

textures/blasted/bunkerdoor2a
{
	qer_keyword door
	qer_keyword metal
	surfaceParm metal
	{
		map textures/blasted/bunkerdoor2a.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

textures/blasted/bunkerdoor3
{
	qer_keyword door
	qer_keyword metal
	surfaceParm metal
	{
		map textures/blasted/bunkerdoor3.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

textures/blasted/bunkerdoor4
{
	qer_keyword door
	qer_keyword metal
	surfaceParm metal
	{
		map textures/blasted/bunkerdoor4.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

 

 

