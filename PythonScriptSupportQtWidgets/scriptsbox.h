#ifndef SCRIPTSBOX_H
#define SCRIPTSBOX_H

#include <QTextStream>
#include <QString>
#include <QFile>
#include <map>

class ScriptsBox
{
public:
    ScriptsBox();

    bool readScriptDB();
    void writeScriptDB() const;
    void setScript(QString name, QString script);
    void renameScript(QString name, QString new_name);
    QString getScript(QString name) const;
    QStringList getNames() const;

private:
    std::map<QString,QString> scripts;
    const QString pyScritpDBFileName = "sdb.sdbbm";
    QString tmp_key = "???";
};//class ScriptsBox

#endif // SCRIPTSBOX_H
