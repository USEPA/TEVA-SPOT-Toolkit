unit Fproped;

{-------------------------------------------------------------------}
{                    Unit:    Fproped.pas                           }
{                    Project: EPANET2W                              }
{                    Version: 2.0                                   }
{                    Date:    5/29/00                               }
{                    Author:  L. Rossman                            }
{                                                                   }
{   Form unit that is a container for a TPropEdit component.        }
{   This component serves as the Property  Editor for network       }
{   objects and is styled after the Delphi object inspector.        }
{   The form is created on startup and remains active until the     }
{   application closes.                                             }
{-------------------------------------------------------------------}

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs,  ExtCtrls, PropEdit, Xprinter, Uglobals, Uutils;

const
  TXT_PROPERTY = 'Property';
  TXT_VALUE = 'Value';

type
  TPropEditForm = class(TForm)
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormDeactivate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private declarations }
    procedure EditorValidate(Sender: TObject; Index: Integer; var S: String;
      var Errmsg: String; var IsValid: Boolean);
    procedure EditorButtonClick(Sender: TObject; Index: Integer);
    procedure CallHelp;
  public
    { Public declarations }
    Editor: TPropEdit;
    procedure Print(Destination: TDestination);
  end;

var
  PropEditForm: TPropEditForm;

implementation

{$R *.DFM}

uses Uinput, Uinifile, Fbrowser, Fmain;

procedure TPropEditForm.FormCreate(Sender: TObject);
//-------------------------------------------------------------------
// OnCreate handler for the form.
// Creates a TPropEdit component to edit an object's properties.
// The event handler function for properties with an ellipsis button
// is EditorButtonClick. The event handler for validating editor
// input is EditorValidate.
//-------------------------------------------------------------------
begin
  Editor := TPropEdit.Create(self); //Create the TPropEdit component
  with Editor do
  begin
    Parent := self;
    ParentFont := True;
    Align := alClient;
    BorderStyle := bsNone;
    ColHeading1 := TXT_PROPERTY;
    ColHeading2 := TXT_VALUE;
    HeaderSplit := 65;
    ReadOnlyColor := clInfoBk;
    ValueColor := clNavy;
    OnButtonClick := EditorButtonClick;
    OnValidate := EditorValidate;
  end;
end;

procedure TPropEditForm.FormClose(Sender: TObject; var Action: TCloseAction);
//----------------------------------
// OnClose event handler for form.
//----------------------------------
begin
  Action := caHide;
end;

procedure TPropEditForm.FormDestroy(Sender: TObject);
//------------------------------------
// OnDestroy event handler for form.
//------------------------------------
begin
  Editor.Free;
end;

procedure TPropEditForm.FormDeactivate(Sender: TObject);
//----------------------------------------------------
// OnDeactivate event handler for form.
// Calls the Editor's IsValid function to validate
// value of current property when form looses focus.
//----------------------------------------------------
begin
  Editor.IsValid;
end;

procedure TPropEditForm.EditorValidate(Sender: TObject; Index: Integer;
  var S: String; var Errmsg: String; var IsValid: Boolean);
//-------------------------------------------------------------
// OnValidate event handler for the TPropEdit editor component.
// Passes value S of property index Index to the ValidateInput
// function in the Uinput unit.
//------------------------------------------------------------
begin
  IsValid := Uinput.ValidateInput(Index,S,Errmsg);
end;

procedure TPropEditForm.EditorButtonClick(Sender: TObject; Index: Integer);
//---------------------------------------------------------------
// OnButtonClick event handler. Activated when user clicks the
// ellipsis button for a specific property.
//---------------------------------------------------------------
begin
// User wants to edit a junction's list of demand categories
  if (CurrentList = JUNCS) and (Index = JUNC_DMNDCAT_INDEX+3) then
    Uinput.EditDemands(CurrentItem[CurrentList])

// User wants to edit a node's WQ source input
  else if (CurrentList in [JUNCS..TANKS]) then
    Uinput.EditSource(CurrentList, CurrentItem[CurrentList])

// User wants to edit a label's font
  else if (CurrentList = LABELS) then
    Uinput.EditLabelFont(CurrentItem[CurrentList]);
end;

procedure TPropEditForm.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
//----------------------------------------------------------------------
// OnKeyDown event handler for the form.
// Processes certain keystrokes to change which object is being edited.
//----------------------------------------------------------------------
begin
  case Key of

  // Shift-PgDown loads first object into editor.
  // PgDown loads prior object into editor.
    vk_PRIOR:
      begin
        if (CurrentItem[CurrentList] > 0) then
        begin
          if (ssCtrl in Shift) then CurrentItem[CurrentList] := 0
          else Dec(CurrentItem[CurrentList]);
          BrowserForm.UpdateBrowser(CurrentList, CurrentItem[CurrentList]);
        end;
        Key := 0;
      end;

  // Shift-PgUp loads last object into editor.
  // PgUp loads next object into editor.
    vk_NEXT:
      begin
        if (CurrentItem[CurrentList] < Network.Lists[CurrentList].Count-1) then
        begin
          if (ssCtrl in Shift) then
            CurrentItem[CurrentList] := Network.Lists[CurrentList].Count - 1
          else Inc(CurrentItem[CurrentList]);
          BrowserForm.UpdateBrowser(CurrentList, CurrentItem[CurrentList]);
        end;
        Key := 0;
      end;

  // Shift-Tab shifts focus to the MainForm
    vk_TAB:
      begin
        if (ssCtrl in Shift) then MainForm.SetFocus;
        Key := 0;
      end;

  // F1 brings up context sensitive Help
    vk_F1: CallHelp;

  // Escape closes the form
    vk_ESCAPE: Close;
  end;
end;


procedure TPropEditForm.Print(Destination: TDestination);
//----------------------------------------------------
// Prints contents of Property Editor.
// Called from the BrowserForm unit (Fbrowser.pas).
//----------------------------------------------------
var
  i: Integer;
  L: Single;
  W: Single;
  Plist,Vlist: TStringlist;
begin
  Plist := TStringlist.Create;    //List of properties
  Vlist := TStringlist.Create;    //List of values
  try
    with MainForm.thePrinter do
    begin
      BeginJob;
      SetFontInformation('Times New Roman',11,[]);
      SetDestination(Destination);
      with PageLayout do
      begin
        L := LMargin;
        W := GetPageWidth - LMargin - RMargin;
      end;
      CreateTable(2);
      SetColumnHeaderText(1,1,'Property');
      SetColumnHeaderAlignment(1,jLeft);
      SetColumnDimensions(1,L,2);
      SetColumnHeaderText(2,1,'Value');
      SetColumnHeaderAlignment(2,jLeft);
      SetColumnDimensions(2,L+2,W-2);
      Editor.GetProps(Plist, Vlist);
      SetTableStyle([sBorder, sVerticalGrid, sHorizontalGrid]);
      BeginTable;
      for i := 0 to Plist.Count-1 do
      begin
        PrintColumnLeft(1,Plist.Strings[i]);
        PrintColumnLeft(2,Vlist.Strings[i]);
        NextTableRow(False);
      end;
      EndTable;
      EndJob;
    end;
  finally
    Plist.Free;
    Vlist.Free;
  end;
end;

procedure TPropEditForm.CallHelp;
//--------------------------------------------------------
// Determines which Help topic to display when F1 pressed.
//--------------------------------------------------------
var
  HC: Integer;
begin
  case CurrentList of
    JUNCS:   HC := 154;
    RESERVS: HC := 155;
    TANKS:   HC := 156;
    PIPES:   HC := 160;
    PUMPS:   HC := 161;
    VALVES:  HC := 162;
    LABELS:  HC := 284;
    OPTS:    case CurrentItem[OPTS] of
             0: HC := 144;  //Hydraulics
             1: HC := 145;  //Quality
             2: HC := 146;  //Reactions
             3: HC := 148;  //Times
             4: HC := 149;  //Energy
             else HC := 0;
             end;
    else     HC := 0;
  end;
  if HC > 0 then Application.HelpContext(HC);
end;

end.
