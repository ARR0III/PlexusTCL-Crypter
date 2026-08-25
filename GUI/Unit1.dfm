object Form1: TForm1
  Left = 290
  Top = 403
  BorderStyle = bsNone
  ClientHeight = 300
  ClientWidth = 527
  Color = clBtnFace
  CustomTitleBar.CaptionAlignment = taCenter
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 16
  object Shape2: TShape
    Left = 1
    Top = 1
    Width = 525
    Height = 25
    Cursor = crSizeAll
    Align = alCustom
    Brush.Color = clHotLight
    Pen.Color = clPurple
    OnMouseDown = Shape2MouseDown
  end
  object Label7: TLabel
    Left = 473
    Top = 6
    Width = 19
    Height = 15
    Cursor = crHandPoint
    Caption = '[ ? ]'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label7Click
    OnMouseEnter = Label7MouseEnter
    OnMouseLeave = Label7MouseLeave
  end
  object Label5: TLabel
    Left = 498
    Top = 6
    Width = 21
    Height = 15
    Cursor = crHandPoint
    Caption = '[ X ]'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label5Click
    OnMouseEnter = Label5MouseEnter
    OnMouseLeave = Label5MouseLeave
  end
  object Label6: TLabel
    Left = 8
    Top = 6
    Width = 48
    Height = 15
    Cursor = crSizeAll
    Caption = 'CRYCON'
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnMouseDown = Label6MouseDown
  end
  object Shape1: TShape
    Left = 1
    Top = 25
    Width = 525
    Height = 274
    Align = alCustom
    Brush.Color = clBtnFace
    Pen.Color = clPurple
  end
  object Label1: TLabel
    Left = 8
    Top = 33
    Width = 126
    Height = 13
    Caption = #1040#1083#1075#1086#1088#1080#1090#1084' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 80
    Width = 145
    Height = 13
    Caption = #1044#1083#1080#1085#1072' '#1082#1083#1102#1095#1072' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object Label3: TLabel
    Left = 178
    Top = 80
    Width = 93
    Height = 13
    Caption = #1060#1072#1081#1083' '#1085#1072#1079#1085#1072#1095#1077#1085#1080#1103
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 178
    Top = 33
    Width = 109
    Height = 13
    Caption = #1060#1072#1081#1083' '#1076#1083#1103' '#1086#1073#1088#1072#1073#1086#1090#1082#1080
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 8
    Top = 160
    Width = 197
    Height = 13
    Caption = #1055#1072#1088#1086#1083#1100' '#1080#1083#1080' '#1087#1091#1090#1100' '#1082' '#1082#1083#1102#1095#1077#1074#1086#1084#1091' '#1092#1072#1081#1083#1091
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 8
    Top = 274
    Width = 66
    Height = 13
    Caption = #1057#1086#1089#1090#1086#1103#1085#1080#1077'...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object RadioDecrypt: TRadioButton
    Left = 328
    Top = 133
    Width = 99
    Height = 17
    Cursor = crHandPoint
    Hint = '138'
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object RadioEncrypt: TRadioButton
    Left = 225
    Top = 133
    Width = 97
    Height = 17
    Cursor = crHandPoint
    Hint = '138'
    Caption = #1047#1072#1096#1080#1092#1088#1086#1074#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object ProgressBar1: TProgressBar
    Left = 8
    Top = 250
    Width = 511
    Height = 16
    ParentShowHint = False
    Smooth = True
    BarColor = clGreen
    BackgroundColor = clHighlightText
    ShowHint = False
    TabOrder = 2
  end
  object InputFile: TEdit
    Left = 178
    Top = 52
    Width = 249
    Height = 19
    Cursor = crIBeam
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 3
  end
  object OutputFile: TEdit
    Left = 178
    Top = 99
    Width = 249
    Height = 19
    Cursor = crIBeam
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 4
  end
  object CipherBox: TComboBox
    Left = 8
    Top = 52
    Width = 150
    Height = 21
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 5
    Text = 'CipherBox'
    OnChange = CipherBoxChange
  end
  object ButtonInput: TButton
    Left = 444
    Top = 49
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'ButtonInput'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = ButtonInputClick
  end
  object ButtonOutput: TButton
    Left = 444
    Top = 96
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'ButtonOutput'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = ButtonOutputClick
  end
  object ButtonKey: TButton
    Left = 444
    Top = 177
    Width = 75
    Height = 23
    Cursor = crHandPoint
    Caption = 'ButtonKey'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = ButtonKeyClick
  end
  object ButtonStart: TButton
    Left = 444
    Top = 214
    Width = 75
    Height = 23
    Cursor = crHandPoint
    Caption = 'ButtonStart'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = ButtonStartClick
  end
  object MemoKey: TMemo
    Left = 8
    Top = 179
    Width = 419
    Height = 58
    Cursor = crIBeam
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 10
  end
  object PassLen: TEdit
    Left = 104
    Top = 132
    Width = 54
    Height = 19
    Cursor = crIBeam
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    MaxLength = 3
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 11
    Text = '8'
  end
  object ButtonKeyGen: TButton
    Left = 8
    Top = 129
    Width = 75
    Height = 25
    Cursor = crHandPoint
    Caption = 'ButtonKeyGen'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    OnClick = ButtonKeyGenClick
  end
  object KeyLenBox1: TComboBox
    Left = 8
    Top = 98
    Width = 150
    Height = 21
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 13
    Text = 'KeyLenBox'
  end
  object OpenDialog1: TOpenDialog
    Left = 344
    Top = 40
  end
  object SaveDialog1: TSaveDialog
    Left = 376
    Top = 40
  end
end
