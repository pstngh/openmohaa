// pulsating TNT - ghost image
pulsating_tnt
{
	qer_editorimage textures/models/items/tnt.tga
//	{
//		map textures/models/items/tnt.tga
//		rgbGen lightingSpherical
//		blendFunc blend
//		alphaGen const 1
//	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

pulsating_radiobomb
{
	qer_editorimage textures/models/items/radiobomb.tga
//	{
//		map textures/models/items/radiobomb.tga
//		rgbGen lightingSpherical
//		blendFunc blend
//		alphaGen const 1
//	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

pulsating_radiobomb_pickup
{
	qer_editorimage textures/models/items/radiobomb.tga
	{
		map textures/models/items/radiobomb.tga
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

pulsating_hedgebomb
{
	qer_editorimage textures/models/items/hedgebomb.tga
	{
		map textures/models/items/hedgebomb.tga
		rgbGen lightingSpherical
		blendFunc blend
		alphaGen const 1
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// pulsating TNT - for picking up off the ground
pulsating_tnt_pickup
{
	qer_editorimage textures/models/items/tnt.tga
	{
		map textures/models/items/tnt.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Explosive - non pulsating
tnt
{
	qer_editorimage textures/models/items/tnt.tga
	{
		map textures/models/items/tnt.tga
		rgbGen lightingSpherical
	}
}
//radiobomb -non pulating
radiobomb
{
	qer_editorimage textures/models/items/radiobomb.tga
	{
		map textures/models/items/radiobomb.tga
		rgbGen lightingSpherical
	}
}
// German radio pulsating
radio_military_pulsating
{
	qer_editorimage textures/models/items/german_radio.tga
	{
		map textures/models/items/german_radio.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Battery
battery
{
	qer_editorimage textures/models/items/battery.tga
	{
		map textures/models/items/battery.tga
		rgbGen lightingSpherical
	}
}

// Binoculars
binoculars
{
	qer_editorimage textures/models/items/binoculars.tga
	{
		map textures/models/items/binoculars.tga
		rgbGen lightingSpherical
	}
}

// Beef
beef
{
	qer_editorimage textures/models/items/beef.tga
	{
		map textures/models/items/beef.tga
		rgbGen lightingSpherical
	}
}

// Tea set
s_pieces
{
	qer_editorimage textures/models/items/s_pieces.tga
	cull none
	{
		map textures/models/items/s_pieces.tga
		rgbGen lightingSpherical
	}
}

s_tray
{
	qer_editorimage textures/models/items/s_tray.tga
	cull none
	{
		map textures/models/items/s_tray.tga
		rgbGen lightingSpherical
	}
}

// Searchlight                                                   //
searchlight
{
	qer_editorimage textures/models/items/searchlight.tga
	{
		map textures/models/items/searchlight.tga
		rgbGen lightingSpherical
	}
}
searchlight_d
{
	qer_editorimage textures/models/items/searchlight_d.tga
	{
		map textures/models/items/searchlight_d.tga
		rgbGen lightingSpherical
	}
}
// light beam for Searchlight                                                   //
beam
{
	qer_editorimage textures/models/items/beam.tga
	{
		map textures/common/airdust.tga
		rgbGen lightingSpherical
		//tcmod scroll fromEntity 0
		tcmod scroll 0 0.3
		nextbundle
		map textures/models/items/beam.tga
		blendfunc add	
	}
}
// Bangalore torpedo
bangalore
{
	qer_editorimage	textures/models/items/bangalore.tga
	{
		map textures/models/items/bangalore.tga
		rgbGen lightingSpherical
	}
}

// Bangalore torpedo pulsating for pickup
bangalore_pulsating
{
	qer_editorimage	textures/models/items/bangalore.tga
	{
		map textures/models/items/bangalore.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Bangalore torpedo pulsating ghosting for planting
bangalore_pulsating_ghosting
{
	qer_editorimage	textures/models/items/bangalore.tga
	//{
	//	map textures/models/items/bangalore.tga
	//	rgbGen lightingSpherical
	//	blendFunc blend
	//	alphaGen const 1
	//}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Bratwurst weenie
bratwurst
{
	qer_editorimage textures/models/items/bratwurst.tga
	{
		map textures/models/items/bratwurst.tga
		rgbGen lightingSpherical
	}
}

// Flaregun
flaregun
{
	qer_editorimage textures/models/items/flaregun.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/flaregun.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha
	}
}

// German radio
radio_military
{
	qer_editorimage textures/models/items/german_radio.tga
	{
		map textures/models/items/german_radio.tga
		rgbGen lightingSpherical
	}
}



// Gasmask
gasmask
{
	qer_editorimage textures/models/items/gasmask.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/gasmask.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha
	}
}

gasmask_pulse
{
	qer_editorimage textures/models/items/gasmask.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/gasmask.tga
		rgbGen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Camera
camera
{
	qer_editorimage textures/models/items/camera.tga
	{
		map textures/models/items/camera.tga
		rgbGen lightingSpherical
	}
}

// Papers
wehrpass
{
	qer_editorimage textures/models/items/wehrpass.tga
	cull none
	{
		map textures/models/items/wehrpass.tga
		rgbGen lightingSpherical
	}
} 

// Papers
wehrpass_pulsating
{
	qer_editorimage textures/models/items/wehrpass.tga
	cull none
	{
		map textures/models/items/wehrpass.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
} 

wehrpass_l2
{
	qer_editorimage textures/models/items/wehrpass_l2.tga
	cull none
	{
		map textures/models/items/wehrpass_l2.tga
		rgbGen lightingSpherical
	}
}

wehrpass_l2_pulsating
{
	qer_editorimage textures/models/items/wehrpass_l2.tga
	cull none
	{
		map textures/models/items/wehrpass_l2.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// Fan
fan
{
	qer_editorimage textures/models/items/fan.tga
	cull none
	{
		map textures/models/items/fan.tga
		rgbGen lightingSpherical
		alphaFunc GE128
		depthWrite
	}
}

// Desk
static_desk
{
	qer_editorimage textures/models/items/desk.tga

	{
		map textures/models/items/desk.tga
		rgbgen lightingSpherical
	}
}

// Dresser
static_dresser
{
	qer_editorimage textures/models/items/dresser.tga

	{
		map textures/models/items/dresser.tga
		rgbgen lightingSpherical
	}
}

// Wardrobe
static_wardrobe
{
	qer_editorimage textures/models/items/wardrobe.tga

	{
		map textures/models/items/wardrobe.tga
		rgbgen lightingSpherical
	}
}

// Wardrobe damaged
static_wardrobe_damaged
{
	qer_editorimage textures/models/items/wardrobe_damaged.tga

	{
		map textures/models/items/wardrobe_damaged.tga
		rgbgen lightingSpherical
	}
}

// Toilet
static_toilet
{
	qer_editorimage textures/models/items/toilet.tga

	{
		map textures/models/items/toilet.tga
		rgbgen lightingSpherical
	}
}

// Table
table
{
	qer_editorimage textures/models/items/table.tga

	{
		map textures/models/items/table.tga
		rgbGen lightingSpherical
	}
}

// Bell
static_townbell
{
	qer_editorimage textures/models/items/bell.tga
	cull none
	{
		map textures/models/items/bell.tga
		rgbgen lightingSpherical
	}
}

// Signpost for signpost_CEAN
static_signpost_CEAN
{
	qer_editorimage textures/models/items/signpost_CEAN.tga
	{
		map textures/models/items/signpost_CEAN.tga
		rgbgen lightingSpherical
	}
}

// Signpost for signpost_STLO
static_signpost_STLO
{
	qer_editorimage textures/models/items/signpost_STLO.tga
	{
		map textures/models/items/signpost_STLO.tga
		rgbgen lightingSpherical
	}
}

// Signpost for signpost_VERET
static_signpost_VERET
{
	qer_editorimage textures/models/items/signpost_VERET.tga
	{
		map textures/models/items/signpost_VERET.tga
		rgbgen lightingSpherical
	}
}

// Signpost for signpost_VIERVILLE
static_signpost_VIERVILLE
{
	qer_editorimage textures/models/items/signpost_VIERVILLE.tga
	{
		map textures/models/items/signpost_VIERVILLE.tga
		rgbgen lightingSpherical
	}
}

//vanity                                                             
static_Vanity
{
	qer_editorimage textures/models/items/vanity.tga
	{
		map textures/models/items/vanity.tga
		rgbgen lightingSpherical
		alphaFunc GE128
		depthWrite
	}
	{
		map textures/common/reflection1.tga
		blendFunc blend
		depthFunc lequal
		rgbGen lightingSpherical
		tcGen environmentmodel
		nextbundle
		map textures/models/items/vanity_cutout.tga
		tcGen base
	}
}

static_rolltop_desk
{
	qer_editorimage textures/models/items/rolltop_desk.tga
	{
		map textures/models/items/rolltop_desk.tga
		rgbgen lightingSpherical
	}
}

static_Large_desk
{
	qer_editorimage textures/models/items/Large_desk.tga
	{
		map textures/models/items/Large_desk.tga
		rgbgen lightingSpherical
	}
}

static_single_bed
{
	qer_editorimage textures/models/items/single_bed.tga
	{
		map textures/models/items/single_bed.tga
		rgbgen lightingSpherical
	}
}

static_single_bed_damaged
{
	qer_editorimage textures/models/items/single_bed_d.tga
	{
		map textures/models/items/single_bed_d.tga
		rgbgen lightingSpherical
	}
}

static_bigbed
{
	qer_editorimage textures/models/items/bigbed.tga
	{
		map textures/models/items/bigbed.tga
		rgbgen lightingSpherical
	}
}

static_produce_cart
{
	qer_editorimage textures/models/items/cart.tga
	cull none
	{
		map textures/models/items/cart.tga
		rgbGen lightingSpherical
	}
}

static_wagon
{
	qer_editorimage textures/models/items/wagon.tga
	cull none
	{
		map textures/models/items/wagon.tga
		rgbGen lightingSpherical
	}
}

// phone roundbase
phone1
{
	qer_editorimage textures/models/items/phone1.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/phone1.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

//rotary dial on phone1
dial
{
	qer_editorimage textures/models/items/dial.tga
	cull none
	{
		map textures/models/items/dial.tga
		rgbGen lightingSpherical
		blendFunc blend
		depthWrite
	}
}
//{
//	qer_editorimage textures/models/items/dial.tga
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//	}
//	{
//		map textures/models/items/dial.tga
//		rgbGen lightingSpherical
//		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
//	}
//}

// phone squarebase
phone2
{
	qer_editorimage textures/models/items/phone2.tga
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/phone2.tga
		rgbGen lightingSpherical
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}

//Books
book1
{
	qer_editorimage textures/models/items/book1.tga
	{
		map textures/models/items/book1.tga
		rgbGen lightingSpherical
	}
}

book2
{
	qer_editorimage textures/models/items/book2.tga
	{
		map textures/models/items/book2.tga
		rgbGen lightingSpherical
	}
}

book3
{
	qer_editorimage textures/models/items/book3.tga
	{
		map textures/models/items/book3.tga
		rgbGen lightingSpherical
	}
}

book4
{
	qer_editorimage textures/models/items/book4.tga
	{
		map textures/models/items/book4.tga
		rgbGen lightingSpherical
	}
}

book5
{
	qer_editorimage textures/models/items/book5.tga
	{
		map textures/models/items/book5.tga
		rgbGen lightingSpherical
	}
}

//glasses
shotglass1
{
	qer_editorimage textures/models/items/shotglass1.tga
	cull none
	{
		map textures/common/dglass.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.3
		depthwrite
		blendFunc blend
	}
	{
		map textures/models/ITEMS/shotglass1.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

shotglass2
{
	qer_editorimage textures/models/items/shotglass2.tga
	cull none
	{
		map textures/common/dglass.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.3
		blendFunc blend
		depthwrite
	}
	{
		map textures/models/ITEMS/shotglass2.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
}

tallglass
{
	qer_editorimage textures/common/dglass.tga
	cull none
	{
		map textures/common/dglass.tga
		rgbGen lightingSpherical
		tcgen environment
		alphagen const .3
		depthwrite
		blendFunc blend
	}
	{
		map textures/models/ITEMS/tallglass.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
	
}

shortglass
{
	qer_editorimage textures/common/dglassblue.tga
	cull none
	{
		map textures/common/dglassblue.tga
		rgbGen lightingSpherical
		tcgen environment
		alphagen const .3
		depthwrite
		blendFunc blend
	}
	{
		map textures/models/ITEMS/shortglass.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
	
}

drinkglass
{
	qer_editorimage textures/models/items/drinkglass.tga
	cull none
	{
		map textures/common/dglassgreen.tga
		rgbGen lightingSpherical
		tcgen environment
		alphagen const .3
		depthwrite
		blendFunc blend
	}
	{
		map textures/models/items/drinkglass.tga
		rgbGen lightingSpherical
		blendFunc blend
	}
	
}

// German magazines
g_magazine_arbeit1
{
	qer_editorimage textures/models/items/arbeit1.tga
	{
		map textures/models/items/arbeit1.tga
		rgbGen lightingSpherical
	}
}

g_magazine_arbeit2
{
	qer_editorimage textures/models/items/arbeit2.tga
	{
		map textures/models/items/arbeit2.tga
		rgbGen lightingSpherical
	}
}

g_magazine_deradler1
{
	qer_editorimage textures/models/items/der_adler1.tga
	{
		map textures/models/items/der_adler1.tga
		rgbGen lightingSpherical
	}
}

g_magazine_derdeutsche
{
	qer_editorimage textures/models/items/der_deutsche.tga
	{
		map textures/models/items/der_deutsche.tga
		rgbGen lightingSpherical
	}
}

g_magazine_derschulungsbrief
{
	qer_editorimage textures/models/items/schulungsbrief.tga
	{
		map textures/models/items/schulungsbrief.tga
		rgbGen lightingSpherical
	}
}

g_magazine_die_wehrmacht1
{
	qer_editorimage textures/models/items/die_wehrmacht1.tga
	{
		map textures/models/items/die_wehrmacht1.tga
		rgbGen lightingSpherical
	}
}

g_magazine_die_wehrmacht2
{
	qer_editorimage textures/models/items/die_wehrmacht2.tga
	{
		map textures/models/items/die_wehrmacht2.tga
		rgbGen lightingSpherical
	}
}

g_magazine_die_wehrmacht3
{
	qer_editorimage textures/models/items/die_wehrmacht3.tga
	{
		map textures/models/items/die_wehrmacht3.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte1
{
	qer_editorimage textures/models/items/frauen_warte1.tga
	{
		map textures/models/items/frauen_warte1.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte2
{
	qer_editorimage textures/models/items/frauen_warte2.tga
	{
		map textures/models/items/frauen_warte2.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte3
{
	qer_editorimage textures/models/items/frauen_warte3.tga
	{
		map textures/models/items/frauen_warte3.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte4
{
	qer_editorimage textures/models/items/frauen_warte4.tga
	{
		map textures/models/items/frauen_warte4.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte5
{
	qer_editorimage textures/models/items/frauen_warte5.tga
	{
		map textures/models/items/frauen_warte5.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte6
{
	qer_editorimage textures/models/items/frauen_warte6.tga
	{
		map textures/models/items/frauen_warte6.tga
		rgbGen lightingSpherical
	}
}

g_magazine_frauen_warte7
{
	qer_editorimage textures/models/items/frauen_warte7.tga
	{
		map textures/models/items/frauen_warte7.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib1
{
	qer_editorimage textures/models/items/ib1.tga
	{
		map textures/models/items/ib1.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib2
{
	qer_editorimage textures/models/items/ib2.tga
	{
		map textures/models/items/ib2.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib3
{
	qer_editorimage textures/models/items/ib3.tga
	{
		map textures/models/items/ib3.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib4
{
	qer_editorimage textures/models/items/ib4.tga
	{
		map textures/models/items/ib4.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib5
{
	qer_editorimage textures/models/items/ib5.tga
	{
		map textures/models/items/ib5.tga
		rgbGen lightingSpherical
	}
}

g_magazine_ib6
{
	qer_editorimage textures/models/items/ib6.tga
	{
		map textures/models/items/ib6.tga
		rgbGen lightingSpherical
	}
}

radio_civilian
{
	qer_editorimage textures/models/items/radio_civilian.tga
	{
		map textures/models/items/radio_civilian.tga
		rgbGen lightingSpherical
	}
}

static_churchpew
{
	qer_editorimage textures/models/items/pew.tga
	{
		map textures/models/items/pew.tga
		rgbgen lightingSpherical
	}
}

static_churchpew_damaged
{
	qer_editorimage textures/models/items/pewdamaged.tga
	{
		map textures/models/items/pewdamaged.tga
		rgbgen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}

static_tub_shower
{
	qer_editorimage textures/models/items/tub_shower.tga
	{
		map textures/models/items/tub_shower.tga
		rgbgen lightingSpherical
	}
}

static_simpledesk
{
	qer_editorimage textures/models/items/simpledesk.tga
	{
		map textures/models/items/simpledesk.tga
		rgbgen lightingSpherical
	}
}

simpledesk
{
	qer_editorimage textures/models/items/simpledesk.tga
	{
		map textures/models/items/simpledesk.tga
		rgbGen lightingSpherical
	}
}

static_metaldesk
{
	qer_editorimage textures/models/items/metaldesk.tga
	{
		map textures/common/reflection1.tga
		rgbgen lightingSpherical
		tcgen environmentmodel
	}
	{
		map textures/models/items/metaldesk.tga
		rgbgen lightingSpherical
		blendfunc gl_one_minus_src_alpha gl_src_alpha
	}
}

metaldesk
{
	qer_editorimage textures/models/items/metaldesk.tga
	{
		map textures/models/items/metaldesk.tga
		rgbGen lightingSpherical
	}
}

static_card_deck
{
	qer_editorimage textures/models/items/carddeck.tga
	{
		map textures/models/items/carddeck.tga
		rgbgen lightingSpherical
	}
}

bunkerstove
{
	qer_editorimage textures/models/items/bunkerstove.tga
	{
		map textures/models/items/bunkerstove.tga
		rgbgen lightingSpherical
	}
}

inter_bunkerstove
{
	qer_editorimage textures/models/items/bunkerstove.tga
	{
		map textures/models/items/bunkerstove.tga
		rgbGen lightingSpherical
	}
}

static_bottle_large
{
	qer_editorimage textures/models/items/milkjug.tga
	{
		map textures/models/items/milkjug.tga
		rgbgen lightingSpherical
	}
}

static_bottle_small
{
	qer_editorimage textures/models/items/small_milkjug.tga
	{
		map textures/models/items/small_milkjug.tga
		rgbgen lightingSpherical
	}
}

static_small_pitcher
{
	qer_editorimage textures/models/items/pitcher.tga
	cull none
	{
		map textures/models/items/pitcher.tga
		rgbgen lightingSpherical
	}
}

static_pitcher
{
	qer_editorimage textures/models/items/tall_pitcher.tga
	cull none
	{
		map textures/models/items/tall_pitcher.tga
		rgbgen lightingSpherical
	}
}

static_jug
{
	qer_editorimage textures/models/items/jug_whandle.tga
	{
		map textures/models/items/jug_whandle.tga
		rgbgen lightingSpherical
	}
}

static_bucket
{
	qer_editorimage textures/models/items/bucket.tga
	cull none
	{
		map textures/models/items/bucket.tga
		rgbgen lightingSpherical
	}
}

hedgebomb
{
	qer_editorimage textures/models/items/hedgebomb.tga
	{
		map textures/models/items/hedgebomb.tga
		rgbGen lightingSpherical
	}
}

static_coatrack
{
	qer_editorimage textures/models/items/coatrack.tga
	{
		map textures/models/items/coatrack.tga
		rgbgen lightingSpherical
	}
}

detonator
{
	qer_editorimage textures/models/items/detonator.tga
	{
		map textures/models/items/detonator.tga
		rgbGen lightingSpherical
	}
}

clipboard
{
	qer_editorimage textures/models/items/clipboard.tga
	{
		map textures/models/items/clipboard.tga
		rgbGen lightingSpherical
	}
}

clipboard_pulse
{
	qer_editorimage textures/models/items/clipboard.tga
	{
		map textures/models/items/clipboard.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

pencil
{
	qer_editorimage textures/models/items/pencil.tga
	{
		map textures/models/items/pencil.tga
		rgbGen lightingSpherical
	}
}

///////////////////////////////
//healthpacks
///////////////////////////////

healthcanteen

{
	qer_editorimage textures/models/items/healthcanteen.tga
	{
		map textures/models/items/healthcanteen.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map $whiteimage
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.1 0.07 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

firstaid
{
	qer_editorimage textures/models/items/first_aid.tga
	{
		map textures/models/items/first_aid.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map $whiteimage
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.1 0.07 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

// this is the texture that's used on the health packed dropped in DM
firstaid_dm
{
	qer_editorimage textures/models/items/first_aid.tga
	{
		map textures/models/items/first_aid.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
//		map textures/models/items/pulse.tga
		map $whiteimage
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

surgeonpack
{
	qer_editorimage textures/models/items/field_medpack.tga
	{
		map textures/models/items/field_medpack.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map $whiteimage
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.1 0.07 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

cigarette
{
	qer_editorimage textures/models/items/cigarette.tga
	{
		map textures/models/items/cigarette.tga
		rgbGen lightingSpherical
	}
}

cherry_c
{
	qer_editorimage textures/models/items/cigarette_a.tga
	{
		map textures/models/items/cigarette.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map textures/common/cherry.tga
		rgbGen Identity
        blendfunc add
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

trainswitch
{
	qer_editorimage textures/models/items/trainswitch.tga
	{
		map textures/models/items/trainswitch.tga
		rgbGen lightingSpherical
	}
}

pulsating_trainswitch
{
	qer_editorimage textures/models/items/trainswitch.tga
	{
		map textures/models/items/trainswitch.tga
		rgbGen lightingSpherical
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

officer_uniform
{
	qer_editorimage textures/models/items/officer_uniform.tga
	{
		map textures/models/items/officer_uniform.tga
		rgbGen lightingSpherical
	}
}

officer_uniform_pulsating
{
	qer_editorimage textures/models/items/officer_uniform.tga
	{
		map textures/models/items/officer_uniform.tga
		rgbGen lightingSpherical
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}

static_metal_funnel
{
	qer_editorimage textures/models/items/metal_funnel.tga
	cull none	
	{
		map textures/models/items/metal_funnel.tga
		rgbGen static
	}
}

static_screwdriver
{
	qer_editorimage textures/models/items/screwdriver.tga	
	{
		map textures/models/items/screwdriver.tga
		rgbGen static
	}
}

static_wrenches
{
	qer_editorimage textures/models/items/wrenches.tga	
	{
		map textures/models/items/wrenches.tga
		rgbGen static
	}
}

static_toolbox_opened
{
	qer_editorimage textures/models/items/toolbox_opened.tga	
	cull none
	{
		map textures/models/items/toolbox_opened.tga
		rgbGen static
	}
}

static_toolbox_closed
{
	qer_editorimage textures/models/items/toolbox_closed.tga	
	{
		map textures/models/items/toolbox_closed.tga
		rgbGen static
	}
}

//sliding ruler
static_sliding_ruler_alpha
{
	qer_editorimage textures/models/items/sliding_ruler.tga	
	{
		map textures/models/items/sliding_ruler.tga
		rgbGen static
//		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA

	}
}

static_sliding_ruler
{
	qer_editorimage textures/models/items/sliding_ruler.tga	
	{
		map textures/models/items/sliding_ruler.tga
		rgbGen static
	}
}

static_metal_file
{
	qer_editorimage textures/models/items/metal_file.tga	
	{
		map textures/models/items/metal_file.tga
		rgbGen static
	}
}

static_pitchfork
{
	qer_editorimage textures/models/items/pitchfork.tga	
	{
		map textures/models/items/pitchfork.tga
		rgbGen static
	}
}

static_ballpeen_hammer
{
	qer_editorimage textures/models/items/ballpeen_hammer.tga	
	{
		map textures/models/items/ballpeen_hammer.tga
		rgbGen static
	}
}

static_protractor
{
	qer_editorimage textures/models/items/protractor.tga	
	{
		map textures/models/items/protractor.tga
		rgbGen static
	}
}

//coat_hanging_stand
static_coat_hanging_stand_1
{
	qer_editorimage textures/models/items/coat_hanging_stand.tga	
	{
		map textures/models/items/coat_hanging_stand.tga
		rgbGen static

	}
}

static_coat_hanging_stand_2
{
	qer_editorimage textures/models/items/coat_hanging_stand.tga	
	cull none
	{
		map textures/models/items/coat_hanging_stand.tga
		rgbGen static
	}
}

static_wicker_basket_1
{
	qer_editorimage textures/models/items/wicker_basket_1.tga	
	cull none
	{
		map textures/models/items/wicker_basket_1.tga
		rgbGen static
	}
}

static_wicker_basket_2
{
	qer_editorimage textures/models/items/wicker_basket_2.tga	
	cull none
	{
		map textures/models/items/wicker_basket_2.tga
		rgbGen static
	}
}

static_wicker_basket_3
{
	qer_editorimage textures/models/items/wicker_basket_3.tga	
	cull none
	{
		map textures/models/items/wicker_basket_3.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_1
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_1.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_1.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_2
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_2.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_2.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_3
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_3.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_3.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_4
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_4.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_4.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_5
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_5.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_5.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_6
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_6.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_6.tga
		rgbGen static
	}
}

static_south_africa_ceramic_pot_7
{
	qer_editorimage textures/models/items/south_africa_ceramic_pot_7.tga	
	cull none
	{
		map textures/models/items/south_africa_ceramic_pot_7.tga
		rgbGen static
	}
}

static_rations_sh
{
	qer_editorimage textures/models/items/rations_sh.tga	
	{
		map textures/models/items/rations_sh.tga
		rgbGen static
	}
}

//towel
static_towel_2
{
	qer_editorimage textures/models/items/towel.tga	
	{
		map textures/models/items/towel.tga
		rgbGen static

	}
}

static_towel_1
{
	qer_editorimage textures/models/items/towel.tga	
	cull none
	{
		map textures/models/items/towel.tga
		rgbGen static
	}
}

static_metal_mug
{
	qer_editorimage textures/models/items/metal_mug.tga	
	cull none
	{
		map textures/models/items/metal_mug.tga
		rgbGen static
	}
}

static_night_stand
{
	qer_editorimage textures/models/items/night_stand.tga	
	{
		map textures/models/items/night_stand.tga
		rgbGen static
	}
}

static_worklamp
{
	qer_editorimage textures/models/items/worklamp.tga	
	cull none
	{
		map textures/models/items/worklamp.tga
		rgbGen static
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
//drawing_compass
static_drawing_compass_2
{
	qer_editorimage textures/models/items/drawing_compass.tga	
	{
		map textures/models/items/drawing_compass.tga
		rgbGen static

	}
}

static_drawing_compass_1
{
	qer_editorimage textures/models/items/drawing_compass.tga	
	cull none
	{
		map textures/models/items/drawing_compass.tga
		rgbGen static
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
//messkit
static_messkit_1
{
	qer_editorimage textures/models/items/messkit.tga	
	{
		map textures/models/items/messkit.tga
		rgbGen static

	}
}

static_messkit_2
{
	qer_editorimage textures/models/items/messkit.tga	
	cull none
	{
		map textures/models/items/messkit.tga
		rgbGen static
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
//pens
static_grn_metal_mug
{
	qer_editorimage textures/models/items/grn_metal_mug.tga	
	cull none
	{
		map textures/models/items/grn_metal_mug.tga
		rgbGen static

	}
}

static_pencils_01
{
	qer_editorimage textures/models/items/pencils_01.tga	
	{
		map textures/models/items/pencils_01.tga
		rgbGen static
	}
}
static_pencils_02
{
	qer_editorimage textures/models/items/pencils_02.tga	
	{
		map textures/models/items/pencils_02.tga
		rgbGen static
	}
}
static_pencils_03
{
	qer_editorimage textures/models/items/pencils_03.tga	
	{
		map textures/models/items/pencils_03.tga
		rgbGen static
	}
}
static_pens_01
{
	qer_editorimage textures/models/items/pens_01.tga	
	cull none
	{
		map textures/models/items/pens_01.tga
		rgbGen static
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
static_pens_04
{
	qer_editorimage textures/models/items/pens_04.tga	
	{
		map textures/models/items/pens_04.tga
		rgbGen static
	}
}
static_german_rations_1
{
	qer_editorimage textures/models/items/german_rations_1.tga	
	{
		map textures/models/items/german_rations_1.tga
		rgbGen static
	}
}
static_german_rations_2
{
	qer_editorimage textures/models/items/german_rations_2.tga	
	{
		map textures/models/items/german_rations_2.tga
		rgbGen static
	}
}

pulsating_stg44plans
{
	qer_editorimage textures/models/items/sturmgewehr_plans.tga	
	{
		map textures/models/items/sturmgewehr_plans.tga
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

stg44plans
{
	qer_editorimage textures/models/items/sturmgewehr_plans.tga	
	{
		map textures/models/items/sturmgewehr_plans.tga
		rgbGen lightingSpherical
	}
}

pulsating_tigerfibel
{
	qer_editorimage textures/models/items/tigerfibel.tga	
	{
		map textures/models/items/tigerfibel.tga
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

tigerfibel
{
	qer_editorimage textures/models/items/tigerfibel.tga	
	{
		map textures/models/items/tigerfibel.tga
		rgbGen lightingSpherical
	}
}

// grandfathers_clock
grandfathers_clock
{
	qer_editorimage textures/models/items/grandfathers_clock.tga

	{
		map textures/models/items/grandfathers_clock.tga
		rgbGen lightingSpherical
	}
}
grandfathers_clock_pendulum
{
	qer_editorimage textures/models/items/grandfathers_clock_pendulum.tga
	//cull none
	{
		map textures/models/items/grandfathers_clock_pendulum.tga
		rgbGen lightingSpherical
		//blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphafunc GE128
		depthWrite
	}
}
grandfathers_clock_face
{
	qer_editorimage textures/models/items/grandfathers_clock_face.tga
	//cull none
	{
		map textures/models/items/grandfathers_clock_face.tga
		rgbGen lightingSpherical
		alphafunc GE128
		depthWrite
	}
}
grandfathers_clock_glass
{	
	cull none
	//sort nearest
	nopicmip
	nomipmaps
	qer_editorimage textures/models/items/grandfathers_clock_glass2.tga
//	{
//		map textures/models/items/clockglass_env.tga
//		tcgen environmentmodel
//		alphagen const .20
//		blendFunc blend
//	     //rgbGen static
//	}
	{
		map textures/models/items/grandfathers_clock_glass2.tga
		rgbGen lightingSpherical
		alphafunc GT0
		depthwrite
		//blendfunc blend
		rgbGen lightingSpherical
	}
}
// grandfathers_clock destroyed
grandfathers_clock_d
{
	qer_editorimage textures/models/items/grandfathers_clock_d.tga

	{
		map textures/models/items/grandfathers_clock_d.tga
		rgbGen lightingSpherical
	}
}
grandfathers_clock_pendulum_d
{
	qer_editorimage textures/models/items/grandfathers_clock_pendulum.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//	}
	{
		map textures/models/items/grandfathers_clock_pendulum.tga
		rgbGen lightingSpherical
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}
grandfathers_clock_face_d
{
	qer_editorimage textures/models/items/grandfathers_clock_face_d.tga
	cull none
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//	}
	{
		map textures/models/items/grandfathers_clock_face_d.tga
		rgbGen lightingSpherical
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}
grandfathers_clock_glass_d
{
	qer_editorimage textures/models/items/grandfathers_clock_glass_d.tga
	cull none
//	{
//		map textures/common/dglass.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		alphagen const 0.3
//		blendfunc blend
//	}
	{
		map textures/models/items/grandfathers_clock_glass_d.tga
		rgbGen lightingSpherical
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		depthWrite
	}
}
static_hopechest
{
	qer_editorimage textures/models/items/hopechest.tga	
	{
		map textures/models/items/hopechest.tga
		rgbGen static
	}
}
static_ornaterectable
{
	qer_editorimage textures/models/items/ornaterectable.tga	
	{
		map textures/models/items/ornaterectable.tga
		rgbGen static
	}
}
static_ornaterectable_d
{
	qer_editorimage textures/models/items/ornaterectable_d.tga	
	{
		map textures/models/items/ornaterectable_d.tga
		rgbGen static
	}
}
static_simplerectable
{
	qer_editorimage textures/models/items/simplerectable.tga	
	{
		map textures/models/items/simplerectable.tga
		rgbGen static
	}
}
//table with umbrella
static_tablewithumbrella_1
{
	qer_editorimage textures/models/items/tablewithumbrella.tga
	{
		map textures/models/items/tablewithumbrella.tga
		rgbGen static
	}
}
static_tablewithumbrella_2
{
	qer_editorimage textures/models/items/umbrella.tga
	cull none
	{
		map textures/models/items/umbrella.tga
		rgbGen static
		alphafunc ge128
	}
}

/////////////////////////////////////////
//shaders for document interact objects
/////////////////////////////////////////

envelope
{
	qer_editorimage textures/models/items/envelope.tga
	{
		map textures/models/items/envelope.tga
		rgbGen lightingSpherical
	}
}

document1
{
	qer_editorimage textures/models/items/doc1.tga
	{
		map textures/models/items/doc1.tga
		rgbGen lightingSpherical
	}
}

book1
{
	qer_editorimage textures/models/items/book1.tga
	{
		map textures/models/items/book1.tga
		rgbGen lightingSpherical
	}
}

book2
{
	qer_editorimage textures/models/items/book2.tga
	{
		map textures/models/items/book2.tga
		rgbGen lightingSpherical
	}
}

book4
{
	qer_editorimage textures/models/items/book4.tga
	{
		map textures/models/items/book4.tga
		rgbGen lightingSpherical
	}
}

book5
{
	qer_editorimage textures/models/items/book5.tga
	{
		map textures/models/items/book5.tga
		rgbGen lightingSpherical
	}
}

////////////////////////////////////////////
// naxos radar plans and stuff
////////////////////////////////////////////

naxosplans1_pulsating
{
	qer_editorimage textures/models/items/naxosplans1.tga
	{
		map textures/models/items/naxosplans1.tga
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

naxosplans2_pulsating
{
	qer_editorimage textures/models/items/naxosplans2.tga
	{
		map textures/models/items/naxosplans2.tga
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

naxosplans3_pulsating
{
	cull none
	qer_editorimage textures/models/items/naxosplans3.tga
	{
		map textures/models/items/naxosplans3.tga
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

naxosplans4_pulsating
{
	cull none
	qer_editorimage textures/models/items/naxosplans4.tga
	{
		map textures/models/items/naxosplans4.tga
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

projektnaxos_pulsating
{
	qer_editorimage textures/models/items/projektnaxos.tga
	{
		map textures/models/items/projektnaxos.tga
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

///////////////////////////////////////////

alarmswitch
{
	qer_editorimage textures/models/items/alarmswitch.tga
	{
		map textures/models/items/alarmswitch.tga
		rgbGen lightingSpherical
	}
}


radar
{
	qer_editorimage textures/models/items/radar.tga
	{
		map textures/models/items/radar.tga
		rgbGen lightingSpherical
	}
}
radarcull
{
	qer_editorimage textures/models/items/radar.tga
	cull none
	{
		map textures/models/items/radar.tga
		rgbGen lightingSpherical
	}
}

sledgehammer
{
	qer_editorimage textures/models/items/sledgehammer.tga
	{
		map textures/models/items/sledgehammer.tga
		rgbGen lightingSpherical
	}
}
kettle
{
	qer_editorimage textures/models/items/kettle.tga
	{
		map textures/models/items/kettle.tga
		rgbGen lightingSpherical
	}
}
///////////////////////////////////////////////////
//playing cards for ze germans
//////////////////////////////////////////////////
cardhand01
{
	qer_editorimage textures/models/items/cardhand01.tga
	{
		map textures/models/items/cardhand01.tga
		rgbGen lightingSpherical
		alphafunc ge128
	}
}
cardhand02
{
	qer_editorimage textures/models/items/cardhand02.tga
	{
		map textures/models/items/cardhand02.tga
		rgbGen lightingSpherical
		alphafunc ge128
	}
}
cardsingle
{
	qer_editorimage textures/models/items/cardsingle.tga
	{
		map textures/models/items/cardsingle.tga
		rgbGen lightingSpherical
	}
}
cardpile
{
	qer_editorimage textures/models/items/cardpile.tga
	{
		map textures/models/items/cardpile.tga
		rgbGen lightingSpherical
		alphafunc ge128
	}
}
picklock
{
	qer_editorimage textures/models/items/picklock.tga
	{
		map textures/models/items/picklock.tga
		rgbGen lightingSpherical
	}
}
wrench
{
	qer_editorimage textures/models/items/wrench.tga
	{
		map textures/models/items/wrench.tga
		rgbGen lightingSpherical
	}
}
hammer
{
	qer_editorimage textures/models/items/hammer.tga
	{
		map textures/models/items/hammer.tga
		rgbGen lightingSpherical
	}
}

///////////////////////////////////////////////////////
// shaders for maps and aerial photo objectives
///////////////////////////////////////////////////////

mapfrance
{
	qer_editorimage textures/models/maps/france.tga
	{
		map textures/models/items/france.tga
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

mapcarentan
{
	qer_editorimage textures/models/maps/carentan.tga
	{
		map textures/models/items/carentan.tga
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

mapstlo
{
	qer_editorimage textures/models/maps/st_lo.tga
	{
		map textures/models/items/st_lo.tga
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

mapstrehaia
{
	qer_editorimage textures/models/maps/strehaia.tga
	{
		map textures/models/items/strehaia.tga
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

surveybook
{
	qer_editorimage textures/models/maps/surveybook.tga
	{
		map textures/models/items/surveybook.tga
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

aerialrecon1
{
	qer_editorimage textures/models/maps/aerialrecon1.tga
	{
		map textures/models/maps/aerialrecon1.tga
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

aerialrecon2
{
	qer_editorimage textures/models/maps/aerialrecon2.tga
	{
		map textures/models/maps/aerialrecon2.tga
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

aerialrecon3
{
	qer_editorimage textures/models/maps/aerialrecon3.tga
	{
		map textures/models/maps/aerialrecon3.tga
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


//
// STICKYBOMBS
//
Inventory_StickyBomb_stickybomb
{
	qer_editorimage textures/models/items/StickyBomb/stickybomb.tga
	{
		map textures/models/items/StickyBomb/stickybomb.tga
		rgbGen lightingSpherical
	}
}

Inventory_StickyBomb_stickybomb_pulsing
{
	qer_editorimage textures/models/items/StickyBomb/stickybomb.tga
	{
		map textures/models/items/StickyBomb/stickybomb.tga
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

// Stickybomb pickup

sticky_backpack
{
	qer_editorimage textures/models/items/stickybomb_pickup/sticky_backpack.jpg
	{
		map textures/models/items/stickybomb_pickup/sticky_backpack.jpg
		rgbGen lightingSpherical
	}
}
