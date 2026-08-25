object Form2: TForm2
  Left = 228
  Top = 154
  BorderStyle = bsNone
  Caption = #1057#1055#1056#1040#1042#1050#1040
  ClientHeight = 160
  ClientWidth = 383
  Color = clBtnFace
  CustomTitleBar.CaptionAlignment = taCenter
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Shape1: TShape
    Left = 1
    Top = 1
    Width = 381
    Height = 25
    Cursor = crSizeAll
    Align = alCustom
    Brush.Color = clNavy
    Pen.Color = clNavy
    OnMouseDown = Shape1MouseDown
  end
  object Label5: TLabel
    Left = 354
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
  object Label4: TLabel
    Left = 8
    Top = 5
    Width = 3
    Height = 15
    Cursor = crSizeAll
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnMouseDown = Label4MouseDown
  end
  object Shape2: TShape
    Left = 1
    Top = 25
    Width = 381
    Height = 134
    Align = alCustom
    Brush.Color = clBtnFace
  end
  object Label3: TLabel
    Left = 8
    Top = 136
    Width = 3
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Bevel1: TBevel
    Left = 8
    Top = 52
    Width = 368
    Height = 79
    Shape = bsFrame
  end
  object Label2: TLabel
    Left = 14
    Top = 58
    Width = 3
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 60
    Top = 32
    Width = 258
    Height = 13
    Caption = 'PLEXUS TECHNOLOGY CYBERNETIC LABORATORY'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
  end
end
