//E3 D-Day Modified Textures (ACDSee Enhance: 10% Blackpoint shift in Levels)
//Modifications by Steve Fukuda April 27 2001. 
//Based on textures from the Mohtest, Misc_Outside, and Test subdirectories.
//.TGAs were darkened slightly, no other changes were made.
//Original .TGAs are untouched and in correct directories.
//Modified .TGAs are copies located in the Ddaydark texture subdirectory.

textures/ddaydark/minefield_omaha_dark
{
	qer_editorimage textures/ddaydark/minefield_omaha_dark.tga
	qer_keyword m2
	qer_keyword terrain
	surfaceparm dirt
	{
		map textures/ddaydark/minefield_omaha_dark.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/grassbordertrans_obeachset1_dark
{
	qer_editorimage textures/ddaydark/grassbordertrans_obeachset1_dark.tga
	qer_keyword m2
	qer_keyword terrain
	surfaceparm dirt
	{
		map textures/ddaydark/grassbordertrans_obeachset1_dark.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/rubbledark
{
	qer_keyword natural
	qer_keyword floor
	qer_keyword flat
	qer_keyword dirt
	surfaceparm dirt
	{
		map textures/ddaydark/rubbledark.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/ddaydark/grassbrdr_set3_damage2_dark
{
	qer_keyword terrain
	qer_keyword m2
	qer_keyword dirt
	surfaceParm dirt
	{
		clampmap textures/ddaydark/grassbrdr_set3_damage2_dark.tga
		rgbGen identity
		depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
} 

textures/misc_outside/nugrass_set2atrans_dark
{
	qer_keyword grass
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm grass
	{
		map textures/misc_outside/nugrass_set2atrans_dark.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset2
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset2.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset_sline
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset_sline.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset5
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset5.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset3
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset3.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset9
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset9.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/omaha_darkset8
{
	qer_keyword wall
	qer_keyword tudor
	surfaceparm dirt
	{
		map textures/ddaydark/omaha_darkset8.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
} 

textures/ddaydark/darktrans4
{
	qer_keyword m2
	qer_keyword terrain
	surfaceparm dirt
	{
		clampmap textures/ddaydark/darktrans4.tga
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

textures/ddaydark/d-day3
{
	qer_editorimage textures/sky/dday_ed.tga
	qer_keyword utility
	qer_keyword sky
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	skyParms env/dday3 512 -
}