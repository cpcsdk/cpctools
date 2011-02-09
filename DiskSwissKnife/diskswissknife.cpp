/*
 DiskSwissKnife - Amstrad CPC DSK file editor

 Copyright 2010, Adrien Destugues <pulkomandy@gmail.com>
 Distributed under the terms of the MIT licence
*/

#include "diskswissknife.h"
#include "ui_diskswissknife.h"

#include <QFileDialog>

#include "../iDSK/src/GestDsk.h"


DiskSwissKnife::DiskSwissKnife(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskSwissKnife)
{
    ui->setupUi(this);
}

DiskSwissKnife::~DiskSwissKnife()
{
    delete ui;
}

void DiskSwissKnife::on_actionOpen_triggered()
{
    // Open a DSK file
    QString fileToOpen = QFileDialog::getOpenFileName(this, "Select DSK file", "", "*.dsk");

    // Read the file
    DSK d;
    d.ReadDsk(fileToOpen.toStdString());

    // Update the overview
    // Update the file list
    // Update the sector editor
        // check current track <= trackcount
        // Sector list for current track
        // Current sector exists ?
        // Update current sector
}
