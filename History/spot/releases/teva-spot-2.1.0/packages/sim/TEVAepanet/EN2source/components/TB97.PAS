unit TB97;
{
  Toolbar97 version 1.53

  by Jordan Russell
  email:      jordanr7@aol.com
  home page:  http://members.aol.com/jordanr7

  *PLEASE NOTE*  Before making any bug reports please first verify you are
                 using the latest version by checking my home page.

  You are free to use, modify, and distribute this code subject to following
  restrictions:
  - If you copy portions of my code or get ideas from my code, please give
    me credit.
  - Do not release modified versions of this source code unless it has
    significant differences and is also freeware. If you've made any changes
    that you feel should have been there in the first place, feel free to
    submit them to me so I can try to incorporate them.

  Credits:
  All of the toolbar code was written from scratch by me, except for some of
  the ideas users have submitted. I did originally get a few toolbar ideas from
  the freeware XToolBar component's source code. A lot of the TToolbarButton97
  code is based on TSpeedButton, with some ideas from Neil Booth's TIEButton
  component for C++Builder (with permission).

  Summary of changes to source code in this version:

  1.52/1.53  Improvements:
          - Added a DropdownCombo property to TToolbarButton97. See the
            documentation for details on this property.
          - Added two new properties to TDock97: ToolbarCount and Toolbars.
          - Five improvements to TToolbarButton97:
            - Now supports the old "Office 95" style through the new Flat
              property. Non-flat buttons should not be used on a TToolbar97
              since they obviously look inappropriate. But this property was
              requested by several users and is useful when you occasionally
              need some of TToolbarButton97's functionality for a button not
              on a toolbar.
            - Disabled glyphs are now generated differently. They now look
              just like Office 97 (and MFC). But for backward compatibility,
              you can still use the "old" TSpeedButton disabled look by
              setting the new OldDisabledStyle property to True.
              (Big thanks to Michael Hieke for providing me with new code)
            - Finally has support for a new fifth "mouse in" glyph, if you
              want to add an IE or Communicator look to your buttons.
            - Dropdown arrows now right when the button is disabled.
            - Now in addition to checking Application.Active,
              TToolbarButton97.MouseMove also checks if the parent form is
              active to determine if it should display the active borders.
          - Added two new properties to TToolbarSep97: SizeHorz and SizeVert.
        Fixes & minor improvements:
          - 1.53 fixes the access violations that were popping after you
            deleted a TToolbar97 in 1.52 (may not have happened in all cases).
          - Improved TToolbar97.WMMouseActivate to fix the mouse clicking
            problems on controls like edits on floating toolbars.
          - Now traps Notification in TToolbar97 so that it can clear
            DefaultDock if the dock referenced by DefaultDock is freed.
          - Now calls 'inherited' in TDock97.CMSysColorChange so that child
            controls are notified also.
          - TToolbarButton97's glyph caches are now one pixel wider and
            taller. This fixes the minor problem in previous versions (and
            in TSpeedButton too) that cuts off the right- and bottom-most
            highlight-color pixels on generated disabled glyphs.
          - The dithered Down pattern is now invalidated in
            TToolbarButton97.CMSysColorChange so it now immediately responds
            to color scheme changes.
          - A couple of very minor fixes.


  Misc. notes:
  - As with all code, you shouldn't try going through "optimizing" things
    unless you really know what you're doing. I also recommend you don't make
    any large changes to the code or you'll get left in the dark when I release
    new versions.
  - While debugging the toolbar code you might want to enable the
    'TB97DisableLock' conditional define, as described several pages down.
  - Starting with version 1.4, TToolbar97 now has GetVirtualBoundsRect,
    SetVirtualBounds, and SetVirtualBoundsRect methods. *Always* use these in
    place of getting and setting BoundsRect and using SetBounds, unless of
    course you have specific reason not to. These map the coordinates when
    NotOnScreen=True.
  - In the WM_NCPAINT handlers, GetWindowRect is used to work around a possibly
    undocumented VCL problem. The Width, Height, and BoundsRect properties are
    sometimes wrong. So it avoids any use of these properties in the WM_NCPAINT
    handlers. If anyone knows of a reason why this is happening, let me know.
  - In several places in the code you will see 'if NewStyleControls'. In case
    you don't know, NewStyleControls is a VCL variable set to True at
    application startup if the user is running Windows 95 or NT 4.0 or later.
}

{$IFNDEF WIN32} Toolbar97 no longer supports Delphi 1. Sorry! {$ENDIF}

{$ALIGN ON}
{$BOOLEVAL OFF}
{$LONGSTRINGS ON}
{$WRITEABLECONST ON}

{x$DEFINE TB97DisableLock}
{ Remove the 'x' to enable the define. It will disable calls to
  LockWindowUpdate, which it calls to disable screen updates while dragging.
  You should temporarily enable that while debugging so you are able to see
  your code window if you have something like a breakpoint that's set inside
  the dragging routines }

{ Determine Delphi/C++Builder version }
{$IFNDEF VER90}  { if it's not Delphi 2.0 }
  {$IFNDEF VER93}  { and it's not C++Builder 1.0 }
    {$DEFINE TB97Delphi3orHigher}  { then it must be Delphi 3 or higher
                                     (or a future version of C++Builder) }
  {$ENDIF}
{$ENDIF}


interface

uses
  Windows, Messages, Classes, Controls, Forms, Menus, Graphics, Buttons,
  StdCtrls, ExtCtrls;

const
  Toolbar97Version = '1.52';

type
  { TDock97 }

  TDockBoundLinesValues = (blTop, blBottom, blLeft, blRight);
  TDockBoundLines = set of TDockBoundLinesValues;
  TDockPosition = (dpTop, dpBottom, dpLeft, dpRight);
  TDockType = (dtNotDocked, dtTopBottom, dtLeftRight);

  TToolbar97 = class;

  TInsertRemoveEvent = procedure (Sender: TObject; Inserting: Boolean;
    Bar: TToolbar97) of object;

  TDock97 = class(TCustomControl)
  private
    { Property values }
    FPosition: TDockPosition;
    FAllowDrag: Boolean;
    FBoundLines: TDockBoundLines;
    FBkg, FBkgCache: TBitmap;
    FBkgTransparent: Boolean;
    FFixAlign: Boolean;
    FLimitToOneRow: Boolean;
    FOnInsertRemoveBar: TInsertRemoveEvent;
    FOnResize: TNotifyEvent;

    { Internal }
    DisableArrangeToolbars: Integer; { Increment to disable ArrangeToolbars }
    DockList: TList;  { List of the visible toolbars docked. Items are casted in TToolbar97's.
                        But, at design time, all docked toolbars are here regardless of visibility }
    RowInfo: TList;   { List of info on each row. Items are pointers to TRowInfo's }

    { Property access methods }
    procedure SetAllowDrag (Value: Boolean);
    procedure SetBackground (Value: TBitmap);
    procedure SetBackgroundTransparent (Value: Boolean);
    procedure SetBoundLines (Value: TDockBoundLines);
    procedure SetFixAlign (Value: Boolean);
    procedure SetPosition (Value: TDockPosition);

    function GetToolbarCount: Integer;
    function GetToolbars (Index: Integer): TToolbar97;

    { Internal }
    procedure FreeRowInfo;
    function GetRowOf (const Y: Integer; var Before: Boolean): Integer;
    function GetDesignModeRowOf (const Y: Integer): Integer;
    function GetHighestRow: Integer;
    procedure RemoveBlankRows;
    procedure InsertRowBefore (const BeforeRow: Integer);
    procedure BuildRowInfo;
    procedure ChangeDockList (const Insert: Boolean; const Bar: TToolbar97;
      const IsVisible: Boolean);
    procedure ChangeWidthHeight (const IsClientWidthAndHeight: Boolean;
      NewWidth, NewHeight: Integer);
    procedure ArrangeToolbars;
    procedure DrawBackground (const Canvas: TCanvas;
      const ClippingRect, DrawRect: TRect);
    procedure InvalidateBackgrounds;
    procedure BackgroundChanged (Sender: TObject);

    { Messages }
    procedure CMSysColorChange (var Message: TMessage); message CM_SYSCOLORCHANGE;
    procedure WMMove (var Message: TWMMove); message WM_MOVE;
    procedure WMSize (var Message: TWMSize); message WM_SIZE;
    procedure WMNCCalcSize (var Message: TWMNCCalcSize); message WM_NCCALCSIZE;
    procedure WMNCPaint (var Message: TMessage); message WM_NCPAINT;
  protected
    procedure AlignControls (AControl: TControl; var Rect: TRect); override;
    function  GetPalette: HPALETTE; override;
    procedure Loaded; override;
    procedure SetParent (AParent: TWinControl); override;
    procedure Paint; override;
    procedure VisibleChanging; override;
  public
    constructor Create (AOwner: TComponent); override;
    procedure CreateParams (var Params: TCreateParams); override;
    destructor Destroy; override;
    property ToolbarCount: Integer read GetToolbarCount;
    property Toolbars[Index: Integer]: TToolbar97 read GetToolbars;
  published
    property AllowDrag: Boolean read FAllowDrag write SetAllowDrag default True;
    property Background: TBitmap read FBkg write SetBackground;
    property BackgroundTransparent: Boolean read FBkgTransparent write SetBackgroundTransparent default False;
    property BoundLines: TDockBoundLines read FBoundLines write SetBoundLines default [];
    property Color default clBtnFace;
    property FixAlign: Boolean read FFixAlign write SetFixAlign default False;
    property LimitToOneRow: Boolean read FLimitToOneRow write FLimitToOneRow default False;
    property PopupMenu;
    property Position: TDockPosition read FPosition write SetPosition default dpTop;

    property OnInsertRemoveBar: TInsertRemoveEvent read FOnInsertRemoveBar write FOnInsertRemoveBar;
    property OnResize: TNotifyEvent read FOnResize write FOnResize;
  end;

  { TToolbar97 }

  TToolbar97 = class(TCustomControl)
  private
    { Property variables }
    FBarHeight, FBarWidth, FDockedTotalBarHeight, FDockedTotalBarWidth, FDockPos, FDockRow: Integer;
    FDefaultDock: TDock97;
    FOnRecreating, FOnRecreated, FOnDockChanging, FOnDockChanged, FOnClose: TNotifyEvent;
    FCanDockLeftRight, FCloseButton: Boolean;
    FFloatingRect: TRect;
    FFloatingRightX: Integer;

    { Lists }
    SlaveInfo,               { List of slave controls. Items are pointers to TSlaveInfo's }
    GroupInfo,               { List of the control "groups". List items are pointers to TGroupInfo's }
    LineSeps,                { List of the Y locations of line separators. Items are casted in TLineSep's }
    OrderList: TList;        { List of the child controls, arranged using the current "OrderIndex" values }

    { Misc. }
    UpdatingBounds,          { Increment while internally changing the bounds. This allows it to move the toolbar freely }
    DisableArrangeControls,  { Increment to disable ArrangeControls }
    Hidden: Integer;         { Incremented while the toolbar is temporarily hidden }

    { When floating. These are not used (and FloatParent isn't created) in design mode }
    FloatParent: TWinControl; { The actual Parent of the toolbar when it is floating, }
    MDIParentForm: TForm;     { Either the owner form, or the MDI parent if the owner form is an MDI child form }
    NotOnScreen: Boolean;     { True if the toolbar is currently off the screen, hidden from view.
                                This is True when the toolbar is hidden when application is deactivated }
    VirtualLeft: Integer;     { The Left value the toolbar should be restored to when moving from off the screen }
    CloseButtonDown: Boolean; { True if Close button is currently depressed }

    OldFormWindowProc, OldChildFormWindowProc: Pointer; { The previous form window procedures }

    { Property access methods }
    procedure SetCloseButton (Value: Boolean);
    procedure SetDefaultDock (Value: TDock97);
    function  GetDockedTo: TDock97;
    procedure SetDockedTo (Value: TDock97);
    procedure SetDockPos (Value: Integer);
    procedure SetDockRow (Value: Integer);
    function  GetOrderIndex (Control: TControl): Integer;
    procedure SetOrderIndex (Control: TControl; Value: Integer);

    { Internal }
    procedure FreeGroupInfo (const List: TList);
    procedure BuildGroupInfo (const List: TList; const TranslateSlave: Boolean;
      const OldDockType, NewDockType: TDockType);
    procedure MoveOnScreen (const OnlyIfFullyOffscreen: Boolean);
    procedure ShouldBeVisible (const Control: TControl; const DockType: TDockType;
      const SetIt: Boolean; var AVisible: Boolean);
    procedure AutoArrangeControls;
    procedure ArrangeControls (const CanMove, CanResize: Boolean;
      const OldDockType: TDockType; const DockingTo: TDock97; RightX: Integer;
      const NewClientSize: PPoint);
    procedure DrawDraggingOutline (const DC: HDC; const NewRect, OldRect: PRect;
      const NewDocking, OldDocking: Boolean);
    procedure NewFormWindowProc (var Message: TMessage);
    procedure NewChildFormWindowProc (var Message: TMessage);
    function  NewMainWindowHook (var Message: TMessage): Boolean;
    procedure BeginMoving (const InitX, InitY: Integer);
    procedure BeginSizing (const HitTestValue: Integer);
    procedure DrawNCArea (const Clip: HRGN; const RedrawBorder, RedrawCaption, RedrawCloseButton: Boolean);
    procedure SetNotOnScreen (const Value: Boolean);

    { Messages }
    procedure CMTextChanged (var Message: TMessage); message CM_TEXTCHANGED;
    procedure CMVisibleChanged (var Message: TMessage); message CM_VISIBLECHANGED;
    procedure CMControlListChange (var Message: TCMControlListChange); message CM_CONTROLLISTCHANGE;
    procedure WMMove (var Message: TWMMove); message WM_MOVE;
    procedure WMActivate (var Message: TWMActivate); message WM_ACTIVATE;
    procedure WMMouseActivate (var Message: TWMMouseActivate); message WM_MOUSEACTIVATE;
    procedure WMGetMinMaxInfo (var Message: TWMGetMinMaxInfo); message WM_GETMINMAXINFO;
    procedure WMNCHitTest (var Message: TWMNCHitTest); message WM_NCHITTEST;
    procedure WMNCLButtonDown (var Message: TWMNCLButtonDown); message WM_NCLBUTTONDOWN;
    procedure WMNCPaint (var Message: TMessage); message WM_NCPAINT;
    procedure WMNCCalcSize (var Message: TWMNCCalcSize); message WM_NCCALCSIZE;
  protected
    { Internal }
    function  GetVirtualBoundsRect: TRect;
    procedure SetVirtualBounds (ALeft, ATop, AWidth, AHeight: Integer);
    procedure SetVirtualBoundsRect (const R: TRect);

    { Overridden methods }
    procedure AlignControls (AControl: TControl; var Rect: TRect); override;
    procedure CreateParams (var Params: TCreateParams); override;
    procedure Loaded; override;
    procedure MouseDown (Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure Notification (AComponent: TComponent; Operation: TOperation); override;
    procedure Paint; override;
    procedure SetBounds (ALeft, ATop, AWidth, AHeight: Integer); override;
    procedure SetParent (AParent: TWinControl); override;
  public
    { Public declarations }
    property FloatingRect: TRect read FFloatingRect write FFloatingRect;
    property OrderIndex[Control: TControl]: Integer read GetOrderIndex write SetOrderIndex;
    constructor Create (AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SetSlaveControl (const ATopBottom, ALeftRight: TControl);
  published
    { Published declarations }
    property CanDockLeftRight: Boolean read FCanDockLeftRight write FCanDockLeftRight default True;
    property Caption;
    property Color default clBtnFace;
    property CloseButton: Boolean read FCloseButton write SetCloseButton default True;
    property DefaultDock: TDock97 read FDefaultDock write SetDefaultDock;
    property DockedTo: TDock97 read GetDockedTo write SetDockedTo;
    property DockRow: Integer read FDockRow write SetDockRow default 0;
    property DockPos: Integer read FDockPos write SetDockPos default -1;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Visible;

    property OnClose: TNotifyEvent read FOnClose write FOnClose;
    property OnRecreated: TNotifyEvent read FOnRecreated write FOnRecreated;
    property OnRecreating: TNotifyEvent read FOnRecreating write FOnRecreating;
    property OnDockChanged: TNotifyEvent read FOnDockChanged write FOnDockChanged;
    property OnDockChanging: TNotifyEvent read FOnDockChanging write FOnDockChanging;
  end;

  { TToolbarSep97 }

  TToolbarSepSize = 1..MaxInt;

  TToolbarSep97 = class(TGraphicControl)
  private
    FBlank: Boolean;
    FSizeHorz, FSizeVert: TToolbarSepSize;
    procedure SetBlank (Value: Boolean);
    procedure SetSizeHorz (Value: TToolbarSepSize);
    procedure SetSizeVert (Value: TToolbarSepSize);
  protected
    procedure MouseDown (Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure Paint; override;
    procedure SetParent (AParent: TWinControl); override;
  public
    constructor Create (AOwner: TComponent); override;
  published
    property Blank: Boolean read FBlank write SetBlank default False;
    property SizeHorz: TToolbarSepSize read FSizeHorz write SetSizeHorz default 6;
    property SizeVert: TToolbarSepSize read FSizeVert write SetSizeVert default 6;
  end;

  { TToolbarButton97 }

  TButtonDisplayMode = (dmBoth, dmGlyphOnly, dmTextOnly);
  TButtonState97 = (bsUp, bsDisabled, bsDown, bsExclusive, bsMouseIn);
  TNumGlyphs97 = 1..5;

  TToolbarButton97 = class(TGraphicControl)
  private
    FAllowAllUp: Boolean;
    FDisplayMode: TButtonDisplayMode;
    FDown: Boolean;
    FDropdownArrow: Boolean;
    FDropdownCombo: Boolean;
    FDropdownMenu: TPopupMenu;
    FFlat: Boolean;
    FGlyph: Pointer;
    FGroupIndex: Integer;
    FLayout: TButtonLayout;
    FMargin: Integer;
    FOldDisabledStyle: Boolean;
    FOpaque: Boolean;
    FSpacing: Integer;
    FWordWrap: Boolean;
    FOnMouseEnter, FOnMouseExit: TNotifyEvent;
    { Internal }
    FInClick: Boolean;
    FMouseInControl: Boolean;
    FMouseIsDown: Boolean;
    FMenuIsDown: Boolean;
    procedure GlyphChanged(Sender: TObject);
    procedure UpdateExclusive;
    procedure SetAllowAllUp (Value: Boolean);
    procedure SetDown (Value: Boolean);
    procedure SetDisplayMode (Value: TButtonDisplayMode);
    procedure SetDropdownArrow (Value: Boolean);
    procedure SetDropdownCombo (Value: Boolean);
    procedure SetDropdownMenu (Value: TPopupMenu);
    procedure SetFlat (Value: Boolean);
    function GetGlyph: TBitmap;
    procedure SetGlyph (Value: TBitmap);
    procedure SetGroupIndex (Value: Integer);
    procedure SetLayout (Value: TButtonLayout);
    procedure SetMargin (Value: Integer);
    function GetNumGlyphs: TNumGlyphs97;
    procedure SetNumGlyphs (Value: TNumGlyphs97);
    procedure SetOldDisabledStyle (Value: Boolean);
    procedure SetOpaque (Value: Boolean);
    procedure SetSpacing (Value: Integer);
    procedure SetWordWrap (Value: Boolean);
    procedure UpdateTracking;
    procedure Redraw (const Erase: Boolean);
    procedure MouseEntered;
    procedure MouseLeft;
    procedure ButtonMouseTimerHandler (Sender: TObject);
    class function DeactivateHook (var Message: TMessage): Boolean;
    procedure WMLButtonDblClk (var Message: TWMLButtonDblClk); message WM_LBUTTONDBLCLK;
    procedure CMEnabledChanged (var Message: TMessage); message CM_ENABLEDCHANGED;
    procedure CMButtonPressed (var Message: TMessage); message CM_BUTTONPRESSED;
    procedure CMDialogChar (var Message: TCMDialogChar); message CM_DIALOGCHAR;
    procedure CMFontChanged (var Message: TMessage); message CM_FONTCHANGED;
    procedure CMTextChanged (var Message: TMessage); message CM_TEXTCHANGED;
    procedure CMSysColorChange (var Message: TMessage); message CM_SYSCOLORCHANGE;
    procedure CMMouseLeave (var Message: TMessage); message CM_MOUSELEAVE;
  protected
    FState: TButtonState97;
    function GetPalette: HPALETTE; override;
    procedure Loaded; override;
    procedure Notification (AComponent: TComponent; Operation: TOperation); override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
    procedure Paint; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Click; override;
  published
    property AllowAllUp: Boolean read FAllowAllUp write SetAllowAllUp default False;
    property GroupIndex: Integer read FGroupIndex write SetGroupIndex default 0;
    property DisplayMode: TButtonDisplayMode read FDisplayMode write SetDisplayMode default dmBoth;
    property Down: Boolean read FDown write SetDown default False;
    property DropdownArrow: Boolean read FDropdownArrow write SetDropdownArrow default True;
    property DropdownCombo: Boolean read FDropdownCombo write SetDropdownCombo default False;
    property DropdownMenu: TPopupMenu read FDropdownMenu write SetDropdownMenu;
    property Caption;
    property Enabled;
    property Flat: Boolean read FFlat write SetFlat default True;
    property Font;
    property Glyph: TBitmap read GetGlyph write SetGlyph;
    property Layout: TButtonLayout read FLayout write SetLayout default blGlyphLeft;
    property Margin: Integer read FMargin write SetMargin default -1;
    property NumGlyphs: TNumGlyphs97 read GetNumGlyphs write SetNumGlyphs default 1;
    property OldDisabledStyle: Boolean read FOldDisabledStyle write SetOldDisabledStyle default False;
    property Opaque: Boolean read FOpaque write SetOpaque default True;
    property ParentFont;
    property ParentShowHint;
    property ShowHint;
    property Spacing: Integer read FSpacing write SetSpacing default 4;
    property Visible;
    property WordWrap: Boolean read FWordWrap write SetWordWrap default False;

    property OnClick;
    property OnDblClick;
    property OnMouseDown;
    property OnMouseEnter: TNotifyEvent read FOnMouseEnter write FOnMouseEnter;
    property OnMouseExit: TNotifyEvent read FOnMouseExit write FOnMouseExit;
    property OnMouseMove;
    property OnMouseUp;
  end;

  { TEdit97 }

  TEdit97 = class(TCustomEdit)
  private
    MouseInControl: Boolean;
    procedure RedrawBorder (const Clip: HRGN);
    procedure NewAdjustHeight;
    procedure CMEnabledChanged (var Message: TMessage); message CM_ENABLEDCHANGED;
    procedure CMFontChanged (var Message: TMessage); message CM_FONTCHANGED;
    procedure CMMouseEnter (var Message: TMessage); message CM_MOUSEENTER;
    procedure CMMouseLeave (var Message: TMessage); message CM_MOUSELEAVE;
    procedure WMSetFocus (var Message: TWMSetFocus); message WM_SETFOCUS;
    procedure WMKillFocus (var Message: TWMKillFocus); message WM_KILLFOCUS;
    procedure WMNCCalcSize (var Message: TWMNCCalcSize); message WM_NCCALCSIZE;
    procedure WMNCPaint (var Message: TMessage); message WM_NCPAINT;
  protected
    procedure Loaded; override;
  public
    constructor Create (AOwner: TComponent); override;
  published
    property CharCase;
    property DragCursor;
    property DragMode;
    property Enabled;
    property Font;
    property HideSelection;
    {$IFDEF TB97Delphi3orHigher}
    property ImeMode;
    property ImeName;
    {$ENDIF}
    property MaxLength;
    property OEMConvert;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PasswordChar;
    property PopupMenu;
    property ReadOnly;
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

procedure RegLoadToolbarPositions (const Form: TForm; const BaseRegistryKey: String);
procedure RegSaveToolbarPositions (const Form: TForm; const BaseRegistryKey: String);
procedure IniLoadToolbarPositions (const Form: TForm; const Filename: String);
procedure IniSaveToolbarPositions (const Form: TForm; const Filename: String);

type
  TPositionReadIntProc = function(const ToolbarName, Value: String; const Default: Longint;
    const ExtraData: Pointer): Longint;
  TPositionReadStringProc = function(const ToolbarName, Value, Default: String;
    const ExtraData: Pointer): String;
  TPositionWriteIntProc = procedure(const ToolbarName, Value: String; const Data: Longint;
    const ExtraData: Pointer);
  TPositionWriteStringProc = procedure(const ToolbarName, Value, Data: String;
    const ExtraData: Pointer);
procedure CustomLoadToolbarPositions (const Form: TForm;
  const ReadIntProc: TPositionReadIntProc;
  const ReadStringProc: TPositionReadStringProc; const ExtraData: Pointer);
procedure CustomSaveToolbarPositions (const Form: TForm;
  const WriteIntProc: TPositionWriteIntProc;
  const WriteStringProc: TPositionWriteStringProc; const ExtraData: Pointer);

procedure Register;

implementation

uses
  CommCtrl, Registry, IniFiles, SysUtils, Consts;

const
  { Exception messages }
  STB97DockNotFormOwner = 'TDock97 must be owned by a form';
  STB97DockParentNotAllowed = 'A TDock97 control cannot be placed inside a TToolbar97 or another TDock97';
  STB97DockCannotHide = 'Cannot hide a TDock97';
  STB97DockCannotChangePosition = 'Cannot change Position of a TDock97 if it already contains controls';
  STB97ToolbarNotFormOwner = 'TToolbar97 must be owned by a form';
  STB97ToolbarNameNotSet = 'Name property not set';
  STB97ToolbarDockToNameNotSet = 'DockTo''s Name property not set';
  STB97ToolbarParentNotAllowed = 'TToolbar97 can only be placed on a TDock97 or directly on the form';
  STB97ToolbarControlNotChildOfToolbar = 'Control is not a child of the toolbar';
  STB97SepParentNotAllowed = 'TToolbarSep97 can only be placed on a TToolbar97';

  { All spacing & margin values are here. It's recommended that you don't
    try changing any of this! }
  LineSpacing = 6;
  DropdownComboWidth = 11;
  { TopMargin is really a left margin when docked to left or right side.
    Likewise, LeftMargin is really a top margin. }
  TopMargin: array[TDockType] of Integer = (2, 2, 2);
  BottomMargin: array[TDockType] of Integer = (1, 2, 2);
  LeftMargin: array[Boolean, TDockType] of Integer = ((4, 2, 2), (4, 11, 11));
  RightMargin: array[TDockType] of Integer = (4, 2, 2);

  DefaultBarWidthHeight = 8;

  ForceDockAtTopRow = 0;
  ForceDockAtLeftPos = -8;

  PositionLeftOrRight = [dpLeft, dpRight];

  { Names of registry values }
  rvVisible = 'Visible';
  rvDockedTo = 'DockedTo';
  rvDockRow = 'DockRow';
  rvDockPos = 'DockPos';
  rvFloatLeft = 'FloatLeft';
  rvFloatTop = 'FloatTop';
  rvFloatRight = 'FloatRight';
  rvFloatBottom = 'FloatBottom';
  rvFloatRightX = 'FloatRightX';

type
  { Used in GroupInfo lists }
  PGroupInfo = ^TGroupInfo;
  TGroupInfo = record
    GroupWidth,           { Width in pixels of the group, if all controls were
                            lined up left-to-right }
    GroupHeight: Integer; { Heights in pixels of the group, if all controls were
                            lined up top-to-bottom }
    Members: TList;
  end;

  { Used in SlaveInfo lists }
  PSlaveInfo = ^TSlaveInfo;
  TSlaveInfo = record
    LeftRight,
    TopBottom: TControl;
  end;

  { Used in RowInfo lists }
  PRowInfo = ^TRowInfo;
  TRowInfo = record
    BarHeight, BarWidth, DockedTotalBarHeight, DockedTotalBarWidth: Integer;
  end;

  { Used in LineSeps lists }
  TLineSep = record
    Y: SmallInt;
    Blank: Boolean;
    Unused: Boolean;
  end;

  { Use by CompareControls }
  PCompareExtra = ^TCompareExtra;
  TCompareExtra = record
    Toolbar: TToolbar97;
    ComparePositions: Boolean;
    CurDockType: TDockType;
  end;

  TFloatParent = class(TWinControl)
  protected
    procedure CreateParams (var Params: TCreateParams); override;
  end;

function InstallNewWindowProc (const AID: Integer; const AForm: TForm;
  const NewProc: TWndMethod; const NewHook: TWindowHook): Pointer; forward;
procedure UninstallNewWindowProc (const AID: Integer; const AForm: TForm); forward;
type
  PUsedFormInfo = ^TUsedFormInfo;
  TUsedFormInfo = record
    ID: Integer;
    Form: TForm;
    Old, New: Pointer;
    Hook: TWindowHook;
    RefCount: Integer;
  end;

var
  UsedForms: TList;

  { See TToolbarButton97.ButtonMouseTimerHandler for info on these }
  ButtonMouseTimer: TTimer = nil;
  ButtonMouseInControl: TToolbarButton97 = nil;

procedure Register;
begin
  RegisterComponents ('Toolbar97',
    [TToolbar97, TDock97, TToolbarButton97, TToolbarSep97, TEdit97]);
end;


{ Misc. functions }

function GetCaptionHeight: Integer;
{ Returns height of the caption of a small window }
begin
  if NewStyleControls then
    Result := GetSystemMetrics(SM_CYSMCAPTION)
  else
    { Win 3.x doesn't support small captions, so, like Office 97, use the size
      of normal captions minus one }
    Result := GetSystemMetrics(SM_CYCAPTION)-1;
end;
function GetBorderSize: Integer;
{ Returns width of a thick border. Note that, depending on the Windows version,
  this may not be the same as the actual window metrics since it draws its
  own border }
begin
  Result := GetSystemMetrics(SM_CXFRAME);
end;

procedure AddNCAreaToRect (var R: TRect);
begin
  Dec (R.Left, GetBorderSize);
  Inc (R.Right, GetBorderSize);
  Inc (R.Bottom, GetCaptionHeight + GetBorderSize*2);
end;
procedure RemoveNCAreaFromRect (var R: TRect);
begin
  Inc (R.Left, GetBorderSize);
  Dec (R.Right, GetBorderSize);
  Dec (R.Bottom, GetCaptionHeight + GetBorderSize*2);
end;

(* not currently used
function GetDragFullWindows: Boolean;
var
  S: BOOL;
begin
  Result := False;
  if SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, @S, 0) then
    Result := S;
end;
*)

function GetDesktopArea: TRect;
{ Returns a rectangle of the screen. But, under Win95 and NT 4.0, it excludes
  the area taken up by the taskbar. }
begin
  if not SystemParametersInfo(SPI_GETWORKAREA, 0, @Result, 0) then
    { SPI_GETWORKAREA is only supported by Win95 and NT 4.0. So it fails under
      Win 3.x. In that case, return a rectangle of the entire screen }
    Result := Rect(0, 0, GetSystemMetrics(SM_CXSCREEN),
      GetSystemMetrics(SM_CYSCREEN));
end;

function GetParentToolbar97 (Control: TControl): TToolbar97;
{ Returns the parent toolbar (direct or indirect) of the control, or nil if it
  is not a child of a TToolbar97 }
begin
  Result := nil;
  while Control <> nil do begin
    if Control is TToolbar97 then begin
      Result := TToolbar97(Control);
      Break;
    end;
    Control := Control.Parent;
  end;
end;

function ControlIsChildOf (Control, IsChildOf: TControl): Boolean;
{ Returns True if Control is directly or indirectly a child of IsChildOf }
begin
  Result := False;
  while Control <> nil do begin
    if Control = IsChildOf then begin
      Result := True;
      Break;
    end;
    Control := Control.Parent;
  end;
end;

function InstallNewWindowProc (const AID: Integer; const AForm: TForm;
  const NewProc: TWndMethod; const NewHook: TWindowHook): Pointer;
{ Installs a new window procedure on the specified form that overrides the
  existing one. Also, if NewHook <> nil, it adds a main window hook.
  It returns the address of old window procedure, which the new window
  procedure should call. }
var
  I: Integer;
  Info: PUsedFormInfo;
begin
  Result := nil;
  for I := 0 to UsedForms.Count-1 do
    with PUsedFormInfo(UsedForms[I])^ do
      { If AForm already exists in list with the same ID, only increment
        the reference count }
      if (ID = AID) and (Form = AForm) then begin
        Inc (RefCount);
        Exit;
      end;
  New (Info);
  try
    with Info^ do begin
      ID := AID;
      Form := AForm;
      New := MakeObjectInstance(NewProc);
      Old := Pointer(SetWindowLong(AForm.Handle, GWL_WNDPROC, LongInt(New)));
      Hook := NewHook;
      if Assigned(Hook) then
        Application.HookMainWindow (Hook);
      RefCount := 1;
      Result := Old;
    end;
    UsedForms.Add (Info);
  except
    Dispose (Info);
    raise;
  end;
end;

procedure UninstallNewWindowProc (const AID: Integer; const AForm: TForm);
{ Removes the new window procedure installing using InstallNewWindowProc from
  the specified form. }
var
  I: Integer;
begin
  for I := UsedForms.Count-1 downto 0 do
    with PUsedFormInfo(UsedForms[I])^ do
      if (ID = AID) and (Form = AForm) then begin
        Dec (RefCount);
        if RefCount = 0 then begin
          if Form.HandleAllocated then
            SetWindowLong (Form.Handle, GWL_WNDPROC, LongInt(Old));
          FreeObjectInstance (New);
          if Assigned(Hook) then
            Application.UnhookMainWindow (Hook);
          Dispose (PUsedFormInfo(UsedForms[I]));
          UsedForms.Delete (I);
        end;
      end;
end;

function GetMDIParent (const Form: TForm): TForm;
{ Returns the parent of the specified MDI child form. But, if Form isn't a
  MDI child, it simply returns Form. }
var
  I, J: Integer;
begin
  Result := Form;
  if Form.FormStyle = fsMDIChild then
    for I := 0 to Screen.FormCount-1 do
      with Screen.Forms[I] do begin
        if FormStyle <> fsMDIForm then Continue;
        for J := 0 to MDIChildCount-1 do
          if MDIChildren[J] = Form then begin
            Result := Screen.Forms[I];
            Exit;
          end;
      end;
end;

function GetActiveForm: {$IFDEF TB97Delphi3orHigher} TCustomForm {$ELSE} TForm {$ENDIF};
{ Returns the active top-level form }
var
  Ctl: TWinControl;
begin
  Result := nil;
  Ctl := FindControl(GetActiveWindow);
  if Assigned(Ctl) then begin
    Result := GetParentForm(Ctl);
    if Result is TForm then
      Result := GetMDIParent(TForm(Result));
  end;
end;

procedure ShowHideFloatParents (const Form: TForm; const AppActive: Boolean);
var
  HideFloatingToolbars: Boolean;
  I: Integer;
begin
  { First call ShowHideFloatParent on child forms }
  for I := 0 to Form.MDIChildCount-1 do
    ShowHideFloatParents (Form.MDIChildren[I], AppActive);

  { Hide any child toolbars if: the application is not active or is
    minimized, or the form is not visible or is minimized }
  HideFloatingToolbars := not AppActive or IsIconic(Application.Handle) or
    not IsWindowVisible(Form.Handle) or IsIconic(Form.Handle);
  for I := 0 to Form.ComponentCount-1 do
    if Form.Components[I] is TToolbar97 then
      with TToolbar97(Form.Components[I]) do
        SetNotOnScreen ((DockedTo = nil) and HideFloatingToolbars);
end;

function GetDockTypeOf (const Control: TDock97): TDockType;
begin
  if Control = nil then
    Result := dtNotDocked
  else begin
    if not(Control.Position in PositionLeftOrRight) then
      Result := dtTopBottom
    else
      Result := dtLeftRight;
  end;
end;

type
  TListSortExCompare = function (const Item1, Item2, ExtraData: Pointer): Integer;
procedure ListSortEx (const List: TList; const Compare: TListSortExCompare;
  const ExtraData: Pointer);
{ Similar to TList.Sort, but lets you pass a user-defined ExtraData pointer }
  procedure QuickSortEx (L: Integer; const R: Integer);
  var
    I, J: Integer;
    P: Pointer;
  begin
    repeat
      I := L;
      J := R;
      P := List[(L + R) shr 1];
      repeat
        while Compare(List[I], P, ExtraData) < 0 do Inc(I);
        while Compare(List[J], P, ExtraData) > 0 do Dec(J);
        if I <= J then
        begin
          List.Exchange (I, J);
          Inc (I);
          Dec (J);
        end;
      until I > J;
      if L < J then QuickSortEx (L, J);
      L := I;
    until I >= R;
  end;
begin
  if List.Count > 1 then
    QuickSortEx (0, List.Count-1);
end;

procedure ProcessPaintMessages;
{ Dispatches all pending WM_PAINT messages. In effect, this is like an
  'UpdateWindow' on all visible windows }
var
  Msg: TMsg;
begin
  while PeekMessage(Msg, 0, WM_PAINT, WM_PAINT, PM_NOREMOVE) do begin
    case Integer(GetMessage(Msg, 0, WM_PAINT, WM_PAINT)) of
      -1: Break; { if GetMessage failed }
      0: begin
           { Repost WM_QUIT messages }
           PostQuitMessage (Msg.WParam);
           Break;
         end;
    end;
    DispatchMessage (Msg);
  end;
end;


{ TDock97 - internal }

constructor TDock97.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);

  if not(AOwner is TForm) then
    raise EInvalidOperation.Create(STB97DockNotFormOwner);
    { because TToolbar97 depends on docks being in the form's component list } 

  FAllowDrag := True;
  DockList := TList.Create;
  RowInfo := TList.Create;

  Inc (DisableArrangeToolbars);
  try
    ControlStyle := ControlStyle +
      [csAcceptsControls, csNoStdEvents] -
      [csClickEvents, csCaptureMouse, csOpaque];
    FBkg := TBitmap.Create;
    FBkg.OnChange := BackgroundChanged;
    Position := dpTop;
    Color := clBtnFace;
  finally
    Dec (DisableArrangeToolbars);
  end;
  { Rearranging was disabled, so manually rearrange it now }
  ArrangeToolbars;
end;

procedure TDock97.CreateParams (var Params: TCreateParams);
begin
  inherited CreateParams (Params);
  { Disable complete redraws when size changes. CS_H/VREDRAW cause flicker
    and are not necessary for this control at run time }
  if not(csDesigning in ComponentState) then
    with Params.WindowClass do
      Style := Style and not(CS_HREDRAW or CS_VREDRAW);
end;

destructor TDock97.Destroy;
begin
  if Assigned(FBkgCache) then
    FBkgCache.Free;
  if Assigned(FBkg) then
    FBkg.Free;
  FreeRowInfo;
  if Assigned(RowInfo) then
    RowInfo.Free;
  if Assigned(DockList) then
    DockList.Free;
  inherited Destroy;
end;

procedure TDock97.SetParent (AParent: TWinControl);
begin
  if (AParent is TToolbar97) or (AParent is TDock97) then
    raise EInvalidOperation.Create(STB97DockParentNotAllowed);

  inherited SetParent (AParent);
end;

procedure TDock97.VisibleChanging;
begin
  if Visible then
    raise EInvalidOperation.Create(STB97DockCannotHide);
  inherited VisibleChanging;
end;

procedure TDock97.FreeRowInfo;
var
  I: Integer;
begin
  if RowInfo = nil then Exit;
  for I := RowInfo.Count-1 downto 0 do begin
    FreeMem (RowInfo.Items[I]);
    RowInfo.Delete (I);
  end;
end;

procedure TDock97.BuildRowInfo;
var
  R, I: Integer;
  HighestBarHeight, HighestBarWidth: Integer;
  NewRowInfo: PRowInfo;
begin
  FreeRowInfo;
  for R := 0 to GetHighestRow do begin
    HighestBarHeight := DefaultBarWidthHeight;
    HighestBarWidth := DefaultBarWidthHeight;
    for I := 0 to DockList.Count-1 do begin
      with TToolbar97(DockList[I]) do begin
        if FDockRow <> R then Continue;
        if FBarHeight > HighestBarHeight then
          HighestBarHeight := FBarHeight;
        if FBarWidth > HighestBarWidth then
          HighestBarWidth := FBarWidth;
      end;
    end;
    GetMem (NewRowInfo, SizeOf(TRowInfo));
    try
      with NewRowInfo^ do begin
        BarHeight := HighestBarHeight;
        BarWidth := HighestBarWidth;
        DockedTotalBarHeight := TopMargin[dtTopBottom] + HighestBarHeight + BottomMargin[dtTopBottom];
        DockedTotalBarWidth := TopMargin[dtLeftRight] + HighestBarWidth + BottomMargin[dtLeftRight];
      end;
      RowInfo.Add (NewRowInfo);
    except
      FreeMem (NewRowInfo);
      raise;
    end;
  end;
end;

function GetRowInfo (const Row: Integer; const Dock: TDock97;
  const DefaultToolbar: TToolbar97): TRowInfo;
begin
  if Row < Dock.RowInfo.Count then
    Result := PRowInfo(Dock.RowInfo[Row])^
  else begin
    { If it's out of bounds }
    if DefaultToolbar = nil then
      FillChar (Result, SizeOf(Result), 0)
    else
    with Result do begin
      BarHeight := DefaultToolbar.FBarHeight;
      BarWidth := DefaultToolbar.FBarWidth;
      DockedTotalBarHeight := DefaultToolbar.FDockedTotalBarHeight;
      DockedTotalBarWidth := DefaultToolbar.FDockedTotalBarWidth;
    end;
  end;
end;

function TDock97.GetRowOf (const Y: Integer; var Before: Boolean): Integer;
{ Returns row number of the specified Y. Before is set to True if it was
  close to being in between two rows. }
var
  HighestRow, R, CurY, NextY: Integer;
  CurRowInfo: TRowInfo;
begin
  Result := 0;  Before := False;
  HighestRow := GetHighestRow;
  CurY := 0;
  for R := 0 to HighestRow+1 do begin
    if R <= HighestRow then begin
      CurRowInfo := GetRowInfo(R, Self, nil);
      if not(Position in PositionLeftOrRight) then
        NextY := CurY + CurRowInfo.DockedTotalBarHeight
      else
        NextY := CurY + CurRowInfo.DockedTotalBarWidth;
    end
    else
      NextY := High(NextY);
    if Y <= CurY+5 then begin
      Result := R;
      Before := True;
      Break;
    end;
    if (Y >= CurY+5) and (Y <= NextY-5) then begin
      Result := R;
      Break;
    end;
    CurY := NextY;
  end;
end;

function TDock97.GetDesignModeRowOf (const Y: Integer): Integer;
{ Similar to GetRowOf, but is a little different to accomidate design mode
  better }
var
  HighestRowPlus1, R, CurY, NextY: Integer;
  CurRowInfo: TRowInfo;
begin
  Result := 0;
  HighestRowPlus1 := GetHighestRow+1;
  CurY := 0;
  for R := 0 to HighestRowPlus1 do begin
    Result := R;
    if R = HighestRowPlus1 then Break;
    CurRowInfo := GetRowInfo(R, Self, nil);
    if not(Position in PositionLeftOrRight) then
      NextY := CurY + CurRowInfo.DockedTotalBarHeight
    else
      NextY := CurY + CurRowInfo.DockedTotalBarWidth;
    if Y < NextY then
      Break;
    CurY := NextY;
  end;
end;

function TDock97.GetHighestRow: Integer;
{ Returns highest used row number, or -1 if no rows are used }
var
  I: Integer;
begin
  Result := -1;
  for I := 0 to DockList.Count-1 do
    with TToolbar97(DockList[I]) do
      if FDockRow > Result then
        Result := FDockRow;
end;

procedure TDock97.RemoveBlankRows;
{ Deletes any blank row numbers, adjusting the docked toolbars' FDockRow as
  needed }
var
  HighestRow, R, I: Integer;
  RowIsEmpty: Boolean;
begin
  HighestRow := GetHighestRow;
  R := 0;
  while R <= HighestRow do begin
    RowIsEmpty := True;
    for I := 0 to DockList.Count-1 do
      if TToolbar97(DockList[I]).FDockRow = R then begin
        RowIsEmpty := False;
        Break;
      end;
    if RowIsEmpty then begin
      { Shift all ones higher than R back one }
      for I := 0 to DockList.Count-1 do
        with TToolbar97(DockList[I]) do
          if FDockRow > R then
            Dec (FDockRow);
      Dec (HighestRow);
    end;
    Inc (R);
  end;
end;

procedure TDock97.InsertRowBefore (const BeforeRow: Integer);
{ Inserts a blank row before BeforeRow, adjusting all the docked toolbars'
  FDockRow as needed }
var
  I: Integer;
begin
  for I := 0 to DockList.Count-1 do
    with TToolbar97(DockList[I]) do
      if FDockRow >= BeforeRow then
        Inc (FDockRow);
end;

procedure TDock97.ChangeWidthHeight (const IsClientWidthAndHeight: Boolean;
  NewWidth, NewHeight: Integer);
{ Same as setting Width/Height or ClientWidth/ClientHeight directly, but does
  not lose Align position. }
begin
  if IsClientWidthAndHeight then begin
    Inc (NewWidth, Width-ClientWidth);
    Inc (NewHeight, Height-ClientHeight);
  end;
  case Align of
    alTop, alLeft:
      SetBounds (Left, Top, NewWidth, NewHeight);
    alBottom:
      SetBounds (Left, Top-NewHeight+Height, NewWidth, NewHeight);
    alRight:
      SetBounds (Left-NewWidth+Width, Top, NewWidth, NewHeight);
  end;
end;

procedure TDock97.AlignControls (AControl: TControl; var Rect: TRect);
begin
  ArrangeToolbars;
end;

function CompareDockRowPos (const Item1, Item2, ExtraData: Pointer): Integer; far;
begin
  if TToolbar97(Item1).FDockRow <> TToolbar97(Item2).FDockRow then
    Result := TToolbar97(Item1).FDockRow - TToolbar97(Item2).FDockRow
  else
    Result := TToolbar97(Item1).FDockPos - TToolbar97(Item2).FDockPos;
end;

procedure TDock97.ArrangeToolbars;
{ The main procedure to arrange all the toolbars docked to it }
var
  LeftRight: Boolean;
  EmptySize: Integer;
  HighestRow, R, CurDockPos, CurRowPixel, I, J: Integer;
  HighestTotalBarHeight, HighestTotalBarWidth,
  CurTotalBarHeight, CurTotalBarWidth: Integer;
begin
  if (DisableArrangeToolbars > 0) or (csLoading in ComponentState) then
    Exit;

  LeftRight := Position in PositionLeftOrRight;

  if DockList.Count = 0 then begin
    EmptySize := Ord(FFixAlign);
    if csDesigning in ComponentState then
      EmptySize := 9;
    if not LeftRight then
      ChangeWidthHeight (False, Width, EmptySize)
    else
      ChangeWidthHeight (False, EmptySize, Height);
    Exit;
  end;

  { Ensure list is in correct ordering according to DockRow/DockPos }
  ListSortEx (DockList, CompareDockRowPos, nil);

  { If LimitToOneRow is True, only use the first row }
  if FLimitToOneRow then
    for I := 0 to DockList.Count-1 do
      with TToolbar97(DockList[I]) do
        FDockRow := 0;
  { Remove any blank rows }
  RemoveBlankRows;
  { Rebuild the RowInfo, since rows numbers were probably shifted after
    RemoveBlankRows }
  BuildRowInfo;

  { Find highest row number }
  HighestRow := GetHighestRow;

  { Arrange, first without actually moving the toolbars onscreen }
  for R := 0 to HighestRow do begin
    CurDockPos := 0;
    for I := 0 to DockList.Count-1 do begin
      with TToolbar97(DockList[I]) do begin
        if FDockRow <> R then Continue;
        if FDockPos <= CurDockPos then
          FDockPos := CurDockPos
        else
          CurDockPos := FDockPos;
        if not LeftRight then
          Inc (CurDockPos, Width)
        else
          Inc (CurDockPos, Height);
      end;
    end;
  end;
  { Try to move all the toolbars that are offscreen to a fully visible position }
  for R := 0 to HighestRow do begin
    for I := 0 to DockList.Count-1 do begin
      if TToolbar97(DockList[I]).FDockRow <> R then Continue;
      for J := DockList.Count-1 downto I do begin
        with TToolbar97(DockList[J]) do begin
          if FDockRow <> R then Continue;
          if not LeftRight then begin
            if FDockPos+Width > Self.ClientWidth then begin
              TToolbar97(DockList[I]).FDockPos :=
                TToolbar97(DockList[I]).FDockPos - ((FDockPos+Width) - Self.ClientWidth);
              Break;
            end;
          end
          else begin
            if FDockPos+Height > Self.ClientHeight then begin
              TToolbar97(DockList[I]).FDockPos :=
                TToolbar97(DockList[I]).FDockPos - ((FDockPos+Height) - Self.ClientHeight);
              Break;
            end;
          end;
        end;
      end;
    end;
  end;
  { Arrange again, this time actually moving the toolbars }
  CurRowPixel := 0;
  for R := 0 to HighestRow do begin
    CurDockPos := 0;
    HighestTotalBarHeight := DefaultBarWidthHeight;
    HighestTotalBarWidth := DefaultBarWidthHeight;
    for I := 0 to DockList.Count-1 do begin
      with TToolbar97(DockList[I]) do begin
        if FDockRow <> R then Continue;
        CurTotalBarHeight := GetRowInfo(FDockRow, DockedTo, TToolbar97(DockList[I])).DockedTotalBarHeight;
        CurTotalBarWidth := GetRowInfo(FDockRow, DockedTo, TToolbar97(DockList[I])).DockedTotalBarWidth;
        if CurTotalBarHeight > HighestTotalBarHeight then
          HighestTotalBarHeight := CurTotalBarHeight;
        if CurTotalBarWidth > HighestTotalBarWidth then
          HighestTotalBarWidth := CurTotalBarWidth;
        if FDockPos <= CurDockPos then
          FDockPos := CurDockPos
        else
          CurDockPos := FDockPos;
        Inc (UpdatingBounds);
        try
          if not LeftRight then
            SetVirtualBounds (CurDockPos, CurRowPixel, Width, CurTotalBarHeight)
          else
            SetVirtualBounds (CurRowPixel, CurDockPos, CurTotalBarWidth, Height);
        finally
          Dec (UpdatingBounds);
        end;
        if not LeftRight then
          Inc (CurDockPos, Width)
        else
          Inc (CurDockPos, Height);
      end;
    end;
    if not LeftRight then
      Inc (CurRowPixel, HighestTotalBarHeight)
    else
      Inc (CurRowPixel, HighestTotalBarWidth);
  end;

  { Set the size of the dock }
  if not LeftRight then
    ChangeWidthHeight (True, ClientWidth, CurRowPixel)
  else
    ChangeWidthHeight (True, CurRowPixel, ClientHeight);
end;

procedure TDock97.ChangeDockList (const Insert: Boolean;
  const Bar: TToolbar97; const IsVisible: Boolean);
{ Inserts or removes Bar. It inserts only if IsVisible is True, or is in
  design mode }
var
  Modified: Boolean;
begin
  Modified := False;

  if Insert then begin
    { Delete if already exists }
    if DockList.IndexOf(Bar) <> -1 then
      DockList.Remove (Bar);
    { Only add to dock list if visible }
    if (csDesigning in ComponentState) or IsVisible then begin
      DockList.Add (Bar);
      Modified := True;
    end;
  end
  else begin
    if DockList.IndexOf(Bar) <> -1 then begin
      DockList.Remove (Bar);
      Modified := True;
    end;
  end;

  if Modified then begin
    ArrangeToolbars;
    { This corrects a problem in past versions when toolbar is shown after it
      was initially hidden }
    Bar.AutoArrangeControls;

    if Assigned(FOnInsertRemoveBar) then
      FOnInsertRemoveBar (Self, Insert, Bar);
  end;
end;

procedure TDock97.Loaded;
begin
  inherited Loaded;
  { Rearranging is disabled while the component is loading, so now that it's
    loaded, rearrange it. }
  ArrangeToolbars;
end;

function TDock97.GetPalette: HPALETTE;
begin
  Result := FBkg.Palette;
end;

procedure TDock97.DrawBackground (const Canvas: TCanvas;
  const ClippingRect, DrawRect: TRect);
var
  SaveClipRgn: HRGN;
  R2: TRect;
  UseBmp: TBitmap;
begin
  { Check if Background is assigned, and make sure it doesn't get caught in
    an endless loop }
  if Assigned(FBkg) and (FBkg.Width = 0) or (FBkg.Height = 0) then Exit;

  UseBmp := FBkg;
  { When FBkgTransparent is True, it keeps a cached copy of the
    background that has the transparent color already translated. Without the
    cache, redraws can be very slow.
    Note: The cache is cleared in the OnChange event of FBkg }
  if FBkgTransparent then begin
    if FBkgCache = nil then begin
      FBkgCache := TBitmap.Create;
      with FBkgCache do begin
        Palette := FBkg.Palette;
        Width := FBkg.Width;
        Height := FBkg.Height;
        Canvas.Brush.Color := Self.Color;
        Canvas.BrushCopy (Rect(0, 0, Width, Height), FBkg,
          Rect(0, 0, Width, Height), FBkg.Canvas.Pixels[0, Height-1]);
      end;
    end;
    UseBmp := FBkgCache;
  end;

  SaveClipRgn := 0;
  GetClipRgn (Canvas.Handle, SaveClipRgn);
  with ClippingRect do
    IntersectClipRect (Canvas.Handle, Left, Top, Right, Bottom);
  try
    R2 := DrawRect;
    while R2.Left < R2.Right do begin
      while R2.Top < R2.Bottom do begin
        Canvas.Draw (R2.Left, R2.Top, UseBmp);

        Inc (R2.Top, UseBmp.Height);
      end;
      R2.Top := DrawRect.Top;
      Inc (R2.Left, UseBmp.Width);
    end;
  finally
    { Restore the previous clipping region back }
    SelectClipRgn (Canvas.Handle, SaveClipRgn);
    if SaveClipRgn <> 0 then DeleteObject (SaveClipRgn);
  end;
end;

procedure TDock97.Paint;
var
  R, R2: TRect;
  P1, P2: TPoint;
begin
  inherited Paint;
  with Canvas do begin
    R := ClientRect;

    { Draw dotted border in design mode }
    if csDesigning in ComponentState then begin
      Pen.Style := psDot;
      Pen.Color := clBtnShadow;
      Brush.Style := bsClear;
      Rectangle (R.Left, R.Top, R.Right, R.Bottom);
      Pen.Style := psSolid;
      InflateRect (R, -1, -1);
    end;

    { Draw the Background }
    if Assigned(FBkg) then begin
      R2 := ClientRect;
      { Make up for nonclient area }
      P1 := ClientToScreen(Point(0, 0));
      P2 := Parent.ClientToScreen(BoundsRect.TopLeft);
      Dec (R2.Left, Left + (P1.X-P2.X));
      Dec (R2.Top, Top + (P1.Y-P2.Y));
      DrawBackground (Canvas, R, R2);
    end;
  end;
end;

procedure TDock97.WMMove (var Message: TWMMove);
begin
  inherited;
  if (FBkg.Width <> 0) and (FBkg.Height <> 0) then
    InvalidateBackgrounds;
end;

procedure TDock97.WMSize (var Message: TWMSize);
begin
  inherited;
  if Assigned(FOnResize) then
    FOnResize (Self);
end;

procedure TDock97.WMNCCalcSize (var Message: TWMNCCalcSize);
begin
  inherited;
  with Message.CalcSize_Params^.rgrc[0] do begin
    { Don't add a border when width or height is zero (or one in case of
      FixAlign=True) }
    if ((Right-Left) <= 1) or ((Bottom-Top) <= 1) then
      Exit;
    if blTop in BoundLines then Inc (Top);
    if blBottom in BoundLines then Dec (Bottom);
    if blLeft in BoundLines then Inc (Left);
    if blRight in BoundLines then Dec (Right);
  end;
end;

procedure TDock97.WMNCPaint (var Message: TMessage);
var
  R, R2: TRect;
  DC: HDC;
  NewClipRgn: HRGN;
  HighlightPen, ShadowPen, SavePen: HPEN;
begin
  { This works around WM_NCPAINT problem described at top of source code }
  {no!  R := Rect(0, 0, Width, Height);}
  GetWindowRect (Handle, R);  OffsetRect (R, -R.Left, -R.Top);

  { Don't draw border when width or height is zero (or one in case of
    FixAlign=True) }
  if ((R.Right-R.Left) <= 1) or ((R.Bottom-R.Top) <= 1) then
    Exit;

  DC := GetWindowDC(Handle);
  try
    { Use update region }
    if Message.WParam <> 0 then begin
      GetWindowRect (Handle, R2);
      { An invalid region is generally passed when the window is first created }
      if SelectClipRgn(DC, Message.WParam) = ERROR then begin
        NewClipRgn := CreateRectRgnIndirect(R2);
        SelectClipRgn (DC, NewClipRgn);
        DeleteObject (NewClipRgn);
      end;
      OffsetClipRgn (DC, -R2.Left, -R2.Top);
    end;

    { Draw BoundLines }
    HighlightPen := CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
    ShadowPen := CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
    if blTop in BoundLines then begin
      SavePen := SelectObject(DC, ShadowPen);
      MoveToEx (DC, R.Left, R.Top, nil);  LineTo (DC, R.Right, R.Top);
      SelectObject (DC, SavePen);
    end;
    if blBottom in BoundLines then begin
      SavePen := SelectObject(DC, HighlightPen);
      MoveToEx (DC, R.Left, R.Bottom-1, nil);  LineTo (DC, R.Right, R.Bottom-1);
      SelectObject (DC, SavePen);
    end;
    if blLeft in BoundLines then begin
      SavePen := SelectObject(DC, ShadowPen);
      MoveToEx (DC, R.Left, R.Top, nil);  LineTo (DC, R.Left, R.Bottom);
      SelectObject (DC, SavePen);
    end;
    if blRight in BoundLines then begin
      SavePen := SelectObject(DC, HighlightPen);
      MoveToEx (DC, R.Right-1, R.Top, nil);  LineTo (DC, R.Right-1, R.Bottom);
      SelectObject (DC, SavePen);
    end;
    DeleteObject (ShadowPen);
    DeleteObject (HighlightPen);
  finally
    ReleaseDC (Handle, DC);
  end;
end;

procedure TDock97.CMSysColorChange (var Message: TMessage);
begin
  inherited;
  { Erase the cache }
  BackgroundChanged (FBkg);
end;

{ TDock97 - property access methods }

procedure TDock97.SetAllowDrag (Value: Boolean);
var
  I: Integer;
begin
  if FAllowDrag <> Value then begin
    FAllowDrag := Value;
    for I := 0 to ControlCount-1 do
      if Controls[I] is TToolbar97 then
        with TToolbar97(Controls[I]) do begin
          Invalidate;
          AutoArrangeControls;
        end;
  end;
end;

procedure TDock97.SetBackground (Value: TBitmap);
begin
  FBkg.Assign (Value);
end;

procedure TDock97.InvalidateBackgrounds;
{ Called after background is changed }
var
  I: Integer;
begin
  Invalidate;
  { Synchronize child toolbars also }
  for I := 0 to ControlCount-1 do
    if Controls[I] is TToolbar97 then
      Controls[I].Invalidate;
end;

procedure TDock97.BackgroundChanged (Sender: TObject);
begin
  { Erase the cache }
  if Assigned(FBkgCache) then begin
    FBkgCache.Free;
    FBkgCache := nil;
  end;
  InvalidateBackgrounds;
end;

procedure TDock97.SetBackgroundTransparent (Value: Boolean);
begin
  if FBkgTransparent <> Value then begin
    FBkgTransparent := Value;
    { Erase the cache }
    BackgroundChanged (FBkg);
  end;
end;

procedure TDock97.SetBoundLines (Value: TDockBoundLines);
begin
  if FBoundLines <> Value then begin
    FBoundLines := Value;

    { Recalculate the non-client area }
    SetWindowPos (Handle, 0, 0, 0, 0, 0, SWP_FRAMECHANGED or 
      SWP_NOACTIVATE or SWP_NOMOVE or SWP_NOSIZE or SWP_NOZORDER);
  end;
end;

procedure TDock97.SetFixAlign (Value: Boolean);
begin
  if FFixAlign <> Value then begin
    FFixAlign := Value;
    ArrangeToolbars;
  end;
end;

procedure TDock97.SetPosition (Value: TDockPosition);
begin
  if ControlCount <> 0 then
    raise EInvalidOperation.Create(STB97DockCannotChangePosition);
  FPosition := Value;
  case Position of
    dpTop: Align := alTop;
    dpBottom: Align := alBottom;
    dpLeft: Align := alLeft;
    dpRight: Align := alRight;
  end;
end;

function TDock97.GetToolbarCount: Integer;
begin
  Result := DockList.Count;
end;

function TDock97.GetToolbars (Index: Integer): TToolbar97;
begin
  Result := TToolbar97(DockList[Index]);
end;


{ TFloatParent - Internal }

procedure TFloatParent.CreateParams (var Params: TCreateParams);
begin
  inherited CreateParams (Params);
  with Params do begin
    Style := WS_CHILD;
    ExStyle := 0;
  end;
end;


{ TToolbar97 - Internal }

constructor TToolbar97.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);

  if not(AOwner is TForm) then
    raise EInvalidOperation.Create(STB97ToolbarNotFormOwner);
    { because it frequently casts Owner into a TForm }
  MDIParentForm := GetMDIParent(TForm(AOwner));

  GroupInfo := TList.Create;
  SlaveInfo := TList.Create;
  LineSeps := TList.Create;
  OrderList := TList.Create;

  Inc (DisableArrangeControls);
  try
    ControlStyle := ControlStyle +
      [csAcceptsControls, csClickEvents, csDoubleClicks, csSetCaption] -
      [csCaptureMouse{capturing is done manually}, csOpaque];

    if not(csDesigning in ComponentState) then begin
      FloatParent := TFloatParent.Create(TForm(AOwner));
      FloatParent.Parent := MDIParentForm;
      { Set up the new window procedure for the form (or the MDI parent, if
        it's owner is an MDI child) of the toolbar, and a main window hook }
      OldFormWindowProc := InstallNewWindowProc(0, MDIParentForm,
        NewFormWindowProc, NewMainWindowHook);
      { Add a another window procedure if it's owner is an MDI child }
      if TForm(AOwner).FormStyle = fsMDIChild then
        OldChildFormWindowProc := InstallNewWindowProc(1, TForm(AOwner),
          NewChildFormWindowProc, nil);
      { Need to move it offscreen while loading to prevent any flashing as it's
        updating }
      SetNotOnScreen (True);
    end
    else
      FloatParent := TForm(AOwner);

    FCanDockLeftRight := True;
    FCloseButton := True;
    FDockPos := -1;
    FBarHeight := DefaultBarWidthHeight;
    FBarWidth := DefaultBarWidthHeight;
    Color := clBtnFace;
    DockedTo := nil;
  finally
    Dec (DisableArrangeControls);
  end;
  AutoArrangeControls;
end;

function TToolbar97.GetVirtualBoundsRect: TRect;
begin
  Result := BoundsRect;
  if NotOnScreen then begin
    Result.Right := VirtualLeft + (Result.Right-Result.Left);
    Result.Left := VirtualLeft;
  end;
end;

procedure TToolbar97.SetVirtualBounds (ALeft, ATop, AWidth, AHeight: Integer);
begin
  VirtualLeft := ALeft;
  { Move it off the left of the screen when NotOnScreen is True }
  if NotOnScreen then
    ALeft := -AWidth;
  SetBounds (ALeft, ATop, AWidth, AHeight);
end;

procedure TToolbar97.SetVirtualBoundsRect (const R: TRect);
begin
  SetVirtualBounds (R.Left, R.Top, R.Right-R.Left, R.Bottom-R.Top);
end;

procedure TToolbar97.SetNotOnScreen (const Value: Boolean);
var
  SaveVirtualBounds: TRect;
begin
  if NotOnScreen <> Value then begin
    SaveVirtualBounds := GetVirtualBoundsRect;
    NotOnScreen := Value;
    { Update the bounds so that the change to Value is immediately
      reflected }
    SetVirtualBoundsRect (SaveVirtualBounds);
  end;
end;

procedure TToolbar97.WMMove (var Message: TWMMove);
begin
  inherited;
  if (DockedTo <> nil) and (DockedTo.FBkg.Width <> 0) and
     (DockedTo.FBkg.Height <> 0) then
    { Needs to redraw so that background is lined up with the dock at the
      new position }
    Repaint;
end;

procedure TToolbar97.WMGetMinMaxInfo (var Message: TWMGetMinMaxInfo);
begin
  inherited;
  { This removes the minimum size limit of the window, so it can resize it
    to however small is necessary. This has no effect on Win95/NT 4.0 when it
    uses the WS_EX_TOOLWINDOW style, but is required for Win 3.x. }
  Message.MinMaxInfo^.ptMinTrackSize := Point(1, 1);
end;

procedure TToolbar97.CreateParams (var Params: TCreateParams);
begin
  inherited CreateParams (Params);
  if Parent = FloatParent then
    with Params do begin
      if not(csDesigning in ComponentState) then
        Style := WS_POPUP
      else
        Style := Style and not (WS_BORDER or WS_THICKFRAME);
      { Only Win95/NT 4.0 uses WS_EX_TOOLWINDOW }
      ExStyle := WS_EX_TOOLWINDOW;
    end;
end;

destructor TToolbar97.Destroy;
var
  I: Integer;
begin
  if Assigned(OrderList) then
    OrderList.Free;
  if Assigned(LineSeps) then
    LineSeps.Free;
  if Assigned(SlaveInfo) then begin
    for I := SlaveInfo.Count-1 downto 0 do begin
      FreeMem (SlaveInfo.Items[I]);
      SlaveInfo.Delete (I);
    end;
    SlaveInfo.Free;
  end;
  FreeGroupInfo (GroupInfo);
  if Assigned(GroupInfo) then
    GroupInfo.Free;
  if not(csDesigning in ComponentState) then begin
    UninstallNewWindowProc (0, MDIParentForm);
    if TForm(Owner).FormStyle = fsMDIChild then
      UninstallNewWindowProc (1, TForm(Owner));
  end;

  inherited Destroy;
end;

procedure TToolbar97.Notification (AComponent: TComponent; Operation: TOperation);
begin
  inherited Notification (AComponent, Operation);
  if (AComponent = FDefaultDock) and (Operation = opRemove) then
    FDefaultDock := nil;
end;

procedure TToolbar97.MoveOnScreen (const OnlyIfFullyOffscreen: Boolean);
{ Moves the (floating) toolbar so that it is fully (or at least mostly) in
  view on the screen }
var
  R, S, Test: TRect;
begin
  if DockedTo = nil then begin
    R := GetVirtualBoundsRect;
    S := GetDesktopArea;

    if OnlyIfFullyOffscreen and IntersectRect(Test, R, S) then
      Exit;

    if R.Right > S.Right then
      OffsetRect (R, S.Right - R.Right, 0);
    if R.Bottom > S.Bottom then
      OffsetRect (R, 0, S.Bottom - R.Bottom);
    if R.Left < S.Left then
      OffsetRect (R, S.Left - R.Left, 0);
    if R.Top < S.Top then
      OffsetRect (R, 0, S.Top - R.Top);
    SetVirtualBoundsRect (R);
  end;
end;

function CompareControls (const Item1, Item2, ExtraData: Pointer): Integer; far;
begin
  with PCompareExtra(ExtraData)^ do
    if ComparePositions then begin
      if CurDockType <> dtLeftRight then
        Result := TControl(Item1).Left - TControl(Item2).Left
      else
        Result := TControl(Item1).Top - TControl(Item2).Top;
    end
    else
      with Toolbar.OrderList do
        Result := IndexOf(Item1) - IndexOf(Item2);
end;

procedure TToolbar97.Loaded;
var
  R: TRect;
  Extra: TCompareExtra;
begin
  inherited Loaded;
  { Adjust coordinates if it was initially floating }
  if (not(csDesigning in ComponentState)) and (DockedTo = nil) then begin
    { Read BoundsRect, not VirtualBoundsRect, since it's unable to set the
      VirtualBoundsRect while loading }
    R := BoundsRect;
    MapWindowPoints (TForm(Owner).Handle, 0, R, 2);
    SetVirtualBoundsRect (R);
    MoveOnScreen (False);
  end;
  { Initialize order of items in OrderList }
  if not(csDesigning in ComponentState) then begin
    with Extra do begin
      Toolbar := Self;
      ComparePositions := True;
      CurDockType := GetDockTypeOf(DockedTo);
    end;
    ListSortEx (OrderList, CompareControls, @Extra);
  end;
  { Arranging of controls is disabled while component was loading, so rearrange
    it now }
  AutoArrangeControls;
  if not(csDesigning in ComponentState) then
    { Since SetNotOnScreen(True) was called in the Create constructor, it needs
      to restore it back }
    ShowHideFloatParents (MDIParentForm, Application.Active);
end;

function TToolbar97.GetOrderIndex (Control: TControl): Integer;
begin
  Result := OrderList.IndexOf(Control);
  if Result = -1 then
    raise EInvalidOperation.Create(STB97ToolbarControlNotChildOfToolbar);
end;

procedure TToolbar97.SetOrderIndex (Control: TControl; Value: Integer);
var
  OldIndex: Integer;
begin
  with OrderList do begin
    OldIndex := IndexOf(Control);
    if OldIndex = -1 then
      raise EInvalidOperation.Create(STB97ToolbarControlNotChildOfToolbar);
    if Value < 0 then Value := 0;
    if Value >= Count then Value := Count-1;
    if Value <> OldIndex then begin
      Delete (OldIndex);
      Insert (Value, Control);
      AutoArrangeControls;
    end;
  end;
end;

procedure TToolbar97.SetSlaveControl (const ATopBottom, ALeftRight: TControl);
var
  NewVersion: PSlaveInfo;
begin
  GetMem (NewVersion, SizeOf(TSlaveInfo));
  with NewVersion^ do begin
    TopBottom := ATopBottom;
    LeftRight := ALeftRight;
  end;
  SlaveInfo.Add (NewVersion);
  AutoArrangeControls;
end;

procedure CustomLoadToolbarPositions (const Form: TForm;
  const ReadIntProc: TPositionReadIntProc;
  const ReadStringProc: TPositionReadStringProc; const ExtraData: Pointer);

  function FindDock (AName: String): TDock97;
  var
    I: Integer;
  begin
    Result := nil;
    for I := 0 to Form.ComponentCount-1 do
      if (Form.Components[I] is TDock97) and (Form.Components[I].Name = AName) then begin
        Result := TDock97(Form.Components[I]);
        Break;
      end;
  end;
  procedure ReadValues (const Toolbar: TToolbar97);
  begin
    with Toolbar do begin
      FDockRow := ReadIntProc(Name, rvDockRow, FDockRow, ExtraData);
      FDockPos := ReadIntProc(Name, rvDockPos, FDockPos, ExtraData);
      FFloatingRect.Left := ReadIntProc(Name, rvFloatLeft, 0, ExtraData);
      FFloatingRect.Top := ReadIntProc(Name, rvFloatTop, 0, ExtraData);
      FFloatingRect.Right := ReadIntProc(Name, rvFloatRight, 0, ExtraData);
      FFloatingRect.Bottom := ReadIntProc(Name, rvFloatBottom, 0, ExtraData);
      FFloatingRightX := ReadIntProc(Name, rvFloatRightX, 0, ExtraData);
    end;
  end;
var
  DocksDisabled: TList;
  I: Integer;
  aDock97: TDock97;
  DockedToName: String;
begin
  DocksDisabled := TList.Create;
  try
    with Form do
      for I := 0 to ComponentCount-1 do
        if Components[I] is TDock97 then begin
          Inc (TDock97(Components[I]).DisableArrangeToolbars);
          DocksDisabled.Add (Components[I]);
        end;

    for I := 0 to Form.ComponentCount-1 do
      if Form.Components[I] is TToolbar97 then
        with TToolbar97(Form.Components[I]) do begin
          if Length(Name) = 0 then
            raise Exception.Create (STB97ToolbarNameNotSet);
          Visible := ReadIntProc(Name, rvVisible, Ord(Visible), ExtraData) <> 0;
          DockedToName := ReadStringProc(Name, rvDockedTo, '', ExtraData);
          if Length(DockedToName) <> 0 then begin
            if DockedToName <> 'nil' then begin
              aDock97 := FindDock(DockedToName);
              if (aDock97 <> nil) and (aDock97.FAllowDrag) then begin
                ReadValues (TToolbar97(Form.Components[I]));
                if not IsRectEmpty(FFloatingRect) then
                  AddNCAreaToRect (FFloatingRect);
                DockedTo := aDock97;
              end;
            end
            else begin
              ReadValues (TToolbar97(Form.Components[I]));
              AddNCAreaToRect (FFloatingRect);
              DockedTo := nil;
              MoveOnScreen (False);
            end;
          end;
        end;
  finally
    for I := DocksDisabled.Count-1 downto 0 do begin
      Dec (TDock97(DocksDisabled[I]).DisableArrangeToolbars);
      TDock97(DocksDisabled[I]).ArrangeToolbars;
    end;
    DocksDisabled.Free;
  end;
end;

procedure CustomSaveToolbarPositions (const Form: TForm;
  const WriteIntProc: TPositionWriteIntProc;
  const WriteStringProc: TPositionWriteStringProc; const ExtraData: Pointer);
var
  R: TRect;

  procedure WriteValues (const Toolbar: TToolbar97; const DockedToName: String);
  begin
    with Toolbar do begin
      WriteStringProc (Name, rvDockedTo, DockedToName, ExtraData);
      WriteIntProc (Name, rvDockRow, FDockRow, ExtraData);
      WriteIntProc (Name, rvDockPos, FDockPos, ExtraData);
      WriteIntProc (Name, rvFloatLeft, R.Left, ExtraData);
      WriteIntProc (Name, rvFloatTop, R.Top, ExtraData);
      WriteIntProc (Name, rvFloatRight, R.Right, ExtraData);
      WriteIntProc (Name, rvFloatBottom, R.Bottom, ExtraData);
      WriteIntProc (Name, rvFloatRightX, FFloatingRightX, ExtraData);
    end;
  end;
var
  I: Integer;
begin
  for I := 0 to Form.ComponentCount-1 do
    if Form.Components[I] is TToolbar97 then
      with TToolbar97(Form.Components[I]) do begin
        if Length(Name) = 0 then
          raise Exception.Create (STB97ToolbarNameNotSet);
        if (DockedTo <> nil) and (Length(DockedTo.Name) = 0) then
          raise Exception.Create (STB97ToolbarDockToNameNotSet);
        WriteIntProc (Name, rvVisible, Ord(Visible), ExtraData);
        if DockedTo <> nil then begin
          if DockedTo.FAllowDrag then begin
            R := FFloatingRect;
            if not IsRectEmpty(FFloatingRect) then
              RemoveNCAreaFromRect (R);
            WriteValues (TToolbar97(Form.Components[I]), DockedTo.Name);
          end;
        end
        else begin
          R := GetVirtualBoundsRect;
          RemoveNCAreaFromRect (R);
          WriteValues (TToolbar97(Form.Components[I]), 'nil');
        end;
      end;
end;

function IniReadInt (const ToolbarName, Value: String; const Default: Longint;
  const ExtraData: Pointer): Longint; far;
begin
  Result := TIniFile(ExtraData).ReadInteger(ToolbarName, Value, Default);
end;
function IniReadString (const ToolbarName, Value, Default: String;
  const ExtraData: Pointer): String; far;
begin
  Result := TIniFile(ExtraData).ReadString(ToolbarName, Value, Default);
end;
procedure IniWriteInt (const ToolbarName, Value: String; const Data: Longint;
  const ExtraData: Pointer); far;
begin
  TIniFile(ExtraData).WriteInteger (ToolbarName, Value, Data);
end;
procedure IniWriteString (const ToolbarName, Value, Data: String;
  const ExtraData: Pointer); far;
begin
  TIniFile(ExtraData).WriteString (ToolbarName, Value, Data);
end;

procedure IniLoadToolbarPositions (const Form: TForm; const Filename: String);
var
  Ini: TIniFile;
begin
  Ini := TIniFile.Create(Filename);
  try
    CustomLoadToolbarPositions (Form, IniReadInt, IniReadString, Ini);
  finally
    Ini.Free;
  end;
end;

procedure IniSaveToolbarPositions (const Form: TForm; const Filename: String);
var
  Ini: TIniFile;
begin
  Ini := TIniFile.Create(Filename);
  try
    CustomSaveToolbarPositions (Form, IniWriteInt, IniWriteString, Ini);
  finally
    Ini.Free;
  end;
end;

function RegReadInt (const ToolbarName, Value: String; const Default: Longint;
  const ExtraData: Pointer): Longint; far;
begin
  Result := TRegIniFile(ExtraData).ReadInteger(ToolbarName, Value, Default);
end;
function RegReadString (const ToolbarName, Value, Default: String;
  const ExtraData: Pointer): String; far;
begin
  Result := TRegIniFile(ExtraData).ReadString(ToolbarName, Value, Default);
end;
procedure RegWriteInt (const ToolbarName, Value: String; const Data: Longint;
  const ExtraData: Pointer); far;
begin
  TRegIniFile(ExtraData).WriteInteger (ToolbarName, Value, Data);
end;
procedure RegWriteString (const ToolbarName, Value, Data: String;
  const ExtraData: Pointer); far;
begin
  TRegIniFile(ExtraData).WriteString (ToolbarName, Value, Data);
end;

procedure RegLoadToolbarPositions (const Form: TForm; const BaseRegistryKey: String);
var
  Reg: TRegIniFile;
begin
  Reg := TRegIniFile.Create(BaseRegistryKey);
  try
    CustomLoadToolbarPositions (Form, RegReadInt, RegReadString, Reg);
  finally
    Reg.Free;
  end;
end;

procedure RegSaveToolbarPositions (const Form: TForm; const BaseRegistryKey: String);
var
  Reg: TRegIniFile;
begin
  Reg := TRegIniFile.Create(BaseRegistryKey);
  try
    CustomSaveToolbarPositions (Form, RegWriteInt, RegWriteString, Reg);
  finally
    Reg.Free;
  end;
end;

procedure TToolbar97.FreeGroupInfo (const List: TList);
var
  I: Integer;
  L: PGroupInfo;
begin
  if List = nil then Exit;
  for I := List.Count-1 downto 0 do begin
    L := List.Items[I];
    if Assigned(L) then begin
      if Assigned(L^.Members) then
        L^.Members.Free;
      FreeMem (L);
    end;
    List.Delete (I);
  end;
end;

procedure TToolbar97.BuildGroupInfo (const List: TList;
  const TranslateSlave: Boolean; const OldDockType, NewDockType: TDockType);
var
  I, J: Integer;
  IsVisible: Boolean;
  GI: PGroupInfo;
  Children: TList; {items casted into TControls}
  NewGroup: Boolean;
  Extra: TCompareExtra;
begin
  FreeGroupInfo (List);
  if ControlCount = 0 then Exit;

  Children := TList.Create;
  try
    for I := 0 to ControlCount-1 do begin
      IsVisible := Controls[I].Visible;
      if TranslateSlave then begin
        for J := 0 to SlaveInfo.Count-1 do
          with PSlaveInfo(SlaveInfo[J])^ do begin
            if TopBottom = Controls[I] then begin
              IsVisible := NewDockType <> dtLeftRight;
              Break;
            end;
            if LeftRight = Controls[I] then begin
              IsVisible := NewDockType = dtLeftRight;
              Break;
            end;
          end;
      end;
      if IsVisible then
        Children.Add (Controls[I]);
    end;

    with Extra do begin
      Toolbar := Self;
      CurDockType := OldDockType;
    end;
    if csDesigning in ComponentState then begin
      Extra.ComparePositions := True;
      ListSortEx (OrderList, CompareControls, @Extra);
    end;
    Extra.ComparePositions := csDesigning in ComponentState;
    ListSortEx (Children, CompareControls, @Extra);

    GI := nil;
    NewGroup := True;
    for I := 0 to Children.Count-1 do begin
      if NewGroup then begin
        NewGroup := False;
        List.Add (AllocMem(SizeOf(TGroupInfo)));
        { Note: AllocMem initializes the newly allocated data to zero }
        GI := List[List.Count-1];
        GI^.Members := TList.Create;
      end;
      GI^.Members.Add (Children[I]);
      if TControl(Children[I]) is TToolbarSep97 then
        NewGroup := True
      else begin
        with TControl(Children[I]) do begin
          Inc (GI^.GroupWidth, Width);
          Inc (GI^.GroupHeight, Height);
        end;
      end;
    end;
  finally
    Children.Free;
  end;
end;

procedure TToolbar97.AutoArrangeControls;
begin
  ArrangeControls (True, True, GetDockTypeOf(DockedTo), DockedTo, FFloatingRightX, nil);
end;

procedure TToolbar97.ShouldBeVisible (const Control: TControl;
  const DockType: TDockType; const SetIt: Boolean; var AVisible: Boolean);
{ Sets AVisible only if it is a master or slave control. AVisible is left as is
  otherwise }
var
  J: Integer;
begin
  for J := 0 to SlaveInfo.Count-1 do
    with PSlaveInfo(SlaveInfo[J])^ do
      if TopBottom = Control then begin
        AVisible := DockType <> dtLeftRight;
        if SetIt then begin
          TopBottom.Visible := AVisible;
          LeftRight.Visible := not AVisible;
        end;
      end
      else
      if LeftRight = Control then begin
        AVisible := DockType = dtLeftRight;
        if SetIt then begin
          TopBottom.Visible := not AVisible;
          LeftRight.Visible := AVisible;
        end;
      end;
end;

procedure TToolbar97.ArrangeControls (const CanMove, CanResize: Boolean;
  const OldDockType: TDockType; const DockingTo: TDock97; RightX: Integer;
  const NewClientSize: PPoint);
{ This arranges the controls on the toolbar }
var
  NewDockType: TDockType;
  OldBarHeight, OldBarWidth, OldDockedTotalBarHeight, OldDockedTotalBarWidth,
  I, NewBarHeight, NewBarWidth: Integer;
  AllowWrap, V: Boolean;
  BarPosSize, CurPosPixel, CurLinePixel, G: Integer;
  GInfo: TList;
  GI: PGroupInfo;
  Member: TControl;
  MemberIsSep: Boolean;
  GroupPosSize, MemberPosSize: Integer;
  NewLine, Prec1Line: Boolean;
  MinPosPixels, MinRowPixels, CW, CH: Integer;
  DocksBarHeight, DocksBarWidth, DocksTotalBarHeight, DocksTotalBarWidth: Integer;
  PreviousSep: TToolbarSep97;  PrevMinPosPixels: Integer;
  NewLineSep: TLineSep;
  DockAllowsDrag: Boolean;
label 1;
begin
  if (DisableArrangeControls > 0) or
     { Prevent flicker while loading or destroying }
     (csLoading in ComponentState) or
     { Following line added in 1.53 to stop the access violations that 1.52 was
       causing while destroying. }
     (csDestroying in ComponentState) or
     (Parent.HandleAllocated and (csDestroying in Parent.ComponentState)) then
    Exit;

  NewDockType := GetDockTypeOf(DockingTo);

  if (NewDockType <> dtNotDocked) or (RightX = 0) then
    RightX := High(RightX);

  DockAllowsDrag := (DockedTo = nil) or (DockedTo.FAllowDrag);

  Inc (DisableArrangeControls);
  try
    OldBarWidth := FBarWidth;
    OldBarHeight := FBarHeight;
    OldDockedTotalBarHeight := FDockedTotalBarHeight;
    OldDockedTotalBarWidth := FDockedTotalBarWidth;

    try
      NewBarHeight := DefaultBarWidthHeight;
      NewBarWidth := DefaultBarWidthHeight;
      for I := 0 to ControlCount-1 do begin
        if Controls[I] is TToolbarSep97 then Continue;
        with Controls[I] do begin
          V := Visible;
          ShouldBeVisible (Controls[I], NewDockType, CanMove, V);
          if not V then Continue;
          if Height > NewBarHeight then
            NewBarHeight := Height;
          if Width > NewBarWidth then
            NewBarWidth := Width;
        end;
      end;
      FBarHeight := NewBarHeight;
      FBarWidth := NewBarWidth;
      { the following line is mirrored in BuildRowInfo }
      FDockedTotalBarHeight := TopMargin[dtTopBottom] + FBarHeight + BottomMargin[dtTopBottom];
      FDockedTotalBarWidth := TopMargin[dtLeftRight] + FBarWidth + BottomMargin[dtLeftRight];

      DocksBarHeight := FBarHeight;
      DocksBarWidth := FBarWidth;
      DocksTotalBarHeight := FDockedTotalBarHeight;
      DocksTotalBarWidth := FDockedTotalBarWidth;
      if CanMove and (DockingTo <> nil) and (DockingTo = DockedTo) then
        with DockingTo do begin
          BuildRowInfo;
          with GetRowInfo(FDockRow, DockingTo, Self) do begin
            DocksBarHeight := BarHeight;
            DocksBarWidth := BarWidth;
            DocksTotalBarHeight := DockedTotalBarHeight;
            DocksTotalBarWidth := DockedTotalBarWidth;
          end;
        end;

      if CanMove then
        GInfo := GroupInfo
      else
        GInfo := TList.Create;
      try
        BuildGroupInfo (GInfo, not CanMove, OldDockType, NewDockType);

        if CanMove then
          LineSeps.Clear;

        AllowWrap := NewDockType = dtNotDocked;
        if GInfo.Count <> 0 then begin
          if NewDockType <> dtLeftRight then
            BarPosSize := FBarHeight
          else
            BarPosSize := FBarWidth;
          MinPosPixels := 0;
          CurPosPixel := 0;
          CurLinePixel := TopMargin[NewDockType];
          Prec1Line := True;  NewLine := True;
          PreviousSep := nil;  PrevMinPosPixels := 0;
          for G := 0 to GInfo.Count-1 do begin
            GI := PGroupInfo(GInfo[G]);

            if NewDockType <> dtLeftRight then
              GroupPosSize := GI^.GroupWidth
            else
              GroupPosSize := GI^.GroupHeight;
            if (not AllowWrap) or (Prec1Line) then begin
              if NewLine then begin
                NewLine := False;
                Inc (CurPosPixel, LeftMargin[DockAllowsDrag, NewDockType])
              end;
              if CurPosPixel+GroupPosSize+RightMargin[NewDockType] > RightX then
                goto 1;  { I know it's sloppy to use a goto. But it's fast }
              if CurPosPixel > MinPosPixels then MinPosPixels := CurPosPixel;
            end
            else begin
            1:CurPosPixel := LeftMargin[DockAllowsDrag, NewDockType];
              if CurPosPixel > MinPosPixels then MinPosPixels := CurPosPixel;
              if (G <> 0) and (PGroupInfo(GInfo[G-1])^.Members.Count <> 0) then begin
                Inc (CurLinePixel, BarPosSize+LineSpacing);
                if Assigned(PreviousSep) then begin
                  MinPosPixels := PrevMinPosPixels;
                  if CanMove then begin
                    PreviousSep.Width := 0;

                    LongInt(NewLineSep) := 0;
                    NewLineSep.Y := CurLinePixel;
                    NewLineSep.Blank := PreviousSep.Blank;
                    LineSeps.Add (Pointer(NewLineSep));
                  end;
                end;
              end;
            end;
            Prec1Line := True;
            for I := 0 to GI^.Members.Count-1 do begin
              Member := TControl(GI^.Members[I]);
              MemberIsSep := Member is TToolbarSep97;
              with Member do begin
                if not MemberIsSep then begin
                  if NewDockType <> dtLeftRight then
                    MemberPosSize := Width
                  else
                    MemberPosSize := Height;
                end
                else begin
                  if NewDockType <> dtLeftRight then
                    MemberPosSize := TToolbarSep97(Member).SizeHorz
                  else
                    MemberPosSize := TToolbarSep97(Member).SizeVert;
                end;
                { If past right (or bottom) side of screen, proceed to next line }
                if not MemberIsSep and
                   (CurPosPixel+MemberPosSize+RightMargin[NewDockType] > RightX) then begin
                  CurPosPixel := LeftMargin[DockAllowsDrag, NewDockType];
                  if CurPosPixel > MinPosPixels then MinPosPixels := CurPosPixel;
                  Inc (CurLinePixel, FBarHeight);
                  Prec1Line := False;
                end;
                if NewDockType <> dtLeftRight then begin
                  if not MemberIsSep then begin
                    if CanMove then
                      SetBounds (CurPosPixel, CurLinePixel+((DocksBarHeight-Height) div 2), Width, Height);
                    Inc (CurPosPixel, Width);
                  end
                  else begin
                    if CanMove then
                      SetBounds (CurPosPixel, CurLinePixel, TToolbarSep97(Member).SizeHorz, DocksBarHeight);
                    Inc (CurPosPixel, TToolbarSep97(Member).SizeHorz);
                  end;
                end
                else begin
                  if not MemberIsSep then begin
                    if CanMove then
                      SetBounds (CurLinePixel+((DocksBarWidth-Width) div 2), CurPosPixel, Width, Height);
                    Inc (CurPosPixel, Height);
                  end
                  else begin
                    if CanMove then
                      SetBounds (CurLinePixel, CurPosPixel, DocksBarWidth, TToolbarSep97(Member).SizeVert);
                    Inc (CurPosPixel, TToolbarSep97(Member).SizeVert);
                  end;
                end;
                PrevMinPosPixels := MinPosPixels;
                if not MemberIsSep then
                  PreviousSep := nil
                else
                  PreviousSep := TToolbarSep97(Member);
                if CurPosPixel > MinPosPixels then MinPosPixels := CurPosPixel;
              end;
            end;
          end;
        end
        else begin
          if DockedTo <> nil then begin
            MinPosPixels := LeftMargin[DockAllowsDrag, NewDockType];
            CurLinePixel := TopMargin[NewDockType];
            if not(DockedTo.Position in PositionLeftOrRight) then begin
              Inc (MinPosPixels, DefaultBarWidthHeight);
              BarPosSize := GetRowInfo(FDockRow, DockedTo, Self).BarHeight;
            end
            else begin
              Inc (MinPosPixels, DefaultBarWidthHeight);
              BarPosSize := GetRowInfo(FDockRow, DockedTo, Self).BarWidth;
            end;
          end
          else begin
            MinPosPixels := LeftMargin[DockAllowsDrag, NewDockType] + DefaultBarWidthHeight;
            CurLinePixel := TopMargin[NewDockType];
            BarPosSize := DefaultBarWidthHeight;
          end;
        end;

        if csDesigning in ComponentState then
          Invalidate;
      finally
        if not CanMove then begin
          FreeGroupInfo (GInfo);
          GInfo.Free;
        end;
      end;

      if CanMove then begin
        CW := ClientWidth;
        CH := ClientHeight;
      end
      else begin
        CW := 0;
        CH := 0;
      end;

      Inc (MinPosPixels, RightMargin[NewDockType]);
      if NewDockType <> dtLeftRight then
        CW := MinPosPixels
      else
        CH := MinPosPixels;

      MinRowPixels := CurLinePixel + BarPosSize + BottomMargin[NewDockType];
      if NewDockType <> dtLeftRight then 
        CH := MinRowPixels
      else
        CW := MinRowPixels;

      if DockedTo <> nil then begin
        if NewDockType <> dtLeftRight then begin
          if CH < DocksTotalBarHeight then
            CH := DocksTotalBarHeight;
        end
        else begin
          if CW < DocksTotalBarWidth then
            CW := DocksTotalBarWidth;
        end;
      end;

      if CanResize and ((ClientWidth <> CW) or (ClientHeight <> CH)) then begin
        Inc (UpdatingBounds);
        try
          SetVirtualBounds (Left, Top,
            (Width-ClientWidth)+CW, (Height-ClientHeight)+CH);
        finally
          Dec (UpdatingBounds);
        end;
      end;
      if Assigned(NewClientSize) then begin
        NewClientSize^.X := CW;
        NewClientSize^.Y := CH;
      end;
    finally
      if not CanMove then begin
        FBarWidth := OldBarWidth;
        FBarHeight := OldBarHeight;
        FDockedTotalBarHeight := OldDockedTotalBarHeight;
        FDockedTotalBarWidth := OldDockedTotalBarWidth;
      end;
    end;
  finally
    Dec (DisableArrangeControls);
  end;
end;

procedure TToolbar97.AlignControls (AControl: TControl; var Rect: TRect);
{ VCL calls this whenever any child controls in the toolbar are moved, sized,
  inserted, etc. It doesn't make use of the AControl and Rect parameters,
  since it doesn't need to. }
begin
  AutoArrangeControls;
end;

procedure TToolbar97.SetBounds (ALeft, ATop, AWidth, AHeight: Integer);
begin
  inherited SetBounds (ALeft, ATop, AWidth, AHeight);
  { This allows you to drag the toolbar at design time }
  if (csDesigning in ComponentState) and not(csLoading in ComponentState) and
     (DockedTo <> nil) and (UpdatingBounds = 0) then begin
    if not(DockedTo.Position in PositionLeftOrRight) then begin
      FDockRow := DockedTo.GetDesignModeRowOf(ATop+(AHeight div 2));
      FDockPos := ALeft;
    end
    else begin
      FDockRow := DockedTo.GetDesignModeRowOf(ALeft+(AWidth div 2));
      FDockPos := ATop;
    end;
    DockedTo.ArrangeToolbars;
  end;
end;

procedure TToolbar97.SetParent (AParent: TWinControl);
begin
  if not(csDesigning in ComponentState) and (AParent = TForm(Owner)) then
    AParent := FloatParent;

  if (AParent <> nil) and not(AParent is TDock97) and
     not(AParent = Owner) and not(AParent is TFloatParent) then
    raise EInvalidOperation.Create(STB97ToolbarParentNotAllowed);

  if not(csDestroying in ComponentState) and Assigned(FOnRecreating) then
    FOnRecreating (Self);

  if Parent is TDock97 then
    TDock97(Parent).ChangeDockList (False, Self, Visible or (Hidden <> 0));

  inherited SetParent (AParent);

  if Parent is TDock97 then
    TDock97(Parent).ChangeDockList (True, Self, Visible or (Hidden <> 0));

  if not(csDestroying in ComponentState) and Assigned(FOnRecreated) then
    FOnRecreated (Self);
end;

procedure TToolbar97.CMControlListChange (var Message: TCMControlListChange);
{ The VCL sends this message is sent whenever a child control is inserted into
  or deleted from the toolbar }
var
  I: Integer;
begin
  inherited;
  with Message, OrderList do begin
    { Delete any previous occurances of Control in OrderList. There shouldn't
      be any if Inserting=True, but just to be safe, check anyway. }
    while True do begin
      I := IndexOf(Control);
      if I = -1 then Break;
      Delete (I);
    end;
    if Inserting then
      Add (Control);
  end;
end;

function GetCaptionRect (const Control: TWinControl;
  const AdjustForBorder, MinusCloseButton: Boolean): TRect;
begin
  Result := Rect(0, 0, Control.ClientWidth, GetCaptionHeight-1);
  if MinusCloseButton then
    Dec (Result.Right, GetCaptionHeight-1);
  if AdjustForBorder then
    OffsetRect (Result, GetBorderSize, GetBorderSize);
end;

function GetCloseButtonRect (const Control: TWinControl;
  const AdjustForBorder: Boolean): TRect;
begin
  Result := Rect(0, 0, Control.ClientWidth, GetCaptionHeight-1);
  if AdjustForBorder then
    OffsetRect (Result, GetBorderSize, GetBorderSize);
  Result.Left := Result.Right - (GetCaptionHeight-1);
end;

procedure TToolbar97.WMNCCalcSize (var Message: TWMNCCalcSize);
begin
  { Note to self: inherited must be up here or it doesn't work right }
  inherited;
  if DockedTo = nil then begin
    InflateRect (Message.CalcSize_Params^.rgrc[0], -GetBorderSize, -GetBorderSize);
    Inc (Message.CalcSize_Params^.rgrc[0].Top, GetCaptionHeight);
  end;
end;

procedure TToolbar97.DrawNCArea (const Clip: HRGN;
  const RedrawBorder, RedrawCaption, RedrawCloseButton: Boolean);
{ Redraws all the non-client area (the border, title bar, and close button) of
  the toolbar when it is floating.
  At design time, the caption is always drawn with the activated color. }

  procedure Win3DrawCaption (const DC: HDC; const R: TRect);
  { Emulates DrawCaption, which isn't supported in Win 3.x }
  const
    Ellipsis = '...';
  var
    R2: TRect;
    SaveTextColor, SaveBkColor: TColorRef;
    NewFont, SaveFont: HFONT;
    NewBrush: HBRUSH;
    Cap: String;

    function CaptionTextWidth: Integer;
    var
      Size: TSize;
    begin
      GetTextExtentPoint32 (DC, @Cap[1], Length(Cap), Size);
      Result := Size.cx;
    end;
  begin
    R2 := R;

    { Fill the rectangle }
    NewBrush := CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));
    FillRect (DC, R2, NewBrush);
    DeleteObject (NewBrush);

    Inc (R2.Left);
    Dec (R2.Right);

    NewFont := CreateFont(-11, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 'MS Sans Serif');
    SaveFont := SelectObject(DC, NewFont);

    { Add ellipsis to caption if necessary }
    Cap := Caption;
    if CaptionTextWidth > R2.Right-R2.Left then begin
      Cap := Cap + Ellipsis;
      while CaptionTextWidth > R2.Right-R2.Left do begin
        if Length(Cap) <= 4 then Break;
        Delete (Cap, Length(Cap)-Length(Ellipsis), 1)
      end;
    end;

    { Draw the text }
    SaveBkColor := SetBkColor(DC, GetSysColor(COLOR_ACTIVECAPTION));
    SaveTextColor := SetTextColor(DC, GetSysColor(COLOR_CAPTIONTEXT));
    DrawText (DC, @Cap[1], Length(Cap), R2, DT_SINGLELINE or DT_NOPREFIX or DT_VCENTER);
    SetTextColor (DC, SaveTextColor);
    SetBkColor (DC, SaveBkColor);

    SelectObject (DC, SaveFont);
    DeleteObject (NewFont);
  end;
const
  CloseButtonState: array[Boolean] of UINT = (0, DFCS_PUSHED);
var
  DC: HDC;
  R: TRect;
  NewClipRgn: HRGN;
  I: Integer;
  NewDrawCaption: function(p1: HWND; p2: HDC; const p3: TRect; p4: UINT): BOOL; stdcall;
  Pen, SavePen: HPEN;
  Brush: HBRUSH;
begin
  if DockedTo <> nil then Exit;

  DC := GetWindowDC(Handle);
  try
    { Use update region }
    if Clip <> 0 then begin
      GetWindowRect (Handle, R);
      { An invalid region is generally passed when the window is first created }
      if SelectClipRgn(DC, Clip) = ERROR then begin
        NewClipRgn := CreateRectRgnIndirect(R);
        SelectClipRgn (DC, NewClipRgn);
        DeleteObject (NewClipRgn);
      end;
      OffsetClipRgn (DC, -R.Left, -R.Top);
    end;

    { Border }
    if RedrawBorder then begin
      { This works around WM_NCPAINT problem described at top of source code }
      {no!  R := Rect(0, 0, Width, Height);}
      GetWindowRect (Handle, R);  OffsetRect (R, -R.Left, -R.Top);
      Brush := CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
      for I := 1 to GetBorderSize do
        case I of
          1: DrawEdge (DC, R, EDGE_RAISED, BF_RECT or BF_ADJUST);
          2: ;
        else
          FrameRect (DC, R, Brush);
          InflateRect (R, -1, -1);
        end;
      DeleteObject (Brush);
    end;

    { Caption }
    if RedrawCaption then begin
      R := GetCaptionRect(Self, True, FCloseButton);
      if NewStyleControls then begin
        { Use a dynamic import of DrawCaption since it's Win95/NT 4.0 only.
          Also note that Delphi's Win32 help for DrawCaption is totally wrong!
          I got updated info from www.microsoft.com/msdn/sdk/ }
        NewDrawCaption := GetProcAddress(GetModuleHandle(user32), 'DrawCaption');
        NewDrawCaption (Handle, DC, R, DC_ACTIVE or DC_TEXT or DC_SMALLCAP);
      end
      else
        Win3DrawCaption (DC, R);

      { Line below caption }
      R := GetCaptionRect(Self, True, False);
      Pen := CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
      SavePen := SelectObject(DC, Pen);
      MoveToEx (DC, R.Left, R.Bottom, nil);
      LineTo (DC, R.Right, R.Bottom);
      SelectObject (DC, SavePen);
      DeleteObject (Pen);
    end;

    { Close button }
    if FCloseButton then begin
      if RedrawCloseButton then begin
        R := GetCloseButtonRect(Self, True);
        InflateRect (R, -1, -1);
        DrawFrameControl (DC, R, DFC_CAPTION, DFCS_CAPTIONCLOSE or
          CloseButtonState[CloseButtonDown]);
      end;
      if RedrawCaption then begin
        { Caption-colored frame around close button }
        R := GetCloseButtonRect(Self, True);
        Brush := CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));
        FrameRect (DC, R, Brush);
        DeleteObject (Brush);
      end;
    end;
  finally
    ReleaseDC (Handle, DC);
  end;
end;

procedure TToolbar97.WMNCPaint (var Message: TMessage);
begin
  inherited;
  DrawNCArea (Message.WParam, True, True, True);
end;

procedure DrawDragRect (const DC: HDC; const NewRect, OldRect: PRect;
  const NewSize, OldSize: TSize; const Brush: HBRUSH; BrushLast: HBRUSH);
{ Draws a dragging outline, hiding the old one if neccessary. This is
  completely flicker free, unlike the old DrawFocusRect method. In case
  you're wondering, I got a lot of ideas from the MFC sources.

  Either NewRect or OldRect can be nil or empty.

  NOTE: If the specific DC had a clipping region, it will be gone when this
  function exits. }
const
  BlankRect: TRect = (Left: 0; Top: 0; Right: 0; Bottom: 0);
var
  rgnNew, rgnOutside, rgnInside, rgnLast, rgnUpdate: HRGN;
  R: TRect;
  SaveBrush: HBRUSH;
begin
  rgnLast := 0;
  rgnUpdate := 0;

  { First, determine the update region and select it }
  if NewRect = nil then begin
    R := BlankRect;
    rgnOutside := CreateRectRgnIndirect(R);
  end
  else begin
    R := NewRect^;
    rgnOutside := CreateRectRgnIndirect(R);
    InflateRect (R, -NewSize.cx, -NewSize.cy);
    IntersectRect (R, R, NewRect^);
  end;
  rgnInside := CreateRectRgnIndirect(R);
  rgnNew := CreateRectRgnIndirect(BlankRect);
  CombineRgn (rgnNew, rgnOutside, rgnInside, RGN_XOR);

  if BrushLast = 0 then
    BrushLast := Brush;

  if OldRect <> nil then begin
    { Find difference between new region and old region }
    rgnLast := CreateRectRgnIndirect(BlankRect);
    with OldRect^ do
      SetRectRgn (rgnOutside, Left, Top, Right, Bottom);
    R := OldRect^;
    InflateRect (R, -OldSize.cx, -OldSize.cy);
    IntersectRect (R, R, OldRect^);
    SetRectRgn (rgnInside, R.Left, R.Top, R.Right, R.Bottom);
    CombineRgn (rgnLast, rgnOutside, rgnInside, RGN_XOR);

    { Only diff them if brushes are the same}
    if Brush = BrushLast then begin
      rgnUpdate := CreateRectRgnIndirect(BlankRect);
      CombineRgn (rgnUpdate, rgnLast, rgnNew, RGN_XOR);
    end;
  end;
  if (Brush <> BrushLast) and (OldRect <> nil) then begin
    { Brushes are different -- erase old region first }
    SelectClipRgn (DC, rgnLast);
    GetClipBox (DC, R);
    SaveBrush := SelectObject(DC, BrushLast);
    PatBlt (DC, R.Left, R.Top, R.Right-R.Left, R.Bottom-R.Top, PATINVERT);
    SelectObject (DC, SaveBrush);
  end;

  { Draw into the update/new region }
  if rgnUpdate <> 0 then
    SelectClipRgn (DC, rgnUpdate)
  else
    SelectClipRgn (DC, rgnNew);
  GetClipBox (DC, R);
  SaveBrush := SelectObject(DC, Brush);
  PatBlt (DC, R.Left, R.Top, R.Right-R.Left, R.Bottom-R.Top, PATINVERT);
  SelectObject (DC, SaveBrush);

  { Free regions }
  if rgnNew <> 0 then DeleteObject (rgnNew);
  if rgnOutside <> 0 then DeleteObject (rgnOutside);
  if rgnInside <> 0 then DeleteObject (rgnInside);
  if rgnLast <> 0 then DeleteObject (rgnLast);
  if rgnUpdate <> 0 then DeleteObject (rgnUpdate);

  { Clean up DC }
  SelectClipRgn (DC, 0);
end;

procedure TToolbar97.DrawDraggingOutline (const DC: HDC;
  const NewRect, OldRect: PRect; const NewDocking, OldDocking: Boolean);

  function CreateHalftoneBrush: HBRUSH;
  const
    Patterns: array[Boolean] of Word = ($5555, $AAAA);
  var
    I: Integer;
    GrayPattern: array[0..7] of Word;
    GrayBitmap: HBITMAP;
  begin
    Result := 0;
    for I := 0 to 7 do
      GrayPattern[I] := Patterns[I and 1 <> 0];
    GrayBitmap := CreateBitmap(8, 8, 1, 1, @GrayPattern);
    if GrayBitmap <> 0 then begin
      Result := CreatePatternBrush(GrayBitmap);
      DeleteObject (GrayBitmap);
    end;
  end;
var
  NewSize, OldSize: TSize;
  Brush: HBRUSH;
begin
  Brush := CreateHalftoneBrush;
  try
    if NewDocking then NewSize.cx := 1 else NewSize.cx := GetBorderSize;
    NewSize.cy := NewSize.cx;
    if OldDocking then OldSize.cx := 1 else OldSize.cx := GetBorderSize;
    OldSize.cy := OldSize.cx;
    DrawDragRect (DC, NewRect, OldRect, NewSize, OldSize, Brush, Brush);
  finally
    DeleteObject (Brush);
  end;
end;

procedure TToolbar97.Paint;
  procedure DrawRaisedEdge (R: TRect; const FillInterior: Boolean);
  const
    FillMiddle: array[Boolean] of UINT = (0, BF_MIDDLE);
  begin
    DrawEdge (Canvas.Handle, R, BDR_RAISEDINNER, BF_RECT or FillMiddle[FillInterior]);
  end;
var
  DockType: TDockType;
  X, Y, S: Integer;
  R, R2: TRect;
  P1, P2: TPoint;
  LS: TLineSep;
begin
  inherited Paint;

  if DockedTo = nil then
    DockType := dtNotDocked
  else begin
    if not(DockedTo.Position in PositionLeftOrRight) then
      DockType := dtTopBottom
    else
      DockType := dtLeftRight;
  end;

  if DockType <> dtNotDocked then begin
    { Border }
    R := ClientRect;
    DrawRaisedEdge (R, False);

    { Draw the Background }
    if (DockedTo <> nil) and Assigned(DockedTo.FBkg) then begin
      R2 := R;
      P1 := DockedTo.ClientToScreen(Point(0, 0));
      P2 := DockedTo.Parent.ClientToScreen(DockedTo.BoundsRect.TopLeft);
      Dec (R2.Left, Left + DockedTo.Left + (P1.X-P2.X));
      Dec (R2.Top, Top + DockedTo.Top + (P1.Y-P2.Y));
      InflateRect (R, -1, -1);
      DockedTo.DrawBackground (Canvas, R, R2);
    end;

    { The drag handle at the left, or top }
    if (DockedTo <> nil) and (DockedTo.FAllowDrag) then
      if DockType <> dtLeftRight then begin
        Y := ClientHeight-2;
        DrawRaisedEdge (Rect(4, 2, 7, Y), True);
        Canvas.Pixels[4, Y-1] := clBtnHighlight;
        DrawRaisedEdge (Rect(7, 2, 10, Y), True);
        Canvas.Pixels[7, Y-1] := clBtnHighlight;
      end
      else begin
        X := ClientWidth-2;
        DrawRaisedEdge (Rect(2, 4, X, 7), True);
        Canvas.Pixels[X-1, 4] := clBtnHighlight;
        DrawRaisedEdge (Rect(2, 7, X, 10), True);
        Canvas.Pixels[X-1, 7] := clBtnHighlight;
      end;
  end;

  { Long separators when not docked }
  if DockedTo = nil then
    for S := 0 to LineSeps.Count-1 do begin
      Pointer(LS) := LineSeps[S];
      with LS do begin
        if Blank then Continue;
        Canvas.Pen.Color := clBtnShadow;
        Canvas.MoveTo (1, Y-4);  Canvas.LineTo (ClientWidth-1, Y-4);
        Canvas.Pen.Color := clBtnHighlight;
        Canvas.MoveTo (1, Y-3);  Canvas.LineTo (ClientWidth-1, Y-3);
      end;
    end;
end;

procedure TToolbar97.CMTextChanged (var Message: TMessage);
begin
  inherited;
  { Update the title bar to use the new Caption }
  DrawNCArea (0, False, True, False);
end;

procedure TToolbar97.CMVisibleChanged (var Message: TMessage);
begin
  if not(csDesigning in ComponentState) and
     (Hidden = 0) and (DockedTo <> nil) then
    DockedTo.ChangeDockList (Visible, Self, Visible);
  inherited;
end;

procedure TToolbar97.WMActivate (var Message: TWMActivate);
begin
  SendMessage (MDIParentForm.Handle, WM_NCACTIVATE, Ord(Message.Active <> WA_INACTIVE), 0);
  inherited;
end;

procedure TToolbar97.WMMouseActivate (var Message: TWMMouseActivate);
begin
  if (csDesigning in ComponentState) or (DockedTo <> nil) then
    inherited
  else begin
    { When floating, prevent the toolbar from activating when clicked.
      This is so it doesn't take the focus away from the window that had it }
    Message.Result := MA_NOACTIVATE;

    { Similar to calling BringWindowToTop, but doesn't activate it }
    SetWindowPos (Handle, HWND_TOP, 0, 0, 0, 0,
      SWP_NOACTIVATE or SWP_NOMOVE or SWP_NOSIZE);

    { Since it is returning MA_NOACTIVATE, activate the form instead. }
    if GetActiveForm <> MDIParentForm then
      { Must only activate if it wasn't already activated so it doesn't
        change the focus }
      SetActiveWindow (MDIParentForm.Handle);
  end;
end;

procedure TToolbar97.BeginMoving (const InitX, InitY: Integer);
type
  PDockedSize = ^TDockedSize;
  TDockedSize = record
    Dock: TDock97;
    Size: TPoint;
  end;
var
  NewDockedSizes: TList; {items are pointers to TDockedSizes}
  MouseOverDock: TDock97;
  MoveRect: TRect;
  PreventDocking: Boolean;
  ScreenDC: HDC;
  NPoint, DPoint: TPoint;

  procedure Dropped;
  var
    NewDockRow: Integer;
    Before: Boolean;
    MoveRectClient: TRect;
    C: Integer;
  begin
    if MouseOverDock <> nil then begin
      MoveRectClient := MoveRect;
      MapWindowPoints (0, MouseOverDock.Handle, MoveRectClient, 2);
      if not(MouseOverDock.Position in PositionLeftOrRight) then
        C := (MoveRectClient.Top+MoveRectClient.Bottom) div 2
      else
        C := (MoveRectClient.Left+MoveRectClient.Right) div 2;
      NewDockRow := TDock97(MouseOverDock).GetRowOf(C, Before);
      if Before then
        TDock97(MouseOverDock).InsertRowBefore (NewDockRow);
      FDockRow := NewDockRow;
      if not(MouseOverDock.Position in PositionLeftOrRight) then
        FDockPos := MoveRectClient.Left
      else
        FDockPos := MoveRectClient.Top;
      DockedTo := MouseOverDock;
    end
    else begin
      FFloatingRect := MoveRect;
      DockedTo := nil;
    end;

    { Make sure it doesn't go completely off the screen }
    MoveOnScreen (True);
  end;

  procedure MouseMoved;
  var
    OldMouseOverDock: TDock97;
    OldMoveRect: TRect;
    Pos: TPoint;

    function CheckIfCanDockTo (Control: TDock97): Boolean;
    const
      DockSensX = 32;
      DockSensY = 20;
    var
      R, S, Temp: TRect;
      I: Integer;
      Sens: Integer;
    begin
      with Control do begin
        Result := False;

        R := ClientRect;
        MapWindowPoints (Handle, 0, R, 2);
        for I := 0 to NewDockedSizes.Count-1 do
          with PDockedSize(NewDockedSizes[I])^ do begin
            if Dock <> Control then Continue;
            S := Bounds(Pos.X-MulDiv(Size.X-1, NPoint.X, DPoint.X),
              Pos.Y-MulDiv(Size.Y-1, NPoint.Y, DPoint.Y),
              Size.X, Size.Y);
            Break;
          end;
        if (R.Left = R.Right) or (R.Top = R.Bottom) then begin
          if not(Control.Position in PositionLeftOrRight) then
            InflateRect (R, 0, 1)
          else
            InflateRect (R, 1, 0);
        end;

        { Like Office 97, distribute ~32 pixels of extra dock detection area
          to the left side if the toolbar was grabbed at the left, both sides
          if the toolbar was grabbed at the middle, or the right side if
          toolbar was grabbed at the right. If outside, don't try to dock. }
        Sens := MulDiv(DockSensX, NPoint.X, DPoint.X);
        if (Pos.X < R.Left-(DockSensX-Sens)) or (Pos.X > R.Right-1+Sens) then
          Exit;

        { Don't try to dock to the left or right if pointer is above or below
          the boundaries of the dock }
        if (Control.Position in PositionLeftOrRight) and
           ((Pos.Y < R.Top) or (Pos.Y >= R.Bottom)) then
          Exit;

        { And also distribute ~20 pixels of extra dock detection area to
          the top or bottom side }
        Sens := MulDiv(DockSensY, NPoint.Y, DPoint.Y);
        if (Pos.Y < R.Top-(DockSensY-Sens)) or (Pos.Y > R.Bottom-1+Sens) then
          Exit;

        Result := IntersectRect(Temp, R, S);
      end;
    end;
  var
    R: TRect;
    I: Integer;
  begin
    OldMouseOverDock := MouseOverDock;
    OldMoveRect := MoveRect;

    GetCursorPos (Pos);

    { Check if it can dock }
    MouseOverDock := nil;
    if not PreventDocking then begin
      { Search through the form's controls and see if it can find a
        Dock97. If it finds one, assign it to MouseOverDock. }
      with TForm(Owner) do begin
        { Try top/bottom first }
        for I := 0 to ComponentCount-1 do
          if (Components[I] is TDock97) and
             not(TDock97(Components[I]).Position in PositionLeftOrRight) and
             TDock97(Components[I]).FAllowDrag and
             CheckIfCanDockTo(TDock97(Components[I])) then begin
            MouseOverDock := TDock97(Components[I]);
            Break;
          end;
        { Then left/right }
        if (MouseOverDock = nil) and (FCanDockLeftRight) then
          for I := 0 to ComponentCount-1 do
            if (Components[I] is TDock97) and
               (TDock97(Components[I]).Position in PositionLeftOrRight) and
               TDock97(Components[I]).FAllowDrag and
               CheckIfCanDockTo(TDock97(Components[I])) then begin
              MouseOverDock := TDock97(Components[I]);
              Break;
            end;
      end;
    end;

    { If not docking, clip the point so it doesn't get dragged under the
      taskbar }
    if MouseOverDock = nil then begin
      R := GetDesktopArea;
      if Pos.X < R.Left then Pos.X := R.Left;
      if Pos.X > R.Right then Pos.X := R.Right;
      if Pos.Y < R.Top then Pos.Y := R.Top;
      if Pos.Y > R.Bottom then Pos.Y := R.Bottom;
    end;

    for I := 0 to NewDockedSizes.Count-1 do
      with PDockedSize(NewDockedSizes[I])^ do begin
        if Dock <> MouseOverDock then Continue;
        MoveRect := Bounds(Pos.X-MulDiv(Size.X-1, NPoint.X, DPoint.X),
          Pos.Y-MulDiv(Size.Y-1, NPoint.Y, DPoint.Y),
          Size.X, Size.Y);
        Break;
      end;

    { Make sure title bar (or at least part of the toolbar) is still accessible
      if it's dragged almost completely off the screen. This prevents the
      problem seen in Office 97 where you drag it offscreen so that only the
      border is visible, sometimes leaving you no way to move it back short of
      resetting the toolbar. }
    if MouseOverDock = nil then begin
      R := GetDesktopArea;
      InflateRect (R, -(GetBorderSize+4), -(GetBorderSize+4));
      if MoveRect.Bottom < R.Top then
        OffsetRect (MoveRect, 0, R.Top-MoveRect.Bottom);
      if MoveRect.Top > R.Bottom then
        OffsetRect (MoveRect, 0, R.Bottom-MoveRect.Top);
      if MoveRect.Right < R.Left then
        OffsetRect (MoveRect, R.Left-MoveRect.Right, 0);
      if MoveRect.Left > R.Right then
        OffsetRect (MoveRect, R.Right-MoveRect.Left, 0);
    end;

    { Update the dragging outline }
    DrawDraggingOutline (ScreenDC, @MoveRect, @OldMoveRect, MouseOverDock <> nil,
      OldMouseOverDock <> nil);
  end;
var
  Accept: Boolean;
  DT: TDockType;
  R: TRect;
  SaveShowHint: Boolean;
  Msg: TMsg;
  NewDockedSize: PDockedSize;
  I: Integer;
begin
  Accept := False;

  NPoint := Point(InitX, InitY);
  if DockedTo = nil then begin
    { Adjust for non-client area }
    NPoint := ClientToScreen(NPoint);
    Dec (NPoint.X, Left);
    Dec (NPoint.Y, Top);
  end;
  DPoint := Point(Width-1, Height-1);

  PreventDocking := GetKeyState(VK_CONTROL) < 0;

  { Set up potential sizes for each dock type }
  NewDockedSizes := TList.Create;
  try
    DT := GetDockTypeOf(DockedTo);
    SetRectEmpty (R);
    ArrangeControls (False, False, DT, nil, FFloatingRightX, @R.BottomRight);
    AddNCAreaToRect (R);
    New (NewDockedSize);
    try
      with NewDockedSize^ do begin
        Dock := nil;
        Size := Point(R.Right-R.Left, R.Bottom-R.Top);
      end;
      NewDockedSizes.Add (NewDockedSize);
    except
      Dispose (NewDockedSize);
      raise;
    end;
    with TForm(Owner) do
      for I := 0 to ComponentCount-1 do begin
        if not(Components[I] is TDock97) then Continue;
        New (NewDockedSize);
        try
          with NewDockedSize^ do begin
            Dock := TDock97(Components[I]);
            if Components[I] <> DockedTo then
              ArrangeControls (False, False, DT, TDock97(Components[I]), FFloatingRightX, @Size)
            else
              Size := Self.ClientRect.BottomRight;
          end;
          NewDockedSizes.Add (NewDockedSize);
        except
          Dispose (NewDockedSize);
          raise;
        end;
      end;

    { Before locking, make sure all pending paint messages are processed }
    ProcessPaintMessages;

    { This uses LockWindowUpdate to suppress all window updating so the
      dragging outlines doesn't sometimes get garbled. (This is safe, and in
      fact, is the main purpose of the LockWindowUpdate function)
      IMPORTANT! While debugging you might want to enable the 'TB97DisableLock'
      conditional define (see top of the source code). }
    {$IFNDEF TB97DisableLock}
    LockWindowUpdate (GetDesktopWindow);
    {$ENDIF}
    { Get a DC of the entire screen. Works around the window update lock
      by specifying DCX_LOCKWINDOWUPDATE. }
    ScreenDC := GetDCEx(GetDesktopWindow, 0,
      DCX_LOCKWINDOWUPDATE or DCX_CACHE or DCX_WINDOW);
    try
      { Hints must be disabled while dragging or it can mess up ScreenDC.
        Previous value is of Application.ShowHint is restored when done moving }
      SaveShowHint := Application.ShowHint;
      Application.ShowHint := False;
      try
        SetCapture (Handle);

        { Initialize }
        MouseOverDock := nil;
        SetRectEmpty (MoveRect);
        MouseMoved;

        { Stay in message loop until capture is lost. Capture is removed either
          by this procedure manually doing it, or by an outside influence (like
          a message box or menu popping up) }
        while GetCapture = Handle do begin
          case Integer(GetMessage(Msg, 0, 0, 0)) of
            -1: Break; { if GetMessage failed }
            0: begin
                 { Repost WM_QUIT messages }
                 PostQuitMessage (Msg.WParam);
                 Break;
               end;
          end;

          case Msg.Message of
            WM_KEYDOWN, WM_KEYUP:
              { Ignore all keystrokes while dragging. But process Ctrl }
              if (Msg.WParam = VK_CONTROL) and
                 (PreventDocking <> (Msg.Message = WM_KEYDOWN)) then begin
                PreventDocking := Msg.Message = WM_KEYDOWN;
                MouseMoved;
              end;
            WM_MOUSEMOVE:
              MouseMoved;
            WM_LBUTTONDOWN, WM_LBUTTONDBLCLK:
              { Make sure it doesn't begin another loop }
              Break;
            WM_LBUTTONUP: begin
                Accept := True;
                Break;
              end;
            WM_RBUTTONDOWN..WM_MBUTTONDBLCLK:
              { Ignore all other mouse up/down messages }
              ;
          else
            TranslateMessage (Msg);
            DispatchMessage (Msg);
          end;
        end;
      finally
        { Since it sometimes breaks out of the loop without capture being
          released }
        if GetCapture = Handle then
          ReleaseCapture;

        Application.ShowHint := SaveShowHint;
      end;
    finally
      { Hide dragging outline and release the DC }
      DrawDraggingOutline (ScreenDC, nil, @MoveRect, False, MouseOverDock <> nil);
      ReleaseDC (GetDesktopWindow, ScreenDC);

      { Release window update lock }
      {$IFNDEF TB97DisableLock}
      LockWindowUpdate (0);
      {$ENDIF}
    end;

    { Move to new position }
    if Accept then
      Dropped;
  finally
    for I := NewDockedSizes.Count-1 downto 0 do begin
      Dispose (PDockedSize(NewDockedSizes[I]));
      NewDockedSizes.Delete (I);
    end;
    NewDockedSizes.Free;
  end;
end;

function CompareNewSizes (const Item1, Item2, ExtraData: Pointer): Integer; far;
begin
  { Sorts in descending order }
  if ExtraData = nil then
    Result := TSmallPoint(Item2).X - TSmallPoint(Item1).X
  else
    Result := TSmallPoint(Item2).Y - TSmallPoint(Item1).Y;
end;

procedure TToolbar97.BeginSizing (const HitTestValue: Integer);
var
  NewSizes: TList; { List of valid new sizes. Items are casted into TSmallPoints }

  procedure BuildNewSizes (const YOrdering: Boolean);
  { Adds items to the NewSizes list. The list must be empty when this is called }
    function AddNCAreaToSize (const P: TPoint): TPoint;
    var
      R: TRect;
    begin
      with R do begin
        Top := 0;  Left := 0;
        BottomRight := P;
      end;
      AddNCAreaToRect (R);
      OffsetRect (R, -R.Left, -R.Top);
      Result := R.BottomRight;
    end;
  var
    DT: TDockType;
    Max, X, LastY, SkipTo: Integer;
    S, S2: TPoint;
  begin
    DT := GetDockTypeOf(DockedTo);
    ArrangeControls (False, False, DT, nil, 0, @S);
    S2 := AddNCAreaToSize(S);
    NewSizes.Add (Pointer(PointToSmallPoint(S2)));
    LastY := S.Y;
    Max := S.X;
    SkipTo := High(SkipTo);
    for X := Max-1 downto LeftMargin[True, dtNotDocked]+FBarWidth+RightMargin[dtNotDocked] do begin
      if X > SkipTo then Continue;
      ArrangeControls (False, False, DT, nil, X, @S);
      if X = S.X then begin
        if S.Y = LastY then
          NewSizes.Delete (NewSizes.Count-1);
        S2 := AddNCAreaToSize(S);
        if NewSizes.IndexOf(Pointer(PointToSmallPoint(S2))) = -1 then
          NewSizes.Add (Pointer(PointToSmallPoint(S2)));
        LastY := S.Y;
      end
      else
        SkipTo := S.X;
    end;
    ListSortEx (NewSizes, CompareNewSizes, Pointer(Longint(YOrdering)));
  end;

var
  DragRect, OrigDragRect: TRect;
  CurRightX: Integer;
  ScreenDC: HDC;
  DisableSensCheck, OpSide: Boolean;
  SizeSens: Integer;

  procedure MouseMoved;
  var
    Pos: TPoint;
    NCXDiff: Integer;
    NewOpSide: Boolean;
    OldDragRect: TRect;
    Reverse: Boolean;
    I: Integer;
    P: TSmallPoint;
  begin
    GetCursorPos (Pos);
    NCXDiff := ClientToScreen(Point(0, 0)).X - Left;
    Dec (Pos.X, Left);  Dec (Pos.Y, Top);
    if HitTestValue = HTLEFT then
      Pos.X := Width-Pos.X
    else
    if HitTestValue = HTTOP then
      Pos.Y := Height-Pos.Y;

    { Adjust Pos to make up for the "sizing sensitivity", as seen in Office 97 }
    if HitTestValue in [HTLEFT, HTRIGHT] then
      NewOpSide := Pos.X < Width
    else
      NewOpSide := Pos.Y < Height;
    if (not DisableSensCheck) or (OpSide <> NewOpSide) then begin
      DisableSensCheck := False;
      OpSide := NewOpSide;
      if HitTestValue in [HTLEFT, HTRIGHT] then begin
        if (Pos.X >= Width-SizeSens) and (Pos.X < Width+SizeSens) then
          Pos.X := Width;
      end
      else begin
        if (Pos.Y >= Height-SizeSens) and (Pos.Y < Height+SizeSens) then
          Pos.Y := Height;
      end;
    end;

    OldDragRect := DragRect;

    if HitTestValue in [HTLEFT, HTRIGHT] then
      Reverse := Pos.X > Width
    else
      Reverse := Pos.Y > Height;
    if not Reverse then
      I := NewSizes.Count-1
    else
      I := 0;
    while True do begin
      if (not Reverse and (I < 0)) or
         (Reverse and (I >= NewSizes.Count)) then
        Break;
      Pointer(P) := NewSizes[I];
      if HitTestValue in [HTLEFT, HTRIGHT] then begin
        if (not Reverse and ((I = NewSizes.Count-1) or (Pos.X >= P.X))) or
           (Reverse and ((I = 0) or (Pos.X < P.X))) then begin
          CurRightX := P.X - NCXDiff;
          if HitTestValue = HTRIGHT then
            DragRect.Right := DragRect.Left + P.X
          else
            DragRect.Left := DragRect.Right - P.X;
          DragRect.Bottom := DragRect.Top + P.Y;
          DisableSensCheck := not EqualRect(DragRect, OrigDragRect);
        end;
      end
      else begin
        if (not Reverse and ((I = NewSizes.Count-1) or (Pos.Y >= P.Y))) or
           (Reverse and ((I = 0) or (Pos.Y < P.Y))) then begin
          CurRightX := P.X - NCXDiff;
          if HitTestValue = HTBOTTOM then
            DragRect.Bottom := DragRect.Top + P.Y
          else
            DragRect.Top := DragRect.Bottom - P.Y;
          DragRect.Right := DragRect.Left + P.X;
          DisableSensCheck := not EqualRect(DragRect, OrigDragRect);
        end;
      end;
      if not Reverse then
        Dec (I)
      else
        Inc (I);
    end;

    { Update the dragging outline, only if changed }
    if not EqualRect(DragRect, OldDragRect) then
      DrawDraggingOutline (ScreenDC, @DragRect, @OldDragRect, False, False);
  end;
const
  MaxSizeSens = 12;
var
  Accept: Boolean;
  I, NewSize: Integer;
  S, N: TSmallPoint;
  SaveShowHint: Boolean;
  Msg: TMsg;
begin
  Accept := False;
  CurRightX := FFloatingRightX;
  DisableSensCheck := False;
  OpSide := False;

  NewSizes := TList.Create;
  try
    { Initialize }
    BuildNewSizes (HitTestValue in [HTTOP, HTBOTTOM]);
    SizeSens := MaxSizeSens;
    { Adjust sensitivity if it's too high }
    for I := 0 to NewSizes.Count-1 do begin
      Pointer(S) := NewSizes[I];
      if (S.X = Width) and (S.Y = Height) then begin
        if I > 0 then begin
          Pointer(N) := NewSizes[I-1];
          if HitTestValue in [HTLEFT, HTRIGHT] then
            NewSize := N.X - S.X - 1
          else
            NewSize := N.Y - S.Y - 1;
          if NewSize < SizeSens then SizeSens := NewSize;
        end;
        if I < NewSizes.Count-1 then begin
          Pointer(N) := NewSizes[I+1];
          if HitTestValue in [HTLEFT, HTRIGHT] then
            NewSize := S.X - N.X - 1
          else
            NewSize := S.Y - N.Y - 1;
          if NewSize < SizeSens then SizeSens := NewSize;
        end;
        Break;
      end;
    end;
    if SizeSens < 0 then SizeSens := 0;
    DragRect := GetVirtualBoundsRect;
    OrigDragRect := DragRect;

    { Before locking, make sure all pending paint messages are processed }
    ProcessPaintMessages;
    
    { This uses LockWindowUpdate to suppress all window updating so the
      dragging outlines doesn't sometimes get garbled. (This is safe, and in
      fact, is the main purpose of the LockWindowUpdate function)
      IMPORTANT! While debugging you might want to enable the 'TB97DisableLock'
      conditional define (see top of the source code). }
    {$IFNDEF TB97DisableLock}
    LockWindowUpdate (GetDesktopWindow);
    {$ENDIF}
    { Get a DC of the entire screen. Works around the window update lock
      by specifying DCX_LOCKWINDOWUPDATE. }
    ScreenDC := GetDCEx(GetDesktopWindow, 0,
      DCX_LOCKWINDOWUPDATE or DCX_CACHE or DCX_WINDOW);
    try
      { Hints must be disabled while dragging or it can mess up ScreenDC.
        Previous value is of Application.ShowHint is restored when done moving }
      SaveShowHint := Application.ShowHint;
      Application.ShowHint := False;
      try
        SetCapture (Handle);

        { Initialize }
        DrawDraggingOutline (ScreenDC, @DragRect, nil, False, False);

        { Stay in message loop until capture is lost. Capture is removed either
          by this procedure manually doing it, or by an outside influence (like
          a message box or menu popping up) }
        while GetCapture = Handle do begin
          case Integer(GetMessage(Msg, 0, 0, 0)) of
            -1: Break; { if GetMessage failed }
            0: begin
                 { Repost WM_QUIT messages }
                 PostQuitMessage (Msg.WParam);
                 Break;
               end;
          end;

          case Msg.Message of
            WM_KEYDOWN, WM_KEYUP:
              { Ignore all keystrokes while sizing }
              ;
            WM_MOUSEMOVE:
              MouseMoved;
            WM_LBUTTONDOWN, WM_LBUTTONDBLCLK:
              { Make sure it doesn't begin another loop }
              Break;
            WM_LBUTTONUP: begin
                Accept := True;
                Break;
              end;
            WM_RBUTTONDOWN..WM_MBUTTONDBLCLK:
              { Ignore all other mouse up/down messages }
              ;
          else
            TranslateMessage (Msg);
            DispatchMessage (Msg);
          end;
        end;
      finally
        { Since it sometimes breaks out of the loop without capture being
          released }
        if GetCapture = Handle then
          ReleaseCapture;

        Application.ShowHint := SaveShowHint;
      end;
    finally
      { Hide dragging outline and release the DC }
      DrawDraggingOutline (ScreenDC, nil, @DragRect, False, False);
      ReleaseDC (GetDesktopWindow, ScreenDC);

      { Release window update lock }
      {$IFNDEF TB97DisableLock}
      LockWindowUpdate (0);
      {$ENDIF}
    end;
  finally
    NewSizes.Free;
  end;

  if Accept then begin
    FFloatingRightX := CurRightX;
    FFloatingRect := DragRect;
    SetVirtualBoundsRect (FFloatingRect);
    AutoArrangeControls;

    { Make sure it doesn't go completely off the screen }
    MoveOnScreen (True);
  end;
end;

procedure TToolbar97.MouseDown (Button: TMouseButton; Shift: TShiftState; X, Y: Integer);

  function ControlExistsAtPos (const P: TPoint): Boolean;
  var
    I: Integer;
  begin
    Result := False;
    for I := 0 to ControlCount-1 do
      if not(Controls[I] is TToolbarSep97) and Controls[I].Visible and
         PtInRect(Controls[I].BoundsRect, P) then begin
        Result := True;
        Break;
      end;
  end;
begin
  inherited MouseDown (Button, Shift, X, Y);
  if (Button <> mbLeft) or
     { Only process if AllowDrag is True }
     ((DockedTo <> nil) and (not DockedTo.FAllowDrag)) or
     { Ignore message if user clicked on a child control that was probably
       disabled }
     ControlExistsAtPos(Point(X, Y)) then
    Exit;

  { Handle double click }
  if ssDouble in Shift then begin
    if DockedTo <> nil then
      DockedTo := nil
    else begin
      FDockRow := ForceDockAtTopRow;
      FDockPos := ForceDockAtLeftPos;
      DockedTo := DefaultDock;
    end;
    Exit;
  end;

  BeginMoving (X, Y);
end;

procedure TToolbar97.WMNCHitTest (var Message: TWMNCHitTest);
var
  P: TPoint;
begin
  inherited;
  if DockedTo <> nil then Exit;

  with Message do begin
    P := SmallPointToPoint(Pos);
    Dec (P.X, Left);  Dec (P.Y, Top);
    case Result of
      HTNOWHERE: begin
          if PtInRect(GetCaptionRect(Self, True, False), P) then begin
            Result := HTCLIENT;
            if FCloseButton and PtInRect(GetCloseButtonRect(Self, True), P) then
              Result := HTCLOSE;
          end
          else
          if (P.Y >= 0) and (P.Y <= GetBorderSize) then Result := HTTOP else
          if (P.Y < Height) and (P.Y >= Height-GetBorderSize-1) then Result := HTBOTTOM else
          if (P.X >= 0) and (P.X <= GetBorderSize) then Result := HTLEFT else
          if (P.X < Width) and (P.X >= Width-GetBorderSize-1) then Result := HTRIGHT;
        end;
    end;
  end;
end;

procedure TToolbar97.WMNCLButtonDown (var Message: TWMNCLButtonDown);
  procedure CloseButtonLoop;
  var
    Accept, NewCloseButtonDown: Boolean;
    P: TPoint;
    Msg: TMsg;
  begin
    Accept := False;

    CloseButtonDown := True;
    DrawNCArea (0, False, False, True);

    SetCapture (Handle);

    try
      while GetCapture = Handle do begin
        case Integer(GetMessage(Msg, 0, 0, 0)) of
          -1: Break; { if GetMessage failed }
          0: begin
               { Repost WM_QUIT messages }
               PostQuitMessage (Msg.WParam);
               Break;
             end;
        end;

        case Msg.Message of
          WM_KEYDOWN, WM_KEYUP:
            { Ignore all keystrokes while in a close button loop }
            ;
          WM_MOUSEMOVE: begin
              GetCursorPos (P);
              Dec (P.X, Left);  Dec (P.Y, Top);

              NewCloseButtonDown := PtInRect(GetCloseButtonRect(Self, True), P);
              if CloseButtonDown <> NewCloseButtonDown then begin
                CloseButtonDown := NewCloseButtonDown;
                DrawNCArea (0, False, False, True);
              end;
            end;
          WM_LBUTTONDOWN, WM_LBUTTONDBLCLK:
            { Make sure it doesn't begin another loop }
            Break;
          WM_LBUTTONUP: begin
              if CloseButtonDown then
                Accept := True;
              Break;
            end;
          WM_RBUTTONDOWN..WM_MBUTTONDBLCLK:
            { Ignore all other mouse up/down messages }
            ;
        else
          TranslateMessage (Msg);
          DispatchMessage (Msg);
        end;
      end;
    finally
      if GetCapture = Handle then
        ReleaseCapture;
      if CloseButtonDown <> False then begin
        CloseButtonDown := False;
        DrawNCArea (0, False, False, True);
      end;
    end;
    if Accept then begin
      { Hide the window after close button is pushed }
      Hide;
      if Assigned(FOnClose) then
        FOnClose (Self);
    end;
  end;
begin
  if DockedTo <> nil then begin
    inherited;
    Exit;
  end;

  case Message.HitTest of
    HTLEFT, HTRIGHT, HTTOP, HTBOTTOM:
      BeginSizing (Message.HitTest);
    HTCLOSE:
      CloseButtonLoop;
  else
    inherited;
  end;
end;

procedure TToolbar97.NewFormWindowProc (var Message: TMessage);
{ This procedure is only used on MDI parents or non-MDI forms }
begin
  case Message.Msg of
    WM_ACTIVATE, WM_SETFOCUS:
      { Prevent re-focus of toolbar
        Note to self: Must process WM_ACTIVATE too so that it correctly
        activates the form when it was deactivated }
      if GetParentToolbar97(MDIParentForm.ActiveControl) <> nil then
        Exit;
    WM_WINDOWPOSCHANGED:
      { This must be here so the toolbars reappear when the form is
        restored after being minimized }
      ShowHideFloatParents (MDIParentForm, Application.Active);
  end;
  with Message do
    Result := CallWindowProc(OldFormWindowProc, MDIParentForm.Handle,
      Msg, WParam, LParam);
end;

procedure TToolbar97.NewChildFormWindowProc (var Message: TMessage);
{ This procedure is only used on MDI child forms }
begin
  case Message.Msg of
    WM_WINDOWPOSCHANGED:
      { This must be here so the toolbars reappear when the form is
        restored after being minimized }
      ShowHideFloatParents (TForm(Owner), Application.Active);
  end;
  with Message do
    Result := CallWindowProc(OldChildFormWindowProc, TForm(Owner).Handle,
      Msg, WParam, LParam);
end;

function TToolbar97.NewMainWindowHook (var Message: TMessage): Boolean;
var
  T: TToolbar97;
begin
  Result := False;
  case Message.Msg of
    CM_ACTIVATE, CM_DEACTIVATE: begin
        { When application is being activated, make sure it doesn't try to
          reactivate a floating toolbar. If this isn't here the form may not
          appear when application is activated }
        if (Message.Msg = CM_ACTIVATE) and
           ControlIsChildOf(Screen.ActiveControl, MDIParentForm) then begin
          T := GetParentToolbar97(Screen.ActiveControl);
          if Assigned(T) and (T.DockedTo = nil) then
            { Activate the owner form instead }
            Windows.SetActiveWindow (MDIParentForm.Handle);
        end;
        { Hide or restore toolbars when application is deactivated or activated }
        ShowHideFloatParents (MDIParentForm, Message.Msg = CM_ACTIVATE);
        { Correct the color of the form's caption. }
        SendMessage (TForm(Owner).Handle, WM_NCACTIVATE, Ord((Message.Msg = CM_ACTIVATE) and
          (FindControl(GetActiveWindow) = MDIParentForm)), 0);
      end;
  end;
end;

{ TToolbar97 - property access methods }

procedure TToolbar97.SetCloseButton (Value: Boolean);
begin
  if FCloseButton <> Value then begin
    FCloseButton := Value;

    { Update the close button's visibility }
    DrawNCArea (0, False, True, True);
  end;
end;

procedure TToolbar97.SetDefaultDock (Value: TDock97);
begin
  if FDefaultDock <> Value then begin
    FDefaultDock := Value;
    if Assigned(Value) then
      Value.FreeNotification (Self);
  end;
end;

function TToolbar97.GetDockedTo: TDock97;
begin
  if not(Parent is TDock97) then
    Result := nil
  else
    Result := TDock97(Parent);
end;
procedure TToolbar97.SetDockedTo (Value: TDock97);
var
  OldDockedTo: TDock97;
  HiddenInced: Boolean;
begin
  OldDockedTo := DockedTo;

  if Assigned(FOnDockChanging) and (Value <> OldDockedTo) then
    FOnDockChanging (Self);

  if Assigned(Value) then
    Inc (Value.DisableArrangeToolbars);
  try
    { Before changing between docked and floating state (and vice-versa)
      or between docks, hide the toolbar. This prevents any flashing while
      it's being moved }
    HiddenInced := False;
    if not(csDesigning in ComponentState) and (Value <> OldDockedTo) and (Visible) then begin
      Inc (Hidden);
      HiddenInced := True;
      if Assigned(OldDockedTo) then
        { Need to disable arranging of current dock so it doesn't lose it's
          FDockRow/FDockPos it's going to set later }
        Inc (OldDockedTo.DisableArrangeToolbars);
      try
        Hide; {must Hide AFTER incing Hidden}
      finally
        if Assigned(OldDockedTo) then
          Dec (OldDockedTo.DisableArrangeToolbars);
      end;
    end;
    try
      if Value <> nil then begin
        { Must pre-arrange controls in new dock orientation before changing
          the Parent }
        if Parent <> nil then
          ArrangeControls (True, False, GetDockTypeOf(OldDockedTo),
            Value, FFloatingRightX, nil);
        if Parent <> Value then begin
          Inc (DisableArrangeControls);
          try
            Parent := Value;
          finally
            Dec (DisableArrangeControls);
          end;
        end;
        AutoArrangeControls;
      end
      else begin
        if IsRectEmpty(FFloatingRect) then begin
          FFloatingRect := GetVirtualBoundsRect;
          AddNCAreaToRect (FFloatingRect);
          OffsetRect (FFloatingRect, -FFloatingRect.Left, -FFloatingRect.Top);
        end;
        { Must pre-arrange controls in new dock orientation before changing
          the Parent }
        if Parent <> nil then
          ArrangeControls (True, False, GetDockTypeOf(OldDockedTo),
            Value, FFloatingRightX, nil);
        Inc (DisableArrangeControls);
        try
          if Parent <> FloatParent then
            Parent := FloatParent;
          SetVirtualBoundsRect (FFloatingRect);
        finally
          Dec (DisableArrangeControls);
        end;
        AutoArrangeControls;
      end;
    finally
      if HiddenInced then begin
        Dec (Hidden);
        Show;
      end;
    end;
  finally
    if Assigned(Value) then
      Dec (Value.DisableArrangeToolbars);
  end;
  if Assigned(Value) then
    Value.ArrangeToolbars;

  if Assigned(FOnDockChanged) and (Value <> OldDockedTo) then
    FOnDockChanged (Self);
end;

procedure TToolbar97.SetDockPos (Value: Integer);
begin
  FDockPos := Value;
  if DockedTo <> nil then
    DockedTo.ArrangeToolbars;
end;

procedure TToolbar97.SetDockRow (Value: Integer);
begin
  FDockRow := Value;
  if DockedTo <> nil then
    DockedTo.ArrangeToolbars;
end;


{ TToolbarSep97 - internal }

constructor TToolbarSep97.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);
  FSizeHorz := 6;
  FSizeVert := 6;
  ControlStyle := ControlStyle - [csOpaque, csCaptureMouse];
end;

procedure TToolbarSep97.SetParent (AParent: TWinControl);
begin
  if (AParent <> nil) and not(AParent is TToolbar97) then
    raise EInvalidOperation.Create(STB97SepParentNotAllowed);
  inherited SetParent (AParent);
end;

procedure TToolbarSep97.SetBlank (Value: Boolean);
begin
  if FBlank <> Value then begin
    FBlank := Value;
    Invalidate;
  end;
end;

procedure TToolbarSep97.SetSizeHorz (Value: TToolbarSepSize);
begin
  if FSizeHorz <> Value then begin
    FSizeHorz := Value;
    if Parent is TToolbar97 then
      TToolbar97(Parent).AutoArrangeControls;
  end;
end;

procedure TToolbarSep97.SetSizeVert (Value: TToolbarSepSize);
begin
  if FSizeVert <> Value then begin
    FSizeVert := Value;
    if Parent is TToolbar97 then
      TToolbar97(Parent).AutoArrangeControls;
  end;
end;

procedure TToolbarSep97.Paint;
var
  R: TRect;
  Z: Integer;
begin
  inherited Paint;
  if not(Parent is TToolbar97) then Exit;

  with Canvas do begin
    { Draw dotted border in design mode }
    if csDesigning in ComponentState then begin
      Pen.Style := psDot;
      Pen.Color := clBtnShadow;
      Brush.Style := bsClear;
      R := ClientRect;
      Rectangle (R.Left, R.Top, R.Right, R.Bottom);
      Pen.Style := psSolid;
    end;

    if not FBlank then
      if GetDockTypeOf(TToolbar97(Parent).DockedTo) <> dtLeftRight then begin
        Z := Width div 2;
        Pen.Color := clBtnShadow;
        MoveTo (Z-1, 0);  LineTo (Z-1, Height);
        Pen.Color := clBtnHighlight;
        MoveTo (Z, 0);  LineTo (Z, Height);
      end
      else begin
        Z := Height div 2;
        Pen.Color := clBtnShadow;
        MoveTo (0, Z-1);  LineTo (Width, Z-1);
        Pen.Color := clBtnHighlight;
        MoveTo (0, Z);  LineTo (Width, Z);
      end;
  end;
end;

procedure TToolbarSep97.MouseDown (Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  P: TPoint;
begin
  inherited MouseDown (Button, Shift, X, Y);

  { Relay the message to the parent toolbar }
  P := Parent.ScreenToClient(ClientToScreen(Point(X, Y)));
  TToolbar97(Parent).MouseDown (Button, Shift, P.X, P.Y);
end;


{ TToolbarButton97 - internal }

type
  TGlyphList = class(TImageList)
  private
    Used: TBits;
    FCount: Integer;
    function AllocateIndex: Integer;
  public
    constructor CreateSize (AWidth, AHeight: Integer);
    destructor Destroy; override; 
    function Add (Image, Mask: TBitmap): Integer;
    function AddMasked (Image: TBitmap; MaskColor: TColor): Integer;
    procedure Delete (Index: Integer);
    property Count: Integer read FCount;
  end;

  TGlyphCache = class
  private
    GlyphLists: TList;
  public
    constructor Create;
    destructor Destroy; override;
    function GetList(AWidth, AHeight: Integer): TGlyphList;
    procedure ReturnList(List: TGlyphList);
    function Empty: Boolean;
  end;

  TButtonGlyph = class
  private
    FOriginal: TBitmap;
    FGlyphList: TGlyphList;
    FIndexs: array[TButtonState97] of Integer;
    FTransparentColor: TColor;
    FNumGlyphs: TNumGlyphs97;
    FOnChange: TNotifyEvent;
    FOldDisabledStyle: Boolean;
    procedure GlyphChanged (Sender: TObject);
    procedure SetGlyph(Value: TBitmap);
    procedure SetNumGlyphs (Value: TNumGlyphs97);
    procedure Invalidate;
    function CreateButtonGlyph (State: TButtonState97): Integer;
    procedure DrawButtonGlyph (Canvas: TCanvas; const GlyphPos: TPoint;
      State: TButtonState97; Transparent: Boolean);
    procedure DrawButtonText (Canvas: TCanvas;
      const Caption: string; TextBounds: TRect;
      WordWrap: Boolean; State: TButtonState97);
    procedure CalcButtonLayout (Canvas: TCanvas; const Client: TRect;
      const Offset: TPoint; DrawGlyph, DrawCaption: Boolean;
      const Caption: string; WordWrap: Boolean;
      Layout: TButtonLayout; Margin, Spacing: Integer; DropArrow: Boolean;
      var GlyphPos, ArrowPos: TPoint; var TextBounds: TRect);
  public
    constructor Create;
    destructor Destroy; override;
    { returns the text rectangle }
    function Draw (Canvas: TCanvas; const Client: TRect; const Offset: TPoint;
      DrawGlyph, DrawCaption: Boolean; const Caption: string; WordWrap: Boolean;
      Layout: TButtonLayout; Margin, Spacing: Integer; DropArrow: Boolean;
      State: TButtonState97; Transparent: Boolean): TRect;
    procedure DrawButtonDropArrow (Canvas: TCanvas;
      const X, Y: Integer; State: TButtonState97);
    property Glyph: TBitmap read FOriginal write SetGlyph;
    property NumGlyphs: TNumGlyphs97 read FNumGlyphs write SetNumGlyphs;
    property OnChange: TNotifyEvent read FOnChange write FOnChange;
  end;


{ TGlyphList }

constructor TGlyphList.CreateSize(AWidth, AHeight: Integer);
begin
  inherited CreateSize (AWidth, AHeight);
  Used := TBits.Create;
end;

destructor TGlyphList.Destroy;
begin
  Used.Free;
  inherited Destroy;
end;

function TGlyphList.AllocateIndex: Integer;
begin
  Result := Used.OpenBit;
  if Result >= Used.Size then
  begin
    Result := inherited Add(nil, nil);
    Used.Size := Result + 1;
  end;
  Used[Result] := True;
end;

function TGlyphList.Add (Image, Mask: TBitmap): Integer;
begin
  Result := AllocateIndex;
  Replace (Result, Image, Mask);
  Inc (FCount);
end;

function TGlyphList.AddMasked (Image: TBitmap; MaskColor: TColor): Integer;
  procedure BugfreeReplaceMasked (Index: Integer; NewImage: TBitmap; MaskColor: TColor);
    procedure CheckImage (Image: TGraphic);
    begin
      if Image = nil then Exit;
      if (Image.Height < Height) or (Image.Width < Width) then
        raise EInvalidOperation.Create({$IFNDEF TB97Delphi3orHigher}LoadStr{$ENDIF}(SInvalidImageSize));
    end;
  var
    TempIndex: Integer;
    Image, Mask: TBitmap;
  begin
    if HandleAllocated then begin
      CheckImage(NewImage);
      TempIndex := inherited AddMasked(NewImage, MaskColor);
      if TempIndex <> -1 then
        try
          Image := TBitmap.Create;
          Mask := TBitmap.Create;
          try
            Image.Height := Height;
            Image.Width := Width;
            Mask.Monochrome := True;
            { ^ Prevents the "invisible glyph" problem when used with certain
                color schemes. }
            Mask.Height := Height;
            Mask.Width := Width;
            ImageList_Draw (Handle, TempIndex, Image.Canvas.Handle, 0, 0, ILD_NORMAL);
            ImageList_Draw (Handle, TempIndex, Mask.Canvas.Handle, 0, 0, ILD_MASK);
            if not ImageList_Replace(Handle, Index, Image.Handle, Mask.Handle) then
              raise EInvalidOperation.Create({$IFNDEF TB97Delphi3orHigher}LoadStr{$ENDIF}(SReplaceImage));
          finally
            Image.Free;
            Mask.Free;
          end;
        finally
          inherited Delete(TempIndex);
        end
      else
        raise EInvalidOperation.Create({$IFNDEF TB97Delphi3orHigher}LoadStr{$ENDIF}(SReplaceImage));
    end;
    Change;
  end;
begin
  Result := AllocateIndex;
  { This works two very serious bugs in the Delphi 2/BCB and Delphi 3
    implementations of the ReplaceMasked method. In the Delphi 2 and BCB
    versions of the ReplaceMasked method, it incorrectly uses ILD_NORMAL as
    the last parameter for the second ImageList_Draw call, in effect causing
    all white colors to be considered transparent also. And in the Delphi 2/3
    and BCB versions it doesn't set Monochrome to True on the Mask bitmap,
    causing the bitmaps to be invisible on certain color schemes. }
  BugfreeReplaceMasked (Result, Image, MaskColor);
  Inc (FCount);
end;

procedure TGlyphList.Delete (Index: Integer);
begin
  if Used[Index] then begin
    Dec(FCount);
    Used[Index] := False;
  end;
end;

{ TGlyphCache }

constructor TGlyphCache.Create;
begin
  inherited Create;
  GlyphLists := TList.Create;
end;

destructor TGlyphCache.Destroy;
begin
  GlyphLists.Free;
  inherited Destroy;
end;

function TGlyphCache.GetList(AWidth, AHeight: Integer): TGlyphList;
var
  I: Integer;
begin
  for I := GlyphLists.Count - 1 downto 0 do begin
    Result := GlyphLists[I];
    with Result do
      if (AWidth = Width) and (AHeight = Height) then Exit;
  end;
  Result := TGlyphList.CreateSize(AWidth, AHeight);
  GlyphLists.Add(Result);
end;

procedure TGlyphCache.ReturnList(List: TGlyphList);
begin
  if List = nil then Exit;
  if List.Count = 0 then begin
    GlyphLists.Remove(List);
    List.Free;
  end;
end;

function TGlyphCache.Empty: Boolean;
begin
  Result := GlyphLists.Count = 0;
end;

var
  GlyphCache: TGlyphCache = nil;
  Pattern: TBitmap = nil;
  PatternBtnFace, PatternBtnHighlight: TColor;
  ButtonCount: Integer = 0;

procedure CreateBrushPattern;
var
  X, Y: Integer;
begin
  PatternBtnFace := ColorToRGB(clBtnFace);
  PatternBtnHighlight := ColorToRGB(clBtnHighlight);
  Pattern := TBitmap.Create;
  Pattern.Width := 8;
  Pattern.Height := 8;
  with Pattern.Canvas do begin
    Brush.Style := bsSolid;
    Brush.Color := clBtnFace;
    FillRect(Rect(0, 0, Pattern.Width, Pattern.Height));
    for Y := 0 to 7 do
      for X := 0 to 7 do
        if (Y mod 2) = (X mod 2) then  { toggles between even/odd pixles }
          Pixels[X, Y] := clBtnHighlight;     { on even/odd rows }
  end;
end;


{ TButtonGlyph }

constructor TButtonGlyph.Create;
var
  I: TButtonState97;
begin
  inherited Create;
  FOriginal := TBitmap.Create;
  FOriginal.OnChange := GlyphChanged;
  FTransparentColor := clOlive;
  FNumGlyphs := 1;
  for I := Low(I) to High(I) do
    FIndexs[I] := -1;
  if GlyphCache = nil then
    GlyphCache := TGlyphCache.Create;
end;

destructor TButtonGlyph.Destroy;
begin
  FOriginal.Free;
  Invalidate;
  if Assigned(GlyphCache) and GlyphCache.Empty then begin
    GlyphCache.Free;
    GlyphCache := nil;
  end;
  inherited Destroy;
end;

procedure TButtonGlyph.Invalidate;
var
  I: TButtonState97;
begin
  for I := Low(I) to High(I) do begin
    if FIndexs[I] <> -1 then FGlyphList.Delete (FIndexs[I]);
    FIndexs[I] := -1;
  end;
  GlyphCache.ReturnList (FGlyphList);
  FGlyphList := nil;
end;

procedure TButtonGlyph.GlyphChanged(Sender: TObject);
begin
  if Sender = FOriginal then begin
    FTransparentColor := FOriginal.TransparentColor;
    Invalidate;
    if Assigned(FOnChange) then FOnChange (Self);
  end;
end;

procedure TButtonGlyph.SetGlyph(Value: TBitmap);
var
  Glyphs: Integer;
begin
  Invalidate;
  FOriginal.Assign(Value);
  if (Value <> nil) and (Value.Height > 0) then begin
    FTransparentColor := Value.TransparentColor;
    if Value.Width mod Value.Height = 0 then begin
      Glyphs := Value.Width div Value.Height;
      if Glyphs > High(TNumGlyphs97) then Glyphs := 1;
      SetNumGlyphs (Glyphs);
    end;
  end;
end;

procedure TButtonGlyph.SetNumGlyphs (Value: TNumGlyphs97);
begin
  if (Value <> FNumGlyphs) and (Value > 0) then begin
    Invalidate;
    FNumGlyphs := Value;
    GlyphChanged (Glyph);
  end;
end;

{$IFNDEF TB97Delphi3orHigher}
type
  PMaxLogPalette = ^TMaxLogPalette;
  TMaxLogPalette = packed record
    palVersion: Word;
    palNumEntries: Word;
    palPalEntry: array[Byte] of TPaletteEntry;
  end;
function CopyPalette (Palette: HPALETTE): HPALETTE;
var
  PaletteSize: Integer;
  LogPal: TMaxLogPalette;
begin
  Result := 0;
  if Palette = 0 then Exit;
  PaletteSize := 0;
  if GetObject(Palette, SizeOf(PaletteSize), @PaletteSize) = 0 then Exit;
  if PaletteSize = 0 then Exit;
  with LogPal do begin
    palVersion := $0300;
    palNumEntries := PaletteSize;
    GetPaletteEntries (Palette, 0, PaletteSize, palPalEntry);
  end;
  Result := CreatePalette(PLogPalette(@LogPal)^);
end;
{$ENDIF}

function TButtonGlyph.CreateButtonGlyph (State: TButtonState97): Integer;
const
  ROP_DSPDxax = $00E20746;
  ROP_PSDPxax = $00B8074A;
var
  TmpImage, DDB, MonoBmp: TBitmap;
  I: TButtonState97;
  IWidth, IHeight: Integer;
  IRect, ORect: TRect;
  DestDC: HDC;
const
  Add = 1;
begin
  if (State <> bsDisabled) and (Ord(State) >= NumGlyphs) then
    State := bsUp;
  Result := FIndexs[State];
  if Result <> -1 then Exit;
  if (FOriginal.Width or FOriginal.Height) = 0 then Exit;
  { + 1 is to make sure the highlight color on generated disabled glyphs
    doesn't get cut off }
  IWidth := FOriginal.Width div FNumGlyphs + Add;
  IHeight := FOriginal.Height + Add;
  IRect := Rect(0, 0, IWidth, IHeight);
  if FGlyphList = nil then begin
    if GlyphCache = nil then
      GlyphCache := TGlyphCache.Create;
    FGlyphList := GlyphCache.GetList(IWidth, IHeight);
  end;
  TmpImage := TBitmap.Create;
  try
    TmpImage.Width := IWidth;
    TmpImage.Height := IHeight;
    TmpImage.Canvas.Brush.Color := clBtnFace;
    TmpImage.Palette := CopyPalette(FOriginal.Palette);
    I := State;
    if Ord(I) >= NumGlyphs then I := bsUp;
    ORect := Bounds(Ord(I) * (IWidth-Add), 0, IWidth-Add, IHeight-Add);
    if State <> bsDisabled then begin
      with TmpImage.Canvas do begin
        { Because the CopyRect doesn't fill the whole rectangle }
        Brush.Color := FTransparentColor;
        FillRect (IRect);

        CopyRect (Rect(0, 0, IWidth-Add, IHeight-Add), FOriginal.Canvas, ORect);
      end;
      { Delphi 2 doesn't have a TransparentMode like Delphi 3 }
      {$IFDEF TB97Delphi3orHigher}
      if FOriginal.TransparentMode = tmFixed then
      {$ENDIF}
        FIndexs[State] := FGlyphList.AddMasked(TmpImage, FTransparentColor)
      {$IFDEF TB97Delphi3orHigher}
      else
        FIndexs[State] := FGlyphList.AddMasked(TmpImage, clDefault);
      {$ENDIF}
    end
    else begin
      MonoBmp := TBitmap.Create;
      try
        DDB := TBitmap.Create;
        try
          DDB.Assign (FOriginal);
          {$IFDEF TB97Delphi3orHigher}
          DDB.HandleType := bmDDB;
          {$ENDIF}
          if NumGlyphs > 1 then
            with TmpImage.Canvas do begin
              { Because the CopyRect doesn't fill the whole rectangle }
              Brush.Color := FTransparentColor;
              FillRect (IRect);

              CopyRect (Rect(0, 0, IWidth-Add, IHeight-Add), DDB.Canvas, ORect);

              MonoBmp.Monochrome := True;
              MonoBmp.Width := IWidth;
              MonoBmp.Height := IHeight;

              { Convert white to clBtnHighlight }
              DDB.Canvas.Brush.Color := clWhite;
              BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight,
                DDB.Canvas.Handle, ORect.Left, ORect.Top, SRCCOPY);
              Brush.Color := clBtnHighlight;
              DestDC := Handle;
              SetTextColor (DestDC, clBlack);
              SetBkColor (DestDC, clWhite);
              BitBlt (DestDC, 0, 0, IWidth, IHeight,
                MonoBmp.Canvas.Handle, 0, 0, ROP_DSPDxax);

              { Convert gray to clBtnShadow }
              DDB.Canvas.Brush.Color := clGray;
              BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight,
                DDB.Canvas.Handle, ORect.Left, ORect.Top, SRCCOPY);
              Brush.Color := clBtnShadow;
              DestDC := Handle;
              SetTextColor (DestDC, clBlack);
              SetBkColor (DestDC, clWhite);
              BitBlt (DestDC, 0, 0, IWidth, IHeight,
                MonoBmp.Canvas.Handle, 0, 0, ROP_DSPDxax);

              { Generate the transparent mask in MonoBmp. The reason why
                it doesn't just use a mask color is because the mask needs
                to be of the glyph -before- the clBtnHighlight/Shadow were
                translated }
              DDB.Canvas.Brush.Color := FTransparentColor;
              BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight,
                DDB.Canvas.Handle, ORect.Left, ORect.Top, SRCCOPY);

              FIndexs[State] := FGlyphList.Add(TmpImage, MonoBmp);
            end
          else begin
            { Create a disabled version }
            if FOldDisabledStyle then begin
              { "Old" TSpeedButton style }
              with MonoBmp do begin
                Assign (FOriginal);
                {$IFDEF TB97Delphi3orHigher}
                HandleType := bmDDB;
                {$ENDIF}
                Canvas.Brush.Color := clBlack;
                if Monochrome then begin
                  Canvas.Font.Color := clWhite;
                  Monochrome := False;
                  Canvas.Brush.Color := clWhite;
                end;
                Monochrome := True;
              end;
              with TmpImage.Canvas do begin
                Brush.Color := clBtnFace;
                FillRect (IRect);
                Brush.Color := clBtnHighlight;
                SetTextColor (Handle, clBlack);
                SetBkColor (Handle, clWhite);
                BitBlt (Handle, 1, 1, IWidth-Add, IHeight-Add,
                  MonoBmp.Canvas.Handle, 0, 0, ROP_DSPDxax);
                Brush.Color := clBtnShadow;
                SetTextColor (Handle, clBlack);
                SetBkColor (Handle, clWhite);
                BitBlt (Handle, 0, 0, IWidth-Add, IHeight-Add,
                  MonoBmp.Canvas.Handle, 0, 0, ROP_DSPDxax);
              end;
            end
            else begin
              { The new Office 97 / MFC look }
              MonoBmp.Monochrome := True;
              MonoBmp.Width := IWidth;
              MonoBmp.Height := IHeight;

              with TmpImage.Canvas do begin
                CopyRect (Rect(0, 0, IWidth-Add, IHeight-Add), DDB.Canvas, ORect);
                { Generate the mask in MonoBmp. Mask FTransparentColor }
                SetBkColor (Handle, ColorToRGB(FTransparentColor));
                BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight, Handle,
                  ORect.Left, ORect.Top, SRCCOPY);
                { and clWhite }
                SetBkColor (Handle, clWhite);
                BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight, Handle,
                  ORect.Left, ORect.Top, SRCPAINT);
                { and clSilver }
                SetBkColor (Handle, clSilver);
                BitBlt (MonoBmp.Canvas.Handle, 0, 0, IWidth, IHeight, Handle,
                  ORect.Left, ORect.Top, SRCPAINT);

                Brush.Color := clBtnFace;
                FillRect (IRect);
                Brush.Color := clBtnHighlight;
                SetTextColor (Handle, clBlack);
                SetBkColor (Handle, clWhite);
                BitBlt (Handle, 1, 1, IWidth-1, IHeight-1,
                  MonoBmp.Canvas.Handle, 0, 0, ROP_PSDPxax);
                Brush.Color := clBtnShadow;
                SetTextColor (Handle, clBlack);
                SetBkColor (Handle, clWhite);
                BitBlt (Handle, 0, 0, IWidth, IHeight,
                  MonoBmp.Canvas.Handle, 0, 0, ROP_PSDPxax);
              end;
            end;
            FIndexs[State] := FGlyphList.AddMasked(TmpImage, clBtnFace);
          end;
        finally
          DDB.Free;
        end;
      finally
        MonoBmp.Free;
      end;
    end;
  finally
    TmpImage.Free;
  end;
  Result := FIndexs[State];
  FOriginal.Dormant;
end;

procedure TButtonGlyph.DrawButtonGlyph(Canvas: TCanvas; const GlyphPos: TPoint;
  State: TButtonState97; Transparent: Boolean);
var
  Index: Integer;
begin
  if FOriginal = nil then Exit;
  if (FOriginal.Width = 0) or (FOriginal.Height = 0) then Exit;
  Index := CreateButtonGlyph(State);
  if Transparent or (State = bsExclusive) then
    ImageList_DrawEx(FGlyphList.Handle, Index, Canvas.Handle,
      GlyphPos.X, GlyphPos.Y, 0, 0, clNone, clNone, ILD_Transparent)
  else
    ImageList_DrawEx(FGlyphList.Handle, Index, Canvas.Handle,
      GlyphPos.X, GlyphPos.Y, 0, 0, ColorToRGB(clBtnFace), clNone, ILD_Normal);
end;

procedure TButtonGlyph.DrawButtonText(Canvas: TCanvas;
  const Caption: string; TextBounds: TRect;
  WordWrap: Boolean; State: TButtonState97);
var
  Format: UINT;
begin
  Format := DT_CENTER or DT_VCENTER;
  if not WordWrap then
    Format := Format or DT_SINGLELINE
  else
    Format := Format or DT_WORDBREAK;
  with Canvas do begin
    Brush.Style := bsClear;
    if State = bsDisabled then begin
      OffsetRect (TextBounds, 1, 1);
      Font.Color := clBtnHighlight;
      DrawText (Handle, PChar(Caption), Length(Caption), TextBounds, Format);
      OffsetRect (TextBounds, -1, -1);
      Font.Color := clBtnShadow;
      DrawText (Handle, PChar(Caption), Length(Caption), TextBounds, Format);
    end
    else
      DrawText (Handle, PChar(Caption), Length(Caption), TextBounds, Format);
  end;
end;

procedure TButtonGlyph.DrawButtonDropArrow (Canvas: TCanvas;
  const X, Y: Integer; State: TButtonState97);
begin
  with Canvas do begin
    if State = bsDisabled then begin
      Pen.Color := clBtnHighlight;
      Brush.Color := clBtnHighlight;
      Polygon ([Point(X+5, Y+1), Point(X+9, Y+1), Point(X+7, Y+3)]);
      Pen.Color := clBtnShadow;
      Brush.Color := clBtnShadow;
      Polygon ([Point(X+4, Y), Point(X+8, Y), Point(X+6, Y+2)]);
    end
    else begin
      Pen.Color := Font.Color;
      Brush.Color := Font.Color;
      Polygon ([Point(X+4, Y), Point(X+8, Y), Point(X+6, Y+2)]);
    end;
  end;
end;

procedure TButtonGlyph.CalcButtonLayout(Canvas: TCanvas; const Client: TRect;
  const Offset: TPoint; DrawGlyph, DrawCaption: Boolean;
  const Caption: string; WordWrap: Boolean;
  Layout: TButtonLayout; Margin, Spacing: Integer; DropArrow: Boolean;
  var GlyphPos, ArrowPos: TPoint; var TextBounds: TRect);
var
  TextPos: TPoint;
  ClientSize, GlyphSize, TextSize, ArrowSize: TPoint;
  TotalSize: TPoint;
  Format: UINT;
  Margin1, Spacing1: Integer;
  LayoutLeftOrRight: Boolean;
begin
  { calculate the item sizes }
  ClientSize := Point(Client.Right-Client.Left, Client.Bottom-Client.Top);

  if DrawGlyph and (FOriginal <> nil) then
    GlyphSize := Point(FOriginal.Width div FNumGlyphs, FOriginal.Height)
  else
    GlyphSize := Point(0, 0);

  if DropArrow then
    ArrowSize := Point(9, 3)
  else
    ArrowSize := Point(0, 0);

  LayoutLeftOrRight := (Layout = blGlyphLeft) or (Layout = blGlyphRight);
  if not LayoutLeftOrRight and ((GlyphSize.X = 0) or (GlyphSize.Y = 0)) then begin
    Layout := blGlyphLeft;
    LayoutLeftOrRight := True;
  end;

  if DrawCaption and (Length(Caption) <> 0) then begin
    TextBounds := Rect(0, 0, Client.Right-Client.Left, 0);
    if LayoutLeftOrRight then
      Dec (TextBounds.Right, ArrowSize.X);
    Format := DT_CALCRECT;
    if WordWrap then begin
      Format := Format or DT_WORDBREAK;
      Margin1 := 4;
      if LayoutLeftOrRight and (GlyphSize.X <> 0) and (GlyphSize.Y <> 0) then begin
        if Spacing = -1 then
          Spacing1 := 4
        else
          Spacing1 := Spacing;
        Dec (TextBounds.Right, GlyphSize.X + Spacing1);
        if Margin <> -1 then
          Margin1 := Margin
        else
        if Spacing <> -1 then
          Margin1 := Spacing;
      end;
      Dec (TextBounds.Right, Margin1 * 2);
    end;
    DrawText (Canvas.Handle, PChar(Caption), Length(Caption), TextBounds, Format);
    TextSize := Point(TextBounds.Right - TextBounds.Left, TextBounds.Bottom -
      TextBounds.Top);
  end
  else begin
    TextBounds := Rect(0, 0, 0, 0);
    TextSize := Point(0,0);
  end;

  { If the layout has the glyph on the right or the left, then both the
    text and the glyph are centered vertically.  If the glyph is on the top
    or the bottom, then both the text and the glyph are centered horizontally.}
  if LayoutLeftOrRight then begin
    GlyphPos.Y := (ClientSize.Y - GlyphSize.Y + 1) div 2;
    TextPos.Y := (ClientSize.Y - TextSize.Y + 1) div 2;
  end
  else begin
    GlyphPos.X := (ClientSize.X - GlyphSize.X - ArrowSize.X + 1) div 2;
    TextPos.X := (ClientSize.X - TextSize.X + 1) div 2;
    if (GlyphSize.X = 0) or (GlyphSize.Y = 0) then
      ArrowPos.X := TextPos.X + TextSize.X
    else
      ArrowPos.X := GlyphPos.X + GlyphSize.X;
  end;

  { if there is no text or no bitmap, then Spacing is irrelevant }
  if (TextSize.X = 0) or (TextSize.Y = 0) or
     (GlyphSize.X = 0) or (GlyphSize.Y = 0) then
    Spacing := 0;

  { adjust Margin and Spacing }
  if Margin = -1 then begin
    if Spacing = -1 then begin
      TotalSize := Point(GlyphSize.X + TextSize.X + ArrowSize.X,
        GlyphSize.Y + TextSize.Y);
      if LayoutLeftOrRight then
        Margin := (ClientSize.X - TotalSize.X) div 3
      else
        Margin := (ClientSize.Y - TotalSize.Y) div 3;
      Spacing := Margin;
    end
    else begin
      TotalSize := Point(GlyphSize.X + Spacing + TextSize.X + ArrowSize.X,
        GlyphSize.Y + Spacing + TextSize.Y);
      if LayoutLeftOrRight then
        Margin := (ClientSize.X - TotalSize.X + 1) div 2
      else
        Margin := (ClientSize.Y - TotalSize.Y + 1) div 2;
    end;
  end
  else begin
    if Spacing = -1 then begin
      TotalSize := Point(ClientSize.X - (Margin + GlyphSize.X + ArrowSize.X),
        ClientSize.Y - (Margin + GlyphSize.Y));
      if LayoutLeftOrRight then
        Spacing := (TotalSize.X - TextSize.X) div 2
      else
        Spacing := (TotalSize.Y - TextSize.Y) div 2;
    end;
  end;

  case Layout of
    blGlyphLeft: begin
        GlyphPos.X := Margin;
        TextPos.X := GlyphPos.X + GlyphSize.X + Spacing;
        ArrowPos.X := TextPos.X + TextSize.X;
      end;
    blGlyphRight: begin
        ArrowPos.X := ClientSize.X - Margin - ArrowSize.X;
        GlyphPos.X := ArrowPos.X - GlyphSize.X;
        TextPos.X := GlyphPos.X - Spacing - TextSize.X;
      end;
    blGlyphTop: begin
        GlyphPos.Y := Margin;
        TextPos.Y := GlyphPos.Y + GlyphSize.Y + Spacing;
      end;
    blGlyphBottom: begin
        GlyphPos.Y := ClientSize.Y - Margin - GlyphSize.Y;
        TextPos.Y := GlyphPos.Y - Spacing - TextSize.Y;
      end;
  end;
  if (GlyphSize.X = 0) or (GlyphSize.Y = 0) then
    ArrowPos.Y := TextPos.Y + (TextSize.Y - ArrowSize.Y) div 2
  else
    ArrowPos.Y := GlyphPos.Y + (GlyphSize.Y - ArrowSize.Y) div 2;

  { fixup the result variables }
  with GlyphPos do begin
    Inc (X, Client.Left + Offset.X);
    Inc (Y, Client.Top + Offset.Y);
  end;
  with ArrowPos do begin
    Inc (X, Client.Left + Offset.X);
    Inc (Y, Client.Top + Offset.Y);
  end;
  OffsetRect (TextBounds, TextPos.X + Client.Left + Offset.X,
    TextPos.Y + Client.Top + Offset.X);
end;

function TButtonGlyph.Draw (Canvas: TCanvas; const Client: TRect;
  const Offset: TPoint; DrawGlyph, DrawCaption: Boolean; const Caption: string;
  WordWrap: Boolean; Layout: TButtonLayout; Margin, Spacing: Integer;
  DropArrow: Boolean; State: TButtonState97; Transparent: Boolean): TRect;
var
  GlyphPos, ArrowPos: TPoint;
begin
  CalcButtonLayout (Canvas, Client, Offset, DrawGlyph, DrawCaption, Caption,
    WordWrap, Layout, Margin, Spacing, DropArrow, GlyphPos, ArrowPos, Result);
  if DrawGlyph then
    DrawButtonGlyph (Canvas, GlyphPos, State, Transparent);
  if DrawCaption then
    DrawButtonText (Canvas, Caption, Result, WordWrap, State);
  if DropArrow then
    DrawButtonDropArrow (Canvas, ArrowPos.X, ArrowPos.Y, State);
end;

{ TToolbarButton97 }

constructor TToolbarButton97.Create (AOwner: TComponent);
begin
  inherited Create(AOwner);
  SetBounds (Left, Top, 23, 22);
  ControlStyle := [csCaptureMouse, csDoubleClicks, csOpaque];
  FGlyph := TButtonGlyph.Create;
  TButtonGlyph(FGlyph).OnChange := GlyphChanged;
  ParentFont := True;
  FFlat := True;
  FOpaque := True;
  FSpacing := 4;
  FMargin := -1;
  FLayout := blGlyphLeft;
  FDropdownArrow := True;
  Inc(ButtonCount);
end;

destructor TToolbarButton97.Destroy;
begin
  if ButtonMouseInControl = Self then begin
    ButtonMouseTimer.Enabled := False;
    ButtonMouseInControl := nil;
  end;
  TButtonGlyph(FGlyph).Free;
  Dec(ButtonCount);
  if ButtonCount = 0 then begin
    Pattern.Free;
    Pattern := nil;
  end;
  inherited Destroy;
end;

procedure TToolbarButton97.Paint;
const
  EdgeStyles: array[Boolean, Boolean] of UINT = (
    (EDGE_RAISED, EDGE_SUNKEN),
    (BDR_RAISEDINNER, BDR_SUNKENOUTER));
  FlagStyles: array[Boolean] of UINT = (BF_RECT or BF_SOFT or BF_MIDDLE, BF_RECT);
var
  Bmp: TBitmap;
  DrawCanvas: TCanvas;
  PaintRect, R: TRect;
  Offset: TPoint;
begin
  if FOpaque or not FFlat then
    Bmp := TBitmap.Create
  else
    Bmp := nil;
  try
    if FOpaque or not FFlat then begin
      Bmp.Width := Width;
      Bmp.Height := Height;
      DrawCanvas := Bmp.Canvas;
      with DrawCanvas do begin
        Brush.Color := Self.Color;
        FillRect (ClientRect);
      end;
    end
    else
      DrawCanvas := Canvas;
    if not Enabled then begin
      FState := bsDisabled;
      FMouseIsDown := False;
    end
    else
    if FState = bsDisabled then
      if FDown and (GroupIndex <> 0) then
        FState := bsExclusive
      else
        FState := bsUp;
    DrawCanvas.Font := Self.Font;
    PaintRect := Rect(0, 0, Width, Height);

    if (not FFlat) or (FState in [bsDown, bsExclusive]) or
       (FMouseInControl and (FState <> bsDisabled)) or
       (csDesigning in ComponentState) then begin
      if DropdownCombo and Assigned(DropDownMenu) then begin
        R := PaintRect;
        R.Left := R.Right - DropdownComboWidth;
        Dec (R.Right, 2);
        DrawEdge (DrawCanvas.Handle, R,
          EdgeStyles[FFlat, (FState in [bsDown, bsExclusive]) and FMenuIsDown],
          FlagStyles[FFlat]);
        Dec (PaintRect.Right, DropdownComboWidth);
      end;
      DrawEdge (DrawCanvas.Handle, PaintRect,
        EdgeStyles[FFlat, (FState in [bsDown, bsExclusive]) and (not(DropdownCombo and Assigned(DropDownMenu)) or not FMenuIsDown)],
        FlagStyles[FFlat]);
    end
    else
      if DropdownCombo and Assigned(DropDownMenu) then
        Dec (PaintRect.Right, DropdownComboWidth);
    if FFlat then
      InflateRect (PaintRect, -1, -1)
    else
      InflateRect (PaintRect, -2, -2);

    if (FState in [bsDown, bsExclusive]) and (not(DropdownCombo and Assigned(DropDownMenu)) or not FMenuIsDown) then begin
      if (FState = bsExclusive) and (not FFlat or not FMouseInControl) then begin
        if Pattern = nil then CreateBrushPattern;
        DrawCanvas.Brush.Bitmap := Pattern;
        DrawCanvas.FillRect(PaintRect);
      end;
      Offset.X := 1;
      Offset.Y := 1;
    end
    else begin
      Offset.X := 0;
      Offset.Y := 0;
    end;

    TButtonGlyph(FGlyph).Draw (DrawCanvas, PaintRect, Offset,
      FDisplayMode <> dmTextOnly, FDisplayMode <> dmGlyphOnly,
      Caption, FWordWrap, FLayout, FMargin, FSpacing,
      FDropdownArrow and not FDropdownCombo and Assigned(FDropdownMenu),
      FState, True);
    if FDropdownCombo and Assigned(FDropdownMenu) then
      TButtonGlyph(FGlyph).DrawButtonDropArrow (DrawCanvas, Width-DropdownComboWidth-2,
        Height div 2 - 1, FState);

    if FOpaque or not FFlat then
      Canvas.Draw (0, 0, Bmp);
  finally
    if FOpaque or not FFlat then
      Bmp.Free;
  end;
end;

procedure TToolbarButton97.UpdateTracking;
var
  P: TPoint;
begin
  if Enabled then begin
    GetCursorPos (P);
    { Use FindDragTarget instead of PtInRect since we want to check based on
      the Z order }
    FMouseInControl := not (FindDragTarget(P, True) = Self);
    if FMouseInControl then
      MouseLeft
    else
      MouseEntered;
  end;
end;

procedure TToolbarButton97.Loaded;
var
  State: TButtonState97;
begin
  inherited Loaded;
  if Enabled then
    State := bsUp
  else
    State := bsDisabled;
  TButtonGlyph(FGlyph).CreateButtonGlyph (State);
end;

procedure TToolbarButton97.Notification (AComponent: TComponent; Operation: TOperation);
begin
  inherited Notification (AComponent, Operation);
  if (AComponent = FDropdownMenu) and (Operation = opRemove) then
    FDropdownMenu := nil;
end;

procedure TToolbarButton97.MouseDown (Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  inherited MouseDown (Button, Shift, X, Y);
  if Enabled then begin
    if Button = mbLeft then begin
      { We know mouse has to be over the control if the mouse went down. }
      if not FMouseInControl then begin
        { Doesn't call MouseEntered since the redrawing it does is unnecessary
          here }
        FMouseInControl := True;
        if Assigned(FOnMouseEnter) then
          FOnMouseEnter (Self);
      end;
      if not FDropdownCombo then
        FMenuIsDown := Assigned(FDropdownMenu)
      else
        FMenuIsDown := Assigned(FDropdownMenu) and (X >= Width-DropdownComboWidth);
      try
        if not FDown then begin
          FState := bsDown;
          Redraw (True);
        end
        else
          if FAllowAllUp then
            Redraw (True);
        if not FMenuIsDown then
          FMouseIsDown := True
        else
          Click;
      finally
        FMenuIsDown := False;
      end;
    end
    else
      MouseEntered;
  end;
end;

procedure TToolbarButton97.MouseMove (Shift: TShiftState; X, Y: Integer);
var
  P: TPoint;
  NewState: TButtonState97;
  ActiveForm, ParentForm: {$IFDEF TB97Delphi3orHigher} TCustomForm {$ELSE} TForm {$ENDIF};
  ParentFormActive: Boolean;
begin
  inherited MouseMove (Shift, X, Y);

  { Check if mouse just entered the control. It works better to check this
    in MouseMove rather than using CM_MOUSEENTER, since the VCL doesn't send
    a CM_MOUSEENTER in all cases
    Use FindDragTarget instead of PtInRect since we want to check based on
    the Z order }
  P := ClientToScreen(Point(X, Y));
  if (ButtonMouseInControl <> Self) and (FindDragTarget(P, True) = Self) then begin
    if Assigned(ButtonMouseInControl) then
      ButtonMouseInControl.MouseLeft;
    { Like Office 97, only draw the active borders when the application and the
      parent form is active }
    if Application.Active then begin
      ParentFormActive := True;
      ActiveForm := GetActiveForm;
      if Assigned(ActiveForm) then begin
        ParentForm := GetParentForm(Self);
        if Assigned(ParentForm) then begin
          if ParentForm is TForm then
            ParentForm := GetMDIParent(TForm(ParentForm));
          ParentFormActive := ParentForm = ActiveForm;
        end;
      end;
      if ParentFormActive then begin
        ButtonMouseInControl := Self;
        ButtonMouseTimer.OnTimer := ButtonMouseTimerHandler;
        ButtonMouseTimer.Enabled := True;
        MouseEntered;
      end;
    end;
  end;

  if FMouseIsDown then begin
    if FDown then
      NewState := bsExclusive
    else begin
      if (X >= 0) and
         (X < ClientWidth-(DropdownComboWidth * Ord(FDropdownCombo and Assigned(FDropdownMenu)))) and
         (Y >= 0) and (Y < ClientHeight) then
        NewState := bsDown
      else
        NewState := bsUp;
    end;
    if NewState <> FState then begin
      FState := NewState;
      Redraw (True);
    end;
  end;
end;

procedure TToolbarButton97.MouseUp (Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
var
  DoClick: Boolean;
begin
  { Remove active border when right button is clicked }
  if (Button = mbRight) and Enabled then begin
    FMouseIsDown := False;
    MouseLeft;
  end;
  inherited MouseUp (Button, Shift, X, Y);
  if (Button = mbLeft) and FMouseIsDown then begin
    FMouseIsDown := False;
    DoClick := (X >= 0) and
      (X < ClientWidth-(DropdownComboWidth * Ord(FDropdownCombo and Assigned(FDropdownMenu)))) and
      (Y >= 0) and (Y < ClientHeight);
    if FGroupIndex <> 0 then begin
      if DoClick then
        SetDown (not FDown)
      else begin
        if FDown then
          FState := bsExclusive;
      end;
    end;
    if DoClick then 
      Click
    else
      UpdateTracking;
  end;
end;

procedure TToolbarButton97.Click;
var
  SaveAlignment: TPopupAlignment;
  PopupPoint: TPoint;
  RepostList: TList; {pointers to TMsg's}
  Msg: TMsg;
  Repost: Boolean;
  I: Integer;
  P: TPoint;
begin
  FInClick := True;
  try
    if FState in [bsUp, bsMouseIn] then begin
      FState := bsDown;
      Redraw (True);
    end;

    { Stop tracking }
    MouseLeft;
    if (DropdownMenu = nil) or (FDropdownCombo and not FMenuIsDown) then
      inherited Click
    else begin
      { It must release its capture before displaying the popup menu since
        this control uses csCaptureMouse. If it doesn't, the VCL seems to
        get confused and think the mouse is still captured even after the
        popup menu is displayed, causing mouse problems after the menu is
        dismissed. }
      MouseCapture := False;
      try
        SaveAlignment := DropdownMenu.Alignment;
        try
          DropdownMenu.Alignment := paLeft;
          PopupPoint := Point(0, Height);
          if (Parent is TToolbar97) and
             (GetDockTypeOf(TToolbar97(Parent).DockedTo) = dtLeftRight) then begin
            { Drop out right or left side }
            if TToolbar97(Parent).DockedTo.Position = dpLeft then
              PopupPoint := Point(Width, 0)
            else begin
              PopupPoint := Point(0, 0);
              DropdownMenu.Alignment := paRight;
            end;
          end;
          PopupPoint := ClientToScreen(PopupPoint);
          DropdownMenu.PopupComponent := Self;
          DropdownMenu.Popup (PopupPoint.X, PopupPoint.Y);
        finally
          DropdownMenu.Alignment := SaveAlignment;
        end;
      finally
        { To prevent a mouse click from redisplaying the menu, filter all
          mouse up/down messages, and repost the ones that don't need
          removing. This is sort of bulky, but it's the only way I could
          find that works perfectly and like Office 97. }
        RepostList := TList.Create;
        try
          while PeekMessage(Msg, 0, WM_LBUTTONDOWN, WM_MBUTTONDBLCLK,
             PM_REMOVE or PM_NOYIELD) do
             { ^ The WM_LBUTTONDOWN to WM_MBUTTONDBLCLK range encompasses all
               of the DOWN and DBLCLK messages for the three buttons }
            with Msg do begin
              Repost := True;
              case Message of
                WM_LBUTTONDOWN, WM_LBUTTONDBLCLK,
                WM_RBUTTONDOWN, WM_RBUTTONDBLCLK,
                WM_MBUTTONDOWN, WM_MBUTTONDBLCLK: begin
                    P := SmallPointToPoint(TSmallPoint(lParam));
                    Windows.ClientToScreen (hwnd, P);
                    if FindDragTarget(P, True) = Self then
                      Repost := False;
                  end;
              end;
              if Repost then begin
                RepostList.Add (AllocMem(SizeOf(TMsg)));
                PMsg(RepostList[RepostList.Count-1])^ := Msg;
              end;
            end;
        finally
          for I := 0 to RepostList.Count-1 do begin
            with PMsg(RepostList[I])^ do
              PostMessage (hwnd, message, wParam, lParam);
            FreeMem (RepostList[I]);
          end;
          RepostList.Free;
        end;
      end;
    end;
  finally
    FInClick := False;
    if FState = bsDown then
      FState := bsUp;
    { Need to check if it's destroying in case the OnClick handler freed
      the button. If it doesn't check this here, it can sometimes cause an
      access violation }
    if not(csDestroying in ComponentState) then begin
      Redraw (True);
      UpdateTracking;
    end;
  end;
end;

function TToolbarButton97.GetPalette: HPALETTE;
begin
  Result := Glyph.Palette;
end;

function TToolbarButton97.GetGlyph: TBitmap;
begin
  Result := TButtonGlyph(FGlyph).Glyph;
end;

procedure TToolbarButton97.SetGlyph (Value: TBitmap);
begin
  TButtonGlyph(FGlyph).Glyph := Value;
  Redraw (True);
end;

function TToolbarButton97.GetNumGlyphs: TNumGlyphs97;
begin
  Result := TButtonGlyph(FGlyph).NumGlyphs;
end;

procedure TToolbarButton97.SetNumGlyphs (Value: TNumGlyphs97);
begin
  if Value < 0 then
    Value := 1
  else
  if Value > High(TNumGlyphs97) then
    Value := High(TNumGlyphs97);
  if Value <> TButtonGlyph(FGlyph).NumGlyphs then begin
    TButtonGlyph(FGlyph).NumGlyphs := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.GlyphChanged(Sender: TObject);
begin
  Redraw (True);
end;

procedure TToolbarButton97.UpdateExclusive;
var
  Msg: TMessage;
begin
  if (FGroupIndex <> 0) and (Parent <> nil) then begin
    Msg.Msg := CM_BUTTONPRESSED;
    Msg.WParam := FGroupIndex;
    Msg.LParam := Longint(Self);
    Msg.Result := 0;
    Parent.Broadcast (Msg);
  end;
end;

procedure TToolbarButton97.SetDown (Value: Boolean);
begin
  if FGroupIndex = 0 then
    Value := False;
  if Value <> FDown then begin
    if FDown and (not FAllowAllUp) then Exit;
    FDown := Value;
    if Value then
      FState := bsExclusive
    else
      FState := bsUp;
    Redraw (True);
    if Value then UpdateExclusive;
  end;
end;

procedure TToolbarButton97.SetFlat (Value: Boolean);
begin
  if FFlat <> Value then begin
    FFlat := Value;
    if FOpaque or not FFlat then
      ControlStyle := ControlStyle + [csOpaque]
    else
      ControlStyle := ControlStyle - [csOpaque];
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetGroupIndex (Value: Integer);
begin
  if FGroupIndex <> Value then begin
    FGroupIndex := Value;
    UpdateExclusive;
  end;
end;

procedure TToolbarButton97.SetLayout (Value: TButtonLayout);
begin
  if FLayout <> Value then begin
    FLayout := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetMargin (Value: Integer);
begin
  if (FMargin <> Value) and (Value >= -1) then begin
    FMargin := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetOldDisabledStyle (Value: Boolean);
begin
  if FOldDisabledStyle <> Value then begin
    FOldDisabledStyle := Value;
    with TButtonGlyph(FGlyph) do begin
      FOldDisabledStyle := Value;
      Invalidate;
    end;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetOpaque (Value: Boolean);
begin
  if FOpaque <> Value then begin
    FOpaque := Value;
    if FOpaque or not FFlat then
      ControlStyle := ControlStyle + [csOpaque]
    else
      ControlStyle := ControlStyle - [csOpaque];
    Invalidate;
  end;
end;

procedure TToolbarButton97.Redraw (const Erase: Boolean);
var
  AddedOpaque: Boolean;
begin
  if FOpaque or not FFlat or not Erase then begin
    { Temporarily add csOpaque to the style. This prevents Invalidate from
      erasing, which isn't needed when Erase is false. }
    AddedOpaque := False;
    if not(csOpaque in ControlStyle) then begin
      AddedOpaque := True;
      ControlStyle := ControlStyle + [csOpaque];
    end;
    try
      Invalidate;
    finally
      if AddedOpaque then
        ControlStyle := ControlStyle - [csOpaque];
    end;
  end
  else
  if not(FOpaque or not FFlat) then
    Invalidate;
end;

procedure TToolbarButton97.SetSpacing(Value: Integer);
begin
  if Value <> FSpacing then begin
    FSpacing := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetAllowAllUp(Value: Boolean);
begin
  if FAllowAllUp <> Value then begin
    FAllowAllUp := Value;
    UpdateExclusive;
  end;
end;

procedure TToolbarButton97.SetDropdownMenu (Value: TPopupMenu);
begin
  if FDropdownMenu <> Value then begin
    FDropdownMenu := Value;
    if Assigned(Value) then
      Value.FreeNotification (Self);
    if FDropdownArrow then
      Redraw (True);
  end;
end;

procedure TToolbarButton97.SetWordWrap (Value: Boolean);
begin
  if FWordWrap <> Value then begin
    FWordWrap := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetDropdownArrow (Value: Boolean);
begin
  if FDropdownArrow <> Value then begin
    FDropdownArrow := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetDropdownCombo (Value: Boolean);
var
  W: Integer;
begin
  if FDropdownCombo <> Value then begin
    FDropdownCombo := Value;
    if not(csLoading in ComponentState) then begin
      if Value then
        Width := Width + DropdownComboWidth
      else begin
        W := Width - DropdownComboWidth;
        if W < 1 then W := 1;
        Width := W;
      end;
    end;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.SetDisplayMode (Value: TButtonDisplayMode);
begin
  if FDisplayMode <> Value then begin
    FDisplayMode := Value;
    Redraw (True);
  end;
end;

procedure TToolbarButton97.WMLButtonDblClk (var Message: TWMLButtonDblClk);
begin
  inherited;
  if FDown then DblClick;
end;

procedure TToolbarButton97.CMEnabledChanged (var Message: TMessage);
const
  NewState: array[Boolean] of TButtonState97 = (bsDisabled, bsUp);
begin
  TButtonGlyph(FGlyph).CreateButtonGlyph (NewState[Enabled]);
  UpdateTracking;
  Redraw (True);
end;

procedure TToolbarButton97.CMButtonPressed (var Message: TMessage);
var
  Sender: TToolbarButton97;
begin
  { UpdateExclusive broadcasts these messages }
  if Message.WParam = FGroupIndex then begin
    Sender := TToolbarButton97(Message.LParam);
    if Sender <> Self then begin
      if Sender.Down and FDown then begin
        FDown := False;
        FState := bsUp;
        Redraw (True);
      end;
      FAllowAllUp := Sender.AllowAllUp;
    end;
  end;
end;

procedure TToolbarButton97.CMDialogChar (var Message: TCMDialogChar);
begin
  with Message do
    if IsAccel(CharCode, Caption) and Enabled and Visible then begin
      { NOTE: There is a bug in TSpeedButton where accelerator keys are still
        processed even when the button is not visible. The 'and Visible'
        corrects it, so TToolbarButton97 doesn't have this problem. }
      Click;
      Result := 1;
    end
    else
      inherited;
end;

procedure TToolbarButton97.CMFontChanged (var Message: TMessage);
begin
  Redraw (True);
end;

procedure TToolbarButton97.CMTextChanged (var Message: TMessage);
begin
  Redraw (True);
end;

procedure TToolbarButton97.CMSysColorChange (var Message: TMessage);
begin
  inherited;
  if Assigned(Pattern) and
     ((PatternBtnFace <> ColorToRGB(clBtnFace)) or
      (PatternBtnHighlight <> ColorToRGB(clBtnHighlight))) then begin
    Pattern.Free;
    Pattern := nil;
  end;
  with TButtonGlyph(FGlyph) do begin
    Invalidate;
    CreateButtonGlyph (FState);
  end;
end;

procedure TToolbarButton97.MouseEntered;
begin
  if Enabled and not FMouseInControl then begin
    FMouseInControl := True;
    if FState = bsUp then
      FState := bsMouseIn;
    Redraw (FDown);
    if Assigned(FOnMouseEnter) then
      FOnMouseEnter (Self);
  end;
end;

procedure TToolbarButton97.MouseLeft;
begin
  if Enabled and FMouseInControl and not FMouseIsDown then begin
    if (FState = bsMouseIn) or (not FInClick and (FState = bsDown)) then
      FState := bsUp;
    FMouseInControl := False;
    Redraw (True);
    if ButtonMouseInControl = Self then begin
      ButtonMouseTimer.Enabled := False;
      ButtonMouseInControl := nil;
    end;
    if Assigned(FOnMouseExit) then
      FOnMouseExit (Self);
  end;
end;

procedure TToolbarButton97.CMMouseLeave (var Message: TMessage);
begin
  inherited;
  MouseLeft;
end;

procedure TToolbarButton97.ButtonMouseTimerHandler (Sender: TObject);
var
  P: TPoint;
begin
  { The button mouse timer is used to periodically check if mouse has left.
    Normally it receives a CM_MOUSELEAVE, but the VCL does not send a
    CM_MOUSELEAVE if the mouse is moved quickly from the button to another
    application's window. For some reason, this problem doesn't seem to occur
    on Windows NT 4 -- only 95 and 3.x.

    The timer (which ticks 8 times a second) is only enabled when the
    application is active and the mouse is over a button, so it uses virtually
    no processing power

    For something interesting to try: If you want to know just how often this
    is called, try putting a Beep call in here }

  GetCursorPos (P);
  if FindDragTarget(P, True) <> Self then
    MouseLeft;
end;

class function TToolbarButton97.DeactivateHook (var Message: TMessage): Boolean;
begin
  Result := False;
  { Hide any active border when application is deactivated }
  if (Message.Msg = CM_DEACTIVATE) and Assigned(ButtonMouseInControl) then
    ButtonMouseInControl.MouseLeft;
end;


{ TEdit97 - internal }

constructor TEdit97.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);
  AutoSize := False;
  Ctl3D := False;
  BorderStyle := bsNone;
  ControlStyle := ControlStyle - [csFramed]; {fixes a VCL bug with Win 3.x}
  Height := 19;
end;

procedure TEdit97.CMMouseEnter (var Message: TMessage);
begin
  inherited;
  MouseInControl := True;
  RedrawBorder (0);
end;

procedure TEdit97.CMMouseLeave (var Message: TMessage);
begin
  inherited;
  MouseInControl := False;
  RedrawBorder (0);
end;

procedure TEdit97.NewAdjustHeight;
var
  DC: HDC;
  SaveFont: HFONT;
  Metrics: TTextMetric;
begin
  DC := GetDC(0);
  SaveFont := SelectObject(DC, Font.Handle);
  GetTextMetrics (DC, Metrics);
  SelectObject (DC, SaveFont);
  ReleaseDC (0, DC);

  Height := Metrics.tmHeight + 6;
end;

procedure TEdit97.Loaded;
begin
  inherited Loaded;
  if not(csDesigning in ComponentState) then
    NewAdjustHeight;
end;

procedure TEdit97.CMEnabledChanged (var Message: TMessage);
const
  EnableColors: array[Boolean] of TColor = (clBtnFace, clWindow);
begin
  inherited;
  Color := EnableColors[Enabled];
end;

procedure TEdit97.CMFontChanged (var Message: TMessage);
begin
  inherited;
  if not((csDesigning in ComponentState) and (csLoading in ComponentState)) then
    NewAdjustHeight;
end;

procedure TEdit97.WMSetFocus (var Message: TWMSetFocus);
begin
  inherited;
  if not(csDesigning in ComponentState) then
    RedrawBorder (0);
end;

procedure TEdit97.WMKillFocus (var Message: TWMKillFocus);
begin
  inherited;
  if not(csDesigning in ComponentState) then
    RedrawBorder (0);
end;

procedure TEdit97.WMNCCalcSize (var Message: TWMNCCalcSize);
begin
  inherited;
  InflateRect (Message.CalcSize_Params^.rgrc[0], -3, -3);
end;

procedure TEdit97.WMNCPaint (var Message: TMessage);
begin
  inherited;
  RedrawBorder (Message.WParam);
end;

procedure TEdit97.RedrawBorder (const Clip: HRGN);
var
  DC: HDC;
  R: TRect;
  NewClipRgn: HRGN;
  BtnFaceBrush, WindowBrush: HBRUSH;
begin
  DC := GetWindowDC(Handle);
  try
    { Use update region }
    if Clip <> 0 then begin
      GetWindowRect (Handle, R);
      { An invalid region is generally passed when the window is first created }
      if SelectClipRgn(DC, Clip) = ERROR then begin
        NewClipRgn := CreateRectRgnIndirect(R);
        SelectClipRgn (DC, NewClipRgn);
        DeleteObject (NewClipRgn);
      end;
      OffsetClipRgn (DC, -R.Left, -R.Top);
    end;

    { This works around WM_NCPAINT problem described at top of source code }
    {no!  R := Rect(0, 0, Width, Height);}
    GetWindowRect (Handle, R);  OffsetRect (R, -R.Left, -R.Top);
    BtnFaceBrush := CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
    WindowBrush := CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    if ((csDesigning in ComponentState) and Enabled) or
       (not(csDesigning in ComponentState) and
        (Focused or (MouseInControl and not(Screen.ActiveControl is TEdit97)))) then begin
      DrawEdge (DC, R, BDR_SUNKENOUTER, BF_RECT or BF_ADJUST);
      FrameRect (DC, R, BtnFaceBrush);
      InflateRect (R, -1, -1);
      FrameRect (DC, R, WindowBrush);
    end
    else begin
      FrameRect (DC, R, BtnFaceBrush);
      InflateRect (R, -1, -1);
      FrameRect (DC, R, BtnFaceBrush);
      InflateRect (R, -1, -1);
      FrameRect (DC, R, WindowBrush);
    end;
    DeleteObject (WindowBrush);
    DeleteObject (BtnFaceBrush);
  finally
    ReleaseDC (Handle, DC);
  end;
end;


const
  Sig: PChar = '- Toolbar97 version ' + Toolbar97Version + ' by Jordan Russell -';

initialization
  Sig := Sig;

  UsedForms := TList.Create;

  ButtonMouseTimer := TTimer.Create(nil);
  ButtonMouseTimer.Enabled := False;
  ButtonMouseTimer.Interval := 125;  { 8 times a second }

  Application.HookMainWindow (TToolbarButton97.DeactivateHook);
finalization
  Application.UnhookMainWindow (TToolbarButton97.DeactivateHook);
  if Assigned(ButtonMouseTimer) then ButtonMouseTimer.Free;
  if Assigned(UsedForms) then UsedForms.Free;
end.
