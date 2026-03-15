
resource
Label
{
name "Default"
rect 0 0 512 512
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "briefingroom_a"
}

resource
Label
{
name "Default"
rect 384 0 256 512
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "briefingroom_b"
}

resource
Label
{
name "Default"
rect 0 0 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_a.tga"
enabledcvar "ui_voodoo"

}
resource
Label
{
name "Default"
rect 384 0 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_e.tga"
enabledcvar "ui_voodoo"

}
resource
Label
{
name "Default"
rect 256 0 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_b.tga"
enabledcvar "ui_voodoo"

}
resource
Label
{
name "Default"
rect 384 256 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_f.tga"
enabledcvar "ui_voodoo"

}
resource
Label
{
name "Default"
rect 256 256 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_d.tga"
enabledcvar "ui_voodoo"
}

resource
Label
{
name "Default"
rect 0 256 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_c.tga"
enabledcvar "ui_voodoo"
}


//resource
//Label
//{
//name "briefing sign"
//rect 448 8 170 52
//fgcolor 1.00 1.00 1.00 1.00
//bgcolor 0.50 0.50 0.50 0.00
//borderstyle "NONE"
//shader "mission1_sign"
//linkcvar "mission1_sign"
//}
resource
Label
{
name "briefing sign"
rect 448 8 170 52
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
linkcvar "ui_briefingsign"
linkcvartoshader

}
