textures/hud/crosshair
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

textures/hud/crosshair_friend
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	cull none
	{
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map textures/hud/crosshair_friend.tga
	}
}

//-------------------------------------------
// These are the HUD icons for weapon classes
//-------------------------------------------

//-------------------------------------
// These are the HUD icons for items
//-------------------------------------

//-------------------------------------------
// These are the shaders for the hud compass, health meter, and stealth meter
//-------------------------------------------

// the shader used for the player's health meter
textures/hud/potentialhealthmeter
{
	surfaceparm nolightmap
	nomipmaps
	nopicmip
	{
		clampmap textures/hud/potentialhealthmeter.tga
		blendFunc blend
		alphaGen oneMinusVertex
//		tcmod rotate 5
//		nextbundle
//		clampmap textures/hud/healthmeter.tga
//		tcmod rotate -5
	}
}

//-------------------------------------------
// These are the shaders for the hud ammo meter, and weapon clip meter
//-------------------------------------------

//-------------------------------------------
// These are shaders for the ammo bars
//-------------------------------------------

//-------------------------------------------
// This is the overlay that's done when the player is zoomed
//-------------------------------------------

//------------------------------------------- //
// Team icons for use over teammates & on hud //
//------------------------------------------- //

textures/hud/talking_headicon
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
		clampmap textures/hud/talking.tga
	}
}

textures/hud/inmenu_headicon
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
		clampmap textures/hud/inmenu.tga
	}
}

textures/hud/inmenu_artilleryicon
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
		clampmap textures/hud/artillery.tga
	}
}
