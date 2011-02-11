#ifndef DISKSWISSKNIFE_H
#define DISKSWISSKNIFE_H

#include <QMainWindow>

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

private slots:
    void on_actionOpen_triggered();
};

#endif // DISKSWISSKNIFE_H
