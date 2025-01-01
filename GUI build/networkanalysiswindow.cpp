#include "networkanalysiswindow.h"
#include "./ui_networkanalysiswindow.h"
#include <QGridLayout>
#include <QSizePolicy>
#include <QString>
#include <QInputDialog>

NetworkAnalysisWindow::NetworkAnalysisWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NetworkAnalysisWindow)
{
    ui->setupUi(this);

    // Connect buttons to their respective slots
    connect(ui->btnMostInfluencer, &QPushButton::clicked, this, &NetworkAnalysisWindow::onMostInfluencerClicked);
    connect(ui->btnMostActive, &QPushButton::clicked, this, &NetworkAnalysisWindow::onMostActiveClicked);
    connect(ui->btnMutualFollowers, &QPushButton::clicked, this, &NetworkAnalysisWindow::onMutualFollowersClicked);
    connect(ui->btnSuggest, &QPushButton::clicked, this, &NetworkAnalysisWindow::onSuggestClicked);

}


NetworkAnalysisWindow::~NetworkAnalysisWindow()
{
    delete ui;
}

void NetworkAnalysisWindow::set_xmlFile(string filename){
    xmlFile = filename;
}

void NetworkAnalysisWindow::displayResult(const QString& result)
{
    // Display the result in the QTextBrowser
    ui->textEdit->setPlainText(result);
}

void NetworkAnalysisWindow::onSuggestClicked()
{
    bool ok;
    int userId = QInputDialog::getInt(this, "Enter User ID", "Please enter the user ID:", 0, 0, 10000, 1, &ok);

    if (ok) {

        string result= printS_string(print_suggested(xmlFile,userId),userId);

        // Display the result in the output browser
        displayResult(QString::fromStdString(result));
    } else {
        // If the user cancels the input, display a message
        displayResult("User ID input was canceled.");
    }
}

void NetworkAnalysisWindow::onMostActiveClicked()
{
    string result = helper_function(xmlFile);
    displayResult(QString::fromStdString(result));

}

void NetworkAnalysisWindow::onMutualFollowersClicked()
{
    // Prompt the user to enter multiple IDs separated by commas
    bool ok;
    QString inputIds = QInputDialog::getText(this, "Enter User IDs",
                                             "Please enter user IDs (separate by commas):",
                                             QLineEdit::Normal, "",
                                             &ok);

   if (ok) {
        string result = get_mutual_followers_string(parse_ids(inputIds.toStdString()),xmlFile);
        // Display the result in the text browser
        displayResult(QString::fromStdString(result));
    } else {
       // If the user cancels the input, display a message
       displayResult("User ID input was canceled.");
   }
}


void NetworkAnalysisWindow::onMostInfluencerClicked()
{
    string result = getMostFollowedUserDetails(xmlFile);
    displayResult(QString::fromStdString(result));

}
