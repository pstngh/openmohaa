flames
{
	qer_editorimage textures/models/vehicles/c47_damage_all/fire.tga
	{
		map textures/models/vehicles/c47_damage_all/fire.tga
		alphafunc ge128
		rgbGen lightingSpherical
		tcmod scroll 3 0
		alphaGen wave sawtooth -1 5 0 6
		
	}
}
window_fire
{
	qer_editorimage textures/models/vehicles/c47_damage_all/fire.tga
	{
		map textures/models/vehicles/c47_damage_all/fire.tga
		alphafunc GE128
		tcmod scroll 0 1
		rgbGen lightingSpherical
	}
}
C47_Prop
{
	qer_editorimage textures/models/vehicles/c47_damage_all/c47prop.tga
	{
		map textures/models/vehicles/c47_damage_all/c47prop.tga
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
C47_Destroyed
{
	cull none
	qer_editorimage textures/models/vehicles/C47_Destroyed/C47_Destroyed.tga
	{
		map textures/models/vehicles/C47_Destroyed/C47_Destroyed.tga
		rgbGen lightingSpherical
	}
}
