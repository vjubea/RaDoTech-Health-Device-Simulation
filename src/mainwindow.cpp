#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    batteryLevel = 100;
    menuPage = nullptr;
    loginPage = nullptr;
    profilesPage = nullptr;
    createProfPage = nullptr;
    measurePage = nullptr;
    scanningPage = nullptr;
    snapshotDetailsPage = nullptr;
    historyPage = nullptr;
    bodyWidget = nullptr;
    chartWidget = nullptr;
    recommendationsWidget = nullptr;
    indicatorsWidget = nullptr;

    curSnap  = nullptr;
    scanner=nullptr;

    dbm = model.getDBM();


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
    connect(measureNowButton, &QPushButton::clicked, [this] () { showLoginPage();});
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
   if(menuPage == nullptr){

    // Create a layout for the Menu Page
    menuPage = new QWidget();
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

    qDebug() << "Menu page index: " << stackedWidget->indexOf(menuPage);
   }
   else{
       // should be emptyy
   }
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
            showMenuPage();
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

        qDebug() << "Profiles page index: " << stackedWidget->indexOf(profilesPage);
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
    }
}

void MainWindow::setupLoginPage()
{
    if(loginPage == nullptr){

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

    loginPage = new QWidget(this);
    loginPage->setLayout(loginLayout);

    // Add the login page to stacked widget
    stackedWidget->addWidget(loginPage);

    // Populate the dropdown with existing profiles
    populateUserDropdown();

    qDebug() << "Log in page index: " << stackedWidget->indexOf(loginPage);
    }

    else{
        //can be empty
    }

}

void MainWindow::setupCreateProfilePage()
{
    if(createProfPage == nullptr){

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

    createProfPage = new QWidget(this);
    createProfPage->setLayout(createProfLayout);

    // Add the create profile page to stacked widget
    stackedWidget->addWidget(createProfPage);

    qDebug() << "Create Profiles page index: " << stackedWidget->indexOf(createProfPage);
    }

    else{
        //CLEAR INPUT ROWS
    }
}

void MainWindow::setupMeasurePage()
{

   if(measurePage == nullptr){


   // Create the measure page
   measurePage = new QWidget(this);

   // Create layout
   QVBoxLayout *measureLayout = new QVBoxLayout;


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

   qDebug() << "Measure Now page index: " << stackedWidget->indexOf(measurePage);
   }

   else{

       if (!model.getCurProfile()) {
           QMessageBox::warning(this, "Error", "No profile selected. Please log in first.");
           showLoginPage();
           return;
       }


    }

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

    if(scanningPage == nullptr){
    scanningPage = new QWidget(this);
    QVBoxLayout *scanningLayout = new QVBoxLayout;

    QLabel *scanHeaderLabel = new QLabel("Scanning Page", this);
    QFont labelFont8("Arial", 16, QFont::Bold);
    scanHeaderLabel->setFont(labelFont8);
    scanningLayout->addWidget(scanHeaderLabel);

    // Add the battery label
    batteryLabel = new QLabel("Battery: 100%", this);
    batteryLabel->setAlignment(Qt::AlignCenter);
    scanningLayout->addWidget(batteryLabel);


    scanningInstructionLabel = new QLabel("Place the device on your skin and press the button below:", this);
    QFont labelFont7("Arial", 14, QFont::Bold);
    scanningInstructionLabel->setFont(labelFont7);
    scanningLayout->addWidget(scanningInstructionLabel);

    contactWithSkinButton = new QPushButton("Contact with Skin", this);
    //connect(contactWithSkinButton, &QPushButton::clicked, this, &MainWindow::startScanningProcess);
    scanningLayout->addWidget(contactWithSkinButton);

    scanningPage->setLayout(scanningLayout);
    stackedWidget->addWidget(scanningPage);

    qDebug() << "scannings page index: " << stackedWidget->indexOf(scanningPage);
    }

    else{
        //probably won't need this else
    }
}

void MainWindow::setupSnapshotDetailsPage() {

    if(snapshotDetailsPage ==  nullptr){
    snapshotDetailsPage = new QWidget(this);
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

        delete scanner;
        showMenuPage();
    });

    snapshotDetailsPage->setLayout(snapshotLayout);
    stackedWidget->addWidget(snapshotDetailsPage);

    qDebug() << "Snapshot details page index: " << stackedWidget->indexOf(snapshotDetailsPage);
    }

    else {
        weightInput->clear();
        bodyTempInput->clear();
        heartRateInput->clear();
        notesInput->clear();
        sleepHoursInput->clear();
        sleepMinutesInput->clear();
    }
}

void MainWindow::setupHistoryPage(){
    if(historyPage == nullptr){

    // Create the history page
    historyPage = new QWidget(this);

    // Create layout
    QVBoxLayout *historyLayout = new QVBoxLayout;

    historyLabel = new QLabel("History", this);
    QFont labelFont3("Arial", 16, QFont::Bold);
    historyLabel->setFont(labelFont3);
    historyLayout->addWidget(historyLabel);


    // Create a table widget for displaying history data
    historyTable = new QTableWidget(this);
    historyTable->setHorizontalHeaderLabels({"Date & Time", "Profile", "Notes"});
    historyTable->setColumnCount(3); // Date & Time, Profile, Notes
    for(int i = 0; i<3; i++){
        historyTable->setColumnWidth(i,200);
    }
    historyTable->setColumnWidth(2,400);


    if(snaps.length() == 0) {dbm->getAllSnapshots(snaps);}
    historyTable->setRowCount(snaps.length());


    for(int i = 0; i< snaps.length(); i++){
        historyTable->setItem(i, 0, new QTableWidgetItem(snaps[i]->getTimestamp()));
        historyTable->setItem(i, 1, new QTableWidgetItem(snaps[i]->getProfileID()));
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
        showMenuPage();
    });


    // Add Back button to layout
    historyLayout->addWidget(historyBackButton);


    // Set the layout for the history page
    historyPage->setLayout(historyLayout);


    // Add the history page to stacked widget
    stackedWidget->addWidget(historyPage);

    qDebug() << "History page index: " << stackedWidget->indexOf(historyPage);

    }

    else{
    //refresh case
        if(snaps.length() == 0) {dbm->getAllSnapshots(snaps);}
        historyTable->setRowCount(snaps.length());

        for(int i = 0; i< snaps.length(); i++){
            historyTable->setItem(i, 0, new QTableWidgetItem(snaps[i]->getTimestamp()));
            historyTable->setItem(i, 1, new QTableWidgetItem(model.getAllProfiles()[snaps[i]->getProfileID()-1]->getFullName()));
            historyTable->setItem(i, 2, new QTableWidgetItem(snaps[i]->getNotes()));
        }
    }
}

void MainWindow::setupBodyScreen()
{
   if(bodyWidget == nullptr){
   bodyWidget = new QWidget(this);
   QVBoxLayout *bodyLayout = new QVBoxLayout;


   QLabel *bodyScreenLabel = new QLabel("Body Screen", this);
   QFont labelFont4("Arial", 16, QFont::Bold);
   bodyScreenLabel->setFont(labelFont4);
   bodyLayout->addWidget(bodyScreenLabel);


   // Create table for organ readings
   organTable = new QTableWidget(this);
   organTable->setRowCount(12); // 12 organs to display (6 from H1-H6 and 6 from F1-F6)
   organTable->setColumnCount(2); // Column 1: Organ, Column 2: Status


   // Set table headers
   QStringList headers = {"Organ", "Status"};
   organTable->setHorizontalHeaderLabels(headers);
   organTable->setColumnWidth(0, 200);
   organTable->setColumnWidth(1,200);

   // Define font for organ rows
   QFont labelFont5("Arial", 14, QFont::Bold);


   // Define organ readings and ranges (from defs.h)


   // Loop through the organs and set their color based on the reading
   for (int i = 0; i < 12; ++i) {
       QTableWidgetItem *organItem = new QTableWidgetItem(organs[i]);
       organItem->setFont(labelFont5);
       organTable->setItem(i, 0, organItem);
    }

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
       showHistoryPage();
   });


   bodyWidget->setLayout(bodyLayout);
   stackedWidget->addWidget(bodyWidget);

    qDebug() << "Body page index: " << stackedWidget->indexOf(bodyWidget);

   }

   else{
      if(curSnap == nullptr){return;}
      setupChartPage();
      setupRecommendationsPage();
      setupIndicatorsPage();

      //POPULATE BODY CHART HERE
      QVector<QString> organVals = curSnap->getOrganValues();
      QFont labelFont5("Arial", 16, QFont::Bold);


      for(int i = 0; i < organVals.length(); i++){

          QColor color;
          if (organVals[i] == "Below Normal") {color = Qt::red;
          } else if (organVals[i] == "Above Normal") {color = Qt::blue;
          } else {color = Qt::green;
          }

          QTableWidgetItem *statusItem = new QTableWidgetItem(organVals[i]);
          statusItem->setBackground(color);
          statusItem->setFont(labelFont5);
          organTable->setItem(i, 1, statusItem);

      }




   }

}

void MainWindow::setupChartPage()
{
   if(chartWidget == nullptr){
   chartWidget = new QWidget(this);
   QVBoxLayout *chartLayout = new QVBoxLayout(chartWidget);


   // Add label for the chart
   QLabel *chartLabel = new QLabel("Chart Page");
   chartLabel->setAlignment(Qt::AlignCenter);
   chartLayout->addWidget(chartLabel);


   // Create a horizontal layout for the bar graph
   barLayout = new QHBoxLayout();

   chartLayout->addLayout(barLayout);


   // Add the Back button
   QPushButton *chartBackButton = new QPushButton("Back");
   connect(chartBackButton, &QPushButton::clicked, this, [this]() {
       showBodyScreen();
   });
   chartLayout->addWidget(chartBackButton);


   // Set the layout and add to the stacked widget
   chartWidget->setLayout(chartLayout);
   stackedWidget->addWidget(chartWidget);

    qDebug() << "Chart page index: " << stackedWidget->indexOf(chartWidget);
   }

   else{
       //POPULATE BAR CHART HERE
       if(curSnap  == nullptr) return;


       // Example data
       QVector<int> data = curSnap->getRawReadings();

       while (barLayout->count()) {
               QLayoutItem *item = barLayout->takeAt(0);  // Take out each child layout

               if (item->layout()) {  // If it's a QVBoxLayout
                   QVBoxLayout *subLayout = qobject_cast<QVBoxLayout *>(item->layout());

                   while (subLayout->count()) {
                       QLayoutItem *childItem = subLayout->takeAt(0);

                       if (childItem->widget()) {
                           delete childItem->widget();  // Delete the QLabel (barLabel or valueLabel)
                       }
                   }

                   delete subLayout;  // Delete the QVBoxLayout itself
               }
           }

       // Create bars as QLabels with QPixmap
       for (int value : data) {
           QLabel *barLabel = new QLabel();
           QPixmap pixmap(20, value*3);  // Set width to 20, height based on value
           pixmap.fill(Qt::blue);  // Fill the pixmap with a color (e.g., blue)
           barLabel->setPixmap(pixmap);
           barLabel->setAlignment(Qt::AlignBottom);  // Align to bottom of the bar

           QLabel *valueLabel = new QLabel(QString::number(value));
           valueLabel->setAlignment(Qt::AlignBottom);

           QVBoxLayout *barWithLabelLayout = new QVBoxLayout();
           barWithLabelLayout->addWidget(barLabel);
           barWithLabelLayout->addWidget(valueLabel);

           barWithLabelLayout->setAlignment(Qt::AlignBottom);

           barLayout->addLayout(barWithLabelLayout);

      }

   }

}

void MainWindow::setupRecommendationsPage()
{
    if(recommendationsWidget == nullptr){

    recommendationsWidget = new QWidget(this);
    QVBoxLayout *recommendationsLayout = new QVBoxLayout(recommendationsWidget);


    QFont labelFont5("Arial", 16, QFont::Bold);




    QLabel *recommendationsLabel = new QLabel("Recommendations Page");
    recommendationsLabel->setFont(labelFont5);
    recommendationsLabel->setAlignment(Qt::AlignCenter);
    recommendationsLayout->addWidget(recommendationsLabel);


    recBox = new QTextEdit();
    recBox->setText("");
    recBox->setWordWrapMode(QTextOption::WordWrap); // Ensure wrapping
    recBox->setReadOnly(true);
    recommendationsLayout->addWidget(recBox);

    QPushButton *recBackButton = new QPushButton("Back");
    connect(recBackButton, &QPushButton::clicked, this, [this]() {
        showBodyScreen();
    });
    recommendationsLayout->addWidget(recBackButton);


    recommendationsWidget->setLayout(recommendationsLayout);
    stackedWidget->addWidget(recommendationsWidget);

    qDebug() << "Recommendations page index: " << stackedWidget->indexOf(recommendationsWidget);
    }

    else{
        //POPULATE  RECOMMENDATIONS HERE
        if(curSnap == nullptr) return;

        recBox->setText(curSnap->getRecommendations());

    }

}

void MainWindow::setupIndicatorsPage()
{
    if(indicatorsWidget == nullptr){
    indicatorsWidget = new QWidget(this);
    QVBoxLayout *indicatorsLayout = new QVBoxLayout(indicatorsWidget);


    QFont labelFont5("Arial", 16, QFont::Bold);


    QLabel *indicatorsLabel = new QLabel("Indicators Page");
    indicatorsLabel->setFont(labelFont5);
    indicatorsLabel->setAlignment(Qt::AlignCenter);
    indicatorsLayout->addWidget(indicatorsLabel);

    indBox = new QTextEdit();
    indBox->setText("");
    indBox->setWordWrapMode(QTextOption::WordWrap); // Ensure wrapping
    indBox->setReadOnly(true);
    indBox->setText("This is text which stands as a placeholder for the indicators. Some specific  data aboout energy levels tc. goes here.");
    indicatorsLayout->addWidget(indBox);


    QPushButton *indBackButton = new QPushButton("Back");
    connect(indBackButton, &QPushButton::clicked, this, [this]() {
        showBodyScreen();
    });
    indicatorsLayout->addWidget(indBackButton);


    indicatorsWidget->setLayout(indicatorsLayout);
    stackedWidget->addWidget(indicatorsWidget);

    qDebug() << "Indicators page index: " << stackedWidget->indexOf(indicatorsWidget);
    }

    else{
        //POPULATE INDICATORS PAGE WITH DATA HERE

        if(curSnap == nullptr) return;
        QVector<int> raw  = curSnap->getRawReadings();

        float energy =( raw[0] + raw[1] + raw[2] + raw[3]) / 78.0;
        QString enLevel = (energy < 2.2) ? "Below Normal" : (energy < 3.4) ? "Normal" : "Above Normal";
        float immune =( raw[5] + raw[6] + raw[7] + raw[9]) / 62.0;
        QString imnLevel = (immune < 2.5) ? "Below Normal" : (immune < 2.9) ? "Normal" : "Above Normal";
        float meta =( raw[11] + raw[12] + raw[16]) / 51.0;
        QString metLevel = (meta < 1.1) ? "Below Normal" : (meta < 3.4) ? "Normal" : "Above Normal";
        float emo =( raw[9] + raw[14] + raw[13] + raw[21]) / 110.0;
        QString emLevel = (emo < 0.3) ? "Below Normal" : (emo < 1.2) ? "Normal" : "Above Normal";
        float mus =( raw[22] + raw[8] + raw[13] + raw[23]) / 41.0;
        QString musLevel = (mus < 3.5) ? "Below Normal" : (mus < 5.6) ? "Normal" : "Above Normal";



        QString preppedText = QString(R"(Your scan results show the following to be true:
                Energy Level: %1  %2
                Immune System: %3  %4
                Metabolism:  %5  %6
                Psycho-emotional State: %7  %8
                Musculoskeletal System: %9  %10

                This tells us specifically that [...])")
                .arg(QString::number(energy, 'f', 2))    // Format as 2 decimal places
                .arg(enLevel)
                .arg(QString::number(immune, 'f', 2))
                .arg(imnLevel)
                .arg(QString::number(meta, 'f', 2))
                .arg(metLevel)
                .arg(QString::number(emo, 'f', 2))
                .arg(emLevel)
                .arg(QString::number(mus, 'f', 2))
                .arg(musLevel);


        indBox->setText(preppedText);



    }


}


//GUI SHOW FUNCTIONS
void MainWindow::showLoginPage()
{
    stackedWidget->setCurrentIndex(3);
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

void MainWindow::showProfilesPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showChartPage()
{
    stackedWidget->setCurrentIndex(9);
}

void MainWindow::showIndicatorsPage()
{
    stackedWidget->setCurrentIndex(10);
}

void MainWindow::showRecommendationsPage()
{
    stackedWidget->setCurrentIndex(11);
}

void MainWindow::showScanningPage() {
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showSnapshotDetailsPage()
{
    stackedWidget->setCurrentIndex(6);
}

void MainWindow::showHistoryPage()
{
    setupHistoryPage();
    stackedWidget->setCurrentIndex(7);
}

void MainWindow::showBodyScreen()
{
    stackedWidget->setCurrentIndex(8);
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
    scanner = model.startScan();
    if (scanner == nullptr) {
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
       if(scanner){
           delete scanner;
       }

       batteryLevel = 100;
       showMenuPage();
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


    if (scanner) {

        float weight = weightInput->text().toFloat();
        float bodyTemp = bodyTempInput->text().toFloat();
        int heartRate = heartRateInput->text().toInt();
        QString notes = notesInput->toPlainText()   ;
        int sleepHours = sleepHoursInput->text().toInt();
        int sleepMinutes = sleepMinutesInput->text().toInt();


        int currentYear = QDateTime::currentDateTime().date().year();
        int currentMonth = QDateTime::currentDateTime().date().month();
        int currentDay = QDateTime::currentDateTime().date().day();
        int currentHour = QDateTime::currentDateTime().time().hour();
        int currentMinute = QDateTime::currentDateTime().time().minute();

        if (weight <= 0 || bodyTemp <= 0 || heartRate <= 0 || sleepHours < 0 || sleepMinutes < 0) {
            QMessageBox::warning(this, "Input Error", "Please enter positive numbers in numerical  fields.");
            return;
        }


        scanner->registerWeight(weight);
        scanner->registerBlood('L', 0,0);
        scanner->registerBlood('R',0,0);
        scanner->registerBodyTemp(bodyTemp);
        scanner->registerHeartRate(heartRate);
        scanner->registerSleepTime(sleepHours,sleepMinutes);
        scanner->registerTime(currentHour,currentMinute);
        scanner->registerDate(currentMonth, currentDay,currentYear);
        scanner->registerNotes(notes);
        // -----

        if(!scanner->finishScan()) {
            QMessageBox::warning(this, "Scan Incomplete", "Scan parameters incomplete");
            return;
        }

       curSnap = scanner->getFinishedSnap();
       snaps.append(curSnap);
       delete scanner;
       scanner = nullptr;


       QMessageBox::information(this, "Measurement Complete", "The measurement is complete.");
       setupBodyScreen();
       showBodyScreen();

    } else {
        QMessageBox::information(this, "Measurement Complete", "The measurement is complete.");
    }

}

void MainWindow::startScanningProcess() {
    const int totalPoints = 24;
    for (int i = 0; i < totalPoints; i++) {

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
            int reading = scanner->genScanVal(limbChar, i%6);
            scanner->registerReading(sideChar, limbChar, reading);
        }
    }

    setupSnapshotDetailsPage();
    showSnapshotDetailsPage();

}

void MainWindow::onSaveSnapshotClicked() {
    // Check if all fields are filled
    if (weightInput->text().isEmpty() || heartRateInput->text().isEmpty() ||
        bodyTempInput->text().isEmpty() || sleepHoursInput->text().isEmpty() ||
        sleepMinutesInput->text().isEmpty() || notesInput->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Incomplete Data", "Please fill all the input fields.");
    } else {
        finishMeasurement();
    }
}

void MainWindow::onLoginButtonClicked() {
    int selectedIndex = userDropdown->currentIndex() - 1;
    if (selectedIndex >= 0 && model.selectProfile(selectedIndex)) {

        showScanningPage();
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

    //This sets current snapshot to be displayed as the profile's snap at associated index
    curSnap = snaps[row];

    // Example: Navigate to the Body Screen and display detailed info
    setupBodyScreen(); // Ensure this method sets up the Body Screen if not already do
    // TBD
    showBodyScreen();
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
