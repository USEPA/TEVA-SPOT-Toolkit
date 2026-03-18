unit Dprefers;

{-------------------------------------------------------------------}
{                    Unit:    Dprefers.pas                          }
{                    Project: EPANET2W                              }
{                    Version: 2.0                                   }
{                    Date:    5/29/00                               }
{                    Author:  L. Rossman                            }
{                                                                   }
{   Form unit with a dialog box for setting program preferences.    }
{-------------------------------------------------------------------}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Spin, StdCtrls, ComCtrls, FileCtrl, Uglobals, Uutils, ExtCtrls;

const
  MSG_NO_DIRECTORY = ' - directory does not exist.';
  MSG_SELECT_NUMBER_OF = 'Select number of decimal places to';
  MSG_WHEN_DISPLAYING = 'use when displaying computed results';

type
  TPreferencesForm = class(TForm)
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    CheckBoldFonts: TCheckBox;
    CheckBlinking: TCheckBox;
    CheckFlyOvers: TCheckBox;
    CheckAutoBackup: TCheckBox;
    Label5: TLabel;
    EditTempDir: TEdit;
    DirSelectBtn: TButton;
    Label1: TLabel;
    Label2: TLabel;
    NodeVarBox: TComboBox;
    NodeVarSpin: TSpinEdit;
    Label3: TLabel;
    Label4: TLabel;
    LinkVarBox: TComboBox;
    LinkVarSpin: TSpinEdit;
    BtnOK: TButton;
    BtnCancel: TButton;
    BtnHelp: TButton;
    Panel1: TPanel;
    Label6: TLabel;
    CheckConfirmDelete: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure NodeVarSpinChange(Sender: TObject);
    procedure LinkVarSpinChange(Sender: TObject);
    procedure NodeVarBoxChange(Sender: TObject);
    procedure LinkVarBoxChange(Sender: TObject);
    procedure BtnOKClick(Sender: TObject);
    procedure BtnCancelClick(Sender: TObject);
    procedure DirSelectBtnClick(Sender: TObject);
    procedure BtnHelpClick(Sender: TObject);
  private
    { Private declarations }
    NodeDigits: array[DEMAND..NODEQUAL] of Integer;
    LinkDigits: array[FLOW..LINKQUAL] of Integer;
    function SetPreferences: Boolean;
  public
    { Public declarations }
  end;

//var
//  PreferencesForm: TPreferencesForm;

implementation

{$R *.DFM}

uses Fmain, Fbrowser;

procedure TPreferencesForm.FormCreate(Sender: TObject);
//----------------------------------------------------
// OnCreate handler for form.
//----------------------------------------------------
var
  i: Integer;
begin
// Set font size & style
  Uglobals.SetFont(self);

// Initialize general preferences
  CheckBoldFonts.Checked := BoldFonts;
  CheckBlinking.Checked := Blinking;
  CheckFlyOvers.Checked := FlyOvers;
  CheckAutoBackup.Checked := AutoBackup;
  CheckConfirmDelete.Checked := ConfirmDelete;
  EditTempDir.Text := TempDir;

// Assign items to node & link variable combo boxes
  for i := DEMAND to NODEQUAL do
  begin
    NodeVarBox.Items.Add(NodeVariable[i].Name);
    NodeDigits[i] := NodeUnits[i].Digits;
  end;
  for i := FLOW to LINKQUAL do
  begin
    LinkVarBox.Items.Add(LinkVariable[i].Name);
    LinkDigits[i] := LinkUnits[i].Digits;
  end;
  NodeVarBox.ItemIndex := 0;
  NodeVarSpin.Value := NodeDigits[DEMAND];
  LinkVarBox.ItemIndex := 0;
  LinkVarSpin.Value := LinkDigits[FLOW];
  Label6.Caption := MSG_SELECT_NUMBER_OF + #13 + MSG_WHEN_DISPLAYING;
  PageControl1.ActivePage := TabSheet1;
end;

procedure TPreferencesForm.NodeVarSpinChange(Sender: TObject);
//-----------------------------------------------------------
// OnChange handler for SpinEdit control that
// sets decimal places for a node variable.
//-----------------------------------------------------------
begin
  NodeDigits[NodeVarBox.ItemIndex+DEMAND] := NodeVarSpin.Value;
end;

procedure TPreferencesForm.LinkVarSpinChange(Sender: TObject);
//-----------------------------------------------------------
// OnChange handler for SpinEdit control that
// sets decimal places for a link variable.
//-----------------------------------------------------------
begin
  LinkDigits[LinkVarBox.ItemIndex+FLOW] := LinkVarSpin.Value;
end;

procedure TPreferencesForm.NodeVarBoxChange(Sender: TObject);
//-----------------------------------------------------------
// OnChange handler for ComboBox control that
// selects a node variable.
//-----------------------------------------------------------
begin
  NodeVarSpin.Value := NodeDigits[NodeVarBox.ItemIndex+DEMAND];
end;

procedure TPreferencesForm.LinkVarBoxChange(Sender: TObject);
//-----------------------------------------------------------
// OnChange handler for ComboBox control that
// selects a link variable.
//-----------------------------------------------------------
begin
  LinkVarSpin.Value := LinkDigits[LinkVarBox.ItemIndex+FLOW];
end;

procedure TPreferencesForm.BtnOKClick(Sender: TObject);
//------------------------------------------------------
// OnClick handler for OK button.
//------------------------------------------------------
begin
  if SetPreferences then ModalResult := mrOK;
end;

procedure TPreferencesForm.BtnCancelClick(Sender: TObject);
//------------------------------------------------------
// OnClick handler for Cancel button.
//------------------------------------------------------
begin
  ModalResult := mrCancel;
end;

procedure TPreferencesForm.DirSelectBtnClick(Sender: TObject);
//-----------------------------------------------------------
// OnClick handler for Select button that selects a
// choice of Temporary Folder. Uses built-in Delphi
// function SelectDirectory to display a directory
// selection dialog box.
//-----------------------------------------------------------
var
  s: String;
begin
  s := EditTempDir.Text;
  if SelectDirectory(s,[sdAllowCreate,sdPerformCreate,sdPrompt],0) then
    EditTempDir.Text := s;
end;

function TPreferencesForm.SetPreferences: Boolean;
//------------------------------------------------------------
// Transfers contents of form to program preference variables.
//------------------------------------------------------------
var
  j: Integer;
  buffer: String;
begin
// Use default temporary directory if user has a blank entry
  buffer := Trim(EditTempDir.Text);
  if (Length(buffer) = 0) then
  begin
    TempDir := Uutils.GetTempFolder;
    if TempDir = '' then TempDir := EpanetDir;
  end

// Otherwise check that user's directory choice exists
  else
  begin
    if buffer[Length(buffer)] <> '\' then buffer := buffer + '\';
    if DirectoryExists(buffer) then TempDir := buffer
    else
    begin
      MessageDlg(buffer + MSG_NO_DIRECTORY, mtWARNING, [mbOK], 0);
      EditTempDir.Text := TempDir;
      PageControl1.ActivePage := TabSheet1;
      EditTempDir.SetFocus;
      Result := False;
      Exit;
    end;
  end;

// Save the other preferences to their respective global variables.
  BoldFonts := CheckBoldFonts.Checked;
  Blinking := CheckBlinking.Checked;
  FlyOvers := CheckFlyOvers.Checked;
  AutoBackup := CheckAutoBackup.Checked;
  ConfirmDelete := CheckConfirmDelete.Checked;
  for j := DEMAND to NODEQUAL do NodeUnits[j].Digits := NodeDigits[j];
  for j := FLOW to LINKQUAL do LinkUnits[j].Digits := LinkDigits[j];
  Result := True;
end;

procedure TPreferencesForm.BtnHelpClick(Sender: TObject);
begin
   with PageControl1 do
     if ActivePage = TabSheet1 then
       Application.HelpContext(137)
     else
       Application.HelpContext(142);
end;

end.
