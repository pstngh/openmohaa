textures/common/blastmark3
{
	qer_keyword special
	qer_keyword masked
	surfaceparm metal
	surfaceparm fence
    qer_editorimage textures/common/blastmark3.tga
	PolygonOffset
	nopicmip
	{
		map textures/common/blastmark3.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}


textures/common/black
{
	qer_keyword utility
	surfaceparm nolightmap
	{
		map textures/common/black.tga
		blendFunc GL_ONE GL_ZERO
	}
} 

textures/common/vis
{
	qer_editorimage textures/common/vis.tga
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nolightmap
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm structural
//	surfaceparm trans
	surfaceparm hint
}

textures/common/trigger
{
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nodraw
} 

textures/common/rain
{
	qer_editorimage textures/common/rain.tga
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nodraw
}

textures/common/origin
{
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm origin
}

textures/common/caulk
{
	qer_editorimage textures/common/caulk.tga // needed for recognition by the fence tracing system
	qer_keyword utility
	surfaceparm nodraw
	surfaceparm nomarks
}

textures/common/sunblock
{
	qer_editorimage textures/common/sunblock.tga // needed for recognition by the fence tracing system
	surfaceparm alphashadow
	qer_keyword utility
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm trans	
}

// For removing the sky entirely, mainly for fogged levels
textures/common/caulksky
{
	qer_keyword utility
	qer_keyword sky
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm sky
	skyParms env/idontexist 512 - 
}

textures/common/caulkshadow
{
	qer_keyword utility
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
}

textures/common/nodraw
{
	qer_editorimage textures/common/nodraw.tga // needed for recognition by the fence tracing system
	qer_keyword utility
	qer_nocarve
//	qer_trans .3
		// made it trans again

	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
} 

textures/common/patharea
{
	qer_editorimage textures/common/patharea.tga
	qer_keyword utility
	qer_nocarve
	qer_trans .5
}

textures/common/clip
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
}

// for use as a collision volume for foliage
textures/common/foliageclip
{
	qer_editorimage textures/common/foliageclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm foliage
   	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm shootonly // only collide with bullets
	surfaceparm trans
}

// for use as a collision volume for tree trunks & amodel wooden furniture
textures/common/woodclip
{
	qer_editorimage textures/common/woodclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm wood
	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for paper (pretty useless!)(probably using this for cloth or cushion models, like beds or chairs)
textures/common/paperclip
{
	qer_editorimage textures/common/paperclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm paper
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
}

// for use as a collision volume for metal
textures/common/metalclip
{
	qer_editorimage textures/common/metalclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm metal
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for tanks
textures/common/tankclip
{
	qer_editorimage textures/common/tankclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm metal
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
   	surfaceparm nodraw
	surfaceparm nomarks
} 

// for use as a collision volume for stone
textures/common/stoneclip
{
	qer_editorimage textures/common/stoneclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm stone
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for dirt
textures/common/dirtclip
{
	qer_editorimage textures/common/dirtclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm dirt
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
}

// for use as a collision volume for mud
textures/common/mudclip
{
	qer_editorimage textures/common/mudclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm mud
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for grill
textures/common/grillclip
{
	qer_editorimage textures/common/grillclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm grill
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
} 

// for use as a collision volume for barbwire bplane
textures/common/bplaneclip
{
	qer_editorimage textures/models/items/bplane.tga
	qer_keyword utility
//	qer_trans .4
	cull none
	surfaceparm grill
	surfaceparm fence
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
   	surfaceparm nodraw
	surfaceparm nomarks
} 

// for use as a collision volume for barbwire bspindle
textures/common/bspindleclip
{
	qer_editorimage textures/models/items/bspindle.tga
	qer_keyword utility
//	qer_trans .4
	cull none
	surfaceparm grill
	surfaceparm fence
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
   	surfaceparm nodraw
	surfaceparm nomarks
} 

// for use as a collision volume for hedgehogs
textures/common/hedgehogclip
{
	qer_editorimage textures/models/items/hedgehog.tga
	qer_keyword utility
//	qer_trans .4
	cull none
	surfaceparm grill
	surfaceparm fence
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
   	surfaceparm nodraw
	surfaceparm nomarks
} 

// for use as a collision volume for grass
textures/common/grassclip
{
	qer_editorimage textures/common/grassclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm grass
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for puddle
textures/common/puddleclip
{
	qer_editorimage textures/common/puddleclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm puddle
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for glass
textures/common/glassclip
{
	qer_editorimage textures/common/glassclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm glass
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for gravel
textures/common/gravelclip
{
	qer_editorimage textures/common/gravelclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm gravel
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

// for use as a collision volume for sand
textures/common/sandclip
{
	qer_editorimage textures/common/sandclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm sand
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
} 

textures/common/snowclip
{
	qer_editorimage textures/common/snowclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm snow
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
}

// for use as a collision volume for carpet
textures/common/carpetclip
{
	qer_editorimage textures/common/carpetclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm carpet
   	surfaceparm nodraw
	surfaceparm trans
	surfaceparm nomarks
}

textures/common/monster
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm monsterclip
}

textures/common/playerclip
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm playerclip
}

textures/common/weapon
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm weaponclip
}

textures/common/clipall
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm vehicleclip	
	surfaceparm weaponclip
}

textures/common/hint
{
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm structural
//	surfaceparm trans
	surfaceparm hint
}

textures/common/skip
{
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm noimpact
//	surfaceparm structural
	surfaceparm trans	
}

textures/common/waterskip
{
	qer_editorimage textures/common/skip.tga
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm water
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/areaportal
{
	qer_keyword utility
	qer_nocarve
	qer_trans .4
	surfaceparm areaportal
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
} 

textures/common/portal
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nolightmap
	portal
	{
		map textures/common/portal.tga
		blendFunc GL_ZERO GL_ONE
		depthWrite
	}
}

textures/common/light
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nolightmap
	surfaceColor 1 1 1
	{
		map textures/common/light.tga
	}
}

textures/common/ladder
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodraw
	surfaceparm ladder
		
	{
		map textures/common/ladder.tga
	}
}

textures/common/skyportal
{
	qer_keyword utility
	surfaceparm nolightmap
	surfaceparm sky
	portalsky
	{
		map $whiteimage
		blendFunc GL_ZERO GL_ONE
		depthWrite
	}
}

textures/common/modelshader
{
	qer_keyword utility
	qer_trans .4
	surfaceparm nodraw
	surfaceparm nolightmap
}

// projectionShadow is used for cheap squashed model shadows
projectionShadow
{
	polygonOffset
	deformVertexes projectionShadow
	{
		map *white
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen constant 0 0 0
		alphaGen entity
	}
}

// markShadow is the very cheap blurry blob underneat the player
markShadow
{
	polygonOffset
	{
		map textures/common/shadow.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}

beamshader
{
	surfaceparm nolightmap
	sort additive
	{
		map $whiteimage
		blendFunc add
		rgbGen vertex
		alphaGen vertex
	}
}

flatbeamshader
{
	surfaceparm nolightmap
	{
		map $whiteimage
		blendFunc GL_ONE GL_ZERO
		rgbGen vertex
		alphaGen vertex
	}
}

flare
{
	cull none
	{
		map textures/sprites/flare.tga
		blendFunc add
		rgbGen vertex
	}
}

flareshader
{
	cull none
	{
		map textures/sprites/flare.tga
		blendFunc add
		rgbGen vertex
	}
}

textures/common/white_volumetric
{
	qer_editorimage textures/common/qer_volumetric_fade.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	sort additive
	cull none
	{
		map $whiteimage
		blendFunc add
		rgbGen constant .15 .15 .15
	nextbundle
		map textures/common/qer_volumetric_fade.tga
	}
	{
		map textures/common/dust.tga
		blendFunc add
		rgbGen constant .2 .2 .2
		tcMod scroll -.1 0
		tcMod scale 4 4
	nextbundle
		map textures/common/qer_volumetric_fade.tga
	}
}

textures/common/adjustable_color
{
	qer_editorimage textures/common/white.tga
	surfaceparm nolightmap
	{
		map $whiteimage
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen fromEntity
	}
}

textures/common/adjustable_volumetric
{
	qer_editorimage textures/common/qer_volumetric_fade.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	cull none
	sort additive
	{
		map textures/common/dust.tga
		blendFunc add
		rgbGen fromEntity
		tcMod scroll .1 0
		tcMod scale 2 2
	nextbundle
		map textures/common/qer_volumetric_fade.tga
	}
	{
		map textures/common/dust.tga
		blendFunc add
		rgbGen fromEntity
		tcMod scroll -.03 0
		tcMod scale 4 4
	nextbundle
		map textures/common/qer_volumetric_fade.tga
	}
}

textures/common/adjustable_volumetric2
{
	qer_editorimage textures/common/qer_volumetric_fade.tga
	qer_keyword utility
	qer_trans .4	
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	cull none
	sort additive
	{
		map textures/common/volumetric_fade.tga
		blendFunc add
		rgbGen fromEntity
	}
}

textures/common/rope
{
	qer_editorimage textures/sprites/rope.tga
	surfaceparm nolightmap
	{
		map textures/sprites/rope.tga
	}
}

blank
{
	{
		map $whiteimage
		rgbGen identity
	}
}

blank_spherical
{
	{
		map $whiteimage
		rgbGen lightingSpherical
	}
}

textures/common/blank_lightmap
{
	qer_editorimage textures/common/caulk.tga
	{
		map $whiteimage
		nextbundle
		map $lightmap
	}
}

textures/common/static_visible
{
	qer_editorimage textures/common/caulk.tga
	surfaceparm nonsolid
	{
		map $whiteimage
		blendfunc blend
//		rgbGen const 1.0 1.0 1.0
		alphaGen const 0.3
		nextbundle
		map $lightmap
	}
}

// for lighting test sphere
blank_spherical_gray
{
	{
		map $whiteimage
		rgbGen lightingSpherical
	}
	{
		map $whiteimage
		rgbGen const .5 .5 .5
		blendFunc filter
	}
}
// for testing alphagen dot
dotTest
{
	{
		map $whiteimage
		rgbGen identity
		blendfunc blend
		alphaGen dot 0 2
	}
}
oneMinusDotTest
{
	{
		map $whiteimage
		rgbGen identity
		blendfunc blend
		alphaGen oneMinusDot 0 2
	}
}

static_blank
{
	{
		map $whiteimage
		rgbGen vertex
	}
}
static_cullblank
{
     cull none
	{
		map $whiteimage
		rgbGen vertex
	}
}
textures/common/vehicleclip
{
	qer_editorimage textures/common/vehicleclip.tga
	qer_keyword utility
	qer_trans .4
	surfaceparm nodamage
	surfaceparm nodraw
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm vehicleclip
	surfaceparm nomarks
	surfaceparm trans
}

showgrid
{
	deformVertexes autosprite
	cull none
	{
		map $whiteimage
		rgbGen vertex
	}
}

textures/common/switchflat
{
	surfaceparm metal
	{
		map textures/common/switchflat.tga
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

textures/common/switchflat_pulse
{
	surfaceparm metal
	{
		map textures/common/switchflat.tga
		depthWrite
		rgbGen identity
	}
	{ // pulsating layer
		map textures/models/items/pulse.tga
		blendFunc GL_SRC_ALPHA GL_ONE // this is an additive blend that uses alpha
		rgbGen wave sin 0.25 0.25 0 0.75
		//rgbGen wave sin 0.15 0.075 0 0.75
		alphaGen distFade 1024 512 // this makes the pulsating fade when you go away from it
	}
}
