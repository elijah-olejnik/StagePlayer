#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nonedittablecolumndelegate.h"
#include "spinboxdelegate.h"
#include "wavheader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<deckState>("deckState");
    detectDevices();
    dateTimer.setInterval(20);
    connect(&dateTimer,SIGNAL(timeout()),this,SLOT(displayDateTime()));
    dateTimer.start();
    ui->lcdNumber->display("00:00:00");
    ui->lcdNumber_2->display("00:00:00");
    ui->label->setText("STOPPED");
    ui->label_2->setText("STOPPED");
    ui->label_3->setText("STOPPED");
    ui->totalTimeDisp->display("00:00:00");
    ui->totalTimeDisp_2->display("00:00:00");
    ui->volumeSlider->setSliderPosition(100);
    ui->volumeSlider2->setSliderPosition(100);
    deck1 = new Controller();
    deck2 = new Controller();
    deck3 = new Controller();
    connect(deck1,SIGNAL(currentDeckState(qint64,QTime,QTime,int,deckState,QString)),
            this,SLOT(updatePos(qint64,QTime,QTime,int,deckState,QString)));
    connect(deck2,SIGNAL(currentDeckState(qint64,QTime,QTime,int,deckState,QString)),
            this,SLOT(updatePos2(qint64,QTime,QTime,int,deckState,QString)));
    connect(deck3,SIGNAL(currentDeckState(qint64,QTime,QTime,int,deckState,QString)),
            this,SLOT(updatePos3(qint64,QTime,QTime,int,deckState,QString)));
    connect(ui->actionFiles,SIGNAL(triggered()),this,SLOT(addFiles()));
    connect(ui->actionFiles_2,SIGNAL(triggered()),this,SLOT(addFiles2()));
    connect(ui->actionLoad_wave_samples,SIGNAL(triggered()),this,SLOT(addFiles3()));
    connect(ui->actionReplace,SIGNAL(triggered()),this,SLOT(replaceFile()));
    connect(ui->actionReplace_2,SIGNAL(triggered()),this,SLOT(replaceFile2()));
    connect(ui->actionReplace_3,SIGNAL(triggered()),this,SLOT(replaceFile3()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addRow()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(itemRemove()));
    connect(ui->removeButton_2,SIGNAL(clicked()),this,SLOT(itemRemove3()));
    connect(ui->actionOpen_playlist,SIGNAL(triggered()),this,SLOT(openPlaylist()));
    connect(ui->actionOpen_Sampler_list,SIGNAL(triggered()),this,SLOT(loadList2()));
    connect(ui->actionSave_playlist,SIGNAL(triggered()),this,SLOT(saveList()));
    connect(ui->actionSave_Sampler_list,SIGNAL(triggered()),this,SLOT(saveList2()));
    connect(ui->actionClear_playlist_2,SIGNAL(triggered()),this,SLOT(itemClear()));
    connect(ui->actionClear_Sampler,SIGNAL(triggered()),this,SLOT(itemClear3()));
    connect(ui->playButton,SIGNAL(clicked()),this,SLOT(itemPlay()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(itemStop()));
    connect(ui->stopButton_2,SIGNAL(clicked()),this,SLOT(itemStop2()));
    connect(ui->SsButton,SIGNAL(clicked()),this,SLOT(itemStop3()));
    connect(ui->pauseButton,SIGNAL(clicked()),deck1,SLOT(pause()));
    connect(ui->pauseButton_2,SIGNAL(clicked()),deck2,SLOT(pause()));
    connect(ui->fadeOutButton,SIGNAL(clicked()),this,SLOT(fadeOut()));
    connect(ui->fadeOutButton_2,SIGNAL(clicked()),this,SLOT(fadeOut2()));
    connect(ui->fadeButton,SIGNAL(clicked()),this,SLOT(fade()));
    connect(ui->fadeButton_2,SIGNAL(clicked()),this,SLOT(fade2()));
    connect(ui->fadeInButton,SIGNAL(clicked()),this,SLOT(fadeIn()));
    connect(ui->seekSlider,SIGNAL(sliderMoved(int)),deck1,SLOT(search(int)));
    connect(ui->seekSlider_2,SIGNAL(sliderMoved(int)),deck2,SLOT(search(int)));
    connect(ui->volumeSlider,SIGNAL(sliderMoved(int)),deck1,SLOT(setVolume(int)));
    connect(ui->volumeSlider2,SIGNAL(sliderMoved(int)),deck2,SLOT(setVolume(int)));
    connect(ui->crossButton,SIGNAL(clicked()),this,SLOT(crossFade()));
    connect(ui->fButton,SIGNAL(clicked()),this,SLOT(f1()));
    connect(ui->fButton_2,SIGNAL(clicked()),this,SLOT(f2()));
    connect(ui->fButton_3,SIGNAL(clicked()),this,SLOT(f3()));
    connect(ui->fButton_4,SIGNAL(clicked()),this,SLOT(f4()));
    connect(ui->fButton_5,SIGNAL(clicked()),this,SLOT(f5()));
    connect(ui->fButton_6,SIGNAL(clicked()),this,SLOT(f6()));
    connect(ui->fButton_7,SIGNAL(clicked()),this,SLOT(f7()));
    connect(ui->fButton_8,SIGNAL(clicked()),this,SLOT(f8()));
    connect(ui->fButton_9,SIGNAL(clicked()),this,SLOT(f9()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(aboutToClose()));
    connect(ui->actionShow_fullscreen,SIGNAL(triggered()),this,SLOT(showFullScreen()));
    connect(ui->actionShow_normal_window,SIGNAL(triggered()),this,SLOT(showNormal()));
    connect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(moveUp()));
    connect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(moveDown()));
    connect(ui->moveLButton,SIGNAL(clicked()),this,SLOT(moveLeftRight()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionKeyboard,SIGNAL(triggered()),this,SLOT(showBindings()));
    ui->tableWidget->setItemDelegateForColumn(0,new NonEditTableColumnDelegate);
    ui->tableWidget->setItemDelegateForColumn(2,new NonEditTableColumnDelegate);
    ui->tableWidget->setItemDelegateForColumn(3,new NonEditTableColumnDelegate);
    ui->tableWidget->setItemDelegateForColumn(5,new NonEditTableColumnDelegate);
    ui->tableWidget_3->setItemDelegateForColumn(0,new NonEditTableColumnDelegate);
    ui->tableWidget->setItemDelegateForColumn(1,new SpinBoxDelegate);
    ui->tableWidget->setItemDelegateForColumn(4,new SpinBoxDelegate);
    ui->tableWidget_3->setItemDelegateForColumn(1,new SpinBoxDelegate);
    itemsResize();
    autocross = false;
    autocross2 = false;
    d = new exitDialog();
    connect(d,SIGNAL(SaveSignal()),this,SLOT(saveList()));
    connect(d,SIGNAL(SaveSampler()),this,SLOT(saveList2()));
    ui->volumeBox->setRange(0,100);
    ui->volumeBox->setSingleStep(1);
    ui->volumeBox->setValue(100);
    ui->volumeBox_2->setRange(0,100);
    ui->volumeBox_2->setSingleStep(1);
    ui->volumeBox_2->setValue(100);
    ui->timeBox->setRange(1,60);
    ui->timeBox->setValue(3);
    ui->timeBox_2->setRange(1,60);
    ui->timeBox_2->setValue(3);
    getCfg();
}

void MainWindow::detectDevices()
{
    foreach(const QAudioDeviceInfo &devInfo,QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        ui->deviceBox->addItem(devInfo.deviceName(),qVariantFromValue(devInfo));
        ui->deviceBox_2->addItem(devInfo.deviceName(),qVariantFromValue(devInfo));
        ui->deviceBox_3->addItem(devInfo.deviceName(),qVariantFromValue(devInfo));
    }
}

void MainWindow::getCfg()
{
    QFile rcfgFile(".StagePlayer.dat");
    if(!rcfgFile.exists())
    {
        return;
    }
    else
    {
        QDataStream dataIn(&rcfgFile);
        if(!rcfgFile.open(QIODevice::ReadOnly))
        {
            return;
        }
        else
        {
            int a = 0;
            int b = 0;
            int c = 0;
            dataIn >> a >> b >> c;
            ui->deviceBox->setCurrentIndex(a);
            ui->deviceBox_2->setCurrentIndex(b);
            ui->deviceBox_3->setCurrentIndex(c);
            rcfgFile.close();
        }
    }
}

void MainWindow::addFile1(QString mediaSource,QString lvl,const int currentRow)
{
    if(QFileInfo(mediaSource).exists())
    {
        if(parse(mediaSource) == true)
        {
            ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem(track.fileName));
            ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(lvl));
            ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(track.durationTime.toString("hh:mm:ss")));
            ui->tableWidget->setItem(currentRow,3,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(""));
        }
        else
        {
            ui->tableWidget->setItem(currentRow,0,
                                     new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() +
                                                          "\" HAS AN UNSUPPORTED FORMAT!"));
            ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,3,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(""));
        }
    }
    else
    {
        ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() + "\" LOST!"));
        ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,3,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(""));
    }
    itemsResize();
    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount(),0,QItemSelectionModel::SelectCurrent);
    ui->tableWidget->setFocus();
}

void MainWindow::addFile2(QString mediaSource, QString lvl, const int currentRow)
{
    if(QFileInfo(mediaSource).exists())
    {
        if(parse(mediaSource) == true)
        {
            ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,3,new QTableWidgetItem(track.fileName));
            ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(lvl));
            ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(track.durationTime.toString("hh:mm:ss")));
        }
        else
        {
            ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,3,
                                     new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() +
                                                          "\" HAS AN UNSUPPORTED FORMAT!"));
            ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(""));
            ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(""));
        }
    }
    else
    {
        ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,1,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,2,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,3,new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() + "\" LOST!"));
        ui->tableWidget->setItem(currentRow,4,new QTableWidgetItem(""));
        ui->tableWidget->setItem(currentRow,5,new QTableWidgetItem(""));
    }
    itemsResize();
    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount(),0,QItemSelectionModel::SelectCurrent);
    ui->tableWidget->setFocus();
}

void MainWindow::addFile3(QString mediaSource, QString lvl, QString lCheck, const int currentRow)
{
    if(QFileInfo(mediaSource).exists())
    {
        if(parse(mediaSource) == true)
        {
            ui->tableWidget_3->setItem(currentRow,0,new QTableWidgetItem(track.fileName));
            ui->tableWidget_3->setItem(currentRow,1,new QTableWidgetItem(lvl));
            ui->tableWidget_3->setItem(currentRow,2,new QTableWidgetItem("loop"));
            ui->tableWidget_3->item(currentRow,2)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            if(lCheck.toInt() == 1)
            {
                ui->tableWidget_3->item(currentRow,2)->setCheckState(Qt::Checked);
            }
            else
            {
                ui->tableWidget_3->item(currentRow,2)->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            ui->tableWidget_3->setItem(currentRow,0,
                                       new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() +
                                                            "\" HAS AN UNSUPPORTED FORMAT!"));
            ui->tableWidget_3->setItem(currentRow,1,new QTableWidgetItem(""));
        }
    }
    else
    {
        ui->tableWidget_3->setItem(currentRow,0,new QTableWidgetItem("\"" + QFileInfo(mediaSource).fileName() + "\" LOST!"));
        ui->tableWidget_3->setItem(currentRow,1,new QTableWidgetItem(""));
    }
    itemsResize();
}

void MainWindow::addFiles()
{
    QStringList mediaFiles;
    mediaFiles = QFileDialog::getOpenFileNames(this,"Load Audio Files", path, QString("Wave Files (*.wav)"));
    if(mediaFiles.isEmpty())
    {
        return;
    }
    else
    {
        path = QFileInfo(mediaFiles.last()).path();
        foreach(QString mediaFile,mediaFiles)
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
            addFile1(mediaFile,"100",ui->tableWidget->rowCount()-1);
        }
    }
}

void MainWindow::addFiles2()
{
    QStringList mediaFiles;
    mediaFiles = QFileDialog::getOpenFileNames(this,"Load Audio Files", path, QString("Wave Files (*.wav)"));
    if(mediaFiles.isEmpty())
    {
        return;
    }
    else
    {
        path = QFileInfo(mediaFiles.last()).path();
        foreach(QString mediaFile,mediaFiles)
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
            addFile2(mediaFile,"100",ui->tableWidget->rowCount()-1);
        }
    }
}

void MainWindow::addFiles3()
{
    QStringList mediaFiles;
    mediaFiles = QFileDialog::getOpenFileNames(this,"Load Audio Files", path, QString("Wave Files (*.wav)"));
    if(mediaFiles.isEmpty())
    {
        return;
    }
    else
    {
        path = QFileInfo(mediaFiles.last()).path();
        foreach(QString mediaFile,mediaFiles)
        {
            if(ui->tableWidget_3->rowCount() < 9)
            {
                ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount() + 1);
                addFile3(mediaFile,"100","0",ui->tableWidget_3->rowCount()-1);
            }
            else
            {
                return;
            }
        }
    }
}

void MainWindow::replaceFile()
{
    QString mediaFile;
    mediaFile = QFileDialog::getOpenFileName(this,"LoadAudioFile",path,QString("Wave File (*.wav)"));
    if(mediaFile.isNull())
        return;
    else
        addFile1(mediaFile,"100",ui->tableWidget->currentRow());
}

void MainWindow::replaceFile2()
{
    QString mediaFile;
    mediaFile = QFileDialog::getOpenFileName(this,"LoadAudioFile",path,QString("Wave File (*.wav)"));
    if(mediaFile.isNull())
        return;
    else
        addFile2(mediaFile,"100",ui->tableWidget->currentRow());
}

void MainWindow::replaceFile3()
{
    QString mediaFile;
    mediaFile = QFileDialog::getOpenFileName(this,"LoadAudioFile",path,QString("Wave File (*.wav)"));
    if(mediaFile.isNull())
        return;
    else
        addFile3(mediaFile,"100","0",ui->tableWidget_3->currentRow());
}

void MainWindow::addRow()
{
    ui->tableWidget->insertRow(ui->tableWidget->currentRow()+1);
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,0,new QTableWidgetItem(""));
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,2,new QTableWidgetItem(""));
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,3,new QTableWidgetItem(""));
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,4,new QTableWidgetItem(""));
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,5,new QTableWidgetItem(""));
}

void MainWindow::itemRemove()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::itemRemove3()
{
    ui->tableWidget_3->removeRow(ui->tableWidget_3->currentRow());
}

void MainWindow::itemClear()
{
    playList.clear();
    for(int x = ui->tableWidget->rowCount()-1; x >= 0; --x)
    {
        ui->tableWidget->removeRow(x);
    }
    for(int x = ui->tableWidget_3->rowCount()-1; x >= 0; --x)
    {
        ui->tableWidget_3->removeRow(x);
    }
}

void MainWindow::itemClear3()
{
    for(int x = ui->tableWidget_3->rowCount()-1; x >= 0; --x)
    {
        ui->tableWidget_3->removeRow(x);
    }
}

void MainWindow::openPlaylist()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Load Playlist", path,QString("List Files (*.lst)"));
    loadList(filePath);
}

void MainWindow::loadList(QString fileName)
{
    itemClear();
    if(fileName.isNull() == false)
    {
        path = QFileInfo(fileName).path();
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            wDlg = new WarningDialog();
            wDlg->display("CAN'T READ THIS FILE!");
            wDlg->exec();
            return;
        }
        else
        {
            QTextStream textStream(&file);
            QStringList lines;
            while(true)
            {
                QString line = textStream.readLine();
                lines = line.split(";");
                if(line.isNull())
                {
                    break;
                }
                else
                {
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
                    if(lines.value(0).isEmpty() == false)
                    {
                        addFile1(lines.value(0),lines.value(1),ui->tableWidget->rowCount()-1);
                    }
                    else if(lines.value(2).isEmpty() == false)
                    {
                        addFile2(lines.value(2),lines.value(3),ui->tableWidget->rowCount()-1);
                    }
                }
            }
        }
    }
    else
    {
        return;
    }
}

void MainWindow::loadList2()
{
    itemClear3();
    QString fileName = QFileDialog::getOpenFileName(this,"Load Playlist", path,QString("List Files (*.lst)"));
    if(fileName.isNull() == false)
    {
        path = QFileInfo(fileName).path();
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            wDlg = new WarningDialog();
            wDlg->display("CAN'T READ THIS FILE!");
            wDlg->exec();
            return;
        }
        else
        {
            QTextStream textStream(&file);
            QStringList lines;
            while(true)
            {
                QString line = textStream.readLine();
                lines = line.split(";");
                if(line.isNull())
                {
                    break;
                }
                else
                {
                    ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount() + 1);
                    if(lines.value(0).isEmpty() != true)
                    {
                        addFile3(lines.value(0),lines.value(1),lines.value(2),ui->tableWidget_3->rowCount()-1);
                    }
                }
            }
        }
    }
    else
    {
        return;
    }
}

void MainWindow::saveList()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save Playlist", path, QString("List File (*.lst)"));
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        wDlg = new WarningDialog();
        wDlg->display("CAN'T WRITE THIS FILE!");
        wDlg->show();
        return;
    }
    else
    {
        QTextStream out(&file);
        for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            for(int j = 0; j < playList.size(); ++j)
            {
                outputFile.setFile(playList.at(j).filePath);
                if(ui->tableWidget->item(i,0)->text() == outputFile.fileName())
                {
                    out<<playList.at(j).filePath<<";"<<ui->tableWidget->item(i,1)->text()<<";"<<";"<<endl;
                }
                else if(ui->tableWidget->item(i,3)->text() == outputFile.fileName())
                {
                    out<<";"<<";"<<playList.at(j).filePath<<";"<<ui->tableWidget->item(i,4)->text()<<endl;
                }
            }
        }
        file.close();
    }
    path = QFileInfo(fileName).path();
    ui->tableWidget->setFocus();
}

void MainWindow::saveList2()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save Playlist", path, QString("List File (*.lst)"));
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        wDlg = new WarningDialog();
        wDlg->display("CAN'T WRITE THIS FILE!");
        wDlg->exec();
        return;
    }
    else
    {
        QTextStream out(&file);
        for(int i = 0; i < ui->tableWidget_3->rowCount(); ++i)
        {
            for(int j = 0;j < playList.size();++j)
            {
                outputFile.setFile(playList.at(j).filePath);
                if(ui->tableWidget_3->item(i,0)->text() == outputFile.fileName())
                {
                    switch (ui->tableWidget_3->item(i,2)->checkState())
                    {
                    case Qt::Checked:
                        out<<playList.at(j).filePath<<";"<<ui->tableWidget_3->item(i,1)->text()<<";"<<"1"<<endl;
                        break;
                    case Qt::Unchecked:
                        out<<playList.at(j).filePath<<";"<<ui->tableWidget_3->item(i,1)->text()<<";"<<"0"<<endl;
                        break;
                    default:
                        out<<playList.at(j).filePath<<";"<<ui->tableWidget_3->item(i,1)->text()<<";"<<"0"<<endl;
                        break;
                    }
                }
            }
        }
        file.close();
    }
    path = QFileInfo(fileName).path();
    ui->tableWidget->setFocus();
}

void MainWindow::itemPlay()
{
    if(ui->tableWidget->rowCount() == 0)
    {
        wDlg = new WarningDialog();
        wDlg->display("PLAYLIST IS EMPTY, LOAD FILES FIRST!");
        wDlg->exec();
        return;
    }
    else
    {
        if((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
            ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) &&
                (ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
                 ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull()))
        {
            if(ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1)
            {
                return;
            }
            else
            {
                if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                    ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) &&
                        (ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                         ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()))
                {
                    ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                    autocross = false;
                }
                else
                {
                    if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                        ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) == false)
                    {
                        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                        autocross = false;
                    }
                    else if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                             ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()) == false)
                    {
                        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,3,QItemSelectionModel::SelectCurrent);
                        autocross = false;
                    }
                }
            }
        }
        else
        {
            if((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
                ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) == false)
            {
                Volume = ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text().toInt();
                for(int i = 0;i < playList.size();++i)
                {
                    if(playList.at(i).fileName == ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text())
                    {
                        if(QFileInfo(playList.at(i).filePath).exists())
                        {
                            ui->seekSlider->setMaximum(static_cast<int>(playList.at(i).dataLengthSecs * 1000));
                            deck1->play(&playList.at(i),ui->deviceBox->itemData(ui->deviceBox->currentIndex()).value<QAudioDeviceInfo>(),Volume,false);
                        }
                        else
                        {
                            ui->label->setText("FILE \"" + playList.at(i).fileName + "\" LOST!!!");
                            return;
                        }
                    }
                }
                ui->tableWidget->scrollToItem(ui->tableWidget->currentItem(),QAbstractItemView::PositionAtTop);
                for(int x=0;x<ui->tableWidget->rowCount();++x)
                    for(int y=0;y<ui->tableWidget->columnCount();++y)
                        ui->tableWidget->item(x,y)->setBackgroundColor(QColor::fromRgb(216, 216, 216, 255));
                for(int a=0;a<ui->tableWidget->columnCount();++a)
                    ui->tableWidget->item(ui->tableWidget->currentRow(),a)->setBackgroundColor(QColor::fromRgb(80, 240, 117, 255));
                if(ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1)
                {
                    return;
                }
                else
                {
                    if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                        ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) &&
                            (ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                             ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()))
                    {
                        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                        autocross = false;
                    }
                    else
                    {
                        if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                            ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) == false)
                        {
                            ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                            autocross = false;
                        }
                        else if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                                 ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()) == false)
                        {
                            ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,3,QItemSelectionModel::SelectCurrent);
                            autocross = true;
                        }
                    }
                }
            }
            else if((ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
                ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull()) == false)
            {
                Volume2 = ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text().toInt();
                for(int i = 0;i < playList.size();++i)
                {
                    if(playList.at(i).fileName == ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text())
                    {
                        if(QFileInfo(playList.at(i).filePath).exists())
                        {
                            ui->seekSlider_2->setMaximum(static_cast<int>(playList.at(i).dataLengthSecs * 1000));
                            deck2->play(&playList.at(i),ui->deviceBox_2->itemData(ui->deviceBox_2->currentIndex()).value<QAudioDeviceInfo>(),Volume2,false);
                        }
                        else
                        {
                            ui->label_2->setText("FILE \"" + playList.at(i).fileName + "\" LOST!!!");
                            return;
                        }
                    }
                }
                ui->tableWidget->scrollToItem(ui->tableWidget->currentItem(),QAbstractItemView::PositionAtTop);
                for(int x=0;x<ui->tableWidget->rowCount();++x)
                    for(int y=0;y<ui->tableWidget->columnCount();++y)
                        ui->tableWidget->item(x,y)->setBackgroundColor(QColor::fromRgb(216, 216, 216, 255));
                for(int a=0;a<ui->tableWidget->columnCount();++a)
                    ui->tableWidget->item(ui->tableWidget->currentRow(),a)->setBackgroundColor(QColor::fromRgb(80, 240, 117, 255));
                if(ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1)
                {
                    return;
                }
                else
                {
                    if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                        ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) &&
                            (ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                             ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()))
                    {
                        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                        autocross = false;
                    }
                    else
                    {
                        if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isEmpty() ||
                            ui->tableWidget->item(ui->tableWidget->currentRow()+1,0)->text().isNull()) == false)
                        {
                            ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,0,QItemSelectionModel::SelectCurrent);
                            autocross = true;
                        }
                        else if((ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isEmpty() ||
                                 ui->tableWidget->item(ui->tableWidget->currentRow()+1,3)->text().isNull()) == false)
                        {
                            ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow()+1,3,QItemSelectionModel::SelectCurrent);
                            autocross = false;
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::itemPlay3()
{
    ui->tableWidget_3->setCurrentCell(ui->tableWidget_3->currentRow(),1,QItemSelectionModel::SelectCurrent);
    Volume3 = ui->tableWidget_3->currentItem()->text().toInt();
    switch (ui->tableWidget_3->item(ui->tableWidget_3->currentRow(),2)->checkState())
    {
    case Qt::Checked:
        repeat = true;
        break;
    case Qt::Unchecked:
        repeat = false;
        break;
    default:
        repeat = false;
        break;
    }
    for(int i = 0;i < playList.size();++i)
    {
        if(playList.at(i).fileName == ui->tableWidget_3->item(ui->tableWidget_3->currentRow(),0)->text())
        {
            if(QFileInfo(playList.at(i).filePath).exists())
                deck3->play(&playList.at(i),ui->deviceBox_3->itemData(ui->deviceBox_3->currentIndex()).value<QAudioDeviceInfo>(),Volume3,repeat);
            else
                return;
        }
    }
}

void MainWindow::fadeOut(){deck1->fadeOut(ui->timeBox->value());}

void MainWindow::fadeOut2(){deck2->fadeOut(ui->timeBox_2->value());}

void MainWindow::fade()
{
    if(ui->tableWidget->rowCount() == 0)
        return;
    else
        deck1->fade(ui->volumeBox->value(),ui->timeBox->value());
}

void MainWindow::fade2()
{
    if(ui->tableWidget->rowCount() == 0)
        return;
    else
        deck2->fade(ui->volumeBox_2->value(),ui->timeBox_2->value());
}

void MainWindow::fadeIn()
{
    if((ui->tableWidget->rowCount() == 0) || ((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
        ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) &&
            (ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
             ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull())))
    {
        return;
    }
    else
    {
        if((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
            ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) == false)
        {
            QString tempVolume = ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),1,new QTableWidgetItem("0"));
            ui->volumeBox->setValue(tempVolume.toInt());
            itemPlay();
            QTimer::singleShot(500,this,SLOT(fade()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow() - 1,1,new QTableWidgetItem(tempVolume));
        }
        else if((ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
                 ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull()) == false)
        {
            QString tempVolume2 = ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text();
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),4,new QTableWidgetItem("0"));
            ui->volumeBox_2->setValue(tempVolume2.toInt());
            itemPlay();
            QTimer::singleShot(500,this,SLOT(fade2()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow() - 1,4,new QTableWidgetItem(tempVolume2));
        }
    }
}

void MainWindow::crossFade()
{
    if((ui->tableWidget->rowCount() == 0) || ((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
        ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) &&
            (ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
             ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull())))
    {
        return;
    }
    else
    {
        if((ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() ||
            ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isNull()) == false)
        {
            fadeOut2();
            fadeIn();
        }
        else if((ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() ||
                 ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isNull()) == false)
        {
            fadeOut();
            fadeIn();
        }
    }
}

void MainWindow::itemStop()
{
    deck1->stop();
    ui->seekSlider->setSliderPosition(0);
    autocross = false;
}

void MainWindow::itemStop2()
{
    deck2->stop();
    ui->seekSlider_2->setSliderPosition(0);
    autocross2 = false;
}

void MainWindow::itemStop3(){deck3->stop();}

void MainWindow::f1()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(0,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f2()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(1,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f3()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(2,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f4()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(3,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f5()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(4,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f6()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(5,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f7()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(6,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f8()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(7,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::f9()
{
    if(ui->tableWidget_3->rowCount() == 0)
        return;
    else
        ui->tableWidget_3->setCurrentCell(8,0,QItemSelectionModel::SelectCurrent);itemPlay3();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Asterisk: case Qt::Key_F11:
        itemStop();
        break;
    case Qt::Key_Minus: case Qt::Key_F12:
        itemStop2();
        break;
    case Qt::Key_Slash: case Qt::Key_F10:
        itemStop3();
        break;
    case Qt::Key_F1: case Qt::Key_1:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(0,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F2: case Qt::Key_2:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(1,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F3: case Qt::Key_3:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(2,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F4: case Qt::Key_4:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(3,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F5: case Qt::Key_5:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(4,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F6: case Qt::Key_6:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(5,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F7: case Qt::Key_7:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(6,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F8: case Qt::Key_8:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(7,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_F9: case Qt::Key_9:
        if(ui->tableWidget_3->rowCount() == 0)
            return;
        else
            ui->tableWidget_3->setCurrentCell(8,0,QItemSelectionModel::SelectCurrent);itemPlay3();
        break;
    case Qt::Key_Enter: case Qt::Key_Return:
        if(ui->tableWidget->hasFocus())
           itemPlay();
        else
            return;
        break;
    case Qt::Key_BracketLeft:
        deck1->pause();
        break;
    case Qt::Key_BracketRight:
        deck2->pause();
        break;
    case Qt::Key_Comma:
        fadeOut();
        break;
    case Qt::Key_Period:
        fadeOut2();
        break;
    case Qt::Key_Space:
        if(ui->tableWidget->hasFocus())
            crossFade();
        else
            return;
        break;
    case Qt::Key_F:
        if(ui->tableWidget->hasFocus())
            fadeIn();
        else
            return;
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

void MainWindow::updatePos(qint64 nPos, QTime tPostime, QTime reverTime, int nVol, deckState qCurrentState, QString plngFileName)
{
    switch (qCurrentState)
    {
    case QAudio::ActiveState:
        ui->label->setText("PLAYING " + plngFileName);
        ui->label->setStyleSheet("QLabel {background-color : lightgreen;}");
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color : lightgreen;}");
        ui->totalTimeDisp->setStyleSheet("QLCDNumber {background-color : lightgreen;}");
        break;
    case QAudio::SuspendedState:
        ui->label->setText("PAUSED " + plngFileName);
        ui->label->setStyleSheet("QLabel {background-color : yellow;}");
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color : yellow;}");
        ui->totalTimeDisp->setStyleSheet("QLCDNumber {background-color : yellow;}");
        break;
    case QAudio::StoppedState:
        ui->label->setText("STOPPED");
        ui->label->setStyleSheet("QLabel {background-color : lightgray;}");
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        ui->totalTimeDisp->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        break;
    case QAudio::IdleState:
        ui->label->setText("IDLE");
        ui->label->setStyleSheet("QLabel {background-color : lightgray;}");
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        ui->totalTimeDisp->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        break;
    case QAudio::InterruptedState:
        ui->label->setText("INTERRUPTED");
        ui->label->setStyleSheet("QLabel {background-color : lightred;}");
        ui->lcdNumber->setStyleSheet("QLCDNumber {background-color : lightred;}");
        ui->totalTimeDisp->setStyleSheet("QLCDNumber {background-color : lightred;}");
        break;
    }
    ui->lcdNumber->display(reverTime.toString("hh:mm:ss"));
    ui->totalTimeDisp->display(tPostime.toString("hh:mm:ss"));
    ui->seekSlider->setSliderPosition(static_cast<int>(nPos));
    ui->volumeSlider->setSliderPosition(nVol);
    if(qCurrentState == QAudio::IdleState)
    {
        if(ui->actionAuto_play_between_decks->isChecked())
        {
            if(autocross == true)
                itemPlay();
            else
                return;
        }
        else
        {
            return;
        }
    }
}

void MainWindow::updatePos2(qint64 nPos2, QTime tPostime2, QTime reverTime2, int nVol2, deckState qCurrentState2, QString plngFileName2)
{
    switch (qCurrentState2)
    {
    case QAudio::ActiveState:
        ui->label_2->setText("PLAYING " + plngFileName2);
        ui->label_2->setStyleSheet("QLabel {background-color : lightgreen;}");
        ui->lcdNumber_2->setStyleSheet("QLCDNumber {background-color : lightgreen;}");
        ui->totalTimeDisp_2->setStyleSheet("QLCDNumber {background-color : lightgreen;}");
        break;
    case QAudio::SuspendedState:
        ui->label_2->setText("PAUSED " + plngFileName2);
        ui->label_2->setStyleSheet("QLabel {background-color : yellow;}");
        ui->lcdNumber_2->setStyleSheet("QLCDNumber {background-color : yellow;}");
        ui->totalTimeDisp_2->setStyleSheet("QLCDNumber {background-color : yellow;}");
        break;
    case QAudio::StoppedState:
        ui->label_2->setText("STOPPED");
        ui->label_2->setStyleSheet("QLabel {background-color : lightgray;}");
        ui->lcdNumber_2->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        ui->totalTimeDisp_2->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        break;
    case QAudio::IdleState:
        ui->label_2->setText("IDLE");
        ui->label_2->setStyleSheet("QLabel {background-color : lightgray;}");
        ui->lcdNumber_2->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        ui->totalTimeDisp_2->setStyleSheet("QLCDNumber {background-color : lightgray;}");
        break;
    case QAudio::InterruptedState:
        ui->label_2->setText("INTERRUPTED");
        ui->label_2->setStyleSheet("QLabel {background-color : lightred;}");
        ui->lcdNumber_2->setStyleSheet("QLCDNumber {background-color : lightred;}");
        ui->totalTimeDisp_2->setStyleSheet("QLCDNumber {background-color : lightred;}");
        break;
    }
    ui->lcdNumber_2->display(reverTime2.toString("hh:mm:ss"));
    ui->totalTimeDisp_2->display(tPostime2.toString("hh:mm:ss"));
    ui->seekSlider_2->setSliderPosition(static_cast<int>(nPos2));
    ui->volumeSlider2->setSliderPosition(nVol2);
    if(qCurrentState2 == QAudio::IdleState)
    {
        if(ui->actionAuto_play_between_decks->isChecked())
        {
            if(autocross == true)
                itemPlay();
            else
                return;
        }
        else
        {
            return;
        }
    }
}

void MainWindow::updatePos3(qint64 /*nPos3*/, QTime /*tPostime3*/, QTime /*reverTime3*/, int /*nVol3*/, deckState qCurrentState3, QString plngFileName3)
{
    switch (qCurrentState3)
    {
    case QAudio::ActiveState:
        ui->label_3->setText("PLAYING " + plngFileName3);
        ui->label_3->setStyleSheet("QLabel {background-color : lightgreen;}");
        break;
    case QAudio::SuspendedState:
        ui->label_3->setText("PAUSED " + plngFileName3);
        ui->label_3->setStyleSheet("QLabel {background-color : yellow;}");
        break;
    case QAudio::StoppedState:
        ui->label_3->setText("STOPPED");
        ui->label_3->setStyleSheet("QLabel {background-color : lightgray;}");
        break;
    case QAudio::IdleState:
        ui->label_3->setText("IDLE");
        ui->label_3->setStyleSheet("QLabel {background-color : lightgray;}");
        break;
    case QAudio::InterruptedState:
        ui->label_3->setText("INTERRUPTED");
        ui->label_3->setStyleSheet("QLabel {background-color : lightred;}");
        break;
    }
}

void MainWindow::saveCfg()
{
    QFile wcfgFile(".StagePlayer.dat");
    QDataStream dataOut(&wcfgFile);
    if(!wcfgFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    else
    {
        dataOut << ui->deviceBox->currentIndex() << ui->deviceBox_2->currentIndex() << ui->deviceBox_3->currentIndex();
        wcfgFile.close();
    }
}

void MainWindow::aboutToClose()
{
    saveCfg();
    d->exec();
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    aboutToClose();
    event->ignore();
}

bool MainWindow::parse(QString source)
{
    QFile media;
    media.setFileName(source);
    if(!media.open(QIODevice::ReadOnly))
    {
        return false;
    }
    else
    {
        media.seek(0);
        CombinedHeader header;
        bool result = media.read(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader);
        if(result)
        {
            if((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0 || memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0) &&
                    memcmp(&header.riff.type, "WAVE", 4) == 0 &&
                    memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0 &&
                    (header.wave.audioFormat == 1 || header.wave.audioFormat == 0))
            {
                DATAHeader dataHeader;
                if(qFromLittleEndian<quint32>(header.wave.descriptor.size) > sizeof(WAVEHeader))
                {
                    quint16 extraFormatBytes;
                    if(media.peek(reinterpret_cast<char*>(&extraFormatBytes), sizeof(quint16)) != sizeof(quint16))
                        return false;
                    const qint64 throwAwayBytes = sizeof(quint16) + qFromLittleEndian<quint16>(extraFormatBytes);
                    if(media.read(throwAwayBytes).size() != throwAwayBytes)
                        return false;
                }
                if(media.read(reinterpret_cast<char*>(&dataHeader), sizeof(DATAHeader)) != sizeof(DATAHeader))
                    return false;
                if(memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0)
                    track.audioFormat.setByteOrder(QAudioFormat::LittleEndian);
                else
                    track.audioFormat.setByteOrder(QAudioFormat::BigEndian);
                int bps = qFromLittleEndian<quint16>(header.wave.bitsPerSample);
                track.audioFormat.setChannelCount(qFromLittleEndian<quint16>(header.wave.numChannels));
                track.audioFormat.setCodec("audio/pcm");
                track.audioFormat.setSampleRate(static_cast<int>(qFromLittleEndian<quint32>(header.wave.sampleRate)));
                track.audioFormat.setSampleSize(qFromLittleEndian<quint16>(header.wave.bitsPerSample));
                track.audioFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
                track.dataSize = qFromLittleEndian<quint32>(dataHeader.descriptor.size);
                track.byteRate = qFromLittleEndian<quint32>(header.wave.byteRate);
                track.dataLengthSecs = track.dataSize / track.byteRate;
                int totalhours = (track.dataLengthSecs / 60) / 60;
                int totalminutes = (track.dataLengthSecs / 60) % 60;
                int totalseconds = (track.dataLengthSecs) % 60;
                track.durationTime.setHMS(totalhours,totalminutes,totalseconds,0);
                track.dataPosition = static_cast<quint64>(media.size() - track.dataSize);
                track.fileName = QFileInfo(source).fileName();
                track.filePath = source;
                playList.append(track);
                for(int i = 0;i < playList.size();++i)
                {
                    for(int j = i +1;j < playList.size(); ++j)
                    {
                        if(playList.at(i).fileName == playList.at(j).fileName)
                        {
                            playList.removeAt(i);
                        }
                    }
                }
            }
            else
                result = false;
        }
        media.close();
        return result;
    }
}

void MainWindow::displayDateTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dateLabel->setText(QDateTime::currentDateTime().toString("ddd dd.MM.yyyy"));
}

void MainWindow::itemsResize()
{
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget_3->resizeColumnsToContents();
    ui->tableWidget->setColumnWidth(0,(ui->tableWidget->width()/2) - (ui->tableWidget->columnWidth(1) + ui->tableWidget->columnWidth(2) + 10));
    ui->tableWidget->setColumnWidth(3,(ui->tableWidget->width()/2) - (ui->tableWidget->columnWidth(4) + ui->tableWidget->columnWidth(5) + 10));
    ui->tableWidget_3->setColumnWidth(0,(ui->tableWidget_3->width()) - (ui->tableWidget_3->columnWidth(1) + ui->tableWidget_3->columnWidth(2) + 20));
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    itemsResize();
}

void MainWindow::moveUp()
{
    if(ui->tableWidget->rowCount() == 0)
    {
        return;
    }
    else
    {
        ui->tableWidget->insertRow(ui->tableWidget->currentRow() - 1);
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,0,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,1,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,2,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,3,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,4,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() - 2,5,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text()));
        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow() - 2,0,QItemSelectionModel::SelectCurrent);
        ui->tableWidget->removeRow(ui->tableWidget->currentRow() + 2);
    }
}

void MainWindow::moveDown()
{
    if(ui->tableWidget->rowCount() == 0)
    {
        return;
    }
    else
    {
        ui->tableWidget->insertRow(ui->tableWidget->currentRow() + 2);
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,0,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,1,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,2,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,3,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,4,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text()));
        ui->tableWidget->setItem(ui->tableWidget->currentRow() + 2,5,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text()));
        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow() + 2,0,QItemSelectionModel::SelectCurrent);
        ui->tableWidget->removeRow(ui->tableWidget->currentRow() - 2);
    }
}

void MainWindow::moveLeftRight()
{
    if(ui->tableWidget->rowCount() == 0 || (ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() && ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty()))
    {
        return;
    }
    else
    {
        if(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() == true && ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() == false)
        {
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),0,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),1,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),2,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),3,new QTableWidgetItem(""));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),4,new QTableWidgetItem(""));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),5,new QTableWidgetItem(""));
        }
        else if(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().isEmpty() == false && ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text().isEmpty() == true)
        {
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),3,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),4,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),5,new QTableWidgetItem(ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),0,new QTableWidgetItem(""));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),1,new QTableWidgetItem(""));
            ui->tableWidget->setItem(ui->tableWidget->currentRow(),2,new QTableWidgetItem(""));
        }
    }
}

void MainWindow::showBindings()
{
    keyB = new KeyBindings();
    keyB->exec();
    return;
}

void MainWindow::about()
{
    mInfo = new About();
    mInfo->exec();
    return;
}

MainWindow::~MainWindow()
{
    delete deck1;
    delete deck2;
    delete deck3;
    dateTimer.stop();
    delete ui;
}
