#ifndef BROWSEFILEDIALOG_H
#define BROWSEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class BrowseFileDialog;
}

class BrowseFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseFileDialog(QWidget *parent = 0);
    ~BrowseFileDialog();

private:
    Ui::BrowseFileDialog *ui;
};

#endif // BROWSEFILEDIALOG_H
