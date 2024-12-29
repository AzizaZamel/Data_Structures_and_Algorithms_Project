#include "searchwindow.h"
#include "ui_searchwindow.h"

// Constructor for MainWindow, sets up the UI and initializes components
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Adding options to the ComboBox for search types: "Word" and "Topic"
    ui->comboBox->addItems({"Word", "Topic"});

    // Connect the search button's click signal to the onSearchButtonClicked slot
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
}

// Destructor for MainWindow, ensures proper cleanup of the UI
MainWindow::~MainWindow()
{
    delete ui;
}

// Setter function to assign the XML file name or path
void MainWindow::set_xmlFile(string File ){
    xmlFile = File; // Assign the provided file name to the class variable
}

// Slot function called when the search button is clicked
void MainWindow::onSearchButtonClicked()
{
    // Retrieve the selected search type from the ComboBox
    QString searchType = ui->comboBox->currentText();
    // Retrieve the user input from the LineEdit
    QString searchInput = ui->lineEdit->text();

    // Check if the input is empty or contains the placeholder text
    if (searchInput.isEmpty() || searchInput == "Enter search term or topic") {
        // Display a warning message box if input is invalid
        QMessageBox::warning(this, "Input Error", "Please enter a search term or topic.");
        return; // Exit the function early
    }

    std::string results;

    // Perform a search based on the selected type
    if (searchType == "Word") {
        // Call the search_by_word function for word-based search
        results = search_by_word(xmlFile, searchInput.toStdString());
    } else if (searchType == "Topic") {
        // Call the search_by_topic function for topic-based search
        string topic;
        topic = searchInput.toStdString();
        results = search_by_topic(xmlFile, topic);
    }

    // Display the search results in the TextEdit widget
    if (!results.empty()) {
        // If results are found, display them
        ui->textEdit->setPlainText(QString::fromStdString(results));
    } else {
        // If no results are found, display a default message
        ui->textEdit->setPlainText("No results found.");
    }
}
