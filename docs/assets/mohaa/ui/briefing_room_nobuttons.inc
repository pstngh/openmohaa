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
rect 384 0 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_c"
enabledcvar "ui_voodoo"
}

resource
Label
{
name "Default"
rect 0 0 256 256
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "textures/mohmenu/voodoo/briefingroom_a"
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
shader "textures/mohmenu/voodoo/briefingroom_b"
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
shader "textures/mohmenu/voodoo/briefingroom_f"
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
shader "textures/mohmenu/voodoo/briefingroom_d"
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
shader "textures/mohmenu/voodoo/briefingroom_e"
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
resource
Label
{
name "siegfried line"
rect 274 224 106 16
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "siegfried"
//hovershader "siegfried_h"
//stuffcommand "pushmenu mission6difficulty"
//hovercommand "set ui_briefingsign mission6_sign"
enabledcvar "g_m6l1"

}
resource
Label
{
name "omaha beach"
rect 134 220 102 24
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "omaha"
//hovershader "omaha_h"
//stuffcommand "pushmenu mission3difficulty"
//hovercommand "set ui_briefingsign mission3_sign"
enabledcvar "g_m3l1"

}
resource
Label
{
name "trondheim"
rect 256 80 84 20
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "trondheim"
//hovershader "trondheim_h"
//stuffcommand "pushmenu mission2difficulty"
//hovercommand "set ui_briefingsign mission2_sign"
enabledcvar "g_m2l1"

}
resource
Label
{
name "normandy"
rect 216 241 80 24
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "normandy"
//hovershader "normandy_h"
//stuffcommand "pushmenu mission4difficulty"
//hovercommand "set ui_briefingsign mission4_sign"
enabledcvar "g_m4l1"

}
resource
Label
{
name "brittany"
rect 136 248 72 22
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "brittany"
//hovershader "brittany_h"
//stuffcommand "pushmenu mission5difficulty"
//hovercommand "set ui_briefingsign mission5_sign"
enabledcvar "g_m5l1"

}
resource
Label
{
name "arzew"
rect 176 370 48 24
fgcolor 1.00 1.00 1.00 1.00
bgcolor 0.50 0.50 0.50 0.00
borderstyle "NONE"
shader "arzew"
//hovershader "arzew_h"
//stuffcommand "pushmenu mission1difficulty"
//hovercommand "set ui_briefingsign mission1_sign"

}