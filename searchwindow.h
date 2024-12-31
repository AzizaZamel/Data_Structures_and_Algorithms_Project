#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H
#include <QMessageBox>
#include <QMainWindow>
#include <QString>
#include <QVector>

#include "word_topic_search.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SearchWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
   void set_xmlFile(string xmlFile );
    ~MainWindow();

private slots:
    void onSearchButtonClicked(); // Slot for handling search button clicks

private:
    Ui::SearchWindow *ui;
   string xmlFile ;
};

#endif // SEARCHWINDOW_H
