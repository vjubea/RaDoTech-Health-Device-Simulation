#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap> //!!!for testing user loggin in -> menu page
#include <QMessageBox>
#include <QModelIndex>
#include <QString>
// #include <QSqlQuery>
// #include <QSqlDatabase>
// #include <QSqlError>
// #include <QSqlRecord>
// #include <QSqlField>
// #include <QSqlQueryModel>
// #include <QSqlTableModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
  Q_OBJECT


public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private slots:
  void onLoginButtonClicked(); // Slot for login button
  void onLogoutButtonClicked(); // Slot for logout button
  void onCreateProfButtonClicked(); // Slot for create profile button
  void onSaveProfButtonClicked(); // Slot for save profile button
  void onHistoryRowClicked(int row, int column); // Slot for history row click
  void addMeasurementToHistory(const QString& result);
  void measureWrist(); // Slot for wrist measurement
  void measureAnkle(); // Slot for ankle measurement
  void finishMeasurement(); // Slot for finishing measurement



private:
  Ui::MainWindow *ui;


  QMap<QString, QString> userCredentials; //!!! Temporary credentials store


  // GUI elements for Login Page
  QLabel *welcomeLabel;
  QLabel *pleaseLogInStatement;
  QLabel *noAccLabel;
  QLabel *userLabel;
  QLabel *pwLabel;
  QLineEdit *userInput;
  QLineEdit *pwInput;
  QPushButton *loginButton;
  QPushButton *createProfButton;


  // GUI elements for Create Profile Page
  QLabel *createProfLabel;
  QLabel *fNameLabel;
  QLabel *lNameLabel;
  QLabel *weightLabel;
  QLabel *heightLabel;
  QLabel *dobLabel;
  QLabel *countryLabel;
  QLabel *pwLabel2;
  QLineEdit *fNameInput;
  QLineEdit *lNameInput;
  QLineEdit *weightInput;
  QLineEdit *heightInput;
  QLineEdit *dobInput;
  QLineEdit *countryInput;
  QLineEdit *pwInput2;
  QPushButton *saveProfButton;


  // GUI elements for Menu Page
  QLabel *userGreetingLabel;
  QLabel *userNameLabel;
  QPushButton *measureNowButton;
  QPushButton *profilesButton;
  QPushButton *historyButton;
  QPushButton *logoutButton;


  // GUI elements for History Page
  QLabel *historyLabel;
  QTableWidget *historyTable;    // Table for displaying history
  QPushButton *historyBackButton;       // Back button on History page


  //GUI elements for Profiles Page
  QLabel *profilesLabel;
  QTableWidget *profilesTable;    // Table for displaying profiles
  QPushButton *profilesBackButton;       // Back button on Profiles page
  QPushButton *editProfileButton; // Edit profile button
  QPushButton *deleteProfileButton; // Delete profile button


  // GUI elements for Body Screen
  QLabel *bodyScreenLabel;
  QPushButton *bodyBackButton;       // Back button on Body Screen page
  QPushButton *indicatorsButton; // Indicators button
  QPushButton *recommendationsButton; // Recommendations button
  QPushButton *chartButton; // Chart button


  // GUI elements for Chart Page
  QLabel *chartLabel;
  QPushButton *chartBackButton;// Back button on Chart page


  // GUI elements for Indicators Page
  QLabel *indicatorsLabel;
  QPushButton *indicatorsBackButton;// Back button on Indicators page


  // GUI elements for Recommendations Page
  QLabel *recommendationsLabel;
  QPushButton *recommendationsBackButton;// Back button on Recommendations page



  // Stacked Widget
  QStackedWidget *stackedWidget;


  // Functions
  void setupLoginPage(); // Function to initialize Login Page
  void setupCreateProfilePage(); // Function to initialize Create Profile Page
  void setupMenuPage();  // Function to initialize Menu Page
  void updateGreeting(const QString &firstName, const QString &lastName);  // Update greeting labels
  void setupMeasurePage(); // Set up the Measure page
  void setupHistoryPage();// Set up the History page
  void setupBodyScreen(); // Show the body screen
  void showChartPage(); // Show the chart page
  void showIndicatorsPage(); // Show the indicators page
  void showRecommendationsPage(); // Show the recommendations page
  void showHistoryPage(); // Show the history page
  // void showBarGraph(const QString &reading); // Show the bar graph
  void setupProfilesPage(); // Set up the Profiles page
  void editProfile(int row); // Edit profile
  void deleteProfile(int row); // Delete profile
  void setupChartPage(); // Set up the Chart page
  void setupIndicatorsPage(); // Set up the Indicators page
  void setupRecommendationsPage(); // Set up the Recommendations page


};


#endif // MAINWINDOW_H
