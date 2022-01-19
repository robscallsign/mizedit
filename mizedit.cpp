#include "mizedit.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>

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
}
