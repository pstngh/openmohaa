garand
{
	qer_editorimage textures/models/weapons/m1garand/garand.tga
	{
		map textures/models/weapons/m1garand/garand.tga
		rgbGen lightingSpherical
	}
}
m1clip
{
	qer_editorimage textures/models/weapons/m1garand/m1clip.tga
	cull none
	{
		map textures/models/weapons/m1garand/m1clip.tga
		rgbGen lightingSpherical
	}
}
sniperclip
{
	qer_editorimage textures/models/weapons/springfield/sniperclip.tga
	cull none
	{
		map textures/models/weapons/springfield/sniperclip.tga
		rgbGen lightingSpherical
	}
}
//30cal


static_garand
{
	qer_editorimage textures/models/weapons/m1garand/garand.tga
	{
		map textures/models/weapons/m1garand/garand.tga
		rgbGen static
	}
}

static_30cal
{
	qer_editorimage textures/models/weapons/30cal/30cal.tga
	cull none
	{
		map textures/models/weapons/30cal/30cal.tga
		rgbGen static
		alphaFunc GE128
		depthWrite
	}
	{
		map textures/common/reflection1.tga
		blendFunc blend
		depthFunc lequal
		rgbGen static
		tcGen environmentmodel
	nextbundle
		map textures/models/weapons/30cal/30calholes.tga
		tcGen base
	}
}

30cal
{
	qer_editorimage textures/models/weapons/30cal/30cal.tga
	cull none
	{
		map textures/models/weapons/30cal/30cal.tga
		rgbGen lightingSpherical
		alphaFunc GE128
		depthWrite
	}
	{
		map textures/common/reflection1.tga
		blendFunc blend
		depthFunc lequal
		rgbGen lightingSpherical
		tcGen environmentmodel
	nextbundle
		map textures/models/weapons/30cal/30calholes.tga
		tcGen base
	}
}

30calmount
{
	qer_editorimage textures/models/weapons/30cal/30calmount.tga
	{
		map textures/models/weapons/30cal/30calmount.tga
		rgbGen lightingSpherical
	}
}

ThompsonSMG
{
	qer_editorimage textures/models/weapons/ThompsonSMG/ThompsonSMG.tga
	{
		map textures/models/weapons/ThompsonSMG/ThompsonSMG.tga
		rgbGen lightingSpherical
	}
}

thompsite
{
	qer_editorimage textures/models/weapons/ThompsonSMG/thompsite.tga
	cull none
	{
		map textures/models/weapons/ThompsonSMG/thompsite.tga
		rgbGen lightingSpherical
		alphaFunc GE128
		depthWrite
	}
}

static_ThompsonSMG
{
	qer_editorimage textures/models/weapons/ThompsonSMG/ThompsonSMG.tga
	{
		map textures/models/weapons/ThompsonSMG/ThompsonSMG.tga
		rgbGen vertex
	}
}
//springfield sniper rifle
springfield
{
	qer_editorimage textures/models/weapons/springfield/springfield.tga
	{
		map textures/models/weapons/springfield/springfield.tga
		rgbGen lightingSpherical
	}
}

lens
{
	{
		map textures/common/reflection1.tga
		rgbGen lightingSpherical
		tcgen environmentmodel
		alphaGen const 0.5
		blendFunc blend
	}
}

//(
//	{
//		map textures/common/reflection2.tga
//		tcgen environment
//		alphagen const .1
//		depthwrite
//		blendFunc blend
//	}
//)

//colt 45

colt
{
	qer_editorimage textures/models/weapons/colt45/colt45.tga
//	{
//		map textures/common/reflection1.tga
//		rgbGen lightingSpherical
//		tcGen environmentmodel
//	}
	{
		map textures/models/weapons/colt45/colt45.tga
		rgbGen lightingSpherical
//		blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
	}
}
us_bomb
{
	qer_editorimage models/ammo/us_bomb/us_bomb.tga
	{
		map models/ammo/us_bomb/us_bomb.tga
		rgbGen lightingSpherical
	}
}
bar
{
	qer_editorimage textures/models/weapons/bar/bar.tga
	{
		map textures/models/weapons/bar/bar.tga
		rgbGen lightingSpherical
	}
}
barclip
{
	qer_editorimage textures/models/weapons/bar/barclip.tga
	{
		map textures/models/weapons/bar/barclip.tga
		rgbGen lightingSpherical
	}
}
bazooka
{
	qer_editorimage textures/models/weapons/bazooka/bazooka.tga
	{
		map textures/models/weapons/bazooka/bazooka.tga
		rgbGen lightingSpherical
	}
}
bazookarim
{
	qer_editorimage textures/models/weapons/bazooka/bazookarim.tga
	cull none
	{
		map textures/models/weapons/bazooka/bazookarim.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
bazookashell
{
	qer_editorimage textures/models/weapons/bazooka/bazookashell.tga
	cull none
	{
		map textures/models/weapons/bazooka/bazookashell.tga
		rgbGen lightingSpherical
		alphaFunc ge128
		depthWrite
	}
}
shotgun
{
	qer_editorimage textures/models/weapons/winchester/winchester.tga
	{
		map textures/models/weapons/winchester/winchester.tga
		rgbGen lightingSpherical
	}
}
hi_standard
{
	qer_editorimage textures/models/weapons/hi_standard/hi_standard.tga
	{
		map textures/models/weapons/hi_standard/hi_standard.tga
		rgbGen lightingSpherical
	}
}