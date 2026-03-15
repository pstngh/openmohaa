// Pine textures
pine_radial
{
	qer_editorimage textures/models/natural/pineradial.tga
	cull none
	{
		clampmap textures/models/natural/pineradial.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pine_cross
{
	qer_editorimage textures/models/natural/pinecross.tga
	cull none
	{
		clampmap textures/models/natural/pinecross.tga
		alphafunc GE128
		alphaGen dot
		rgbGen lightingSpherical
	}
}
pine_branches
{
	qer_editorimage textures/models/natural/pinebranchs.tga
	cull none
	{
		clampmap textures/models/natural/pinebranchs.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Static Pine textures
static_pine_radial
{
	qer_editorimage textures/models/natural/pineradial.tga
	cull none
	{
		clampmap textures/models/natural/pineradial.tga
		depthWrite
		alphafunc GE128
		rgbgen vertex
	}
}
static_pine_cross
{
	qer_editorimage textures/models/natural/pinecross.tga
	cull none
	{
		clampmap textures/models/natural/pinecross.tga
		alphafunc GE128
		alphaGen dot
		rgbgen vertex
	}
}
static_pine_branches
{
	qer_editorimage textures/models/natural/pinebranchs.tga
	cull none
	{
		clampmap textures/models/natural/pinebranchs.tga
		depthWrite
		alphafunc GE128
		rgbgen vertex
	}
}


// Snow covered pine tree
pine_radial_snow
{
	qer_editorimage textures/models/natural/pineradial_snow.tga
	cull none
	{
		clampmap textures/models/natural/pineradial_snow.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
pine_cross_snow
{
	qer_editorimage textures/models/natural/pinecross_snow.tga
	cull none
	{
		clampmap textures/models/natural/pinecross_snow.tga
		alphafunc GE128
		alphaGen dot
		rgbGen lightingSpherical
	}
}
pine_branches_snow
{
	qer_editorimage textures/models/natural/pinebranchs_snow.tga
	cull none
	{
		clampmap textures/models/natural/pinebranchs_snow.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Snow covered pine tree
static_pine_radial_snow
{
	qer_editorimage textures/models/natural/pineradial_snow.tga
	cull none
	{
		clampmap textures/models/natural/pineradial_snow.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_pine_cross_snow
{
	qer_editorimage textures/models/natural/pinecross_snow.tga
	cull none
	{
		clampmap textures/models/natural/pinecross_snow.tga
		alphafunc GE128
		alphaGen dot
		rgbGen vertex
	}
}
static_pine_branches_snow
{
	qer_editorimage textures/models/natural/pinebranchs_snow.tga
	cull none
	{
		clampmap textures/models/natural/pinebranchs_snow.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// General tree textures
trunk1drk
{
	qer_editorimage textures/models/natural/trunk1drk.tga
	{
		map textures/models/natural/trunk1drk.tga
		rgbGen lightingSpherical
	}
}
N_trunk
{
	qer_editorimage textures/models/natural/trunk_s.tga
	{
		map textures/models/natural/trunk_s.tga
		rgbGen lightingSpherical
	}
}

// Test tree textures
tree_horiz
{
	qer_editorimage textures/models/natural/tree_horiz.tga
	cull none
	{
		clampmap textures/models/natural/tree_horiz.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree_radial
{
	qer_editorimage textures/models/natural/tree_radial.tga
	cull none
	{
		clampmap textures/models/natural/tree_radial.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree_crossect
{
	qer_editorimage textures/models/natural/tree_crossect.tga
	cull none
	{
		clampmap textures/models/natural/tree_crossect.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree_new_end1
{
	qer_editorimage textures/models/natural/new_end1.tga
	cull none
	{
		clampmap textures/models/natural/new_end1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree_trunk1root
{
	qer_editorimage textures/models/natural/trunk1root.tga
	{
		map textures/models/natural/trunk1root.tga
		rgbGen lightingSpherical
	}
}
tree_limb1drk
{
	qer_editorimage textures/models/natural/limb1drk.tga
	{
		map textures/models/natural/limb1drk.tga
		rgbGen lightingSpherical
	}
}

// Tree1-regular tree
tree1_1	// Trunk
{
	qer_editorimage textures/models/natural/tree1_1.tga
	{
		map textures/models/natural/tree1_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 2304 0
	}
}
tree1_2 // Small fill branches
{
	qer_editorimage textures/models/natural/tree1_2.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 256
		rgbGen lightingGrid
	}
}
tree1_3 // Cross section
{
	qer_editorimage textures/models/natural/tree1_3.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen lightingGrid
	}
}
tree1_4 // Horizontal cross section
{
	qer_editorimage textures/models/natural/tree1_4.tga
	nomipmaps
	
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 256 256
		rgbGen lightingGrid
	}
}
tree1_5 // Fill leaves
{
	qer_editorimage textures/models/natural/tree1_5.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_5.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbGen lightingGrid
	}
}
tree1sprite // Sprite version
{
	qer_editorimage textures/models/natural/tree1sprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree1sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen lightingGrid
	}
}

// Tree1-regular tree
static_tree1_1	// Trunk
{
	qer_editorimage textures/models/natural/tree1_1.tga
	{
		map textures/models/natural/tree1_1.tga
		alphaGen distFade 2304 0
		rgbgen vertex
	}
}
static_tree1_2 // Small fill branches
{
	qer_editorimage textures/models/natural/tree1_2.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 256
		rgbgen vertex
	}
}
static_tree1_3 // Cross section
{
	qer_editorimage textures/models/natural/tree1_3.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbgen vertex
	}
}
static_tree1_4 // Horizontal cross section
{
	qer_editorimage textures/models/natural/tree1_4.tga
	nomipmaps
	
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree1_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 256 256
		rgbgen vertex
	}
}
static_tree1_5 // Fill leaves
{
	qer_editorimage textures/models/natural/tree1_5.tga
	nomipmaps
	cull none
//	good low wind values
//	deformVertexes flap t 24 sin 2 3 0   .25 1 0

	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0

//	deformVertexes flap <s|t> <div> <function> <base> <amplitude> <phase> <frequency> <optional min> <optional max>
//	deformVertexes flap t     128   sin        0      4           0       .2          1              0
//	deformVertexes flap t     128   sin        0      4          .25      .25         1              0
	{
		clampmap textures/models/natural/tree1_5.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbgen vertex
	}
}
static_tree1sprite // Sprite version
{
	qer_editorimage textures/models/natural/tree1sprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree1sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbgen vertex
	}
}


// Tree2  //birch
tree2_1 //trunk
{
	qer_editorimage textures/models/natural/tree2_1.tga
	{
		map textures/models/natural/tree2_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 1000 600
	}
}

tree2_2 //vertacle cross plains
{
	qer_editorimage textures/models/natural/tree2_2.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen lightingGrid
	}
}

tree2_3 //fill plains
{
	qer_editorimage textures/models/natural/tree2_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree2_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		rgbGen lightingGrid
	}
}

tree2_4 //horizontal plains
{
	qer_editorimage textures/models/natural/tree2_4.tga
	nomipmaps
	cull none
//	check into these, not that important, get the rest looking right 1st
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 512
		rgbGen lightingGrid
	}
}

tree2sprite //birch sprite
{
	qer_editorimage textures/models/natural/tree2sprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree2sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbGen lightingGrid
	}
}

// Tree2  //birch
static_tree2_1 //trunk
{
	qer_editorimage textures/models/natural/tree2_1.tga
	{
		map textures/models/natural/tree2_1.tga
		alphaGen distFade 1000 600
		rgbgen vertex
	}
}
static_tree2_2 //vertacle cross plains
{
	qer_editorimage textures/models/natural/tree2_2.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbgen vertex
	}
}
static_tree2_3 //fill plains
{
	qer_editorimage textures/models/natural/tree2_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree2_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		rgbgen vertex
	}
}
static_tree2_4 //horizontal plains
{
	qer_editorimage textures/models/natural/tree2_4.tga
		nomipmaps
	cull none
//	check into these, not that important, get the rest looking right 1st
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 512
		rgbgen vertex
	}
}
static_tree2sprite //birch sprite
{
	qer_editorimage textures/models/natural/tree2sprite.tga
		qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree2sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbgen vertex
	}
}



// Tree2a //juniper
tree2a_1 //trunk
{
	qer_editorimage textures/models/natural/tree2a_1.tga
	{
		map textures/models/natural/tree2a_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 1000 600
	}
}

tree2a_2 //vertacle cross plains
{
	qer_editorimage textures/models/natural/tree2a_2.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2a_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen lightingGrid
	}
}

tree2a_3 //fill plains
{
	qer_editorimage textures/models/natural/tree2a_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree2a_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		rgbGen lightingGrid
	}
}

tree2a_4 //horizontal plains
{
	qer_editorimage textures/models/natural/tree2a_4.tga
		nomipmaps
	cull none
//	check into these, not that important, get the rest looking right 1st
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree2a_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 512
		rgbGen lightingGrid
	}
}

tree2asprite //juniper sprite
{
	qer_editorimage textures/models/natural/tree2asprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree2asprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbGen lightingGrid
	}
}

// Tree2a //juniper
static_tree2a_1 //trunk
{
	qer_editorimage textures/models/natural/tree2a_1.tga
	{
		map textures/models/natural/tree2a_1.tga
		rgbGen vertex
		alphaGen distFade 1000 600
	}
}

static_tree2a_2 //vertacle cross plains
{
	qer_editorimage textures/models/natural/tree2a_2.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree2a_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen vertex
	}
}

static_tree2a_3 //fill plains
{
	qer_editorimage textures/models/natural/tree2a_3.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 3.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 3.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree2a_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2100 1500
		rgbGen vertex
	}
}

static_tree2a_4 //horizontal plains
{
	qer_editorimage textures/models/natural/tree2a_4.tga
	nomipmaps
	cull none
	deformVertexes wave 24 sin 0 1.5 0    0.2 
	deformVertexes wave 24 sin 0 1.5 0.25 0.3 
	{
		clampmap textures/models/natural/tree2a_4.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 512 512
		rgbGen vertex
	}
}

static_tree2asprite //juniper sprite
{
	qer_editorimage textures/models/natural/tree2asprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree2asprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbGen vertex
	}
}


// Tree3-small pine tree
tree3_1
{
	qer_editorimage textures/models/natural/tree3_1.tga
	{
		map textures/models/natural/tree3_1.tga
		rgbGen lightingSpherical
	}
}
tree3_2
{
	qer_editorimage textures/models/natural/tree3_2.tga
	cull none
	{
		clampmap textures/models/natural/tree3_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree3_3
{
	qer_editorimage textures/models/natural/tree3_3.tga
	cull none
	{
		clampmap textures/models/natural/tree3_3.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Tree3-small pine tree
static_tree3_1
{
	qer_editorimage textures/models/natural/tree3_1.tga
	{
		map textures/models/natural/tree3_1.tga
		rgbGen vertex
	}
}
static_tree3_2
{
	qer_editorimage textures/models/natural/tree3_2.tga
	cull none
	{
		clampmap textures/models/natural/tree3_2.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_tree3_3
{
	qer_editorimage textures/models/natural/tree3_3.tga
	cull none
	{
		clampmap textures/models/natural/tree3_3.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Tree3s-small pine tree
tree3s_1
{
	qer_editorimage textures/models/natural/tree3s_1.tga
	{
		map textures/models/natural/tree3s_1.tga
		rgbGen lightingSpherical
	}
}
tree3s_2
{
	qer_editorimage textures/models/natural/tree3s_2.tga
	cull none
	{
		clampmap textures/models/natural/tree3s_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree3s_3
{
	qer_editorimage textures/models/natural/tree3s_3.tga
	cull none
	{
		clampmap textures/models/natural/tree3s_3.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Tree3s-small pine tree
static_tree3s_1
{
	qer_editorimage textures/models/natural/tree3s_1.tga
	{
		map textures/models/natural/tree3s_1.tga
		rgbGen vertex
	}
}
static_tree3s_2
{
	qer_editorimage textures/models/natural/tree3s_2.tga
	cull none
	{
		clampmap textures/models/natural/tree3s_2.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_tree3s_3
{
	qer_editorimage textures/models/natural/tree3s_3.tga
	cull none
	{
		clampmap textures/models/natural/tree3s_3.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Tree4  //Oak type tree
tree4_1 //trunk
{
	qer_editorimage textures/models/natural/tree4_1.tga
	{
		map textures/models/natural/tree4_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 2000 600
	}
}

tree4_2 //vertical cross section
{
	qer_editorimage textures/models/natural/tree4_2.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree4_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2000 1500
		rgbGen lightingGrid
	}
}
tree4_3 //fill plains
{
	qer_editorimage textures/models/natural/tree4_3.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree4_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1400 1400
		rgbGen lightingGrid
	}
}

tree4_4 //horizontal cross plains
{
	qer_editorimage textures/models/natural/tree4radial.tga
	nomipmaps
	cull none
//	check into these, not that important, get the rest looking right 1st
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree4radial.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 256 256
		rgbGen lightingGrid
	}
}
tree4_6 // Small fill branches
{
	qer_editorimage textures/models/natural/tree4_3.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree4_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1000 1000
		rgbGen lightingGrid
	}
}
tree4_7 // inner buffer branches
{
	qer_editorimage textures/models/natural/tree4radial.tga
	nomipmaps
   
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree4radial.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 300 300
		rgbGen lightingGrid
	}
}

tree4sprite // Sprite version
{
	qer_editorimage textures/models/natural/tree4sprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree4sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 900
		rgbGen lightingGrid
	}
}

// Tree4  //Oak type tree
static_tree4_1 //trunk
{
	qer_editorimage textures/models/natural/tree4_1.tga
	{
		map textures/models/natural/tree4_1.tga
		rgbGen vertex
		alphaGen distFade 2000 600
	}
}
static_tree4_2 //vertical cross section
{
	qer_editorimage textures/models/natural/tree4_2.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 2.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 2.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree4_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2000 1500
		rgbGen vertex
	}
}
static_tree4_3 //fill plains
{
	qer_editorimage textures/models/natural/tree4_3.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 2.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 2.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree4_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1400 1400
		rgbGen vertex
	}
}
static_tree4_4 //horizontal cross plains
{
	qer_editorimage textures/models/natural/tree4radial.tga
	nomipmaps
	cull none
	deformVertexes wave 24 sin 0 0.5 0    0.2
	deformVertexes wave 24 sin 0 0.5 0.25 0.3
	{
		clampmap textures/models/natural/tree4radial.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 256 256
		rgbGen vertex
	}
}
static_tree4_6 // Small fill branches
{
	qer_editorimage textures/models/natural/tree4_3.tga
	nomipmaps
	cull none
	deformVertexes flap t 24 sin 0 2.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 2.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree4_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1000 1000
		rgbGen vertex
	}
}
static_tree4_7 // inner buffer branches
{
	qer_editorimage textures/models/natural/tree4radial.tga
	nomipmaps
   
	deformVertexes flap t 24 sin 0 1.5 0    0.2 1 0
	deformVertexes flap t 24 sin 0 1.5 0.25 0.3 1 0
	{
		clampmap textures/models/natural/tree4radial.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 300 300
		rgbGen vertex
	}
}
static_tree4sprite // Sprite version
{
	qer_editorimage textures/models/natural/tree4sprite.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree4sprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 900
		rgbGen vertex
	}
}



// Tree5-tall pine
tree5_1 //trunk
{
	qer_editorimage textures/models/natural/tree5_1.tga
	{
		map textures/models/natural/tree5_1.tga
		rgbGen lightingSpherical
	}
}
tree5_2 //verticle cross sections
{
	qer_editorimage textures/models/natural/tree5_2.tga
	cull none
	{
		clampmap textures/models/natural/tree5_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
tree5_3 //fill plains
{
	qer_editorimage textures/models/natural/tree5_3.tga
	cull none
	{
		clampmap textures/models/natural/tree5_3.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}


// Tree5-tall pine
static_tree5_1 //trunk
{
	qer_editorimage textures/models/natural/tree5_1.tga
	{
		map textures/models/natural/tree5_1.tga
		rgbGen vertex
	}
}
static_tree5_2 //verticle cross sections
{
	qer_editorimage textures/models/natural/tree5_2.tga
	cull none
	{
		clampmap textures/models/natural/tree5_2.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_tree5_3 //fill plains
{
	qer_editorimage textures/models/natural/tree5_3.tga
	cull none
	{
		clampmap textures/models/natural/tree5_3.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Tree5s-tall pine
tree5s_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen lightingSpherical
//	alphaGen distFade 900 0
	}
}

static_tree5s_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen vertex
	alphaGen distFade 900 0
	}
}

tree5s_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen lightingGrid
	}
}

static_tree5s_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen vertex
	}
}

tree5s_3 //vertical cross sections
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
		rgbGen lightingGrid
	}
}

static_tree5s_3 //vertical cross sections
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
		rgbGen vertex
	}
}

tree5ssprite
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
		alphaGen oneMinusDistFade 900 500
		rgbGen lightingGrid
	}
}

// Tree5s-tall pine
static_tree5s_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen vertex
//	alphaGen distFade 900 0
	}
}

static_tree5s_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1200 500
		rgbGen vertex
	}
}


static_tree5s_3 //vertical cross sections
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
		rgbGen vertex
	}
}

static_tree5ssprite
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
		alphaGen oneMinusDistFade 900 500
		rgbGen vertex
	}
}


// Winter Mid Pine
tree5sa_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 900 0
	}
}

tree5sa_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1500 800
		rgbGen lightingGrid
	}
}


tree5sa_3 //vertical cross sections
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
		alphaGen distFade 1800 900
		rgbGen lightingGrid
	}
}
tree5sspritea
{
	qer_editorimage textures/models/natural/tree5sspritea.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree5sspritea.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbGen lightingGrid
	}
}

// Winter Mid Pine
static_tree5sa_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		alphaGen distFade 900 0
		rgbGen vertex
	}
}
static_tree5sa_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1500 800
		rgbGen vertex
	}
}
static_tree5sa_3 //vertical cross sections
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
		alphaGen distFade 1800 900
		rgbGen vertex
	}
}
static_tree5sspritea
{
	qer_editorimage textures/models/natural/tree5sspritea.tga
	qer_trans 0
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tree5sspritea.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 900 500
		rgbGen lightinggrid
	}
}



// Winter Small Pine
tree5sb_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen lightingSpherical
		alphaGen distFade 900 0
	}
}

tree5sb_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1500 800
		rgbGen lightingGrid
	}
}


tree5sb_3 //vertical cross sections
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
		alphaGen distFade 1800 900
		rgbGen lightingGrid
	}
}
tree5sspriteb
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
		alphaGen oneMinusDistFade 900 500
		rgbGen lightingGrid
	}
}

// Winter Small Pine
static_tree5sb_1 //trunk
{
	qer_editorimage textures/models/natural/tree5s_1.tga
	{
		map textures/models/natural/tree5s_1.tga
		rgbGen vertex
		alphaGen distFade 900 0
	}
}
static_tree5sb_2 //fill sections
{
	qer_editorimage textures/models/natural/tree5s_2.tga
		nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tree5s_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1500 800
		rgbGen vertex
	}
}
static_tree5sb_3 //vertical cross sections
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
		alphaGen distFade 1800 900
		rgbGen vertex
	}
}
static_tree5sspriteb
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
		alphaGen oneMinusDistFade 900 500
		rgbGen vertex
	}
}


// Bush3- regualr bush
bush3_1
{
	qer_editorimage textures/models/natural/bush3_1.tga
	cull none
	{
		map textures/models/natural/bush3_1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
bush3_2
{
	qer_editorimage textures/models/natural/bush3_2.tga
	cull none
	{
		clampmap textures/models/natural/bush3_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
bush3_3
{
	qer_editorimage textures/models/natural/bush3_3.tga
	cull none
	{
		clampmap textures/models/natural/bush3_3.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush3- regualr bush
static_bush3_1
{
	qer_editorimage textures/models/natural/bush3_1.tga
	cull none
	deformVertexes flap t 24 sin 0 1 0    0.2 1 0
	deformVertexes flap t 24 sin 0 1 0.25 0.3 1 0
	{
		map textures/models/natural/bush3_1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_bush3_2
{
	qer_editorimage textures/models/natural/bush3_2.tga
	cull none
	deformVertexes wave 24 sin 0 0.75 0    0.2
	deformVertexes wave 24 sin 0 0.75 0.25 0.3
	{
		clampmap textures/models/natural/bush3_2.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_bush3_3
{
	qer_editorimage textures/models/natural/bush3_3.tga
	cull none
	deformVertexes flap t 24 sin 0 1 0    0.2 1 0
	deformVertexes flap t 24 sin 0 1 0.25 0.3 1 0
	{
		clampmap textures/models/natural/bush3_3.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Bush2- sparse shrub
bush2_1
{
	qer_editorimage textures/models/natural/bush2_1.tga
	cull none
	{
		map textures/models/natural/bush2_1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush2- sparse shrub
static_bush2_1
{
	qer_editorimage textures/models/natural/bush2_1.tga
	cull none
	deformVertexes flap t 24 sin 0 0.5 0    0.2 1 0 // 0.5 was 1.5
	deformVertexes flap t 24 sin 0 0.5 0.25 0.3 1 0 // 0.5 was 1.5
	{
		map textures/models/natural/bush2_1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Bush2a- sparse shrub
bush2a_1
{
	qer_editorimage textures/models/natural/bush2a_1.tga
	cull none
	{
		clampmap textures/models/natural/bush2a_1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush2a- sparse shrub
static_bush2a_1
{
	qer_editorimage textures/models/natural/bush2a_1.tga
	cull none
	{
		clampmap textures/models/natural/bush2a_1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Bush1- low lying leaves
bush1_1
{
	qer_editorimage textures/models/natural/bush1_1.tga
	cull none
	{
		clampmap textures/models/natural/bush1_1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush1- low lying leaves
static_bush1_1
{
	qer_editorimage textures/models/natural/bush1_1.tga
	cull none
	{
		clampmap textures/models/natural/bush1_1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}

// Bush1a- ivy cover
bush1a_1
{
	qer_editorimage textures/models/natural/bush1a_1.tga
	cull none
	{
		clampmap textures/models/natural/bush1a_1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush1a- ivy cover
static_bush1a_1
{
	qer_editorimage textures/models/natural/bush1a_1.tga
	cull none
	{
		clampmap textures/models/natural/bush1a_1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Bush4- brush tree
bush4_1
{
	qer_editorimage textures/models/natural/bush4_1.tga
	{
		map textures/models/natural/bush4_1.tga
		rgbGen lightingSpherical
	}
}
bush4_2
{
	qer_editorimage textures/models/natural/bush4_2.tga
	cull none
	{
		clampmap textures/models/natural/bush4_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
bush4_3
{
	qer_editorimage textures/models/natural/bush4_2.tga
	cull none
	{
		clampmap textures/models/natural/bush4_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
bush4_4
{
	qer_editorimage textures/models/natural/bush4_2.tga
	cull none
	{
		clampmap textures/models/natural/bush4_2.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Bush4- brush tree (bush_buckthorn.tik)
static_bush4_1 // trunk
{
	qer_editorimage textures/models/natural/bush4_1.tga
	{
		map textures/models/natural/bush4_1.tga
		rgbGen vertex
	}
}
static_bush4_2
{
	qer_editorimage textures/models/natural/bush4_2.tga
	cull none
	deformVertexes wave 24 sin 0 0.5 0    0.2 // 0.5 was 1.5
	deformVertexes wave 24 sin 0 0.5 0.25 0.3 // 0.5 was 1.5
	{
		clampmap textures/models/natural/bush4_2.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Stump
stump
{
	qer_editorimage textures/models/natural/Stump.tga
	{
		map textures/models/natural/Stump.tga
		rgbGen lightingSpherical
	}
}

// Stump
static_stump
{
	qer_editorimage textures/models/natural/Stump.tga
	{
		map textures/models/natural/Stump.tga
		rgbGen vertex
	}
}


// Winter shrub-dogwood
w_shrub
{
	qer_editorimage textures/models/natural/w_shrub.tga
	cull none
	{
		clampmap textures/models/natural/w_shrub.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Winter shrub-dogwood
static_w_shrub
{
	qer_editorimage textures/models/natural/w_shrub.tga
	cull none
	{
		clampmap textures/models/natural/w_shrub.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Winter fill pine
wfill_pine2
{
	qer_editorimage textures/models/natural/wpinefill_cross.tga
	cull none
	{
		clampmap textures/models/natural/wpinefill_cross.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
wfill_pine3
{
	qer_editorimage textures/models/natural/wpinefill_radial.tga
	cull none
	{
		clampmap textures/models/natural/wpinefill_radial.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Winter fill pine
static_wfill_pine2
{
	qer_editorimage textures/models/natural/wpinefill_cross.tga
	cull none
	{
		clampmap textures/models/natural/wpinefill_cross.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_wfill_pine3
{
	qer_editorimage textures/models/natural/wpinefill_radial.tga
	cull none
	{
		clampmap textures/models/natural/wpinefill_radial.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Winter shrub full
wshrub
{
	qer_editorimage textures/models/natural/wshrub1.tga
	cull none
	{
		clampmap textures/models/natural/wshrub1.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}
wshrubfill
{
	qer_editorimage textures/models/natural/wshrubfill.tga
	cull none
	{
		clampmap textures/models/natural/wshrubfill.tga
		depthWrite
		alphafunc GE128
		rgbGen lightingSpherical
	}
}

// Winter shrub full
static_wshrub
{
	qer_editorimage textures/models/natural/wshrub1.tga
	cull none
	{
		clampmap textures/models/natural/wshrub1.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}
static_wshrubfill
{
	qer_editorimage textures/models/natural/wshrubfill.tga
	cull none
	{
		clampmap textures/models/natural/wshrubfill.tga
		depthWrite
		alphafunc GE128
		rgbGen vertex
	}
}


// Can be placed in levels as a normal texture
textures/models/natural/tree4_2_brush
{
	qer_editorimage textures/models/natural/tree4_2.tga
	qer_keyword tree
	qer_keyword folliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
 	//nopicmip
	cull none
	{
		map textures/models/natural/tree4_2.tga
		depthWrite
		alphaFunc GE128
		nextbundle
		map $lightmap
	}
}

// Can be placed in levels as a normal texture
textures/models/natural/tree1_3_brush
{
	qer_editorimage textures/models/natural/tree1_3.tga
	qer_keyword tree
	qer_keyword folliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
 	//nopicmip
	cull none
	{
		map textures/models/natural/tree1_3.tga
		depthWrite
		alphafunc GE128
		nextbundle
		map $lightmap
	}
}

// Can be placed in levels as a normal texture
textures/models/natural/tree4_3_brush
{
	qer_editorimage textures/models/natural/tree4_3.tga
	qer_keyword tree
	qer_keyword folliage
	qer_keyword masked
	surfaceparm foliage
	surfaceparm fence
	surfaceparm alphashadow
 	//nopicmip
	cull none
	{
		map textures/models/natural/tree4_3.tga
		depthWrite
		alphafunc GE128
		nextbundle
		map $lightmap
	}
}


// Regular Palm Tree
palmregtrunk	// Trunk
{
	qer_editorimage textures/models/natural/palmregtrunk.tga
	{
		map textures/models/natural/palmregtrunk.tga
		rgbGen lightingSpherical
		alphaGen distFade 2304 0
	}
}
static_palmregtrunk	// Trunk
{
	qer_editorimage textures/models/natural/palmregtrunk.tga
	{
		map textures/models/natural/palmregtrunk.tga
//		rgbGen vertex
		rgbGen lightingSpherical
		alphaGen distFade 2304 0
	}
}

palmcross // Cross section
{
	qer_editorimage textures/models/natural/palmcross.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/palmcross.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen lightingGrid
	}
}
static_palmcross // Cross section
{
	qer_editorimage textures/models/natural/palmcross.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/palmcross.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen lightingSpherical
//		rgbGen vertex
	}
}
palmfill // Fill leaves
{
	qer_editorimage textures/models/natural/palmfill.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/palmfill.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbGen lightingGrid
	}
}
static_palmfill // Fill leaves
{
	qer_editorimage textures/models/natural/palmfill.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/palmfill.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbGen lightingSpherical
//		rgbGen vertex
	}
}

regpalmsprite // Sprite version
{
	qer_editorimage textures/models/natural/regpalmsprite.tga
	qer_trans .25
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/regpalmsprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen lightingGrid
	}
}
// Shorter Palm Tree
static_shortpalmtrunk	// Trunk
{
	qer_editorimage textures/models/natural/shortpalmtrunk.tga
	{
		map textures/models/natural/shortpalmtrunk.tga
		rgbGen vertex
		alphaGen distFade 2304 0
	}
}
static_shortpalmcross // Cross section
{
	qer_editorimage textures/models/natural/shortpalmcross.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/shortpalmcross.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen vertex
	}
}

static_shortpalmfill // Fill leaves
{
	qer_editorimage textures/models/natural/shortpalmfill.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/shortpalmfill.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbGen vertex
	}
}
static_shortpalmsprite // Sprite version
{
	qer_editorimage textures/models/natural/shortpalmsprite.tga
	qer_trans .25
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/shortpalmsprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen vertex
	}
}
// Small Yucca
static_smallyuccatrunk	// Trunk
{
	qer_editorimage textures/models/natural/smallyuccatrunk.tga
	{
		map textures/models/natural/smallyuccatrunk.tga
		rgbGen vertex
		alphaGen distFade 2304 0
	}
}
static_smallyuccacross // Cross section
{
	qer_editorimage textures/models/natural/smallyuccacross.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/smallyuccacross.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen vertex
	}
}

static_smallyuccafill // Fill leaves
{
	qer_editorimage textures/models/natural/smallyuccafill.tga
	nomipmaps
	cull none
//	good low wind values
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/smallyuccafill.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 256
		rgbGen vertex
	}
}
static_smallyuccasprite // Sprite version
{
	qer_editorimage textures/models/natural/smallyuccasprite.tga
	qer_trans .25
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/smallyuccasprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen vertex
	}
}
// Tall Yucca
static_tallyuccatrunk	// Trunk
{
	qer_editorimage textures/models/natural/tallyuccatrunk.tga
	{
		map textures/models/natural/tallyuccatrunk.tga
		rgbGen vertex
		alphaGen distFade 2304 0
	}
}
static_tallyuccacross // Cross section
{
	qer_editorimage textures/models/natural/tallyuccacross.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/tallyuccacross.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen vertex
	}
}

static_tallyuccasprite // Sprite version
{
	qer_editorimage textures/models/natural/tallyuccasprite.tga
	qer_trans .25
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/tallyuccasprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen vertex
	}
}
// Squat Palm
static_squatpalmtrunk	// Trunk
{
	qer_editorimage textures/models/natural/shortpalmtrunk.tga
	{
		map textures/models/natural/shortpalmtrunk.tga
		rgbGen vertex
		alphaGen distFade 2304 0
	}
}
static_squatpalmcross // Cross section
{
	qer_editorimage textures/models/natural/shortpalmfill.tga
	nomipmaps
	cull none
//	deformVertexes flap t 24 sin 4 4 0 1 1 0
	{
		clampmap textures/models/natural/shortpalmfill.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1536 512
		rgbGen vertex
	}
}

static_squatpalmsprite // Sprite version
{
	qer_editorimage textures/models/natural/squatpalmsprite.tga
	qer_trans .25
	nomipmaps
	deformVertexes autoSprite2
	cull none
	{
		clampmap textures/models/natural/squatpalmsprite.tga
		depthWrite
		alphaFunc GE128
		alphaGen oneMinusDistFade 1216 512
		rgbGen vertex
	}
} 

// Bush_full_heavyweather  //Rainy day bush
static_bushhw_1 //trunk
{
	qer_editorimage textures/models/natural/tree4_1.tga
	{
		map textures/models/natural/tree4_1.tga
		rgbGen vertex
		alphaGen distFade 2000 600
	}
}
static_bushhw_2 //vertical cross section
{
	qer_editorimage textures/models/natural/tree4_2.tga
	nomipmaps
	cull none
	//deformVertexes flap t 24 sin 0 2.5 0    0.2 1 0
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree4_2.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 2000 1500
		rgbGen vertex
	}
}
static_bushhw_3 //fill plains
{
	qer_editorimage textures/models/natural/tree4_3.tga
	nomipmaps
	cull none
	//deformVertexes flap t 24 sin 0 2.5 0    0.2 1 0
	deformVertexes flap t 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree4_3.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 1400 1400
		rgbGen vertex
	}
}
static_tree4_4a //horizontal cross plains
{
	qer_editorimage textures/models/natural/tree4radial.tga
	nomipmaps
	cull none
	//deformVertexes wave 24 sin 0 0.5 0    0.2
	deformVertexes wave 24 sin 2 3 0 .25 1 0
	{
		clampmap textures/models/natural/tree4radial.tga
		depthWrite
		alphaFunc GE128
		alphaGen distFade 256 256
		rgbGen vertex
	}
}

