// #include <windows.h>
// #include "resource.h"

/*
extern BOOL DF_INTRO; // zda se ma poustet intro
extern BOOL DF_ZVUK; // zda se ma byt zvuk
extern BOOL DF_HUDBA; // zda ma byt hudba
extern BOOL DF_FULLSCREEN; // zda ma byt fullscreen
extern BOOL DF_NOCDISABLE; // zakaze noc
extern BOOL DF_SHADOWDISABLE; // zakaze stiny
extern BOOL DF_SMOGDISABLE; // zakaze smog
extern BOOL DF_SMYKDISABLE;  // zakaze smyky
extern int  DF_BPP;
extern long  hicon;



BOOL CALLBACK SetupDlg( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );

void SetupDialog()
{
	if (DialogBox( NULL, MAKEINTRESOURCE(IDD_SETUP), NULL, &SetupDlg) == IDCANCEL) exit(0);
}


BOOL CALLBACK SetupDlg( HWND hDlg, UINT msg, WPARAM wParam, LPARAM  )
{

    switch( msg ) 
    {
        case WM_INITDIALOG:
				  if (DF_INTRO)
  			        CheckDlgButton( hDlg, IDC_INTRO, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_INTRO, BST_UNCHECKED );
				  if (DF_ZVUK)
  			        CheckDlgButton( hDlg, IDC_ZVUKY, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_ZVUKY, BST_UNCHECKED );
				  if (DF_HUDBA)
  			        CheckDlgButton( hDlg, IDC_HUDBA, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_HUDBA, BST_UNCHECKED );
				  if (DF_FULLSCREEN)
  			        CheckDlgButton( hDlg, IDC_FULLSCREEN, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_FULLSCREEN, BST_UNCHECKED );
				  if (DF_NOCDISABLE)
  			        CheckDlgButton( hDlg, IDC_NOCDISABLE, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_NOCDISABLE, BST_UNCHECKED );
				  if (DF_SHADOWDISABLE)
  			        CheckDlgButton( hDlg, IDC_SHADOWDISABLE, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_SHADOWDISABLE, BST_UNCHECKED );
				  if (DF_SMOGDISABLE)
  			        CheckDlgButton( hDlg, IDC_SMOGDISABLE, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_SMOGDISABLE, BST_UNCHECKED );
				  if (DF_SMYKDISABLE)
  			        CheckDlgButton( hDlg, IDC_SMYKDISABLE, BST_CHECKED );
				  else
  			        CheckDlgButton( hDlg, IDC_SMYKDISABLE, BST_UNCHECKED );

		        CheckDlgButton( hDlg, IDC_SMYKDISABLE, BST_UNCHECKED );
				  switch (DF_BPP) {
				  case 15: CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED ); break;
				  case 16: CheckDlgButton( hDlg, IDC_RADIO2, BST_CHECKED ); break;
				  case 32: CheckDlgButton( hDlg, IDC_RADIO3, BST_CHECKED ); break;
				  }
				  
					

			    PostMessage( hDlg, WM_SETICON, ICON_BIG, ( LPARAM ) hicon );
			    PostMessage( hDlg, WM_SETICON, ICON_SMALL, ( LPARAM ) hicon );

				 RECT r, r2;
				 GetWindowRect(hDlg, &r);
				 GetWindowRect(GetDesktopWindow(), &r2);
				 SetWindowPos(hDlg, HWND_TOP,  (r2.right-(r.right-r.left))/2, (r2.bottom-(r.bottom-r.top))/2, 0 ,0,SWP_NOSIZE|SWP_NOZORDER);


            break;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case IDCANCEL:
                    EndDialog( hDlg, IDCANCEL );
                    break;

                case IDOK:
						  DF_INTRO = ( SendMessage(GetDlgItem( hDlg, IDC_INTRO), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_ZVUK = ( SendMessage(GetDlgItem( hDlg, IDC_ZVUKY), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_HUDBA = ( SendMessage(GetDlgItem( hDlg, IDC_HUDBA), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_FULLSCREEN = ( SendMessage(GetDlgItem( hDlg, IDC_FULLSCREEN), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_NOCDISABLE = ( SendMessage(GetDlgItem( hDlg, IDC_NOCDISABLE), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_SHADOWDISABLE = ( SendMessage(GetDlgItem( hDlg, IDC_SHADOWDISABLE), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_SMOGDISABLE = ( SendMessage(GetDlgItem( hDlg, IDC_SMOGDISABLE), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
						  DF_SMYKDISABLE = ( SendMessage(GetDlgItem( hDlg, IDC_SMYKDISABLE), BM_GETSTATE, 0, 0 ) == BST_CHECKED );
                    if (SendMessage(GetDlgItem( hDlg, IDC_RADIO1), BM_GETSTATE, 0, 0 ) == BST_CHECKED ) DF_BPP = 15;
                    if (SendMessage(GetDlgItem( hDlg, IDC_RADIO2), BM_GETSTATE, 0, 0 ) == BST_CHECKED ) DF_BPP = 16;
                    if (SendMessage(GetDlgItem( hDlg, IDC_RADIO3), BM_GETSTATE, 0, 0 ) == BST_CHECKED ) DF_BPP = 32;
						  EndDialog( hDlg, IDOK );
                    break;

                default:
                    return FALSE; // Didn't handle message
            }
            break;

        case WM_DESTROY:
            break; 

        default:
            return FALSE; // Didn't handle message
    }

    return TRUE; // Handled message
}

*/
