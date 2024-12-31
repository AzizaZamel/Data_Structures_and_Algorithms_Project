#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include <QPlainTextEdit>
#include "largefileviewer.h"
#include "searchwindow.h"
#include "word_topic_search.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void chooseXMLFile();
    void prettifyXML();
    void saveFile();  // Slot for saving the displayed content
    void minifyXML();
    void XML2json();
    void compressXML();
    void openSearchWindow();
    void checkXML();
    void graphXML();
private:
    QString inputFilePath;   // Store the selected XML file path
    QLabel *fileLabel;       // Label to display the file name
    LargeFileViewer *fileContent;  // Text edit to display the content of the file
    SearchWindow *searchWindow;
};

#endif // MAINWINDOW_H
