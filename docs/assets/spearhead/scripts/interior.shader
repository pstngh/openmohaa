textures/interior/sp_wdtrim_townhall
{
qer_keyword indoor
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword m5
 qer_editorimage textures/interior/sp_wdtrim_townhall.tga
 {
  map textures/interior/sp_wdtrim_townhall.tga
//map $bumpmap textures/bumpmaps/sp_wdtrim_townhallbmp.tga
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



textures/interior/bookcase1
{
qer_keyword wood
surfaceparm wood
qer_keyword special
qer_keyword indoor
qer_keyword panel
  qer_editorimage textures/interior/bookcase1.tga
  {
  map textures/mohcommon/normandy4.tga
  tcGen environment
  }
  {
  map textures/interior/bookcase1.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
  }
  {
  map $lightmap
  rgbGen Identity
  blendFunc GL_DST_COLOR GL_ZERO
  }
} 

textures/interior/carptset1a
{
qer_keyword carpet
surfaceparm carpet
qer_keyword floor
 qer_editorimage textures/interior/carptset1a.tga
 {
  map textures/interior/carptset1a.tga
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

textures/interior/carptset2
{
surfaceparm carpet
qer_keyword floor
qer_keyword carpet
 qer_editorimage textures/interior/carptset2.tga
 {
  map textures/interior/carptset2.tga
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

textures/interior/ceiling_mural1
{
qer_keyword indoor
qer_keyword signs
qer_keyword stone
qer_keyword concrete
surfaceparm rock
qer_keyword ceiling
 qer_editorimage textures/interior/ceiling_mural1.tga
 {
  map textures/interior/ceiling_mural1.tga
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

textures/interior/ceilingtil_set1
{
qer_keyword panel
qer_keyword plaster
surfaceparm rock
qer_keyword ceiling
 qer_editorimage textures/interior/ceilingtil_set1.tga
 {
  map textures/interior/ceilingtil_set1.tga
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

textures/interior/cellarroof
{
qer_keyword wood
surfaceparm wood
qer_keyword ceiling
 qer_editorimage textures/interior/cellarroof.tga
 {
  map textures/interior/cellarroof.tga
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

textures/interior/hearth_set1basef
{
surfaceparm rock
qer_keyword plaster
qer_keyword stone
qer_keyword concrete
qer_keyword special
qer_keyword flat
 qer_editorimage textures/interior/hearth_set1baseF.tga
 {
  map textures/interior/hearth_set1baseF.tga
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

textures/interior/hearth_set1det
{
surfaceparm rock
qer_keyword stone
qer_keyword special
qer_keyword plaster
qer_keyword concrete
 qer_editorimage textures/interior/hearth_set1det.tga
 {
  map textures/interior/hearth_set1det.tga
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

textures/interior/hearth_set1trim
{
qer_keyword trim
surfaceparm rock
qer_keyword stone
qer_keyword special
qer_keyword plaster
qer_keyword concrete
 qer_editorimage textures/interior/hearth_set1trim.tga
 {
  map textures/interior/hearth_set1trim.tga
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

textures/interior/hearthsides
{
surfaceparm rock
qer_keyword trim
qer_keyword stone
qer_keyword special
qer_keyword plaster
qer_keyword concrete
 qer_editorimage textures/interior/hearthsides.tga
 {
  map textures/interior/hearthsides.tga
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

textures/interior/cross
{
surfaceparm metal
qer_keyword metal
qer_keyword masked
  qer_editorimage textures/interior/ironcross.tga
  surfaceparm fence	
  surfaceparm alphashadow
  surfaceparm playerclip
  surfaceparm monsterclip
  surfaceparm nonsolid
  cull none
  nopicmip
  {
  map textures/interior/ironcross.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
  }
}

textures/interior/jh_stonetile1
{
  qer_keyword stone
  surfaceparm rock
  qer_keyword flat
  qer_keyword floor
  portal
  qer_editorimage textures/interior/jh_stonetile1.tga
  {
  map $whiteimage
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
  rgbGen constant 0 0 0
  alphaGen constant .75
  depthWrite
  }
  {
  map textures/interior/jh_stonetile1.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
  depthFunc equal
  nextbundle
  map $lightmap
  }
}

textures/interior/railing
{
qer_keyword wood
qer_keyword masked
  qer_editorimage textures/interior/manor_rail.tga
  surfaceparm wood
  surfaceparm fence
  surfaceparm alphashadow
  cull none
  nopicmip
  {
  map textures/interior/manor_rail.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
  }
} 

textures/interior/marble_foyer_decorative
{
qer_keyword stone
surfaceparm rock
qer_keyword floor
portal
//surfaceparm nolightmap
  qer_editorimage textures/interior/marblefoyer.tga
  {
  map $whiteimage
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
  rgbGen constant 0 0 0
  alphaGen constant .75
  depthWrite
  }
  {
  map textures/interior/marblefoyer.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
  depthFunc equal
  nextbundle
  map $lightmap
  }
//  {
//  map textures/mohcommon/environ_puddle.tga
//  tcGen environment
//  }
//  {
//  map $lightmap
//  rgbGen Identity
//  blendFunc GL_DST_COLOR GL_ZERO
//  blendFunc GL_ONE GL_ONE
//  depthMask
//  }
} 

textures/interior/panel_set1
{
qer_keyword panel
surfaceparm wood
qer_keyword wood
qer_keyword wall
 qer_editorimage textures/interior/panel_set1.tga
 {
  map textures/interior/panel_set1.tga
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

textures/interior/panel_set1a
{
surfaceparm wood
qer_keyword panel
qer_keyword wood
qer_keyword wall
 qer_editorimage textures/interior/panel_set1a.tga
 {
  map textures/interior/panel_set1a.tga
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

textures/interior/ovalpicture1
{
surfaceparm carpet
qer_keyword masked
qer_keyword indoor
qer_keyword signs
  qer_editorimage textures/interior/pj_picture1.tga
  surfaceparm fence	
  surfaceparm alphashadow
  cull none
  nopicmip
  {
  map textures/interior/pj_picture1.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
  }
} 

textures/interior/psuedopic1
{
qer_keyword signs
surfaceparm carpet
qer_keyword indoor
 qer_editorimage textures/interior/psuedopic1.tga
 {
  map textures/interior/psuedopic1.tga
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

textures/interior/tiledfloor_manor
{
qer_keyword flat
surfaceparm rock
qer_keyword stone
qer_keyword floor
 qer_editorimage textures/interior/tiledfloor_manor.tga
 {
  map textures/interior/tiledfloor_manor.tga
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

textures/interior/tiledfloor_manordark
{
qer_keyword flat
surfaceparm rock
qer_keyword stone
qer_keyword floor
 {
  map textures/interior/tiledfloor_manordark.tga
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
 
textures/interior/tiledfloor_manordark2
{
qer_keyword flat
surfaceparm rock
qer_keyword stone
qer_keyword floor
 {
  map textures/interior/tiledfloor_manordark2.tga
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

textures/interior/whitewoodbm
{
qer_keyword wall
surfaceparm wood
qer_keyword wood
qer_keyword flat
 qer_editorimage textures/interior/whitewoodbm.tga
 {
  map textures/interior/whitewoodbm.tga
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

textures/interior/whitewoodbmtrm
{
qer_keyword trim
surfaceparm wood
qer_keyword wood
qer_keyword wall
qer_keyword flat
 qer_editorimage textures/interior/whitewoodbmtrm.tga
 {
  map textures/interior/whitewoodbmtrm.tga
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

textures/interior/woodpanelfill
{
surfaceparm wood
qer_keyword wood
qer_keyword wall
qer_keyword flat
 qer_editorimage textures/interior/woodpanelfill.tga
 {
  map textures/interior/woodpanelfill.tga
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

textures/interior/sp_carpet_townhall
{
qer_keyword m5
surfaceparm carpet
qer_keyword carpet
qer_keyword floor
qer_keyword flat
 qer_editorimage textures/interior/sp_carpet_townhall.tga
 {
  map textures/interior/sp_carpet_townhall.tga
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

textures/interior/sp_carpet_townhalltrim
{
qer_keyword m5
qer_keyword trim
surfaceparm carpet
qer_keyword floor
qer_keyword carpet
 qer_editorimage textures/interior/sp_carpet_townhalltrim.tga
 {
  map textures/interior/sp_carpet_townhalltrim.tga
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

textures/interior/sp_carpet_townhallworn
{
qer_keyword m5
qer_keyword flat
surfaceparm carpet
qer_keyword floor
qer_keyword carpet
 qer_editorimage textures/interior/sp_carpet_townhallworn.tga
 {
  map textures/interior/sp_carpet_townhallworn.tga
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

textures/interior/sp_ceiling_townhall1
{
qer_keyword m5
qer_keyword indoor
surfaceparm rock
qer_keyword stone
qer_keyword ceiling
 qer_editorimage textures/interior/sp_ceiling_townhall1.tga
 {
  map textures/interior/sp_ceiling_townhall1.tga
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

//textures/interior/sp_curtain_townhall
textures/interior/redcurtains_townhall
{
  qer_keyword m5
  qer_keyword indoor
  qer_keyword trim
  qer_keyword masked
  qer_keyword carpet
  qer_editorimage textures/interior/sp_curtain_townhall.tga
  surfaceparm carpet
  surfaceparm alphashadow
  surfaceparm fence
  surfaceparm nonsolid
  cull none
  {
    map textures/interior/sp_curtain_townhall.tga
    depthWrite
    alphaFunc GE128
    nextbundle
    map $lightmap
 }
} 

textures/interior/sp_twnhall_arcwinmsk
{
qer_keyword stone
surfaceparm rock
qer_keyword window
qer_keyword masked
qer_keyword m5
 {
  map textures/interior/sp_twnhall_arcwinmsk.tga 	
  alphaFunc GE128
  depthWrite
nextbundle
  map $lightmap
 }
}




textures/interior/victorioussml
{
surfaceparm wood
surfaceparm alphashadow
qer_keyword wood
qer_keyword indoor
 qer_editorimage textures/interior/victorioussml.tga
 {
  map textures/interior/victorioussml.tga
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


textures/interior/panel_set2
{
qer_keyword panel
surfaceparm wood
qer_keyword wood
 qer_editorimage textures/interior/panel_set2.tga
 {
  map textures/interior/panel_set2.tga
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


textures/interior/picture5
{
qer_keyword m3
qer_keyword signs
surfaceparm wood
qer_keyword wood
 qer_editorimage textures/interior/picture5.tga
 {
  map textures/interior/picture5.tga
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


textures/interior/pictureset_3a
{
PolygonOffset
nopicmip
qer_keyword masked
qer_keyword signs
qer_keyword indoor
surfaceparm carpet
qer_keyword metal
qer_keyword carpet
surfaceparm fence
surfaceparm alphashadow
 qer_editorimage textures/interior/pictureset_3a.tga
 {
  map textures/interior/pictureset_3a.tga
  blendfunc blend 
depthWrite
nextbundle
  map $lightmap
 }
}


textures/interior/pictureset_3b
{
surfaceparm fence
surfaceparm alphashadow
surfaceparm carpet
qer_keyword signs
qer_keyword metal
qer_keyword masked
qer_keyword indoor
qer_keyword carpet
 qer_editorimage textures/interior/pictureset_3b.tga
 {
  map textures/interior/pictureset_3b.tga
  blendfunc blend 
depthWrite
nextbundle
  map $lightmap
 }
}


textures/interior/intrwl1trn
{
qer_keyword plaster
qer_keyword indoor
surfaceparm wood
qer_keyword tudor
qer_keyword wall
 qer_editorimage textures/interior/intrwl1trn.tga
 {
  map textures/interior/intrwl1trn.tga
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

textures/interior/intrwl1ppr
{
surfaceparm wood
qer_keyword wall
qer_keyword tudor
qer_keyword plaster
qer_keyword indoor
 qer_editorimage textures/interior/intrwl1ppr.tga
 {
  map textures/interior/intrwl1ppr.tga
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
textures/interior/ornamental_wallsconce_side
{
surfaceparm metal
qer_keyword metal
qer_keyword masked
  qer_editorimage textures/interior/ornmntl_wall_sconce_side.tga
  surfaceparm fence	
  //surfaceparm alphashadow
  surfaceparm playerclip
  surfaceparm monsterclip
  surfaceparm nonsolid
  cull none
  nopicmip
  nomipmaps
  {
  map textures/interior/ornmntl_wall_sconce_side.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
  }
}
textures/interior/manor_int_door
{
qer_keyword wood
surfaceparm wood
qer_keyword door
 qer_editorimage textures/interior/manor_int_door.tga
 {
  map textures/special/goldenv.tga
  tcgen environment
 }
 {
  map textures/interior/manor_int_door.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/manor_woodfloor
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
qer_keyword floor
 qer_editorimage textures/interior/manor_woodfloor.tga
 {
  map textures/interior/manor_woodfloor.tga
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

textures/interior/gildwall1
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/gildwall1.tga
 {
  map textures/special/goldenv.tga
  tcgen environment
 }
 {
  map textures/interior/gildwall1.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/manor_column
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/manor_column.tga
 {
  map textures/interior/manor_column.tga
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

textures/interior/cathedral_rightfront
{
surfaceparm rock
qer_keyword panel
qer_keyword wall
 qer_editorimage textures/interior/cathwall_right1.tga
 {
  map textures/special/goldenv2.tga
  tcgen environment
 }
 {
  map textures/interior/cathwall_right1.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/cathedral_leftfront
{
surfaceparm rock
qer_keyword panel
qer_keyword wall
 qer_editorimage textures/interior/cathwall_left1.tga
 {
  map textures/special/goldenv2.tga
  tcgen environment
 }
 {
  map textures/interior/cathwall_left1.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/cathedral_lowertrim
{
surfaceparm wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/cathedral_lowtrim.tga
 {
  map textures/special/goldenv2.tga
  tcgen environment
 }
 {
  map textures/interior/cathedral_lowtrim.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 
 
textures/interior/cathedraltrim
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/cathtrim.tga
 {
  map textures/interior/cathtrim.tga
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

textures/interior/remagenchurchrail_lower
{
cull none
nopicmip
surfaceparm grill
qer_keyword masked
qer_keyword metal
surfaceparm nonsolid
surfaceparm monsterclip
surfaceparm playerclip
surfaceparm fence
surfaceparm alphashadow
 qer_editorimage textures/interior/remchurch_lowrail.tga
 {
  map textures/interior/remchurch_lowrail.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
 }
}

textures/interior/cathwall_interior_left
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/cathwallinterior_left.tga
 {
  map textures/interior/cathwallinterior_left.tga
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

textures/interior/cathwall_interior_right
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/cathwallinterior_right.tga
 {
  map textures/interior/cathwallinterior_right.tga
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

textures/interior/cathedralwindow_centerlower
{
surfaceparm glass
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/cathwincenterlow.tga
 {
  map textures/interior/cathwincenterlow.tga
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

textures/interior/cathedral_centertop
{
surfaceparm rock
qer_keyword panel
qer_keyword wall
 qer_editorimage textures/interior/cathcentertop.tga
 {
  map textures/special/goldenv2.tga
  tcgen environment
 }
 {
  map textures/interior/cathcentertop.tga
  blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/cathedraltrim_top
{
surfaceparm metal
qer_keyword metal
qer_keyword masked
  qer_editorimage textures/interior/cathedral_trimtop.tga
  surfaceparm fence	
  surfaceparm alphashadow
  cull none
  nopicmip
  {
  map textures/interior/cathedral_trimtop.tga
  depthWrite
  alphaFunc GE128
  nextbundle
  map $lightmap
  }
} 

textures/interior/woodbeamed_trenchwall1
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/trenchwall1.tga
 {
  map textures/interior/trenchwall1.tga
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

textures/interior/woodbeamed_trenchwall2
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/trenchwall2.tga
 {
  map textures/interior/trenchwall2.tga
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

textures/interior/trenchwall2
{
surfaceparm wood
qer_keyword wood
qer_keyword trim
qer_keyword wall
 qer_editorimage textures/interior/trenchwall2.tga
 {
  map textures/interior/trenchwall2.tga
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

textures/interior/ht_interiorwall_wthrdtrim
{
qer_keyword trim
surfaceparm rock
qer_keyword stone
qer_keyword special
qer_keyword plaster
qer_keyword concrete
 qer_editorimage textures/interior/hotelwthr_trim.tga
 {
  map textures/interior/hotelwthr_trim.tga
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

textures/interior/tarpet_2sided
{
qer_keyword m5
surfaceparm carpet
qer_keyword carpet
qer_keyword floor
qer_keyword flat
cull none
 qer_editorimage textures/interior/sp_carpet_townhall.tga
 {
  map textures/interior/sp_carpet_townhall.tga
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

textures/interior/grand_1
{
qer_keyword signs
surfaceparm wood
qer_keyword wood
 qer_editorimage textures/interior/grand_1.tga
 {
  map textures/interior/grand_1.tga
  blendFunc blend
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
   
textures/interior/grand_2
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_2.tga
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

textures/interior/grand_3
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_3.tga
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

textures/interior/grand_4
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_4.tga
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

textures/interior/grand_5
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_5.tga
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

textures/interior/pianokeys
{
surfaceparm wood
qer_keyword special
 {
  map textures/interior/pianokeys.tga
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

textures/interior/grand_7tapestry
{
qer_keyword indoor
qer_keyword special
qer_keyword masked
surfaceparm paper
 {
  map textures/interior/grand_7tapestry.tga
  depthWrite
  blendFunc blend
  nextbundle
  map $lightmap
 } 
}

textures/interior/grand_8
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_8.tga
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

textures/interior/grand_10
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_10.tga
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

textures/interior/grand_11
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_11.tga
  depthWrite
  alphaFunc GE128
  rgbGen identity
 }
 {
  map $lightmap
  rgbGen identity
  blendFunc GL_DST_COLOR GL_ZERO
  depthFunc equal
 }
} 

textures/interior/grand_12
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_12.tga
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

textures/interior/grand_pj
{
qer_keyword indoor
qer_keyword wood
surfaceparm wood
 {
  map textures/interior/grand_pj.tga
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

 

 


 
 

 

