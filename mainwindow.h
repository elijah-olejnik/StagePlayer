#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <QDateTime>
#include <QCheckBox>
#include <exitdialog.h>
#include <deck.h>
#include <keybindings.h>
#include <about.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void itemsResize();
    
private:
    Ui::MainWindow *ui;
    QString path;
    int Volume,Volume2,Volume3;
    bool autocross, autocross2,repeat;
    exitDialog *d;
    KeyBindings *keyB;
    About *mInfo;
    Controller *deck1,*deck2,*deck3;
    QTimer dateTimer;
    PlayListItem track;
    QList<PlayListItem> playList;
    QFileInfo outputFile;
    WarningDialog *wDlg;
    bool parse(QString source);
    void addFile1(QString mediaSource, QString lvl, const int currentRow);
    void addFile2(QString mediaSource, QString lvl, const int currentRow);
    void addFile3(QString mediaSource, QString lvl, QString lCheck, const int currentRow);
    void loadList(QString fileName);
    void resizeEvent(QResizeEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *event);

private slots:
    void detectDevices();
    void getCfg();
    void saveCfg();
    void addFiles();
    void addFiles2();
    void addFiles3();
    void replaceFile();
    void replaceFile2();
    void replaceFile3();
    void addRow();
    void itemRemove();
    void itemRemove3();
    void itemClear();
    void itemClear3();
    void loadList2();
    void openPlaylist();
    void saveList();
    void saveList2();
    void itemPlay();
    void itemPlay3();
    void itemStop();
    void itemStop2();
    void itemStop3();
    void fadeOut();
    void fadeOut2();
    void fade();
    void fadeIn();
    void fade2();
    void crossFade();
    void updatePos(qint64 nPos, QTime tPostime, QTime reverTime, int nVol, deckState qCurrentState, QString plngFileName);
    void updatePos2(qint64 nPos2,QTime tPostime2,QTime reverTime2,int nVol2,deckState qCurrentState2, QString plngFileName2);
    void updatePos3(qint64 nPos3,QTime tPostime3,QTime reverTime3,int nVol3,deckState qCurrentState3,QString plngFileName3);
    void aboutToClose();
    void displayDateTime();
    void f1();
    void f2();
    void f3();
    void f4();
    void f5();
    void f6();
    void f7();
    void f8();
    void f9();
    void moveUp();
    void moveDown();
    void moveLeftRight();
    void showBindings();
    void about();
};

#endif // MAINWINDOW_H
