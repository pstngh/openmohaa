textures/central_europe_winter/winterwindow1_lit
{
	qer_editorimage textures/central_europe_winter/winterwindow_lit1.tga
	qer_keyword m5
	qer_keyword special
	//surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit1.blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/central_europe_winter/winterwindow_interior1
{
	qer_editorimage textures/central_europe_winter/winterwindow_interior1.tga
	qer_keyword m5
	qer_keyword special
	//surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 1000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_interior1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winter_interior1.blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/central_europe_winter/winterwindow2_lit
{
	qer_editorimage textures/central_europe_winter/winterwindow_lit2.tga
	qer_keyword m5
	qer_keyword special
	//surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit2.blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/central_europe_winter/verticalbracesnow
{
	qer_editorimage textures/central_europe_winter/crossbrace_vert1w.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword rusted
	qer_keyword metal
	qer_keyword snow
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
		map textures/central_europe_winter/crossbrace_vert1w.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/central_europe_winter/grille_general1winter
{
	qer_editorimage textures/central_europe_winter/grille_general1winter.tga
	qer_keyword metal
	qer_keyword snow
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
		map textures/central_europe_winter/grille_general1winter.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/strangebracesnow
{
	qer_editorimage textures/central_europe_winter/strngbrace_set1bw.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword rusted
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
		map textures/central_europe_winter/strngbrace_set1bw.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/crossbracing1
{
	qer_editorimage textures/central_europe_winter/crossbracing1_set1bw.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword masked
	qer_keyword snow
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	{
		map textures/central_europe_winter/crossbracing1_set1bw.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/crossbracing2
{
	qer_editorimage textures/central_europe_winter/crossbracing1_set1w.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword rusted
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
		map textures/central_europe_winter/crossbracing1_set1w.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/crossbracing3
{
	qer_editorimage textures/central_europe_winter/crossbracing1_setw.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword metal
	qer_keyword rusted
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
		map textures/central_europe_winter/crossbracing1_setw.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/snowcoveredtrees
{
	qer_editorimage textures/central_europe_winter/snowtrees1.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword natural
	qer_keyword snow
	qer_keyword tree
	qer_keyword masked
	surfaceparm snow
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	{
		map textures/central_europe_winter/snowtrees1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/snowcoveredtrees_noholes
{
	qer_editorimage textures/central_europe_winter/snowtrees1_noholes.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword natural
	qer_keyword snow
	qer_keyword tree
	qer_keyword masked
	surfaceparm snow
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	{
		map textures/central_europe_winter/snowtrees1_noholes.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/railingwinter
{
	qer_editorimage textures/central_europe_winter/railing_set2wintr.tga
	qer_keyword m6
	qer_keyword m2
	qer_keyword rusted
	qer_keyword metal
	qer_keyword snow
	qer_keyword masked
	surfaceparm metal
//	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	cull none
	nopicmip
	{
		map textures/central_europe_winter/railing_set2wintr.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/evesnow
{
	qer_keyword m7
	qer_keyword m5
	qer_keyword floor
	qer_keyword natural
	qer_keyword snow
	surfaceparm snow
	{
		map textures/central_europe_winter/evesnow.tga
		depthwrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc GL_DST_COLOR GL_ZERO
		depthfunc equal
	}
}

textures/central_europe_winter/jhpj_winterwall1
{
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword stone
	qer_keyword brick
	qer_keyword window
	qer_keyword wall
	surfaceparm stone
	{
		map textures/central_europe_winter/jhpj_winterwall1.tga
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

textures/central_europe_winter/jhpj_winterwall1sprk
{
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword window
	qer_keyword wall
	qer_keyword stone
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe_winter/snowfx3.tga
		tcMod parallax .002 .002
		tcMod scale 15 15
	nextbundle
		map textures/central_europe_winter/snowfx2.tga
		tcMod scale 2 2
	}
	{
		map $whiteimage
		rgbGen constant .2 .2 .2
		blendFunc add
	}
	{
		// This stage lightens up the non-sparkly bits
		map textures/central_europe_winter/jhpj_winterwall1sprk.tga
//		rgbGen const .0625 .0954 .1616
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
//		tcMod scale .4 .5
	nextbundle
		map $lightmap
		rgbGen Identity
	}
}

textures/central_europe_winter/rusticshinglesnow
{
	qer_keyword m7
	qer_keyword m5
	qer_keyword wood
	qer_keyword roof
	qer_keyword snow
	surfaceparm wood
	{
		map textures/central_europe_winter/rusticshinglesnow.tga
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

textures/central_europe_winter/snowfield1
{
	qer_editorimage textures/central_europe_winter/snowfield.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword floor
	qer_keyword natural
	qer_keyword snow
	qer_keyword flat
	surfaceparm snow
	nopicmip	//How the hell does this work?
	{
//		map textures/central_europe_winter/snowfx2.tga
//		tcMod parallax .002 .002
//		tcMod scale 4.5 4.5
		map textures/central_europe_winter/snowfx3.tga
		tcMod parallax .002 .002
		tcMod scale 15 15
	nextbundle
		map textures/central_europe_winter/snowfx2.tga
		tcMod scale 2 2
	}
	{
		// This stage lightens up the non-sparkly bits
		map textures/central_europe_winter/snowfield.tga
//		rgbGen const .0625 .0954 .1616
		blendfunc GL_ONE GL_ONE
		tcMod scale .5 .5
	nextbundle
		map $lightmap
		rgbGen Identity
	}
}

textures/central_europe_winter/stonewall1snow
{
	qer_keyword natural
	qer_keyword stone
	qer_keyword wall
	qer_keyword snow
	qer_keyword m7
	qer_keyword m5
	surfaceparm stone
	{
		map textures/central_europe_winter/stonewall1snow.tga
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

textures/central_europe_winter/strtset_1
{
	qer_keyword road
	qer_keyword floor
	qer_keyword stone
	qer_keyword snow
	qer_keyword m7
	qer_keyword m5
	surfaceparm snow
	{
		map textures/central_europe_winter/strtset_1.tga
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

textures/central_europe_winter/wallsnow2
{
	qer_keyword wall
	qer_keyword snow
	qer_keyword m7
	qer_keyword m5
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe_winter/wallsnow2.tga
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

textures/central_europe_winter/snowcoveredwires
{
	qer_editorimage textures/central_europe_winter/wires_snow.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword snow
	qer_keyword pipe
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
		map textures/central_europe_winter/wires_snow.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}
textures/central_europe_winter/wallsnowsparkle3
{
	qer_editorimage textures/central_europe_winter/wallsnow3.tga
	qer_keyword wall
	qer_keyword snow
	qer_keyword m7
	qer_keyword m5
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe_winter/wallsnow3.tga
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

textures/central_europe_winter/naturalrck1_snow
{
	qer_keyword wall
	qer_keyword snow
	surfaceparm snow
	nopicmip
	{
		map textures/central_europe_winter/snowfx3.tga
		tcMod parallax .002 .002
		tcMod scale 15 15
	nextbundle
		map textures/central_europe_winter/snowfx2.tga
		tcMod scale 2 2
	}
	{
		map $whiteimage
		rgbGen constant .2 .2 .2
		blendFunc add
	}
	{
		// This stage lightens up the non-sparkly bits
		map textures/central_europe_winter/naturalrck1_snow.tga
//		rgbGen const .0625 .0954 .1616
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
//		tcMod scale .4 .5
	nextbundle
		map $lightmap
		rgbGen Identity
	}
}
textures/central_europe_winter/cliffset_1snow
{
	qer_keyword snow
	qer_keyword snow
	qer_keyword snow
	surfaceparm snow
	qer_editorimage textures/central_europe_winter/cliffset_1snow.tga

	{
		map textures/central_europe_winter/cliffset_1snow.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/central_europe_winter/cliffset_1snow_noshd
{
	qer_keyword snow
	qer_keyword snow
	qer_keyword snow
	surfaceparm snow
	surfaceparm trans
	qer_editorimage textures/central_europe_winter/cliffset_1snow.tga

	{
		map textures/central_europe_winter/cliffset_1snow.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/central_europe_winter/snowcap
{
	qer_keyword snow
	surfaceparm snow

	{
		map textures/central_europe_winter/snowcap.tga
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
textures/central_europe_winter/snowcap_trans
{
	qer_keyword snow
	surfaceparm snow
	surfaceparm trans
	qer_editorimage textures/central_europe_winter/snowcap.tga
	{
		map textures/central_europe_winter/snowcap.tga
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



textures/central_europe_winter/snow_treelin2lg
{
	qer_keyword snow
	qer_keyword terrain
	qer_keyword natural
	qer_keyword m5
    qer_editorimage textures/central_europe_winter/snow_treelin2lg.tga
	surfaceparm snow
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
//	surfaceparm alphashadow
	{
		map textures/central_europe_winter/snow_treelin2lg.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/snow_treelin2sml
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword terrain
	qer_keyword natural
    qer_editorimage textures/central_europe_winter/snow_treelin2sml.tga
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm snow
//	surfaceparm alphashadow
	{
		map textures/central_europe_winter/snow_treelin2sml.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/snow_treeline1lg
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword terrain
	qer_keyword natural
    qer_editorimage textures/central_europe_winter/snow_treeline1lg.tga
	surfaceparm snow
    surfaceparm fence
{
		map textures/central_europe_winter/snow_treeline1lg.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/snow_treeline1sml
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword terrain
	qer_keyword natural
    qer_editorimage  textures/central_europe_winter/snow_treeline1sml.tga 
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm snow
//	surfaceparm alphashadow
	{
		map textures/central_europe_winter/snow_treeline1sml.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/cliffset_2snow
{
	qer_editorimage textures/central_europe_winter/cliffset_2snow_ed.tga
	qer_keyword trim
	qer_keyword natural
	qer_keyword rock
	qer_keyword snow
	qer_keyword m5
	surfaceparm snow
	{
		map textures/central_europe_winter/cliffset_2snow.tga
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

textures/central_europe_winter/snow_bumpy_1
{
	qer_keyword terrain
	qer_keyword m5
	qer_keyword snow
	surfaceparm snow
	{
		map textures/central_europe_winter/snow_bumpy_1.tga
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

textures/central_europe_winter/snow_bumpy_brd1
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/snow_bumpy_brd1.tga
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

textures/central_europe_winter/snow_treeline1lg_nomask
{
	qer_keyword m5
	qer_keyword tree
	qer_keyword snow
	surfaceparm snow
	{
		map textures/central_europe_winter/snow_treeline1lg_nomask.tga
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

textures/central_europe_winter/door1simpl_winter
{
	qer_keyword door
	qer_keyword m5
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/door1simpl_winter.tga
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

textures/central_europe_winter/doorold_1aWntrwoknkr
{
	qer_keyword door
	qer_keyword m5
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/doorold_1aWntrwoknkr.tga
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

textures/central_europe_winter/cityrbblbs_wintertrmpl1
{
	qer_keyword floor
	qer_keyword snow
	qer_keyword m5
	{
		map textures/central_europe_winter/cityrbblbs_wintertrmpl1.tga
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

textures/central_europe_winter/cityrbblbs_winterbase
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword floor
	{
		map textures/central_europe_winter/cityrbblbs_winterbase.tga
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

textures/central_europe_winter/nrmndywlset_2aw
{
	qer_keyword wall
	qer_keyword tudor
	qer_keyword m5
	surfaceparm wood
	{
		map textures/central_europe_winter/nrmndywlset_2aw.tga
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

textures/central_europe_winter/nrmndywlset_2bw
{
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/nrmndywlset_2bw.tga
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

textures/central_europe_winter/nrmndywlset_2cw
{
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/nrmndywlset_2cw.tga
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

textures/central_europe_winter/tudor_set1w_exwall1win
{
	qer_keyword window
	qer_keyword tudor
	qer_keyword snow
	qer_keyword m5
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set1w_exwall1win.tga
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

textures/central_europe_winter/tudor_set1w_exwall1b
{
	qer_keyword wall
	qer_keyword snow
	qer_keyword m5
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set1w_exwall1b.tga
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

textures/central_europe_winter/tudor_set1w_exwall1a
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set1w_exwall1a.tga
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

textures/central_europe_winter/winter_road
{
	qer_editorimage textures/central_europe_winter/wintroad_set1.tga
	qer_keyword wall
	qer_keyword snow
	surfaceparm snow
	nopicmip
	{
		map textures/central_europe_winter/wintroad_set1.tga
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

textures/central_europe_winter/stonebricks1drk_wntr
{
	qer_keyword m5
	qer_keyword wall
	qer_keyword rock
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe_winter/stonebricks1drk_wntr.tga
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

textures/central_europe_winter/fort_floor_cobbleflttilew
{
	qer_keyword m5
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe_winter/fort_floor_cobbleflttilew.tga
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

textures/central_europe_winter/winterroof_set1aw
{
	qer_keyword snow
	qer_keyword roof
	qer_keyword m5
	surfaceparm snow
	{
	   	map textures/central_europe_winter/winterroof_set1aw.tga
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

textures/central_europe_winter/winterroof_set1w
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword roof
	surfaceparm snow
	{
		map textures/central_europe_winter/winterroof_set1w.tga
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

textures/central_europe_winter/groovedstn1aw
{
	qer_keyword trim
	qer_keyword snow
	qer_keyword m5
	surfaceparm snow
	{
		map textures/central_europe_winter/groovedstn1aw.tga
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

textures/central_europe_winter/trim_set1fltw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword trim
	surfaceparm snow
	{
		map textures/central_europe_winter/trim_set1fltw.tga
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

textures/central_europe_winter/tudor_set1_exwall1flatw
{
	qer_keyword wall
	qer_keyword m5
	qer_keyword tudor
	qer_keyword snow
	surfaceparm stone
	{
		map textures/central_europe_winter/tudor_set1_exwall1flatw.tga
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

textures/central_europe_winter/transjoist_w
{
	qer_keyword trim
	qer_keyword snow
	qer_keyword m5
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/transjoist_w.tga
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

textures/central_europe_winter/joist_w
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword tudor
	qer_keyword trim
	surfaceparm wood
	{
		map textures/central_europe_winter/joist_w.tga
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

textures/central_europe_winter/tudor_set2bw
{
	qer_keyword wall
	qer_keyword snow
	qer_keyword m5
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/central_europe_winter/tudor_set2bw.tga
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

textures/central_europe_winter/tudor_set1w_exwall1flatw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/central_europe_winter/tudor_set1w_exwall1flatw.tga
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

textures/central_europe_winter/tudor_set2aw
{
	qer_keyword m5
	qer_keyword snow
	qer_keyword tudor
	qer_keyword wall
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set2aw.tga
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

textures/central_europe_winter/tudor_set2cw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set2cw.tga
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

textures/central_europe_winter/tudor_set2cw
{
	qer_keyword window
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set2cw.tga
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

textures/central_europe_winter/tudor_set2dw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword window
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set2dw.tga
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

textures/central_europe_winter/tudor_set2dw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe_winter/tudor_set2dw.tga
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

textures/central_europe_winter/shutter_set2w
{
	qer_keyword m5
	qer_keyword window
	qer_keyword wood
	qer_keyword snow
	surfaceparm wood
	{
		map textures/central_europe_winter/shutter_set2w.tga
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

textures/central_europe_winter/shutter_w
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wood
	qer_keyword window
	surfaceparm wood
	{
		map textures/central_europe_winter/shutter_w.tga
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

textures/central_europe_winter/normndybrik1w
{
	qer_keyword m5
	qer_keyword snow
	qer_keyword wall
	qer_keyword flat
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe_winter/normndybrik1w.tga
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

textures/central_europe_winter/strctrset_2aw
{
	qer_keyword m5
	qer_keyword wall
	qer_keyword snow
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/central_europe_winter/strctrset_2aw.tga
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

textures/central_europe_winter/strctrset_2cw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/central_europe_winter/strctrset_2cw.tga
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

textures/central_europe_winter/strctrset_2dw
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword wall
	qer_keyword tudor
	surfaceparm stone
	{
		map textures/central_europe_winter/strctrset_2dw.tga
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

textures/central_europe_winter/stset_1awinter
{
	qer_keyword m5
	qer_keyword snow
	qer_keyword road
	surfaceparm snow
	{
		map textures/central_europe_winter/stset_1awinter.tga
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

textures/central_europe_winter/secfence1_wntr
{
	qer_editorimage textures/central_europe_winter/secfence1_wntr.tga
	qer_keyword m5
	qer_keyword wall
	qer_keyword masked
	qer_keyword metal
	surfaceparm grill
//	surfaceparm fence
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	nomipmaps
	nopicmip
	cull none
	{
		map textures/central_europe_winter/secfence1_wntr.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/stset_1awinter_fill
{
	qer_editorimage textures/central_europe_winter/strtset1_winter_fill.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword road
	surfaceparm snow
	{
		map textures/central_europe_winter/strtset1_winter_fill.tga
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

textures/central_europe_winter/stset_1awinter_fill_noshadow
{
	qer_editorimage textures/central_europe_winter/strtset1_winter_fill.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword road
	surfaceparm snow
	surfaceparm trans
	qer_editorimage textures/central_europe_winter/strtset1_winter_fill.tga
	{
		map textures/central_europe_winter/strtset1_winter_fill.tga
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
textures/central_europe_winter/floorsnowcovered
{
	qer_editorimage textures/central_europe_winter/flrwood2w.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword floor
	surfaceparm snow
	{
	    map textures/central_europe_winter/flrwood2w.tga
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

textures/central_europe_winter/corrugated_snowcovered
{
	qer_editorimage textures/central_europe_winter/jh_corrugate4aw.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword roof
	surfaceparm snow
	{
		map textures/central_europe_winter/jh_corrugate4aw.tga
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

textures/central_europe_winter/wintergate
{
	qer_editorimage textures/central_europe_winter/wintergate1.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword wall
	surfaceparm snow
	{
		map textures/central_europe_winter/wintergate1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}




//	{
//		map textures/central_europe_winter/snowfx3.tga
//		tcMod parallax .002 .002
//		tcMod scale 15 15
//	nextbundle
//		map textures/central_europe_winter/snowfx2.tga
//		tcMod scale 2 2
//	}
//	{
//		map $whiteimage
//		rgbGen constant .2 .2 .2
//		blendFunc add
//	}
//	{
//		// This stage lightens up the non-sparkly bits
//		map textures/central_europe_winter/wintergate1.tga
//		rgbGen const .0625 .0954 .1616
//		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
//		tcMod scale .4 .5
//	nextbundle
//		map $lightmap
//		rgbGen Identity
//	}
}

textures/central_europe_winter/step_rise
{
	qer_editorimage textures/central_europe_winter/rise_snow.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword wall
	surfaceparm snow
	{
		map textures/central_europe_winter/rise_snow.tga
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

textures/central_europe_winter/step_run
{
	qer_editorimage textures/central_europe_winter/run_snow.tga
	qer_keyword m5
	qer_keyword snow
	qer_keyword wall
	surfaceparm snow
	{
			map textures/central_europe_winter/run_snow.tga
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

textures/central_europe_winter/snowcoveredtrees_noholes_nolight
{
	qer_editorimage textures/central_europe_winter/snowtrees1_noholes.tga
	qer_keyword m7
	qer_keyword m5
	qer_keyword natural
	qer_keyword snow
	qer_keyword tree
	qer_keyword masked
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	surfaceparm wood
	cull none
	nopicmip
	{
		map textures/central_europe_winter/snowtrees1_noholes.tga
		alphaFunc GE128
	}
}

// The following are light sources that should be used ONLY in winter or low temp environments.
textures/central_europe_winter
{
	qer_editorimage textures/central_europe_winter/winterwindow_lit1.tga
	//surfaceparm nomarks
	surfaceparm glass
	q3map_surfacelight 2000

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/central_europe_winter/winterwindow_lit.blend.tga
		blendfunc GL_ONE GL_ONE
	}
}

textures/central_europe_winter/snowcoveredtreesnor1
{
	qer_keyword m7
	qer_keyword m5
	qer_keyword natural
	qer_keyword snow
	qer_keyword tree
	qer_keyword masked
    qer_editorimage textures/central_europe_winter/snowcoveredtreesnor1.tga
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nonsolid
	surfaceparm wood
	cull none
	nopicmip
	{
		map textures/central_europe_winter/snowcoveredtreesnor1.tga
		alphaFunc GE128
	}
}

textures/central_europe_winter/railside_wntr
{
	qer_keyword m5
	qer_keyword panel
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/railside_wntr.tga
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

textures/central_europe_winter/railside2_wntr
{
	qer_keyword snow
	qer_keyword m5
	qer_keyword panel
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/railside2_wntr.tga
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

textures/central_europe_winter/raildoor_wntr
{
	qer_keyword door
	qer_keyword snow
	qer_keyword m5
	qer_keyword panel
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/raildoor_wntr.tga
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

textures/central_europe_winter/railroof_wntr
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword roof
	qer_keyword snow
	qer_keyword m5
	qer_keyword panel
	surfaceparm snow
	{
		map textures/central_europe_winter/railroof_wntr.tga
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

textures/central_europe_winter/icecicles
{
	qer_keyword special
	qer_keyword m5
	surfaceparm snow
    surfaceparm fence
    qer_editorimage textures/central_europe_winter/icecicles.tga
	PolygonOffset
	{
		map textures/central_europe_winter/icecicles.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/forstsnow_med512
{
	qer_keyword terrain
	qer_keyword snow
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_med512.tga
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

textures/central_europe_winter/forstsnow_med256
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_med256.tga
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

textures/central_europe_winter/forstsnow_lite512
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_lite512.tga
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

textures/central_europe_winter/forstsnow_lite256
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_lite256.tga
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

textures/central_europe_winter/forstsnow_rough512
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_rough512.tga
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

textures/central_europe_winter/forstsnow_rough256
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_rough256.tga
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

textures/central_europe_winter/forstsnow_mdlttrns512
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_mdlttrns512.tga
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

textures/central_europe_winter/forstsnow_mdlttrns256
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_mdlttrns256.tga
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

textures/central_europe_winter/forstsnow_mdrghtrans512
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_mdrghtrans512.tga
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

textures/central_europe_winter/forstsnow_mdrghtrans256
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_mdrghtrans256.tga
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

textures/central_europe_winter/forstsnow_rock512
{
	qer_keyword rock
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_rock512.tga
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

textures/central_europe_winter/forstsnow_rock256
{
	qer_keyword snow
	qer_keyword terrain
	qer_keyword rock
	surfaceparm snow
	{
		map textures/central_europe_winter/forstsnow_rock256.tga
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

textures/central_europe_winter/netting_winter
{
	qer_keyword special
	qer_keyword masked
	qer_keyword snow
    qer_editorimage textures/central_europe_winter/netting_winter.tga
	surfaceparm snow
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/central_europe_winter/netting_winter.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe_winter/winterbunkerwall
{
	qer_editorimage textures/central_europe_winter/bunker_wall1winter.tga
	qer_keyword snow
	qer_keyword wall
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe_winter/bunker_wall1winter.tga
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

textures/central_europe_winter/ibeam_horizwnter
{
	qer_keyword snow
	qer_keyword metal
	surfaceparm metal
	{
		map textures/central_europe_winter/ibeam_horizwnter.tga
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

textures/central_europe_winter/ibeam_vertwnter
{
	qer_keyword snow
	qer_keyword metal
	surfaceparm metal
	{
		map textures/central_europe_winter/ibeam_vertwnter.tga
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

textures/central_europe_winter/celeste_tavrnsinWntr
{
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/celeste_tavrnsinWntr.tga
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

textures/central_europe_winter/tavernsign_b1Wntr
{
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/tavernsign_b1Wntr.tga
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

textures/central_europe_winter/doorold_1aWntr
{
	qer_keyword door
    qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/doorold_1aWntr.tga
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

textures/central_europe_winter/tavernsign_grmn1wntr
{
	qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/tavernsign_grmn1wntr.tga
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

textures/central_europe_winter/frozenriver
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/frozenriver.tga
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

textures/central_europe_winter/floorwd2Wntr
{
	qer_keyword floor
    qer_keyword snow
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe_winter/floorwd2Wntr.tga
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

textures/central_europe_winter/rembridge_ibeamAW
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword snow
	qer_keyword panel
	surfaceparm metal
	{
		map textures/central_europe_winter/rembridge_ibeamAW.tga
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

textures/central_europe_winter/rembridgebeam1W
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword snow
	qer_keyword panel
	surfaceparm metal
	{
		map textures/central_europe_winter/rembridgebeam1W.tga
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

textures/central_europe_winter/JeffWallW
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword snow
	qer_keyword panel
	surfaceparm metal
	{
		map textures/central_europe_winter/JeffWallW.tga
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

textures/central_europe_winter/custom_supprt1
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword snow
	qer_keyword panel
	surfaceparm metal
	{
		map textures/central_europe_winter/custom_supprt1.tga
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

textures/central_europe_winter/ties_decalW
{
	qer_keyword snow
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	surfaceparm monsterclip
	surfaceparm playerclip
    qer_editorimage textures/central_europe_winter/ties_decalW.tga
	surfaceparm fence
	nopicmip
	{
		map textures/central_europe_winter/ties_decalW.tga
                depthwrite
		blendfunc blend
		//alphafunc ge128
		nextbundle
		map $lightmap
	}
} 

textures/central_europe_winter/snowcliff_rev512
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/snowcliff_rev512.tga
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

textures/central_europe_winter/rockwall_s1wintr
{
	qer_keyword snow
	qer_keyword terrain
	surfaceparm snow
	{
		map textures/central_europe_winter/rockwall_s1wintr.tga
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

textures/central_europe_winter/jh_corrdoor2bWntr
{
        qer_keyword door
	qer_keyword snow
	qer_keyword metal
	surfaceparm metal
	{
		map textures/central_europe_winter/jh_corrdoor2bWntr.tga
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
 
