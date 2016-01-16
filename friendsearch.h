#ifndef FRIENDSEARCH_H
#define FRIENDSEARCH_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class friendsearch;
}

class friendsearch : public QDialog
{
    Q_OBJECT

public:
    explicit friendsearch(QWidget *parent = 0);
    ~friendsearch();
    QStringList namelister;

public slots:
    void name_in(QString name);

signals:
    void search_keyout(QString name);
     void friend_request(QString name);
private slots:
    void on_lineEdit_returnPressed();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::friendsearch *ui;


};

#endif // FRIENDSEARCH_H
