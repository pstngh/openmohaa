VERSION 5.00
Begin VB.Form ToolBar 
   BorderStyle     =   0  'None
   Caption         =   "ToolBar"
   ClientHeight    =   405
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   12345
   LinkTopic       =   "Form2"
   ScaleHeight     =   405
   ScaleWidth      =   12345
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton Command18 
      Caption         =   "-"
      Height          =   330
      Left            =   1980
      TabIndex        =   23
      Top             =   495
      Width           =   285
   End
   Begin VB.CommandButton Command17 
      Caption         =   "+"
      Height          =   285
      Left            =   1980
      TabIndex        =   22
      Top             =   225
      Width           =   285
   End
   Begin VB.CommandButton Command16 
      Caption         =   "-"
      Height          =   330
      Left            =   1710
      TabIndex        =   20
      Top             =   495
      Width           =   285
   End
   Begin VB.CommandButton Command15 
      Caption         =   "+"
      Height          =   285
      Left            =   1710
      TabIndex        =   19
      Top             =   225
      Width           =   285
   End
   Begin VB.CommandButton Command14 
      Caption         =   "-"
      Height          =   330
      Left            =   1440
      TabIndex        =   18
      Top             =   495
      Width           =   285
   End
   Begin VB.CommandButton Command13 
      Caption         =   "+"
      Height          =   285
      Left            =   1440
      TabIndex        =   17
      Top             =   225
      Width           =   285
   End
   Begin VB.CommandButton Command12 
      Caption         =   "-"
      Height          =   330
      Left            =   1170
      TabIndex        =   16
      Top             =   495
      Width           =   285
   End
   Begin VB.CommandButton Command11 
      Caption         =   "+"
      Height          =   285
      Left            =   1170
      TabIndex        =   15
      Top             =   225
      Width           =   285
   End
   Begin VB.CommandButton Command10 
      Caption         =   "\"
      Height          =   330
      Left            =   3015
      TabIndex        =   11
      Top             =   720
      Width           =   330
   End
   Begin VB.CommandButton Command9 
      Caption         =   "/"
      Height          =   330
      Left            =   3015
      TabIndex        =   10
      Top             =   0
      Width           =   330
   End
   Begin VB.CommandButton Command8 
      Caption         =   ">"
      Height          =   330
      Left            =   3015
      TabIndex        =   9
      Top             =   360
      Width           =   330
   End
   Begin VB.CommandButton Command7 
      Caption         =   "/"
      Height          =   330
      Left            =   2295
      TabIndex        =   8
      Top             =   720
      Width           =   330
   End
   Begin VB.CommandButton Command6 
      Caption         =   "\/"
      Height          =   330
      Left            =   2655
      TabIndex        =   7
      Top             =   720
      Width           =   330
   End
   Begin VB.CommandButton Command5 
      Caption         =   "\"
      Height          =   330
      Left            =   2295
      TabIndex        =   6
      Top             =   0
      Width           =   330
   End
   Begin VB.CommandButton Command4 
      Caption         =   "/\"
      Height          =   330
      Left            =   2655
      TabIndex        =   5
      Top             =   0
      Width           =   330
   End
   Begin VB.CommandButton Command3 
      Caption         =   "<"
      Height          =   330
      Left            =   2295
      TabIndex        =   4
      Top             =   360
      Width           =   330
   End
   Begin VB.CommandButton Command1 
      Caption         =   "o"
      Height          =   330
      Left            =   2655
      TabIndex        =   3
      Top             =   360
      Width           =   330
   End
   Begin VB.CommandButton DownMoveSpeed 
      Caption         =   "<"
      Height          =   330
      Left            =   450
      TabIndex        =   2
      Top             =   45
      Width           =   330
   End
   Begin VB.CommandButton UpMoveSpeed 
      Caption         =   ">"
      Height          =   330
      Left            =   810
      TabIndex        =   1
      Top             =   45
      Width           =   330
   End
   Begin VB.CommandButton Command2 
      Caption         =   "S"
      Height          =   330
      Left            =   45
      TabIndex        =   0
      Top             =   45
      Width           =   375
   End
   Begin VB.Label Label4 
      Caption         =   "I"
      Height          =   195
      Left            =   1980
      TabIndex        =   21
      Top             =   0
      Width           =   240
   End
   Begin VB.Label Label3 
      Caption         =   "B"
      Height          =   195
      Left            =   1755
      TabIndex        =   14
      Top             =   0
      Width           =   195
   End
   Begin VB.Label Label2 
      Caption         =   "G"
      Height          =   195
      Left            =   1440
      TabIndex        =   13
      Top             =   0
      Width           =   285
   End
   Begin VB.Label Label1 
      Caption         =   "R"
      Height          =   195
      Left            =   1170
      TabIndex        =   12
      Top             =   0
      Width           =   240
   End
End
Attribute VB_Name = "ToolBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' R+
Private Sub Command11_Click()

lumel = GetVar("l_lumel")

red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

red = red + 10
If red > 255 Then
 red = 255
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'R-
Private Sub Command12_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

red = red - 10
If red < 0 Then
 red = 0
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'G+
Private Sub Command13_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

green = green + 10
If green > 255 Then
 green = 255
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'G-
Private Sub Command14_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

green = green - 10
If green < 0 Then
 green = 0
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'B+
Private Sub Command15_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

blue = blue + 10
If blue > 255 Then
 blue = 255
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'B-
Private Sub Command16_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

blue = blue - 10
If blue < 0 Then
 blue = 0
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'I+
Private Sub Command17_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

red = red * 1.1
green = green * 1.1
blue = blue * 1.1

' Convert To Bytes
red = red And 65535
green = green And 65535
blue = blue And 65535

cmax = red
If green > cmax Then
 cmax = green
endif
If blue > cmax Then
 cmax = blue
endif

If cmax > 255 Then
 scale = 255 / cmax
 red = red * scale
 green = green * scale
 blue = blue * scale

 ' Convert To Bytes
 red = red And 255
 green = green And 255
 blue = blue And 255
endif

lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'I-
Private Sub Command18_Click()
lumel = GetVar("l_lumel")
red = lumel And 255
lumel = lumel / 256
green = lumel And 255
lumel = lumel / 256
blue = lumel And 255

red = red * 0.9
green = green * 0.9
blue = blue * 0.9

' Convert To Bytes
red = red And 65535
green = green And 65535
blue = blue And 65535

cmin = red
If green < cmin Then
 cmin = green
endif
If blue < cmin Then
 cmin = blue
endif

If cmin < 1 Then
 red = red / cmin
 green = green / cmin
 blue = blue / cmin

 ' Convert To Bytes
 red = red And 255
 green = green And 255
 blue = blue And 255

endif


lumel = blue * 256
lumel = lumel + green
lumel = lumel * 256
lumel = lumel + red
a = SetVar("l_lumel", lumel)
End Sub

'S
Private Sub Command2_Click()
kn$ = "savemap " + KeyString("World", "mapname")
a = ConsoleFunc(kn$)
End Sub

Private Sub DownMoveSpeed_Click()
mspeed = GetVar("m_scale") ' Get Movement Scale
mspeed = mspeed - 500
If mspeed < 10 Then
 mspeed = 10
endif
a = SetVar("m_scale", mspeed)
End Sub

Private Sub Form_Load()
'a = SetKey("SaveButton", "shader", "editor/disk")
End Sub

Private Sub UpMoveSpeed_Click()
mspeed = GetVar("m_scale") ' Get Movement Scale
mspeed = mspeed + 500
If mspeed > 4000 Then
 mspeed = 4000
endif
a = SetVar("m_scale", mspeed)
End Sub
