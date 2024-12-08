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
#include <QTextEdit>
#include <QRandomGenerator>
#include <QThread>
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
  void onCreateProfButtonClicked(); // Slot for create profile button
  void onProfileRowClicked(int row, int col); // Slot for selecting profile
  void onSaveProfButtonClicked(); // Slot for save profile button
  void onDeleteProfileClicked(); // Slot for delete profile button
  void onEditProfileClicked(); // Slot for edit profile button
  void onSaveSnapshotClicked();
  void onHistoryRowClicked(int row, int col); // Slot for history row click
  void startMeasurement(); // Start the measurement
  void finishMeasurement(); // Slot for finishing measurement


private:
  Ui::MainWindow *ui;


  Model model;
  Scanner* scanner;
  Snapshot* curSnap;
  QVector<Snapshot*>snaps;
  DBManager *dbm;


  // GUI elements for Login Page
  QLabel *welcomeLabel;
  QLabel *pleaseLogInStatement;
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
  QWidget *menuPage;
  QLabel *userGreetingLabel;
  QLabel *userNameLabel;
  QString currentFirstName;
  QString currentLastName;
  double currentWeight;
  double currentHeight;
  QPushButton *measureNowButton;
  QPushButton *profilesButton;
  QPushButton *historyButton;


  //Page Widgets
  QWidget *loginPage;
  QWidget *profilesPage;
  QWidget *createProfPage;
  QWidget *measurePage;
  QWidget *scanningPage;
  QWidget *snapshotDetailsPage;
  QWidget *historyPage;
  QWidget *bodyWidget;
  QWidget *chartWidget;
  QWidget *recommendationsWidget;
  QWidget *indicatorsWidget;


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

  // GUI elements for Measure Page
  // Battery variables
  QLabel *batteryLabel;
  int batteryLevel; // Battery level
  QTimer *batteryDepletionTimer; // Timer to simulate battery depletion
  QVBoxLayout *measureLayout;

  // GUI elements for Scanning Page
  QLabel scanHeaderLabel;
  QLabel *scanningInstructionLabel;
  QPushButton *contactWithSkinButton; // Contact with skin button

  // GUI elements for Snapshots Page
  QLineEdit *weightInputSnap;
  QLineEdit *heartRateInput;
  QLineEdit *bodyTempInput;
  QLineEdit *sleepHoursInput;
  QLineEdit *sleepMinutesInput;
  QTextEdit *notesInput;
  QPushButton *saveSnapshotButton;
  QPushButton *backToMeasureButton;



  // GUI elements for Body Screen
  QLabel *bodyScreenLabel;
  QPushButton *bodyBackButton;       // Back button on Body Screen page
  QPushButton *indicatorsButton; // Indicators button
  QPushButton *recommendationsButton; // Recommendations button
  QPushButton *chartButton; // Chart button
  QTableWidget *organTable;


  // GUI elements for Chart Page
  QLabel *chartLabel;
  QPushButton *chartBackButton;// Back button on Chart page
  QHBoxLayout *barLayout;


  // GUI elements for Indicators Page
  QLabel *indicatorsLabel;
  QPushButton *indicatorsBackButton;// Back button on Indicators page


  // GUI elements for Recommendations Page
  QLabel *recommendationsLabel;
  QTextEdit *recBox;
  QPushButton *recommendationsBackButton;// Back button on Recommendations page



  // Stacked Widget
  QStackedWidget *stackedWidget;


  // Functions
  //Page Setups
  void setupLoginPage();
  void setupCreateProfilePage();
  void setupMenuPage();
  void setupMeasurePage();
  void setupBattery();
  void setupProfilesPage();
  void setupChartPage();
  void setupIndicatorsPage();
  void setupRecommendationsPage();
  void setupSnapshotDetailsPage();
  void setupHistoryPage();
  void setupBodyScreen();


  // Page Switches
  void showLoginPage();
  void showCreateProfilePage();
  void showMenuPage();
  void showMeasurePage();
  void showProfilesPage();
  void showChartPage();
  void showIndicatorsPage();
  void showRecommendationsPage();
  void showSnapshotDetailsPage();
  void showHistoryPage();
  void showBodyScreen();
  void showScanningPage();


  //Misc
  void populateUserDropdown(); // Populate the user dropdown

  void updateGreeting(const QString &firstName, const QString &lastName);  // Update greeting labels

  void measureHands(const QString &side);
  void measureFeet(const QString &side);

  void depleteBattery(); // Deplete the battery
  void setupScanningPage(); // Set up the Scanning page
  void startScanningProcess();









};


#endif // MAINWINDOW_H
