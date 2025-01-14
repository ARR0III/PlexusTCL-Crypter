object Form1: TForm1
  Left = 226
  Top = 285
  BorderStyle = bsNone
  ClientHeight = 276
  ClientWidth = 506
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Shape2: TShape
    Left = 1
    Top = 1
    Width = 504
    Height = 22
    Cursor = crSizeAll
    Align = alCustom
    Brush.Color = clNavy
    Pen.Color = clPurple
    OnMouseDown = Shape2MouseDown
  end
  object Label7: TLabel
    Left = 456
    Top = 5
    Width = 18
    Height = 13
    Cursor = crHandPoint
    Caption = '[ ? ]'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label7Click
    OnMouseEnter = Label7MouseEnter
    OnMouseLeave = Label7MouseLeave
  end
  object Label5: TLabel
    Left = 480
    Top = 5
    Width = 19
    Height = 13
    Cursor = crHandPoint
    Caption = '[ X ]'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label5Click
    OnMouseEnter = Label5MouseEnter
    OnMouseLeave = Label5MouseLeave
  end
  object Label6: TLabel
    Left = 8
    Top = 5
    Width = 45
    Height = 13
    Caption = 'CRYCON'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Shape1: TShape
    Left = 1
    Top = 23
    Width = 504
    Height = 252
    Align = alCustom
    Brush.Color = clBtnFace
    Pen.Color = clPurple
  end
  object Label1: TLabel
    Left = 8
    Top = 30
    Width = 116
    Height = 13
    Caption = #1040#1083#1075#1086#1088#1080#1090#1084' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
  end
  object Label4: TLabel
    Left = 8
    Top = 78
    Width = 134
    Height = 13
    Caption = #1044#1083#1080#1085#1072' '#1082#1083#1102#1095#1072' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
    Visible = False
  end
  object Label3: TLabel
    Left = 168
    Top = 78
    Width = 91
    Height = 13
    Caption = #1060#1072#1081#1083' '#1085#1072#1079#1085#1072#1095#1077#1085#1080#1103
  end
  object Label2: TLabel
    Left = 168
    Top = 30
    Width = 106
    Height = 13
    Caption = #1060#1072#1081#1083' '#1076#1083#1103' '#1086#1073#1088#1072#1073#1086#1090#1082#1080
  end
  object Label8: TLabel
    Left = 8
    Top = 154
    Width = 186
    Height = 13
    Caption = #1055#1072#1088#1086#1083#1100' '#1080#1083#1080' '#1087#1091#1090#1100' '#1082' '#1082#1083#1102#1095#1077#1074#1086#1084#1091' '#1092#1072#1081#1083#1091
  end
  object Label9: TLabel
    Left = 8
    Top = 256
    Width = 63
    Height = 13
    Caption = #1057#1086#1089#1090#1086#1103#1085#1080#1077'...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 48
    Width = 137
    Height = 21
    Ctl3D = False
    ItemHeight = 13
    MaxLength = 20
    ParentCtl3D = False
    TabOrder = 3
    TabStop = False
    OnChange = ComboBox1Change
  end
  object ComboBox2: TComboBox
    Left = 8
    Top = 96
    Width = 137
    Height = 21
    Ctl3D = False
    ItemHeight = 13
    MaxLength = 20
    ParentCtl3D = False
    TabOrder = 0
    TabStop = False
    Visible = False
  end
  object Edit2: TEdit
    Left = 168
    Top = 96
    Width = 242
    Height = 21
    Cursor = crIBeam
    Ctl3D = True
    MaxLength = 2048
    ParentCtl3D = False
    TabOrder = 2
  end
  object Edit1: TEdit
    Left = 168
    Top = 48
    Width = 242
    Height = 21
    Cursor = crIBeam
    Ctl3D = True
    MaxLength = 2048
    ParentCtl3D = False
    TabOrder = 1
  end
  object Button1: TButton
    Left = 424
    Top = 48
    Width = 75
    Height = 21
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 424
    Top = 96
    Width = 75
    Height = 21
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 5
    OnClick = Button2Click
  end
  object CheckBox1: TCheckBox
    Left = 424
    Top = 136
    Width = 73
    Height = 17
    Caption = #1057#1090#1077#1088#1077#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
  end
  object RadioButton2: TRadioButton
    Left = 296
    Top = 130
    Width = 113
    Height = 17
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 7
  end
  object RadioButton1: TRadioButton
    Left = 168
    Top = 130
    Width = 97
    Height = 17
    Caption = #1047#1072#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 8
  end
  object Edit3: TEdit
    Left = 96
    Top = 128
    Width = 49
    Height = 21
    Cursor = crIBeam
    BiDiMode = bdRightToLeft
    Ctl3D = True
    MaxLength = 3
    ParentBiDiMode = False
    ParentCtl3D = False
    TabOrder = 9
    Text = '64'
  end
  object Button5: TButton
    Left = 8
    Top = 128
    Width = 81
    Height = 21
    Cursor = crHandPoint
    Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088
    TabOrder = 10
    OnClick = Button5Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 170
    Width = 401
    Height = 57
    Cursor = crIBeam
    Ctl3D = True
    MaxLength = 2048
    ParentCtl3D = False
    ScrollBars = ssVertical
    TabOrder = 11
  end
  object Button3: TButton
    Left = 424
    Top = 170
    Width = 75
    Height = 21
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 12
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 424
    Top = 206
    Width = 75
    Height = 21
    Cursor = crHandPoint
    Caption = #1057#1090#1072#1088#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
    OnClick = Button4Click
  end
  object ProgressBar1: TProgressBar
    Left = 8
    Top = 236
    Width = 491
    Height = 16
    Cursor = crHandPoint
    Min = 0
    Max = 100
    ParentShowHint = False
    Smooth = True
    ShowHint = False
    TabOrder = 14
  end
  object OpenDialog1: TOpenDialog
    Left = 344
    Top = 32
  end
  object SaveDialog1: TSaveDialog
    Left = 376
    Top = 32
  end
end
