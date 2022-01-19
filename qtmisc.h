#pragma once
#include <QString>

#include <string>

namespace qtutils {
//QString qtOpenFile(QWidget * parent = 0, const QString & caption = QString(),
//                   const QString & dir = QString(), const QString & filter = QString(), QString *
//                   selectedFilter = 0, QFileDialog::Options options = 0);

QString getDefaultDirectory(const QString &dir = QString());

void setDefaultDirectory(const QString &fileName);
std::string getUuid();

// store a directory using QSettings.  The directory is associated with an identifier
QString getDirectory(const QString &directoryID, const QString &defaultValue="");
void setDirectory(const QString &directoryID, const QString &directoryName);
}

