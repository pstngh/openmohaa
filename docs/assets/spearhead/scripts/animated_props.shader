//A-frame Phone Pole
phone_pole_wood
{
	qer_editorimage textures/models/phone_pole/telephone_pole_wood_dark.tga
	{
		map textures/models/phone_pole/telephone_pole_wood_dark.tga
		rgbGen lightingSpherical
	}
}
phone_pole_insulator
{
	qer_editorimage textures/models/phone_pole/telephone_pole_insulator.tga
	{
		map textures/models/phone_pole/telephone_pole_insulator.tga
		rgbGen lightingSpherical
	}
}
phone_pole_wood_dark
{
	qer_editorimage textures/models/phone_pole/telephone_pole_wood_light.tga
	{
		map textures/models/phone_pole/telephone_pole_wood_light.tga
		rgbGen lightingSpherical
	}
}
phone_pole_wires
{
	qer_editorimage textures/models/phone_pole/telephone_pole_wire_snow.tga
	{
		map textures/models/phone_pole/telephone_pole_wire_snow.tga
		rgbGen lightingSpherical
	}
}

// 1_3 row boat
rowboat_planks
{
	qer_editorimage textures/models/animate/1_3_Rowboat/planks.tga
	cull none
	{
		map textures/models/animate/1_3_Rowboat/planks.tga
		rgbGen lightingSpherical
	}
}
rowboat_ring
{
	qer_editorimage textures/models/animate/1_3_Rowboat/ring.tga
	cull none
	{
		map textures/models/animate/1_3_Rowboat/ring.tga
//		blendfunc blend 
		rgbGen lightingSpherical
	}
}

// BMW motorcycle
bmw_wheel
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_wheel.tga
	{
		map textures/models/animate/1_3_bmw/bmw_wheel.tga
		rgbGen lightingSpherical
	}
}
bmw_wheel2
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_wheel1.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/bmw_wheel1.tga
//		blendfunc blend
		alphafunc GE128
		depthwrite
		rgbGen lightingSpherical
	}
}

bmw_body
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_body.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/bmw_body.tga
		rgbGen lightingSpherical
	}
}
bmw_handlebars
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_handlebars.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/bmw_handlebars.tga
		rgbGen lightingSpherical
	}
}
bmw_flic
{
	qer_editorimage textures/models/animate/1_3_bmw/lic1.tga
//	cull none
	{
		map textures/models/animate/1_3_bmw/lic1.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}
bmw_blic
{
	qer_editorimage textures/models/animate/1_3_bmw/mc_back_plate.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/mc_back_plate.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}
bmw_disc
{
	qer_editorimage textures/models/animate/1_3_bmw/wheel_disc.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/wheel_disc.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

// BMW motorcycle snow
bmw_wheel_s
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_wheel_s.tga
	{
		map textures/models/animate/1_3_bmw/bmw_wheel_s.tga
		rgbGen lightingSpherical
	}
}

bmw_body_s
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_body_s.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/bmw_body_s.tga
		rgbGen lightingSpherical
	}
}
bmw_handlebars_s
{
	qer_editorimage textures/models/animate/1_3_bmw/bmw_handlebars_s.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/bmw_handlebars_s.tga
		rgbGen lightingSpherical
	}
}

bmw_disc_s
{
	qer_editorimage textures/models/animate/1_3_bmw/wheel_disc_s.tga
	cull none
	{
		map textures/models/animate/1_3_bmw/wheel_disc_s.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

// 3_1 Safe
safe_main
{
	qer_editorimage textures/models/animate/3_1_Safe/safe.tga
	{
		map textures/models/animate/3_1_Safe/safe.tga
		rgbGen lightingSpherical
	}
}

safe_dial_pulse
{
	{
		map textures/models/animate/3_1_Safe/safe.tga
		rgbGen lightingSpherical
		blendfunc gl_src_alpha gl_one
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

safe_docs_pulse
{
	qer_editorimage textures/models/animate/3_1_Safe/safe_docs.tga
	{
		map textures/models/animate/3_1_Safe/safe_docs.tga
		rgbGen lightingSpherical
		blendfunc gl_src_alpha gl_one
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Paratrooper Billboard
paratrooper_chute
{
	qer_editorimage textures/models/animate/Paratrooper_billboard/Parachute.tga
	cull none
	{
		map textures/models/animate/Paratrooper_billboard/Parachute.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
paratrooper
{
	qer_editorimage textures/models/animate/Paratrooper_billboard/paratrooper.tga
	{
		map textures/models/animate/Paratrooper_billboard/paratrooper.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
paratrooper_cords
{
	qer_editorimage textures/models/animate/Paratrooper_billboard/ConeCord.tga
	cull none
	{
		map textures/models/animate/Paratrooper_billboard/ConeCord.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
// Lamp_post_damaged
post_d
{
	qer_editorimage textures/models/animate/Lamp_d/t_postD.tga
	{
		map textures/models/animate/Lamp_d/t_postD.tga
		rgbGen lightingSpherical
	}
}
support_d
{
	qer_editorimage textures/models/animate/Lamp_d/supportD.tga
	cull none
	{
		map textures/models/animate/Lamp_d/supportD.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
lantern_d
{
	qer_editorimage textures/models/animate/Lamp_d/lanternD.tga
	cull none
	{
		map textures/models/animate/Lamp_d/lanternD.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

opel_cover2
{
	qer_editorimage textures/models/vehicles/opel_w/cover2.tga
	{
		map textures/models/vehicles/opel_w/cover2.tga
		rgbGen lightingSpherical
	}
}

