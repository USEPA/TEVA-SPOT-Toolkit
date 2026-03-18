{
  Author:	Fredrik Nordbakke - FNProgramvare © 1997
  E-mail:	fredrik.nordbakke@ostfoldnett.no
  WWW:	  http://www.prodat.no/fnp/delphi.html
}

unit ComboColor;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TSortBy = (ccsUnsorted, ccsColor, ccsText);
  TComboColor = class(TCustomComboBox)
  private
    { Private declarations }
    FColorWidth: Integer;
    FSortBy: TSortBy;
    FVersion: String;
    function GetSelectedColor: TColor;
    function GetSelectedColorText: String;
    procedure SetColorWidth(Value: Integer);
    procedure SetSelectedColor(Value: TColor);
    procedure SetSelectedColorText(Value: String);
    procedure SetSortBy(Value: TSortBy);
    procedure DrawItem(Index: Integer; Rect: TRect;
                       State: TOwnerDrawState); override;
    procedure SetVersion(Value: String);
  protected
    { Protected declarations }
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    procedure AddColor(ColorText: String; Color: TColor);
    property SelectedColor:TColor read GetSelectedColor write SetSelectedColor;
    property SelectedColorText:String read GetSelectedColorText write SetSelectedColorText;
  published
    { Published declarations }
    property Color;
    property Ctl3D;
    property DragMode;
    property DragCursor;
    property DropDownCount;
    property Enabled;
    property Font;
    property ItemHeight;
    property Items;
    property MaxLength;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Text;
    property Visible;
    property OnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnDropDown;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnStartDrag;
    property ColorWidth: Integer read FColorWidth write SetColorWidth default 18;
    property SortBy: TSortBy read FSortBy write SetSortBy default ccsUnsorted;
    property Version: String read FVersion write SetVersion stored False;
  end;

procedure Register;

implementation

constructor TComboColor.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FColorWidth := 18;
  FSortBy := ccsUnsorted;
  Style := csOwnerDrawFixed;
  FVersion := '1.00.00';
end;

procedure TComboColor.AddColor(ColorText: String; Color: TColor);
var
  X: Integer;
begin
  if (FSortBy = ccsUnsorted) or (Items.Count = 0) then
    Items.AddObject(ColorText, Pointer(Color))
  else if FSortBy = ccsColor then
    begin
      for X := 0 to Items.Count - 1 do
      begin
        if TColor(Items.Objects[X]) > Color then
        begin
          Break;
        end;
      end;
      Items.InsertObject(X , ColorText, Pointer(Color));
    end
  else
    begin
      for X := 0 to Items.Count - 1 do
      begin
        if AnsiLowerCase(Items[X]) > AnsiLowerCase(ColorText) then
        begin
          Break;
        end;
      end;
      Items.InsertObject(X , ColorText, Pointer(Color));
    end;
end;

function TComboColor.GetSelectedColor: TColor;
begin
  if ItemIndex = -1 then
    Result := -1
  else
    Result := TColor(Items.Objects[ItemIndex]);
end;

function TComboColor.GetSelectedColorText: String;
begin
  if ItemIndex = -1 then
    Result := ''
  else
    Result := Items[ItemIndex];
end;

procedure TComboColor.SetColorWidth(Value: Integer);
begin
  if (FColorWidth <> Value) and (Value > 4) then
    begin
      FColorWidth := Value;
      if not (csDesigning in ComponentState) then
        Invalidate;
    end;
end;

procedure TComboColor.SetSelectedColor(Value: TColor);
var
  X: Integer;
begin
  for X := 0 to Items.Count - 1 do
  begin
    if TColor(Items.Objects[X]) = Value then
    begin
      ItemIndex := X;
      Break;
    end;
  end;
end;

procedure TComboColor.SetSelectedColorText(Value: String);
var
  X: Integer;
begin
  for X := 0 to Items.Count - 1 do
  begin
    if Items[X] = Value then
    begin
      ItemIndex := X;
      Break;
    end;
  end;
end;

procedure TComboColor.SetSortBy(Value: TSortBy);
var
  C: TColor;
  X: Integer;
  Y: Integer;
begin
  if FSortBy <> Value then
    FSortBy := Value;
    { Use a "Buble Sort". Not the fastest algorithm, but it works fine here! }
    if FSortBy <> ccsUnsorted then
    begin
      C := SelectedColor;
      X := 0;
      while X < Items.Count - 1 do
      begin
        Y := Items.Count -1;
        while Y > X do
        begin
          if FSortBy = ccsColor then
            begin
              if TColor(Items.Objects[Y]) < TColor(Items.Objects[Y - 1]) then
                Items.Exchange(Y, Y - 1);
            end
          else
            begin
              if AnsiLowerCase(Items[Y]) < AnsiLowerCase(Items[Y - 1]) then
                Items.Exchange(Y, Y - 1);
            end;
          Y := Y - 1;
        end;
        X := X + 1;
      end;
      SelectedColor := C;
    end
end;

procedure TComboColor.DrawItem(Index: Integer; Rect: TRect; State: TOwnerDrawState);
var
  ColorR: TRect;
  TextR: TRect;
  OldColor: TColor;
begin
  ColorR.Left := Rect.Left + 1;
  ColorR.Top := Rect.Top + 1;
  ColorR.Right := Rect.Left + FColorWidth - 1;
  ColorR.Bottom := Rect.Top + ItemHeight - 1;
  TextR.Left := Rect.Left + FColorWidth + 4;
  TextR.Top := Rect.Top + 1;
  TextR.Right := Rect.Right;
  TextR.Bottom := Rect.Bottom - 1;
  with Canvas do
    begin
      FillRect(Rect);	{ clear the rectangle }
      OldColor := Brush.Color;
      Brush.Color := TColor(Items.Objects[Index]);
      Rectangle(ColorR.Left, ColorR.Top, ColorR.Right, ColorR.Bottom);
      Brush.Color := OldColor;
      DrawText(Handle, PChar(Items[Index]), -1, TextR, DT_VCENTER or DT_SINGLELINE);
    end;
end;

procedure TComboColor.SetVersion(Value: String);
begin
  { This property is read only! }
end;

procedure Register;
begin
  RegisterComponents('Extensions', [TComboColor]);
end;

end.
