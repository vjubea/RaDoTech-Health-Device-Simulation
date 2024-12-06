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
#include <QMessageBox>
#include <QModelIndex>
#include <QString>
#include <QVector>
#include <QComboBox>
#include <QInputDialog>
#include <QTimer>
#include <QRandomGenerator>
// #include <QSqlQuery>
// #include <QSqlDatabase>
// #include <QSqlError>
// #include <QSqlRecord>
// #include <QSqlField>
// #include <QSqlQueryModel>
// #include <QSqlTableModel>

#include "model.h"
#include "scanner.h"
#include "profile.h"
#include "snapshot.h"
#include "defs.h"

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
  void onDeleteProfileClicked(); // Slot for delete profile button
  void onEditProfileClicked(); // Slot for edit profile button
  void onHistoryRowClicked(int row, int column); // Slot for history row click
  void addMeasurementToHistory(const QString& result);
  void startMeasurement(); // Start the measurement
  void finishMeasurement(); // Slot for finishing measurement


private:
  Ui::MainWindow *ui;


  Model model;
  Scanner* scanner;
  Snapshot* curSnap;
  DBManager dbm;


  // GUI elements for Login Page
  QLabel *welcomeLabel;
  QLabel *pleaseLogInStatement;
  QLabel *noAccLabel;
  QPushButton *createProfButton;
  QVector<QString> userProfiles; // List of "First Last" user names
  QComboBox *userDropdown;


  // GUI elements for Create Profile Page
  QLabel *createProfLabel;
  QLabel *fNameLabel;
  QLabel *lNameLabel;
  QLabel *weightLabel;
  QLabel *heightLabel;
  QLabel *dobLabel;
  QLabel *countryLabel;
  QLineEdit *fNameInput;
  QLineEdit *lNameInput;
  QLineEdit *weightInput;
  QLineEdit *heightInput;
  QLineEdit *dobInput;
  QLineEdit *countryInput;
  QPushButton *saveProfButton;


  // GUI elements for Menu Page
  QLabel *userGreetingLabel;
  QLabel *userNameLabel;
  QString currentFirstName;
  QString currentLastName;
  double currentWeight;
  double currentHeight;
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
  QWidget *profilesPage = nullptr;
  QTableWidget *profilesTable;    // Table for displaying profiles
  QPushButton *profilesBackButton;       // Back button on Profiles page
  QPushButton *editProfileButton; // Edit profile button
  QPushButton *deleteProfileButton; // Delete profile button

  // GUI elements for Measure Page
  // Battery variables
  QLabel *batteryLabel;
  int batteryLevel = 100; // Start at 100% battery
  QTimer *batteryDepletionTimer; // Timer to simulate battery depletion
  QVBoxLayout *measureLayout;


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
  void populateUserDropdown(); // Populate the user dropdown
  void setupMenuPage();  // Function to initialize Menu Page
  void updateGreeting(const QString &firstName, const QString &lastName);  // Update greeting labels
  void setupMeasurePage(); // Set up the Measure page
  void measureHands(const QString &side);
  void measureFeet(const QString &side);
  void setupBattery();
  void depleteBattery(); // Deplete the battery
  void setupHistoryPage();// Set up the History page
  void setupBodyScreen(); // Show the body screen
  void showChartPage(); // Show the chart page
  void showIndicatorsPage(); // Show the indicators page
  void showRecommendationsPage(); // Show the recommendations page
  void showHistoryPage(); // Show the history page
  void setupProfilesPage(); // Set up the Profiles page
  void populateProfilesTable(); // Populate the profiles table
  void setupChartPage(); // Set up the Chart page
  void setupIndicatorsPage(); // Set up the Indicators page
  void setupRecommendationsPage(); // Set up the Recommendations page


};


#endif // MAINWINDOW_H
