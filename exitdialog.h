#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QDialog>

namespace Ui {
class exitDialog;
}

class exitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit exitDialog(QWidget *parent = nullptr);
    ~exitDialog();

private:
    Ui::exitDialog *ui;

private slots:
    void closeaccept();
    void saveslot();
    void savesamplerslot();

signals:
    void SaveSignal();
    void SaveSampler();
};

#endif // EXITDIALOG_H
