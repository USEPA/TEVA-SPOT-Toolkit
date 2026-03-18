unit Dcontour;

{-------------------------------------------------------------------}
{                    Unit:    Dcontour.pas                          }
{                    Project: EPANET2W                              }
{                    Version: 2.0                                   }
{                    Date:    5/29/00                               }
{                    Author:  L. Rossman                            }
{                                                                   }
{   Form unit with a dialog box that retrieves options for          }
{   displaying a Contour Plot.                                      }
{-------------------------------------------------------------------}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Spin, Uglobals, Fcontour, ExtCtrls, ComboColor;

const
  Colors: array[0..15] of TColor =
    (clBlack,clMaroon,clGreen,clOlive,clNavy,clPurple,clTeal,clGray,clSilver,
     clRed,clLime,clYellow,clBlue,clFuchsia,clAqua,clWhite);
  ColorText: array[0..15] of PChar =
    ('Black','Maroon','Green','Olive','Navy','Purple','Teal','Gray','Silver',
     'Red','Lime','Yellow','Blue','Fuchsia','Aqua','White');
type
  TContourOptionsForm = class(TForm)
    BtnOK: TButton;
    BtnCancel: TButton;
    BtnHelp: TButton;
    LegendGroup: TGroupBox;
    LgndDisplay: TCheckBox;
    StyleGroup: TGroupBox;
    StyleFilled: TRadioButton;
    StyleLines: TRadioButton;
    NetworkGroup: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    ForeColorBox: TComboColor;
    BackColorBox: TComboColor;
    LinkSize: TSpinEdit;
    ContourGroup: TGroupBox;
    NumLines: TSpinEdit;
    LineSize: TSpinEdit;
    DefaultBox: TCheckBox;
    LgndModify: TButton;
    procedure FormCreate(Sender: TObject);
    procedure LgndModifyClick(Sender: TObject);
    procedure StyleFilledClick(Sender: TObject);
    procedure StyleLinesClick(Sender: TObject);
    procedure BtnOKClick(Sender: TObject);
    procedure BtnCancelClick(Sender: TObject);
    procedure BtnHelpClick(Sender: TObject);
  private
    { Private declarations }
    Cform: TContourForm;
    function GetColorIndex(aColor: TColor): Integer;
  public
    { Public declarations }
    procedure LoadOptions(Sender: TObject);
    procedure UnloadOptions;
  end;

//var
//  ContourOptionsForm: TContourOptionsForm;

implementation

{$R *.DFM}

uses Umap;

procedure TContourOptionsForm.FormCreate(Sender: TObject);
//-------------------------------------------------------
// OnCreate handler for form.
//-------------------------------------------------------
var
  i: Integer;
begin

// Set font size and style
  Uglobals.SetFont(self);

// Load colors into ComboColor controls
  for i := 0 to High(Colors) do
  begin
    BackColorBox.AddColor(ColorText[i],Colors[i]);
    ForeColorBox.AddColor(ColorText[i],Colors[i]);
  end;
end;

procedure TContourOptionsForm.LoadOptions(Sender: TObject);
//--------------------------------------------------------
// Loads existing options of the Sender TContourForm
// which invoked the dialog box. This form is saved
// in variable Cform so it can be used in the UnloadOptions
// procedure to assign updated options back to itself.
//--------------------------------------------------------
begin
  if (Sender is TContourForm) then with Sender as TContourForm do
  begin
    Cform := TContourForm(Sender);
    LgndDisplay.Checked := Cform.LegendPanel.Visible;
    if (Cform.Options.Style = csFilled) then StyleFilled.Checked := True;
    if (Cform.Options.Style = csLines) then StyleLines.Checked := True;
    BackColorBox.ItemIndex := GetColorIndex(Cform.Options.BackColor);
    ForeColorBox.ItemIndex := GetColorIndex(Cform.Options.ForeColor);
    LinkSize.Value := Cform.Options.LinkSize;
    LineSize.Value := Cform.Options.LineSize;
    NumLines.Value := Cform.Options.NumLines;
  end;
end;

procedure TContourOptionsForm.UnLoadOptions;
//-----------------------------------------------------
// Unloads options selected in the dialog back into the
// TContourForm (Cform) which invoked the dialog.
//-----------------------------------------------------
begin
  with Cform do
  begin
    LegendPanel.Visible := LgndDisplay.Checked;
    if (StyleFilled.Checked) then Options.Style := csFilled;
    if (StyleLines.Checked) then Options.Style := csLines;
    Options.BackColor := Colors[BackColorBox.ItemIndex];
    Options.ForeColor := Colors[ForeColorBox.ItemIndex];
    Options.LinkSize := LinkSize.Value;
    if (StyleLines.Checked) then
    begin
      Options.LineSize := LineSize.Value;
      Options.NumLines := NumLines.Value;
    end;
  end;
  if (DefaultBox.Checked) then
  begin
    if (StyleFilled.Checked) then DefContourOptions.Style := csFilled;
    if (StyleLines.Checked) then DefContourOptions.Style := csLines;
    DefContourOptions.BackColor := Colors[BackColorBox.ItemIndex];
    DefContourOptions.ForeColor := Colors[ForeColorBox.ItemIndex];
    DefContourOptions.LinkSize := LinkSize.Value;
    if (StyleLines.Checked) then
    begin
      DefContourOptions.LineSize := LineSize.Value;
      DefContourOptions.NumLines := NumLines.Value;
    end;
  end;
end;

procedure TContourOptionsForm.LgndModifyClick(Sender: TObject);
//------------------------------------------------------------
// OnClick handler for the "Modify Legend" button.
// Launches a dialog box, from the Umap unit, that modifies
// the Contour Plot's legend.
//------------------------------------------------------------
begin
  Umap.EditLegend(Cform.Legend,Cform.TimePeriod,Cform.MapColor,
    Cform.LegendFrame.Framed);
end;

procedure TContourOptionsForm.StyleFilledClick(Sender: TObject);
//----------------------------------------------------------------------
// OnClick handler for StyleFilled checkbox.
// Disables the Contour Lines options when filled contours are selected.
//----------------------------------------------------------------------
begin
  ContourGroup.Enabled := False;
end;

procedure TContourOptionsForm.StyleLinesClick(Sender: TObject);
//-------------------------------------------------------------------
// OnClick handler for StyleLines checkbox.
// Enables the Contour Lines options when line contours are selected.
//-------------------------------------------------------------------
begin
  ContourGroup.Enabled := True;
end;

procedure TContourOptionsForm.BtnOKClick(Sender: TObject);
//-------------------------------------------------------
// OnClick handler for "OK" button.
//-------------------------------------------------------
begin
  Hide;
  ModalResult := mrOK;
end;

procedure TContourOptionsForm.BtnCancelClick(Sender: TObject);
//-----------------------------------------------------------
// OnClick handler for "Cancel" button.
//-----------------------------------------------------------
begin
  Hide;
  ModalResult := mrCancel;
end;

function TContourOptionsForm.GetColorIndex(aColor: TColor): Integer;
//-----------------------------------------------------------------
// Determines position in Colors array occupied by color aColor.
//-----------------------------------------------------------------
var
  i: Integer;
begin
  for i := 0 to High(Colors) do
    if (aColor = Colors[i]) then break;
  Result := i;
end;

procedure TContourOptionsForm.BtnHelpClick(Sender: TObject);
begin
  Application.HelpContext(231);
end;

end.
