unit Dabout;

{-------------------------------------------------------------------}
{                    Unit:    Dabout.pas                            }
{                    Project: EPANET2W                              }
{                    Version: 2.0                                   }
{                    Date:    5/31/00                               }
{                             9/7/00                                }
{                             12/29/00                              }
{                             1/5/01                                }
{                             3/1/01                                }
{                             11/19/01                              }
{                             12/8/01                               }
{                             6/24/02                               }
{                    Author:  L. Rossman                            }
{                                                                   }
{   Form unit containing the "About" dialog box for EPANET2W.       }
{-------------------------------------------------------------------}

interface

uses WinTypes, WinProcs, Classes, Graphics, Forms, Controls, StdCtrls,
  Buttons, ExtCtrls;

type
  TAboutBoxForm = class(TForm)
    Panel1: TPanel;
    ProductName: TLabel;
    Version: TLabel;
    Label3: TLabel;
    Button1: TButton;
    Build: TLabel;
    Panel2: TPanel;
    ProgramIcon: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

//var
//  AboutBoxForm: TAboutBoxForm;

implementation

{$R *.DFM}

procedure TAboutBoxForm.FormCreate(Sender: TObject);
begin
   Build.Caption := 'Build 2.00.10';
end;

end.

