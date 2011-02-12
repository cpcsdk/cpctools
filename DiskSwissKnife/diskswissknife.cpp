/*
 DiskSwissKnife - Amstrad CPC DSK file editor

 Copyright 2010, Adrien Destugues <pulkomandy@gmail.com>
 Distributed under the terms of the MIT licence
*/

#include "diskswissknife.h"
#include "ui_diskswissknife.h"

#include <QFileDialog>

#include <cassert>

DiskSwissKnife::DiskSwissKnife(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskSwissKnife),
    currentTrack(0)
{
    ui->setupUi(this);
    delete(ui->hexView);
    ui->hexView = new QHexEdit();
    ui->sectorTab->layout()->addWidget(ui->hexView);

    // signals connection
    assert(connect(ui->sectorList, SIGNAL(itemSelectionChanged()), this, SLOT(on_changeSector())));
    assert(connect(ui->trackSpin, SIGNAL(valueChanged(int)), this, SLOT(on_changeTrack(int))));
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

#define trackpix 11

    // Update the overview
    for (unsigned int track = 0; track < currentDisk.tracks; track++)
    {
        int x = 0;
        for (unsigned int sector = 0; sector<currentDisk.track[track]->sectors; sector++)
        {
            int w = min(currentDisk.track[track][0].sector[sector].declared_size,
                        currentDisk.track[track][0].sector[sector].size)/ 8;
            // Draw the sector
            // Brush color :
                // white : normal
                // grey : erased
                // red : weak
            QBrush b;
            b.setStyle(Qt::SolidPattern);
            if (currentDisk.track[track][0].sector[sector].size !=
                currentDisk.track[track][0].sector[sector].declared_size)
                    b.setColor(Qt::red);
            else
                b.setColor(Qt::white);
            if (currentDisk.track[track][0].sector[sector].flags[1]&0x40)
                b.setColor(b.color().darker());
            s->addRect(x+8, track * trackpix, w, trackpix, Qt::SolidLine, b);

            // Draw the sector ID
            QString qstr;
            qstr.setNum(currentDisk.track[track][0].sector[sector].CHRN[2],16);
            QGraphicsSimpleTextItem* i = s->addSimpleText(qstr);
            i->setPos(x+w/2,(int)track*trackpix-1);

            x += w;
        }
    }

    // Update the file list

    // Update the sector editor
    on_changeTrack(0);
}


void DiskSwissKnife::on_changeSector()
{
    int sectornum = ui->sectorList->currentRow();
    sectornum = max(0, sectornum);
    // Update sector view
    int size = currentDisk.track[currentTrack][0].sector[sectornum].declared_size;
    unsigned char* data = currentDisk.track[currentTrack][0].sector[sectornum].data;
    QByteArray q((char*)data, size);
    ui->hexView->setData(q);
}


void DiskSwissKnife::on_changeTrack(int i)
{
    currentTrack = i;
    ui->sectorList->clear();

    for (unsigned int sector = 0; sector<currentDisk.track[currentTrack]->sectors; sector++)
    {
        QString str;
        str.setNum(currentDisk.track[currentTrack]->sector[sector].CHRN[2],16);
        ui->sectorList->addItem(str);
    }
    on_changeSector();
}
