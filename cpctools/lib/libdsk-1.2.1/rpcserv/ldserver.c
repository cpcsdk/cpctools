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

#include "ldserver.h"   			/* specific to this program 		*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include "libdsk.h"

HANDLE hInst;                       /* current instance                      */
HWND hwndServer;                
UINT nTimer;

unsigned short hMailslot;
unsigned short cbMessageSize, cbMailslotSize, cbNextSize,
    usNextPriority, cMessages;
unsigned short err;
unsigned char inbuf[16384], outbuf[16384];

unsigned int nRefCount = 0;
unsigned int busy = 0;

VOID DoServer(HWND); 

/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

    PURPOSE: calls initialization function, processes message loop

    COMMENTS:

        Windows recognizes this function by name as the initial entry point
        for the program.  This function calls the application initialization
        routine, if no other instance of the program is running, and always
        calls the instance initialization routine.  It then executes a message
        retrieval and dispatch loop that is the top-level control structure
        for the remainder of execution.  The loop is terminated when a WM_QUIT
        message is received, at which time this function exits the application
        instance by returning the value passed by PostQuitMessage().

        If this function must abort before entering the message loop, it
        returns the conventional value NULL.

****************************************************************************/

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;                            /* current instance             */
HANDLE hPrevInstance;                        /* previous instance            */
LPSTR lpCmdLine;                             /* command line                 */
int nCmdShow;                                /* show-window type (open/icon) */
{
    MSG msg;                                 /* message                      */

    if (!hPrevInstance)                  /* Other instances of app running? */
        if (!InitApplication(hInstance)) /* Initialize shared things */
            return (FALSE);              /* Exits if unable to initialize     */

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,        /* message structure                      */
            NULL,                  /* handle of window receiving the message */
            NULL,                  /* lowest message to examine              */
            NULL))                 /* highest message to examine             */
        {
        TranslateMessage(&msg);    /* Translates virtual key codes           */
        DispatchMessage(&msg);     /* Dispatches message to window           */
    }

    UnregisterClass("LDServerWClass", hInstance);
    return (msg.wParam);           /* Returns the value from PostQuitMessage */
}


/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

    COMMENTS:

        This function is called at initialization time only if no other
        instances of the application are running.  This function performs
        initialization tasks that can be done once for any number of running
        instances.

        In this case, we initialize a window class by filling out a data
        structure of type WNDCLASS and calling the Windows RegisterClass()
        function.  Since all instances of this application use the same window
        class, we only need to do this when the first instance is initialized.


****************************************************************************/

BOOL InitApplication(hInstance)
HANDLE hInstance;                              /* current instance           */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = NULL;                    /* Class style(s).                    */
    wc.lpfnWndProc = MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = LoadIcon(hInstance, "server");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = COLOR_BTNFACE+1;
    wc.lpszMenuName =  "ServerMenu";   /* Name of menu resource in .RC file. */
    wc.lpszClassName = "LDServerWClass"; /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return (RegisterClass(&wc));

}


/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

    COMMENTS:

        This function is called at initialization time for every instance of
        this application.  This function performs initialization tasks that
        cannot be shared by multiple instances.

        In this case, we save the instance handle in a static variable and
        create and display the main program window.

****************************************************************************/

BOOL InitInstance(hInstance, nCmdShow)
    HANDLE          hInstance;          /* Current instance identifier.       */
    int             nCmdShow;           /* Param for first ShowWindow() call. */
{       
	char sErr[128];
//    int i;
    RECT Rect;
//  TEXTMETRIC metrics;
//  HDC hdc;

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    hInst = hInstance;


    /* Create a main window for this application instance.  */

    hwndServer = CreateWindow(
        "LDServerWClass",                /* See RegisterClass() call.          */
        "LibDSK mailslot server",
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        CW_USEDEFAULT,                  /* Default horizontal position.       */
        CW_USEDEFAULT,                  /* Default vertical position.         */
        400,
        200,
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer not needed.                */
    );

    GetClientRect(hwndServer, (LPRECT) &Rect);

    /* If window could not be created, return "failure" */

    if (!hwndServer)
        return (FALSE);

/*  hdc = GetDC(hwndServer);
    GetTextMetrics(hdc, &metrics);
    cyText = metrics.tmHeight + metrics.tmExternalLeading;
    ReleaseDC(hwndServer, hdc);
*/
    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hwndServer, nCmdShow);  /* Show the window                        */
    UpdateWindow(hwndServer);          /* Sends WM_PAINT message                 */

	err = DosMakeMailslot("\\mailslot\\LibDsk16", 16384, 0,
    				&hMailslot        // receives handle for mailslot
    );
                                 
    strcpy(sErr, "Cannot create mailslot: ");                             
	switch (err)
    {                            
    	case NERR_Success:		 nTimer = SetTimer(hwndServer, 1, 10, NULL);
    							 return TRUE; 
    	case NERR_NetNotStarted: strcat(sErr, "The network is not started.");
        						 break;  
		case ERROR_ALREADY_EXISTS: strcat(sErr, "The mailslot is already in use. "
									      "Another LibDsk server is probably running.");
									      break;
		case ERROR_INVALID_PARAMETER:	 
								 strcat(sErr, "Invalid parameter");
								 break;
		case ERROR_PATH_NOT_FOUND:	 
								 strcat(sErr, "Path not found");
								 break;   
		default:				 wsprintf(sErr + strlen(sErr), "Unknown error 0x%04x", err);
								 break;
		    	
    }
    MessageBox(hwndServer, sErr, "LibDSK mailslot server", MB_ICONSTOP | MB_OK);
    return(FALSE);
}           


void ExitInstance()
{                                         
	if (hMailslot)
	{
		DosDeleteMailslot(hMailslot);
		hMailslot = 0;
	}
}

    

/****************************************************************************

    FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)

    PURPOSE:  Processes messages

    MESSAGES:

        WM_COMMAND    - application menu (About dialog box)
        WM_DESTROY    - destroy window

    COMMENTS:

        To process the IDM_ABOUT message, call MakeProcInstance() to get the
        current instance address of the About() function.  Then call Dialog
        box which will create the box according to the information in your
        server.rc file and turn control over to the About() function.   When
        it returns, free the intance address.

****************************************************************************/

long FAR PASCAL __export MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;                                /* window handle                   */
UINT message;			      /* type of message		 */
WPARAM wParam;				    /* additional information	       */
LPARAM lParam;				    /* additional information	       */
{
    PAINTSTRUCT ps;

    switch (message) {
    case WM_INITMENU:
        if (GetMenu(hWnd) != wParam)
            break;

        break;

    case WM_COMMAND:           /* message: command from application menu */
        switch (wParam) {
        case IDM_EXIT:
        	PostMessage(hWnd, WM_CLOSE, 0, 0);
        	break;
        case IDM_ABOUT:
            DoDialog("ABOUT", About, 0, TRUE);
            break;

        case IDM_HELP:
           break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;

    case WM_PAINT:
	    BeginPaint(hWnd, &ps);
        PaintServer(hWnd, ps.hdc);
    	EndPaint(hWnd, &ps);
        break;
    
    case WM_TIMER:
    	DoServer(hWnd);
    	break;
    
	case WM_QUIT:
		ExitInstance();
		break;

    case WM_DESTROY:                  /* message: window being destroyed */
	 	if (nTimer) 
		{
			KillTimer(hWnd, nTimer);
			nTimer = 0;
		}
       PostQuitMessage(0);
        break;
                                        
    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}





void Delay(
DWORD delay,
BOOL fModal)
{
    MSG msg;
    delay = GetCurrentTime() + delay;
    while (GetCurrentTime() < delay) {
        if (fModal && PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
    
    
VOID DoServer(HWND hWnd)
{ 
	unsigned v = DosMailslotInfo(hMailslot, &cbMessageSize, &cbMailslotSize, &cbNextSize, &usNextPriority, &cMessages);
	int outlen = 16384;
	unsigned char *backslot;
	unsigned int buflen;
	dsk_err_t err;
	HDC dc;
			  
	dc = GetDC(hWnd);
	if (!cMessages) 
	{          
		if (busy)	/* Reset the "busy" indicator */
		{
			--busy;
			if (busy == 0) PaintServer(hWnd, dc);
		} 
		ReleaseDC(hWnd, dc);
		return;
	}
	if (!busy)
	{
		busy = 10;
		PaintServer(hWnd, dc);
    }
    else busy = 10;
    
	DosReadMailslot(hMailslot, inbuf, &cbMessageSize, &cbNextSize, &usNextPriority, MAILSLOT_NO_TIMEOUT);
	    
	buflen = (((unsigned int)inbuf[0]) << 8) | inbuf[1];   
	backslot = inbuf + 2;
	    
	err = dsk_rpc_server(inbuf + 2 + buflen, cbMessageSize - buflen - 2, outbuf, &outlen, &nRefCount);
	outlen = 16384 - outlen;
	if (!memcmp(backslot, "\\\\.", 3)) backslot += 3;
	DosWriteMailslot(backslot, outbuf, outlen, 0, 2, MAILSLOT_NO_TIMEOUT);
	ReleaseDC(hWnd, dc);
}                     

    



/* Repaint status
 */
VOID PaintServer(HWND hwnd, HDC hdc)
{
    RECT rc;
    char str[50];
                     
	SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
	SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
//    SetBkMode(hdc, TRANSPARENT);
    GetClientRect(hwnd, &rc);
                 
    sprintf(str, "Ref count = %d  Status = %s", nRefCount, busy ? "Active   " : "Waiting   ");
    TextOut(hdc, rc.left, rc.top, str, strlen(str)); 


}


