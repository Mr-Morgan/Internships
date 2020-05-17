#include "scriptsbox.h"

ScriptsBox::ScriptsBox()
{
}//SciptsBox();

bool ScriptsBox::readScriptDB()
{
    QFile file(pyScritpDBFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream s(&file);
        QString tmp = s.readLine();
        if (tmp != "#@SDBBM") return false;
        QString name = "??", script = "??";
        for (tmp = s.readLine(); !s.atEnd(); tmp = s.readLine()) {
            if (tmp == "#@N") name = s.readLine();
            if (tmp == "#@{") {
                script = "";
                for (tmp = s.readLine(); ; tmp = s.readLine())
                    if (tmp != "#@}") script += tmp + '\n';
                    else {
                        scripts[name] = script;
                        break;
                    }//if (tmp != "#@}")
            }//if (tmp == "#@{")
        }//for (tmp = s.readLine(); !s.atEnd(); tmp = s.readLine())
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    return true;
}//bool readScriptDB();

void ScriptsBox::writeScriptDB() const
{
    QFile file(pyScritpDBFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << "#@SDBBM\n";
        for (auto i : scripts)
            s << "#@N\n" << i.first << "\n#@{\n" << i.second << "\n#@}\n";
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
}//bool writeScriptDB() const;

void ScriptsBox::setScript(QString name, QString script)
{
    scripts[name] = script;
}//void setScript(QString name, QString script);

void ScriptsBox::renameScript(QString name, QString new_name)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    auto it = scripts.find(name);
    if (it != scripts.cend())
        if (scripts.try_emplace(new_name, it->second).second)
            scripts.erase(it);
}//void renameScript(QString name, QString new_name);

QString ScriptsBox::getScript(QString name) const
{
    return scripts.at(name);
}//QString getScript(QString name) const;

QStringList ScriptsBox::getNames() const
{
    QStringList names;
    for (auto i : scripts) names << i.first;
    return names;
}//QStringList getNames() const;
