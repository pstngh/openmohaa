textures/mohcommon/bordered_window_border
{
	qer_editorimage textures/mohcommon/window32.tga
	sort banner
	cull none
	{
		map textures/mohcommon/window32.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/bordered_window_glass
{
	qer_editorimage textures/mohcommon/window32.tga
	cull none
	{
		map textures/mohcommon/envnormndy_day.tga
		tcGen environment
	}
	{
		map textures/mohcommon/window32.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/peacockgrill
{
	qer_editorimage textures/mohcommon/m_wndwiron1.tga
	sort banner
	cull none
	{
		map textures/mohcommon/m_wndwiron1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	{
		map $lightmap
		rgbGen Identity
		blendFunc GL_DST_COLOR GL_ZERO
	}
}

textures/mohcommon/stonewindow_border
{
	qer_editorimage textures/mohcommon/winbrdr1.tga
	surfaceparm trans
	surfaceparm alphashadow
	cull none
	{
		map textures/mohcommon/winbrdr1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}

// Duplicate from mohcommon.shader, this needs to be sorted out
//textures/mohcommon/panedwindow1
//{
//	qer_editorimage textures/mohcommon/window32_p1.tga
//	cull none
////	sort banner
//	{
////		map textures/models/weapons/m1garand/reflection1.tga
//		map textures/mohcommon/envnormndy_day.tga
//		tcGen environment
//	}
//	{
//		map textures/mohcommon/window32_p1.tga
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//	}
//	{
//		map $lightmap
//		rgbGen Identity
//		blendFunc GL_DST_COLOR GL_ZERO
//	}
//}

textures/mohcommon/jh_maskwin1
{
	qer_editorimage textures/mohcommon/jh_maskwin1.tga
	surfaceparm trans
	cull none
	nopicmip
	{
		map textures/mohcommon/jh_maskwin1.tga
		depthWrite
		alphaFunc GE128
	nextbundle
		map $lightmap
	}
}