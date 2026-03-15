///////////////////////////////////////////
//whitehall animated fan graphics
///////////////////////////////////////////

fan_anim1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      animMap 24 textures/mohmenu/fan1a.tga textures/mohmenu/fan1b.tga textures/mohmenu/fan1c.tga textures/mohmenu/fan1d.tga textures/mohmenu/fan1e.tga textures/mohmenu/fan1f.tga
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
   }
}

fan_anim2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	animMap 24 textures/mohmenu/fan2a.tga textures/mohmenu/fan2b.tga textures/mohmenu/fan2c.tga textures/mohmenu/fan2d.tga textures/mohmenu/fan2e.tga textures/mohmenu/fan2f.tga
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
   }
}

/////////////////////////////////////
//button overlays for menu buttons
/////////////////////////////////////

menu_button_trans
{
	{
		map $whiteimage
		blendfunc alphaadd
		alphagen constant 0.0
	}
}

menu_button_glow
{
	{
		map $whiteimage
		blendfunc alphaadd
		alphagen constant 0.2
	}
}

menu_button_glow_lite
{
	{
		map $whiteimage
		blendfunc alphaadd
		alphagen constant 0.1
	}
}



video_button_trans
{
	{
		map $whiteimage
		blendfunc alphaadd
		alphagen constant 0.0
	}
}

video_button_glow
{
	{
		map textures/mohmenu/vidmask.tga
		blendfunc alphaadd
		alphagen constant 0.2
	}
}

load_button_glow
{
	{
		map textures/mohmenu/rec_load_button.tga
		blendfunc alphaadd
		alphagen constant 0.1
	}
}

save_button_glow
{
	{
		map textures/mohmenu/rec_save_button.tga
		blendfunc alphaadd
		alphagen constant 0.1
	}
}

new_button_glow
{
	{
		map textures/mohmenu/rec_new_button.tga
		blendfunc alphaadd
		alphagen constant 0.1
	}
}

maplist_h
{
	{
		map textures/mohmenu/maplist_glow.tga
		blendfunc alphaadd
		alphagen constant 0.2
	}
}
records_h
{
	{
		map textures/mohmenu/records_h.tga
		blendfunc alphaadd
		alphagen constant 0.1
	}
}

medals_h
{
	{
		clampMap textures/mohmenu/medals_h.tga
		blendfunc alphaadd
		alphagen constant 0.1
	}
}

textures/mohmenu/multiplayer_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      clampMap textures/mohmenu/multiplayer_h.tga
   }
}

textures/mohmenu/briefing_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
      clampMap textures/mohmenu/briefing_h.tga
   }
}

textures/mohmenu/options_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/options_h.tga
   }
}

/////////////////////////////////////////////////////////
// Shadow for underneath the player models in multiplayer options
/////////////////////////////////////////////////////////

textures/mohmenu/pmshadow
{
	polygonOffset
	{
		map textures/common/shadow.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbgen global
		//alphagen global
	}
}

///////////////////////////////////////
//menu screen backgrounds
///////////////////////////////////////

main_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/main_a.tga
   }
}

main_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/main_b.tga
   }
}

options_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/options_a.tga
   }
}

options_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/options_b.tga
   }
}

controls_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/controls_a.tga
   }
}

controls_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/controls_b.tga
   }
}

audio_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/audio_a.tga
   }
}

audio_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/audio_b.tga
   }
}

video_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/video_a.tga
   }
}

video_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/video_b.tga
   }
}

advanced_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/advanced_a.tga
   }
}

advanced_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/advanced_b.tga
   }
}

multiplayer_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/multiplayer_a.tga
   }
}

multiplayer_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/multiplayer_b.tga
   }
}

warrecords_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/warrecords_a.tga
   }
}

warrecords_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/warrecords_b.tga
   }
}

loadsave_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/loadsave_a.tga
   }
}

loadsave_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/loadsave_b.tga
   }
}


options_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/options_sign.tga
   }
}

briefing_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing_sign.tga
   }
}

briefingroom_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefingroom_sign.tga
   }
}


multiplayer_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/multiplayer_sign.tga
   }
}

maplist_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/maplist_sign.tga
   }
}

warrecords_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/warrecords_sign.tga
   }
}

advanced_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/advanced_sign.tga
   }
}

controls_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/controls_sign.tga
   }
}

audio_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/audio_sign.tga
   }
}

newgame_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/newgame_sign.tga
   }
}

continue_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/continue_sign.tga
   }
}

video_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/video_sign.tga
   }
}

loaddemo_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/loaddemo_sign.tga
   }
}

continue
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/continue.tga
   }
}

continue_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/continue_h.tga
   }
}

backtogame
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/backtogame.tga
   }
}

backtogame_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/backtogame_h.tga
   }
}

refresh
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/refresh.tga
   }
}

refresh_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/refresh_h.tga
   }
}

///////////////////////////////////////
//flashy briefing screen load widget
///////////////////////////////////////

loadingwidget
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      Map textures/mohmenu/loadingwidget.tga
   }
   {
      blendFunc GL_SRC_ALPHA GL_ONE
      Map textures/mohmenu/loadingwidget.tga
      rgbGen wave sin .5 .5 0 1
  }
}

load_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/load_h.tga
   }
}

save_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	Map textures/mohmenu/save_h.tga
   }
}

delete_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	Map textures/mohmenu/delete_h.tga
   }
}

loadsaveback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	Map textures/mohmenu/loadsaveback.tga
   }
}

paperclip
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/paperclip.tga
   }
}

loadsave_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/loadsave_h.tga
   }
}

medalscase_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/medalscase_h.tga
   }
}

personalrecords_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/personalrecords_sign.tga
   }
}

loadsave_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/loadsave_sign.tga
   }
}

medalcaseback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/medalcaseback.tga
   }
}

///////////////////////////////////
//graphics for medals in medalcase
///////////////////////////////////

armycommendation_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/armycommendation_med.tga
   }
}

bronzestar_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/bronzestar_med.tga
   }
}

distinguishedcross_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/distinguishedcross_med.tga
   }
}

distinguishedservice_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/distinguishedservice_med.tga
   }
}

goodconduct_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/goodconduct_med.tga
   }
}

legionmerit_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/legionmerit_med.tga
   }
}

purpleheart_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/purpleheart_med.tga
   }
}

silverstar_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/silverstar_med.tga
   }
}

soldiers_med
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/soldiers_med.tga
   }
}
/////////////////////////////////////////////////
//level briefing background art
/////////////////////////////////////////////////

///////////////////////////
//m5 briefing art
///////////////////////////

m5a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m5a.tga
   }
}

m5b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m5b.tga
   }
}

////////////////////////////
//m5l1
////////////////////////////

m5l1a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m5l1a.tga
   }
}
////////////////////////////
// m5l2
////////////////////////////

m5l2a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m5l2a.tga
   }
}
////////////////////////////////
// m5l3
////////////////////////////////

m5l3a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m5l3a.tga
   }
}

/////////////////////////////////
//dday briefing
/////////////////////////////////


m3l1a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m3l1a.tga
   }
}

m3l1b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m3l1b.tga
   }
}
//////////////////////////////////
//m3 briefings
//////////////////////////////////

m3b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m3b.tga
   }
}

m3l2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m3l2.tga
   }
}

m3l3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m3l3.tga
   }
}

//////////////////////////////////
//briefing room main screen
//////////////////////////////////

briefingroom_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefingroom_a.tga
   }
}

briefingroom_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefingroom_b.tga
   }
}


//////////////////////////////////
//briefing room map overlays
//////////////////////////////////

arzew
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/arzew.tga
   }
}

arzew_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/arzew_h.tga
   }
}

trondheim
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/trondheim.tga
   }
}

trondheim_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/trondheim_h.tga
   }
}

omaha
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/omaha.tga
   }
}

bootcamp
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/bootcamp.tga
   }
}

subdiving
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/subdiving.tga
   }
}

naxos2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/naxos2.tga
   }
}

documents
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/documents.tga
   }
}

subpen2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/subpen2.tga
   }
}

powtrain
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/powtrain.tga
   }
}

schmerzen
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/schmerzen.tga
   }
}

radioshack
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/radioshack.tga
   }
}

nazitroops
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/nazitroops.tga
   }
}


brestmap
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/brestmap.tga
   }
}

francemap2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      //blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/briefing/francemap2.tga
   }
}


omaha_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/omaha_h.tga
   }
}

normandy
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/normandy.tga
   }
}

normandy_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/normandy_h.tga
   }
}

brittany
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/brittany.tga
   }
}

brittany_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/brittany_h.tga
   }
}

siegfried
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/siegfried.tga
   }
}

siegfried_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/siegfried_h.tga
   }
}
////////////////////////////////
//mission 2 level briefing art
////////////////////////////////

m2b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m2b.tga
   }
}

m2l1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m2l1.tga
   }
}
m2l2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m2l2.tga
   }
}
m2l3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m2l3.tga
   }
}
///////////////////////////////////
//mission 4 briefing stuff
///////////////////////////////////

m4l1a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m4l1a.tga
   }
}

m4l1b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m4l1b.tga
   }
}

m4b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m4b.tga
   }
}

m4l3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m4l3.tga
   }
}

m4l2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m4l2.tga
   }
}

//////////////////////////////////
//mission 1 briefing 
//////////////////////////////////

m1b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m1b.tga
   }
}

m1l1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m1l1.tga
   }
}

m1l2a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m1l2a.tga
   }
}

m1l2b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m1l2b.tga
   }
}

m1l3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m1l3.tga
   }
}

////////////////////////////////////////////////
//mission 6 briefing
////////////////////////////////////////////////

m6b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6b.tga
   }
}

m6l1a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6l1a.tga
   }
}

m6l1c
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6l1c.tga
   }
}

m6l2a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6l2a.tga
   }
}

m6l2b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6l2b.tga
   }
}

m6l3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/m6l3.tga
   }
}

/////////////////////////////////////
//war records main menu button
/////////////////////////////////////

war_records
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/war_records.tga
   }
}

war_records_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/war_records_h.tga
   }
}

/////////////////////////////////////
//big map board main menu button
/////////////////////////////////////

bigmap
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/bigmap.tga
   }
}

bigmap_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      rgbgen   global
      //alphagen global
      blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	clampMap textures/mohmenu/bigmap_h.tga
   }
}

/////////////////////////////////
//default loading screen logo
/////////////////////////////////

mohlogo
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      Map textures/mohmenu/moh_logo.tga
   }
}

//////////////////////////////////
//black background tile
//////////////////////////////////

black
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      Map textures/mohmenu/black.tga
   }
}

//////////////////////////////////////
// deathmatch level loading graphics
//////////////////////////////////////

paper
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/paper.tga
   }
}

sepiaoverlay
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/sepiaoverlay.tga
	blendfunc filter
   }
}

shadow
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/shadow.tga
	blendfunc filter
   }
}

wrinkles
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/wrinkles.tga
	blendfunc filter
   }
}

mohdm1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm1.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm2.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm3.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm4
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm4.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm5
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm5.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm6
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm6.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

mohdm7
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/dmloading/mohdm7.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

//////////////////////////////////////
//objective dm loading photos
//////////////////////////////////////

objdm1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/objloading/objdm1.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

objdm2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/objloading/objdm2.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}

objdm4
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/objloading/objdm4.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}


objdm5
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
	map $whiteimage
   }
   {
     clampMap textures/mohmenu/objloading/objdm5.tga
	blendfunc gl_one_minus_src_alpha gl_src_alpha
   }
}


//////////////////////////////////////
// photo corner tabs
//////////////////////////////////////

ul_corner
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/dmloading/ul_corner.tga
	blendfunc blend
   }
}

ur_corner
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/ur_corner.tga
	blendfunc blend
   }
}
ll_corner
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/dmloading/ll_corner.tga
	blendfunc blend
   }
}
lr_corner
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/dmloading/lr_corner.tga
	blendfunc blend
   }
}

greytest1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/greytest1.tga
   }
}

greytest2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/greytest2.tga
   }
}

greytest3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/greytest3.tga
   }
}

/////////////////////////////////////////////
// misc menu items
/////////////////////////////////////////////

missionlog
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/missionlog.tga
   }
}


//--------------------------------------------------------------------
// Jon's No Server Found Pic
//--------------------------------------------------------------------
textures/menu/noservers
{
	nomipmaps
	nopicmip
	cull none
	{
		map textures/mohmenu/noservers.tga
		blendfunc blend
	}
}

//--------------------------------------------------------------------
// Jon's Game Saved Pic
//--------------------------------------------------------------------
textures/menu/gamesaved
{
	nomipmaps
	nopicmip
	cull none
	{
		map textures/mohmenu/gamesaved.tga
		blendfunc blend
	}
}

/////////////////////////////

textures/menu/slider
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/slider.tga
   }
}

2015logo
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/2015.tga
//      rgbgen   vertex
      rgbgen   global
   }
}

textures/menu/arrow_left
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/arrow_left.tga
	blendfunc blend
   }
   
}

textures/menu/arrow_left_sel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/arrow_left_sel.tga
	blendfunc blend
   }
}
textures/menu/arrow_right
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/arrow_right.tga
	blendfunc blend
   }
   
}

textures/menu/arrow_right_sel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/arrow_right_sel.tga
	blendfunc blend
   }
}

textures/menu/blank2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/blank2.tga
   }
}

textures/menu/check_mark
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/check_mark.tga
   }
}

textures/menu/checkbox_checked
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/checkbox_checked.tga
   }
}

textures/menu/checkbox_unchecked
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/checkbox_unchecked.tga
   }
}

textures/menu/ea
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/ea.tga
   }
}

textures/menu/slider_thumb_sel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/slider_thumb_sel.tga
   }
}

textures/menu/no_saved_games
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/no_saved_games.tga
   }
}

textures/menu/presskey
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/presskey.tga
   }
}

textures/menu/paused
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/paused.tga
   }
}

textures/menu/slider_thumb
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
     clampMap textures/mohmenu/slider_thumb.tga
   }
}

textures/menu/loading
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      Map textures/mohmenu/moh_loading.tga
   }
   {
      blendFunc GL_SRC_ALPHA GL_ONE
      Map textures/mohmenu/moh_loading.tga
      rgbGen wave sin .5 .5 0 1
  }
}

textures/menu/pulldownarrow
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/pulldownarrow.tga
	blendfunc blend
   }
   
}

textures/menu/pulldownarrow_sel
{
	nomipmaps
	nopicmip
	cull none
	force32bit
	surfaceparm nolightmap
	{
		clampMap textures/mohmenu/pulldownarrow_sel.tga
		blendfunc blend
	}
}

textures/menu/pulldownarrowsmall
{
	nomipmaps
	nopicmip
	cull none
	force32bit
	surfaceparm nolightmap
	{
		clampMap textures/mohmenu/pulldownarrowsmall.tga
		blendfunc blend
	}
}

textures/menu/pulldownarrowsmall_sel
{
	nomipmaps
	nopicmip
	cull none
	force32bit
	surfaceparm nolightmap
	{
		clampMap textures/mohmenu/pulldownarrowsmall_sel.tga
		blendfunc blend
	}
}

/////////////////////////////////////
//multiplayer room menu buttons
/////////////////////////////////////

multistart_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multistart_h.tga
   }
}

maproom_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/maproom_h.tga
   }
}

multijoin_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multijoin_h.tga
   }
}

multioptions_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multioptions_h.tga
	blendfunc blend
   }
}

//////////////////////////////////////
//multiplayer room signs
//////////////////////////////////////

startgame_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startgame_sign.tga
   }
}

maproom_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/maproom_sign.tga
   }
}
joingame_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/joingame_sign.tga
   }
}
multiplayeroptions_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multiplayeroptions_sign.tga
   }
}

/////////////////////////////////
//multiplayer server screen stuff
/////////////////////////////////

selectgame_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectgame_a.tga
   }
}

selectgame_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectgame_b.tga
   }
}

serverback_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/serverback_a.tga
   }
}

serverback_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/serverback_b.tga
   }
}

roundbutton
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/roundbutton.tga
	blendfunc blend
   }
}

ffabutton
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/ffabutton.tga
	blendfunc blend
   }
}

teambutton
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/teambutton.tga
	blendfunc blend
   }
}

objectivebutton
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/objectivebutton.tga
	blendfunc blend
   }
}

gametype_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/gametype_h.tga
//	blendfunc blend
		blendfunc alphaadd
		alphagen constant 0.2

   }
}

/////////////////////////////////////////
//briefing room backgrounds
/////////////////////////////////////////

briefingroomdark_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefingroomdark_a.tga
   }
}

briefingroomdark_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefingroomdark_a.tga
   }
}

briefingroomdark_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefingroomdark_b.tga
   }
}

////////////////////////////////
//start server pieces
////////////////////////////////

startffa_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startffa_a.tga
   }
}

startffa_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startffa_b.tga
   }
}

startobjective_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startobjective_a.tga
   }
}

startobjective_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startobjective_b.tga
   }
}

startround_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startround_a.tga
   }
}

startround_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startround_b.tga
   }
}

startteam_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startteam_a.tga
   }
}

startteam_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startteam_b.tga
   }
}

selectmap
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectmap.tga
   }
}

startgame
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/startgame.tga
   }
}

inactivespectate
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/inactivespectate.tga
   }
}

inactivekick
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/inactivekick.tga
   }
}

teamdamage
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/teamdamage.tga
   }
}

maxplayers
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/maxplayers.tga
   }
}

timelimit
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/timelimit.tga
   }
}

roundlimit
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/roundlimit.tga
   }
}

scorelimit
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/scorelimit.tga
   }
}

gameservername
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/gameservername.tga
   }
}

ffaserverback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/ffaserverback.tga
   }
}

teamback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/teamback.tga
   }
}
objectiveback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/objectiveback.tga
   }
}
roundback
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/roundback.tga
   }
}

modelselect_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/modelselect_a.tga
	blendfunc blend
   }
}

modelselect_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/modelselect_b.tga
	blendfunc blend
   }
}

///////////////////////////////////
// mission briefing signs
///////////////////////////////////

mission1_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission1_sign.tga
   }
}

mission2_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission2_sign.tga
   }
}

mission3_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission3_sign.tga
   }
}

mission4_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission4_sign.tga
   }
}

mission5_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission5_sign.tga
   }
}

mission6_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mission6_sign.tga
   }
}

changemap_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/changemap_sign.tga
   }
}

briefingtemp
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefingtemp.tga
	blendfunc blend
   }
}

/////////////////////////////////////////
//mission briefing specific artwork
/////////////////////////////////////////

briefing1a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing1a.tga
	blendfunc blend
   }
}

briefing2a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing2a.tga
	blendfunc blend
   }
}

briefing3a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing3a.tga
	blendfunc blend
   }
}

briefing4a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing4a.tga
	blendfunc blend
   }
}

briefing5a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing5a.tga
	blendfunc blend
   }
}

briefing6a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/briefing6a.tga
	blendfunc blend
   }
}

////////////////////////////////////////
//start of briefing pics
////////////////////////////////////////

arzewaerial
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/arzewaerial.tga
	//blendfunc blend
   }
}

afrika88s
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/afrika88s.tga
	//blendfunc blend
   }
}

portphoto
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/portphoto.tga 
	//blendfunc blend
   }
}

torch
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/torch.tga
	//blendfunc blend
   }
}

sabotage
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/sabotage.tga
	//blendfunc blend
   }
}

osseagle
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle.tga
	//blendfunc blend
   }
}

osseagle1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle1.tga
	//blendfunc blend
   }
}

osseagle2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle2.tga
	//blendfunc blend
   }
}

osseagle3
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle3.tga
	//blendfunc blend
   }
}

osseagle4
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle4.tga
	//blendfunc blend
   }
}

osseagle5
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle5.tga
	//blendfunc blend
   }
}

osseagle6
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/osseagle6.tga
	//blendfunc blend
   }
}

pill
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/pill.tga
	//blendfunc blend
   }
}

convoy
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/convoy.tga
	blendfunc blend
   }
}

shoreguns
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/shoreguns.tga
	//blendfunc blend
   }
}

bombers
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/bombers.tga
	//blendfunc blend
   }
}

rommel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/rommel.tga
	//blendfunc blend
   }
}

naxos
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/naxos.tga
	//blendfunc blend
   }
}

defeat
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/defeat.tga
	//blendfunc blend
   }
}

subpen
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/subpen.tga
	//blendfunc blend
   }
}

subsink
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/subsinnk.tga
	//blendfunc blend
   }
}

depot
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/depot.tga
	//blendfunc blend
   }
}

defenses
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/defenses.tga
	//blendfunc blend
   }
}

landingmap
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/landingmap.tga
	//blendfunc blend
   }
}

battleship
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/battleship.tga
	//blendfunc blend
   }
}

airborne
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/airborne.tga
	//blendfunc blend
   }
}

subsink
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/subsink.tga
	//blendfunc blend
   }
}

normandycoast
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/normandy.tga
	//blendfunc blend
   }
}

mannon
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/mannon.tga
   }
}

blurrytiger
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/blurrytiger.tga
	//blendfunc blend
   }
}

kingtiger
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/kingtiger.tga
	//blendfunc blend
   }
}

manorhouse
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/manorhouse.tga
	//blendfunc blend
   }
}

omahalanding
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/omahalanding.tga
	//blendfunc blend
   }
}

higginsboat
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/higginsboat.tga
	//blendfunc blend
   }
}

invasionfleet
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/invasionfleet.tga
	//blendfunc blend
   }
}

m5bridge
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/m5bridge.tga
	//blendfunc blend
   }
}

schmerzenaerial
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/schmerzenaerial.tga
   }
}

briefing_train
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/train.tga
   }
}

beachhead
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/beachhead.tga
   }
}

grillo_id
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/grillo_id.tga
   }
}

rangers
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/briefing/rangers.tga
   }
}

////////////////////////////////
//difficulty setting
////////////////////////////////

difficulty
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/difficulty.tga
	blendfunc blend
   }
}

/////////////////////////////////
//brad art for statscreen background
/////////////////////////////////

statscreen_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/statscreen_a.tga
   }
}

statscreen_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/statscreen_b.tga
   }
}

statscreen2_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/statscreen2_a.tga
   }
}

statscreen2_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/statscreen2_b.tga
   }
}

sepiaload_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/sepiaload_a.tga
   }
}

sepiaload_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/sepiaload_b.tga
   }
}

///////////////////////////////////////////
//weapon selection graphics
///////////////////////////////////////////

rifle
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/rifle.tga
   }
}

rifle_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/rifle_sign.tga
   }
}

sniperrifle
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/sniperrifle.tga
   }
}

sniperrifle_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/sniperrifle_sign.tga
   }
}

submachinegun
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/submachinegun.tga
   }
}

submachinegun_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/submachinegun_sign.tga
   }
}

machinegun
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/machinegun.tga
   }
}

machinegun_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/machinegun_sign.tga
   }
}

rocketlauncher
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/rocketlauncher.tga
   }
}

rocketlauncher_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/rocketlauncher_sign.tga
   }
}

cancel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/cancel.tga
   }
}

primaryweapon
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/primaryweapon.tga
   }
}

weapon_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/weapon_sign.tga
   }
}

shotgun_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/shotgun_sign.tga
   }
}

shotgunbutton
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/shotgun.tga
   }
}

clickfire
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/clickfire.tga
	blendfunc blend
   }
}

//////////////////////////////////////////
//legal screen
//////////////////////////////////////////

studio
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/studio.tga
//      rgbgen   vertex
      rgbgen   global
   }
}

legal
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/legal_screen.tga
//      rgbgen   vertex
      rgbgen   global
   }
}

mohaa_title
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/mohaa_title.tga
//      rgbgen   vertex
      rgbgen   global
   }
}

/////////////////////////////////////////////
// join multiplayer lan or internet butt5ons
/////////////////////////////////////////////

joingame
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/joingame.tga
	blendfunc blend
   }
}

join_game
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/join_game.tga
   }
}

join_game_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/join_game_h.tga
   }
}

forcemodels
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/forcemodels.tga
   }
}

///////////////////////////////////////
//confirm delete save game
///////////////////////////////////////

confirm_delete
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/confirm_delete.tga
   }
}

/////////////////////////////////////////////////
//misc multiplayer menu stuff
////////////////////////////////////////////////

selectteam
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectteam.tga
	blendfunc blend
   }
}

selectmodel
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectmodel.tga
	blendfunc blend
   }
}

objteamselect
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/objteamselect.tga
	blendfunc blend
   }
}

escmenu
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/escmenu.tga
	blendfunc blend
   }
}

disconnect
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/disconnect.tga
   }
}

disconnect_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/disconnect_h.tga
   }
}

//////////////////////////////////////////
//medal case medal labels
//////////////////////////////////////////

armycommendation_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/armycommendation_sign.tga
   }
}

bronzestar_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/bronzestar_sign.tga
   }
}

silverstar_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/silverstar_sign.tga
   }
}

armycommendation_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/armycommendation_sign.tga
   }
}

distinguishedcross_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/distinguishedcross_sign.tga
   }
}

distinguishedservice_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/distinguishedservice_sign.tga
   }
}

europeanmedal_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/europeanmedal_sign.tga
   }
}

goodconduct_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/goodconduct_sign.tga
   }
}

legionmerit_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/legionmerit_sign.tga
   }
}

norwegiancross_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/norwegiancross_sign.tga
   }
}

norwegiancross
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/norwegiancross.tga
   }
}

americanmedal
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/americanmedal.tga
   }
}

legion_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/legion_desc.tga
   }
}

norwegian_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/norwegian_desc.tga
   }
}

american_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/american_desc.tga
   }
}

distinguishedservice_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/distinguishedservice_desc.tga
   }
}

armycommendation_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/armycommendation_desc.tga
   }
}

bronzestar_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/bronzestar_desc.tga
   }
}

silverstar_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/silverstar_desc.tga
   }
}

distinguishedcross_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/distinguishedcross_desc.tga
   }
}

blank_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/blank_desc.tga
   }
}

goodconduct_desc
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/goodconduct_desc.tga
   }
}

americanmedal_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/americanmedal_sign.tga
   }
}


/////////////////////////////////////////
//final level loading briefings
/////////////////////////////////////////

m1l1_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l1_1.tga
   }
}

m1l1_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l1_2.tga
   }
}

m1l2a_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l2a_1.tga
   }
}

m1l2a_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l2a_2.tga
   }
}

m1l2b_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l2b_1.tga
   }
}

m1l2b_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l2b_2.tga
   }
}
m1l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l3_1.tga
   }
}

m1l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m1l3_2.tga
   }
}

m2l1_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l1_1.tga
   }
}

m2l1_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l1_2.tga
   }
}

m2l2_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l2_1.tga
   }
}

m2l2_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l2_2.tga
   }
}

m2l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l3_1.tga
   }
}

m2l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m2l3_2.tga
   }
}

m3l1a_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l1a_1.tga
   }
}

m3l1a_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l1a_2.tga
   }
}

m3l2_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l2_1.tga
   }
}

m3l2_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l2_2.tga
   }
}

m3l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l3_1.tga
   }
}

m3l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m3l3_2.tga
   }
}

m4l1_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l1_1.tga
   }
}

m4l1_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l1_2.tga
   }
}

m4l2_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l2_1.tga
   }
}

m4l2_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l2_2.tga
   }
}

m4l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l3_1.tga
   }
}

m4l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m4l3_2.tga
   }
}

m5l1ab_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l1ab_1.tga
   }
}

m5l1ab_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l1ab_2.tga
   }
}

m5l2ab_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l2ab_1.tga
   }
}

m5l2ab_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l2ab_2.tga
   }
}

m5l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l3_1.tga
   }
}

m5l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m5l3_2.tga
   }
}

m6l1ab_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l1ab_1.tga
   }
}

m6l1ab_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l1ab_2.tga
   }
}

m6l1c_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l1c_1.tga
   }
}

m6l1c_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l1c_2.tga
   }
}

m6l2a_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l2a_1.tga
   }
}

m6l2a_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l2a_2.tga
   }
}

m6l2b_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l2b_1.tga
   }
}

m6l2b_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l2b_2.tga
   }
}

m6l3_1
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l3_1.tga
   }
}

m6l3_2
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/levelbriefing/m6l3_2.tga
   }
}

/////////////////////////////////////////////////
//multiplayer options drop down arrow
/////////////////////////////////////////////////

multiarrow
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multiarrow.tga
   }
}

multiarrow_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/multiarrow_h.tga
   }
}


internet
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/internet.tga
	blendfunc blend
   }
}

trainingcourse_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/trainingcourse_sign.tga
   }
}

langame_a
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/langame_a.tga
	blendfunc blend
   }
}

langame_b
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/langame_b.tga
	blendfunc blend
   }
}

gallery_h
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/gallery_h.tga
   }
}

credits_sign
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/credits_sign.tga
   }
}

selectprimary
{
   nomipmaps
   nopicmip
   cull none
   force32bit
   surfaceparm nolightmap
   {
      clampMap textures/mohmenu/selectprimary.tga
	blendfunc blend
   }
}
