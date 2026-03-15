textures/central_europe/brick1trim
{
	qer_keyword brick
	qer_keyword stone
	qer_keyword wall
	surfaceparm stone
	{
		map textures/central_europe/brick1trim.tga
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

textures/central_europe/brick1trimfncy
{
	qer_keyword brick
	qer_keyword wall
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe/brick1trimfncy.tga
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

textures/central_europe/brick1trimfncylt
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brick1trimfncylt.tga
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

textures/central_europe/brick1trimfncyshdw
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brick1trimfncyshdw.tga
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

textures/central_europe/brick1trimshadowed
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brick1trimshadowed.tga
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

textures/central_europe/brick1trimwht
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brick1trimwht.tga
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

textures/central_europe/redshingle
{
	qer_keyword wood
	qer_keyword roof
	surfaceparm wood
	{
		map textures/central_europe/redshingle.tga
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

textures/central_europe/rusticshingle
{
	qer_keyword wood
	qer_keyword roof
	surfaceparm wood
	{
		map textures/central_europe/rusticshingle.tga
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

textures/central_europe/windowtownhall1
{
	qer_keyword stone
	qer_keyword brick
	qer_keyword window
	surfaceparm stone
	{
		map textures/central_europe/windowtownhall1.tga
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

textures/central_europe/railroadbrdr_rail1
{
	qer_keyword special
	qer_keyword terrain
	qer_keyword m3
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm gravel
	{
		map textures/central_europe/railroadbrdr_rail1.tga
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

textures/central_europe/railroadbrdr_rail1_rb2c
{
	qer_keyword special
	qer_keyword terrain
	qer_keyword m3
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm gravel
	{
		map textures/central_europe/railroadbrdr_rail1_rb2c.tga
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
textures/central_europe/railroadbrdr_hill
{
	qer_keyword grass
	qer_keyword m3
	qer_keyword terrain
	qer_keyword special
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/central_europe/railroadbrdr_hill.tga
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

textures/central_europe/ties_decal
{
	qer_editorimage textures/central_europe/ties_decal.tga
	qer_keyword m3
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm alphashadow
	nopicmip
	{
		map textures/central_europe/ties_decal.tga
		blendfunc blend
		alphafunc ge128
	nextbundle
		map $lightmap
	}
}

textures/central_europe/jh_gtwindow1
{
	qer_keyword window
	qer_keyword glass
	surfaceparm trans
	nopicmip
	cull none
	{
		map textures/central_europe/jh_gtwindow1.tga
		blendfunc blend
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/central_europe/jh_gtwindow1
{
	qer_keyword window
	qer_keyword glass
	surfaceparm trans
	surfaceparm glass
	nopicmip
	cull none
	{
		map textures/central_europe/jh_gtwindow1.tga
		blendfunc blend
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/central_europe/earth_road_gen1bend
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword dirt
	qer_keyword gravel
	surfaceparm dirt
	{
		clampmap textures/central_europe/earth_road_gen1bend.tga
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

textures/central_europe/earth_road_gen1up
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm dirt
	{
		clampmap textures/central_europe/earth_road_gen1up.tga
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

textures/central_europe/earth_road_gen1dn
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm dirt
	{
		clampmap textures/central_europe/earth_road_gen1dn.tga
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

textures/central_europe/earth_road_gen1yup
{
	qer_keyword terrain
	qer_keyword road
	qer_keyword gravel
	qer_keyword dirt
	surfaceparm dirt
	{
		clampmap textures/central_europe/earth_road_gen1yup.tga
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

textures/central_europe/church_pillar_top
{
	qer_keyword special
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe/church_pillar_top.tga
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

textures/central_europe/church_interior_wall1
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe/church_interior_wall1.tga
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

textures/central_europe/church_interior_wall1drk
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe/church_interior_wall1drk.tga
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

textures/central_europe/church_interior_wall2flt
{
	qer_keyword flat
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe/church_interior_wall2flt.tga
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

textures/central_europe/church_interior_collumn1
{
	qer_keyword wall
	qer_keyword trim
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe/church_interior_collumn1.tga
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

textures/central_europe/rockwall_2bev
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe/rockwall_2bev.tga
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

textures/central_europe/rockwall_2flt
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm stone
	{
		map textures/central_europe/rockwall_2flt.tga
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

textures/central_europe/small_cobble
{
	qer_keyword rock
	qer_keyword flat
	qer_keyword floor
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe/small_cobble.tga
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

textures/central_europe/thatchroof_window
{
	qer_keyword roof
	qer_keyword window
	qer_keyword tudor
	qer_keyword grass
	surfaceparm grass
	{
		map textures/central_europe/thatchroof_window.tga
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

textures/central_europe/thatchroof_wmoss
{
	qer_keyword tudor
	qer_keyword roof
	qer_keyword grass
	surfaceparm grass
	{
		map textures/central_europe/thatchroof_wmoss.tga
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

textures/central_europe/tudor_set1_exwall1a
{
	qer_keyword wall
	qer_keyword wood
	qer_keyword plaster
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/tudor_set1_exwall1a.tga
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

textures/central_europe/tudor_set1_exwall1fltseem
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	qer_keyword plaster
	surfaceparm plaster
	{
		map textures/central_europe/tudor_set1_exwall1fltseem.tga
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

textures/central_europe/tudor_set1_exwall1flat
{
	qer_keyword flat
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	qer_keyword plaster
	surfaceparm plaster
	{
		map textures/central_europe/tudor_set1_exwall1flat.tga
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

textures/central_europe/tudor_set1_exwall1b
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/tudor_set1_exwall1b.tga
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

textures/central_europe/tudor_set1_exwall1win
{
	qer_keyword window
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/tudor_set1_exwall1win.tga
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

textures/central_europe/small_cobble_tan
{
	qer_keyword floor
	qer_keyword road
	qer_keyword stone
	qer_keyword flat
	surfaceparm stone
	{
		map textures/central_europe/small_cobble_tan.tga
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

textures/central_europe/jh_cathwin1
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	cull none
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/central_europe/jh_cathwin1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/central_europe/exterior_wall_2bottomtrim
{

	qer_keyword wall
	qer_keyword trim
	qer_keyword stone
	qer_keyword rock
	qer_keyword flat
	surfaceparm plaster
	{
		map textures/central_europe/exterior_wall_2bottomtrim.tga
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

textures/central_europe/exterior_wall_2
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	qer_keyword flat
	surfaceparm plaster
	{
		map textures/central_europe/exterior_wall_2.tga
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

textures/central_europe/spouting_hori_1
{
	qer_keyword corrugated
	qer_keyword metal
	qer_keyword trim
	surfaceparm metal
	{
		map textures/central_europe/spouting_hori_1.tga
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

textures/central_europe/spouting_vertical_drain
{
	qer_keyword pipe
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	{
		map textures/central_europe/spouting_vertical_drain.tga
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

textures/central_europe/damaged_roof_europe1
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword masked
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
    qer_editorimage textures/central_europe/damaged_roof_europe1.tga
	surfaceparm alphashadow
	surfaceparm wood
	cull none
	{
		map textures/central_europe/damaged_roof_europe1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}
textures/central_europe/damaged_roof_europe1a
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword masked
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/central_europe/damaged_roof_europe1a.tga
	surfaceparm wood
	cull none
	{
		map textures/central_europe/damaged_roof_europe1a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe/balcny_rail_set1
{
	qer_keyword masked
	qer_keyword rusted
	qer_keyword metal
	surfaceparm grill
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
    qer_editorimage textures/central_europe/balcny_rail_set1.tga
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map textures/central_europe/balcny_rail_set1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe/shutter_set2
{
	qer_keyword wood
	qer_keyword special
	qer_keyword window
	surfaceparm wood
	{
		map textures/central_europe/shutter_set2.tga
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

textures/central_europe/interiorwall_set2
{
	qer_keyword wall
	qer_keyword m3
	qer_keyword plaster
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe/interiorwall_set2.tga
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

textures/central_europe/interiorwall_set2wtrim
{
	qer_keyword m3
	qer_keyword wood
	qer_keyword wall
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/interiorwall_set2wtrim.tga
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

textures/central_europe/interiorwall_set2chrrl
{
	qer_keyword m3
	qer_keyword wood
	qer_keyword wall
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/interiorwall_set2chrrl.tga
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

textures/central_europe/interiorwall_set3chrrl
{
	qer_keyword m3
	qer_keyword wood
	qer_keyword wall
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/interiorwall_set3chrrl.tga
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

textures/central_europe/interiorwall_set3wtrim
{
	qer_keyword m3
	qer_keyword wood
	qer_keyword wall
	qer_keyword plaster
	surfaceparm wood
	{
		map textures/central_europe/interiorwall_set3wtrim.tga
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

textures/central_europe/damaged_roof_europe1b
{
	qer_keyword masked
	qer_keyword ceiling
	qer_keyword wood
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/central_europe/damaged_roof_europe1b.tga
	surfaceparm wood
	nomipmaps
	{
		map textures/central_europe/damaged_roof_europe1b.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/central_europe/frenchdoor_wood1
{
	qer_keyword door
	qer_keyword wood
	surfaceparm wood

	{
		map textures/central_europe/frenchdoor_wood1.tga
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

textures/central_europe/awning_french_overhang
{
	qer_keyword trim
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
    qer_editorimage textures/central_europe/awning_french_overhang.tga
	surfaceparm alphashadow
	nopicmip
	cull none
	nomipmaps
	{
		map textures/central_europe/awning_french_overhang.tga
		alphafunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/central_europe/remgn_wallexter1
{
	qer_keyword wall
	qer_keyword plaster
	qer_keyword wood
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe/remgn_wallexter1.tga
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

textures/central_europe/nrmndywlset_2c
{
	qer_keyword wall
	qer_keyword wood
	qer_keyword tudor
	surfaceparm plaster

	{
		map textures/central_europe/nrmndywlset_2c.tga
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

textures/central_europe/nrmndywlset_2b
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/nrmndywlset_2b.tga
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

textures/central_europe/nrmndywlset_2a
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/nrmndywlset_2a.tga
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

textures/central_europe/nrmndywlset_wood
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/nrmndywlset_wood.tga
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

textures/central_europe/nu_earth_putty
{
	qer_keyword road
	qer_keyword terrain
	qer_keyword gravel
	surfaceparm gravel
	{
		map textures/central_europe/nu_earth_putty.tga
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

textures/central_europe/cityrbbl1_revised
{
	qer_keyword gravel
	qer_keyword road
	qer_keyword terrain
	surfaceparm gravel

	{
		map textures/central_europe/cityrbbl1_revised.tga
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

textures/central_europe/tudor_set2a
{
	qer_keyword wall
	qer_keyword tudor
	qer_keyword wood
	surfaceparm wood
	{
		map textures/central_europe/tudor_set2a.tga
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

textures/central_europe/tudor_set2b
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/tudor_set2b.tga
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

textures/central_europe/tudor_set2c
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe/tudor_set2c.tga
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

textures/central_europe/tudor_set2d
{
	qer_keyword wood
	qer_keyword wall
	qer_keyword tudor
	surfaceparm wood
	{
		map textures/central_europe/tudor_set2d.tga
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

textures/central_europe/tudor_set1w_exwall1flat
{
	qer_keyword wall
	qer_keyword concrete
	qer_keyword tudor
	surfaceparm plaster
	{
		map textures/central_europe/tudor_set1w_exwall1flat.tga
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

textures/central_europe/brik_normandy1lt
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brik_normandy1lt.tga
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

textures/central_europe/brik_normandy1drk
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/brik_normandy1drk.tga
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

textures/central_europe/normndybrik1a
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/normndybrik1a.tga
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

textures/central_europe/normndybrik1_0
{
	qer_editorimage textures/central_europe/normndybrik1.tga
	qer_keyword wall
	qer_keyword brick
	surfaceparm stone
	{
		map textures/central_europe/normndybrik1.tga
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

textures/central_europe/normndybrik1sml
{
	qer_keyword wall
	qer_keyword brick
	qer_keyword flat
	surfaceparm stone
	{
		map textures/central_europe/normndybrik1sml.tga
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

textures/central_europe/normndybrik1sml_sharp
{
	qer_keyword wall
	qer_keyword brick
	qer_keyword flat
	surfaceparm stone
	{
		map textures/central_europe/normndybrik1sml_sharp.tga
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

textures/central_europe/strtset_cew
{
	qer_keyword brick
	qer_keyword road
	qer_keyword stone
	surfaceparm stone
	{
		map textures/central_europe/strtset_cew.tga
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

textures/central_europe/tudor_mackeywin
{
	qer_keyword avatar
	qer_keyword wall
	qer_keyword tudor
    qer_editorimage textures/central_europe/tudor_mackeywin.tga
	surfaceparm stone
	{
		map textures/central_europe/tudor_mackeywin.tga
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

textures/central_europe/curtain_1
{
	qer_keyword carpet
	qer_keyword special
	qer_keyword masked
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
    qer_editorimage textures/central_europe/curtain_1.tga
	surfaceparm alphashadow
	surfaceparm carpet
	cull none
        deformvertexes flap t 10 sin 0 10 0 .10 0 10
	{
		map textures/central_europe/curtain_1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/central_europe/curtain_1dirty
{
	qer_keyword carpet
	qer_keyword special
	qer_keyword masked
	surfaceparm fence
        surfaceparm nonsolid
    qer_editorimage textures/central_europe/curtain_1dirty.tga
	surfaceparm alphashadow
	cull none
        deformvertexes flap t 10 sin 1 3 50 .08 0 10
	{
		clampmap textures/central_europe/curtain_1dirty.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
        }
}

textures/central_europe/remgn_wallexter1win
{
	qer_keyword avatar
	qer_keyword wall
	qer_keyword tudor
    qer_editorimage textures/central_europe/remgn_wallexter1win.tga
	surfaceparm stone
	{
		map textures/central_europe/remgn_wallexter1win.tga
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

textures/central_europe/frenchdoor_wood1side
{
	qer_keyword door
	qer_keyword wood
	surfaceparm wood

	{
		map textures/central_europe/frenchdoor_wood1side.tga
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

textures/central_europe/altartop
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/altartop.tga

	{
		map textures/central_europe/altartop.tga
		nextbundle
		map $lightmap
	}
} 

textures/central_europe/altartop_pulse
{
qer_editorimage textures/central_europe/altartop.tga
qer_keyword indoor
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword m5
	{
		map textures/central_europe/altartop.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

textures/central_europe/altarend
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/altarend.tga

	{
		map textures/central_europe/altarend.tga
		nextbundle
		map $lightmap
	}
}

textures/central_europe/altarside
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/altarside.tga

	{
		map textures/central_europe/altarside.tga
		nextbundle
		map $lightmap
	}
}

textures/central_europe/altartrim1
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/trim1.tga

	{
		map textures/central_europe/trim1.tga
		nextbundle
		map $lightmap
	}
}

textures/central_europe/altartrim2
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/trim2.tga

	{
		map textures/central_europe/trim2.tga
		nextbundle
		map $lightmap
	}
}

textures/central_europe/altartrim3
{
	qer_keyword altar
	surfaceparm stone
	qer_editorimage textures/central_europe/trim3.tga

	{
		map textures/central_europe/trim3.tga
		nextbundle
		map $lightmap
	}
}




