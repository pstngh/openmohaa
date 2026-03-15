textures/mohcommon/archbrkset_1asml
{
	qer_keyword stone
	qer_keyword brick
	qer_keyword trim
	qer_keyword flat
	surfaceparm stone
	{
		map textures/mohcommon/archbrkset_1asml.tga
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

textures/mohcommon/archbrkset_1sml
{
	qer_keyword trim
	qer_keyword stone
	qer_keyword flat
	qer_keyword brick
	surfaceparm stone
	{
		map textures/mohcommon/archbrkset_1sml.tga
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

textures/mohcommon/barbwire
{
	qer_editorimage textures/mohcommon/barbwire_strand.tga
	qer_keyword metal
	qer_keyword masked
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	nomipmaps
	{
		map textures/mohcommon/barbwire_strand.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/irongate1
{
	qer_editorimage textures/mohcommon/irongate_1.tga
	qer_keyword door
	qer_keyword metal
	qer_keyword masked
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	{
		map textures/mohcommon/irongate_1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/irongate2
{
	qer_editorimage textures/mohcommon/irongate_2.tga
	qer_keyword metal
	qer_keyword masked
	qer_keyword door
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	cull none
	nopicmip
	{
		map textures/mohcommon/irongate_2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/beam2
{
	qer_keyword wood
	qer_keyword trim
	surfaceparm wood
	{
		map textures/mohcommon/beam2.tga
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

textures/mohcommon/bulletholes
{
	qer_editorimage textures/mohcommon/bulletset_1.tga
	qer_keyword masked
	qer_trans .4
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	sort banner
	cull none
	{
		map textures/mohcommon/bulletset_1.tga
		blendfunc blend
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/second_story_norman_env
{
	qer_editorimage textures/mohcommon/strctrset_1env.tga
	qer_keyword wall
	qer_keyword tudor
	qer_keyword window
	surfaceparm stone
	cull none
	{
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
	}
	{
		map textures/mohcommon/strctrset_1env.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

// Temporary fix for press event to stop the double fog problem
textures/mohcommon/second_story_norman_noenv
{
	qer_editorimage textures/mohcommon/strctrset_1env.tga
	qer_keyword wall
	qer_keyword tudor
	qer_keyword window
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1env.tga
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

textures/mohcommon/cncr04s
{
	qer_keyword concrete
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/cncr04s.tga
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

textures/mohcommon/cncrtset_4
{
	qer_keyword floor
	qer_keyword flat
	qer_keyword concrete
	surfaceparm stone
	{
		map textures/mohcommon/cncrtset_4.tga
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

textures/mohcommon/crackedconcrete
{
	qer_keyword floor
	qer_keyword flat
	qer_keyword concrete
	surfaceparm stone
	{
		map textures/mohcommon/crackedconcrete.tga
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

textures/mohcommon/doorold_1a
{
	qer_keyword wood
	qer_keyword door
	surfaceparm wood
	{
		map textures/mohcommon/doorold_1a.tga
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

textures/mohcommon/doorold_1waffen
{
	qer_keyword signs
	qer_keyword wood
	qer_keyword door
	surfaceparm wood
	{
		map textures/mohcommon/doorold_1waffen.tga
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

textures/mohcommon/gravelrd
{
	qer_keyword floor
	qer_keyword natural
	qer_keyword dirt
	qer_keyword flat
	surfaceparm dirt
	{
		map textures/mohcommon/gravelrd.tga
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

textures/mohcommon/hexbrkflr_set1
{
	qer_keyword stone
	qer_keyword floor
	qer_keyword flat
	surfaceparm stone
	{
		map textures/mohcommon/hexbrkflr_set1.tga
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

textures/mohcommon/ibeam_1
{
	qer_keyword rusted
	qer_keyword metal
	qer_keyword trim
	surfaceparm metal
	{
		map textures/mohcommon/ibeam_1.tga
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

textures/mohcommon/ibeam_1a
{
	qer_keyword trim
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	{
		map textures/mohcommon/ibeam_1a.tga
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

textures/mohcommon/ibeamcnctr
{
	qer_keyword trim
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	{
		map textures/mohcommon/ibeamcnctr.tga
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

textures/mohcommon/jeff-concrete-walla
{
	qer_keyword concrete
	qer_keyword wall
	qer_keyword flat
	surfaceparm stone
	{
		map textures/mohcommon/jeff-concrete-walla.tga
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

textures/mohcommon/jh_brokenwindow1
{
	qer_editorimage textures/mohcommon/jh_brkwindow_1.tga
	qer_keyword glass
	qer_keyword damaged
	qer_keyword window
	surfaceparm glass
	surfaceparm trans
	cull none
	{
		map textures/mohcommon/jh_brkwindow_1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		alphaFunc GT0
		depthWrite
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
	{
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
		alphagen oneMinusDot .3 .7
		blendfunc blend
		depthFunc equal
	}
}

textures/mohcommon/jh_maskwin1
{
	qer_keyword brick
	qer_keyword masked
	qer_keyword window
	surfaceparm stone
	surfaceparm trans
	cull none
	nopicmip
	{
		map textures/mohcommon/jh_maskwin1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/jh_window1
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/mohcommon/jh_window1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/jh_window2
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/mohcommon/jh_window2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/jh_window3
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/mohcommon/jh_window3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/jh_woodwall_2brn
{
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/mohcommon/jh_woodwall_2brn.tga
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

textures/mohcommon/jh_woodwall_2shd
{
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/mohcommon/jh_woodwall_2shd.tga
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

textures/mohcommon/bordered_window_border
{
	qer_editorimage textures/mohcommon/window32.tga
	qer_keyword stone
	qer_keyword masked
	qer_keyword window
	surfaceparm stone
	sort banner
	cull none
	{
		map textures/mohcommon/window32.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/bordered_window_glass
{
	qer_editorimage textures/mohcommon/window32.tga
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass
	cull none
	{
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
	}
	{
		map textures/mohcommon/window32.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/peacockgrill
{
	qer_editorimage textures/mohcommon/m_wndwiron1.tga
	qer_keyword rusted
	qer_keyword metal
	qer_keyword masked
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm metal
	PolygonOffset
	cull none
	{
		map textures/mohcommon/m_wndwiron1.tga
		depthwrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/stonewindow_border
{
	qer_editorimage textures/mohcommon/winbrdr1.tga
	qer_keyword stone
	qer_keyword masked
	qer_keyword window
	surfaceparm stone
	surfaceparm trans
	surfaceparm alphashadow
	cull none
	{
		map textures/mohcommon/winbrdr1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/panedwindow1
{
	qer_editorimage textures/mohcommon/window32_p1.tga
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass
	cull none
//	sort banner
	{
//		map textures/models/weapons/m1garand/reflection1.tga
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
	}
	{
		map textures/mohcommon/window32_p1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/mohcommon/panedwindow1_oneside
{
	qer_editorimage textures/mohcommon/window32_p1.tga
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass

//	sort banner
	{
//		map textures/models/weapons/m1garand/reflection1.tga
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
	}
	{
		map textures/mohcommon/window32_p1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}
textures/mohcommon/normandyset_3
{
	qer_keyword tudor
	qer_keyword wall
	surfaceparm wood
	{
		map textures/mohcommon/normandyset_3.tga
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

textures/mohcommon/nrmndyset_2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/nrmndyset_2.tga
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

textures/mohcommon/nromndyset_3a
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/nromndyset_3a.tga
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

textures/mohcommon/nromndyset_3b
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/nromndyset_3b.tga
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

textures/mohcommon/nromndyset_3c
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/nromndyset_3c.tga
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

textures/mohcommon/nromndyset_3d
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/nromndyset_3d.tga
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

textures/mohcommon/dirty_nromndyset_3b
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/dirty_nromndyset_3b.tga
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

textures/mohcommon/dirty_nromndyset_3c
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/dirty_nromndyset_3c.tga
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

textures/mohcommon/rckfcset1_1
{
	qer_keyword natural
	qer_keyword rock
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/rckfcset1_1.tga
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

textures/mohcommon/secfence
{
	qer_editorimage textures/mohcommon/secfence1.tga
	qer_keyword metal
	qer_keyword wall
	qer_keyword masked
	surfaceparm fence
	surfaceparm metal
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	cull none
	nopicmip
	{
		map textures/mohcommon/secfence1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/secfence2
{
	qer_keyword metal
	qer_keyword masked
	qer_keyword wall
	surfaceparm metal
	surfaceparm fence
    qer_editorimage textures/mohcommon/secfence2.tga 
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	cull none
	nopicmip
	{
		map textures/mohcommon/secfence2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/stonebaset_1
{
	qer_keyword natural
	qer_keyword rock
	qer_keyword trim
	surfaceparm stone
	{
		map textures/mohcommon/stonebaset_1.tga
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

textures/mohcommon/strctrset_1base
{
	qer_keyword flat
	qer_keyword concrete
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1base.tga
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

textures/mohcommon/strctrset_1flt
{
	qer_keyword flat
	qer_keyword concrete
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1flt.tga
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

textures/mohcommon/strctrset_1fltdirt
{
	qer_keyword flat
	qer_keyword concrete
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1fltdirt.tga
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

textures/mohcommon/strctrset_1d
{
	qer_keyword window
	qer_keyword tudor
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1d.tga
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

textures/mohcommon/dirty_strctrset_1g
{
	qer_keyword window
	qer_keyword tudor
	qer_keyword wall
	surfaceparm stone
	{
		map textures/mohcommon/dirty_strctrset_1g.tga
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

textures/mohcommon/strctrset_1e
{
	qer_keyword window
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1e.tga
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

textures/mohcommon/dirty_strctrset_1e
{
	qer_keyword window
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/mohcommon/strctrset_1e.tga
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

textures/mohcommon/stset_1a
{
	qer_keyword stone
	qer_keyword road
	qer_keyword floor
	surfaceparm stone
	{
		map textures/mohcommon/stset_1a.tga
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

textures/mohcommon/stset_1base
{
	qer_keyword flat
	qer_keyword stone
	qer_keyword road
	qer_keyword floor
	surfaceparm stone
	{
		map textures/mohcommon/stset_1base.tga
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

textures/mohcommon/stset_1flatsml
{
	qer_keyword stone
	qer_keyword road
	qer_keyword floor
	qer_keyword flat
	surfaceparm stone
	{
		map textures/mohcommon/stset_1flatsml.tga
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

textures/mohcommon/tile_set1
{
	qer_keyword stone
	qer_keyword floor
	qer_keyword flat
	surfaceparm stone
	{
		map textures/mohcommon/tile_set1.tga
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

textures/mohcommon/tile_set1adec
{
	qer_keyword stone
	qer_keyword floor
	surfaceparm stone
	{
		map textures/mohcommon/tile_set1adec.tga
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

textures/mohcommon/tile_set1bdec
{
	qer_keyword stone
	qer_keyword floor
	surfaceparm stone
	{
		map textures/mohcommon/tile_set1bdec.tga
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

textures/mohcommon/trim_set1a
{
	qer_keyword concrete
	qer_keyword trim
	surfaceparm stone
	{
		map textures/mohcommon/trim_set1a.tga
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

textures/mohcommon/trim_set1flt
{
	qer_keyword trim
	qer_keyword concrete
	surfaceparm stone
	{
		map textures/mohcommon/trim_set1flt.tga
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

textures/mohcommon/wetmud
{
	qer_keyword natural
	qer_keyword Mud
	qer_keyword floor
	surfaceparm mud
	{
		map textures/mohcommon/wetmud.tga
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

textures/mohcommon/window5
{
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/mohcommon/window5.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/window5masked
{
	qer_editorimage textures/mohcommon/window5msk.tga
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm trans
	surfaceparm glass
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/mohcommon/window5msk.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/wndwset1env
{
	qer_editorimage textures/mohcommon/wndwset_1env.tga
	qer_keyword wood
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/mohcommon/wndwset_1env.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/wndwset_1masked
{
	qer_editorimage textures/mohcommon/wndwset_1msk.tga
	qer_keyword wood
	qer_keyword glass
	qer_keyword masked
	qer_keyword window
	surfaceparm glass
	surfaceparm trans
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/mohcommon/wndwset_1msk.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/woodpost_1
{
	qer_keyword trim
	qer_keyword wood
	surfaceparm wood
	{
		map textures/mohcommon/woodpost_1.tga
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

textures/mohcommon/woodpost_1drk
{
	qer_keyword wood
	qer_keyword trim
	surfaceparm wood
	{
		map textures/mohcommon/woodpost_1drk.tga
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

textures/mohcommon/woodtrm_set2
{
	qer_keyword wood
	qer_keyword trim
	surfaceparm wood
	{
		map textures/mohcommon/woodtrm_set2.tga
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

textures/mohcommon/woodtrm_set2drk
{
	qer_keyword wood
	qer_keyword trim
	surfaceparm wood
	{
		map textures/mohcommon/woodtrm_set2drk.tga
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

textures/mohcommon/jh_window2pj
{
	qer_keyword window
	qer_keyword wood
	surfaceparm glass
	surfaceparm trans
	surfaceparm alphashadow
	{
		map textures/mohcommon/jh_window2pj.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/jh_window3pj
{
	qer_keyword window
	qer_keyword wood
	surfaceparm glass
	surfaceparm trans
	surfaceparm alphashadow
	{
		map textures/mohcommon/jh_window3pj.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/mohcommon/jeff-concrete-wallb
{
	qer_keyword ceiling
	qer_keyword concrete
	surfaceparm stone
	{
		map textures/mohcommon/jeff-concrete-wallb.tga
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