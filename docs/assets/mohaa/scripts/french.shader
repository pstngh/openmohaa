textures/french/advertisement_beer
{
	qer_editorimage textures/french/poster4.tga
	qer_keyword masked
	qer_keyword signs
	surfaceparm carpet
	surfaceparm fence
	surfaceparm alphashadow
	cull back
	nopicmip
	{
		map textures/french/poster4.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/pinup1duparispj
{
	qer_keyword wood
	qer_keyword tudor
	qer_keyword signs
	qer_keyword natural
	qer_keyword carpet
	surfaceparm nonsolid
	surfaceparm fence
	surfaceparm alphashadow
	surfaceparm wood
	qer_editorimage textures/french/pinup1duparispj.tga
	{
		clampmap textures/french/pinup1duparispj.tga
		alphaFunc GE128
		depthWrite
	nextbundle
		map $lightmap
	}
}

textures/french/revue_poster2
{
	qer_keyword masked
	qer_keyword signs
	qer_keyword m3
	qer_keyword wood
	qer_keyword plaster
	surfaceparm carpet
	surfaceparm fence
	surfaceparm alphashadow
	nopicmip
	cull back
	PolygonOffset
	qer_editorimage textures/french/revue_poster2.tga
	{
		map textures/french/revue_poster2.tga
		blendFunc blend
	nextbundle
		map $lightmap
	}
}

textures/french/brest
{
	qer_keyword signs
	qer_keyword wood
	surfaceparm wood
	{
		map textures/french/brest.tga
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

textures/french/cherbourg
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/cherbourg.tga
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

textures/french/laconquet
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/laconquet.tga
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

textures/french/paris
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/paris.tga
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

textures/french/sign_back
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/sign_back.tga
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

textures/french/st_lo_paris
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	PolygonOffset
	{
		map textures/french/st_lo_paris.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/st_renan
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/st_renan.tga
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

textures/french/1st_stage
{
	qer_keyword signs
	qer_keyword concrete
	qer_keyword wall
	qer_keyword masked
	surfaceparm stone
	PolygonOffset
	{
		map textures/french/1st_stage.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/butcher
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/butcher.tga
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

textures/french/dyer
{
	qer_keyword rusted
	qer_keyword metal
	qer_keyword signs
	surfaceparm metal
	{
		map textures/french/dyer.tga
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

textures/french/familistere
{
	qer_keyword masked
	qer_keyword wall
	qer_keyword concrete
	qer_keyword signs
	surfaceparm stone
	PolygonOffset
	{
		map textures/french/familistere.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/motosalerepair2
{
	qer_keyword wall
	qer_keyword signs
	qer_keyword masked
	qer_keyword concrete
	surfaceparm stone
	PolygonOffset
	{
		map textures/french/motosalerepair2.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/renault
{
	qer_keyword metal
	qer_keyword signs
	qer_keyword masked
	surfaceparm metal
	PolygonOffset
	{
		map textures/french/renault.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

textures/french/shepherd
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/shepherd.tga
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

textures/french/townhall
{
	qer_keyword wood
	qer_keyword signs
	surfaceparm wood
	{
		map textures/french/townhall.tga
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

textures/french/celeste_tavrnsin
{
	qer_keyword signs
	qer_keyword wood
	surfaceparm wood
	{
		map textures/french/celeste_tavrnsin.tga
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

textures/french/french_advert2
{
	qer_keyword signs
	qer_keyword wood
	surfaceparm wood
	{
		map textures/french/french_advert2.tga
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

textures/french/french_advert3
{
	qer_keyword signs
	qer_keyword wood
	surfaceparm wood
	{
		map textures/french/french_advert3.tga
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


