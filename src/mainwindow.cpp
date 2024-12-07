#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    batteryLevel = 100;
    profilesPage = nullptr;

    // Set up the stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Set up the Login Page
    setupMenuPage();

    setupProfilesPage();

    // Set up the Create Profile Page
    setupCreateProfilePage();

    setupLoginPage();

    setupMeasurePage();

    setupBattery();

    setupScanningPage();

    setupSnapshotDetailsPage();

    setupHistoryPage();

    setupBodyScreen();

    setupChartPage();
    setupIndicatorsPage();
    setupRecommendationsPage();


    // Connect buttons to respective slots   
    connect(measureNowButton, &QPushButton::clicked, [this] () { startMeasurement();});
    connect(profilesButton, &QPushButton::clicked, [this]() {
        setupProfilesPage();    // Ensure the Profiles Page is initialized
        showProfilesPage(); // Go to the Profiles Page
    });
    connect(historyButton, &QPushButton::clicked, [this]() {
        showHistoryPage();
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}


//GUI SETUP/REFRESH FUNCTIONS

void MainWindow::setupMenuPage()
{

    // Create a layout for the Menu Page
    QWidget *menuPage = new QWidget();
    QVBoxLayout *verticalLayout_3 = new QVBoxLayout(menuPage);


    // Create and configure user greeting labels
    //userGreetingLabel = new QLabel("Hello, ");
    userNameLabel = new QLabel("");


    QHBoxLayout *horizantalLayout_2 = new QHBoxLayout();
    //horizantalLayout_2->addWidget(userGreetingLabel);
    horizantalLayout_2->addWidget(userNameLabel);


    // Add greeting to the layout
    verticalLayout_3->addLayout(horizantalLayout_2);


    // Add the buttons (Measure Now, Profiles, History, Log Out)
    measureNowButton = new QPushButton("Measure Now");
    profilesButton = new QPushButton("Profiles");
    historyButton = new QPushButton("History");


    verticalLayout_3->addWidget(measureNowButton);
    verticalLayout_3->addWidget(profilesButton);
    verticalLayout_3->addWidget(historyButton);


    // Set the layout for the menu page
    menuPage->setLayout(verticalLayout_3);
    stackedWidget->addWidget(menuPage);
}

void MainWindow::setupProfilesPage()
{
    if (profilesPage == nullptr) {
        profilesPage = new QWidget(this);
        QVBoxLayout *profilesLayout = new QVBoxLayout;

        QLabel *profilesLabel = new QLabel("Manage Profiles", this);
        QFont labelFont("Arial", 16, QFont::Bold);
        profilesLabel->setFont(labelFont);
        profilesLayout->addWidget(profilesLabel);

        // Table to display profiles
        profilesTable = new QTableWidget(this);
        profilesTable->setColumnCount(2); // Columns: First Name, Last Name
        profilesTable->setHorizontalHeaderLabels({"First Name", "Last Name"});

        //profilesTable->clearContents(); //clears existing data
        //profilesTable->setRowCount(0); display curr user

        QVector<Profile*> profiles = model.getAllProfiles();
        profilesTable->clearContents();

        // Populate profilesTable with profiles
        profilesTable->setRowCount(profiles.length());
        qInfo() << profiles.length();
        for(int i = 0; i < profiles.length(); i++){
            profilesTable->setItem(i, 0, new QTableWidgetItem(profiles.at(i)->getFname()));
            profilesTable->setItem(i, 1, new QTableWidgetItem(profiles.at(i)->getLname()));
        }

        // Set table properties
        profilesTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable editing
        profilesTable->setSelectionBehavior(QAbstractItemView::SelectRows); // Entire row selection
        profilesTable->setSelectionMode(QAbstractItemView::SingleSelection);

        //create, edit & delete buttons
        QPushButton *createProfButton = new QPushButton("Create New Profile", this);
        QPushButton *editProfileButton = new QPushButton("Edit", this);
        QPushButton *deleteProfileButton = new QPushButton("Delete", this);

        // Back Button
        QPushButton *profilesBackButton = new QPushButton("Back");
        connect(profilesBackButton, &QPushButton::clicked, [this]() {
            stackedWidget->setCurrentIndex(0); // Go back to Menu page
        });

        profilesLayout->addWidget(profilesTable);
        profilesLayout->addWidget(createProfButton);
        profilesLayout->addWidget(editProfileButton);
        profilesLayout->addWidget(deleteProfileButton);
        profilesLayout->addWidget(profilesBackButton);

        profilesPage->setLayout(profilesLayout);
        stackedWidget->addWidget(profilesPage);

        connect(createProfButton, &QPushButton::clicked, this, &MainWindow::onCreateProfButtonClicked);
        // Connect the row click signal to the slot
        connect(profilesTable, &QTableWidget::cellClicked, this, &MainWindow::onProfileRowClicked);
        connect(editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
        connect(deleteProfileButton, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
    }
    else {
        //profilesTable->clearContents(); //clears existing data
        //profilesTable->setRowCount(0); display curr user
        profilesTable->clearContents();
        QVector<Profile*> profiles = model.getAllProfiles();
        // Populate profilesTable with profiles
        profilesTable->setRowCount(profiles.length());
        qInfo() << profiles.length();
        for(int i = 0; i < profiles.length(); i++){
            profilesTable->setItem(i, 0, new QTableWidgetItem(profiles.at(i)->getFname()));
            profilesTable->setItem(i, 1, new QTableWidgetItem(profiles.at(i)->getLname()));
        }
        stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::setupLoginPage()
{
    // Initialize the login page widgets
    welcomeLabel = new QLabel("Select User", this);
    pleaseLogInStatement = new QLabel("Please Select User", this);

    userDropdown = new QComboBox(this);
    userDropdown->setEditable(false); // Prevent manual entry
    userDropdown->addItem("Select User"); // Default placeholder
    //Connect the dropdown's signal to the login slot
    //connect(userDropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) { if (index > 0) { onLoginButtonClicked(); } });
    connect(userDropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > 0) {
            onLoginButtonClicked();
        }
    });


    // Create a QFont for customizing the font
    QFont labelFont("Arial", 16, QFont::Bold);

    // Apply the font to the labels
    welcomeLabel->setFont(labelFont);
    pleaseLogInStatement->setFont(labelFont);



    // Create layout for login page
    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addWidget(welcomeLabel);
    loginLayout->addWidget(pleaseLogInStatement);
    loginLayout->addWidget(userDropdown);

    QWidget *loginPage = new QWidget(this);
    loginPage->setLayout(loginLayout);

    // Add the login page to stacked widget
    stackedWidget->addWidget(loginPage);

    // Populate the dropdown with existing profiles
    populateUserDropdown();

    qDebug() << "Log in page index: " << stackedWidget->indexOf(loginPage);
}

void MainWindow::setupCreateProfilePage()
{
    // Initialize the create profile page widgets
    createProfLabel = new QLabel("Create Your Profile", this);
    QFont labelFont9("Arial", 16, QFont::Bold);
    createProfLabel->setFont(labelFont9);

    fNameLabel = new QLabel("First Name:", this);
    lNameLabel = new QLabel("Last Name:", this);
    weightLabel = new QLabel("Weight (kg):", this);
    heightLabel = new QLabel("Height (cm):", this);
    dobLabel = new QLabel("Date of Birth:", this);
    fNameInput = new QLineEdit(this);
    lNameInput = new QLineEdit(this);
    weightInput = new QLineEdit(this);
    heightInput = new QLineEdit(this);
    dobInput = new QLineEdit(this);
    saveProfButton = new QPushButton("Save Profile", this);
    connect(saveProfButton, &QPushButton::clicked, this, &MainWindow::onSaveProfButtonClicked);
    // Create a "Back" button
    QPushButton *saveProfBackButton = new QPushButton("Back", this);
    connect(saveProfBackButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(1); // Go back to Profile page
    });

    QFont labelFont2("Arial", 16, QFont::Bold);

    // Apply the font to the labels
    fNameLabel->setFont(labelFont2);
    lNameLabel->setFont(labelFont2);
    weightLabel->setFont(labelFont2);
    heightLabel->setFont(labelFont2);
    dobLabel->setFont(labelFont2);


    // Create layout for create profile page
    QVBoxLayout *createProfLayout = new QVBoxLayout;
    createProfLayout->addWidget(createProfLabel);
    createProfLayout->addWidget(fNameLabel);
    createProfLayout->addWidget(fNameInput);
    createProfLayout->addWidget(lNameLabel);
    createProfLayout->addWidget(lNameInput);
    createProfLayout->addWidget(weightLabel);
    createProfLayout->addWidget(weightInput);
    createProfLayout->addWidget(heightLabel);
    createProfLayout->addWidget(heightInput);
    createProfLayout->addWidget(dobLabel);
    createProfLayout->addWidget(dobInput);
    createProfLayout->addWidget(saveProfButton);
    createProfLayout->addWidget(saveProfBackButton);

    QWidget *createProfPage = new QWidget(this);
    createProfPage->setLayout(createProfLayout);

    // Add the create profile page to stacked widget
    stackedWidget->addWidget(createProfPage);
}

void MainWindow::setupMeasurePage()
{
    if (!model.getCurProfile()) {
        QMessageBox::warning(this, "Error", "No profile selected. Please log in first.");
        stackedWidget->setCurrentIndex(3); // Go back to login page
        return;
    }

    scanner = model.startScan();
    if (!scanner) {
        qWarning() << "Failed to initialize scanner. No profile selected?";
        // Disable measurement buttons or show an error message
    }

   // Create the measure page
   QWidget *measurePage = new QWidget(this);

   // Create layout
   QVBoxLayout *measureLayout = new QVBoxLayout;

   // Add the battery label
   batteryLabel = new QLabel("Battery: 100%", this);
   batteryLabel->setAlignment(Qt::AlignCenter);
   measureLayout->addWidget(batteryLabel);


   // Instruction Label
   QLabel *instructionLabel = new QLabel("Follow the instructions to measure your body parts:", this);
   QFont labelFont5("Arial", 16, QFont::Bold);
   instructionLabel->setFont(labelFont5);
   measureLayout->addWidget(instructionLabel);

   // Start button for measurement
   QPushButton *startMeasureButton = new QPushButton("Start Measurement", this);
   connect(startMeasureButton, &QPushButton::clicked, this, &MainWindow::startMeasurement);
   measureLayout->addWidget(startMeasureButton);


   // Finish Button to go back to History or Menu page
   QPushButton* finishButton = new QPushButton("Finish Measurement", this);
   connect(finishButton, &QPushButton::clicked, this, &MainWindow::finishMeasurement);
   measureLayout->addWidget(finishButton);

   // Create a "Back" button
   QPushButton *measureBackButton = new QPushButton("Back", this);
   connect(measureBackButton, &QPushButton::clicked, [this]() {
       showMenuPage();
   });

   // Add Back button to layout
   measureLayout->addWidget(measureBackButton);

   // Set the layout for the measure page
   measurePage->setLayout(measureLayout);


   // Add the history page to stacked widget
   stackedWidget->addWidget(measurePage);

   //qDebug() << "Measure Now page index: " << stackedWidget->indexOf(measurePage);

}

void MainWindow::setupBattery()
{
   batteryLevel = 100;
   // Timer setup
   if (!batteryDepletionTimer) {
       batteryDepletionTimer = new QTimer(this);
       connect(batteryDepletionTimer, &QTimer::timeout, this, &MainWindow::depleteBattery);
       qDebug() << "Battery timer initialized.";
   }
}

void MainWindow::setupScanningPage() {
    QWidget *scanningPage = new QWidget(this);
    QVBoxLayout *scanningLayout = new QVBoxLayout;

    QLabel *scanHeaderLabel = new QLabel("Scanning Page", this);
    QFont labelFont8("Arial", 16, QFont::Bold);
    scanHeaderLabel->setFont(labelFont8);
    scanningLayout->addWidget(scanHeaderLabel);

    scanningInstructionLabel = new QLabel("Place the device on your skin and press the button below:", this);
    QFont labelFont7("Arial", 14, QFont::Bold);
    scanningInstructionLabel->setFont(labelFont7);
    scanningLayout->addWidget(scanningInstructionLabel);

    contactWithSkinButton = new QPushButton("Contact with Skin", this);
    //connect(contactWithSkinButton, &QPushButton::clicked, this, &MainWindow::startScanningProcess);
    scanningLayout->addWidget(contactWithSkinButton);

    scanningPage->setLayout(scanningLayout);
    stackedWidget->addWidget(scanningPage);
}

void MainWindow::setupSnapshotDetailsPage() {

    QWidget *snapshotDetailsPage = new QWidget(this);
    QVBoxLayout *snapshotLayout = new QVBoxLayout;

    QLabel *snapshotTitle = new QLabel("Snapshot Details", this);
    QFont titleFont("Arial", 18, QFont::Bold);
    snapshotTitle->setFont(titleFont);
    snapshotLayout->addWidget(snapshotTitle);

    QFont labelFont("Arial", 14, QFont::Bold);

    // Blood Pressure
    for (const auto& hand : {"Left Hand", "Right Hand"}) {
        QLabel *pressureLabel = new QLabel(QString("%1 Pressure:").arg(hand), this);
        pressureLabel->setFont(labelFont);
        snapshotLayout->addWidget(pressureLabel);

        QHBoxLayout *pressureLayout = new QHBoxLayout;
        QLineEdit *systolicInput = new QLineEdit(this);
        systolicInput->setPlaceholderText("Systolic");
        QLineEdit *diastolicInput = new QLineEdit(this);
        diastolicInput->setPlaceholderText("Diastolic");
        pressureLayout->addWidget(systolicInput);
        pressureLayout->addWidget(diastolicInput);
        snapshotLayout->addLayout(pressureLayout);
    }

    // Other measurements
    QLabel *weightLabel = new QLabel("Weight (kg):", this);
    weightLabel->setFont(labelFont);
    weightInput = new QLineEdit(this);
    snapshotLayout->addWidget(weightLabel);
    snapshotLayout->addWidget(weightInput);

    QLabel *heartRateLabel = new QLabel("Heart Rate:", this);
    heartRateLabel->setFont(labelFont);
    heartRateInput = new QLineEdit(this);
    snapshotLayout->addWidget(heartRateLabel);
    snapshotLayout->addWidget(heartRateInput);

    QLabel *bodyTempLabel = new QLabel("Body Temperature (°C):", this);
    bodyTempLabel->setFont(labelFont);
    bodyTempInput = new QLineEdit(this);
    snapshotLayout->addWidget(bodyTempLabel);
    snapshotLayout->addWidget(bodyTempInput);

    QLabel *sleepTimeLabel = new QLabel("Sleep Time (hh:mm):", this);
    sleepTimeLabel->setFont(labelFont);
    snapshotLayout->addWidget(sleepTimeLabel);
    QHBoxLayout *sleepTimeLayout = new QHBoxLayout;
    sleepHoursInput = new QLineEdit(this);
    sleepHoursInput->setPlaceholderText("Hours");
    sleepMinutesInput = new QLineEdit(this);
    sleepMinutesInput->setPlaceholderText("Minutes");
    sleepTimeLayout->addWidget(sleepHoursInput);
    sleepTimeLayout->addWidget(sleepMinutesInput);
    snapshotLayout->addLayout(sleepTimeLayout);

    QLabel *notesLabel = new QLabel("Notes:", this);
    notesLabel->setFont(labelFont);
    notesInput = new QTextEdit(this);
    snapshotLayout->addWidget(notesLabel);
    snapshotLayout->addWidget(notesInput);

    saveSnapshotButton = new QPushButton("Save Snapshot", this);
    backToMeasureButton = new QPushButton("Back", this);
    snapshotLayout->addWidget(saveSnapshotButton);
    snapshotLayout->addWidget(backToMeasureButton);

    connect(saveSnapshotButton, &QPushButton::clicked, this, &MainWindow::onSaveSnapshotClicked);
    connect(backToMeasureButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(4); // Assuming measure page index is 4
    });

    snapshotDetailsPage->setLayout(snapshotLayout);
    stackedWidget->addWidget(snapshotDetailsPage);
}

void MainWindow::setupHistoryPage()
{
    // Create the history page
    QWidget *historyPage = new QWidget(this);

    // Create layout
    QVBoxLayout *historyLayout = new QVBoxLayout;

    historyLabel = new QLabel("History", this);
    QFont labelFont3("Arial", 16, QFont::Bold);
    historyLabel->setFont(labelFont3);
    historyLayout->addWidget(historyLabel);


    // Create a table widget for displaying history data
    QTableWidget *historyTable = new QTableWidget(this);
    historyTable->setHorizontalHeaderLabels({"Date & Time", "Profile", "Notes"});
    historyTable->setColumnCount(3); // Date & Time, Profile, Notes

    QVector<Snapshot*> snaps = model.getCurSnapshots();
    historyTable->setRowCount(snaps.length());

    for(int i = 0; i< snaps.length(); i++){
        historyTable->setItem(i, 0, new QTableWidgetItem(snaps[i]->getTimestamp()));
        historyTable->setItem(i, 1, new QTableWidgetItem(model.getCurProfile()->getFullName()));
        historyTable->setItem(i, 2, new QTableWidgetItem(snaps[i]->getNotes()));
    }

    // Set table properties
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable editing
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows); // Entire row selection
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);


    // Connect the row click signal to the slot
    connect(historyTable, &QTableWidget::cellClicked, this, &MainWindow::onHistoryRowClicked);


    // Add the table to the layout
    historyLayout->addWidget(historyTable);


    // Create a "Back" button
    QPushButton *historyBackButton = new QPushButton("Back", this);
    connect(historyBackButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(0); // Go back to Home page
    });


    // Add Back button to layout
    historyLayout->addWidget(historyBackButton);


    // Set the layout for the history page
    historyPage->setLayout(historyLayout);


    // Add the history page to stacked widget
    stackedWidget->addWidget(historyPage);

    qDebug() << "History page index: " << stackedWidget->indexOf(historyPage);

}

void MainWindow::setupBodyScreen()
{
   QWidget *bodyWidget = new QWidget(this);
   QVBoxLayout *bodyLayout = new QVBoxLayout;


   QLabel *bodyScreenLabel = new QLabel("Body Screen", this);
   QFont labelFont4("Arial", 16, QFont::Bold);
   bodyScreenLabel->setFont(labelFont4);
   bodyLayout->addWidget(bodyScreenLabel);


   // Create table for organ readings
   QTableWidget *organTable = new QTableWidget(this);
   organTable->setRowCount(12); // 12 organs to display (6 from H1-H6 and 6 from F1-F6)
   organTable->setColumnCount(2); // Column 1: Organ, Column 2: Status


   // Set table headers
   QStringList headers = {"Organ", "Status"};
   organTable->setHorizontalHeaderLabels(headers);


   // Define font for organ rows
   QFont labelFont5("Arial", 14, QFont::Bold);


   // Sample snapshot data - assuming snapshot data is available from a previous scan
   //Snapshot currentSnapshot = getCurrentSnapshot(); // This should be obtained dynamically from the current snapshot


   // Populate the table with organ data and color
   struct Organ {
       QString name;
       int reading;
       int goodStart;
       int goodEnd;
   };


   // Define organ readings and ranges (from defs.h)
   /*
   QList<Organ> organs = {
       {"Lung (H1)", currentSnapshot.getLeftHandPressReadId(), H1_GoodReadSTART, H1_GoodReadEND},
       {"Pericardium (H2)", currentSnapshot.getRightHandPressReadId(), H2_GoodReadSTART, H2_GoodReadEND},
       {"Heart (H3)", currentSnapshot.getHeartRate(), H3_GoodReadSTART, H3_GoodReadEND},
       {"Small Intestine (H4)", currentSnapshot.getLeftHandPressReadId(), H4_GoodReadSTART, H4_GoodReadEND},
       {"Lymph Vessels (H5)", currentSnapshot.getRightHandPressReadId(), H5_GoodReadSTART, H5_GoodReadEND},
       {"Large Intestine (H6)", currentSnapshot.getHeartRate(), H6_GoodReadSTART, H6_GoodReadEND},
       {"Lung (F1)", currentSnapshot.getLeftHandPressReadId(), F1_GoodReadSTART, F1_GoodReadEND},
       {"Pericardium (F2)", currentSnapshot.getRightHandPressReadId(), F2_GoodReadSTART, F2_GoodReadEND},
       {"Heart (F3)", currentSnapshot.getHeartRate(), F3_GoodReadSTART, F3_GoodReadEND},
       {"Small Intestine (F4)", currentSnapshot.getLeftHandPressReadId(), F4_GoodReadSTART, F4_GoodReadEND},
       {"Lymph Vessels (F5)", currentSnapshot.getRightHandPressReadId(), F5_GoodReadSTART, F5_GoodReadEND},
       {"Large Intestine (F6)", currentSnapshot.getHeartRate(), F6_GoodReadSTART, F6_GoodReadEND}
   };
  `

   // Loop through the organs and set their color based on the reading
   for (int i = 0; i < organs.size(); ++i) {
       QTableWidgetItem *organItem = new QTableWidgetItem(organs[i].name);
       organItem->setFont(labelFont5);
       organTable->setItem(i, 0, organItem);


       // Determine the status color based on the reading
       QString status;
       QColor color;


       if (organs[i].reading < organs[i].goodStart) {
           status = "Below Normal";
           color = Qt::red;
       } else if (organs[i].reading > organs[i].goodEnd) {
           status = "Above Normal";
           color = Qt::purple;
       } else {
           status = "Normal";
           color = Qt::green;
       }


       QTableWidgetItem *statusItem = new QTableWidgetItem(status);
       statusItem->setBackgroundColor(color);
       statusItem->setFont(labelFont5);
       organTable->setItem(i, 1, statusItem);
   }
   */

   // Add the table to the layout
   bodyLayout->addWidget(organTable);


   // Add buttons for navigation
   QPushButton *chartButton = new QPushButton("Chart", this);
   QPushButton *indicatorsButton = new QPushButton("Indicators", this);
   QPushButton *recommendationsButton = new QPushButton("Recommendations", this);
   QPushButton *backBodyButton = new QPushButton("Back", this);


   // Add buttons to the layout
   bodyLayout->addWidget(chartButton);
   bodyLayout->addWidget(indicatorsButton);
   bodyLayout->addWidget(recommendationsButton);
   bodyLayout->addWidget(backBodyButton);


   // Connect buttons to respective slots (or pages in stackedWidget)
   connect(chartButton, &QPushButton::clicked, this, &MainWindow::showChartPage);
   connect(indicatorsButton, &QPushButton::clicked, this, &MainWindow::showIndicatorsPage);
   connect(recommendationsButton, &QPushButton::clicked, this, &MainWindow::showRecommendationsPage);
   connect(backBodyButton, &QPushButton::clicked, this, [this]() {
       stackedWidget->setCurrentIndex(5); // Navigate back to the History Page
   });


   bodyWidget->setLayout(bodyLayout);
   stackedWidget->addWidget(bodyWidget);
}

void MainWindow::setupChartPage()
{
   QWidget *chartWidget = new QWidget(this);
   QVBoxLayout *chartLayout = new QVBoxLayout(chartWidget);


   // Add label for the chart
   QLabel *chartLabel = new QLabel("Chart Page");
   chartLabel->setAlignment(Qt::AlignCenter);
   chartLayout->addWidget(chartLabel);


   // Create a horizontal layout for the bar graph
   QHBoxLayout *barLayout = new QHBoxLayout();


   // Example data
   QVector<int> data = {15, 30, 45, 60, 75};


   // Create bars as QLabels with QPixmap
   for (int value : data) {
       QLabel *barLabel = new QLabel();
       QPixmap pixmap(50, value);  // Set width to 50, height based on value
       pixmap.fill(Qt::blue);  // Fill the pixmap with a color (e.g., blue)
       barLabel->setPixmap(pixmap);
       barLabel->setAlignment(Qt::AlignBottom);  // Align to bottom of the bar
       barLayout->addWidget(barLabel);
   }


   chartLayout->addLayout(barLayout);


   // Add the Back button
   QPushButton *chartBackButton = new QPushButton("Back");
   connect(chartBackButton, &QPushButton::clicked, this, [this]() {
       stackedWidget->setCurrentIndex(6); // Navigate back to the Body Screen page
   });
   chartLayout->addWidget(chartBackButton);


   // Set the layout and add to the stacked widget
   chartWidget->setLayout(chartLayout);
   stackedWidget->addWidget(chartWidget);
}

void MainWindow::setupRecommendationsPage()
{
    QWidget *recommendationsWidget = new QWidget(this);
    QVBoxLayout *recommendationsLayout = new QVBoxLayout(recommendationsWidget);


    QFont labelFont5("Arial", 16, QFont::Bold);


    QLabel *recommendationsLabel = new QLabel("Recommendations Page");
    recommendationsLabel->setFont(labelFont5);
    recommendationsLabel->setAlignment(Qt::AlignCenter);
    recommendationsLayout->addWidget(recommendationsLabel);


    QPushButton *recBackButton = new QPushButton("Back");
    connect(recBackButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(6);
    });
    recommendationsLayout->addWidget(recBackButton);


    recommendationsWidget->setLayout(recommendationsLayout);
    stackedWidget->addWidget(recommendationsWidget);
}

void MainWindow::setupIndicatorsPage()
{
    QWidget *indicatorsWidget = new QWidget(this);
    QVBoxLayout *indicatorsLayout = new QVBoxLayout(indicatorsWidget);


    QFont labelFont5("Arial", 16, QFont::Bold);


    QLabel *indicatorsLabel = new QLabel("Indicators Page");
    indicatorsLabel->setFont(labelFont5);
    indicatorsLabel->setAlignment(Qt::AlignCenter);
    indicatorsLayout->addWidget(indicatorsLabel);


    QPushButton *indBackButton = new QPushButton("Back");
    connect(indBackButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(6);
    });
    indicatorsLayout->addWidget(indBackButton);


    indicatorsWidget->setLayout(indicatorsLayout);
    stackedWidget->addWidget(indicatorsWidget);
}


//GUI SHOW FUNCTIONS
void MainWindow::showLoginPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCreateProfilePage()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showMenuPage()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showMeasurePage()
{
    stackedWidget->setCurrentIndex(5);
}

void MainWindow::showBattery()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showProfilesPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showChartPage()
{
    stackedWidget->setCurrentIndex(7);
}

void MainWindow::showIndicatorsPage()
{
    stackedWidget->setCurrentIndex(8);
}

void MainWindow::showRecommendationsPage()
{
    stackedWidget->setCurrentIndex(9);
}

void MainWindow::showSnapshotDetailsPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showHistoryPage()
{
    stackedWidget->setCurrentIndex(7);
}

void MainWindow::showBodyScreen()
{
    stackedWidget->setCurrentIndex(6);
}





//MISC/HELPER FUNCTIONS

void MainWindow::populateUserDropdown() {
    userDropdown->clear();
    userDropdown->addItem("Select User");

    QVector<QString> profileNames = model.getProfileNames();
    for (const QString &name : profileNames) {
        userDropdown->addItem(name);
    }
}

void MainWindow::measureHands(const QString &side) {
   // Simulate data for measurement
   qDebug() << "Measuring" << side << "Hand";


   for(int i = 0; i < 6; i++){
       int reading = QRandomGenerator::global()->bounded(HandGoodReadStart[i] - 7, HandGoodReadEnd[i] +7); // Generate a random reading in range
       if (scanner) {
          char sideChar = (side == "Left") ? 'L' : 'R';
          scanner->registerReading(sideChar, 'H', reading); // H for Hand
       } else {
           qWarning() << "Scanner not initialized.";
       }
   }

   QMessageBox::information(this, "Measurement Complete", side + " Hand measurement recorded.");
}


void MainWindow::measureFeet(const QString &side) {
   // Simulate data for measurement
    qDebug() << "Measuring" << side << "Foot";

   for(int i = 0; i < 6; i++){
       int reading = QRandomGenerator::global()->bounded(FootGoodReadStart[i] - 7, FootGoodReadEnd[i] +7); // Generate a random reading in range
       if (scanner) {
          char sideChar = (side == "Left") ? 'L' : 'R';
          scanner->registerReading(sideChar, 'F', reading); // F for Foot
       } else {
           qWarning() << "Scanner not initialized.";
       }
   }

   //Update UI or indicate that the foot measurement was successful
   QMessageBox::information(this, "Measurement Complete", side + " Foot measurement recorded.");
}

void MainWindow::startMeasurement() {
    if (!scanner) {
        QMessageBox::warning(this, "Error", "Scanner not initialized. Please try again.");
        return;
    }

    if (!batteryDepletionTimer || !batteryLabel) {
        qDebug() << "Battery components are not initialized!";
        return;
    }

    if (!batteryDepletionTimer->isActive()) {
        batteryDepletionTimer->start(2400);
        qDebug() << "Battery depletion timer started.";
    }

    showMeasurePage();
    startScanningProcess();
}

void MainWindow::depleteBattery()
{
   if (batteryLevel > 0) {
       batteryLevel--;
       if (batteryLabel) {
           batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");
       } else {
           qDebug() << "Error: Battery label not initialized!";
       }
   } else {
       batteryDepletionTimer->stop();
       QMessageBox::warning(this, "Battery Low", "Battery is completely drained!");
   }
}

// Change to be used by SnapshotDetails page into "SaveSnapshot"
void MainWindow::finishMeasurement()
{
    if (!scanner) {
        QMessageBox::warning(this, "Error", "No active scan in progress.");
        return;
    }

    if (batteryDepletionTimer && batteryDepletionTimer->isActive()) {
        batteryDepletionTimer->stop();
    }

    //!!USER INPUTS POST MEASUREMENT GO HERE -- To remove after
    if (scanner) {
        scanner->registerWeight(0.0f);
        scanner->registerBlood('L', 0,0);
        scanner->registerBlood('R',0,0);
        scanner->registerBodyTemp(0.0);
        scanner->registerHeartRate(0);
        scanner->registerSleepTime(0,0);
        scanner->registerTime(0,0); //might want to get current from system
        scanner->registerDate(0,0,0); //might want to geet current from system
        scanner->registerNotes("Normal scan"); // let user write these
        // -----

        if(!scanner->finishScan()) {
            QMessageBox::warning(this, "Scan Incomplete", "Scan parameters incomplete");
            return;
        }

       //clean up scan.
       delete scanner;
       scanner = nullptr;
    } else {
        QMessageBox::information(this, "Measurement Complete", "The measurement is complete.");
    }

}

void MainWindow::startScanningProcess() {
    const int totalPoints = 24;
    for (int i = 0; i < totalPoints; i++) {
        qDebug() <<"scan step " << i;

        QString pointType = (i < 12) ? "Hand" : "Foot";
        QString side = (i%12 < 6) ? "Right" : "Left";
        int pointNumber = (i % 6) + 1;

        scanningInstructionLabel->setText("Press Device to " + side + " " + pointType + " point " + QString::number(pointNumber));
        while(!contactWithSkinButton->isDown()){
                QApplication::processEvents();
        }

        if(contactWithSkinButton->isDown()){
            scanningInstructionLabel->setText("Scanning...");
            QApplication::processEvents();
            QThread::msleep(600);
        }

        scanningInstructionLabel->setText("Remove Device From Skin");
        while(contactWithSkinButton->isDown()){
            QApplication::processEvents();
        }

        if (scanner) {
            char sideChar = (side == "Left") ? 'L' : 'R';
            char limbChar = (pointType == "Hand") ? 'H' : 'F';
            int reading = QRandomGenerator::global()->bounded(0, 100); // Replace with actual reading
            scanner->registerReading(sideChar, limbChar, reading);
        }
    }

    // After 24 points, go to snapshot page
    setupBodyScreen();
    showBodyScreen();
}

void MainWindow::onSaveSnapshotClicked() {
    // Check if all fields are filled
    if (weightInput->text().isEmpty() || heartRateInput->text().isEmpty() ||
        bodyTempInput->text().isEmpty() || sleepHoursInput->text().isEmpty() ||
        sleepMinutesInput->text().isEmpty() || notesInput->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Incomplete Data", "Please fill all the input fields.");
    } else {
        QMessageBox::information(this, "Snapshot Saved", "Snapshot details have been successfully saved.");
        stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::onLoginButtonClicked() {
    int selectedIndex = userDropdown->currentIndex() - 1;
    if (selectedIndex >= 0 && model.selectProfile(selectedIndex)) {
        stackedWidget->setCurrentIndex(4); // Go to Measure page
    } else {
        QMessageBox::warning(this, "Login Error", "Please select a valid user");
    }
}

// Slot for handling create profile button click
void MainWindow::onCreateProfButtonClicked()
{
    showCreateProfilePage();
}

// Slot for handling save profile button click
void MainWindow::onSaveProfButtonClicked()
{
    // Fetch input values
    QString fname = fNameInput->text().trimmed();
    QString lname = lNameInput->text().trimmed();
    QString weightString = weightInput->text().trimmed();
    float weight;
    QString heightString = heightInput->text().trimmed();
    float height;
    QString dob = dobInput->text().trimmed();

    // Validate inputs
    if (fname.isEmpty() || lname.isEmpty() || weightString.isEmpty() || heightString.isEmpty() || dob.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "All fields are required!");
        return;
    }

    bool isValid;
    weight = weightString.toFloat(&isValid);

    if (!isValid){
        QMessageBox::warning(this, "Validation Error", "Weight must be a number");
        return;
    }

    height = heightString.toFloat(&isValid);
    if (!isValid){
        QMessageBox::warning(this, "Validation Error", "Height must be a number");
        return;
    }

    if (! model.createProfile(fname, lname, weight, height, dob)) {
        QMessageBox::warning(this, "Validation Error", "Something went wrong creatting this profile in the database");
        return;
    }

    QMessageBox::information(this, "Success", "Profile created successfully!");

    // Clear input fields after saving
    fNameInput->clear();
    lNameInput->clear();
    weightInput->clear();
    heightInput->clear();
    dobInput->clear();

    // Reload the Profile page
    setupProfilesPage();
}

void MainWindow::updateGreeting(const QString &firstName, const QString &lastName)
{
    userGreetingLabel->setText("Hello, ");
    userNameLabel->setText(firstName + " " + lastName); // Display full name
}

void MainWindow::onHistoryRowClicked(int row, int column) {

    // Access the QTableWidget (history table)
    QTableWidget *historyTable = qobject_cast<QTableWidget *>(sender());
    if (!historyTable) return; // Safety check

    //This sets current snapshot to be displayed as the profile's snap at associated index.
    curSnap = model.getCurProfile()->getSnapshots()[row];

    // Example: Navigate to the Body Screen and display detailed info
    setupBodyScreen(); // Ensure this method sets up the Body Screen if not already done
    // TBD
    stackedWidget->setCurrentIndex(6); // Assuming the Body Screen index is 5
}

void MainWindow::onProfileRowClicked(int row, int col) {
    // Access the QTableWidget (history table)
    QTableWidget *profilesTable = qobject_cast<QTableWidget *>(sender());
    if (!profilesTable) return; // Safety check
    // Selects current profile to enable Update/Delete
    model.selectProfile(row);
}

void MainWindow::onEditProfileClicked()
{
    // Ensure the profile has been selected
    if (model.getCurProfile()  == nullptr) {
        QMessageBox::warning(this, "EDIT_ERR", "No profile selected. Please select a profile first.");
        return;
    }

    // Create a dialog or input fields to edit the profile
    bool ok;

    QString newFname = QInputDialog::getText(this, "Edit First Name", "Enter new weight first name:", QLineEdit::Normal, model.getCurProfile()->getFname(), &ok);
    if (!ok) {
        newFname = model.getCurProfile()->getFname();
    }
    QString newLname = QInputDialog::getText(this, "Edit First Name", "Enter new weight first name:", QLineEdit::Normal, model.getCurProfile()->getLname(), &ok);
    if (!ok) {
        newLname = model.getCurProfile()->getLname();
    }

    float newWeight = QInputDialog::getDouble(this, "Edit Weight", "Enter new weight (kg):", model.getCurProfile()->getWeight(), 0.0, 200.0, 2, &ok);
    if (!ok) {
        newWeight = model.getCurProfile()->getWeight();
    }

    float newHeight = QInputDialog::getDouble(this, "Edit Height", "Enter new height (cm):", model.getCurProfile()->getHeight(), 50.0, 300.0, 2, &ok);
    if (!ok) {
        newHeight = model.getCurProfile()->getHeight(); // Update height
    }

    QString empty = "";
    model.editCurProfile(newFname, newLname, newWeight, newHeight, empty);

    // Inform the user of the successful update
    QMessageBox::information(this, "Profile Updated", "The profile has been successfully updated.");

    setupProfilesPage();
}

void MainWindow::onDeleteProfileClicked()
{
    // Ensure the profile has been selected
    if (model.getCurProfile()  == nullptr) {
        QMessageBox::warning(this, "EDIT_ERR", "No profile selected. Please select a profile first.");
        return;
    }

    // Confirm the deletion
    int result = QMessageBox::question(this, "Delete Profile",
        "Are you sure you want to delete this profile?",
        QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::No) {
        return; // Exit if the user cancels
    }

    if (!model.deleteCurrentProfile()) {
        // Show success message
        QMessageBox::information(this, "Error", "Profile could not be deleted.");
    }
    QMessageBox::information(this, "Profile Deleted", "The profile has been deleted.");
    // Clear current user data
    currentFirstName.clear();
    currentLastName.clear();
    currentWeight = 0.0;
    currentHeight = 0.0;

    // Reload the Profiles Page
    setupProfilesPage();
}
