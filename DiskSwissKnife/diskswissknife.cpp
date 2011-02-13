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
#define pixelFactor 8.0
#define trackHead 62

    // Update the overview
    unsigned int track;
    float maxw = 0;
    for (track = 0; track < currentDisk.tracks; track++)
    {
        QString qstr;
        qstr.setNum(track, 16);
        QGraphicsSimpleTextItem* trackn = s->addSimpleText(qstr);
        trackn->setPos(1,(int)track*trackpix - 1);
        float x = 18.0;
        for (unsigned int sector = 0; sector<currentDisk.track[track]->sectors; sector++)
        {
            float w = (min(currentDisk.track[track][0].sector[sector].declared_size,
                        currentDisk.track[track][0].sector[sector].size)+trackHead)/ pixelFactor;
            // Draw the sector
            // Brush color :
                // white : normal
                // grey : erased
                // red : weak
            QBrush b;
            QColor c;
            b.setStyle(Qt::SolidPattern);
            if (currentDisk.track[track][0].sector[sector].size !=
                currentDisk.track[track][0].sector[sector].declared_size)
                    c = Qt::red;
            else
                c = Qt::white;
            if (currentDisk.track[track][0].sector[sector].flags[1]&0x40)
                c = c.darker();
            b.setColor(c);
            s->addRect(x, track * trackpix, w, trackpix, Qt::SolidLine, b);


            // Weak sectors : check if it's "partially not formatted" or
            // "missing data"
            if (currentDisk.track[track][0].sector[sector].declared_size >
                currentDisk.track[track][0].sector[sector].size)
            {
                QLinearGradient grad(w+x-8, track*trackpix, w+x, track*trackpix);
                grad.setColorAt(0,c);
                grad.setColorAt(1,Qt::white);
                QBrush brush(grad);
                s->addRect(x+w-8, track * trackpix + 1, 8, trackpix - 1, Qt::NoPen, brush);
            }


            // Draw sector header in blue
            QColor blue = Qt::blue;
            blue.setAlpha(64);
            b.setColor(blue);
            s->addRect(x, track * trackpix, trackHead/pixelFactor, trackpix, Qt::NoPen, b);

            // Draw the sector ID
            qstr.setNum(currentDisk.track[track][0].sector[sector].CHRN[2],16);
            QGraphicsSimpleTextItem* i = s->addSimpleText(qstr);
            i->setPos(x+2, (int)track*trackpix-1);

            x += w;

            // Add the GAP3 as empty space
            x += currentDisk.track[track][0].gap3/pixelFactor;
        }
        if (x > maxw) maxw = x;
    }

    // Now draw a "grey area" where there shouldn't be any useful data
    QBrush b;

    // Possible with overlapping (erasing the track id)
    b.setStyle(Qt::Dense6Pattern);
    s->addRect(6120/pixelFactor, 0, 17, 83*trackpix, Qt::NoPen, b);

    // Spec. limit (it's possible to go a bit further on some 3" drives,
    // uusally not on 3.5"
    b.setStyle(Qt::FDiagPattern);
    s->addRect(6250/pixelFactor, 0, 19, 83*trackpix, Qt::NoPen, b);

    // "World record" limit for 3".
    b.setStyle(Qt::DiagCrossPattern);
    s->addRect(6400/pixelFactor, 0, 1000, 83*trackpix, Qt::NoPen, b);

    // Ensure the view only show the useful part of the info
    s->setSceneRect(0, 0, maxw + 10, track*trackpix + 10);

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
