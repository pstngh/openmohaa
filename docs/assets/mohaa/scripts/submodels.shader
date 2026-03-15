// sub bunkbed
static_sub_bunk
{
	qer_editorimage textures/models/submodels/sub_bunk.tga
	{
		map textures/models/submodels/sub_bunk.tga
		rgbGen static
	}
}

// subradio1
static_subradio1
{
	qer_editorimage textures/models/submodels/subradio1.tga
	{
		map textures/models/submodels/subradio1.tga
		rgbGen static
	}
}

// black equipment panel t-handle
static_subknob1
{
	qer_editorimage textures/models/submodels/subknob1.tga
	{
		map textures/models/submodels/subknob1.tga
		rgbGen static
	}
}

// tan power junction box
static_tanpower1
{
	qer_editorimage textures/models/submodels/tanpower1.tga
	{
		map textures/models/submodels/tanpower1.tga
		rgbGen static
	}
}

// tan power junction box 2
static_tanpower2
{
	qer_editorimage textures/models/submodels/tanpower2.tga
	{
		map textures/models/submodels/tanpower2.tga
		rgbGen static
	}
}

// tan power junction box 3
static_tanpower3
{
	qer_editorimage textures/models/submodels/tanpower3.tga
	{
		map textures/models/submodels/tanpower3.tga
		rgbGen static
	}
}

// engine cover tank thingie
static_enginecover
{
	qer_editorimage textures/models/submodels/engine_cover.tga
	{
		map textures/models/submodels/engine_cover.tga
		rgbGen static
	}
}

// grey speakerbox
static_speakerbox1
{
	qer_editorimage textures/models/submodels/speakerbox1.tga
	{
		map textures/models/submodels/speakerbox1.tga
		rgbGen static
	}
}

// atlas-echolot
static_atlas-echolot
{
	qer_editorimage textures/models/submodels/atlas-echolot.tga
	{
		map textures/das_boot/sub_envmap.tga
		tcgen environmentmodel
		rgbGen static
	}
	{
		map textures/models/submodels/atlas-echolot.tga
		blendFunc blend
		rgbGen static
	}
}

//helmsmans stool for bridge

static_substool
{
	qer_editorimage textures/models/submodels/substool.tga
	{
		map textures/models/submodels/substool.tga
		rgbGen static
	}
}

//subvalve red spokes-black rim
static_subvalve-red-black
{
	qer_editorimage textures/models/submodels/valve_red.tga
	{
		map textures/models/submodels/valve_red.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

redvalve_pulse
{
	qer_editorimage textures/models/submodels/valve_red.tga
	{
		map textures/models/submodels/valve_red.tga
		alphaFunc GE128
		depthwrite
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// tiefenmesser gauge
static_tiefenmesser
{
	nomipmaps
	qer_editorimage textures/models/submodels/tiefenmesser.tga
	{
		map textures/models/submodels/tiefenmesser.tga
		rgbGen static
	}
}

// black gauge side
static_blackgauge
{
	qer_editorimage textures/models/submodels/blackgauge.tga
	{
		map textures/models/submodels/blackgauge.tga
		rgbGen static
	}
}

///////////////////////////
//environment mapped glass for gauges
///////////////////////////


static_glass
{
//cull none
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		alphagen const .20
		depthwrite
		blendFunc blend
	}
}
///////////////////////////
//environment mapped glass for gauges with overlay
///////////////////////////

static_dieselglass
{
//cull none
	{
		map textures/models/submodels/sub_env.tga
		tcgen environmentmodel
		alphagen const .20
		blendFunc blend
	     //rgbGen static
	}
	{
		map textures/models/submodels/dieselglass.tga
		blendFunc blend
	      rgbGen static
	}

}


///////////////////////////
//environment mapped glass for depth barometer
///////////////////////////


static_baroglass
{
//cull none
	{
		map textures/models/submodels/sub_env.tga
		tcgen environment
		alphagen const .35
		depthwrite
		blendFunc blend
	}
}

// bridge depth barometer
static_barometer
{
	//cull none
	qer_editorimage textures/models/submodels/barometer.tga
	{
		map textures/models/submodels/barometer.tga
		rgbGen static
	}
}
//////////////////////////////////////
//periscope textures
//////////////////////////////////////

// main periscope body

static_scopebody
{
	qer_editorimage textures/models/submodels/scopebody.tga
	{
		map textures/models/submodels/scopebody.tga
		rgbGen static
	}
}

// periscope faceplate
static_scopeface
{
	qer_editorimage textures/models/submodels/scopeface.tga
	{
		map textures/models/submodels/scopeface.tga
		rgbGen static
	}
}

// brushed aluminum bottom

static_subalum
{
	qer_editorimage textures/models/submodels/subalum.tga
	{
		map textures/models/submodels/subalum.tga
		rgbGen static
	}
}

// misc greeble

static_subgreeble
{
	qer_editorimage textures/models/submodels/miscpiece.tga
	{
		map textures/models/submodels/miscpiece.tga
		blendFunc blend
		rgbGen static
	}
}

//diesel rpm gauge

static_dieselrpm

{
	qer_editorimage textures/models/submodels/dieselrpm.tga
	{
		map textures/models/submodels/dieselrpm.tga
		rgbGen static
	}
}

//subvalve black1
static_valve_black1
{
	qer_editorimage textures/models/submodels/valve_black1.tga
	{
		map textures/models/submodels/valve_black1.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

//subvalve black2
static_valve_black2
{
	qer_editorimage textures/models/submodels/valve_black2.tga
	{
		map textures/models/submodels/valve_black2.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}


//subvalve black3
static_valve_black3
{
	qer_editorimage textures/models/submodels/valve_black3.tga
	{
		map textures/models/submodels/valve_black3.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

//subvalve grey1
static_valve_grey1
{
	qer_editorimage textures/models/submodels/valve_grey1.tga
	{
		map textures/models/submodels/valve_grey1.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

//subvalve grey2
static_valve_grey2
{
	qer_editorimage textures/models/submodels/valve_grey2.tga
	{
		map textures/models/submodels/valve_grey2.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

/////////////////////////////////
//static torpedo shaders
/////////////////////////////////

//torpedo body
static_torpedobody
{
	qer_editorimage textures/models/submodels/torpedobody.tga
	{
		map textures/models/submodels/torpedobody.tga
		rgbGen static
	}
}

//torpedo tail
static_torpedotail
{
	qer_editorimage textures/models/submodels/torpedotail.tga
	{
		map textures/models/submodels/torpedotail.tga
		rgbGen static
	}
}

//torpedo nose
static_torpedonose
{
	qer_editorimage textures/models/submodels/torpedonose.tga
	{
		map textures/models/submodels/torpedonose.tga
		rgbGen static
	}
}

//torpedo tip
static_torpedo_tip
{
	qer_editorimage textures/models/submodels/torpedo_tip.tga
	{
		map textures/models/submodels/torpedo_tip.tga
		rgbGen static
	}
}

//torpedo prop
static_torpedoprop
{
	cull none
	qer_editorimage textures/models/submodels/torpedoprop.tga
	{
		map textures/models/submodels/torpedoprop.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}

/////////////////////////////////
//non-static torpedo shaders
/////////////////////////////////

//torpedo body
torpedobody
{
	qer_editorimage textures/models/submodels/torpedobody.tga
	{
		map textures/models/submodels/torpedobody.tga
		rgbGen lightingSpherical
	}
}

//torpedo tail
torpedotail
{
	qer_editorimage textures/models/submodels/torpedotail.tga
	{
		map textures/models/submodels/torpedotail.tga
		rgbGen lightingSpherical
	}
}

//torpedo nose
torpedonose
{
	qer_editorimage textures/models/submodels/torpedonose.tga
	{
		map textures/models/submodels/torpedonose.tga
		rgbGen lightingSpherical
	}
}

//torpedo tip
torpedo_tip
{
	qer_editorimage textures/models/submodels/torpedo_tip.tga
	{
		map textures/models/submodels/torpedo_tip.tga
		rgbGen lightingSpherical
	}
}

//torpedo prop
torpedoprop
{
	cull none
	qer_editorimage textures/models/submodels/torpedoprop.tga
	{
		map textures/models/submodels/torpedoprop.tga
		alphaFunc GE128
		depthwrite
		rgbGen lightingSpherical
	}
}

/////////////////////////////////
// planesman control wheel
/////////////////////////////////

static_helmcontrol
{
	qer_editorimage textures/models/submodels/helmwheel.tga
	{
		map textures/models/submodels/helmwheel.tga
		rgbGen static
	}
}

//////////////////////////////////////
// enigma machine textures
//////////////////////////////////////

static_enigma
{
	qer_editorimage textures/models/submodels/enigma.tga
	{
		map textures/models/submodels/enigma.tga
		rgbGen static
	}
}

static_enigmatext
{
	cull none
	qer_editorimage textures/models/submodels/enigmatext.tga
	{
		map textures/models/submodels/enigmatext.tga
		alphaFunc GE128
		depthwrite
		rgbGen static
	}
}
/////////////////////////////////////
// for cluster of valves on bridge
/////////////////////////////////////

static_valvepipes
{
	qer_editorimage textures/models/submodels/valvepipes.tga
	{
		map textures/models/submodels/valvepipes.tga
		rgbGen static
	}
}

static_valvetree
{
	qer_editorimage textures/models/submodels/valvetree.tga
	{
		map textures/models/submodels/valvetree.tga
		rgbGen static
	}
}

static_e-light
{
	qer_editorimage textures/models/submodels/e-light.tga
	{
		map textures/models/submodels/e-light.tga
		rgbGen static
	}
}

static_bigswitch1
{
	qer_editorimage textures/models/submodels/bigswitch1.tga
	{
		map textures/models/submodels/bigswitch1.tga
		rgbGen static
	}
}

static_bigswitch2
{
	qer_editorimage textures/models/submodels/bigswitch2.tga
	{
		map textures/models/submodels/bigswitch2.tga
		rgbGen static
	}
}

static_6lamp
{
	qer_editorimage textures/models/submodels/6lamp.tga
	{
		map textures/models/submodels/6lamp.tga
		rgbGen static
	}
}

static_telegraphbody1
{
	qer_editorimage textures/models/submodels/telegraphbody1.tga
	{
		map textures/models/submodels/telegraphbody1.tga
		rgbGen static
	}
}

static_telegraphbody2
{
	qer_editorimage textures/models/submodels/telegraphbody2.tga
	{
		map textures/models/submodels/telegraphbody2.tga
		rgbGen static
	}
}

static_telegraphface1
{
	qer_editorimage textures/models/submodels/telegraphface1.tga
	{
		map textures/models/submodels/telegraphface1.tga
		rgbGen static
	}
}

static_telegraphface2
{
	qer_editorimage textures/models/submodels/telegraphface2.tga
	{
		map textures/models/submodels/telegraphface2.tga
		rgbGen static
	}
}

static_electricbox1
{
	qer_editorimage textures/models/submodels/electricbox1.tga
	{
		map textures/models/submodels/electricbox1.tga
		rgbGen static
	}
}

//////////////////////////////////
//gyrocompass gauge face
//////////////////////////////////

static_gyrocompass
{
	qer_editorimage textures/models/submodels/gyrocompass.tga
	{
		map textures/models/submodels/gyrocompass.tga
		rgbGen static
	}
}
///////////////////////////////////
//ruddergauge
///////////////////////////////////

static_ruddergaugebody
{
	qer_editorimage textures/models/submodels/ruddergaugebody.tga
	{
		map textures/models/submodels/ruddergaugebody.tga
		rgbGen static
	}
}

static_ruddergauge1
{
	qer_editorimage textures/models/submodels/ruddergauge1.tga
	{
		map textures/models/submodels/ruddergauge1.tga
		rgbGen static
	}
}

static_ruddergauge2
{
	qer_editorimage textures/models/submodels/ruddergauge2.tga
	{
		map textures/models/submodels/ruddergauge2.tga
		rgbGen static
	}
}
///////////////////////////////
// engine manifold temperature gauge
///////////////////////////////

static_tempgauge
{
	qer_editorimage textures/models/submodels/tempgauge.tga
	{
		map textures/models/submodels/tempgauge.tga
		rgbGen static
	}
}

static_castiron
{
	qer_editorimage textures/models/submodels/castiron.tga
	{
		map textures/models/submodels/castiron.tga
		rgbGen static
	}
}
///////////////////////////////////////////
//airtank textures
///////////////////////////////////////////

static_yellowtank
{
	qer_editorimage textures/models/submodels/yellowtank.tga
	{
		map textures/models/submodels/yellowtank.tga
		rgbGen static
	}
}

static_tanktop
{
	qer_editorimage textures/models/submodels/tanktop.tga
	{
		map textures/models/submodels/tanktop.tga
		rgbGen static
	}
}

////////////////////////////////////
//non static sub model textures
////////////////////////////////////

deckcannon
{
	qer_editorimage textures/models/submodels/deckcannon.tga
	{
		map textures/models/submodels/deckcannon.tga
		rgbGen lightingSpherical
	}
}

deckflak
{
	qer_editorimage textures/models/submodels/deckflak.tga
	{
		map textures/models/submodels/deckflak.tga
		rgbGen lightingSpherical
	}
}

deckguntwins
{
	qer_editorimage textures/models/submodels/deckguntwins.tga
	{
		map textures/models/submodels/deckguntwins.tga
		rgbGen lightingSpherical
	}
}

periscopes
{
	qer_editorimage textures/models/submodels/periscopes.tga
	{
		map textures/models/submodels/periscopes.tga
		rgbGen lightingSpherical
	}
}

subrailing
{
	cull none
	qer_editorimage textures/models/submodels/subrailing.tga
	{
		map textures/models/submodels/subrailing.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

wooddeck
{
	qer_editorimage textures/models/submodels/wooddeck.tga
	{
		map textures/models/submodels/wooddeck.tga
		rgbGen lightingSpherical
	}
}

hulltop1a
{
	qer_editorimage textures/models/submodels/hulltop1a.tga
	cull none
	{
		map textures/models/submodels/hulltop1a.tga
		rgbGen lightingSpherical
	}
}

hulltop1c
{
	qer_editorimage textures/models/submodels/hulltop1c.tga
	cull none
	{
		map textures/models/submodels/hulltop1c.tga
		rgbGen lightingSpherical
	}
}

hulltop1d
{
	qer_editorimage textures/models/submodels/hulltop1d.tga
	cull none
	{
		map textures/models/submodels/hulltop1d.tga
		rgbGen lightingSpherical
	}
}

hulltop1h
{
	qer_editorimage textures/models/submodels/hulltop1h.tga
	cull none
	{
		map textures/models/submodels/hulltop1h.tga
		rgbGen lightingSpherical
	}
}

hulltop1e
{
	qer_editorimage textures/models/submodels/hulltop1e.tga
	cull none
	{
		map textures/models/submodels/hulltop1e.tga
		rgbGen lightingSpherical
	}
}

hulltop1i
{
	qer_editorimage textures/models/submodels/hulltop1i.tga
	cull none
	{
		map textures/models/submodels/hulltop1i.tga
		rgbGen lightingSpherical
	}
}

hulltop1j
{
	qer_editorimage textures/models/submodels/hulltop1j.tga
	cull none
	{
		map textures/models/submodels/hulltop1j.tga
		rgbGen lightingSpherical
	}
}

blackhull1
{
	qer_editorimage textures/models/submodels/blackhull1.tga
	cull none
	{
		map textures/models/submodels/blackhull1.tga
		rgbGen lightingSpherical
	}
}

metaldeck_plating
{
	qer_editorimage textures/models/submodels/metaldeck_plating.tga
	cull none
	{
		map textures/models/submodels/metaldeck_plating.tga
		rgbGen lightingSpherical
	}
}

contowerlamp
{
	qer_editorimage textures/models/submodels/contowerlamp.tga
	{
		map textures/models/submodels/contowerlamp.tga
		rgbGen lightingSpherical
	}
}

contowerwood
{
	qer_editorimage textures/models/submodels/contowerwood.tga
	{
		map textures/models/submodels/contowerwood.tga
		rgbGen lightingSpherical
	}
}

miscpieces
{
	qer_editorimage textures/models/submodels/miscpieces.tga
	{
		map textures/models/submodels/miscpieces.tga
		depthwrite
		blendFunc blend
		rgbGen lightingSpherical
	}
} 
//miscellaneous pieces for the hatch explosion effect

miscpieces2
{
	qer_editorimage textures/models/submodels/miscpieces.tga
	{
		map textures/models/submodels/miscpieces.tga
		rgbGen lightingSpherical
	}
}

////////////////////////////////////
//static sub model textures
////////////////////////////////////

static_deckcannon
{
	qer_editorimage textures/models/submodels/deckcannon.tga
	{
		map textures/models/submodels/deckcannon.tga
		rgbGen static
	}
}

static_deckflak
{
	qer_editorimage textures/models/submodels/deckflak.tga
	{
		map textures/models/submodels/deckflak.tga
		rgbGen static
	}
}

static_deckguntwins
{
	qer_editorimage textures/models/submodels/deckguntwins.tga
	{
		map textures/models/submodels/deckguntwins.tga
		rgbGen static
	}
}

static_periscopes
{
	qer_editorimage textures/models/submodels/periscopes.tga
	{
		map textures/models/submodels/periscopes.tga
		rgbGen static
	}
}

static_subrailing
{
	cull none
	qer_editorimage textures/models/submodels/subrailing.tga
	{
		map textures/models/submodels/subrailing.tga
		alphafunc GE128
		rgbGen static
	}
}

static_wooddeck
{
	qer_editorimage textures/models/submodels/wooddeck.tga
	{
		map textures/models/submodels/wooddeck.tga
		rgbGen static
	}
}

static_hulltop1a
{
	qer_editorimage textures/models/submodels/hulltop1a.tga
	{
		map textures/models/submodels/hulltop1a.tga
		rgbGen static
	}
}

static_hulltop1c
{
	qer_editorimage textures/models/submodels/hulltop1c.tga
	{
		map textures/models/submodels/hulltop1c.tga
		rgbGen static
	}
}

static_hulltop1d
{
	qer_editorimage textures/models/submodels/hulltop1d.tga
	{
		map textures/models/submodels/hulltop1d.tga
		rgbGen static
	}
}

static_hulltop1h
{
	qer_editorimage textures/models/submodels/hulltop1h.tga
	{
		map textures/models/submodels/hulltop1h.tga
		rgbGen static
	}
}

static_hulltop1e
{
	qer_editorimage textures/models/submodels/hulltop1e.tga
	{
		map textures/models/submodels/hulltop1e.tga
		rgbGen static
	}
}

static_hulltop1i
{
	qer_editorimage textures/models/submodels/hulltop1i.tga
	{
		map textures/models/submodels/hulltop1i.tga
		rgbGen static
	}
}

static_hulltop1j
{
	qer_editorimage textures/models/submodels/hulltop1j.tga
	{
		map textures/models/submodels/hulltop1j.tga
		rgbGen static
	}
}

static_blackhull1
{
	qer_editorimage textures/models/submodels/blackhull1.tga
	{
		map textures/models/submodels/blackhull1.tga
		rgbGen static
	}
}

static_metaldeck_plating
{
	qer_editorimage textures/models/submodels/metaldeck_plating.tga
	{
		map textures/models/submodels/metaldeck_plating.tga
		rgbGen static
	}
}

static_contowerlamp
{
	qer_editorimage textures/models/submodels/contowerlamp.tga
	{
		map textures/models/submodels/contowerlamp.tga
		rgbGen static
	}
}

static_contowerwood
{
	qer_editorimage textures/models/submodels/contowerwood.tga
	{
		map textures/models/submodels/contowerwood.tga
		rgbGen static
	}
}

static_miscpieces
{
	qer_editorimage textures/models/submodels/miscpieces.tga
	{
		map textures/models/submodels/miscpieces.tga
		//depthwrite
		//blendFunc blend
		rgbGen static
	}
}

static_subhatch
{
	qer_editorimage textures/models/submodels/subhatch.tga
	{
		map textures/models/submodels/subhatch.tga
		depthwrite
		blendFunc blend
		rgbGen static
	}
}

static_flakshield
{
	cull none
	qer_editorimage textures/models/submodels/deckflak.tga
	{
		map textures/models/submodels/deckflak.tga
		rgbGen static
	}
}

static_subradar
{
	nopicmip
	nomipmaps
	cull none
	qer_editorimage textures/models/submodels/miscpieces.tga
	{
		map textures/models/submodels/miscpieces.tga
		depthwrite
		blendFunc blend
		rgbGen static
	}
}

/////////////////////////////////////
//subcrane skin
/////////////////////////////////////

subcrane
{
	qer_editorimage textures/models/submodels/subcrane.tga
	{
		map textures/models/submodels/subcrane.tga
		rgbGen lightingSpherical
	}
}

/////////////////////////////////////
//sub emblems
/////////////////////////////////////

eagle
{
	qer_editorimage textures/models/submodels/eagle.tga
	{
		map textures/models/submodels/eagle.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_eagle
{
	qer_editorimage textures/models/submodels/eagle.tga
	{
		map textures/models/submodels/eagle.tga
		blendfunc blend
		rgbGen static
	}
}

flotilla11
{
	qer_editorimage textures/models/submodels/flotilla11.tga
	{
		map textures/models/submodels/flotilla11.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_flotilla11
{
	qer_editorimage textures/models/submodels/flotilla11.tga
	{
		map textures/models/submodels/flotilla11.tga
		blendfunc blend
		rgbGen static
	}
}

flotilla13
{
	qer_editorimage textures/models/submodels/flotilla13.tga
	{
		map textures/models/submodels/flotilla13.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_flotilla13
{
	qer_editorimage textures/models/submodels/flotilla13.tga
	{
		map textures/models/submodels/flotilla13.tga
		blendfunc blend
		rgbGen static
	}
}

flotilla7
{
	qer_editorimage textures/models/submodels/flotilla7.tga
	{
		map textures/models/submodels/flotilla7.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_flotilla7
{
	qer_editorimage textures/models/submodels/flotilla7.tga
	{
		map textures/models/submodels/flotilla7.tga
		blendfunc blend
		rgbGen static
	}
}

reddevil
{
	qer_editorimage textures/models/submodels/reddevil.tga
	{
		map textures/models/submodels/reddevil.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_reddevil
{
	qer_editorimage textures/models/submodels/reddevil.tga
	{
		map textures/models/submodels/reddevil.tga
		blendfunc blend
		rgbGen static
	}
}

swordfish
{
	qer_editorimage textures/models/submodels/swordfish.tga
	{
		map textures/models/submodels/swordfish.tga
		blendfunc blend
		rgbGen lightingSpherical
	}
}

static_swordfish
{
	qer_editorimage textures/models/submodels/swordfish.tga
	{
		map textures/models/submodels/swordfish.tga
		blendfunc blend
		rgbGen static
	}
}

static_gaswelder
{
	qer_editorimage textures/models/norway/gaswelder.tga
	{
		map textures/models/norway/gaswelder.tga
		rgbGen vertex
	}
}

gaswelder
{
	qer_editorimage textures/models/norway/gaswelder.tga
	{
		map textures/models/norway/gaswelder.tga
		rgbGen vertex
	}
}

///////////////////////////////////////////
// welder shaders for guys welding on sub
///////////////////////////////////////////

tanktop
{
	qer_editorimage textures/models/submodels/tanktop.tga
	{
		map textures/models/submodels/tanktop.tga
		rgbGen lightingSpherical
	}
}

static_tanktop
{
	qer_editorimage textures/models/submodels/tanktop.tga
	{
		map textures/models/submodels/tanktop.tga
		rgbGen static
	}
}

tanks
{
	qer_editorimage textures/models/submodels/tanks.tga
	{
		map textures/models/submodels/tanks.tga
		rgbGen lightingSpherical
	}
}

static_tanks
{
	qer_editorimage textures/models/submodels/tanks.tga
	{
		map textures/models/submodels/tanks.tga
		rgbGen static
	}
}

wheel
{
	qer_editorimage textures/models/submodels/wheel.tga
	cull none
	{
		map textures/models/submodels/wheel.tga
		rgbGen lightingSpherical
	}
}

static_wheel
{
	qer_editorimage textures/models/submodels/wheel.tga
	cull none
	{
		map textures/models/submodels/wheel.tga
		rgbGen static
	}
}

welder
{
	qer_editorimage textures/models/submodels/welder.tga
	{
		map textures/models/submodels/welder.tga
		rgbGen lightingSpherical
	}
}

static_welder
{
	qer_editorimage textures/models/submodels/welder.tga
	{
		map textures/models/submodels/welder.tga
		rgbGen static
	}
}

redhose
{
	qer_editorimage textures/models/submodels/redhose.tga
	{
		map textures/models/submodels/redhose.tga
		rgbGen lightingSpherical
	}
}

static_redhose
{
	qer_editorimage textures/models/submodels/redhose.tga
	{
		map textures/models/submodels/redhose.tga
		rgbGen static
	}
}

greenhose
{
	qer_editorimage textures/models/submodels/greenhose.tga
	{
		map textures/models/submodels/greenhose.tga
		rgbGen lightingSpherical
	}
}

static_greenhose
{
	qer_editorimage textures/models/submodels/greenhose.tga
	{
		map textures/models/submodels/greenhose.tga
		rgbGen static
	}
}

/////////////////////////////////////
//welding mask and torch
/////////////////////////////////////

weldermask
{
	qer_editorimage textures/models/submodels/weldingmask.tga
		cull none
	{
		map textures/models/submodels/weldingmask.tga
		rgbGen lightingSpherical
	}
}

weldertorch
{
	qer_editorimage textures/models/submodels/weldertorch.tga
		cull none
	{
		map textures/models/submodels/weldertorch.tga
		rgbGen lightingSpherical
	}
}

static_weldertorch
{
	qer_editorimage textures/models/submodels/weldertorch.tga
		cull none
	{
		map textures/models/submodels/weldertorch.tga
		rgbGen static
	}
}
/////////////////////////////////////////////
//naxos radar skins
/////////////////////////////////////////////

naxosradar
{
	qer_editorimage textures/models/submodels/naxos_radar.tga
	cull none
	{
		map textures/models/submodels/naxos_radar.tga
		rgbGen lightingSpherical
	}
}

naxosradar_pulsating
{
	qer_editorimage textures/models/submodels/naxos_radar.tga
	cull none
	{
		map textures/models/submodels/naxos_radar.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

naxosantenna
{
	qer_editorimage textures/models/submodels/naxos_antenna.tga
		cull none
		nopicmip
		nomipmaps
	{
		map textures/models/submodels/naxos_antenna.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

naxosantenna_pulsating
{
	qer_editorimage textures/models/submodels/naxos_antenna.tga
		cull none
		nopicmip
		nomipmaps
	{
		map textures/models/submodels/naxos_antenna.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

naxosinside
{
	qer_editorimage textures/models/submodels/naxosinside.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/naxosinside.tga
		rgbGen lightingSpherical
	}
}

static_bulkheadhatch
{
	qer_editorimage textures/models/submodels/bulkheadhatch.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/bulkheadhatch.tga
		rgbGen static
	}
}

static_powerpanel
{
	qer_editorimage textures/models/submodels/powerpanel.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/powerpanel.tga
		blendfunc blend
		depthwrite
		rgbGen static
	}
}

static_radio2
{
	qer_editorimage textures/models/submodels/radio2.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/radio2.tga
		blendfunc blend
		depthwrite
		rgbGen static
	}
}

static_radiostation1
{
	qer_editorimage textures/models/submodels/radiostation1.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/radiostation1.tga
		blendfunc blend
		depthwrite
		rgbGen static
	}
}

static_radiostation2
{
	qer_editorimage textures/models/submodels/radiostation2.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/radiostation2.tga
		blendfunc blend
		depthwrite
		rgbGen static
	}
}

static_radiostation4
{
	qer_editorimage textures/models/submodels/radiostation4.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/radiostation4.tga
		blendfunc blend
		depthwrite
		rgbGen static
	}
}

static_calendarskin
{
	qer_editorimage textures/models/submodels/calendarskin.tga
		//cull none
		//nopicmip
		//nomipmaps
	{
		map textures/models/submodels/calendarskin.tga
		rgbGen static
	}
}



