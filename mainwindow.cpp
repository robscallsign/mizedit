#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qtmisc.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTemporaryDir>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <quazip/JlCompress.h>

#include "mizedit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

namespace {

bool extractFile(const QString &filename)
{
//    QuaZip zip(filename);
//    zip.open(QuaZip::mdUnzip);

//    QuaZipFile file(&zip);

//    for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {
//        file.open(QIODevice::ReadOnly);
//        //same functionality as QIODevice::readData() -- data is a char*, maxSize is qint64
//        file.readData(data,maxSize);
//        //do something with the data
//        file.close();
//    }

//    zip.close();
    return true;
}

bool writeMission(const QString &filename, const QByteArray &mission)
{
    QuaZip zip(filename);

    zip.open(QuaZip::mdAdd);

    if (!zip.isOpen()) {
        qDebug() << "miz file is not open.";
        return false;
    }

//    qDebug() << "miz file entries: " << zip.getEntriesCount();

    QuaZipFile file(&zip);

//    if (!zip.setCurrentFile("mission")) {
//        qDebug() << "Error mission not found in .miz archive.";
//        return false;
//    }

    file.open(QIODevice::WriteOnly, QuaZipNewInfo("mission"));


    if (!file.isOpen()) {
        qDebug() << "Error, unable to open mission file for writing.";
        return false;
    }
    file.write(mission);
    file.write("Rob was here");
    file.close();

    qDebug() << "File written ok.";
    return true;
}

bool editMission(const QString &filename, QByteArray &output)
{
    QuaZip zip(filename);

    zip.open(QuaZip::mdUnzip);

    if (!zip.isOpen()) {
        qDebug() << "miz file is not open.";
        return false;
    }

    qDebug() << "miz file entries: " << zip.getEntriesCount();


    QuaZipFile file(&zip);

//    zip.goToFirstFile();
    if (!zip.setCurrentFile("mission")) {
        qDebug() << "Error mission not found in .miz archive";
        return false;
    }

    qDebug() << "Current file name inside zip archive: " << zip.getCurrentFileName();

    file.open(QIODevice::ReadOnly);

    if (!file.isOpen()) {
        qDebug() << "Error, unable to open mission file within the miz file archive.";
        return false;
    }

    auto sz = file.size();
    qDebug() << "File Size is: " << sz;

    QByteArray data = file.readAll();

//    QByteArray output;
    output.reserve(data.size()); // pre-allocate to the original data size
    QStringList stringList;
    auto sp = data.split('\n');

    bool inRequiredModules = false;
    for (int i=0; i < sp.size(); ++i) {
        if (sp[i].contains("requiredModules")) {
            qDebug() << "Found line: " << sp[i];
            inRequiredModules = true;
        }

        bool appendLine = true;
        if (inRequiredModules) {
            if (sp[i].contains("A-4E-C")) {
                qDebug() << "Found A-4E-C, ignoring this line";
                appendLine = false;
            }

            // stop searching in the required modules section
             // once the closing } is detected
            if (sp[i].contains("}")) {
                inRequiredModules = false;
            }
        }

        // build the output QByteArray
        if (appendLine) {
            output.append(sp[i]);

            // don't add a newline on the very last one I guess
            if (i != sp.size()-1) {
                output.append('\n');
            }
        }
    }

    QFile outfile("C:/temp/output_mission.txt");
    outfile.open(QIODevice::WriteOnly);
    outfile.write(output);
    outfile.close();

    QFile outfile2("C:/temp/input_mission.txt");
    outfile2.open(QIODevice::WriteOnly);
    outfile2.write(data);
    outfile2.close();

//    for (const auto &s : sp) {
//        qDebug() << s;
//    }

//    QStringList ls(data);

//    qDebug() << "String list size: " << ls.size();

//    Another way to
//    qDebug() << "lineCount: " << lineCount;
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QString sval = codec->toUnicode(data);

//    edit->setPlainText(data);
//    edit->setPlainText(data);

    return true;
}
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

    bool overwriteMission = true;
    if (reply == QMessageBox::Yes) {
        // overwrite
    } else {
        // append _mizedit to the filename before the .miz extension
        overwriteMission = false;
    }


    for (int i = 0; i < files.size(); ++i) {
        qDebug() << "Opening: " << files.at(i);
        QByteArray output;
        extractFile(files.at(i));
//        editMission(files.at(i), output);
//        writeMission(files.at(i),output);
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

