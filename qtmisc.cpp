#include "qtmisc.h"

#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QSettings>
#include <QUuid>


namespace qtutils {

QString getDefaultDirectory(const QString &dir) {
  QSettings settings;
  QString defaultDirectory;

  if (dir.isEmpty()) {
    defaultDirectory = settings.value("defaultDirectory").toString();
  }
  else {
    defaultDirectory = dir;
  }

  if (QDir(defaultDirectory).exists()) {
    QDir::setCurrent(defaultDirectory);
  } else {
    defaultDirectory = QDir::homePath();
  }
  return defaultDirectory;
}

void setDefaultDirectory(const QString &fileName)
{
  QSettings settings;
  settings.setValue("defaultDirectory", QFileInfo(fileName).absolutePath());
}

std::string getUuid()
{
  return QUuid::createUuid().toString().toStdString();
}

// Return directory under "directoryID" from the registry.  If it doesn't exist
// return the users home directory path
QString getDirectory(const QString &directoryID, const QString &defaultValue)
{

  QSettings settings;
  QString retrievedDir = settings.value(directoryID).toString();


  if (retrievedDir.isEmpty()) {
    retrievedDir = defaultValue;
  }

  if (!QDir(retrievedDir).exists()) {
    retrievedDir = QDir::homePath();
  }

  return retrievedDir;
}

void setDirectory(const QString &directoryID, const QString &directoryName)
{
  QSettings settings;
  settings.setValue(directoryID, QFileInfo(directoryName).absolutePath());
}

// qtOpenFile - Create a file open dialog and save the directory containing that
// file into "defaultDirectory" QSettings location. Requires that an application/organization
// name are set.  This will write either the registry, or an ini file depending on the OS.
// returns an empty string if the user cancelled.  Does no error/existance checking on the file
//QString qtOpenFile(QWidget *parent, const QString &caption, const QString &dir,
//                   const QString &filter, QString *selectedFilter, QFileDialog::Options options)
//{
//  QString defaultDirectory = getDefaultDirectory(dir);

//  QString fileName = QFileDialog::getOpenFileName(parent, caption,defaultDirectory,
//                                                  filter, selectedFilter,options);
//  if (fileName.isEmpty())
//    return QString("");



//  return fileName;
//}

//QString QtSaveFile(QWidget *parent, const QString &caption, const QString &dir,
//                   const QString &filter, QString *selectedFilter, QFileDialog::Options options)
//{
//  QString defaultDirectory = getDefaultDirectory(dir);

//  QString QFileDialog::getSaveFileName(parent,caption,defaultDirectory, filter, selectedFilter, options);

//  return QString("");
//}

}
