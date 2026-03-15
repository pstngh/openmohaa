
////////////////////////////
//Tall winter pine shaders//
////////////////////////////
// sprite has been hacked off for use in Foggy levels

static_tree5ssprite_nosprite
{
	qer_editorimage textures/models/natural/tree5ssprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree5ssprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 10001 1
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}
static_tree5s_1_nosprite //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen lightingSpherical
//		rgbGen constant .6 .6 .74
	}
}
static_tree5s_2_nosprite //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}
static_tree5s_3_nosprite //vertical cross sections
{
	qer_editorimage textures/models/natural/tree5s_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree5s_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		alphaGen oneMinusDistFade 10001 1
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}

/////winter small pine no sprite/////

static_tree5sb_1_nosprite //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
//		alphaGen distFade 900 0
	}
}
static_tree5sb_2_nosprite //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
//		alphaGen distFade 1500 800
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}

static_tree5sb_3_nosprite //vertical cross sections
{
	qer_editorimage textures/models/natural/tree5s_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree5s_3.tga
		depthWrite
		alphaFunc GE128
//		alphaGen distFade 1800 900
		alphaGen oneMinusDistFade 10001 1
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}
static_tree5sspriteb_nosprite
{
	qer_editorimage textures/models/natural/tree5sspriteb.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree5sspriteb.tga
		depthWrite
		alphaFunc GE128
//		alphaGen oneMinusDistFade 900 500
		alphaGen oneMinusDistFade 10001 1
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}

textures/TAnatural/nospritetreesundown
{
	qer_editorimage textures/TAnatural/nospritetreesundown.tga
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm alphashadow
	surfaceparm nonsolid
	{
		map textures/TAnatural/nospritetreesundown.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

textures/TAnatural/nospritetreesundown_trunk
{
	qer_editorimage textures/TAnatural/nospritetreesundown_trunk.tga
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm alphashadow
	surfaceparm nonsolid
	{
		map textures/TAnatural/nospritetreesundown_trunk.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

commontreespritedynamic // Sprite version
{

//	qer_editorimage textures/models/natural/tree1sprite.tga
	qer_editorimage textures/models/natural/tree1sprite_new.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
//		clampmap textures/models/natural/tree1sprite.tga
		clampmap textures/models/natural/tree1sprite_new.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512 //oneminustikidistfad would crash when the tree came into pvs
		rgbGen lightingSpherical
//		rgbGen constant .5 .5 .55
	}
}

textures/TAnatural/nospritetreesundown_lit
{
	qer_editorimage textures/TAnatural/nospritetreesundown_lit.tga
	surfaceparm nodraw
	surfaceparm castshadow
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm alphashadow
	surfaceparm nonsolid
	{
		map textures/TAnatural/nospritetreesundown_lit.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}
