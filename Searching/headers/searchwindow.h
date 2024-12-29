#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QString>
#include <QVector>


#include "word_topic_search.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    // Function to set the XML file path from outside the class
    void set_xmlFile(string xmlFile );

    ~MainWindow();

private slots:
    // Slot for handling the search button click event
    void onSearchButtonClicked();

private:
    // UI pointer for accessing the UI components
    Ui::MainWindow *ui;

    // Variable to store the XML file path
    string xmlFile ;
};

#endif // SEARCHWINDOW_H
