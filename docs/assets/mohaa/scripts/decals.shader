//======================================================================================
// Bullet holes

bhole_paper
{
	polygonOffset
	{
		map textures/decals/bhole_sand.tga
		blendFunc filter
	}
}

bhole_wood
{
	polygonOffset
	{
		map textures/decals/bhole_wood1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_metal
{
	polygonOffset
	{
		map textures/decals/bhole_metal1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_stone
{
	polygonOffset
	{
		map textures/decals/bhole_stone.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_dirt
{
	polygonOffset
	{
		map textures/decals/bhole_sand.tga
		blendFunc filter
	}
}

bhole_grill
{
	polygonOffset
	{
		map textures/decals/bhole_metal1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_grass
{
	polygonOffset
	{
		map textures/decals/bhole_sand.tga
		blendFunc filter
	}
}

bhole_mud
{
	polygonOffset
	{
		map textures/decals/bhole_mud.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_glass
{
	polygonOffset
	cull none
	{
		map textures/decals/bhole_glass.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_snow
{
	polygonOffset
	{
		map textures/decals/bhole_sand.tga
		blendFunc filter
	}
}

bhole_carpet
{
	polygonOffset
	{
		map textures/decals/bhole_carpet.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

bhole_plaster
{
	polygonOffset
	{
		map textures/decals/bhole_plaster.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
} 

bhole_sand
{
	polygonOffset
	{
		map textures/decals/bhole_sand.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

//======================================================================================
// Shadows

footshadow
{
	polygonOffset
	{
		map textures/decals/footshadow.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}

treeshadows
{
	qer_editorimage textures/decals/treeshadow.tga
	polygonOffset
	cull none

	{
		map textures/decals/treeshadow.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

//======================================================================================
// Explosion blast marks

blastmark
{
	polygonOffset
	{
		map textures/decals/blastmark.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphagen vertex
		rgbgen vertex
	}
}

//======================================================================================
// Tread marks

testtread
{
	polygonOffset
	cull none
	{
		map textures/decals/testtread.tga
		blendFunc blend
		alphagen vertex
		rgbgen vertex
	}
}

//======================================================================================
// Other Decals

// Weathered
waterstain
{
	qer_editorimage textures/decals/ed_waterstain.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/d_waterstain.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

weathered_concrete
{
	qer_editorimage textures/decals/d_wthrdconc.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/d_wthrdconc.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

exposed_brick
{
	qer_editorimage textures/decals/brick1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/brick1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

cracks
{
	qer_editorimage textures/decals/cracked1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/cracked1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

weatheredstone
{
	qer_editorimage textures/decals/weatheredstone.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/weatheredstone.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

desert1
{
	qer_editorimage textures/decals/desert1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/desert1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

wthrwall1
{
	qer_editorimage textures/decals/wthrwall1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/wthrwall1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

cracked2
{
	qer_editorimage textures/decals/cracked2.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/cracked2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

pitted
{
	qer_editorimage textures/decals/pitted.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/pitted.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

muddy1
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/muddy1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

// Detail decals
afrika_trim
{
	qer_editorimage textures/decals/ceiltrim_afrik.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/ceiltrim_afrik.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

afrikatrim2
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/afrikatrim2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

cornerstones
{
	qer_editorimage textures/decals/corners1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/corners1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

tudor
{
	qer_editorimage textures/decals/tudor_1.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/tudor_1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

europe_trim
{
	qer_editorimage textures/decals/europe_trim.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/europe_trim.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}
//Organics

grass_tuft
{
	polygonOffset
	cull none
	{
		map textures/decals/grass_tuft.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 


//FrontCulled Decals
fc_wthrdconcrete
{
	qer_editorimage textures/decals/d_wthrdconc.tga
	polygonOffset
	{
		clampmap textures/decals/d_wthrdconc.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

//Detailing
floordrain
{
	qer_editorimage textures/decals/floordrain.tga
	polygonOffset
	cull none
	{
		clampmap textures/decals/floordrain.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

afrika_map1
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/afrika_map1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

afrika_map2
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/afrika_map2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

frenchpostr_1a
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/frenchpostr_1a.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_km_marker
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/jh_km_marker.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_oran
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/jh_oran.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

tapstry_af1
{
	polygonOffset
	cull none
	{
		clampmap textures/Algiers/tapstry_af1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

mag1
{
	polygonOffset
	cull none
	{
		clampmap textures/barracks/mag1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

pinup1
{
	polygonOffset
	cull none
	{
		clampmap textures/barracks/pinup1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

shutter_set2
{
	polygonOffset
	cull none
	{
		clampmap textures/Central_Europe/shutter_set2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

ties_decal
{
	polygonOffset
	cull none
	{
		clampmap textures/Central_Europe/ties_decal.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

char
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/char.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

bullet_plasterhit
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/bullet_plasterhit.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

cracked3
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/cracked3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

treadmarks_conc
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/treadmarks_conc.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

treadmarks_conc2
{
	polygonOffset
//	cull none
	nomipmaps
	{
//		clampmap textures/decals/treadmarks_conc2.tga
		map textures/decals/treadmarks_conc2.tga
// carl said not to clampmap these (zied)
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

treadmarks_ground
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/treadmarks_ground.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

treadmarks_ground2
{
	polygonOffset
//	cull none
	nomipmaps
	{
//		clampmap textures/decals/treadmarks_ground2.tga
		map textures/decals/treadmarks_ground2.tga
// carl said not to clampmap these (zied)
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

icecicles
{
	polygonOffset
	cull none
	{
		clampmap textures/Central_Europe_Winter/icecicles.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}        

shutter_set2W
{
	polygonOffset
	cull none
	{
		clampmap textures/Central_Europe_Winter/shutter_set2W.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}        

shutter_W
{
	polygonOffset
	cull none
	{
		clampmap textures/Central_Europe_Winter/shutter_W.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

calendar
{
	polygonOffset
	cull none
	{
		clampmap textures/das_boot/calendar.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}        

poster4
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/french/poster4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

revue_poster2
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/french/revue_poster2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

shepherd
{
	polygonOffset
	cull none
	{
		clampmap textures/french/shepherd.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bunkervent_1
{
	polygonOffset
	cull none
	{
		clampmap textures/General_Industrial/bunkervent_1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

church_cornertrimsml
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/church_cornertrimsml.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

daily_gorring
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/German/daily_gorring.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

germanpst1
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/German/germanpst1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

grmnsn_halt1
{
	polygonOffset
	cull none
	{
		clampmap textures/German/grmnsn_halt1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_airfield
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_airfield.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_gasstorage
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_gasstorage.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_gasstorage2
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_gasstorage2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_h20disbursement
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_h20disbursement.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_noentry1
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_noentry1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_nosmoking_ger
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_nosmoking_ger.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_remagen2km
{
	polygonOffset
	cull none
	{
		clampmap textures/German/jh_remagen2km.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

minen
{
	polygonOffset
	cull none
	{
		clampmap textures/German/minen.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

minen_alt
{
	polygonOffset
	cull none
	{
		clampmap textures/German/minen_alt.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

minen_alt2
{
	polygonOffset
	cull none
	{
		clampmap textures/German/minen_alt2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

picture1
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/picture1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

picture2
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/picture2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

pictureset_3a
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/pictureset_3a.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

pictureset_3b
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/pictureset_3b.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

picture5
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/picture5.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

victorioussml
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/victorioussml.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bulletset_1
{
	polygonOffset
	cull none
	{
		clampmap textures/mohcommon/bulletset_1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_maskwin1
{
	polygonOffset
	cull none
	{
		clampmap textures/mohcommon/jh_maskwin1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_cottonwood2
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_cottonwood2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_cottonwood3
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_cottonwood3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_foliage1
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_foliage1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_foliage2
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_foliage2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_foliage4
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_foliage4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

weeds_decal
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/weeds_decal.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_foliage5
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_foliage5.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

jh_foliage6
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/jh_foliage6.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

pj_roots1
{
	polygonOffset
	cull none
	{
		clampmap textures/wilderness/pj_roots1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

treeRootsradial
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/treeRootsradial.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

vorsicht
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/decals/vorsicht.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

erster
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/decals/erster.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

eagleglobe
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/decals/eagleglobe.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bar1
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/bar1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

antiquites
{
	polygonOffset
	cull none
	{
		clampmap textures/decals/antiquites.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

townhallwindow
{
	polygonOffset
	cull none
	{
		clampmap textures/interior/sp_twnhall_arcwinmsk.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

rubble2_decal
{
	polygonOffset
	{
		map textures/decals/rubble2_decal.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

danger_hot
{
	polygonOffset
	{
		map textures/decals/danger_hot.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

french_advert2
{
	polygonOffset
    surfaceparm paper
	{
		map textures/french/french_advert2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

french_advert3
{
	polygonOffset
    surfaceparm paper
	{
		map textures/french/french_advert3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

vivedegaulle
{
	polygonOffset
	{
		map textures/decals/vivedegaulle.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

carpet_fancy1
{
	polygonOffset
	{
		map textures/central_europe/carpet_fancy1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

carpet_fancy2
{
	polygonOffset
	{
		map textures/central_europe/carpet_fancy2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

carpet_fancy3
{
	polygonOffset
	{
		map textures/central_europe/carpet_fancy3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

snowdrift1
{
	polygonOffset
	{
		map textures/decals/snowdrift1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 
 
sign_storeblu
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/decals/sign_storeblu.tga
		alphagen vertex
		rgbgen vertex
	}
} 

sign_store
{
	polygonOffset
    surfaceparm paper
	cull none
	{
		clampmap textures/decals/sign_store.tga
		alphagen vertex
		rgbgen vertex
	}
} 

electrical
 {
	polygonOffset
	{
		map textures/decals/electrical.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

emergency_exit
 {
	polygonOffset
	{
		map textures/decals/emergency_exit.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

enemy_listening
 {
	polygonOffset
	{
		map textures/decals/enemy_listening.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }
 
entrance
 {
	polygonOffset
	{
		map textures/decals/entrance.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

entrance_defense
 {
	polygonOffset
	{
		map textures/decals/entrance_defense.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

gas_attack
 {
	polygonOffset
	{
		map textures/decals/gas_attack.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

gas_victim
 {
	polygonOffset
	{
		map textures/decals/gas_victim.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

machinery
 {
	polygonOffset
	{
		map textures/decals/machinery.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

motto1
 {
	polygonOffset
	{
		map textures/decals/motto1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

no_smoking
 {
	polygonOffset
	{
		map textures/decals/no_smoking.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

plug
 {
	polygonOffset
	{
		map textures/decals/plug.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

quarter_number
 {
	polygonOffset
	{
		map textures/decals/quarter_number.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

rangefinder1
 {
	polygonOffset
	{
		map textures/decals/rangefinder1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

rangefinder2
 {
	polygonOffset
	{
		map textures/decals/rangefinder2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

relief_valve
 {
	polygonOffset
	{
		map textures/decals/relief_valve.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

vierville1
 {
	polygonOffset
	{
		map textures/decals/vierville1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

vierville2
 {
	polygonOffset
	{
		map textures/decals/vierville2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

vierville3
 {
	polygonOffset
	{
		map textures/decals/vierville3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

vierville4
 {
	polygonOffset
	{
		map textures/decals/vierville4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

zutritt_verboten
 {
	polygonOffset
	{
		map textures/decals/zutritt_verboten.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

barrage
{
	polygonOffset
	{
		map textures/decals/barrage.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bunker_legend
{
	polygonOffset
	{
		map textures/decals/bunker_legend.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

CO_danger
{
	polygonOffset
	{
		map textures/decals/CO_danger.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

motto2
{
	polygonOffset
	{
		map textures/decals/motto2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

motto3
{
	polygonOffset
	{
		map textures/decals/motto3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

motto4
{
	polygonOffset
	{
		map textures/decals/motto4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

motto5
{
	polygonOffset
	{
		map textures/decals/motto5.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

motto6
{
	polygonOffset
	{
		map textures/decals/motto6.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

standig
{
	polygonOffset
	{
		map textures/decals/standig.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

1st_floor
 {
	polygonOffset
	{
		map textures/decals/1st_floor.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

2nd_floor
 {
	polygonOffset
	{
		map textures/decals/2nd_floor.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

3rd_floor
 {
	polygonOffset
	{
		map textures/decals/3rd_floor.tga
		blendfunc filter
 	}
 }

4th_floor
 {
	polygonOffset
	{
		map textures/decals/4th_floor.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
 }

resistance_warning
{
	polygonOffset
      surfaceparm paper
	{
		map textures/decals/resistance_warning.tga
		depthwrite
		alphaFunc GE128
		rgbgen vertex
	}

}


a
{
	polygonOffset
	{
		map textures/decals/a.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

b
{
	polygonOffset
	{
		map textures/decals/b.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

1
{
	polygonOffset
	{
		map textures/decals/1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

2
{
	polygonOffset
	{
		map textures/decals/2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

3
{
	polygonOffset
	{
		map textures/decals/3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

Painting_master1
{
	polygonOffset
	{
		map textures/interior/grand_1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master2
{
	polygonOffset
	{
		map textures/interior/grand_2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master3
{
	polygonOffset
	{
		map textures/interior/grand_3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master4
{
	polygonOffset
	{
		map textures/interior/grand_4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master5
{
	polygonOffset
	{
		map textures/interior/grand_5.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

tapestry_master7
{
	polygonOffset
	{
		map textures/interior/grand_7tapestry.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master8
{
	polygonOffset
	{
		map textures/interior/grand_8.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Painting_master9
{
	polygonOffset
	{
		map textures/interior/grand_pj.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

arrow
 {
	polygonOffset
	{
		clampmap textures/decals/arrow.tga
		blendfunc filter
 	}
}

mundepot1
 {
	polygonOffset
	{
		clampmap textures/decals/mundepot1.tga
		blendfunc filter
 	}
}

mundepot2
 {
	polygonOffset
	{
		clampmap textures/decals/mundepot2.tga
		blendfunc filter
 	}
}

ein
 {
	polygonOffset
	{
		clampmap textures/decals/ein.tga
		blendfunc filter
 	}
}

swei
 {
	polygonOffset
	{
		clampmap textures/decals/swei.tga
		blendfunc filter
 	}
}

drei
 {
	polygonOffset
	{
		clampmap textures/decals/drei.tga
		blendfunc filter
 	}
}

sub_motto1
 {
	polygonOffset
	{
		clampmap textures/decals/sub_motto1.tga
		blendfunc filter
 	}
}

torpedolagen1
 {
	polygonOffset
	{
		clampmap textures/decals/torpedolagen1.tga
		blendfunc filter
 	}
}

torpedolagen2
 {
	polygonOffset
	{
		clampmap textures/decals/torpedolagen2.tga
		blendfunc filter
 	}
}

tower_center
 {
	polygonOffset
	{
		clampmap textures/decals/tower_center.tga
		blendfunc filter
 	}
}

versdepot1
 {
	polygonOffset
	{
		clampmap textures/decals/versdepot1.tga
		blendfunc filter
 	}
}

versdepot2
 {
	polygonOffset
	{
		clampmap textures/decals/versdepot2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
 	}
}

we_build
{
	polygonOffset
	{
		map textures/decals/we_build.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bei_brennendem
{
	polygonOffset
	{
		map textures/decals/bei_brennendem.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

bei_gasgefahr
{
	polygonOffset
	{
		map textures/decals/bei_gasgefahr.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

maschinenraum
{
	polygonOffset
	{
		map textures/decals/maschinenraum.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

starkstromverteiler
{
	polygonOffset
	{
		map textures/decals/starkstromverteiler.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

fernsprechverteiler
{
	polygonOffset
	{
		map textures/decals/fernsprechverteiler.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}

watercloset
{
	polygonOffset
	{
		map textures/decals/watercloset.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

french_advert1
{
	polygonOffset
	{
		map textures/french/french_advert1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

nettingshadow
{
	polygonOffset
	{
		map textures/misc_outside/netting.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

fence1shadow
{
	polygonOffset
	{
		map textures/mohcommon/secfence1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}
 
blackboard
{
	polygonOffset
        {
		map textures/decals/blackboard.tga
		depthWrite
                alphagen vertex
		rgbgen vertex
	}
} 

france_advert1
{
	polygonOffset
	{
		map textures/barracks/ad_01.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

mortar_sandhit
{
	polygonOffset
	{
		map textures/decals/mortardecal.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

deadfish
{
	polygonOffset
	{
		map textures/decals/deadfish.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

deadfish_school
{
	polygonOffset
	{
		map textures/decals/deadfish_school.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

regpalmshadow
{
	polygonOffset
	{
		map textures/MODELS/natural/regpalmsprite.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

Pole_shadow
{
	polygonOffset
	{
		map textures/decals/Pole_shadow.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cagelightshadow
{
	polygonOffset
	{
		map textures/decals/cagelightshadow.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cagelightshadow_vert
{
	polygonOffset
	{
		map textures/decals/cagelightshadow_vert.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cards1
{
	polygonOffset
	{
		map textures/decals/cards1.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cards2
{
	polygonOffset
	{
		map textures/decals/cards2.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cards3
{
	polygonOffset
	{
		map textures/decals/cards3.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

cards4
{
	polygonOffset
	{
		map textures/decals/cards4.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
} 

sous_sign
{
	polygonOffset
	{
		map textures/french/sous.tga
		blendfunc blend
		alphagen vertex
		rgbgen vertex
	}
}