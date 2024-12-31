#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "prettify.hpp" // Include your prettifyXML function
#include "Minifing.h"
#include "xml2json.h"
#include "compressor.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inputFilePath("") {
    // Create UI elements
    QPushButton *chooseFileButton = new QPushButton("Choose XML File", this);
    QPushButton *prettifyButton = new QPushButton("Prettify XML", this);
    QPushButton *saveButton = new QPushButton("Save File", this);  // Save button
    QPushButton *minifyButton = new QPushButton("Minify File", this);
    QPushButton *compressButton = new QPushButton("Compress File", this);
    QPushButton *convert2jsonButton = new QPushButton("Convert to json", this);
    fileLabel = new QLabel("No file selected", this);
    fileContent = new LargeFileViewer(this);
    fileContent->setReadOnly(true); // Allow editing if needed

    // Connect signals to slots
    connect(chooseFileButton, &QPushButton::clicked, this, &MainWindow::chooseXMLFile);
    connect(prettifyButton, &QPushButton::clicked, this, &MainWindow::prettifyXML);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);  // Save button signal
    connect(minifyButton, &QPushButton::clicked, this, &MainWindow::minifyXML);
    connect(convert2jsonButton, &QPushButton::clicked, this, &MainWindow::XML2json);
    connect(compressButton, &QPushButton::clicked, this, &MainWindow::compressXML);
    // Left layout (buttons and label)
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(fileLabel);
    buttonLayout->addWidget(chooseFileButton);
    buttonLayout->addWidget(prettifyButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(minifyButton);
    buttonLayout->addWidget(convert2jsonButton);
    buttonLayout->addWidget(compressButton);
    buttonLayout->addStretch(1);

    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setLayout(buttonLayout);

    // Splitter for layout
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(buttonWidget);   // Left part (1/4)
    splitter->addWidget(fileContent);   // Right part (3/4)

    // Set splitter sizes
    splitter->setStretchFactor(0, 1); // Left side buttons: smaller
    splitter->setStretchFactor(1, 3); // Right side txt editor: larger

    setCentralWidget(splitter);
}

void MainWindow::chooseXMLFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml);;Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        inputFilePath = fileName;
        fileLabel->setText("Selected File: " + fileName);

        // Load and display file content
        QFile file(inputFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent->setPlainText(in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the selected file!");
        }
    }
}

void MainWindow::prettifyXML() {
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Formatted XML", "", "XML Files (*.xml)");
    if (outputFileName.isEmpty()) {
        return;
    }

    try {
        // Convert QString to std::string for the function call
        ::prettifyXML(inputFilePath.toStdString(), outputFileName.toStdString());
        QFile file(outputFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent->setPlainText(in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the selected file!");
        }
        QMessageBox::information(this, "Success", "File formatted and saved to: " + outputFileName);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert the XML file: %1").arg(e.what()));
    }
}
void MainWindow::compressXML() {
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Formatted XML", "", "Huff Files (*.huff)");
    if (outputFileName.isEmpty()) {
        return;
    }

    try {
        // Convert QString to std::string for the function call
        ::compressXML(inputFilePath.toStdString(), outputFileName.toStdString());

        QMessageBox::information(this, "Success", "File formatted and saved to: " + outputFileName);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert the XML file: %1").arg(e.what()));
    }
}
void MainWindow::XML2json() {
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Formatted XML", "", "JSON Files (*.json)");
    if (outputFileName.isEmpty()) {
        return;
    }

    try {
        // Convert QString to std::string for the function call
        ::convertXmlToJson(inputFilePath.toStdString(), outputFileName.toStdString());
        QFile file(outputFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent->setPlainText(in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the selected file!");
        }
        QMessageBox::information(this, "Success", "File formatted and saved to: " + outputFileName);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert the XML file: %1").arg(e.what()));
    }
}
void MainWindow::minifyXML() {
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Formatted XML", "", "TXT Files (*.txt)");
    if (outputFileName.isEmpty()) {
        return;
    }

    try {
        // Convert QString to std::string for the function call
        ::minify_xml_file(inputFilePath.toStdString(), outputFileName.toStdString());
        QFile file(outputFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent->setPlainText(in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the selected file!");
        }
        QMessageBox::information(this, "Success", "File formatted and saved to: " + outputFileName);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert the XML file: %1").arg(e.what()));
    }
}


void MainWindow::saveFile() {
    QString outputFileName = QFileDialog::getSaveFileName(this, "Save File As", "", "Text Files (*.txt);;XML Files (*.xml);;All Files (*)");
    if (outputFileName.isEmpty()) {
        return;
    }

    QFile file(outputFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << fileContent->toPlainText(); // Save the content of QTextEdit to the file
        file.close();
        QMessageBox::information(this, "Success", "File saved successfully to: " + outputFileName);
    } else {
        QMessageBox::warning(this, "Error", "Unable to save the file!");
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // QApplication must be properly included

    MainWindow window;
    window.show();

    return app.exec();
}
