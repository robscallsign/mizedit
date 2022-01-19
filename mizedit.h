#ifndef MIZEDIT_H
#define MIZEDIT_H

#include <QStringList>

namespace mizedit {

bool fileExists(const QString &path);

// create a default modules.txt file in the exe directory
bool createDefaultModulesList();

// returns the path to the modules.txt file that lives in the programs exe directory
QString getModuleFile();


// reads the modules.txt file and returns a QStringList of the modules to be excluded
QStringList getModulesList();


}
#endif // MIZEDIT_H
