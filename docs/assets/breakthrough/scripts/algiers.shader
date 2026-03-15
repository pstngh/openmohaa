textures/algiers/columntrm_1
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword stone
	qer_keyword special
	surfaceparm rock
	{
		map textures/algiers/columntrm_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
textures/algiers/columntrm_1_pulse
{
	qer_editorimage textures/algiers/columntrm_1.tga
	qer_keyword m1
	qer_keyword trim
	qer_keyword stone
	qer_keyword special
	surfaceparm rock


	{
		map textures/algiers/columntrm_1.tga
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



textures/algiers/algier_floor_trimsolid
{
	qer_keyword m1
	qer_keyword floor
	qer_keyword trim
	surfaceparm rock
	{
		map textures/algiers/algier_floor_trimsolid.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
textures/algiers/algier_floor_trimsolid_pulse
{
	qer_editorimage textures/algiers/algier_floor_trimsolid.tga
	qer_keyword m1
	qer_keyword floor
	qer_keyword trim
	surfaceparm rock

	{
		map textures/algiers/algier_floor_trimsolid.tga
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



textures/algiers/afrkwlcore_1
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrkwlcore_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/afrika_boulder
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrika_boulder.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/afrika_boulder2
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrika_boulder2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/sand_anom1
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm dirt
	{
		map textures/algiers/sand_anom1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_1a
{
	qer_keyword m1
	qer_keyword stone
	qer_keyword wall
 	surfaceparm rock
	{
		map textures/algiers/algrset1_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_1b
{
	qer_keyword m1
	qer_keyword window
	qer_keyword wall
	qer_keyword stone
 	surfaceparm rock
	{
		map textures/algiers/algrset1_1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_1bs
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_1bs.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_2a
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_2b
{
	qer_keyword m1
	qer_keyword window
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_2bs
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_2bs.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_3a
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_3a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_3b
{
	qer_keyword m1
	qer_keyword window
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_3b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algrset1_3bs
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algrset1_3bs.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzewbalc_se1
{
	qer_keyword m1
	qer_keyword window
	qer_keyword tudor
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/arzewbalc_se1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzewpj_1
{
	qer_keyword m1
	qer_keyword wood
	qer_keyword special
	surfaceparm wood
	{
		map textures/algiers/arzewpj_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algier_ceiling
{
	qer_keyword m1
	qer_keyword wood
	qer_keyword special
	surfaceparm wood
	{
		map textures/algiers/algier_ceiling.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzewpj_2
{
	qer_keyword m1
	qer_keyword wood
	qer_keyword special
	surfaceparm wood
	{
		map textures/algiers/arzewpj_2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzewpj_3
{
	qer_keyword m1
	qer_keyword wood
	qer_keyword special
	surfaceparm wood
	{
		map textures/algiers/arzewpj_3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzewwndw_se1a
{
	qer_keyword m1
	qer_keyword panel
	qer_keyword wall
	qer_keyword wood
	surfaceparm wood
	{
		map textures/algiers/arzewwndw_se1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/arzrblset_1
{
	qer_keyword gravel
	qer_keyword natural
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/arzrblset_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/grndset_2af
{
	qer_keyword gravel
	qer_keyword natural
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/grndset_2af.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/bldrset_1
{
	qer_keyword rock
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/bldrset_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/brick1
{
	qer_keyword brick
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/brick1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/caveset1_1bs
{
	qer_keyword natural
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/caveset1_1bs.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/cliffset_1
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/cliffset_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/cliffset_1a
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/cliffset_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/column_1
{
	qer_keyword m1
	qer_keyword special
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/column_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/column_2
{
	qer_keyword m1
	qer_keyword special
	qer_keyword stone
	surfaceparm rock
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/algiers/column_2.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/column_trimdec
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword stone
	qer_keyword special
	surfaceparm rock
	{
		map textures/algiers/column_trimdec.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}



textures/algiers/ctrckset1_1
{
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/ctrckset1_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/desertcloth
{
	qer_keyword m1
	qer_keyword carpet
	qer_keyword flat
	surfaceparm carpet
	cull none
	{
		map textures/algiers/desertcloth.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/doccrtset_1a
{
	qer_keyword m1
	qer_keyword concrete
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/doccrtset_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/doccrtset_1b
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/doccrtset_1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/doccrtset_1step
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/doccrtset_1step.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/doccrtset_1stepsml
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/doccrtset_1stepsml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/fort_floor_cobbleflttile
{
	qer_keyword wall
	qer_keyword brick
	qer_keyword stone
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/fort_floor_cobbleflttile.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/fort_floor_cobbleflttilebrdr
{
	qer_keyword wall
	qer_keyword stone
	qer_keyword floor
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/fort_floor_cobbleflttilebrdr.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/fort_floor_outside
{
	qer_keyword stone
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/fort_floor_outside.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/fort_floor_outsidelg
{
	qer_keyword stone
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/fort_floor_outsidelg.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/fort_floor_outsidesml
{
	qer_keyword stone
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/fort_floor_outsidesml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/aviation_poster
{
	qer_editorimage textures/algiers/frenchpostr_1a.tga
	qer_keyword signs
	surfaceparm trans
	surfaceparm paper
	PolygonOffset
	nopicmip
	{
		clampmap textures/algiers/frenchpostr_1a.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/algiers/gasvent_1
{
	qer_keyword trim
	qer_keyword metal
	surfaceparm metal
	{
		map textures/algiers/gasvent_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grassbrdr_6
{
	qer_keyword floor
	qer_keyword grass
	qer_keyword dirt
	qer_keyword natural
	surfaceparm dirt
	{
		map textures/algiers/grassbrdr_6.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grassbrdr_7
{
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/grassbrdr_7.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1
{
	qer_keyword m1
	qer_keyword gravel
	qer_keyword natural
	qer_keyword grass
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/grndset_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1a
{
	qer_keyword m1
	qer_keyword dirt
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/grndset_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1b
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/grndset_1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/e3road
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/e3road.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/desertrd_1
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
//what's this for? -Nate
//	surfaceparm nonsolid
//good question probably something an LD wanted :P - Talon        
	surfaceparm translucent
	{
		map textures/algiers/desertrd_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1big
{
	qer_keyword m1
	qer_keyword grass
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/grndset_1big.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1c
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/grndset_1c.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1long
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/grndset_1long.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grvstm1
{
	qer_keyword m1
	qer_keyword stone
	qer_keyword special
	surfaceparm rock
	{
		map textures/algiers/grvstm1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grvstm1a
{
	qer_keyword m1
	qer_keyword stone
	qer_keyword special
	surfaceparm rock
	{
		map textures/algiers/grvstm1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grvstm1b
{
	qer_keyword m1
	qer_keyword stone
	qer_keyword special
	surfaceparm rock
	{
		map textures/algiers/grvstm1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_brick3
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/jh_brick3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_brick3a
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/jh_brick3a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_brickwall1
{
	qer_keyword window
	qer_keyword wall
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/jh_brickwall1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_corrdoor1a
{
	qer_keyword metal
	qer_keyword rusted
	qer_keyword corrugated
	qer_keyword door
	surfaceparm metal
	{
		map textures/algiers/jh_corrdoor1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_corrdoor1b
{
	qer_keyword rusted
	qer_keyword metal
	qer_keyword door
	qer_keyword corrugated
	surfaceparm metal
	{
		map textures/algiers/jh_corrdoor1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_corrdoor2a
{
	qer_keyword rusted
	qer_keyword metal
	qer_keyword door
	qer_keyword corrugated
	surfaceparm metal
	{
		map textures/algiers/jh_corrdoor2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_corrdoor2b
{
	qer_keyword rusted
	qer_keyword metal
	qer_keyword door
	qer_keyword corrugated
	surfaceparm metal
	{
		map textures/algiers/jh_corrdoor2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_corrugate_256
{
	qer_keyword wall
	qer_keyword rusted
	qer_keyword metal
	qer_keyword corrugated
	surfaceparm metal
	{
		map textures/algiers/jh_corrugate_256.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_door1
{
	qer_keyword window
	qer_keyword wood
	qer_keyword door
	surfaceparm wood
	{
		map textures/algiers/jh_door1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_lhwall1
{
	qer_keyword concrete
	qer_keyword wall
	qer_keyword window
	surfaceparm rock
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_lhwall1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_lhwin1_0
{
	qer_editorimage textures/algiers/jh_lhwin1.tga
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/algiers/jh_lhwin1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_lhwin1a
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	surfaceparm trans
	cull none
	nopicmip
	{
		map textures/algiers/jh_lhwin1a.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/jh_rustwin1
{
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_rustwin1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_sliddoortrack
{
	qer_keyword concrete
	qer_keyword metal
	qer_keyword trim
	surfaceparm metal
	{
		map textures/algiers/jh_sliddoortrack.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_warewindow1
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_warewindow1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_warewindow1a
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_warewindow1a.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_warewindow2a
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_warewindow2a.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_warewindow2b
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_warewindow2b.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_warewindow3
{
	qer_keyword glass
	qer_keyword window
	surfaceparm glass
	{
		map textures/mohcommon/environ_puddle.tga
		tcGen environment
	}
	{
		map textures/algiers/jh_warewindow3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/jh_woodwall_1
{
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/algiers/jh_woodwall_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lthouse_set1a
{
	qer_keyword plaster
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/lthouse_set1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lthouse_set1base
{
	qer_keyword plaster
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/lthouse_set1base.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lthouse_wal1a
{
	qer_keyword plaster
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/lthouse_wal1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lthouse_wal1b
{
	qer_keyword pipe
	qer_keyword plaster
	qer_keyword flat
	surfaceparm rock
	{
		map textures/algiers/lthouse_wal1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lighthouse_railing
{
	qer_editorimage textures/algiers/lthserail_set1m.tga
	qer_keyword rusted
	qer_keyword metal
	qer_keyword masked
	surfaceparm grill
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	cull none
	nopicmip
	{
		map textures/algiers/lthserail_set1m.tga
		depthWrite depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/railing_natestah
{
	qer_editorimage textures/algiers/lthserail_set1mnate.tga
	qer_keyword rusted
	qer_keyword metal
	qer_keyword masked
	surfaceparm fence
	surfaceparm grill
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	cull none
	nopicmip
	{
		map textures/algiers/lthserail_set1mnate.tga
		depthWrite depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/omaha_set2
{
	qer_keyword sand
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/omaha_set2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/omaha_set2feet
{
	qer_keyword sand
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/omaha_set2feet.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/algiers/omaha_set2ftsh2
{
	qer_keyword sand
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/omaha_set2ftsh2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_1a1b
{
	qer_keyword m1
	qer_keyword concrete
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/pierset_1a1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3a
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3ptty
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword flat
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3ptty.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3pwb
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3pwb.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3swtop
{
	qer_keyword m1
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3swtop.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/pierset_3swtop1
{
	qer_keyword m1
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/pierset_3swtop1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/lightplaster1
{
	qer_editorimage textures/algiers/plasterwal2.tga
	qer_keyword flat
	qer_keyword plaster
	surfaceparm rock
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/algiers/plasterwal2.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/lightplaster2
{
	qer_editorimage textures/algiers/plasterwal2a.tga
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	qer_keyword flat
    	surfaceparm rock
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/algiers/plasterwal2a.tga
		blendfunc GL_DST_COLOR GL_ZERO
	}
}

textures/algiers/plasterwal2b
{
	qer_keyword stone
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/plasterwal2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/plasterwal4b1
{
	qer_keyword concrete
	qer_keyword wall
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/plasterwal4b1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/radr_set1
{
	qer_keyword special
	qer_keyword indoor
	surfaceparm metal
	{
		map textures/algiers/Radr_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rckfcset2_1a
{
	qer_keyword rock
	qer_keyword natural
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/rckfcset2_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rdr_algierset_1
{
	qer_keyword m1
	qer_keyword concrete
	qer_keyword brick
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/rdr_algierset_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rdr_algierset_1b
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/rdr_algierset_1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rdr_algierset_1cnd
{
	qer_keyword m1
	qer_keyword pipe
	qer_keyword wall
	qer_keyword concrete
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/rdr_algierset_1cnd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rdr_algierset_1cnd1
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword pipe
	qer_keyword concrete
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/rdr_algierset_1cnd1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rock02s
{
	qer_keyword natural
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/rock02S.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rock02s1a
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/rock02S1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/rock04s
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/rock04S.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/seawall_1
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/seawall_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/seawall_1cliff
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword natural
	surfaceparm rock
	{
		map textures/algiers/seawall_1cliff.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/set1feetsml
{
	qer_keyword sand
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/set1feetsml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/stset_2a
{
	qer_keyword road
	qer_keyword stone
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/stset_2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/stset_2base
{
	qer_keyword stone
	qer_keyword road
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/stset_2base.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/stset_2sand
{
	qer_keyword stone
	qer_keyword road
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/stset_2sand.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/tentdsrt
{
	qer_keyword carpet
	qer_keyword masked
	surfaceparm fence
	surfaceparm paper
	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	qer_editorimage textures/algiers/tentdsrt.tga
	cull none
	nopicmip
	{
		map textures/algiers/tentdsrt.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/tentdsrt_trim
{
	qer_keyword carpet
	qer_keyword masked
	surfaceparm fence
	surfaceparm paper
	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	qer_editorimage textures/algiers/tentdsrt_trim.tga
	cull none
	nopicmip
	{
		map textures/algiers/tentdsrt_trim.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
} 

textures/algiers/tentdsrt_trimentre
{
	qer_keyword carpet
	qer_keyword masked
	surfaceparm fence
	surfaceparm paper
	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	qer_editorimage textures/algiers/tentdsrt_trimentre.tga
	cull none
	nopicmip
	{
		map textures/algiers/tentdsrt_trimentre.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/fort_canopy
{
	qer_editorimage textures/algiers/tentdsrt2.tga
	qer_keyword paper
	qer_keyword masked
	surfaceparm paper
	surfaceparm fence
	surfaceparm alphashadow
	cull none
	nopicmip

	{
		map textures/algiers/tentdsrt2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/fort_canopytorn
{
	qer_editorimage textures/algiers/tentdsrt2torn.tga
	qer_keyword carpet
	qer_keyword masked
	surfaceparm fence
	surfaceparm paper
	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	cull none
	nopicmip
	{
		map textures/algiers/tentdsrt2torn.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/walarzset_1flt
{
	qer_keyword flat
	qer_keyword concrete
	qer_keyword wall
	surfaceparm rock

	{
		map textures/algiers/walarzset_1flt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/walarzset_1fltb
{
	qer_keyword wall
	qer_keyword flat
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/walarzset_1fltb.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/walarzset_1trmpc
{
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/walarzset_1trmpc.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wdtrimbal
{
	qer_keyword wood
	qer_keyword trim
	surfaceparm wood
	{
		map textures/algiers/wdtrimbal.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wetmud
{
	qer_keyword flat
	qer_keyword dirt
	qer_keyword floor
	surfaceparm dirt
	{
		map textures/algiers/wetmud.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wh_algierset_1bmrst1
{
	qer_keyword m1
	qer_keyword concrete
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/wh_algierset_1bmrst1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wh_algierset_conc1
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/wh_algierset_conc1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wh_algierset_conc1a
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/wh_algierset_conc1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wh_algierset_conc1b
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/wh_algierset_conc1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wh_algierset_conc1b2
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/wh_algierset_conc1b2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/whsflrset1_1a
{
	qer_keyword flat
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/whsflrset1_1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/whsflrset1_1b
{
	qer_keyword floor
	qer_keyword flat
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/whsflrset1_1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wl_algiers_1flt
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/wl_algiers_1flt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/wl_algiers_1fltdrt
{
	qer_keyword wall
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/wl_algiers_1fltdrt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_oran
{
	qer_keyword signs
	qer_keyword metal
	surfaceparm metal
	{
		map textures/algiers/jh_oran.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_km_marker
{
	qer_keyword signs
	qer_keyword metal
	surfaceparm metal
	{
		map textures/algiers/jh_km_marker.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_portarz_ft
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/algiers/jh_portarz_ft.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/jh_portarz_bk
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/algiers/jh_portarz_bk.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1b
{
	qer_keyword wall
	qer_keyword m1
	qer_keyword plaster
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algierwall1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5drt_moz
{
	qer_keyword wall
	qer_keyword m1
	qer_keyword plaster
	qer_keyword stone
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5drt_moz.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1a
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1win
{
	qer_keyword window
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall1win.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algier_winbox_detail1
{
	qer_keyword wall
	qer_keyword trim
	qer_keyword panel
	qer_keyword wood
	qer_keyword m1
	surfaceparm wood
	{
		map textures/algiers/algier_winbox_detail1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5
{
	qer_keyword wall
	qer_keyword m1
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5brt
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5brt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algier_ciel5
{
	qer_keyword ceiling
	qer_keyword m1
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algier_ciel5.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5blin
{
	qer_keyword wall
	qer_keyword m1
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5blin.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5blindrt
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5blindrt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5decobrt
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5decobrt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5decodrt
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5decodrt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall_set5drt
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5drt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/tapstry_af1
{
	qer_keyword carpet
	qer_keyword masked
	qer_keyword m1
	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm carpet
	surfaceparm fence
	nopicmip
	cull none
	qer_editorimage textures/algiers/tapstry_af1.tga
	{
		map textures/algiers/tapstry_af1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}


textures/algiers/algierwall2b
{
	qer_keyword wall
	qer_keyword concrete
	qer_keyword plaster
	qer_keyword m1
	surfaceparm rock
	{
		map textures/algiers/algierwall2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1_0
{
	qer_editorimage textures/algiers/algierwall1.tga
	qer_keyword flat
	qer_keyword m1
	qer_keyword wall
	qer_keyword plaster
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrikatrim_set1
{
	qer_keyword trim
	qer_keyword masked
	qer_keyword m1
	qer_keyword plaster
	qer_keyword concrete
//	surfaceparm fence
	surfaceparm rock
//	PolygonOffset
	cull none
	{
		map textures/algiers/afrikatrim_set1.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrikatrim_set1ltgrn
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword plaster
	qer_keyword masked
	qer_keyword concrete
	surfaceparm rock
	cull none
	PolygonOffset
	{
		map textures/algiers/afrikatrim_set1ltgrn.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrikatrim_set1ltylw
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword plaster
	qer_keyword masked
	qer_keyword concrete
	surfaceparm rock
	cull none
	PolygonOffset
	{
		map textures/algiers/afrikatrim_set1ltylw.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrikatrim_set1red
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword plaster
	qer_keyword masked
	qer_keyword concrete
	surfaceparm rock
	cull none
	PolygonOffset
	{
		map textures/algiers/afrikatrim_set1red.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/algiers/maze_deco_afrika
{
	qer_keyword metal
	qer_keyword m1
	qer_keyword trim
	qer_keyword masked
	surfaceparm grill
	surfaceparm fence
	cull none
	nopicmip
	qer_editorimage textures/algiers/maze_deco_afrika.tga
	{
		map textures/algiers/maze_deco_afrika.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/maze_deco_afrikawthrd1
{
	qer_keyword m1
	qer_keyword trim
	qer_keyword metal
	qer_keyword masked
	surfaceparm fence
	surfaceparm grill
	cull none
	nopicmip
	qer_editorimage textures/algiers/maze_deco_afrikawthrd1.tga
	{
		map textures/algiers/maze_deco_afrikawthrd1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrika_windecal
{
	qer_keyword wood
	qer_keyword window
	qer_keyword special
	qer_keyword masked
	qer_keyword m1
	surfaceparm trans
	surfaceparm wood
	PolygonOffset
	nopicmip
	{
		clampmap textures/algiers/afrika_windecal.tga
		blendfunc blend
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrika_floor_set1
{
	qer_keyword m1
	qer_keyword concrete
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afrika_floor_set1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrika_floor_set1trm
{
	qer_keyword indoor
	qer_keyword m1
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/afrika_floor_set1trm.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}  

textures/algiers/afrika_set1tile
{
	qer_keyword indoor
	qer_keyword m1
	qer_keyword floor
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/afrika_set1tile.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}



textures/algiers/window_decor_set1
{
	qer_keyword special
	qer_keyword trim
	qer_keyword masked
	qer_keyword window
	qer_keyword m1
	qer_keyword wood
	qer_keyword metal
	surfaceparm metal
	surfaceparm monsterclip
	surfaceparm playerclip
	surfaceparm fence
	surfaceparm alphashadow
    qer_editorimage textures/algiers/window_decor_set1.tga
	PolygonOffset
	cull none
	nopicmip
	{
		map textures/algiers/window_decor_set1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/roadtread_afrika
{
	qer_keyword dirt
	qer_keyword m1
	qer_keyword road
	surfaceparm dirt
	{
		map textures/algiers/roadtread_afrika.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/grndset_1x
{
	qer_keyword m1
	qer_keyword dirt
	qer_keyword natural
	qer_keyword terrain
	surfaceparm dirt
	{
		map textures/algiers/grndset_1x.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1b_mozaic_brn
{
	qer_keyword concrete
	qer_keyword m1
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/algierwall1b_mozaic_brn.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1b_mozaic_red
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1b_mozaic_red.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1b_mozaic_bl
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1b_mozaic_bl.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_1flat
{
	qer_keyword m1
	qer_keyword flat
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_1flat.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_1wthrd
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_1wthrd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_redmoz
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_redmoz.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_blumoz
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_blumoz.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_blumozwd
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_blumozwd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwall1drk_redmozwd
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/algierwall1drk_redmozwd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruinswall_trans
{
	qer_keyword wall
	qer_keyword rock
	qer_keyword stone
	qer_keyword m1
	surfaceparm rock
	{
		map textures/algiers/ruinswall_trans.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruinswall
{
	qer_editorimage textures/algiers/ruinswall.tga
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/algiers/ruinswall.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruinswall_bot
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/algiers/ruinswall_bot.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruins_al1rcksml
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/ruins_al1rcksml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruins_al1rcktrnsml
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/ruins_al1rcktrnsml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/ruins_al1sml
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword rock
	surfaceparm rock
	{
		map textures/algiers/ruins_al1sml.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrikamap1
{
	qer_editorimage textures/algiers/afrika_map1.tga
	qer_keyword special
	qer_keyword carpet
	qer_keyword masked
    surfaceparm paper
	cull none
	nopicmip
	{
		map textures/algiers/afrika_map1.tga
		depthWrite depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrikamap2
{
	qer_editorimage textures/algiers/afrika_map2.tga
	qer_keyword special
	qer_keyword carpet
	qer_keyword masked
    surfaceparm paper
	cull back
	nopicmip
	{
		map textures/algiers/afrika_map2.tga
		depthWrite depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/algiers/afrik_wall1a
{
	qer_keyword wall
	qer_keyword m1
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/afrik_wall1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrik_wall1b
{
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/afrik_wall1b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrik_wall1brick
{
	qer_keyword brick
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/afrik_wall1brick.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrik_wall1c
{
	qer_keyword trim
	qer_keyword m1
	qer_keyword wall
	qer_keyword stone
	qer_keyword plaster
	surfaceparm rock
	{
		map textures/algiers/afrik_wall1c.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrik_wall2des1
{
	qer_keyword concrete
	qer_keyword plaster
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrik_wall2des1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrik_wall2fltdrk
{
	qer_keyword wall
	qer_keyword plaster
	qer_keyword concrete
	surfaceparm rock
	{
		map textures/algiers/afrik_wall2FLTdrk.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/brick1trim
{
	qer_keyword trim
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/brick1trim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/brick1trim_0
{
	qer_editorimage textures/algiers/brick1trim.tga
	qer_keyword trim
	qer_keyword brick
	surfaceparm rock
	{
		map textures/algiers/brick1trim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/canyonwall
{
	qer_editorimage textures/algiers/canyon1.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/canyon1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/canyonwall_bottom
{
	qer_editorimage textures/algiers/canyon_1bottrns.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/canyon_1bottrns.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/canyonylw1
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/canyonylw1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/canyonylw1bottrns
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/canyonylw1bottrns.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/savel_1
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/savel_1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algiertrim
{
	qer_keyword m1
	qer_keyword stone
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/algiertrim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/algierwallwithtrim
{
	qer_editorimage textures/algiers/algierwall_set5wtrim.tga
	qer_keyword m1
	qer_keyword stone
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/algierwall_set5wtrim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrikwall7_set1base
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrikwall7_set1base.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrikwall7_set1doorway
{
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/afrikwall7_set1doorway.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afriktile1_div
{
	qer_keyword rock
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afriktile1_div.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afriktile1_dirtdiv
{
	qer_keyword rock
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afriktile1_dirtdiv.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afriktile1_broke
{
	qer_keyword rock
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afriktile1_broke.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afriktile1_dirt
{
	qer_keyword rock
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afriktile1_dirt.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afriktile1
{
	qer_keyword rock
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/afriktile1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/algiers/afrikadoorwrk
{
	qer_keyword wood
	qer_keyword door
	surfaceparm wood
	{
		map textures/algiers/afrikadoorwrk.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/interiorwall_afrika1trim
{
	qer_keyword concrete
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/interiorwall_afrika1trim.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/interiorwall_afrika1flat
{
	qer_keyword concrete
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/interiorwall_afrika1flat.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/cliffset2
{
	qer_editorimage textures/algiers/cliffset2.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/cliffset2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/cliffset2_sandtop
{
	qer_editorimage textures/algiers/cliffset2_sandtop.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/cliffset2_sandtop.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/cliffset2_sandtop2
{
	qer_editorimage textures/algiers/cliffset2_sandtop2.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/cliffset2_sandtop2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 
textures/algiers/cliffset2_sandtop2small
{
	qer_editorimage textures/algiers/cliffset2_sandtop2small.tga
	qer_keyword rock
	qer_keyword wall
	surfaceparm rock
	{
		map textures/algiers/cliffset2_sandtop2small.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/algiers/desertrd_512
{
	qer_keyword m1
	qer_keyword road
	qer_keyword natural
	qer_keyword gravel
	qer_keyword floor
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/algiers/desertrd_512.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 
	
textures/algiers/fort_roofwhite
{
	qer_keyword m1
	qer_keyword floor
	surfaceparm rock
	{
		map textures/algiers/fort_roofwhite.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


