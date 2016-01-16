#ifndef FRIENDREQUEST_H
#define FRIENDREQUEST_H

#include <QDialog>
#include <QMessageBox>
#include<QKeyEvent>
namespace Ui {
class friendrequest;
}

class friendrequest : public QDialog
{
    Q_OBJECT

public:
    explicit friendrequest(QWidget *parent = 0);
    ~friendrequest();

private:
    Ui::friendrequest *ui;
public slots:
    void new_requests(QString name);
signals:
     void search_requests();
     void accept_request(QString name);
      void friend_delete(QString name);
private slots:
     void on_pushButton_clicked();
     void on_listWidget_doubleClicked(const QModelIndex &index);
protected:
    bool eventFilter(QObject *, QEvent *);
};

#endif // FRIENDREQUEST_H
