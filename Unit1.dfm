object Form1: TForm1
  Left = 200
  Top = 268
  BorderStyle = bsNone
  Caption = 'Form1'
  ClientHeight = 276
  ClientWidth = 507
  Color = clScrollBar
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 507
    Height = 22
    Cursor = crSizeAll
    Align = alTop
    Color = clNavy
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    OnMouseDown = Panel1MouseDown
    object Label5: TLabel
      Left = 450
      Top = 4
      Width = 50
      Height = 13
      Cursor = crHandPoint
      Caption = '['#1047#1072#1082#1088#1099#1090#1100']'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = Label5Click
    end
    object Label6: TLabel
      Left = 6
      Top = 4
      Width = 3
      Height = 13
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 392
      Top = 4
      Width = 49
      Height = 13
      Cursor = crHandPoint
      Caption = '['#1057#1087#1088#1072#1074#1082#1072']'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = Label7Click
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 22
    Width = 507
    Height = 254
    Align = alClient
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 116
      Height = 13
      Caption = #1040#1083#1075#1086#1088#1080#1090#1084' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
    end
    object Label2: TLabel
      Left = 168
      Top = 8
      Width = 106
      Height = 13
      Caption = #1060#1072#1081#1083' '#1076#1083#1103' '#1086#1073#1088#1072#1073#1086#1090#1082#1080
    end
    object Label3: TLabel
      Left = 168
      Top = 56
      Width = 91
      Height = 13
      Caption = #1060#1072#1081#1083' '#1085#1072#1079#1085#1072#1095#1077#1085#1080#1103
    end
    object Label4: TLabel
      Left = 8
      Top = 56
      Width = 134
      Height = 13
      Caption = #1044#1083#1080#1085#1072' '#1082#1083#1102#1095#1072' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103
      Visible = False
    end
    object Shape1: TShape
      Left = 8
      Top = 210
      Width = 489
      Height = 18
    end
    object Shape2: TShape
      Left = 10
      Top = 212
      Width = 0
      Height = 15
      Brush.Color = clGreen
      DragMode = dmAutomatic
      Pen.Color = clGreen
      Pen.Style = psClear
    end
    object Label8: TLabel
      Left = 8
      Top = 130
      Width = 241
      Height = 13
      Caption = #1050#1083#1102#1095' '#1096#1080#1092#1088#1086#1074#1072#1085#1080#1103' '#1080#1083#1080' '#1087#1091#1090#1100' '#1082' '#1082#1083#1102#1095#1077#1074#1086#1084#1091' '#1092#1072#1081#1083#1091
    end
    object ComboBox1: TComboBox
      Left = 8
      Top = 24
      Width = 137
      Height = 21
      Ctl3D = False
      ItemHeight = 13
      MaxLength = 20
      ParentCtl3D = False
      TabOrder = 0
      TabStop = False
      OnChange = ComboBox1Change
    end
    object Edit1: TEdit
      Left = 168
      Top = 24
      Width = 242
      Height = 19
      Cursor = crIBeam
      Ctl3D = False
      MaxLength = 2048
      ParentCtl3D = False
      TabOrder = 1
    end
    object Edit2: TEdit
      Left = 168
      Top = 72
      Width = 242
      Height = 19
      Cursor = crIBeam
      Ctl3D = False
      MaxLength = 2048
      ParentCtl3D = False
      TabOrder = 2
    end
    object Button1: TButton
      Left = 424
      Top = 24
      Width = 75
      Height = 20
      Cursor = crHandPoint
      Caption = #1054#1090#1082#1088#1099#1090#1100
      TabOrder = 3
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 424
      Top = 72
      Width = 75
      Height = 20
      Cursor = crHandPoint
      Caption = #1054#1090#1082#1088#1099#1090#1100
      TabOrder = 4
      OnClick = Button2Click
    end
    object RadioButton1: TRadioButton
      Left = 168
      Top = 104
      Width = 97
      Height = 17
      Caption = #1047#1072#1096#1080#1092#1088#1086#1074#1072#1090#1100
      TabOrder = 5
    end
    object RadioButton2: TRadioButton
      Left = 296
      Top = 104
      Width = 97
      Height = 17
      Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
      TabOrder = 6
    end
    object Memo1: TMemo
      Left = 8
      Top = 146
      Width = 401
      Height = 57
      Cursor = crIBeam
      Ctl3D = False
      MaxLength = 2048
      ParentCtl3D = False
      ScrollBars = ssVertical
      TabOrder = 7
    end
    object Button3: TButton
      Left = 424
      Top = 146
      Width = 75
      Height = 20
      Cursor = crHandPoint
      Caption = #1054#1090#1082#1088#1099#1090#1100
      TabOrder = 8
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 424
      Top = 182
      Width = 75
      Height = 20
      Cursor = crHandPoint
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 9
      OnClick = Button4Click
    end
    object ComboBox2: TComboBox
      Left = 8
      Top = 72
      Width = 137
      Height = 21
      Ctl3D = False
      ItemHeight = 13
      MaxLength = 20
      ParentCtl3D = False
      TabOrder = 10
      TabStop = False
      Visible = False
      Items.Strings = (
        '128'
        '192'
        '256')
    end
    object StatusBar1: TStatusBar
      Left = 1
      Top = 234
      Width = 505
      Height = 19
      Panels = <
        item
          Width = 50
        end>
      SimplePanel = False
    end
    object Edit3: TEdit
      Left = 96
      Top = 104
      Width = 49
      Height = 19
      Cursor = crIBeam
      BiDiMode = bdRightToLeft
      MaxLength = 3
      ParentBiDiMode = False
      TabOrder = 12
      Text = '64'
    end
    object Button5: TButton
      Left = 8
      Top = 104
      Width = 81
      Height = 20
      Cursor = crHandPoint
      Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088
      TabOrder = 13
      OnClick = Button5Click
    end
    object CheckBox1: TCheckBox
      Left = 424
      Top = 112
      Width = 73
      Height = 17
      Caption = #1057#1090#1077#1088#1077#1090#1100
      TabOrder = 14
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 336
    Top = 32
  end
  object SaveDialog1: TSaveDialog
    Left = 376
    Top = 32
  end
end
