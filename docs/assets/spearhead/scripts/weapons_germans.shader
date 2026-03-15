fg42
{
	qer_editorimage models/weapons/fg42/fg42.tga
	{
		map models/weapons/fg42/fg42.tga
		rgbGen lightingSpherical
	}
}

fg42cull
{
	qer_editorimage models/weapons/fg42/fg42.tga
	cull none
	{
		map models/weapons/fg42/fg42.tga
		rgbGen lightingSpherical
	}
}

g43
{
	qer_editorimage models/weapons/g43/g43.tga
	{
		map models/weapons/g43/g43.tga
		rgbGen lightingSpherical
	}
}

g43cull
{
	qer_editorimage models/weapons/g43/g43.tga
	cull none
	{
		map models/weapons/g43/g43.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}

nebelhandgranate
{
	qer_editorimage textures/models/weapons/nebelhandgranate.tga
	{
		map textures/models/weapons/nebelhandgranate.tga
		rgbGen lightingSpherical
	}
}

flak88_base
{
	qer_editorimage textures/models/weapons/flak88_base.tga
	{
		map textures/models/weapons/flak88_base.tga
		rgbGen lightingSpherical
	}
}

flak88_top
{
	qer_editorimage textures/models/weapons/flak88_top.tga
	{
		map textures/models/weapons/flak88_top.tga
		rgbGen lightingSpherical
	}
}

flak88_base_s
{
	qer_editorimage textures/models/weapons/flak88_base_s.tga
	{
		map textures/models/weapons/flak88_base_s.tga
		rgbGen lightingSpherical
	}
}

flak88_top_s
{
	qer_editorimage textures/models/weapons/flak88_top_s.tga
	{
		map textures/models/weapons/flak88_top_s.tga
		rgbGen lightingSpherical
	}
}

flak88_seat
{
	qer_editorimage textures/models/weapons/flak88_seat.tga
	{
		map textures/models/weapons/flak88_seat.tga
//		blendfunc blend
		alphafunc ge128
		depthwrite
		rgbGen lightingSpherical
	}
}

flak88_wheel
{
	qer_editorimage textures/models/weapons/flak88_wheel.tga
	{
		map textures/models/weapons/flak88_wheel.tga
//		blendfunc blend
		alphafunc ge128
		depthwrite		       
		rgbGen lightingSpherical
	}
}

germ_smoke_gren
{
	qer_editorimage textures/models/weapons/german_smoke_grenade.tga
	{
		map textures/models/weapons/german_smoke_grenade.tga
		rgbGen lightingSpherical
	}
}

granatwerfer
{
	qer_editorimage textures/models/weapons/granatwerfer_34.tga
	{
		map textures/models/weapons/granatwerfer_34.tga
		rgbGen lightingSpherical
	}
}

granatwerfer_s
{
	qer_editorimage textures/models/weapons/granatwerfer_34_s.tga
	{
		map textures/models/weapons/granatwerfer_34_s.tga
		rgbGen lightingSpherical
	}
}

granatwerfer_d
{
	qer_editorimage textures/models/weapons/granatwerfer_34_d.tga
	{
		map textures/models/weapons/granatwerfer_34_d.tga
		rgbGen lightingSpherical
	}
}


// Panzerwerfer 42

pzw_body
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/panzerwerfer_42.tga
	{
		map textures/models/vehicles/panzerwerfer42/panzerwerfer_42.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire1
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_front_tire.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_front_tire.tga
		tcmod rotate -120
		rgbGen lightingSpherical
	}
}
pzw42_tread1
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_ftiretread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_ftiretread.tga
		tcmod scroll -7.4 0
		rgbGen lightingSpherical
	}
}
pzw42_tread2
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread.tga
		tcmod scroll 0 1.7
		rgbGen lightingSpherical
	}
}
pzw42_treadedge
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread_edge.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread_edge.tga
		tcmod scroll 0 1.7
		rgbGen lightingSpherical
	}
}
pzw42_teeth
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_teeth.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_teeth.tga
		depthwrite
		alphafunc GE128
		tcmod scroll 0 2
		rgbGen lightingSpherical
	}
}
pzw42_tire2
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire2.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire2.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -120
		rgbGen lightingSpherical
	}
}
pzw42_tread3
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_rtiretread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_rtiretread.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire3
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire1.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire1.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -160
		rgbGen lightingSpherical
	}
}
pzw42_tire4
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire3.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire3.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -100
		rgbGen lightingSpherical
	}
}
pzw42_tire5
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire4.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire4.tga
		tcmod rotate -360
		rgbGen lightingSpherical
	}
}
pzw42_turret
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_turret.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_turret.tga
		rgbGen lightingSpherical
	}
}
pzw42_suspen
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_suspension.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_suspension.tga
		depthwrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Panzerwerfer 42 snowy

pzw_body_s
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/panzerwerfer_42_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/panzerwerfer_42_s.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire1_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_front_tire_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_front_tire_s.tga
		tcmod rotate -120
		rgbGen lightingSpherical
	}
}
pzw42_tread1_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_ftiretread_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_ftiretread_s.tga
		tcmod scroll -7.4 0
		rgbGen lightingSpherical
	}
}
pzw42_tread2_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread_s.tga
		tcmod scroll 0 1.7
		rgbGen lightingSpherical
	}
}
pzw42_treadedge_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread_edge_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread_edge_s.tga
		tcmod scroll 0 1.7
		rgbGen lightingSpherical
	}
}
pzw42_tire2_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire2_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire2_s.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -120
		rgbGen lightingSpherical
	}
}
pzw42_tread3_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_rtiretread_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_rtiretread_s.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire3_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire1_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire1_s.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -160
		rgbGen lightingSpherical
	}
}
pzw42_tire4_s
{
	nomerge
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire3_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire3_s.tga
		depthwrite
		alphafunc GE128
		tcmod rotate -100
		rgbGen lightingSpherical
	}
}

pzw42_turret_s
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_turret_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_turret_s.tga
		rgbGen lightingSpherical
	}
}
pzw42_suspen_s
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_suspension_s.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_suspension_s.tga
		depthwrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}


// Panzerwerfer 42 Destroyed
pzw_body_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/panzerwerfer_42_d.tga
	{
		map textures/models/vehicles/panzerwerfer42/panzerwerfer_42_d.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire1_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_front_tire.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_front_tire.tga
		rgbGen lightingSpherical
	}
}
pzw42_tread1_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_ftiretread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_ftiretread.tga
		rgbGen lightingSpherical
	}
}
pzw42_tread2_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread.tga
		rgbGen lightingSpherical
	}
}
pzw42_treadedge_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_tread_edge.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_tread_edge.tga
		rgbGen lightingSpherical
	}
}
pzw42_teeth_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_teeth.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_teeth.tga
		depthwrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pzw42_tire2_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire2.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire2.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pzw42_tread3_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_rtiretread.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_rtiretread.tga
		rgbGen lightingSpherical
	}
}
pzw42_tire3_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire1.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire1.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pzw42_tire4_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire3.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire3.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pzw42_tire5_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_back_tire4.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_back_tire4.tga
		rgbGen lightingSpherical
	}
}
pzw42_turret_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_turret_d.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_turret_d.tga
		rgbGen lightingSpherical
	}
}
pzw42_suspen_d
{
	qer_editorimage textures/models/vehicles/panzerwerfer42/pzw42_suspension.tga
	{
		map textures/models/vehicles/panzerwerfer42/pzw42_suspension.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// 20mm flak winter
20mmflakwinter
{
	qer_editorimage models/statweapons/20mmflak/20mmflakwinter.jpg
	{
		map models/statweapons/20mmflak/20mmflakwinter.jpg
		rgbGen lightingSpherical
	}
}
20mmflakwinter_d
{
	qer_editorimage models/statweapons/20mmflak/20mmflakwinter_d.jpg
	{
		map models/statweapons/20mmflak/20mmflakwinter_d.jpg
		rgbGen lightingSpherical
	}
}
