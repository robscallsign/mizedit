#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qtmisc.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTemporaryDir>

#include "mizedit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MizEdit v0.1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

namespace {

}
void MainWindow::on_actionOpen_Miz_File_triggered()
{
    auto modules = mizedit::getModulesList();

    for (const auto &s : modules) {
        qDebug() << "Removing: " << s << " as a required module.";
    }

    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Select .miz files to use",
                qtutils::getDefaultDirectory(),
                "MIZ (*.miz)");

    if (files.empty()) {
        return;
    }


    qtutils::setDefaultDirectory(files.at(0));

    auto reply = QMessageBox::question(this, "Overwrite Existing Mission Files?", "Do you want to overwrite the existing Mission Files?  Yes will over write.  No will append _mizedit.miz to the name.",
                                  QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);




    if (reply == QMessageBox::Cancel)  {
        qDebug() << "Cancelled.  Stop.";
        return;
    }

    QString appendName = "_mizedit";
    if (reply == QMessageBox::Yes) {
        appendName = "";
    }

    // set the QPlainTextEdit widget for the message log
    mizedit::logMessage("",ui->plainTextEdit);

    const auto modsToIgnore = mizedit::getModulesList();
    if (!mizedit::editAllMissions(files, modsToIgnore, appendName)) {
        // something wrong happened, popup a QMessageBox
        mizedit::logMessage("Some error occured.");
    }
}


void MainWindow::on_actionEdit_Module_L_ist_triggered()
{
   auto modulesFile = mizedit::getModuleFile();

   // if exePath + modules.txt file does not exist create a new empty one
   if (!mizedit::fileExists(modulesFile)) {
       qDebug() << "modules list does not exist, creating a new empty one";
       mizedit::createDefaultModulesList();
   }

   // open the exePath + modules.txt file in the registered external editor of this OS
   QDesktopServices::openUrl(mizedit::getModuleFile());
}

