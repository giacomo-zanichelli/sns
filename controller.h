#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSettings>
#include "mainwindow.h"

#include "person.h"
#include "dbmanager.h"
#include "validator.h"
#include "databasename.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    ~Controller();

    void registerNewPerson(Person *person);

    void loadSettings();

signals:
    void inputNotValid();
    void newPersonRegistered();
    void registrationFailed();

public slots:
    void createPersonfromInputForm();

    void fillSearchResultsTable(bool year);

    void ImportDatabase(const QString& filepath);

    void createDbPathDialog();
    void createExcelPathDialog();

    void saveDbPath(QString db_path);
    void saveExcelPath(QString excel_path);

    void showNameDialog();
    void createDatabase(QString db_name);

private:


    void ImportDatabaseHelper(int& count, const QString &filepath);
};

#endif // CONTROLLER_H
