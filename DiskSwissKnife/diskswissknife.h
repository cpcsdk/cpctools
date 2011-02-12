#ifndef DISKSWISSKNIFE_H
#define DISKSWISSKNIFE_H

#include <QMainWindow>
#include <QModelIndex>

#include "../Reloaded/src/core/dsk.h"

namespace Ui {
    class DiskSwissKnife;
}

class DiskSwissKnife : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiskSwissKnife(QWidget *parent = 0);
    ~DiskSwissKnife();

private:
    Ui::DiskSwissKnife *ui;
    t_drive currentDisk;

    int currentTrack;

private slots:
    void on_actionOpen_triggered();
    void on_changeSector();
    void on_changeTrack(int);
};

#endif // DISKSWISSKNIFE_H
