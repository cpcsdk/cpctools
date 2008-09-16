VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "LibDsk test app"
   ClientHeight    =   5868
   ClientLeft      =   48
   ClientTop       =   288
   ClientWidth     =   9984
   LinkTopic       =   "Form1"
   ScaleHeight     =   5868
   ScaleWidth      =   9984
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnDskForm 
      Caption         =   "Format"
      Height          =   732
      Left            =   3240
      TabIndex        =   12
      Top             =   5040
      Width           =   1452
   End
   Begin VB.ComboBox cbFormat 
      Height          =   288
      Left            =   5880
      Style           =   2  'Dropdown List
      TabIndex        =   9
      Top             =   2280
      Width           =   3852
   End
   Begin VB.CommandButton btnDsktrans 
      Caption         =   "Copy"
      Height          =   732
      Left            =   1680
      TabIndex        =   11
      Top             =   5040
      Width           =   1452
   End
   Begin VB.TextBox tbRetries 
      Height          =   288
      Left            =   2280
      TabIndex        =   8
      Text            =   "1"
      Top             =   2280
      Width           =   1212
   End
   Begin VB.Frame Frame2 
      Caption         =   "Target"
      Height          =   2052
      Left            =   5040
      TabIndex        =   18
      Top             =   120
      Width           =   4692
      Begin VB.ComboBox cbTargetFilename 
         Height          =   288
         Left            =   1440
         TabIndex        =   4
         Top             =   360
         Width           =   2652
      End
      Begin VB.CommandButton btnTargetBrowse 
         Caption         =   "..."
         Height          =   252
         Left            =   4200
         TabIndex        =   5
         Top             =   360
         Width           =   252
      End
      Begin VB.ComboBox cbTargetDriver 
         Height          =   288
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   720
         Width           =   2652
      End
      Begin VB.ComboBox cbTargetCompression 
         Height          =   288
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   1080
         Width           =   2652
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         Caption         =   "Filename"
         Height          =   252
         Left            =   120
         TabIndex        =   21
         Top             =   360
         Width           =   1212
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "Driver"
         Height          =   252
         Left            =   240
         TabIndex        =   20
         Top             =   720
         Width           =   1092
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "Compression"
         Height          =   252
         Left            =   120
         TabIndex        =   19
         Top             =   1080
         Width           =   1212
      End
   End
   Begin VB.CommandButton btnDskID 
      Caption         =   "Identify"
      Height          =   732
      Left            =   120
      TabIndex        =   10
      Top             =   5040
      Width           =   1452
   End
   Begin VB.ListBox lbResults 
      BeginProperty Font 
         Name            =   "Lucida Console"
         Size            =   7.8
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2076
      Left            =   120
      TabIndex        =   14
      Top             =   2640
      Width           =   9612
   End
   Begin VB.Frame Frame1 
      Caption         =   "Source"
      Height          =   2052
      Left            =   120
      TabIndex        =   13
      Top             =   120
      Width           =   4692
      Begin VB.ComboBox cbCompression 
         Height          =   288
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   3
         Top             =   1080
         Width           =   2652
      End
      Begin VB.ComboBox cbDriver 
         Height          =   288
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   2
         Top             =   720
         Width           =   2652
      End
      Begin VB.CommandButton btnSourceBrowse 
         Caption         =   "..."
         Height          =   252
         Left            =   4200
         TabIndex        =   1
         Top             =   360
         Width           =   252
      End
      Begin VB.ComboBox cbSourceFilename 
         Height          =   288
         Left            =   1440
         TabIndex        =   0
         Top             =   360
         Width           =   2652
      End
      Begin VB.Label labelCompression 
         Alignment       =   1  'Right Justify
         Caption         =   "Compression"
         Height          =   252
         Left            =   120
         TabIndex        =   17
         Top             =   1080
         Width           =   1212
      End
      Begin VB.Label labelDriver 
         Alignment       =   1  'Right Justify
         Caption         =   "Driver"
         Height          =   252
         Left            =   240
         TabIndex        =   16
         Top             =   720
         Width           =   1092
      End
      Begin VB.Label labelSourceFilename 
         Alignment       =   1  'Right Justify
         Caption         =   "Filename"
         Height          =   252
         Left            =   120
         TabIndex        =   15
         Top             =   360
         Width           =   1212
      End
   End
   Begin VB.Label lblProgress 
      Height          =   492
      Left            =   4800
      TabIndex        =   25
      Top             =   5160
      Width           =   4932
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Right Justify
      Caption         =   "Override format:"
      Height          =   252
      Left            =   3720
      TabIndex        =   24
      Top             =   2280
      Width           =   2052
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "Retries in case of error"
      Height          =   252
      Left            =   240
      TabIndex        =   23
      Top             =   2280
      Width           =   1932
   End
   Begin VB.Label lblReport 
      Alignment       =   2  'Center
      Height          =   252
      Left            =   120
      TabIndex        =   22
      Top             =   4800
      Width           =   9612
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/***************************************************************************
' *                                                                         *
' *    LIBDSK: General floppy and diskimage access library                  *
' *    Copyright (C) 2005  John Elliott <jce@seasip.demon.co.uk>            *
' *                                                                         *
' *   This library is free software; you can redistribute it and/or         *
' *    modify it under the terms of the GNU Library General Public          *
' *    License as published by the Free Software Foundation; either         *
' *    version 2 of the License, or (at your option) any later version.     *
' *                                                                         *
' *    This library is distributed in the hope that it will be useful,      *
' *    but WITHOUT ANY WARRANTY; without even the implied warranty of       *
' *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
' *    Library General Public License for more details.                     *
' *                                                                         *
' *    You should have received a copy of the GNU Library General Public    *
' *    License along with this library; if not, write to the Free           *
' *    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,      *
' *    MA 02111-1307, USA                                                   *
' *                                                                         *
' ***************************************************************************/

'
' DSKtools is an example LibDsk client, written in VB, and roughly equivalent
' to a limited clone of dskform/dskid/dsktrans. There is no error handling;
' this is example code, not production quality.
'

' Implementation of the "Reporter" callback
Implements LIBDSK.reporter


Public Sub Reporter_report(ByVal s As String)
    lblReport.Caption = s
End Sub

Public Sub Reporter_endreport()
    lblReport.Caption = ""
End Sub



'
' VB versions of DSKID, DSKTRANS, DSKFORM
'
Private Sub btnDskID_Click()
    Dim Lib As LIBDSK.Library
    Dim dsk As LIBDSK.Disk
    Dim geom As LIBDSK.Geometry
    Set Lib = New LIBDSK.Library
    Lib.reporter = Me
    
    Let driver = cbDriver.Text
    If (driver = "(Auto Detect)") Then
        Let driver = ""
    End If
    Let compress = cbCompression.Text
    If (compress = "(Auto Detect)") Then
        Let compress = ""
    End If
    
    Set dsk = Lib.open(cbSourceFilename.Text, _
        driver, compress)
    dsk.retries = Val(tbRetries.Text)
    ' XXX Set forcehead
    Set geom = dsk.get_geometry()
    Let compress = dsk.compdesc
    Let driver = dsk.drvdesc
    Let comment = dsk.comment
    lbResults.Clear
    
    lbResults.AddItem ("Driver:        " + driver)
    If compress <> "" Then
        lbResults.AddItem ("Compression:   " + compress)
    End If
    If (geom.fm = 0) Then
        recmode = "MFM"
    Else
        recmode = "FM"
    End If
    
    lbResults.AddItem ("Sidedness:    " + Str(geom.Sidedness))
    lbResults.AddItem ("Cylinders:    " + Str(geom.cylinders))
    lbResults.AddItem ("Heads:        " + Str(geom.heads))
    lbResults.AddItem ("Sectors:      " + Str(geom.sectors))
    lbResults.AddItem ("First sector: " + Str(geom.secbase))
    lbResults.AddItem ("Sector size:  " + Str(geom.secsize))
    lbResults.AddItem ("Data rate:    " + Str(geom.DataRate))
    lbResults.AddItem ("Record mode:  " + recmode)
    lbResults.AddItem ("R/W gap:      0x" + Hex(geom.rwgap))
    lbResults.AddItem ("Format gap:   0x" + Hex(geom.fmtgap))
    Let Status = dsk.drive_status(geom, 0)
    lbResults.AddItem ("Status:       " + Str(Status))
    If comment <> "" Then
        lbResults.AddItem ("Comment:      " + comment)
    End If
End Sub

Private Sub btnDsktrans_Click()
    Dim Lib As LIBDSK.Library
    Dim source As LIBDSK.Disk
    Dim target As LIBDSK.Disk
    Dim geom As LIBDSK.Geometry
    Dim geom2 As LIBDSK.Geometry
    Set Lib = New LIBDSK.Library
    Lib.reporter = Me
    
    Let driver = cbDriver.Text
    If (driver = "(Auto Detect)") Then
        Let driver = ""
    End If
    Let compress = cbCompression.Text
    If (compress = "(Auto Detect)") Then
        Let compress = ""
    End If
    
    Let tdriver = cbTargetDriver.Text
    If (tdriver = "(Auto Detect)") Then
        Let tdriver = ""
    End If
    Let tcompress = cbTargetCompression.Text
    If (tcompress = "(None)") Then
        Let tcompress = ""
    End If
    
    Set source = Lib.open(cbSourceFilename.Text, driver, compress)
    Set target = Lib.Create(cbTargetFilename.Text, tdriver, tcompress)
    source.retries = Val(tbRetries.Text)
    target.retries = Val(tbRetries.Text)
    
    If cbFormat.Text = "(Auto Detect)" Then
        Set geom = source.get_geometry()
    Else
        For n = 0 To Lib.stdformat_count - 1
            Set geom2 = Lib.stdformat(n, name, (desc))
            If (cbFormat.Text = name + ":" + desc) Then
                Set geom = geom2
            End If
        Next n
    End If
    For cylinder = 0 To geom.cylinders - 1
        For head = 0 To geom.heads - 1
            Call target.apform(geom, cylinder, head, 229)
            For sector = 0 To geom.sectors - 1
                lblProgress.Caption = "Cylinder " + Str(cylinder) + " Head " + Str(head) + " Sector " + Str(sector + geom.secbase)
                buf = source.pread(geom, cylinder, head, sector + geom.secbase)
                Call target.pwrite(geom, buf, cylinder, head, sector + geom.secbase)
            Next sector
        Next head
    Next cylinder
End Sub


Private Sub btnDskForm_Click()
    Dim Lib As LIBDSK.Library
    Dim target As LIBDSK.Disk
    Dim geom As LIBDSK.Geometry
    Dim geom2 As LIBDSK.Geometry
    Set Lib = New LIBDSK.Library
    Lib.reporter = Me
    
    Let tdriver = cbTargetDriver.Text
    If (tdriver = "(Auto Detect)") Then
        Let tdriver = ""
    End If
    Let tcompress = cbTargetCompression.Text
    If (tcompress = "(None)") Then
        Let tcompress = ""
    End If
    
    Set target = Lib.Create(cbTargetFilename.Text, tdriver, tcompress)
    target.retries = Val(tbRetries.Text)
    
    If cbFormat.Text = "(Auto Detect)" Then
        Set geom = source.get_geometry()
    Else
        For n = 0 To Lib.stdformat_count - 1
            Set geom2 = Lib.stdformat(n, name, (desc))
            If (cbFormat.Text = name + ":" + desc) Then
                Set geom = geom2
            End If
        Next n
    End If
    For cylinder = 0 To geom.cylinders - 1
        For head = 0 To geom.heads - 1
            Call target.apform(geom, cylinder, head, 229)
        Next head
    Next cylinder
    ' Real Dskform would now write the boot sector, but we don't bother
End Sub




Private Sub btnSourceBrowse_Click()
    Dim cd As MSComDlg.CommonDialog
    
    Set cd = New MSComDlg.CommonDialog
    cd.FileName = cbSourceFilename.Text
    cd.MaxFileSize = 2048   ' Set as appropriate
    cd.Flags = cdlOFNExplorer
    Call cd.ShowOpen
    cbSourceFilename.Text = cd.FileName
End Sub

Private Sub btnTargetBrowse_Click()
    Dim cd As MSComDlg.CommonDialog
    
    Set cd = New MSComDlg.CommonDialog
    cd.FileName = cbSourceFilename.Text
    cd.MaxFileSize = 2048   ' Set as appropriate
    cd.Flags = cdlOFNExplorer
    Call cd.ShowOpen
    cbSourceFilename.Text = cd.FileName

End Sub

'
' Initialise the lists of drive types, compression types and formats.
'
Private Sub Form_Load()
    Dim Lib As LIBDSK.Library
    Set Lib = New LIBDSK.Library
    Dim s As String
    Dim name As String
    Dim desc As String
    
    cbSourceFilename.AddItem ("A:")
    cbSourceFilename.AddItem ("B:")
    cbTargetFilename.AddItem ("A:")
    cbTargetFilename.AddItem ("B:")
    
    cbDriver.AddItem ("(Auto Detect)")
    cbCompression.AddItem ("(Auto Detect)")
    cbTargetCompression.AddItem ("(None)")
    Let n = 0
    While Lib.type_enum(n, s) <> False
        cbDriver.AddItem (s)
        cbTargetDriver.AddItem (s)
        n = n + 1
    Wend
    Let n = 0
    While Lib.comp_enum(n, s) <> False
        cbCompression.AddItem (s)
        cbTargetCompression.AddItem (s)
        n = n + 1
    Wend
    cbDriver.ListIndex = 0
    cbCompression.ListIndex = 0
    cbTargetDriver.ListIndex = 0
    cbTargetCompression.ListIndex = 0
    
    cbFormat.AddItem ("(Auto Detect)")
    For n = 0 To Lib.stdformat_count - 1
        Call Lib.stdformat(n, name, desc)
        cbFormat.AddItem (name + ":" + desc)
    Next n
    cbFormat.ListIndex = 0
    Lib.reporter = Me
End Sub


