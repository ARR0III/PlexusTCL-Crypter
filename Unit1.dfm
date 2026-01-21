object Form1: TForm1
  Left = 219
  Top = 351
  BorderStyle = bsNone
  Caption = 'Form1'
  ClientHeight = 306
  ClientWidth = 491
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
  object Shape1: TShape
    Left = 1
    Top = 1
    Width = 489
    Height = 25
    Cursor = crSizeAll
    Brush.Color = clActiveCaption
    OnMouseDown = Shape1MouseDown
  end
  object Shape2: TShape
    Left = 1
    Top = 25
    Width = 489
    Height = 280
    Brush.Color = clBtnFace
  end
  object Label1: TLabel
    Left = 9
    Top = 7
    Width = 70
    Height = 13
    Cursor = crSizeAll
    Caption = 'Crycon for Win'
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnMouseDown = Label1MouseDown
  end
  object Label2: TLabel
    Left = 12
    Top = 30
    Width = 116
    Height = 13
    Caption = #1040#1083#1075#1086#1088#1080#1090#1084' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
  end
  object Label3: TLabel
    Left = 12
    Top = 78
    Width = 134
    Height = 13
    Caption = #1044#1083#1080#1085#1072' '#1082#1083#1102#1095#1072' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
    Color = clBtnFace
    ParentColor = False
  end
  object Label4: TLabel
    Left = 168
    Top = 30
    Width = 106
    Height = 13
    Caption = #1060#1072#1081#1083' '#1076#1083#1103' '#1086#1073#1088#1072#1073#1086#1090#1082#1080
  end
  object Label5: TLabel
    Left = 168
    Top = 78
    Width = 91
    Height = 13
    Caption = #1060#1072#1081#1083' '#1085#1072#1079#1085#1072#1095#1077#1085#1080#1103
  end
  object Label6: TLabel
    Left = 12
    Top = 158
    Width = 186
    Height = 13
    Caption = #1055#1072#1088#1086#1083#1100' '#1080#1083#1080' '#1087#1091#1090#1100' '#1082' '#1082#1083#1102#1095#1077#1074#1086#1084#1091' '#1092#1072#1081#1083#1091
  end
  object Label7: TLabel
    Left = 12
    Top = 282
    Width = 63
    Height = 13
    Caption = #1057#1086#1089#1090#1086#1103#1085#1080#1077'...'
  end
  object Label8: TLabel
    Left = 441
    Top = 7
    Width = 18
    Height = 13
    Cursor = crHandPoint
    Caption = '[ ? ]'
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label8Click
    OnMouseEnter = Label8MouseEnter
    OnMouseLeave = Label8MouseLeave
  end
  object Label9: TLabel
    Left = 463
    Top = 7
    Width = 19
    Height = 13
    Cursor = crHandPoint
    Caption = '[ X ]'
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnClick = Label9Click
    OnMouseEnter = Label9MouseEnter
    OnMouseLeave = Label9MouseLeave
  end
  object CipherBox: TComboBox
    Left = 12
    Top = 48
    Width = 145
    Height = 21
    ItemHeight = 13
    MaxLength = 12
    TabOrder = 0
    OnChange = CipherBoxChange
  end
  object KeyLenBox: TComboBox
    Left = 12
    Top = 96
    Width = 145
    Height = 21
    ItemHeight = 13
    MaxLength = 12
    TabOrder = 1
  end
  object InputFile: TEdit
    Left = 168
    Top = 48
    Width = 217
    Height = 21
    Cursor = crIBeam
    MaxLength = 2048
    TabOrder = 2
  end
  object OutputFile: TEdit
    Left = 168
    Top = 96
    Width = 217
    Height = 21
    Cursor = crIBeam
    MaxLength = 2048
    TabOrder = 3
  end
  object ButtonInput: TButton
    Left = 400
    Top = 48
    Width = 75
    Height = 20
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 4
    OnClick = ButtonInputClick
  end
  object ButtonOutput: TButton
    Left = 400
    Top = 96
    Width = 75
    Height = 20
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 5
    OnClick = ButtonOutputClick
  end
  object ButtonKeyGen: TButton
    Left = 12
    Top = 128
    Width = 75
    Height = 20
    Cursor = crHandPoint
    Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088
    TabOrder = 6
    OnClick = ButtonKeyGenClick
  end
  object PassLen: TEdit
    Left = 99
    Top = 128
    Width = 57
    Height = 21
    Cursor = crIBeam
    MaxLength = 3
    TabOrder = 7
  end
  object RadioEncrypt: TRadioButton
    Left = 176
    Top = 129
    Width = 89
    Height = 17
    Cursor = crHandPoint
    Caption = #1047#1072#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 8
  end
  object RadioDecrypt: TRadioButton
    Left = 288
    Top = 129
    Width = 97
    Height = 17
    Cursor = crHandPoint
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 9
  end
  object MemoKey: TMemo
    Left = 12
    Top = 176
    Width = 377
    Height = 65
    Cursor = crIBeam
    MaxLength = 2048
    ScrollBars = ssVertical
    TabOrder = 10
  end
  object ButtonKey: TButton
    Left = 400
    Top = 176
    Width = 75
    Height = 20
    Cursor = crHandPoint
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 11
    OnClick = ButtonKeyClick
  end
  object ButtonStart: TButton
    Left = 400
    Top = 220
    Width = 75
    Height = 20
    Cursor = crHandPoint
    Caption = #1057#1090#1072#1088#1090
    TabOrder = 12
    OnClick = ButtonStartClick
  end
  object ProgressBar1: TProgressBar
    Left = 12
    Top = 256
    Width = 465
    Height = 17
    Min = 0
    Max = 100
    ParentShowHint = False
    Smooth = True
    ShowHint = False
    TabOrder = 13
  end
  object OpenDialog1: TOpenDialog
    Left = 296
    Top = 32
  end
  object SaveDialog1: TSaveDialog
    Left = 328
    Top = 32
  end
end
