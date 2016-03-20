#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmationDialog;
}

class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmationDialog(QWidget *parent = 0);
    ~ConfirmationDialog();

    void success();
    void failed();
    void updated();
    void peopleSuccessfullyImported(const int row);
    void emailParamsSaved();
    void emailParamsNotSaved();
    void databaseNotReady();
    void databasePathSaved();
    void excelPathSaved();
    void excelPathNotSaved();
    void emailNotReady();
    void excelNotReady();
    void mailSent();
    void emailInvalid();
    void emailContentInvalid();
    void automaticNotificationsSent(const int sent);


private slots:
    void on_OK_button_clicked();

private:
    Ui::ConfirmationDialog *ui;
};

#endif // CONFIRMATIONDIALOG_H
