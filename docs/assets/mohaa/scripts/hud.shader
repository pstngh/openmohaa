qtextures/hud/crosshair
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map textures/hud/crosshair.tga
	}
}

//-------------------------------------------
// These are the HUD icons for weapon classes
//-------------------------------------------

// Overlay for when the player doesn't have the weapon
textures/hud/weap_empty_old
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_empty.tga
	}
}

textures/hud/weap_empty
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
	}
}

textures/hud/weap_grenade_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_grenade_x.tga
	}
}

textures/hud/weap_heavy_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_heavy_x.tga
	}
}

textures/hud/weap_pistol_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_pistol_x.tga
	}
}

textures/hud/weap_rifle_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_rifle_x.tga
	}
}

textures/hud/weap_mg_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_mg_x.tga
	}
}

textures/hud/weap_smg_x
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
//		rgbGen const .3 .3 .3
//		alphaGen const .6
//		map $whiteimage
		map textures/hud/weap_smg_x.tga
	}
}
// Overlay for when the weapon is equipped
textures/hud/weap_equipped
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc GL_SRC_ALPHA GL_ONE
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_selected.tga
	}
}

textures/hud/weap_grenade_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_grenade_s.tga
	}
}

textures/hud/weap_heavy_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_heavy_s.tga
	}
}

textures/hud/weap_pistol_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_pistol_s.tga
	}
}

textures/hud/weap_rifle_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_rifle_s.tga
	}
}

textures/hud/weap_mg_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_mg_s.tga
	}
}

textures/hud/weap_smg_s
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/weap_smg_s.tga
	}
}

textures/hud/weap_pistol
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_pistol.tga
	}
}

textures/hud/weap_rifle
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_rifle.tga
	}
}

textures/hud/weap_smg
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_smg.tga
	}
}

textures/hud/weap_mg
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		alphaGen oneMinusVertex
		map textures/hud/weap_mg.tga
	}
}

textures/hud/weap_grenade
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_grenade.tga
	}
}

textures/hud/weap_heavy
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/weap_heavy.tga
	}
}

//-------------------------------------
// These are the HUD icons for items
//-------------------------------------

textures/hud/item_empty
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/item_empty.tga
	}
}

textures/hud/item_highlight
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that can be controled with the alpha level
		rgbGen wave sin .75 .25 0 .5
		alphaGen oneMinusVertex
		map textures/hud/item_selected.tga
	}
}

textures/hud/item_camera
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_camera.tga
	}
}

textures/hud/item_binoculars
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/binoc3.tga
	}
}

textures/hud/item_bangalore
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/bangalores.tga
	}
}

textures/hud/item_battery
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/battery.tga
	}
}

textures/hud/item_clipboard
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/clipboard.tga
	}
}

textures/hud/item_explosive
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/explosives.tga
	}
}

textures/hud/item_radio_explosive
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_radio_explosive.tga
	}
}

textures/hud/item_gasmask
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/gasmask.tga
	}
}

textures/hud/item_papers1
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_papers2.tga
	}
}

textures/hud/item_papers2
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_papers1.tga
	}
}

textures/hud/item_wirecutters
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/wirecutters.tga
	}
}

textures/hud/item_radio
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/radio1.tga
	}
}

textures/hud/item_radar_blueprints
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_radar_blueprints.tga
	}
}

textures/hud/item_radar_notes
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_radar_notes.tga
	}
}

textures/hud/item_uboat_blueprints
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_uboat_blueprints.tga
	}
}

textures/hud/item_uboat_notes
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_uboat_notes.tga
	}
}

textures/hud/item_uniform
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_uniform.tga
	}
}

textures/hud/item_stg44_blueprints
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_stg44_blueprints.tga
	}
}

textures/hud/item_tigerii_manual
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_tigerii_manual.tga
	}
}

textures/hud/item_battleplans
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		clampmap textures/hud/item_tigerii_manual.tga
	}
} 

//-------------------------------------------
// These are the shaders for the hud compass, health meter, and stealth meter
//-------------------------------------------

// this is the main backdrop for the section of the hud
// that contains the health meter, stealth meter, and the compass
textures/hud/healthstealthback
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/healthstealthback.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

//this is the little directional arrow between the spheres
textures/hud/directional_arrow
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/directional_arrow.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

// the shader used for the player's health meter
textures/hud/healthmeter
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/healthmeter.tga
		blendFunc blend
		alphaGen oneMinusVertex
//		tcmod rotate 5
//		nextbundle
//		clampmap textures/hud/healthmeter.tga
//		tcmod rotate -5
	}
}

// the shader used when the player's health changes
textures/hud/healthmeterflash
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/healthmeterflash.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

// The shader used for the stealth meter
textures/hud/stealthmeter
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/stealthmeter.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

// the main body of the compass
textures/hud/compassback
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/compassback.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/compasspain
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/compasspain.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/compasspainblank
{
	surfaceparm nolightmap
	{
		map $whiteimage
		blendFunc blend
		alphaGen const 0
	}
}

// The compass needle
textures/hud/compassneedle
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/compassneedle.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

// the little thinggys that go around the compass for the objectives
textures/hud/compassobjball
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/compassobjball.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

//-------------------------------------------
// These are the shaders for the hud ammo meter, and weapon clip meter
//-------------------------------------------

// the main backdrop for the section of the hud
// that contains the ammo meter, clip meter, and ammo model display
textures/hud/ammoclipback
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/ammoclipback.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

// this is the shader used for the ammo meter
textures/hud/ammometer
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/ammometer.tga
		blendFunc blend
		alphaGen oneMinusVertex
//		tcmod rotate 5
//		nextbundle
//		clampmap textures/hud/ammometer.tga
//		tcmod rotate -5
	}
}

// this is the shader used for the weapon clip meter
textures/hud/clipmeter
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/clipmeter.tga
		blendFunc blend
		alphaGen oneMinusVertex
//		tcmod rotate 5
//		nextbundle
//		clampmap textures/hud/clipmeter.tga
//		tcmod rotate -5
	}
}

// this is the central backdrop for where the ammo model is drawn
textures/hud/ammoback
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/ammoback.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

//-------------------------------------------
// These are shaders for the ammo bars
//-------------------------------------------

textures/hud/clip_fraggrenade
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_fraggrenade.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/clip_pistol
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_pistol.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/clip_rifle
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_rifle.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/clip_steilhandgranate
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_steilhandgranate.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/clip_bazooka
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_bazooka.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/clip_panzerschreck
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_panzerschreck.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}


textures/hud/clip_shotgun
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		map textures/hud/clip_shotgun.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}
//-------------------------------------------
// This is the overlay that's done when the player is zoomed
//-------------------------------------------

textures/hud/zoomoverlay
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/zoomoverlay.tga
		blendFunc blend
		alphaGen oneMinusVertex
		rgbGen vertex
	}
} 

textures/hud/gasmask_overlay
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/gasmask_overlay.tga
		blendFunc blend
	}
}

textures/hud/cameraoverlay
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/cameraoverlay.tga
		blendFunc blend
		alphaGen oneMinusVertex
		rgbGen vertex
	}
}

textures/hud/binocularsoverlay
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/binocularsoverlay.tga
		blendFunc blend
		alphaGen oneMinusVertex
		rgbGen vertex
	}
}

//---------------------------------------------- //
// The following are hud map components and such //
//---------------------------------------------- //

textures/hud/m3l1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
		rgbGen identity
	    	blendFunc blend
		alphaGen oneMinusVertex
		map textures/hud/m3l1.tga
	}
} 

textures/hud/mapstar
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
 //       alphaFunc GE128
	    blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/mapstar.tga
	}
}

textures/hud/objectives_backddrop
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
 //       alphaFunc GE128
	    blendFunc blend
		rgbGen identity
		alphaGen oneMinusVertex
		map textures/hud/objectives_backddrop.tga
	}
}

//------------------------------------------- //
// Team icons for use over teammates & on hud //
//------------------------------------------- //

textures/hud/allies_headicon
{
	spritegen parallel
	surfaceparm nolightmap
//	nomipmaps
	nopicmip
	cull none
	{
//		alphaFunc GE128
//		alphagen 0.8
//		rgbGen identityLighting
		alphaFunc GT0
		blendFunc blend
		rgbGen vertex
		alphagen vertex
		map textures/hud/allies.tga
	}
}

textures/hud/axis_headicon
{
	spritegen parallel
	surfaceparm nolightmap
//	nomipmaps
	nopicmip
	cull none
	{
//		alphaFunc GE128
//		alphagen 0.8
//		rgbGen identityLighting
		alphaFunc GT0
		blendFunc blend
		rgbGen vertex
		alphagen vertex
		map textures/hud/axis.tga
	}
}
textures/hud/allies
{
	spritegen parallel
	surfaceparm nolightmap
//	nomipmaps
	nopicmip
	cull none
	{
		alphaFunc GE128
		map textures/hud/allies.tga
	}
}

textures/hud/axis
{
	spritegen parallel
	surfaceparm nolightmap
//	nomipmaps
	nopicmip
	cull none
	{
		alphaFunc GE128
		map textures/hud/axis.tga
	}
}


//------------------- //
// stopwatch graphics //
//------------------- //

// the main body of the stopwatch
textures/hud/stopwatchbase
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/stopwatchbase.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

//this is the stopwatch hand
textures/hud/stopwatchhand
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/stopwatchhand.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

//------------------------ //
// instant messaging menus //
//------------------------ //

textures/hud/instamsg_main
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_main.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/instamsg_group_a
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_group_a.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/instamsg_group_b
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_group_b.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/instamsg_group_c
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_group_c.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/instamsg_group_d
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_group_d.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}

textures/hud/instamsg_group_e
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/instamsg_group_e.tga
		blendFunc blend
		alphaGen oneMinusVertex
	}
}
