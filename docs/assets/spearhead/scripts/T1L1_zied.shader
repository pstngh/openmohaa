textures/taholland/furroughed_earth_noshadow
{
	qer_editorimage textures/misc_outside/furroughed_earth1.tga
	qer_keyword dirt
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm trans
	{
		map textures/misc_outside/furroughed_earth1.tga
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
textures/taholland/furroughed_earth1ud_noshadow
{
	qer_editorimage textures/misc_outside/furroughed_earth1ud.tga
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm trans
	{
		map textures/misc_outside/furroughed_earth1ud.tga
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
textures/taholland/furroughed_earth1lr_noshadow
{
	qer_editorimage textures/misc_outside/furroughed_earth1lr.tga
	qer_keyword gouge
	qer_keyword floor
	qer_keyword natural
	qer_keyword flat
	surfaceparm dirt
	surfaceparm trans
	{
		map textures/misc_outside/furroughed_earth1lr.tga
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

textures/tanatural/rubble2c
{
	qer_editorimage textures/mohtest/rubble2c.tga
	qer_keyword natural
	qer_keyword floor
	qer_keyword flat
	qer_keyword dirt
	surfaceparm dirt
	surfaceparm trans
	{
		map textures/mohtest/rubble2c.tga
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

textures/general_industrial/ibeam_vert_culled
{
	qer_editorimage textures/general_industrial/ibeam_vert.tga
	qer_keyword trim
	qer_keyword rusted
	qer_keyword metal
	surfaceParm metal
	{
		map textures/general_industrial/ibeam_vert.tga
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