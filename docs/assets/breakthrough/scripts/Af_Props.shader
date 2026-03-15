Af_P_Lantern
{
	cull none
	qer_editorimage textures/models/items/Lantern/Af_P_Lantern.tga
	{
		map textures/models/items/Lantern/Af_P_Lantern.tga
		alphafunc ge128
		rgbgen lightingSpherical
	}
}
//Af_P_Lantern_Glass
//{
//	cull none
//	surfaceparm glass
//	q3map_surfacelight 2000
//	{
//		map $lightmap
//		rgbGen identity
//	}
//	qer_editorimage textures/models/items/Lantern/Af_P_Lantern_Glass.tga
//	{
//		map textures/models/items/Lantern/Af_P_Lantern_Glass.tga
//		blendfunc blend
//		rgbgen lightingSpherical
//	}
//}
Af_P_Lantern_Glass
{
	qer_editorimage textures/models/items/Lantern/Af_P_Lantern_Glass.tga
	qer_keyword utility
	surfaceparm glass
	q3map_surfacelight 2000
	// light 1
	// test light
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/models/items/Lantern/Af_P_Lantern_Glass.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/models/items/Lantern/Af_P_Lantern_Glass.tga
		blendfunc GL_ONE GL_ONE
	}
}
Af_P_Keys
{
	qer_editorimage textures/models/static/Keys/Af_P_Keys.tga
	{
		map textures/models/static/Keys/Af_P_Keys.tga
		rgbGen identity
	}
}
Af_P_Keys_Pulsating
{
	qer_editorimage textures/models/static/Keys/Af_P_Keys.tga
	{
		map textures/models/static/Keys/Af_P_Keys.tga
		rgbGen identity
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}
Af_P_FoldedUni
{
	qer_editorimage textures/models/static/FoldedUni/Af_P_FoldedUni.tga
	{
		map textures/models/static/FoldedUni/Af_P_FoldedUni.tga
		rgbGen identity
	}
}
Af_P_FoldedUniThrob
{
	qer_editorimage textures/models/static/FoldedUni/Af_P_FoldedUni.tga
	{
		map textures/models/static/FoldedUni/Af_P_FoldedUni.tga
		rgbGen identity
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}
Af_P_LifePreserver
{
	qer_editorimage textures/models/static/LifePreserver/Af_P_LifePreserver.tga
	{
		map textures/models/static/LifePreserver/Af_P_LifePreserver.tga
		rgbGen vertex
	}
}
Af_P_CargoNetting
{
	cull none
	qer_editorimage textures/models/static/CargoNetting/Af_P_CargoNetting.tga
	{
		map textures/models/static/CargoNetting/Af_P_CargoNetting.tga
		alphafunc ge128
		rgbGen identity
	}
}
Af_P_Hook
{
	qer_editorimage textures/models/static/CargoNetting/Af_P_Hook.tga
	{
		map textures/models/static/CargoNetting/Af_P_Hook.tga
		rgbGen vertex
	}
}
Af_P_Glasses
{
	cull none
	qer_editorimage textures/models/equipment/Af_P_Glasses.tga
	{
		map textures/models/equipment/Af_P_Glasses.tga
		rgbGen lightingSpherical
//		alphaGen const 0.5
//		blendFunc blend
	}
}

Af_P_Glasses_reflection
{
	{
		map textures/common/Af_P_Glasses_reflection.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
//		alphaGen const 0.5
//		blendFunc blend
	}
}

////////////////////////////////////////////////////////////////////////
//Af_P_Reflection
//{
//	{
//		map textures/models/equipment/Af_P_Reflection.tga
//		rgbGen lightingSpherical
//		tcgen environmentmodel
//		alphaGen const 0.5
//		blendFunc blend
//	}
//}
///////////////////////////////////////////////////////////////////////
Af_P_Crane
{
	qer_editorimage textures/models/static/Ship_Crane/Af_P_Crane.tga
	{
		map textures/models/static/Ship_Crane/Af_P_Crane.tga
		rgbGen vertex
	}
}
Af_P_Beams
{
	cull none
	qer_editorimage textures/models/static/Ship_Crane/Af_P_Beams.tga
	{
		map textures/models/static/Ship_Crane/Af_P_Beams.tga
		alphafunc ge128
		rgbGen vertex
	}
}
Af_P_Wires
{
	cull none
	qer_editorimage textures/models/static/Ship_Crane/Af_P_Wires.tga
	{
		map textures/models/static/Ship_Crane/Af_P_Wires.tga
		alphafunc ge128
		rgbGen vertex
	}
}
Af_P_Flag
{
	cull none
	qer_editorimage textures/models/animate/Flag/Af_P_Flag.tga
	{
		map textures/models/animate/Flag/Af_P_Flag.tga
		rgbGen vertex
	}
}