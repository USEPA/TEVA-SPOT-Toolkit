{
   TChartDialog Component
   **********************

   A dialog box component for modifying the properties
   of a TeeChart TChart. Works in conjunction with the
   ChartOptionsForm form in the CDForm.pas unit. See
   file ChartDlg.doc for documentation.

   Author:      L. Rossman
   Version:     1.0
   Date:        1/30/00
}

unit ChartDlg;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Chart, CDForm;

type
  TPageType = (General, HorizAxis, VertAxis, Legend, Series);
  TChartDialog = class(TComponent)
  private
    { Private declarations }
    FBoldFont: Boolean;
    FCaption: String;
    FChart: TChart;
    FDefaultBox: Boolean;
    FDefaultChecked: Boolean;
    FHelpButton: Boolean;
    FHelpContext: THelpContext;
    FGlyphButtons: Boolean;
    FTrueTypeFont: Boolean;
  protected
    { Protected declarations }
  public
    { Public declarations }
    function Execute: Boolean;
  published
    { Published declarations }
    property BoldFont: Boolean read FBoldFont write FBoldFont;
    property Caption: String read FCaption write FCaption;
    property Chart: TChart read FChart write FChart;
    property DefaultBox: Boolean read FDefaultBox write FDefaultBox;
    property DefaultChecked: Boolean read FDefaultChecked;
    property HelpButton: Boolean read FHelpButton write FHelpButton;
    property HelpContext: THelpContext read FHelpContext write FHelpContext;
    property GlyphButtons: Boolean read FGlyphButtons write FGlyphButtons;
    property TrueTypeFont: Boolean read FTrueTypeFont write FTrueTypeFont;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Additional', [TChartDialog]);
end;

function TChartDialog.Execute: Boolean;
begin
  if Chart = nil then Result := False
  else
  begin
    FDefaultChecked := False;
    ChartOptionsForm := TChartOptionsForm.Create(Application);
    try
      ChartOptionsForm.Caption := Caption;
      ChartOptionsForm.HelpContext := HelpContext;
      if not GlyphButtons then with ChartOptionsForm do
      begin
        BitBtn1.Glyph := nil;
        BitBtn2.Glyph := nil;
        BitBtn3.Glyph := nil;
      end;
      ChartOptionsForm.BitBtn3.Visible := HelpButton;
      if not HelpButton then with ChartOptionsForm do
      begin
        BitBtn1.Left := BitBtn2.Left;
        BitBtn2.Left := BitBtn3.Left;
      end;
      ChartOptionsForm.DefaultBox.Visible := DefaultBox;
      if TrueTypeFont then ChartOptionsForm.Font.Name := 'Arial';
      if BoldFont then ChartOptionsForm.Font.Style := [fsBold];
      with ChartOptionsForm do
      begin
        LoadOptions(Chart);
        if ShowModal = mrOK then
        begin
          UnloadOptions(Chart);
          Chart.Refresh;
          FDefaultChecked := DefaultBox.Checked;
          Result := True;
        end
        else Result := False;
      end;
    finally
      ChartOptionsForm.Free;
    end;
  end;
end;
end.
