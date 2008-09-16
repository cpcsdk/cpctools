/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2003  John Elliott <jce@seasip.demon.co.uk>            *
 *                                                                         *
 *    This library is free software; you can redistribute it and/or        *
 *    modify it under the terms of the GNU Library General Public          *
 *    License as published by the Free Software Foundation; either         *
 *    version 2 of the License, or (at your option) any later version.     *
 *                                                                         *
 *    This library is distributed in the hope that it will be useful,      *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
 *    Library General Public License for more details.                     *
 *                                                                         *
 *    You should have received a copy of the GNU Library General Public    *
 *    License along with this library; if not, write to the Free           *
 *    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,      *
 *    MA 02111-1307, USA                                                   *
 *                                                                         *
 ***************************************************************************/


#include "ldserver.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void ScreenToClientRect(HWND hWnd, RECT *rc)
{
	POINT ptUpperLeft;  /* client coordinate of upper left  */
	POINT ptLowerRight; /* client coordinate of lower right */

	ptUpperLeft.x   = rc->left;
	ptUpperLeft.y   = rc->top;
	ptLowerRight.x  = rc->right;
	ptLowerRight.y  = rc->bottom;
	
	ScreenToClient(hWnd, &ptUpperLeft );
	ScreenToClient(hWnd, &ptLowerRight);
    rc->left  = ptUpperLeft.x;
    rc->top   = ptUpperLeft.y;
    rc->right = ptLowerRight.x;
    rc->bottom= ptLowerRight.y;
}
            
extern HINSTANCE hInst;

static COLORREF crGrey;                          
static COLORREF crWhite;
static HBRUSH   brGrey;
static HBRUSH	brWhite;
static HBITMAP  bmShade;
static HBRUSH	brShade;          
          
BOOL EraseBackground(HWND hWnd, HDC hDC)
{
	RECT  rc;           /* window's screen coordinates      */
	HWND  hwItem;
	
	GetWindowRect(hWnd, &rc);
    ScreenToClientRect(hWnd, &rc);
    FillRect(hDC, &rc, brGrey);

    hwItem = GetDlgItem(hWnd, IDC_TOPFRAME);
	GetWindowRect(hwItem, &rc);
    ScreenToClientRect(hWnd, &rc);
    FillRect(hDC, &rc, brShade);

    hwItem = GetDlgItem(hWnd, IDC_BOTTOMFRAME);
	GetWindowRect(hwItem, &rc);
    ScreenToClientRect(hWnd, &rc);
    FillRect(hDC, &rc, brWhite);
    
    return TRUE; 
} 


/****************************************************************************

    FUNCTION: About(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages for "About" dialog box

    MESSAGES:

        WM_INITDIALOG - initialize dialog box
        WM_COMMAND    - Input received

    COMMENTS:

        No initialization is needed for this particular dialog box, but TRUE
        must be returned to Windows.

        Wait for user to click on "Ok" button, then close the dialog box.

****************************************************************************/

BOOL FAR PASCAL __export About(hDlg, message, wParam, lParam)
HWND hDlg;                                /* window handle of the dialog box */
unsigned message;                         /* type of message                 */
WORD wParam;                              /* message-specific information    */
LONG lParam;
{
    switch (message) {
        case WM_INITDIALOG:                /* message: initialize dialog box */
            crGrey  = GetSysColor(COLOR_BTNFACE);
            crWhite = GetSysColor(COLOR_WINDOW);
			brGrey  = CreateSolidBrush(crGrey);
			brWhite = CreateSolidBrush(crWhite);           
			bmShade = LoadBitmap(hInst, "SHADE");
			brShade = CreatePatternBrush(bmShade);
			return (TRUE);
        
        case WM_ERASEBKGND:
        	return EraseBackground(hDlg, (HDC)wParam);  
        
        case WM_COMMAND:                      /* message: received a command */
            if (wParam == IDOK                /* "OK" box selected?          */
                || wParam == IDCANCEL) {      /* System menu close command? */
                if (brGrey)  DeleteObject(brGrey);  brGrey  = NULL;
                if (brWhite) DeleteObject(brWhite); brWhite = NULL;
                if (brShade) DeleteObject(brShade); brShade = NULL;
                if (bmShade) DeleteObject(bmShade); bmShade = NULL;
                EndDialog(hDlg, TRUE);        /* Exits the dialog box        */
                return (TRUE);
            }
            break;
    }
    return (FALSE);                           /* Didn't process a message    */
}




/****************************************************************************
 *                                                                          *
 *  FUNCTION   : DoDialog()                                                 *
 *                                                                          *
 *  PURPOSE    : Generic dialog invocation routine.  Handles procInstance   *
 *               stuff, focus management and param passing.                 *
 *  RETURNS    : result of dialog procedure.                                *
 *                                                                          *
 ****************************************************************************/
int FAR DoDialog(
LPSTR lpTemplateName,
FARPROC lpDlgProc,
DWORD param,
BOOL fRememberFocus)
{
    WORD wRet;
    HWND hwndFocus;

    if (fRememberFocus)
        hwndFocus = GetFocus();
    lpDlgProc = MakeProcInstance(lpDlgProc, hInst);
    wRet = DialogBoxParam(hInst, lpTemplateName, hwndServer, lpDlgProc, param);
    FreeProcInstance(lpDlgProc);
    if (fRememberFocus)
        SetFocus(hwndFocus);

    return wRet;
}



