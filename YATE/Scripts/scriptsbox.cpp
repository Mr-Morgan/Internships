#include "scriptsbox.h"

#define EOHK(name)  scripts.at(name).indexOf('\n')                                  //End of hot keys
#define EOIM(name)  scripts.at(name).indexOf('\n', EOHK(name)+1)                    //End of input mode
#define HK(name)    scripts.at(name).left(EOHK(name))                               //Hot keys - QString
#define IMS(name)   scripts.at(name).mid(EOHK(name)+1, EOIM(name)-EOHK(name)-1)     //Input mode - QString
#define EOL(name)   IMS(name).indexOf(' ')                                          //End of language in Input mode string
#define L(name)     IMS(name).left(EOL(name)).toUInt()                              //Language - unsigned
#define IM(name)    IMS(name).right(IMS(name).length()-EOL(name)-1).toUInt()        //Input mode - unsigned
#define SCR(name)   scripts.at(name).right(scripts.at(name).length()-EOIM(name)-1)  //Script - QString

ScriptsBox::ScriptsBox()
{
    readScriptDB();
}//SciptsBox();

ScriptsBox::~ScriptsBox()
{
    writeScriptDB();
}//~ScriptsBox();

bool ScriptsBox::readScriptDB()
{
    QFile file(pyScritpDBFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream s(&file);
        QString tmp = s.readLine();
        if (tmp != "#@SDBBM") return false;
        QString name = "??", script = "";
        for (tmp = s.readLine(); !s.atEnd(); tmp = s.readLine()) {
            if (tmp == "#@N") name = s.readLine();
            if (tmp == "#@HK") script += s.readLine() + '\n';
            if (tmp == "#@IM") script += s.readLine() + '\n';
            if (tmp == "#@{") {
                for (tmp = s.readLine(); ; tmp = s.readLine())
                    if (tmp != "#@}") script += tmp + '\n';
                    else {
                        scripts[name] = script.left(script.length()-1);
                        script = "";
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
            s << "#@N\n"    << i.first          //Name
              << "\n#@HK\n" << HK(i.first)      //Hot keys
              << "\n#@IM\n" << IMS(i.first)     //Input mode
              << "\n#@{\n"  << SCR(i.first)     //Script
              << "\n#@}\n";
        file.close();
    }//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
}//bool writeScriptDB() const;

void ScriptsBox::setScript(QString name, QString script)
{
    scripts[name] = script;
}//void setScript(QString name, QString script);

void ScriptsBox::setScriptLanguage(QString name, ScriptsBox::ScriptLanguage language)
{
    scripts.at(name).remove(EOHK(name)+1, EOL(name));
    scripts.at(name).insert(EOHK(name)+1, QString::number(language));
}//void setScriptLanguage(QString name, ScriptLanguage language);

bool ScriptsBox::renameScript(QString name, QString new_name)
{
    auto it = scripts.find(new_name);
    if (it == scripts.cend()) return false;
    it = scripts.find(name);
    if (it != scripts.cend())
        if (scripts.try_emplace(new_name, it->second).second)
            scripts.erase(it);
    return true;
}//void renameScript(QString name, QString new_name);

QString ScriptsBox::getScript(QString name) const
{
    return SCR(name);
}//QString getScript(QString name) const;

ScriptsBox::ScriptLanguage ScriptsBox::getScriptLanguage(QString name) const
{
    return static_cast<ScriptLanguage>(L(name));
}//ScriptLanguage getScriptLanguage(QString name) const;

QStringList ScriptsBox::getNames() const
{
    QStringList names;
    for (auto i : scripts) names << i.first;
    return names;
}//QStringList getNames() const;

QKeySequence ScriptsBox::getKeySequence(QString name) const
{
    return QKeySequence(HK(name));
}//QKeySequence getKeySequence(QString name) const;

ScriptsBox::InputMode ScriptsBox::getInputMode(QString name) const
{
    return static_cast<InputMode>(IM(name));
}//InputMode getInputMode(QString name) const;

void ScriptsBox::setKeySequence(QString name, QKeySequence keys)
{
    scripts.at(name).remove(0, EOHK(name));
    scripts.at(name).prepend(keys.toString());
}//void setKeySequence(QString name, QKeySequence keys);

void ScriptsBox::setInputMode(QString name, InputMode mode)
{
    scripts.at(name).remove(EOL(name)+1, EOIM(name)-EOL(name)-1);
    scripts.at(name).insert(EOL(name)+1, QString::number(mode));
}//void setInputMode(QString name, InputMode mode);
