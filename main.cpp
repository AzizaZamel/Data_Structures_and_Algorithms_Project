#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "prettify.hpp" // Include your prettifyXML function
#include "Minifing.h"
#include "xml2json.hpp"
#include "compressor.h"
#include "xml_checker.hpp"
#include "xml_helper.hpp"
#include "graph.hpp"
#include <QProcess>

void generateGraphImage() {
    QProcess process;
    process.start("dot", QStringList() << "-Tpng" << "graph.dot" << "-o" << "graph.png");
    process.waitForFinished();

    // Check for errors
    if (process.exitStatus() == QProcess::CrashExit) {
        qDebug() << "Error: Graphviz failed to generate image!";
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inputFilePath("") {
    // Create UI elements
    QPushButton *chooseFileButton = new QPushButton("Choose XML File", this);
    QPushButton *prettifyButton = new QPushButton("Prettify XML", this);
    QPushButton *saveButton = new QPushButton("Save File", this);  // Save button
    QPushButton *minifyButton = new QPushButton("Minify File", this);
    QPushButton *compressButton = new QPushButton("Compress File", this);
    QPushButton *convert2jsonButton = new QPushButton("Convert to json", this);
    QPushButton *searchButton = new QPushButton("Search", this);
    QPushButton *checkButton = new QPushButton("Check consistency", this);
    QPushButton *graphButton = new QPushButton("Visualize", this);
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
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::openSearchWindow);
    connect(checkButton, &QPushButton::clicked, this, &MainWindow::checkXML);
    connect(graphButton, &QPushButton::clicked, this, &MainWindow::graphXML);
    // Left layout (buttons and label)
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(fileLabel);
    buttonLayout->addWidget(chooseFileButton);
    buttonLayout->addWidget(prettifyButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(minifyButton);
    buttonLayout->addWidget(convert2jsonButton);
    buttonLayout->addWidget(compressButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(checkButton);
    buttonLayout->addWidget(graphButton);
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

void MainWindow::graphXML(){
    Graph *g = generateGraph(inputFilePath.toStdString());
    g->printUsers();
    std::string dotFile = g->generateDotFile();
    generateJpgFromDot(dotFile, "graph.jpg");
    // Create a new window
    // Create a new window
    QDialog *imageWindow = new QDialog(this);
    imageWindow->setWindowTitle("Generated Graph");
    imageWindow->resize(800, 600); // Adjust the initial size as needed

    // Create a layout
    QVBoxLayout *layout = new QVBoxLayout(imageWindow);

    // Create a QGraphicsView to display the image
    QGraphicsView *view = new  QGraphicsView(imageWindow);
    QGraphicsScene *scene = new QGraphicsScene(view);

    // Load the image
    QPixmap pixmap("graph.jpg");
    if (pixmap.isNull())
    {
        QMessageBox::critical(this, "Error", "Failed to load the generated image.");
        return;
    }

    // Add the image to the scene
    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);
    view->setScene(scene);

    // Manually scale the view to make the graph more visible
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->scale(2.0, 2.0); // Adjust scale factor (e.g., 2.0 to double the size)

    // Add the view to the layout
    layout->addWidget(view);

    // Show the window
    imageWindow->setLayout(layout);
    imageWindow->exec();

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
void MainWindow::openSearchWindow()
{
    searchWindow = new SearchWindow(this); // Create the search window
    searchWindow->show();
}
void MainWindow::checkXML(){
    QLabel *statusLabel;
    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }


    try {
        std:: string xmlstring = ::fileToString(inputFilePath.toStdString());
        XmlChecker checker;
        int errorCount = checker.checkConsistency(xmlstring);

        if (errorCount == 0)
        {
            QMessageBox::information(this, "Check Consistency", "The XML is valid.");
            statusLabel->setText("Status: Valid XML");

        }
        else
        {
            auto *msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setWindowTitle("XML Validation");
            msgBox->setText(QString("XML file is invalid with %1 violations. Do you wish to fix?").arg(errorCount));
            QPushButton *fixButton = msgBox->addButton("Fix", QMessageBox::AcceptRole);
            QPushButton *noButton = msgBox->addButton("No", QMessageBox::RejectRole);

            msgBox->exec();

            if (msgBox->clickedButton() == fixButton)
            {
                QString outputFileName = QFileDialog::getSaveFileName(this, "Save Formatted XML", "", "XML Files (*.xml)");
                if (outputFileName.isEmpty()) {
                    return;
                }
                else{QMessageBox::information(this, "Success", "File formatted and saved to: " + outputFileName);}
                checker.fixXml(inputFilePath.toStdString(),outputFileName.toStdString()); // Call the fixXml function
                QFile file(outputFileName);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    fileContent->setPlainText(in.readAll());
                    file.close();        } else {
                    QMessageBox::warning(this, "Error", "Unable to open the selected file!");
                }
            }
            else if (msgBox->clickedButton() == noButton)
            {
                statusLabel->setText("Status: Invalid XML (not fixed)");
            }

        }


    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert the XML file: %1").arg(e.what()));
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
