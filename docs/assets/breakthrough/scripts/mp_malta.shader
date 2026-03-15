textures/mp_malta/ironwork_side
{
cull none
nopicmip
surfaceparm grill
qer_keyword masked
qer_keyword metal
qer_keyword malta
surfaceparm nonsolid
surfaceparm monsterclip
surfaceparm playerclip
surfaceparm fence
surfaceparm alphashadow
 qer_editorimage textures/mp_malta/ironwork_side.tga
 {
  map textures/mp_malta/ironwork_side.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
 }
}

textures/mp_malta/balcony_stone1_alpha
{
	qer_keyword stone
	qer_keyword concrete
	qer_keyword wall
	qer_keyword malta
	surfaceparm alphashadow
	surfaceparm playerclip
	surfaceparm monsterclip
	surfaceparm nomarks
	surfaceparm fence
	surfaceparm rock
	{
		map textures/mp_malta/balcony_stone1_alpha.tga
		alphafunc GE128
		rgbGen identity
		depthWrite depthWrite
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
		depthFunc equal
	}
}

textures/mp_malta/arrow_slit
{
	qer_keyword stone
	qer_keyword concrete
	qer_keyword wall
	qer_keyword malta
	surfaceparm rock
	{
		map textures/mp_malta/arrow_slit.tga
	nextbundle
		map $lightmap
	}

}
