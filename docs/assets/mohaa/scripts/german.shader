
textures/german/wheel_surface
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal
	qer_keyword railgun

	{
		map textures/German/wheel_surface.tga

	nextbundle
		map $lightmap
	}
}

textures/german/rusty_iron
{
	qer_keyword special
	qer_keyword metal
	qer_keyword rusted
	surfaceParm metal
	{
		map textures/German/rusty_iron.tga
	nextbundle
		map $lightmap
	}
}

textures/german/rusty_iron_pulsating
{
	qer_editorimage textures/German/rusty_iron.tga
	qer_keyword special
	qer_keyword metal
	qer_keyword rusted
	surfaceParm metal
	{
		map textures/German/rusty_iron.tga
		depthWrite
		rgbGen identity
	nextbundle
		map $lightmap
		blendFunc blend
		alphaGen const 1
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}

}

textures/german/leaf_spring
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal
	qer_keyword rusted

	{
		map textures/German/leaf_spring.tga

	nextbundle
		map $lightmap
	}
}

textures/german/wheel_side
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal
	qer_keyword rusted

	{
		map textures/German/wheel_side.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_stake
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_stake.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minenpost_top
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minenpost_top.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_side
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_side.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_post
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_post.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_gasse
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_gasse.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_gap3
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_gap3.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_gap2
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_gap2.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_gap
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_gap.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_fake
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_fake.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_entmint
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_entmint.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_back
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_back.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_alt2
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_alt2.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen_alt
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/minen_alt.tga

	nextbundle
		map $lightmap
	}
}

textures/german/minen
{


	{
		map textures/german/minen.tga

	nextbundle
		map $lightmap
	}
}

textures/german/naziprop1
{
	surfaceParm carpet
	surfaceParm fence
	surfaceParm alphashadow
	cull back
	nopicmip

	qer_editorimage textures/german/germanpst1.tga
	qer_keyword signs
	qer_keyword masked

	{
		map textures/german/germanpst1.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/german_sign_halt_jakoba
{
	surfaceParm metal
	surfaceParm fence
	surfaceParm alphashadow
	cull none
	nopicmip

	qer_editorimage textures/german/grmnsn_halt1.tga
	qer_keyword signs

	{
		map textures/german/grmnsn_halt1.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/german_sign_halt_green
{
	surfaceParm metal
	surfaceParm fence
	surfaceParm alphashadow
	cull none
	nopicmip

	qer_editorimage textures/german/grmnsn_halt1grn.tga
	qer_keyword signs

	{
		map textures/german/grmnsn_halt1grn.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/german_sign_halt_rust
{
	surfaceParm metal
	surfaceParm fence
	surfaceParm alphashadow
	cull none
	nopicmip

	qer_editorimage textures/german/grmnsn_halt1rst.tga
	qer_keyword signs

	{
		map textures/german/grmnsn_halt1rst.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/nazibanner1
{
	surfaceParm carpet
	surfaceParm nonsolid
	surfaceParm fence
// (Boon) Someone put bulge in here instead of flap.  I don't know why, maybe flap is broken in
// the code or something.  Try putting flap back instead of bulge at some point...
// 	deformvertexes flap t 30 sin 0 10 0 .2
	deformVertexes bulge 32 1 1
	cull none
	nopicmip

	qer_editorimage textures/german/nazbannr1.tga
	qer_keyword masked
	qer_keyword signs

	{
		map textures/german/nazbannr1.tga
		blendFunc blend

	nextbundle
		map $lightmap
	}
}

textures/german/daily_gorring
{
	surfaceParm wood
	surfaceParm nonsolid
	surfaceParm fence
	surfaceParm alphashadow
	nopicmip

	qer_keyword signs
	qer_keyword natural
	qer_keyword carpet
	qer_keyword wood
	qer_keyword tudor
	qer_editorimage textures/german/daily_gorring.tga

	{
		map textures/german/daily_gorring.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/jh_fcawwfy
{
	surfaceParm metal

	qer_keyword metal
	qer_keyword signs

	{
		map textures/german/jh_fcawwfy.tga
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

textures/german/jh_nosmoking_ger1a
{
	surfaceParm metal

	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_nosmoking_ger1a.tga
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

textures/german/jh_nosmoking_ger_0
{
	surfaceParm metal

	qer_editorimage textures/german/jh_nosmoking_ger.tga
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_nosmoking_ger.tga
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

textures/german/jh_nosmoking_ger_0
{
	surfaceParm metal

	qer_editorimage textures/german/jh_nosmoking_ger.tga
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_nosmoking_ger.tga
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

textures/german/jh_noentry1a
{
	surfaceParm metal
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_noentry1a.tga
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

textures/german/nazbanner_eagle
{
	surfaceParm carpet
	qer_keyword signs
	{
		map textures/german/nazbanner_eagle.tga
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

textures/german/jh_driveslow
{
	surfaceParm metal

	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_driveslow.tga
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

textures/german/jh_h20disbursement
{
	surfaceParm metal

	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_h20disbursement.tga
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

textures/german/jh_gasstorage2
{
	surfaceParm metal

	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_gasstorage2.tga
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

textures/german/jh_gasstorage_0
{
	surfaceParm metal

	qer_editorimage textures/german/jh_gasstorage.tga
	qer_keyword rusted
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_gasstorage.tga
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

textures/german/jh_nosmoking_ger_1
{
	surfaceParm metal

	qer_editorimage textures/german/jh_nosmoking_ger.tga
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_nosmoking_ger.tga
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

textures/german/jh_noentry1_0
{
	surfaceParm metal

	qer_editorimage textures/german/jh_noentry1.tga
	qer_keyword signs
	qer_keyword metal

	{
		map textures/german/jh_noentry1.tga
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

textures/german/jh_airfield
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/jh_airfield.tga
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

textures/german/jh_tansign
{
	surfaceParm wood

	qer_keyword wood
	qer_keyword signs

	{
		map textures/german/jh_tansign.tga
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

textures/german/jh_remagen2km
{
	surfaceParm wood

	qer_keyword signs
	qer_keyword wood

	{
		map textures/german/jh_remagen2km.tga
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

textures/german/nazbannr1c
{
	// 	deformvertexes flap t 30 sin 0 10 0 .2
	surfaceParm alphashadow
	surfaceParm nonsolid
	surfaceParm fence
	cull none
	nopicmip
	qer_editorimage textures/german/nazbannr1c.tga


	{
		map textures/german/nazbannr1c.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/raildoor
{
	surfaceParm wood

	qer_keyword wood
	qer_keyword special

	{
		map textures/german/raildoor.tga
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

textures/german/raildoor2
{
	surfaceParm wood

	qer_keyword wood
	qer_keyword special

	{
		map textures/german/raildoor2.tga
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

textures/german/railside
{
	surfaceParm wood

	qer_keyword wood
	qer_keyword special

	{
		map textures/german/railside.tga
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

textures/german/railside2
{
	surfaceParm wood

	qer_keyword wood
	qer_keyword special

	{
		map textures/german/railside2.tga
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

textures/german/bumper
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal

	{
		map textures/german/bumper.tga
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

textures/german/bumpplate
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal

	{
		map textures/german/bumpplate.tga
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

textures/german/railroof
{
	surfaceParm metal
	cull none

	qer_keyword special
	qer_keyword rusted
	qer_keyword metal

	{
		map textures/german/railroof.tga
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

textures/german/radar_frontflattemp
{
	surfaceParm metal

	qer_keyword special
	qer_keyword metal

	{
		map textures/german/radar_frontflattemp.tga
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

textures/german/radar_frontflat_dial
{
	surfaceParm fence
	surfaceParm alphashadow
	surfaceParm metal
	cull none

	qer_keyword masked
	qer_keyword special
	qer_keyword metal
	qer_editorimage textures/german/radar_frontflat_dial.tga

	{
		map textures/german/radar_frontflat_dial.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
}

textures/german/germnsign_1tavrn1Wntr
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/germnsign_1tavrn1Wntr.tga
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

textures/german/germnsign_1tavrn1
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/germnsign_1tavrn1.tga
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

textures/german/germnsign_mapmaker1
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/germnsign_mapmaker1.tga
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

textures/german/germnsign_mapmaker1wntr
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/germnsign_mapmaker1wntr.tga
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

textures/german/innsign_grmn2
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/innsign_grmn2.tga
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

textures/german/innsign_grmn2wntr
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/innsign_grmn2wntr.tga
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

textures/german/tavernsign_grmn1wntr
{
	surfaceParm wood
	qer_keyword special
	qer_keyword signs

	{
		map textures/german/tavernsign_grmn1wntr.tga
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

textures/german/german_songbookInt
{
	surfaceParm alphashadow
	surfaceParm paper
	surfaceParm fence
    surfaceParm nonsolid
	cull none
	nopicmip
	qer_editorimage textures/german/german_songbookInt.tga
	{
		map textures/german/german_songbookInt.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
} 

textures/german/songbook_german
{
	surfaceParm alphashadow
	surfaceParm paper
	surfaceParm fence
    surfaceParm nonsolid
	cull none
	nopicmip
	qer_editorimage textures/german/songbook_german.tga


	{
		map textures/german/songbook_german.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
} 

textures/german/guardpost
{
	surfaceParm wood
	qer_keyword special
	
	{
		map textures/german/guardpost.tga
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

textures/german/battery_side
{
	surfaceParm wood
	qer_keyword special
	
	{
		map textures/german/battery_side.tga
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

textures/german/battery_top
{
	surfaceParm wood
	qer_keyword special
	
	{
		map textures/german/battery_top.tga
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

textures/german/gnrlcratesml_top
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_top.tga
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

textures/german/gnrlcratesml_afrika
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_afrika.tga
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

textures/german/gnrlcratesml_afrikaflnk
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_afrikaflnk.tga
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

textures/german/gnrlcratesml_afrika1flnk
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_afrika1flnk.tga
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

textures/german/gnrlcratesml_afrika1
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_afrika1.tga
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

textures/german/gnrlcratesml_top1
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_top1.tga
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

textures/german/gnrlcratesml_side
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_side.tga
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

textures/german/gnrlcratesml_frnt
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/gnrlcratesml_frnt.tga
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

textures/german/crate_reinforced1_top
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/crate_reinforced1_top.tga
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

textures/german/crate_reinforced1_topflt
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/crate_reinforced1_topflt.tga
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

textures/german/crate_reinforced1_side
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/crate_reinforced1_side.tga
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

textures/german/crate_reinforced1_sideflt
{
	surfaceParm wood
	qer_keyword crate
	qer_keyword wood

	{
		map textures/german/crate_reinforced1_sideflt.tga
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

textures/german/grdhousewindow1
{
	surfaceParm glass
	surfaceParm fence
	surfaceParm alphashadow
	cull none
	nopicmip
	qer_editorimage textures/german/grdhousewindow1.tga
	qer_keyword masked

	{
		map textures/german/grdhousewindow1.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
} 

textures/german/persnl_lockerfrnt
{
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	{
		map textures/german/persnl_lockerfrnt.tga
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

textures/german/persnl_lockerside
{
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	{
		map textures/german/persnl_lockerside.tga
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

textures/german/personel_lockertall
{
	qer_keyword special
	qer_keyword metal
	surfaceParm metal
	{
		map textures/german/personel_lockertall.tga
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

textures/german/bunker_heavydoor1
{
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	{
		map textures/german/bunker_heavydoor1.tga
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

textures/german/bunker_heavydoor1wout
{
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	{
		map textures/german/bunker_heavydoor1wout.tga
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

textures/german/glasstabletop_1
{
	surfaceParm alphashadow
	surfaceParm paper
	surfaceParm fence
    surfaceParm nonsolid
	cull none
	nopicmip
        qer_editorimage textures/german/glasstabletop_q.tga
	{
		map textures/german/glasstabletop_1.tga
		alphaFunc GE128
		depthWrite

	nextbundle
		map $lightmap
	}
} 

textures/german/bulletinboard
{
	qer_keyword special
	qer_keyword wood
	surfaceParm wood
	{
		map textures/german/bulletinboard.tga
		nextbundle
		map $lightmap
	}
} 

textures/german/railgun_barrel
{
	surfaceParm metal
	qer_keyword special
	qer_keyword metal
	qer_keyword railgun

	{
		map textures/German/railgun_barrel.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/railgun_baseback
{
	surfaceParm metal
	qer_keyword special
	qer_keyword metal
	qer_keyword railgun

	{
		map textures/German/railgun_baseback.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/railgun_basefront
{
	surfaceParm metal
	qer_keyword special
	qer_keyword metal
	qer_keyword railgun

	{
		map textures/German/railgun_basefront.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/railgun_flat
{
	surfaceParm metal
	qer_keyword special
	qer_keyword metal
	qer_keyword railgun

	{
		map textures/German/railgun_flat.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/ex_crate1
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate1.tga

	nextbundle
		map $lightmap
	}
}

textures/german/ex_crate2
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate2.tga

	nextbundle
		map $lightmap
	}
}
textures/german/ex_crate3
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate3.tga

	nextbundle
		map $lightmap
	}
}
textures/german/ex_crate4
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate4.tga

	nextbundle
		map $lightmap
	}
}
textures/german/ex_crate5
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate5.tga

	nextbundle
		map $lightmap
	}
}
textures/german/ex_crate6
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/ex_crate6.tga

	nextbundle
		map $lightmap
	}
}

textures/german/misc_crate1a
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/misc_crate1a.tga

	nextbundle
		map $lightmap
	}
}

textures/german/misc_crate1b
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/misc_crate1b.tga

	nextbundle
		map $lightmap
	}
}

textures/german/misc_crate1c
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/misc_crate1c.tga

	nextbundle
		map $lightmap
	}
}

textures/german/misc_crate1d
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/misc_crate1d.tga

	nextbundle
		map $lightmap
	}
}

textures/german/misc_crate1e
{
	surfaceParm wood
	qer_keyword special
	qer_keyword crate
	{
		map textures/German/misc_crate1e.tga

	nextbundle
		map $lightmap
	}
}

textures/german/rauchenverboten
{
	surfaceParm wood
	qer_keyword special
	//qer_keyword crate
	{
	map textures/German/rauchenverboten.tga

	nextbundle
		map $lightmap
	}
}

textures/german/restrictedarea
{
	surfaceParm wood
	qer_keyword special
	//qer_keyword crate
	{
	map textures/German/restrictedarea.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/control
{
	surfaceParm wood
	qer_keyword dmsign
	{
	map textures/German/control.tga

	nextbundle
		map $lightmap
	}
} 

textures/german/platform
{
	surfaceParm wood
	qer_keyword dmsign
	{
	map textures/German/platform.tga

	nextbundle
		map $lightmap
	}
} 

