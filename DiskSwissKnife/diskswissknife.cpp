/*
 DiskSwissKnife - Amstrad CPC DSK file editor

 Copyright 2010, Adrien Destugues <pulkomandy@gmail.com>
 Distributed under the terms of the MIT licence
*/

#include "diskswissknife.h"
#include "ui_diskswissknife.h"

#include <QFileDialog>

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
    QString fileToOpen = QFileDialog::getOpenFileName(this, "Select DSK file",
        "", "*.dsk");

    // Read the file
    dsk_load(fileToOpen.toLocal8Bit(), &currentDisk, 'a');

    QGraphicsScene* s = ui->overviewSectors->scene();

    if (s == NULL)
    {
        s = new QGraphicsScene(this);
        ui->overviewSectors->setScene(s);;
    }

    s->clear();

#define trackpix 10

    // Update the overview
    for (int track = 0; track < currentDisk.tracks; track++)
    {
        int x = 0;
        for (unsigned int sector = 0; sector<currentDisk.track[track]->sectors; sector++)
        {
            int w = currentDisk.track[track][0].sector[sector].declared_size / 16;
            // Draw the sector
            s->addRect(x+8, track * trackpix, w, trackpix/*pen brush*/);
            x += w;
        }
    }

    // Update the file list
    // Update the sector editor
        // check current track <= trackcount
        // Sector list for current track
        // Current sector exists ?
        // Update current sector
}
