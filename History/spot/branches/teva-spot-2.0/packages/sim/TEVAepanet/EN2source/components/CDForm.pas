{
   Unit:    CDForm.pas
   Project: TChartDialog Component
   Author:  L. Rossman
   Version: 1.0
   Date:    1/30/00 

   This is the form unit used for the TChartDialog 
   dialog box component (ChartDlg.pas).

   This unit uses the TComboColor component (ComboColor.pas).
}

unit CDForm;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls, ComboColor, Chart, TeEngine, Series,
  Spin, Buttons, ExtCtrls;

const
  Colors: array[0..41] of TColor =
    (clBlack,clMaroon,clGreen,clOlive,clNavy,clPurple,clTeal,clGray,clSilver,
     clRed,clLime,clYellow,clBlue,clFuchsia,clAqua,clWhite,
     clScrollBar,clBackground,clActiveCaption,clInactiveCaption,clMenu,
     clWindow,clWindowFrame,clMenuText,clWindowText,clCaptionText,
     clActiveBorder,clInactiveBorder,clAppWorkSpace,clHighlight,
     clHighlightText,clBtnFace,clBtnShadow,clGrayText,clBtnText,
     clInactiveCaptionText,clBtnHighlight,cl3DDkShadow,cl3DLight,
     clInfoText,clInfoBk,clNone);
  LegendPosText: array[0..3] of PChar =
    ('Left','Right','Top','Bottom');
  MarkStyleText: array[0..7] of PChar =
    ('Rectangle','Circle','Up Triangle','Down Triangle','Cross',
     'Diagonal Cross','Star','Diamond');
  FillStyleText: array[0..7] of PChar =
    ('Solid','Clear','Horizontal','Vertical','Foward Diagonal','Back Diagonal',
     'Cross','Diagonal Cross');
  StackStyleText: array[0..3] of PChar =
    ('None','Side','Stacked','Stacked 100%');
  LabelStyleText: array[0..8] of PChar =
    ('Value','Percent','Label','Label & %','Label & Value','Legend','% Total',
     'Label & % Total','X Value');

type
//Graph series types
  TSeriesType = (stLine, stPoint, stBar, stHorizBar, stArea, stPie);

//Graph series options
  TSeriesOptions = class(TObject)
    Constructor Create;
    public
      SeriesType      : TSeriesType;
      LineVisible     : Boolean;
      LineStyle       : Integer;
      LineColor       : TColor;
      LineWidth       : Integer;
      PointVisible    : Boolean;
      PointStyle      : Integer;
      PointColor      : TColor;
      PointSize       : Integer;
      AreaFillStyle   : Integer;
      AreaFillColor   : TColor;
      AreaStacking    : Integer;
      PieCircled      : Boolean;
      PieUsePatterns  : Boolean;
      PieRotation     : Integer;
      LabelsVisible   : Boolean;
      LabelsTransparent: Boolean;
      LabelsArrows    : Boolean;
      LabelsBackColor : TColor;
      LabelsStyle     : Integer;
    end;

  TChartOptionsForm = class(TForm)
    PageControl1: TPageControl;
    FontDialog1: TFontDialog;
    GeneralPage: TTabSheet;         {General Page}
      Label1: TLabel;
      Label3: TLabel;
      Label27: TLabel;
      Label31: TLabel;
      PanelColorBox: TComboColor;
      BackColorBox: TComboColor;
      Percent3DBox: TSpinEdit;      {Chart.3DPercent}
      View3DBox: TCheckBox;         {Chart.View3D}
      GraphTitleBox: TEdit;         {Chart.Title.Text}
      GraphTitleFontBtn: TButton;
    XaxisPage: TTabSheet;         {X Axis Page}
      Label4: TLabel;
      Label5: TLabel;
      Label6: TLabel;
      Label8: TLabel;
      Label9: TLabel;
      XdataMinLabel: TLabel;
      XdataMaxLabel: TLabel;
      Xmin: TEdit;                  {Chart.BottomAxis.Minimum}
      Xmax: TEdit;                  {Chart.BottomAxis.Maximum}
      Xinc: TEdit;                  {Chart.BottomAxis.Increment}
      Xauto: TCheckBox;             {Chart.BottomAxis.Automatic}
      XgridNone: TRadioButton;      {Chart.BottomAxis.Grid.Visible}
      XgridSolid: TRadioButton;     {Chart.BottomAxis.Grid.Style}
      XgridDotted: TRadioButton;    {  "     "         "     "  }
      Xtitle: TEdit;                {Chart.BottomAxis.Title.Caption}
      XfontBtn: TButton;
    YaxisPage: TTabSheet;         {Y Axis Page}
      Label10: TLabel;
      Label11: TLabel;
      Label12: TLabel;
      Label14: TLabel;
      Label15: TLabel;
      YdataMinLabel: TLabel;
      YdataMaxLabel: TLabel;
      Ymin: TEdit;
      Ymax: TEdit;
      Yinc: TEdit;
      Yauto: TCheckBox;
      YgridNone: TRadioButton;
      YgridSolid: TRadioButton;
      YgridDotted: TRadioButton;
      Ytitle: TEdit;
      YfontBtn: TButton;
    LegendPage: TTabSheet;
      LegendPosBox: TComboBox;
      LegendColorBox: TComboColor;
      LegendWidthBox: TSpinEdit;
      LegendFrameBox: TCheckBox;
      LegendVisibleBox: TCheckBox;
      Label2: TLabel;
      Label23: TLabel;
      Label24: TLabel;
    SeriesPage: TTabSheet;         {Series Page}
      Label7: TLabel;
      SeriesListBox: TComboBox;
      Label26: TLabel;
      SeriesTitle: TEdit;           {Series.Title}
      LegendFontBtn: TButton;
      PageControl2: TPageControl;
        LineOptionsSheet: TTabSheet;   {Line Options}
          Label16: TLabel;
          LineStyleBox: TComboBox;
          Label17: TLabel;
          LineColorBox: TComboColor;
          Label18: TLabel;
          LineSizeBox: TSpinEdit;
          LineVisibleBox: TCheckBox;
        MarkOptionsSheet: TTabSheet;   {Mark Options}
          Label20: TLabel;
          MarkStyleBox: TComboBox;
          Label21: TLabel;
          MarkColorBox: TComboColor;
          Label22: TLabel;
          MarkSizeBox: TSpinEdit;
          MarkVisibleBox: TCheckBox;
        AreaOptionsSheet: TTabSheet;   {Area Fill Options}
          AreaFillStyleBox: TComboBox;
          Label13: TLabel;
          AreaColorBox: TComboColor;
          StackStyleBox: TComboBox;
        PieOptionsSheet: TTabSheet;
          PieRotationBox: TSpinEdit;
          Label30: TLabel;
        LabelsOptionsSheet: TTabSheet;
          LabelsStyleBox: TComboBox;
          LabelsBackColorBox: TComboColor;
          LabelsTransparentBox: TCheckBox;
          LabelsArrowsBox: TCheckBox;
          LabelsVisibleBox: TCheckBox;
   DefaultBox: TCheckBox;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    BitBtn3: TBitBtn;
    Label25: TLabel;
    Label19: TLabel;
    Label28: TLabel;
    Label29: TLabel;
    PieCircledBox: TCheckBox;
    PiePatternBox: TCheckBox;

    procedure FormCreate(Sender: TObject);
    procedure GraphTitleFontBtnClick(Sender: TObject);
    procedure XfontBtnClick(Sender: TObject);
    procedure YfontBtnClick(Sender: TObject);
    procedure LineStyleBoxDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure SeriesListBoxClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure LegendFontBtnClick(Sender: TObject);
    procedure LineStyleBoxChange(Sender: TObject);
  private
    { Private declarations }
    theIndex: Integer;
    theSeries: TStringlist;
    IsPieChart: Boolean;
    function  GetColorIndex(aColor: TColor): Integer;
    procedure GetSeriesOptions(const Index: Integer);
    procedure SetSeriesOptions(const Index: Integer);
    procedure SetAxisScaling(Axis: TChartAxis; const Smin,Smax,Sinc: String);
  public
    { Public declarations }
    procedure LoadOptions(aChart: TChart);
    procedure UnloadOptions(aChart: TChart);
  end;

var
  ChartOptionsForm: TChartOptionsForm;

implementation

{$R *.DFM}

{Constructor for TSeriesOptions}
Constructor TSeriesOptions.Create;
begin
  Inherited Create;
end;

procedure TChartOptionsForm.FormCreate(Sender: TObject);
{------------------------------------------------------
  OnCreate handler for form
-------------------------------------------------------}
var
  i: Integer;
  s: String;
  c: TColor;
begin

{ Load colors into ColorCombo boxes }
  Font.Size := 8;
  for i := 0 to High(Colors) do
  begin
    s := ColorToString(Colors[i]);
    Delete(s,1,2);
    c := Colors[i];
    PanelColorBox.AddColor(s,c);
    BackColorBox.AddColor(s,c);
    LegendColorBox.AddColor(s,c);
    LineColorBox.AddColor(s,c);
    MarkColorBox.AddColor(s,c);
    AreaColorBox.AddColor(s,c);
    LabelsBackColorBox.AddColor(s,c);
  end;

{ Load options into comboboxes }
  for i := 0 to High(LegendPosText) do
    LegendPosBox.Items.Add(LegendPosText[i]);
  for i := 0 to High(FillStyleText) do
    AreaFillStyleBox.Items.Add(FillStyleText[i]);
  for i := 0 to High(MarkStyleText) do
    MarkStyleBox.Items.Add(MarkStyleText[i]);
  for i := 0 to High(StackStyleText) do
    StackStyleBox.Items.Add(StackStyleText[i]);
  for i := 0 to High(LabelStyleText) do
    LabelsStyleBox.Items.Add(LabelStyleText[i]);

{ Create a stringlist to hold data series options }
  theSeries := TStringlist.Create;
  PageControl1.ActivePage := GeneralPage;
end;

procedure TChartOptionsForm.FormDestroy(Sender: TObject);
{-------------------------------------------------------
  OnDestroy handler for form.
  Frees the data series stringlist & its objects.
--------------------------------------------------------}
var
  i: Integer;
begin
  with theSeries do
  begin
    for i := 0 to Count - 1 do
      Objects[i].Free;
    Free;
  end;
end;

procedure TChartOptionsForm.LoadOptions(aChart: TChart);
{------------------------------------------------------
  Transfers data from aChart to form.
-------------------------------------------------------}
var
  i: Integer;
  s: String;
  SeriesOptions: TSeriesOptions;
begin
  IsPieChart := False;
  with aChart do
  begin

  { General Page }
    View3DBox.Checked := View3D;
    Percent3DBox.Value := Chart3DPercent;
    PanelColorBox.ItemIndex := GetColorIndex(Color);
    BackColorBox.ItemIndex := GetColorIndex(BackColor);
    GraphTitleBox.Font.Assign(Title.Font);
    if (Title.Text.Count > 0) then
      GraphTitleBox.Text := Title.Text[0];

  { Series Page - do before Axis pages to get value for IsPieChart }
  { Save current line series options }
    SeriesTitle.Font.Assign(Legend.Font);
    for i := 0 to SeriesCount-1 do
    begin
      if Series[i].Active then
      begin
        SeriesOptions := TSeriesOptions.Create;
        s := 'Series' + IntToStr(i+1);
        SeriesListBox.Items.Add(s);

        with Series[i], SeriesOptions do
        begin
          LabelsVisible := Marks.Visible;
          LabelsArrows := Marks.Arrow.Visible;
          LabelsTransparent := Marks.Transparent;
          LabelsBackColor := Marks.BackColor;
          LabelsStyle := Ord(Marks.Style);
        end;
        if Series[i] is TLineSeries then
          with Series[i] as TLineSeries, SeriesOptions do
          begin
            SeriesType := stLine;
            LineVisible := LinePen.Visible;
            LineStyle := Ord(LinePen.Style);
            LineColor := SeriesColor;
            LineWidth := LinePen.Width;
            AreaFillStyle := Ord(LineBrush);
            PointVisible := Pointer.Visible;
            PointStyle := Ord(Pointer.Style);
            PointColor := Pointer.Brush.Color;
            PointSize := Pointer.HorizSize;
          end
        else if Series[i] is TPointSeries then
          with Series[i] as TPointSeries, SeriesOptions do
          begin
            SeriesType := stPoint;
            PointVisible := Pointer.Visible;
            PointStyle := Ord(Pointer.Style);
            PointColor := SeriesColor;
            PointSize := Pointer.HorizSize;
          end
        else if Series[i] is TBarSeries then
          with Series[i] as TBarSeries, SeriesOptions do
          begin
            SeriesType := stBar;
            AreaFillStyle := Ord(BarBrush.Style);
            if BarBrush.Style = bsSolid then
            begin
              AreaFillColor := SeriesColor;
              LineColor := BarBrush.Color;
            end
            else
            begin
              LineColor := SeriesColor;
              AreaFillColor := BarBrush.Color;
            end;
            AreaStacking := Ord(MultiBar);
          end
        else if Series[i] is THorizBarSeries then
          with Series[i] as THorizBarSeries, SeriesOptions do
          begin
            SeriesType := stHorizBar;
            AreaFillStyle := Ord(BarBrush.Style);
            if BarBrush.Style = bsSolid then
            begin
              AreaFillColor := SeriesColor;
              LineColor := BarBrush.Color;
            end
            else
            begin
              LineColor := SeriesColor;
              AreaFillColor := BarBrush.Color;
            end;
            AreaStacking := Ord(MultiBar);
          end
        else if Series[i] is TAreaSeries then
          with Series[i] as TAreaSeries, SeriesOptions do
          begin
            SeriesType := stArea;
            LineVisible := AreaLinesPen.Visible;
            LineStyle := Ord(AreaLinesPen.Style);
            LineColor := AreaLinesPen.Color;
            LineWidth := AreaLinesPen.Width;
            AreaFillColor := SeriesColor;
            AreaFillStyle := Ord(AreaBrush);
          end
        else if Series[i] is TPieSeries then
          with Series[i] as TPieSeries, SeriesOptions do
          begin
            SeriesType := stPie;
            IsPieChart := True;
            LineVisible := PiePen.Visible;
            LineStyle := Ord(PiePen.Style);
            LineColor := PiePen.Color;  //SeriesColor;
            LineWidth := PiePen.Width;
            PieCircled := Circled;
            PieUsePatterns := UsePatterns;
            PieRotation := RotationAngle;
          end;
        if Length(Series[i].Title) > 0 then s := Series[i].Title;
        theSeries.AddObject(s,SeriesOptions);
      end;
    end;

  { X Axis Page }
    if IsPieChart then XaxisPage.TabVisible := False
    else
    begin
      XdataMinLabel.Caption := Format('(%f)',[MinXValue(BottomAxis)]);
      XdataMaxLabel.Caption := Format('(%f)',[MaxXValue(BottomAxis)]);
      with BottomAxis do
      begin
        Xauto.Checked := Automatic;
        if not Automatic then
        begin
          Xmin.Text := Format('%f',[Minimum]);
          Xmax.Text := Format('%f',[Maximum]);
          Xinc.Text := Format('%f',[Increment]);
        end;
        if not Grid.Visible then
          XgridNone.Checked := True
        else if Grid.Style = psSolid then
          XgridSolid.Checked := True
        else
          XgridDotted.Checked := True;
        Xtitle.Font.Assign(Title.Font);
        Xtitle.Text := Title.Caption;
      end;
    end;

  { Y Axis Page }
    if IsPieChart then YaxisPage.TabVisible := False
    else
    begin
      YdataMinLabel.Caption := Format('(%f)',[MinYValue(LeftAxis)]);
      YdataMaxLabel.Caption := Format('(%f)',[MaxYValue(LeftAxis)]);
      with LeftAxis do
      begin
        Yauto.Checked := Automatic;
        if not Automatic then
        begin
          Ymin.Text := Format('%f',[Minimum]);
          Ymax.Text := Format('%f',[Maximum]);
          Yinc.Text := Format('%f',[Increment]);
        end;
        if not Grid.Visible then
          YgridNone.Checked := True
        else if Grid.Style = psSolid then
          YgridSolid.Checked := True
        else
          YgridDotted.Checked := True;
        Ytitle.Font.Assign(Title.Font);
        Ytitle.Text := Title.Caption;
      end;
    end;

  { Legend Page }
    LegendPosBox.ItemIndex := Ord(Legend.Alignment);
    LegendColorBox.ItemIndex := GetColorIndex(Legend.Color);
    LegendWidthbox.Value := Legend.ColorWidth;
    LegendFrameBox.Checked := Legend.Frame.Visible;
    LegendVisibleBox.Checked := Legend.Visible;
  end;

//Set current series to first series & update dialog entries
  if aChart.SeriesCount > 0 then
  begin
    SetSeriesOptions(0);
    SeriesListBox.ItemIndex := 0;
    theIndex := 0;
    SeriesListBoxClick(SeriesListBox);
  end
  else SeriesPage.TabVisible := False;
end;

procedure TChartOptionsForm.UnloadOptions(aChart: TChart);
{--------------------------------------------------------
   Transfers data from form back to aChart.
---------------------------------------------------------}
var
  i,j: Integer;
  s  : String;
  SeriesOptions: TSeriesOptions;
begin
  with aChart do
  begin

  { General Page }
    View3D := View3DBox.Checked;
    Chart3DPercent := Percent3DBox.Value;
    BackColor := Colors[BackColorBox.ItemIndex];
    Color := Colors[PanelColorBox.ItemIndex];
    Title.Font.Assign(GraphTitleBox.Font);
    s := GraphTitleBox.Text;
    Title.Text.Clear;
    if (Length(s) > 0) then Title.Text.Add(s);

  { X Axis Page }
    if not IsPieChart then with BottomAxis do
    begin
      Automatic := Xauto.Checked;
      if not Automatic then
        SetAxisScaling(BottomAxis,Xmin.Text,Xmax.Text,Xinc.Text);
      Grid.Visible := not XgridNone.Checked;
      if XgridSolid.Checked then Grid.Style := psSolid;
      if XgridDotted.Checked then Grid.Style := psDot;
      Title.Caption := Xtitle.Text;
      Title.Font.Assign(Xtitle.Font);
      LabelsFont.Assign(Xtitle.Font);
    end;

  { Y Axis Page }
    if not IsPieChart then with LeftAxis do
    begin
      Automatic := Yauto.Checked;
      if not Automatic then
        SetAxisScaling(LeftAxis,Ymin.Text,Ymax.Text,Yinc.Text);
      Grid.Visible := not YgridNone.Checked;
      if YgridSolid.Checked then Grid.Style := psSolid;
      if YgridDotted.Checked then Grid.Style := psDot;
      Title.Caption := Ytitle.Text;
      Title.Font.Assign(Ytitle.Font);
      LabelsFont.Assign(Ytitle.Font);
    end;

  { Legend Page }
    Legend.Alignment := TLegendAlignment(LegendPosBox.ItemIndex);
    Legend.Color := Colors[LegendColorBox.ItemIndex];
    Legend.ColorWidth := LegendWidthBox.Value;
    if LegendFrameBox.Checked then
      Legend.Frame.Visible := True
    else
      Legend.Frame.Visible := False;
    Legend.Visible := LegendVisibleBox.Checked;
    Legend.Font.Assign(SeriesTitle.Font);

  { Series Page }
    GetSeriesOptions(theIndex);
    j := 0;
    for i := 0 to SeriesCount-1 do
    begin
      if Series[i].Active then
      begin
        SeriesOptions := TSeriesOptions(theSeries.Objects[j]);
        Series[i].Title := theSeries.Strings[j];
        with Series[i], SeriesOptions do
        begin
          Marks.Visible := LabelsVisible;
          Marks.Arrow.Visible := LabelsArrows;
          Marks.Transparent := LabelsTransparent;
          Marks.BackColor := LabelsBackColor;
          Marks.Style := TSeriesMarksStyle(LabelsStyle);
        end;
        if Series[i] is TLineSeries then
          with Series[i] as TLineSeries, SeriesOptions do
          begin
            LinePen.Visible := LineVisible;
            if LinePen.Visible then
              LinePen.Style := TPenStyle(LineStyle)
            else
              LinePen.Style := psClear;
            SeriesColor := LineColor;
            LinePen.Width := LineWidth;
            Pointer.Visible := PointVisible;
            Pointer.Style := TSeriesPointerStyle(PointStyle);
            Pointer.Brush.Color := PointColor;
            Pointer.HorizSize := PointSize;
            Pointer.VertSize := Pointer.HorizSize;
            LineBrush := TBrushStyle(AreaFillStyle);
            if (not Pointer.Visible) and (not LinePen.Visible) then
              ShowinLegend := False
            else
              ShowinLegend := True;
          end;
        if Series[i] is TPointSeries then
          with Series[i] as TPointSeries, SeriesOptions do
          begin
            Pointer.Visible := PointVisible;
            Pointer.Style := TSeriesPointerStyle(PointStyle);
            SeriesColor := PointColor;
            //Pointer.Brush.Color := PointColor;
            Pointer.HorizSize := PointSize;
            Pointer.VertSize := Pointer.HorizSize;
          end
        else if Series[i] is TBarSeries then
          with Series[i] as TBarSeries, SeriesOptions do
          begin
            BarBrush.Style := TBrushStyle(AreaFillStyle);
            if BarBrush.Style = bsSolid then
            begin
              SeriesColor := AreaFillColor;
              BarBrush.Color := AreaFillColor
            end
            else
            begin
              SeriesColor := LineColor;
              BarBrush.Color := AreaFillColor;
            end;
            MultiBar := TMultiBar(AreaStacking);
          end
        else if Series[i] is THorizBarSeries then
          with Series[i] as THorizBarSeries, SeriesOptions do
          begin
            BarBrush.Style := TBrushStyle(AreaFillStyle);
            if BarBrush.Style = bsSolid then
            begin
              SeriesColor := AreaFillColor;
              BarBrush.Color := AreaFillColor
            end
            else
            begin
              SeriesColor := LineColor;
              BarBrush.Color := AreaFillColor;
            end;
            MultiBar := TMultiBar(AreaStacking);
          end
        else if Series[i] is TAreaSeries then
          with Series[i] as TAreaSeries, SeriesOptions do
          begin
            AreaBrush := TBrushStyle(AreaFillStyle);
            SeriesColor := AreaFillColor;
            if AreaBrush = bsSolid then
              AreaColor := AreaFillColor
            else
              AreaColor := LineColor;
            AreaLinesPen.Visible := LineVisible;
            AreaLinesPen.Style := TPenStyle(LineStyle);
            AreaLinesPen.Color := LineColor;
            AreaLinesPen.Width := LineWidth;
          end
        else if Series[i] is TPieSeries then
          with Series[i] as TPieSeries, SeriesOptions do
          begin
            PiePen.Visible := LineVisible;
            PiePen.Style := TPenStyle(LineStyle);
            PiePen.Color := LineColor;
            PiePen.Width := LineWidth;
            Circled := PieCircled;
            UsePatterns := PieUsePatterns;
            RotationAngle := PieRotation;
          end;
        Inc(j);
      end;
    end;
  end;
end;

function TChartOptionsForm.GetColorIndex(aColor: TColor): Integer;
{-----------------------------------------------
   Finds index of aColor in Colors array.
------------------------------------------------}
var
  i: Integer;
begin
  for i := 0 to High(Colors) do
    if (aColor = Colors[i]) then
    begin
      Result := i;
      Exit;
    end;
  Result := High(Colors);
end;

procedure TChartOptionsForm.SetAxisScaling(Axis: TChartAxis;
            const Smin,Smax,Sinc: String);
{-------------------------------------------------
   Retrieves axis scaling options from form.
--------------------------------------------------}
var
  code: Integer;
  v   : Double;
begin
  with Axis do
  begin
    AutomaticMinimum := False;
    Val(Smin,v,code);
    if (code = 0) then
      Minimum := v
    else
      AutomaticMinimum := True;
    AutomaticMaximum := False;
    Val(Smax,v,code);
    if (code = 0) then
      Maximum := v
    else
      AutomaticMaximum := True;
    Val(Sinc,v,code);
    if (code = 0) then
      Increment := v
    else
      Increment := 0;
  end;
end;

procedure TChartOptionsForm.GraphTitleFontBtnClick(Sender: TObject);
{-------------------------------------------------
   OnClick handler for GraphTitleFontBtn button.
--------------------------------------------------}
begin
  with FontDialog1 do
  begin
    Font.Assign(GraphTitleBox.Font);
    if Execute then GraphTitleBox.Font.Assign(Font);
  end;
end;

procedure TChartOptionsForm.XfontBtnClick(Sender: TObject);
{-------------------------------------------------------
   OnClick handler for XfontBtn (X-axis font) button.
--------------------------------------------------------}
begin
  with FontDialog1 do
  begin
    Font.Assign(Xtitle.Font);
    if Execute then
    begin
      Xtitle.Font.Assign(Font);
      Ytitle.Font.Assign(Font);
    end;
  end;
end;

procedure TChartOptionsForm.YfontBtnClick(Sender: TObject);
{-------------------------------------------------------
   OnClick handler for YfontBtn (Y-axis font) button.
--------------------------------------------------------}
begin
  with FontDialog1 do
  begin
    Font.Assign(Ytitle.Font);
    if Execute then
    begin
      Ytitle.Font.Assign(Font);
      Xtitle.Font.Assign(Font);
    end;
  end;
end;

procedure TChartOptionsForm.LegendFontBtnClick(Sender: TObject);
{-------------------------------------------------------
   OnClick handler for LegendFontBtn button.
--------------------------------------------------------}
begin
  with FontDialog1 do
  begin
    Font.Assign(SeriesTitle.Font);
    if Execute then SeriesTitle.Font.Assign(Font);
  end;
end;

procedure TChartOptionsForm.LineStyleBoxDrawItem(Control: TWinControl;
  Index: Integer; Rect: TRect; State: TOwnerDrawState);
{------------------------------------------------------
   OnDrawItem handler for LineStyleBox combobox.
-------------------------------------------------------}
begin
  with Control as TComboBox, Canvas do
  begin
    FillRect(Rect);
    MoveTo(Rect.Left+2, (Rect.Top + Rect.Bottom) div 2);
    Pen.Style := TPenStyle(Index);
    if (odSelected in State) then
      Pen.Color := clWhite
    else
      Pen.Color := clBlack;
    LineTo(Rect.Right-2, (Rect.Top + Rect.Bottom) div 2);
  end;
end;

procedure TChartOptionsForm.SeriesListBoxClick(Sender: TObject);
{---------------------------------------------------------------
   OnClick handler for SeriesListBox combobox.
----------------------------------------------------------------}
begin
  if (Sender is TComboBox) then
    with Sender as TComboBox do
    begin
      GetSeriesOptions(theIndex);  {Store options for current series}
      theIndex := ItemIndex;       {Update value of current series}
      SetSeriesOptions(theIndex);  {Load series options into form}
    end;
end;

procedure TChartOptionsForm.LineStyleBoxChange(Sender: TObject);
{---------------------------------------------------------------
   OnChange handler for LineStyleBox combobox.
   Sets line thickness option to 1 when non-solid line chosen.
----------------------------------------------------------------}
begin
  if LineStyleBox.ItemIndex > 0 then
    LineSizeBox.Value := 1;
end;

procedure TChartOptionsForm.SetSeriesOptions(const Index: Integer);
{------------------------------------------------------
   Transfer options for data series Index to form.
------------------------------------------------------}
var
  SeriesOptions: TSeriesOptions;
begin
  SeriesTitle.Text := theSeries.Strings[Index];
  SeriesOptions := TSeriesOptions(theSeries.Objects[Index]);
  with SeriesOptions do
  begin
    LineStyleBox.ItemIndex := LineStyle;
    LineColorBox.ItemIndex := Ord(GetColorIndex(LineColor));
    LineSizeBox.Value := LineWidth;
    LineVisibleBox.Checked := LineVisible;
    MarkStyleBox.ItemIndex := PointStyle;
    MarkColorBox.ItemIndex := Ord(GetColorIndex(PointColor));
    MarkSizeBox.Value := PointSize;
    MarkVisibleBox.Checked := PointVisible;
    AreaFillStyleBox.ItemIndex := AreaFillStyle;
    AreaColorBox.ItemIndex := Ord(GetColorIndex(AreaFillColor));
    StackStyleBox.ItemIndex := AreaStacking;
    PieCircledBox.Checked := PieCircled;
    PiePatternBox.Checked := PieUsePatterns;
    PieRotationBox.Value := PieRotation;
    LabelsVisibleBox.Checked := LabelsVisible;
    LabelsTransparentBox.Checked := LabelsTransparent;
    LabelsBackColorBox.ItemIndex := Ord(GetColorIndex(LabelsBackColor));
    LabelsArrowsBox.Checked := LabelsArrows;
    LabelsStyleBox.ItemIndex := LabelsStyle;
  end;
  PieOptionsSheet.TabVisible := False;
  case SeriesOptions.SeriesType of
  stLine:
  begin
    LineOptionsSheet.TabVisible := True;
    MarkOptionsSheet.TabVisible := True;
    AreaOptionsSheet.TabVisible := True;
    LabelsOptionsSheet.TabVisible := True;
    PageControl2.ActivePage := LineOptionsSheet;
  end;
  stPoint:
  begin
    LineOptionsSheet.TabVisible := False;
    MarkOptionsSheet.TabVisible := True;
    AreaOptionsSheet.TabVisible := False;
    LabelsOptionsSheet.TabVisible := True;
    PageControl2.ActivePage := MarkOptionsSheet;
  end;
  stBar, stHorizBar:
  begin
    LineOptionsSheet.TabVisible := True;
    MarkOptionsSheet.TabVisible := False;
    AreaOptionsSheet.TabVisible := True;
    LabelsOptionsSheet.TabVisible := True;
    PageControl2.ActivePage := AreaOptionsSheet;
  end;
  stArea:
  begin
    LineOptionsSheet.TabVisible := True;
    MarkOptionsSheet.TabVisible := False;
    AreaOptionsSheet.TabVisible := True;
    LabelsOptionsSheet.TabVisible := True;
    PageControl2.ActivePage := AreaOptionsSheet;
  end;
  stPie:
  begin
    LineOptionsSheet.TabVisible := True;
    MarkOptionsSheet.TabVisible := False;
    AreaOptionsSheet.TabVisible := False;
    PieOptionsSheet.TabVisible := True;
    LabelsOptionsSheet.TabVisible := True;
    PageControl2.ActivePage := PieOptionsSheet;
  end;
  end;
end;

procedure TChartOptionsForm.GetSeriesOptions(const Index: Integer);
{------------------------------------------------------
   Transfer options from form to data series Index.
------------------------------------------------------}
var
  SeriesOptions: TSeriesOptions;
begin
  theSeries.Strings[Index] := SeriesTitle.Text;
  SeriesOptions := TSeriesOptions(theSeries.Objects[Index]);
  with SeriesOptions do
  begin
    if LineOptionsSheet.TabVisible then
    begin
      LineStyle := LineStyleBox.ItemIndex;
      LineColor := Colors[LineColorBox.ItemIndex];
      LineWidth := LineSizeBox.Value;
      LineVisible := LineVisibleBox.Checked;
    end;
    if MarkOptionsSheet.TabVisible then
    begin
      PointStyle := MarkStyleBox.ItemIndex;
      PointColor := Colors[MarkColorBox.ItemIndex];
      PointSize := MarkSizeBox.Value;
      PointVisible := MarkVisibleBox.Checked;
    end;
    if AreaOptionsSheet.TabVisible then
    begin
      AreaFillStyle := AreaFillStyleBox.ItemIndex;
      AreaFillColor := Colors[AreaColorBox.ItemIndex];
      AreaStacking := StackStyleBox.ItemIndex;
    end;
    if PieOptionsSheet.TabVisible then
    begin
      PieCircled := PieCircledBox.Checked;
      PieUsePatterns := PiePatternBox.Checked;
      PieRotation := PieRotationBox.Value;
    end;
    if LabelsOptionsSheet.TabVisible then
    begin
      LabelsVisible := LabelsVisibleBox.Checked;
      LabelsArrows := LabelsArrowsBox.Checked;
      LabelsTransparent := LabelsTransparentBox.Checked;
      LabelsBackColor := Colors[LabelsBackColorBox.ItemIndex];
      LabelsStyle := LabelsStyleBox.ItemIndex;
    end;
  end;
end;

end.
