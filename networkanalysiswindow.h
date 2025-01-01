#ifndef NETWORKANALYSISWINDOW_H
#define NETWORKANALYSISWINDOW_H

#include <QMainWindow>
#include <QFileDialog>  // For file dialog
#include "most_active.hpp"
#include "suggest.hpp"
#include "most_influencer.hpp"
#include "utils.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class NetworkAnalysisWindow;
}
QT_END_NAMESPACE

class NetworkAnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    NetworkAnalysisWindow(QWidget *parent = nullptr);
    ~NetworkAnalysisWindow();
    void set_xmlFile(string filename);

private:
    Ui::NetworkAnalysisWindow *ui;
    string xmlFile;

private slots:
    void displayResult(const QString& result);  // Slot to display result in QTextBrowser
    void onMostInfluencerClicked();
    void onMostActiveClicked();
    void onMutualFollowersClicked();
    void onSuggestClicked();

};
#endif // NETWORKANALYSISWINDOW_H
