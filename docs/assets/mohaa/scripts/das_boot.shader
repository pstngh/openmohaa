textures/das_boot/ironwall1
{
	qer_keyword wall
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/ironwall1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
textures/das_boot/ironwall1_pulse
{
	qer_editorimage textures/das_boot/ironwall1.tga
	qer_keyword wall
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/ironwall1.tga
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


textures/das_boot/ballast_ind
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/ballast_ind.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/battery_vent_sign
{
	qer_keyword m2
	qer_keyword signs
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/battery_vent_sign.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/cabinet1
{
	qer_keyword wall
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/cabinet1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/calendar
{
	qer_keyword special
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/calendar.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/chalkboard
{
	qer_keyword wood
	qer_keyword special
	surfaceparm wood
	{
		map textures/das_boot/chalkboard.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/elec_panel1
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/elec_panel1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/elec_panel2
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/elec_panel2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/elec_panel4
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/elec_panel4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/galley_floor
{
	qer_keyword floor
	qer_keyword stone
	surfaceparm stone
	{
		map textures/das_boot/galley_floor.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/head_light
{
	qer_keyword special
	qer_keyword glass
	surfaceparm glass
	{
		map textures/das_boot/head_light.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/iron_floor3
{
	qer_keyword metal
	qer_keyword floor
	surfaceparm metal
	{
		map textures/das_boot/iron_floor3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/iron_floor3a
{
	qer_keyword metal
	qer_keyword floor
	surfaceparm metal

	{
		map textures/das_boot/iron_floor3a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/iron_floor3b
{
	qer_keyword metal
	qer_keyword floor
	surfaceparm metal
	{
		map textures/das_boot/iron_floor3b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}



textures/das_boot/ironwall1_sub		// Special player nonsolid version for the sub hatches
{
	qer_editorimage textures/das_boot/ironwall1.tga
	qer_keyword wall
	qer_keyword metal
	surfaceparm metal
	surfaceparm weaponclip
	surfaceparm monsterclip
	{
		map textures/das_boot/ironwall1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/klar_ind
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/klar_ind.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/klar_ind_sd
{
	qer_keyword panel
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/klar_ind_sd.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/officer_quarter
{
	qer_keyword wall
	qer_keyword wood
	qer_keyword panel
	surfaceparm wood
	{
		map textures/das_boot/officer_quarter.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/officer_quarter1
{
	qer_keyword wall
	qer_keyword wood
	qer_keyword panel
	surfaceparm wood
	{
		map textures/das_boot/officer_quarter1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/officer_quarter2
{
	qer_keyword panel
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/das_boot/officer_quarter2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/officer_quarter3
{
	qer_keyword panel
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/das_boot/officer_quarter3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/officer_quarter4
{
	qer_keyword panel
	qer_keyword wood
	qer_keyword wall
	surfaceparm wood
	{
		map textures/das_boot/officer_quarter4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/ribpanelling
{
	qer_keyword ceiling
	qer_keyword wall
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/ribpanelling.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/speakerbox1
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/speakerbox1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/speakerbox2
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/speakerbox2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/stove_top
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/stove_top.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/subfloor1
{
	qer_keyword special
	qer_keyword floor
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/subfloor1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/subfloor1a
{
	qer_keyword wood
	qer_keyword special
	qer_keyword floor
	surfaceparm wood
	{
		map textures/das_boot/subfloor1a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/torp_door1
{
	qer_keyword metal
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/torp_hatch1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/torp_door2
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/torp_hatch2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/torp_door3
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/torp_hatch3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/torp_door4
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/torp_hatch4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/ventsign
{
	qer_keyword special
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/ventsign.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/wood
{
	qer_keyword wall
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/wood.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/iron_floor2a
{
	qer_keyword floor
	qer_keyword rusted
	qer_keyword metal
	surfaceparm metal
	{
		map textures/das_boot/iron_floor2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/pipes3
{
	qer_keyword masked
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	nomipmaps
	qer_editorimage textures/das_boot/pipes3.tga
	//cull none
	{
		map textures/das_boot/pipes3.tga
		alphaFunc GE128
		depthWrite
		nextbundle
		map $lightmap
	}
} 

textures/das_boot/pipes_dasboot
{
	qer_keyword masked
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	cull none
	qer_editorimage textures/das_boot/pipes_dasboot.tga
	{
		map textures/das_boot/pipes_dasboot.tga
		alphaFunc GE128
		depthWrite
		nextbundle
		map $lightmap
	}
} 
 
textures/das_boot/handle
{
	qer_keyword masked
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	cull none
	nomipmaps
	qer_editorimage textures/das_boot/handle.tga
	{
		map textures/das_boot/handle.tga
		alphaFunc GE128
		depthWrite
		nextbundle
		map $lightmap
	}
} 

textures/das_boot/conduit_uboot
{
	qer_keyword masked
	qer_keyword metal
	surfaceparm metal
	surfaceparm fence
	cull none
	nomipmaps
	qer_editorimage textures/das_boot/conduit_uboot.tga
	{
		map textures/das_boot/conduit_uboot.tga
		alphaFunc GE128
		depthWrite
		nextbundle
		map $lightmap
	}
} 

textures/das_boot/atlas-echolot
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/atlas-echolot.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/hatchway1
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/hatchway1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/hatchway1_nonsolid
{
	qer_editorimage textures/das_boot/hatchway1.tga
	qer_keyword special
	surfaceparm metal
	surfaceparm monsterclip
	surfaceparm weaponclip
	{
		map textures/das_boot/hatchway1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/hatch_door1
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/hatch_door1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/ironwall2
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/ironwall2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/ironwall2a
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/ironwall2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/vent_grating
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/vent_grating.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/torp_room_flr
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/torp_room_flr.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/metal_door1
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/metal_door1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/boot_engine_btmflap
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/boot_engine_btmflap.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/boot_engine_cover
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/boot_engine_cover.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/boot_engine_top
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/boot_engine_top.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/fuse_panel_cover
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/fuse_panel_cover.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/fuse_panel2
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/fuse_panel2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/fuse_panel3
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/fuse_panel3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/grey_panel_flat
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/grey_panel_flat.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/sonar_panel
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/sonar_panel.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/stove_front
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/stove_front.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/stove_side
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/stove_side.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/throttle_plate
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/throttle_plate.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/throttle_plate2
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/throttle_plate2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/throttle_plate3
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/throttle_plate3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/throttle_plate4
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/throttle_plate4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/wire_mesh1
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/wire_mesh1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/das_boot/hatchway2
{
	qer_keyword special
	surfaceparm metal
	surfaceparm weaponclip	// needed because of the special way hatches had to be made so the player can crawl through
	surfaceparm monsterclip
	{
		map textures/das_boot/hatchway2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/hatch_door2a
{
	qer_keyword special
	surfaceparm metal
	{
		map textures/das_boot/hatch_door2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/hatch_door2b
{
	qer_keyword special
	surfaceparm metal
	surfaceparm nonsolid	// needed because of the special way hatches had to be made so the player can crawl through
	{
		map textures/das_boot/hatch_door2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/foodcrate_end
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/foodcrate_end.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/foodcrate1
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/foodcrate1.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/foodcrate2a
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/foodcrate2a.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/foodcrate2b
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/foodcrate2b.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/foodcrate3
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/foodcrate3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/das_boot/subphoto2
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto2.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}

}

textures/das_boot/subphoto3
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto3.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}

}

textures/das_boot/subphoto4
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto4.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}

}

textures/das_boot/subphoto5
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto5.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}

}

textures/das_boot/subphoto6
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto6.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}
}

textures/das_boot/subphoto7
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto7.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}

}

textures/das_boot/subphoto8
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto8.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap
	}
}

textures/das_boot/subphoto9
{
	qer_keyword special
	surfaceparm glass
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		blendFunc blend
		depthwrite
	}
	{
		map textures/das_boot/subphoto9.tga
		depthWrite
		rgbGen identity
	   	nextbundle
		map $lightmap

	}
}

textures/das_boot/pipewall
{
	qer_keyword wall
	surfaceparm metal
	{
		map textures/das_boot/pipewall.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/ballastchain
{
	qer_editorimage textures/das_boot/ballastchain.tga
	qer_keyword special
	surfaceparm metal
	surfaceparm fence
	{
		map textures/das_boot/ballastchain.tga
		alphaFunc GE128
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/compressorgauge
{
	qer_keyword wall
	surfaceparm metal
	{
		map textures/das_boot/compressorgauge.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel1
{
	qer_keyword wall
	surfaceparm metal
	{
		map textures/das_boot/miscpanel1.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/throttlebar1
{
	surfaceparm metal
	{
		map textures/das_boot/throttlebar1.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/throttlebar2
{
	surfaceparm metal
	{
		map textures/das_boot/throttlebar2.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/4gauge
{
	surfaceparm metal
	{
		map textures/das_boot/4gauge.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/ventpanel
{
	surfaceparm metal
	{
		map textures/das_boot/ventpanel.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel2
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel2.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel3
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel3.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel4
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel4.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel5
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel5.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel6
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel6.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/miscpanel7
{
	surfaceparm metal
	{
		map textures/das_boot/miscpanel7.tga
		depthWrite
		rgbGen identity
		nextbundle
		map $lightmap
	}
}

textures/das_boot/redwire1
{
	qer_editorimage	textures/das_boot/wire1.tga
	cull none
	{
		map textures/das_boot/wire1.tga
		depthWrite
		rgbGen identity
	nextbundle
		map $lightmap
	}
}

textures/das_boot/pulsating_redwire1
{
	qer_editorimage	textures/das_boot/wire1.tga
	cull none
	{
		map textures/das_boot/wire1.tga
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

textures/das_boot/cabinetwood
{
	qer_keyword wood
	surfaceparm wood
	{
		map textures/das_boot/cabinetwood.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/4gauge
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/4gauge.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/elec_panel5
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/elec_panel5.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/subgridmap
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/subgridmap.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox5a
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox5a.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox3a
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox3a.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox5
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox5.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox2
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox2.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox4
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox4.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox2a
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox2a.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox1
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox1.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/electricbox1a
{
	qer_keyword gauge
	surfaceparm metal
	{
		map textures/das_boot/electricbox1a.tga
		nextbundle
		map $lightmap
	}

}

textures/das_boot/corded_handle
{
	surfaceparm metal
	{
		map textures/das_boot/corded_handle.tga
		nextbundle
		map $lightmap
	}

}
