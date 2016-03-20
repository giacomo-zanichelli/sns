#include "controller.h"
#include <QDebug>
#include <QSqlQuery>

Controller::Controller(QObject *parent) : QObject(parent)
{


    connect(&main_window, SIGNAL(InserisciClicked_SIGNAL()), this, SLOT(createPersonfromInputForm()));
    connect(this, SIGNAL(inputNotValid()), &main_window, SLOT(reInsertInput()));

    connect(this, SIGNAL(newPersonRegistered()), &main_window, SLOT(on_RegistrationComplete()));
    connect(this, SIGNAL(registrationFailed()), &main_window, SLOT(on_RegistrationFailed()));
    connect(&main_window, SIGNAL(CancellaClicked_SIGNAL()), &main_window, SLOT(on_Cancella()));

    connect(&main_window, SIGNAL(VisualizzaClicked_SIGNAL(bool)), this, SLOT(fillSearchResultsTable(bool)));

    connect(&main_window, SIGNAL(ImportDatabaseDoubleClicked_SIGNAL(QString)), this, SLOT(ImportDatabase(QString)));

    connect(&main_window, SIGNAL(Visualizza_in_scadenza_button_clicked_SIGNAL(bool)), this, SLOT(fillSearchResultsTable(bool)));

    connect(&main_window, SIGNAL(setDbPath()), this, SLOT(createDbPathDialog()));
    connect(&main_window, SIGNAL(setExcelPath()), this, SLOT(createExcelPathDialog()));



    connect(&db_path_dialog, SIGNAL(showNameDialog_SIGNAL()), this, SLOT(showNameDialog()));
    connect(&db_name_dialog, SIGNAL(createDatabase_SIGNAL()), this, SLOT(createDatabase()));


}

Controller::~Controller()
{
   delete settings;
}
















