#ifndef SCRIPTSBOX_H
#define SCRIPTSBOX_H

#include <QKeySequence>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <map>

class ScriptsBox
{
    /*Структура скрипта:
     * #@N
     * <имя>
     * #@HK
     * <горячие клавиши>
     * #@IM
     * <модификатор языка><пробел><режим вставки>
     * #@{
     * <код скрипта>
     * #@}
     * Имя: произвольная строка
     * Горячие клавиши: "Ctrl+O", "Ctrl+Shift+A", "Alt+L" and "Meta+F1"
     * Модификатор языка: P (Python)
     * Режимы вставки: D (default); S (save selecting mode).
     * Код скрипта: произвольный набор символов
     * */
public:
    enum InputMode { IM_Default, IM_SaveSelection, IM_Buffer};
    enum ScriptLanguage { SL_Python };

    ScriptsBox();
    ~ScriptsBox();

    InputMode getInputMode(QString name) const;
    QKeySequence getKeySequence(QString name) const;
    QStringList getNames() const;
    QString getScript(QString name) const;
    ScriptLanguage getScriptLanguage(QString name) const;
    bool readScriptDB();
    bool renameScript(QString name, QString new_name);
    void setInputMode(QString name, InputMode mode);
    void setKeySequence(QString name, QKeySequence keys);
    void setScript(QString name, QString script);
    void setScriptLanguage(QString name, ScriptLanguage language);
    void writeScriptDB() const;


private:
    std::map<QString,QString> scripts;
    const QString pyScritpDBFileName = "sdb.sdbbm";
};//class ScriptsBox

#endif // SCRIPTSBOX_H
