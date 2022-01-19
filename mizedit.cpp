#include "mizedit.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QString>
#include <QTemporaryDir>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <quazip/JlCompress.h>


// anonymous name space contains helper/utility functions that aren't
// exposed through the header file/interface
namespace {



bool extractMissionFile(const QString &missionFile) {

    if (mizedit::fileExists(missionFile)) {
        qDebug() << "Mission file: " << missionFile << " Does not exist.";
        return false;
    }
    return true;
}
}

namespace mizedit {

bool fileExists(const QString &path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}


bool createDefaultModulesList()
{
    auto modulesFile = mizedit::getModuleFile();
    QFile newFile(modulesFile);
    newFile.open(QIODevice::WriteOnly);

    // TODO - check that these are the correct spellings of these modules we want to remove
    newFile.write("T-45\nA-4E");
    return true;
}

QString getModuleFile()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("modules.txt");
}

QStringList getModulesList() {
    auto moduleFile = getModuleFile();
    qDebug() << "Module file: " << moduleFile;

    QStringList stringList;

    QFile file(moduleFile);

    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);

    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull()) {
            break;
        } else {
            if (!line.simplified().isEmpty()) {
                stringList.append(line);
            }
        }
    }
    return stringList;
}


bool editMission(const QString &filename, const QStringList &modules, QByteArray &output)
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
            for (int m=0; m < modules.size(); ++m) {
                const std::string moduleToIgnore = modules.at(m).toStdString();
                if (sp[i].contains(moduleToIgnore.c_str())) {
                    qDebug() << "Ignoring: " << modules.at(m);
                    appendLine = false;
                }
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


    // some debugging output

    bool debugging = false;

    if (debugging) {
        QFile outfile("C:/temp/output_mission.txt");
        outfile.open(QIODevice::WriteOnly);
        outfile.write(output);
        outfile.close();

        QFile outfile2("C:/temp/input_mission.txt");
        outfile2.open(QIODevice::WriteOnly);
        outfile2.write(data);
        outfile2.close();
    }

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

// isn't actually used any more - was trying to edit the mission inside the zip archive
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

bool editAllMissions(const QStringList &missions, const QStringList &modules, const QString appendName)
{

    for (int i=0; i < missions.size(); ++i) {
        QString missionFile = missions.at(i);

        logMessage(missionFile);
        QFileInfo info(missionFile);

        qDebug() << info.canonicalPath();
        qDebug() << info.completeBaseName();
        qDebug() << info.suffix();

        QString outputFilename = info.completeBaseName() + appendName + QString(".") + info.suffix();
        qDebug() << "Final output filename: " << outputFilename;


        QByteArray modifiedMission;
        if (!editMission(missionFile, modules, modifiedMission)) {
            qDebug() << "Error reading the mission file from the zip archive  and modifying it.";
            logMessage("Error - could not find and edit the mission file within the .miz archive.");
            continue;
            //            return false;
        }

        // Now that we have the modified mission
        // 1.) extract the .miz archive to a temporary folder
        QTemporaryDir dir;

        if (!dir.isValid()) {
            qDebug() << "Temporary dir is not valid, put up an error message";
            logMessage("Error creating temporary directory.");
            //            continue;
            return false;
        }

        qDebug() << "Temporary filepath: ";
        qDebug() << dir.path();

        auto results = JlCompress::extractDir(missionFile, dir.path());

        if (results.empty()) {
            qDebug() << "failed to extract mission file to temp folder";
            logMessage("Failed to extract the .miz file archive to temporary folder.");
            continue;
        }

        QString tempMissionFile = dir.filePath("mission");

        if (!fileExists(tempMissionFile)) {
            qDebug() << "Error - temporary  mission file does not exist.";
            logMessage("Error - mission file does not exist in the temp folder.");
            continue;
        }

        // Write the modified mission to the temporary folder
        QFile outfile(tempMissionFile);
        outfile.open(QIODevice::WriteOnly);
        outfile.write(modifiedMission);
        outfile.close();

        // Build the destination filename:
        QDir outputDir(info.canonicalPath());
        QString destinationFile = outputDir.filePath(outputFilename);
        qDebug() << "writing new zip file to: " << destinationFile;

        logMessage("--->" + destinationFile);
        if (!JlCompress::compressDir(destinationFile, dir.path(), true)) {
            qDebug() << "Error zipping the mission file: " << destinationFile;
            logMessage("Error creating the new .miz file archive.");
            continue;
        }
    }

    return true;
}

void logMessage(const QString &message, QPlainTextEdit *edit)
{
    static QPlainTextEdit *sedit;

    if (edit != nullptr) {
        sedit = edit;
    }

    if (sedit == nullptr) {
        return;
    }

    if (!message.isEmpty()) {
        sedit->insertPlainText(message);
        sedit->insertPlainText("\n");
    }
}



}
