#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xlsxdocument.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow main_window;
    main_window.show();

    return a.exec();
}

/*
 * TO DO
 * add new person using Excel file
 * auto email sending - int to Poco:UInt16 conversion
 *
 * confirmation dialogs:
 *      - on registration from Excel file
 * multi-file selection on registration from Excel file
 *
 */
