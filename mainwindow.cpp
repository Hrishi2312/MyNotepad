#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <form.h>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QLabel>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, new Form(this), "untitled");
    timeout = 1000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    ui->tabWidget->insertTab(ui->tabWidget->count(), new Form(this), "untitled");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    ui->statusBar->showMessage("Opened a New Empty File.", timeout);
}

void MainWindow::on_actionOpen_triggered()
{
    QString prev = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    bool closed = false;
    if(QString::compare(prev, "untitled") == 0 && ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->toPlainText().isEmpty()) {
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
        closed = true;
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        ui->statusBar->showMessage("File couldn't be opened.", timeout);
        if(closed)
            ui->tabWidget->addTab(new Form(this), "untitled");
        return ;
    }
    QFileInfo info(fileName);
    QString title(info.fileName());
    ui->tabWidget->insertTab(ui->tabWidget->count(), new Form(this), title);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    ui->tabWidget->currentWidget()->findChild<QLabel *>("filePath")->setText(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->setText(text);
    ui->statusBar->showMessage("\"" + title + "\" was opened.", timeout);
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString title = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(QString::compare(title, "untitled") == 0)
        on_actionSave_as_triggered();
    else{
        QString fileName = ui->tabWidget->currentWidget()->findChild<QLabel *>("filePath")->text();
        QFile file(fileName);
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            ui->statusBar->showMessage("File couldn't be saved.", timeout);
            return ;
        }
        QTextStream out(&file);
        QString text = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->toPlainText();
        out << text;
        file.flush();
        file.close();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filters("Text Files (*.txt);;All Files (*.*)");
    QString defaultFilter("Text Files (*.txt)");
    QString fileName = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filters, &defaultFilter);
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        ui->statusBar->showMessage("File couldn't be saved.", timeout);
        return ;
    }
    QFileInfo info(fileName);
    QString title(info.fileName());
    ui->tabWidget->currentWidget()->findChild<QLabel *>("filePath")->setText(fileName);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), title);
    QTextStream out(&file);
    QString text = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->toPlainText();
    out << text;
    file.flush();
    file.close();
    ui->statusBar->showMessage("\"" + title + "\" saved.", timeout);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Rejected)
        return ;
    else
        ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->print(&printer);
}

void MainWindow::on_actionQuit_triggered()
{
    if(QMessageBox::question(this, "Quit", "All Unsaved Data will be lost.\nAre you sure you want to quit?") == QMessageBox::Yes)
        QApplication::quit();
}

void MainWindow::on_actionCut_triggered()
{
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->redo();
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if(ok){
        ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->setFont(font);
    }else
        return ;
}

void MainWindow::on_actionText_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Set Text Color");
    if(color.isValid())
        ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->setTextColor(color);
    else
        return ;
}

void MainWindow::on_actionHighlight_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Set Highlight Color");
    if(color.isValid())
        ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->setTextBackgroundColor(color);
    else
        return ;
}

void MainWindow::on_actionBackground_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Set Background Color");
    if(color.isValid()){
        QPalette palette = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->palette();
        palette.setColor(QPalette::Base, color);
        ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit")->setPalette(palette);
    }else
        return ;
}

void MainWindow::on_actionAbout_My_Notepad_triggered()
{
    QString str = "Created by: Hrishikesh Agrawal\nCreated On: 02/06/2019";
    QMessageBox::about(this, "About \"My Notepad\"", str);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionClose_Tab_triggered()
{
    if(ui->tabWidget->count() > 1){
        QMessageBox::StandardButton response = QMessageBox::question(this, "Close Tab", "What do you want to do?", QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        if(response == QMessageBox::Save)
            on_actionSave_triggered();
        else if(response == QMessageBox::No){
            ui->statusBar->showMessage("Closed the \"" + ui->tabWidget->tabText(ui->tabWidget->currentIndex()) + "\"" + " tab.", timeout);
            ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
        }else
            ui->statusBar->showMessage("Nothing was closed.", timeout);
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    on_actionClose_Tab_triggered();
}
