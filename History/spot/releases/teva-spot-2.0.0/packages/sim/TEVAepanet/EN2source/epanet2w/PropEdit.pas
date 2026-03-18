//********************************************************************
//  PropEdit.Pas
//
//  Property Editor Component for Delphi
//
//  Version 1.0
//  Written by L. Rossman
//  Updated 12/6/01
//
//  This is a TCustomPanel descendant that is used for editing an
//  array of property values displayed in a 2-column grid. One
//  column displays the property names and the other their values.
//  All property values are stored and displayed as string values.
//
//  Information about each property is contained in a TPropRecord
//  structure, which consists of the following items:
//    Name   -- a string containing the property's name
//    Style  -- one of the following editing styles:
//              esReadOnly  (cannot be edited)
//              esEdit      (edited in an Edit control)
//              esComboList (edited in a csDropDownList ComboBox)
//              esComboEdit (edited in a csDropDown ComboBox)
//              esButton    (edited via an OnClick event handler)
//    Mask   -- an edit mask that applies when Style is esEdit:
//              emNone      (any character is accepted)
//              emNumber    (text must be a number)
//              emPosNumber (text must be a number >= 0)
//              emNoSpace   (text cannot have any spaces in it)
//    Length -- maximum length of text (applies to Style esEdit)
//    List   -- string that contains a list of possible choices for
//              esComboList or esComboEdit styles, where each item is
//              separated by CHR(13).
//
//  The editor is populated via the SetProps method which takes
//  as arguments an array of property records, a stringlist with
//  values for each property, and an integer index representing
//  the property to highlight first in the editor.
//
//  The OnValidate event can be used to validate and record the
//  property values immediately after they are edited.
//
//********************************************************************

unit PropEdit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, ComCtrls, Grids;

type
// Types of edit field styles
  TEditStyle = (esReadOnly, esEdit, esComboList, esComboEdit, esButton);

// Types of restrictions on an esEdit entry
  TEditMask = (emNone, emNumber, emPosNumber, emNoSpace);

// Information associated with each property
  TPropRecord = record
    Name    : String;      // property name
    Style   : TEditStyle;  // style of edit field
    Mask    : TEditMask;   // restrictions on field entry
    Length  : Integer;     // max. length of text entry
    List    : String;      // list of option choices
  end;

// Declare an array of Property records and a pointer to such an array
  TPropArray = Array [0..0] of TPropRecord;
  PPropArray = ^TPropArray;

// Declare event procedures used to validate an edited property value
// and to launch any special editor associated with a property
  TValidateEvent = procedure(Sender: TObject; Index: Integer;
    var S: String; var Errmsg: String; var IsValid: Boolean) of Object;
  TButtonClickEvent = procedure(Sender: TObject; Index: Integer) of Object;

// Define special exception for invalid property values
  EInvalidProperty = class(Exception);

// Declaration of the TPropEdit class
  TPropEdit = class(TCustomPanel)
  private
    { Private declarations }
    FHeader        : THeader;             // Header control for column labels
    FGrid          : TStringGrid;         // StringGrid to display values
    FEdit          : TEdit;               // Edit control for editing values
    FCombo         : TComboBox;           // Combobox for selecting choices
    FModified      : Boolean;             // Modified flag
    FRow           : Integer;             // Current property index
    FReadOnlyColor : TColor;              // Back color of read-only values
    FValueColor    : TColor;              // Fore color of property values
    FColHeading1   : String;              // Heading of property name column
    FColHeading2   : String;              // Heading of property value column
    FHeaderSplit   : Integer;             // % of total width of Name column
    FProps         : PPropArray;          // Pointer to array of properties
    FValues        : TStrings;            // Stringlist with property values
    FOnValidate    : TValidateEvent;      // Property value validation event
    FOnButtonClick : TButtonClickEvent;   // Editor button OnClick event
    ButtonPressed  : Boolean;             // True if editor button pressed
    ButtonVisible  : Boolean;             // True if editor button visible
    VisibleRows    : Integer;             // Number of rows showing in editor
    CXVScroll      : Integer;             // Width of system horiz. scrollbar
    RowHeight      : Integer;             // Row height of StringGrid display
    DecimalChar    : Char;                // Character used for decimal point
    ComponentsCreated: Boolean;           // True if sub-components created
    EditMask       : TEditMask;           // Edit mask for current property
    procedure CreateComponents;
    procedure DrawButton(aCanvas: TCanvas; aRect: TRect);
    procedure EditProperty(CurCol, CurRow: LongInt; Key: Char);
    procedure SetEditBounds(aControl : TWinControl);
    function  GoValidate(S: String): Boolean;
  protected
    { Protected declarations }
    procedure Resize; override;
    procedure ResizeSection(Sender: TObject; aSection, aWidth: Integer);
    procedure ResizeGrid(Sender: TObject);
    procedure EditKeyPress(Sender: TObject; var Key: Char);
    procedure EditKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure EditChange(Sender: TObject);
    procedure EditExit(Sender: TObject);
    procedure GridKeyPress(Sender: TObject; var Key: Char);
    procedure GridKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure GridClick(Sender: TObject);
    procedure GridMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure GridMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure GridDrawCell(Sender: TObject; vCol, vRow: Longint;
      Rect: TRect; State: TGridDrawState);
    procedure GridTopLeftChanged(Sender: TObject);
    procedure SetReadOnlyColor(Value: TColor);
    procedure SetValueColor(Value: TColor);
    procedure SetColHeading1(Value: String);
    procedure SetColHeading2(Value: String);
    procedure SetHeaderSplit(Value: Integer);
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
  published
    { Published declarations }
    procedure SetProps(var PropArray: array of TPropRecord; Values: TStrings);
    procedure GetProps(Plist, Vlist: TStringlist);
    procedure Edit;
    function  IsValid: Boolean;
    property  Modified: Boolean read FModified;
    property  Row: Integer read FRow;
    property  ReadOnlyColor: TColor read FReadOnlyColor write SetReadOnlyColor;
    property  ValueColor: TColor read FValueColor write SetValueColor;
    property  ColHeading1: String read FColHeading1 write SetColHeading1;
    property  ColHeading2: String read FColHeading2 write SetColHeading2;
    property  HeaderSplit: Integer read FHeaderSplit write SetHeaderSplit;
    property  OnValidate: TValidateEvent read FOnValidate write FOnValidate;
    property  OnButtonClick: TButtonClickEvent read FOnButtonClick
                 write FOnButtonClick;
    property  Align;
    property  BevelInner default bvNone;
    property  BevelOuter default bvLowered;
    property  BevelWidth default 1;
    property  BorderStyle default bsNone;
    property  BorderWidth default 0;
    property  Ctl3D default True;
    property  Font;
    property  Height default 100;
    property  Left;
    property  ParentFont;
    property  Top;
    property  Visible;
    property  Width default 100;
    property  OnClick;
    property  OnDblClick;
    property  OnEnter;
    property  OnExit;
    property  OnKeyDown;
    property  OnKeyPress;
    property  OnKeyUp;
  end;

procedure Register;

implementation

procedure Register;
begin
//  RegisterComponents('Extensions', [TPropEdit]);
end;

constructor TPropEdit.Create(AOwner:TComponent);
//----------------------------------------------
// Component constructor
//----------------------------------------------
begin
  inherited Create(AOwner);
  Width := 100;
  Height := 100;
  BevelInner := bvNone;
  BevelOuter := bvLowered;
  BevelWidth := 1;
  BorderStyle := bsNone;
  BorderWidth := 0;
  Ctl3D := True;
  ReadOnlyColor := $0080FFFF;
  ValueColor := clNavy;
  ColHeading1 := 'Property';
  ColHeading2 := 'Value';
  FHeaderSplit := 50;
  FModified := False;
  FRow := 0;
  ButtonPressed := False;
  ButtonVisible := False;
  ComponentsCreated := False;
  DecimalChar := DecimalSeparator;
end;

destructor TPropEdit.Destroy;
//-----------------------------------------------
// Component destructor. Must free sub-components
// in the order listed.
//-----------------------------------------------
begin
  if ComponentsCreated then
  begin
    FCombo.Free;
    FEdit.Free;
    FGrid.Free;
    FHeader.Free;
  end;
  ComponentsCreated := False;
  inherited Destroy;
end;

procedure TPropEdit.CreateComponents;
//-------------------------------------
// Creates sub-components that populate
// the main component.
//-------------------------------------
begin

// FHeader displays column labels
  FHeader := THeader.Create(self);
  FHeader.Parent := self;
  with FHeader do
  begin
    Align := alTop;
    AllowResize := True;
    Visible := True;
    OnSized := ResizeSection;
  end;

// FGrid displays properties & values
  FGrid := TStringGrid.Create(self);
  FGrid.Parent := self;
  with FGrid do
  begin
    Align := alClient;
    BorderStyle := bsNone;
    ColCount := 2;
    Ctl3D := False;
    Color := clBtnFace;
    Options := Options - [goRangeSelect];
    FixedCols := 0;
    FixedRows := 0;
    RowCount := 1;
    ScrollBars := ssVertical;
    Visible := True;
    OnKeypress := GridKeyPress;
    OnKeyDown := GridKeyDown;
    OnClick := GridClick;
    OnMouseDown := GridMouseDown;
    OnMouseUp := GridMouseUp;
    OnDrawCell := GridDrawCell;
    OnTopLeftChanged := GridTopLeftChanged;
  end;

//Create the edit & combobox controls last so that
//they can appear in front of the grid when activated.

// FEdit is used to edit numbers and strings
  FEdit := TEdit.Create(self);
  with FEdit do
  begin
    Parent := self;
    Ctl3D := True;
    BorderStyle := bsSingle;
    AutoSelect := True;
    Visible := False;
    OnKeyPress := EditKeyPress;
    OnKeyDown := EditKeyDown;
    OnChange := EditChange;
  end;

// FCombo is used to edit choices
  FCombo := TComboBox.Create(self);
  with FCombo do
  begin
    Parent := self;
    Ctl3D := True;
    Style := csDropDown;
    Visible := False;
    OnKeyPress := EditKeyPress;
    OnKeyDown := EditKeyDown;
  end;

  RowHeight := FCombo.Height;
  FGrid.DefaultRowHeight := RowHeight;
  ComponentsCreated := True;
  Resize;
end;

//========================================================
// Property Servers
//========================================================

procedure TPropEdit.SetReadOnlyColor(Value: TColor);
begin
  if FReadOnlyColor <> Value then FReadOnlyColor := Value;
end;

procedure TPropEdit.SetValueColor(Value: TColor);
begin
  if FValueColor <> Value then FValueColor := Value;
end;

procedure TPropEdit.SetColHeading1(Value: String);
var
  swidth: Integer;
begin
  if FColHeading1 <> Value then
  begin
    FColHeading1 := Value;
    if ComponentsCreated then with FHeader do
    begin
      swidth := SectionWidth[0];
      Sections[0] := Value;
      SectionWidth[0] := swidth;
    end;
  end;
end;

procedure TPropEdit.SetColHeading2(Value: String);
var
  swidth: Integer;
begin
  if FColHeading2 <> Value then
  begin
    FColHeading2 := Value;
    if ComponentsCreated then with FHeader do
    begin
      swidth := SectionWidth[1];
      Sections[1] := Value;
      SectionWidth[1] := swidth;
    end;
  end;
end;

procedure TPropEdit.SetHeaderSplit(Value: Integer);
begin
  if FHeaderSplit <> Value then
  begin
    if (Value < 1) or (Value > 99) then Exit;
    FHeaderSplit := Value;
    if ComponentsCreated then with FHeader do
    begin
      SectionWidth[0] := (Width*Value) div 100;
    end;
  end;
end;

function TPropEdit.IsValid;
begin
  Result := True;
  if not ComponentsCreated then Exit;
  if (FCombo.Visible) then Result := GoValidate(FCombo.Text);
  if (FEdit.Visible) then Result := GoValidate(FEdit.Text);
end;

//========================================================
// Re-sizing procedures
//========================================================

procedure TPropEdit.Resize;
var
  S: String;
begin
  inherited Resize;
  if ComponentsCreated then
  begin

  //Initialize Header sections
    with FHeader do
    begin
      Height := RowHeight;
      if Sections.Count = 0 then
      begin
        S := FColHeading1 + #13 + FColHeading2;
        Sections.SetText(PChar(S));
        SectionWidth[0] := (Width*FHeaderSplit) div 100;
      end;
    end;

  //Resize Grid & reposition active edit control
    if (FEdit.Visible) then EditExit(FEdit);
    if (FCombo.Visible) then EditExit(FCombo);
    ResizeGrid(Self);
    if (FEdit.Visible) then SetEditBounds(FEdit);
    if (FCombo.Visible) then SetEditBounds(FCombo);
  end;
end;

procedure TPropEdit.ResizeSection(Sender: TObject; aSection, aWidth: Integer);
begin
// Save new HeaderSplit value
  FHeaderSplit := (FHeader.SectionWidth[0]*100)  div FHeader.Width;

  //Resize Grid column widths
  FGrid.ColWidths[0] := FHeader.SectionWidth[0];
  FGrid.ColWidths[1] := FHeader.SectionWidth[1] - CXVScroll -1;

//Resize active edit control
  if (FEdit.Visible) then SetEditBounds(FEdit);
  if (FCombo.Visible) then SetEditBounds(FCombo);
end;

procedure TPropEdit.ResizeGrid(Sender: TObject);
begin

  VisibleRows := (ClientHeight - FHeader.Height) div
                   (RowHeight + 1);
  with FGrid do
  begin

  //Determine number of visible rows
    DefaultRowheight := RowHeight;
    if VisibleRows > RowCount then VisibleRows := RowCount;
    Height := VisibleRows*(RowHeight + 1);

  //Save width of scrollbar if one is needed
    if VisibleRows < RowCount then
      CXVScroll := GetSystemMetrics(SM_CXVSCROLL)
    else
      CXVScroll := 0;

  //Establish column widths
    DefaultColWidth := (ClientWidth - CXVScroll - 2) div 2;
    if FHeader.SectionWidth[0] < ClientWidth then
    begin
      ColWidths[0] := FHeader.SectionWidth[0];
      ColWidths[1] := FHeader.SectionWidth[1] - CXVScroll -1;
    end
    else FHeader.SectionWidth[0] := DefaultColWidth;
  end;

end;

//========================================================
// Editor control event handlers
//========================================================

procedure TPropEdit.EditKeyPress(Sender: TObject; var Key: Char);
//---------------------------------------------------------
// Processes Enter & Escape key presses in editing control.
//---------------------------------------------------------

{*** Updated 12/6/01 ***}
var
  S: String;

begin
  if Key = #13 then   {Enter key}
  begin
    if (Sender = FCombo)
    and (FCombo.DroppedDown) then Exit;
    EditExit(Sender);
    Key := #0;
  end
  else if Key = #27 then   {Escape key}
  begin
    with Sender as TwinControl do Visible := False;
    FGrid.SetFocus;
    Key := #0;
  end
  else if (Sender = FEdit) then
  begin
    if (EditMask in [emNumber, emPosNumber]) then with Sender as TEdit do

{*** Updated 12/6/01 ***}
    begin
      if not (Key in ['0'..'9','-',DecimalChar,#8]) then Key := #0
      else
      begin
        S := Text;
        Delete(S, SelStart+1, SelLength);
        if EditMask = emPosNumber then
        begin
          if Key = '-' then Key := #0;
          if (Key = DecimalChar) and (Pos(Key,S) > 0) then Key := #0;
        end
        else
          if (Key in [DecimalChar,'-']) and (Pos(Key,S) > 0) then Key := #0;
      end;
    end
{***********************}

    else if (EditMask = emNoSpace) then with Sender as TEdit do
    begin
      if (Key = ' ') then Key := #0;
    end;
  end;
end;


procedure TPropEdit.EditKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
//-------------------------------------------------------
// Processes Up & Down arrow key presses and
// Page Up & Page Down key presses in edit control.
//-------------------------------------------------------
begin
  if Key in [VK_UP,VK_DOWN,VK_PRIOR,VK_NEXT] then
  begin
  { Check if Combobox is dropped down }
    if (Sender = FCombo) and (FCombo.DroppedDown) then Exit
    else EditExit(Sender);

  { Pass keystroke onto Grid }
    SendMessage(FGrid.Handle,WM_KEYDOWN,Key,0);
  end;
end;

procedure TPropEdit.EditChange(Sender: TObject);
//-----------------------------------------------
// OnChange handler for Edit control.
// Prevents mis-placed negative sign in numbers.
//-----------------------------------------------
var
  idx: Integer;
  bsp: String;
begin
  if (Sender = FEdit) and (EditMask = emNumber) then
  with Sender as TEdit do
  begin
    idx := Pos('-', Text);
    if idx > 1 then
    begin
      bsp := Text;
      Delete(bsp, idx, 1);
      Text := bsp;
      MessageBeep(0);
    end;
  end;
end;

procedure TPropEdit.EditExit(Sender: TObject);
//-------------------------------------------
// OnExit handler for editor controls
//-------------------------------------------
var
  S : String;
  C: TWinControl;
begin
// Extract text from active edit control
  C := TWinControl(Sender);
  if C.Visible then
  begin
    if C = FEdit then
      S := Trim(FEdit.Text)
    else if C = FCombo then
      S := Trim(FCombo.Text)
    else S := FGrid.Cells[1,FRow];

// Validate text if it has changed
    GoValidate(S);

// Return focus to grid control
    C.Visible := False;
    FGrid.SetFocus;
  end;
end;

function TPropEdit.GoValidate(S: String): Boolean;
//------------------------------------------------
// Validates edited property value and updates
// the value if its valid. S is a string holding
// the newly edited value.
//------------------------------------------------
var
  IsValid: Boolean;
  Errmsg:  String;
begin
// Check if S is different from current value
    IsValid := True;
    if (S <> FGrid.Cells[1,FRow]) then
    try

    // Call validation procedure if it exists
      if (Assigned(FOnValidate)) then
        FOnValidate(self,FRow,S,Errmsg,IsValid);

    // If new value is valid then update the StrinGrid display
    // and the StringList that holds the property values
      if (IsValid) then
      begin
        FGrid.Cells[1,FRow] := S;
        FValues[FRow] := S;
        FModified := True;
      end

    // If not valid then raise exception
      else raise EInvalidProperty.Create('Invalid Property Value');

    except
      on E:EInvalidProperty do
      begin
        if Length(Errmsg) = 0 then Errmsg := E.Message;
        MessageDlg(Errmsg, mtError, [mbOK], 0);
      end;
    end;
    Result := IsValid;
end;

//========================================================
// StringGrid event handlers
//========================================================

procedure TPropEdit.GridClick(Sender: TObject);
//---------------------------------------------
// OnClick handler for grid control
//---------------------------------------------
begin
// Exit any active edit control
  if (FEdit.Visible) then EditExit(FEdit);
  if (FCombo.Visible) then EditExit(FCombo);

// Prevent selection of cell in column 0
  with FGrid do
  begin
    if Col = 0 then Col := 1;   { Can't select column 0 }
    FRow := Row;                { Save current row value }
  end;

// Set button status if row's edit style is Ellipsis
  if (FProps^[FRow].Style = esButton) then
    ButtonVisible := True
  else
    ButtonVisible := False;
end;

procedure TPropEdit.GridKeyPress(Sender: TObject; var Key: Char);
//------------------------------------
// OnKeyPress handler for grid
//------------------------------------
begin
  if FGrid.Col <> 1 then Exit;
  if (Key = #13) or (Key in [#43..#122]) then
  begin
    with FGrid do EditProperty(Col,Row,Key);
  end;
end;

procedure TPropEdit.GridKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
//------------------------------------
// OnKeyDown handler for grid
//------------------------------------
var
  aRow: LongInt;
begin
  aRow := FGrid.Row;
// Fire OnButtonClick event if Enter pressed
// for a row whose edit style is esButton
  if (Key = VK_RETURN) and
     (FProps^[aRow].Style = esButton) then
  try
    if (Assigned(FOnButtonClick)) then
      FOnButtonClick(self,aRow);
  except;
  end

// Let the grid process the Up & Down Arrow key press
  else if Key in [VK_UP, VK_DOWN] then exit

// Pass all other keys to the parent form
  else 
  begin
    SendMessage(Parent.Handle,WM_KEYDOWN,Key,0);
    Key := 0;
  end;

end;

procedure TPropEdit.GridMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
//--------------------------------------
// OnMouseDown handler for grid
//--------------------------------------
var
  aCol,aRow: LongInt;
  W: Integer;
  aRect, R: TRect;
begin
  with Sender as TStringGrid do
  begin
    MouseToCell(X,Y,aCol,aRow);

// If   current row's edit style is esButton,
// and  button is visible,
// and  mouse is over the button,
// then re-draw button in pressed state.
    if (aCol = 1) and (aRow = Row) and
     (FProps^[aRow].Style = esButton) and
     (ButtonVisible) then
    begin

      aRect := CellRect(aCol,aRow);
      W := aRect.Bottom - aRect.Top;
      SetRect(R, aRect.Right - W, aRect.Top,
        aRect.Right, aRect.Bottom);
      if PtInRect(R, Point(X,Y)) then
      begin
        ButtonPressed := True;
        DrawButton(Canvas, aRect);
      end;

    end;
  end;
end;

procedure TPropEdit.GridMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
//-------------------------------------
// OnMouseUp handler for grid
//-------------------------------------
var
  aCol,aRow: LongInt;
  aRect: TRect;
begin
  with Sender as TStringGrid do
  begin
    MouseToCell(X,Y,aCol,aRow);
    if (aCol = 0) then aCol := 1;
// If button is pressed and current row's
// edit style is esButton then redraw button in unpressed
// state and fire the OnButtonClick event.
    if (ButtonPressed) then
    begin
      if (FProps^[aRow].Style = esButton) then
      begin
        aRect := CellRect(1,aRow);
        ButtonPressed := not ButtonPressed;
        DrawButton(Canvas, aRect);
        try
          if (Assigned(FOnButtonClick)) then
            FOnButtonClick(self,aRow);
        except;
        end;
      end
      else
      begin
        ButtonPressed := not ButtonPressed;
      end;
    end;
  end;
  EditProperty(1,FGrid.Row,#13);
end;

procedure TPropEdit.GridTopLeftChanged(Sender: TObject);
//-----------------------------------------
// OnTopLeftChanged event for grid control
//-----------------------------------------
begin
  FEdit.Visible := False;
  FCombo.Visible := False;
  FGrid.SetFocus;
end;

procedure TPropEdit.GridDrawCell(Sender: TObject; vCol,
  vRow: Longint; Rect: TRect; State: TGridDrawState);
//---------------------------------------------------
// OnDrawCell handler for grid
//---------------------------------------------------
begin
  if (FProps = nil) then Exit;
  if vcol = 0 then Exit;
  with Sender as TStringGrid do
  begin
    with Canvas do
    begin

      if (FProps^[vRow].Style = esReadOnly) then
      begin
        Brush.Color := FReadOnlyColor;
        Font.Color := FValueColor;
      end
      else
      if (gdSelected in State) then
      begin
        Brush.Color := clWhite;
        Font.Color := clBlack;
      end
      else Font.Color := FValueColor;
      FillRect(Rect);
      SetBkMode(Handle,TRANSPARENT);
      TextOut(Rect.Left+3,Rect.Top+3,Cells[vCol,vRow]);
      if (gdSelected in State) and
         (FProps^[vRow].Style = esButton) then
        DrawButton(Canvas,Rect);
    end;
  end;
end;

procedure TPropEdit.DrawButton(aCanvas: TCanvas; aRect: TRect);
//-------------------------------------------------------
// Draws button in cell with esButton edit style
//-------------------------------------------------------
var
  Flags: Integer;
  Width, Height: Integer;
  W, H: Integer;
  R: TRect;
begin
  with aRect do
  begin
    Height := Bottom - Top;
    Width := Right - Left;
    W := Height;
    H := Height div 2;
    SetRect(R, Left + Width - W, Top+1, Left + Width-2, Top + Height-2);
  end;
  Flags := 0;
  if ButtonPressed then Flags := BF_FLAT;
  DrawEdge(aCanvas.Handle, R, EDGE_RAISED, BF_RECT or BF_MIDDLE or Flags);
  Flags := ((R.Right - R.Left) shr 1);
  PatBlt(aCanvas.Handle, R.Left+Flags, R.Top + H, 2, 2, BLACKNESS);
  PatBlt(aCanvas.Handle, R.Left+Flags-5, R.Top + H, 2, 2, BLACKNESS);
  PatBlt(aCanvas.Handle, R.Left+Flags+5, R.Top + H, 2, 2, BLACKNESS);
  ButtonVisible := True;
end;

procedure TPropEdit.EditProperty(CurCol, CurRow: LongInt; Key: Char);
//------------------------------------------------------------
// Activates the appropriate editor for the current grid cell
//------------------------------------------------------------
begin
// No editor used for following conditions
  if CurCol <> 1 then Exit;
  if CurRow <> FGrid.Row then Exit;
  if FProps^[CurRow].Style = esReadOnly then Exit;
  if FProps^[CurRow].Style = esButton then Exit;

// Activate the single line text edit control
  if (FProps^[CurRow].Style = esEdit) then
  begin

  // Determine max. length and edit mask
    FEdit.MaxLength := FProps^[CurRow].Length;
    EditMask := FProps^[CurRow].Mask;

  // Place property value in edit control
    if (Key = #13) then
      FEdit.Text := FGrid.Cells[1,CurRow]
    else
      FEdit.Text := '';

  // Activate the control
    SetEditBounds(TwinControl(FEdit));
    if (Key <> #13) then
      PostMessage(FEdit.Handle,WM_KeyDown,VkKeyScan(Key),0);
  end

// Activate the combobox edit control
  else if (FProps^[CurRow].Style in [esComboList, esComboEdit]) then
  with FCombo do
  begin
    Items.Clear;
    Items.SetText(PChar(FProps^[CurRow].List));
    if FProps^[CurRow].Style = esComboList then
    begin
      Style := csDropDownList;
      ItemIndex := Items.IndexOf(FGrid.Cells[1,CurRow]);
    end
    else
    begin
      Style := csDropDown;
      Text := FGrid.Cells[1,CurRow];
    end;
    SetEditBounds(TwinControl(FCombo));
  end;
end;

procedure TPropEdit.SetEditBounds(aControl : TWinControl);
//-----------------------------------------
// Sets bounds on active edit control
//-----------------------------------------
var
  aRect : TRect;
begin
  if aControl <> Nil then with aControl do
  begin
    aRect  := FGrid.CellRect(1,FGrid.Row);
    Left   := FGrid.Left + aRect.Left;
    Top    := FGrid.Top + aRect.Top;
    Width  := FGrid.ColWidths[1];
    Height := FGrid.DefaultRowHeight;
    if Parent.Visible then
    begin
      Visible := True;
      aControl.SetFocus;
    end;
  end;
end;

procedure TPropEdit.SetProps(var PropArray: array of TPropRecord;
  Values: TStrings);
//------------------------------------------------------------
// Updates the property editor with a new set of properties
//------------------------------------------------------------
var
  i,j: Integer;
begin
// If editing components have not been created then create them
  if not ComponentsCreated then CreateComponents;

// Initialize status of editor
  FEdit.Visible := False;
  FCombo.Visible := False;
  ButtonPressed := False;
  ButtonVisible := False;
  FModified := False;

// Assign pointers to the property array & initial values
  FProps := @PropArray;
  FValues := Values;

// Re-size the editor's grid and populate its cells
  with FGrid do
  begin
    RowCount := high(PropArray) - low(PropArray) + 1;
    ResizeGrid(self);
    for i := 0 to RowCount - 1 do
    begin
      j := low(PropArray) + i;
      Cells[0,i] := PropArray[j].Name;
    end;
    Cols[1].Assign(Values);
    Col := 1;
    if FRow >= RowCount then Frow := 0;
    Row := FRow;
  end;
end;

procedure TPropEdit.Edit;
//-------------------------------
// Activates the Property Editor.
//-------------------------------
begin
  if Parent.Visible then FGrid.SetFocus;
end;

procedure TPropEdit.GetProps(Plist, Vlist: TStringlist);
//----------------------------------------------------
// Retrieves property names & values from the Editor.
//----------------------------------------------------
begin
  Plist.Assign(FGrid.Cols[0]);  //List of property names
  Vlist.Assign(Fgrid.Cols[1]);  //List of property values
end;

end.
