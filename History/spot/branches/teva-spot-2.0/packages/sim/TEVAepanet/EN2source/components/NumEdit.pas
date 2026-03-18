{
   TNUMEDIT Component
   ******************

   This is a Delphi component derived from the edit box (TEdit) component
   that restricts text entry to either a number or a string with no spaces.
   It adds only one property, named Style, to those of TEdit. The Style
   property can have one of the following values:
      esNone      -- accepts any characetr (same as a TEdit control)
      esNumber    -- restricts text entry to a numerical value
      esPosNumber -- restricts text entry to a number >= 0
      esNoSpace   -- accepts any character except a space

   Version: 1.00
   Author:  L. Rossman
   Date:    1/30/00
            12/6/01
}

unit NumEdit;
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TEditStyle = (esNone, esNumber, esPosNumber, esNoSpace);
  TNumEdit = class(TEdit)
  private
    { Private declarations }
    FStyle      : TEditStyle;
    DecimalChar : Char;
  protected
    { Protected declarations }
    procedure KeyPress(var Key: Char); override;
    procedure Change; override;
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
  published
    { Published declarations }
    property Style: TEditStyle read FStyle write Fstyle;
    property AutoSelect;
    property AutoSize;
    property BorderStyle;
    property HideSelection;
    property MaxLength;
    property Modified;
    property SelLength;
    property SelStart;
    property SelText;
    property Ctl3D;
    property Color;
    property Enabled;
    property Font;
    property Height;
    property HelpContext;
    property Hint;
    property Left;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Tag;
    property Text;
    property Top;
    property Visible;
    property Width;
    property OnChange;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDrag;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Extensions', [TNumEdit]);
end;

constructor TNumEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  DecimalChar := DecimalSeparator;
  FStyle := esNone;
end;

{*** Updated 12/6/01 ***}
procedure TNumEdit.KeyPress(var Key: Char);
var
  S: String;
begin
  if (FStyle in [esNumber, esPosNumber]) then
  begin
    if not (Key in ['0'..'9','-',DecimalChar,#8]) then Key := #0
    else
    begin
       S := Text;
       Delete(S, SelStart+1, SelLength);
       if FStyle = esPosNumber then
       begin
         if Key = '-' then Key := #0;
         if (Key = DecimalChar) and (Pos(Key,S) > 0) then Key := #0;
       end
       else
         if (Key in [DecimalChar,'-']) and (Pos(Key,S) > 0) then Key := #0;
    end;
  end
  else if (FStyle = esNoSpace) then
  begin
    if (Key = ' ') then Key := #0;
  end;
  inherited KeyPress(Key);
end;

procedure TNumEdit.Change;
var
  idx: Integer;
  bsp: String;
begin
  if (FStyle = esNumber) then
  begin
    idx := Pos('-', Text);
    if idx > 1 then
    begin
      bsp := Text;
      Delete(bsp, idx, 1);
      Text := bsp;
      MessageBeep($FFFF);
    end;
  end;
  inherited Change;
end;

end.
