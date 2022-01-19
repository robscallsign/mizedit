#ifndef MIZEDIT_H
#define MIZEDIT_H

#include <QStringList>

class QPlainTextEdit;
namespace mizedit {

bool fileExists(const QString &path);

// create a default modules.txt file in the exe directory
bool createDefaultModulesList();

// returns the path to the modules.txt file that lives in the programs exe directory
QString getModuleFile();


// reads the modules.txt file and returns a QStringList of the modules to be excluded
QStringList getModulesList();

// Pass in a QString the full filename of the .miz file
// and it returns a QByteArray of the modified mission file
bool editMission(const QString &filename, const QStringList &modules, QByteArray &output);

bool editAllMissions(const QStringList &missions, const QStringList &modules, const QString appendName = "");


void logMessage(const QString &message, QPlainTextEdit *edit=nullptr );
}
#endif // MIZEDIT_H
