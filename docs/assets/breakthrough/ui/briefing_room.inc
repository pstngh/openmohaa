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


resource
Label
{
name "briefing sign"
rect 449 4 169 47
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
tileshader 0
linkcvar "ui_briefingsign"
linkcvartoshader
}

resource
Button
{
name "Tunisia"
rect 283 290 128 32
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/tunisia"
hovershader "textures/mohmenu/tunisia_h"
stuffcommand "pushmenu mission1difficulty"
hovercommand "set ui_briefingsign textures/mohmenu/brief_sign_1"
mouseawaycommand "set ui_briefingsign 0"
}

resource
Button
{
name "Sicily"
rect 368 264 128 32
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
stuffcommand "pushmenu mission2difficulty"
borderstyle "NONE"
shader "textures/mohmenu/sicily"
hovershader "textures/mohmenu/sicily_h"
hovercommand "set ui_briefingsign textures/mohmenu/brief_sign_2"
mouseawaycommand "set ui_briefingsign 0"
enabledcvar "g_t2l1"
}

resource
Button
{
name "Italy"
rect 370 185 128 32
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
stuffcommand "pushmenu mission3difficulty"
shader "textures/mohmenu/italy"
hovershader "textures/mohmenu/italy_h"
hovercommand "set ui_briefingsign textures/mohmenu/brief_sign_3"
mouseawaycommand "set ui_briefingsign 0"
enabledcvar "g_t3l1"
}

