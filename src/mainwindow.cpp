#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //!!! Initialize the temporary credentials
    userCredentials.insert("testuser", "password123");
    userCredentials.insert("user2", "pass456");


    // Set up the stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Set up the Login Page
    setupLoginPage();

    // Set up the Create Profile Page
    setupCreateProfilePage();

    setupMenuPage();

    setupMeasurePage();

    setupProfilesPage();

    setupBattery();

    setupHistoryPage();

    setupBodyScreen();


    setupChartPage();
    setupIndicatorsPage();
    setupRecommendationsPage();


    // Connect buttons to respective slots
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
    connect(createProfButton, &QPushButton::clicked, this, &MainWindow::onCreateProfButtonClicked);
    connect(saveProfButton, &QPushButton::clicked, this, &MainWindow::onSaveProfButtonClicked);
    connect(measureNowButton, &QPushButton::clicked, [this] () { stackedWidget->setCurrentIndex(3);});
    connect(profilesButton, &QPushButton::clicked, [this]() {
    setupProfilesPage();    // Ensure the Profiles Page is initialized
    populateProfilesTable(); // Update the table data dynamically
    stackedWidget->setCurrentIndex(4); // Go to the Profiles Page
    });
    connect(historyButton, &QPushButton::clicked, [this]() {
    stackedWidget->setCurrentIndex(5);


});
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupLoginPage()
{
    // Initialize the login page widgets
    welcomeLabel = new QLabel("Welcome to the App", this);
    pleaseLogInStatement = new QLabel("Please log in to continue", this);

    userDropdown = new QComboBox(this);
    userDropdown->setEditable(false); // Prevent manual entry
    userDropdown->addItem("Select User"); // Default placeholder
    //Connect the dropdown's signal to the login slot
    connect(userDropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) { if (index > 0) { onLoginButtonClicked(); } });


    noAccLabel = new QLabel("Don't have an account?", this);
    createProfButton = new QPushButton("Create New Profile", this);


    // Create a QFont for customizing the font
    QFont labelFont("Arial", 16, QFont::Bold);

    // Apply the font to the labels
    welcomeLabel->setFont(labelFont);
    pleaseLogInStatement->setFont(labelFont);
    noAccLabel->setFont(labelFont);


    // Create layout for login page
    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addWidget(welcomeLabel);
    loginLayout->addWidget(pleaseLogInStatement);
    loginLayout->addWidget(userDropdown);
    loginLayout->addWidget(noAccLabel);
    loginLayout->addWidget(createProfButton);


    QWidget *loginPage = new QWidget(this);
    loginPage->setLayout(loginLayout);


    // Add the login page to stacked widget
    stackedWidget->addWidget(loginPage);


    // Populate the dropdown with existing profiles
    populateUserDropdown();
}

void MainWindow::populateUserDropdown()
{
    userDropdown->clear();
    userDropdown->addItem("Select User");

    if (userProfiles.isEmpty()) {
        QMessageBox::information(this, "No Profiles", "No profiles available. Please create a new profile.");
        return;
    }

    for (const QString &user : userProfiles) {
        userDropdown->addItem(user);
    }
}



void MainWindow::setupCreateProfilePage()
{
    // Initialize the create profile page widgets
    createProfLabel = new QLabel("Create Your Profile", this);
    fNameLabel = new QLabel("First Name:", this);
    lNameLabel = new QLabel("Last Name:", this);
    weightLabel = new QLabel("Weight (kg):", this);
    heightLabel = new QLabel("Height (cm):", this);
    dobLabel = new QLabel("Date of Birth:", this);
    countryLabel = new QLabel("Country:", this);
    fNameInput = new QLineEdit(this);
    lNameInput = new QLineEdit(this);
    weightInput = new QLineEdit(this);
    heightInput = new QLineEdit(this);
    dobInput = new QLineEdit(this);
    countryInput = new QLineEdit(this);
    saveProfButton = new QPushButton("Save Profile", this);
    connect(saveProfButton, &QPushButton::clicked, this, &MainWindow::onSaveProfButtonClicked);


    QFont labelFont2("Arial", 16, QFont::Bold);

    // Apply the font to the labels
    fNameLabel->setFont(labelFont2);
    lNameLabel->setFont(labelFont2);
    weightLabel->setFont(labelFont2);
    heightLabel->setFont(labelFont2);
    dobLabel->setFont(labelFont2);
    countryLabel->setFont(labelFont2);


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
    createProfLayout->addWidget(countryLabel);
    createProfLayout->addWidget(countryInput);
    createProfLayout->addWidget(saveProfButton);


    QWidget *createProfPage = new QWidget(this);
    createProfPage->setLayout(createProfLayout);

    // Add the create profile page to stacked widget
    stackedWidget->addWidget(createProfPage);
}




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
    logoutButton = new QPushButton("Log Out");


    verticalLayout_3->addWidget(measureNowButton);
    verticalLayout_3->addWidget(profilesButton);
    verticalLayout_3->addWidget(historyButton);
    verticalLayout_3->addWidget(logoutButton);


    // Set the layout for the menu page
    menuPage->setLayout(verticalLayout_3);
    stackedWidget->addWidget(menuPage);

}



void MainWindow::setupMeasurePage()
{
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




   QPushButton *rightHandButton = new QPushButton("Measure Right Hand", this);
   connect(rightHandButton, &QPushButton::clicked, this, [this]() { measureHands("Right"); });
   measureLayout->addWidget(rightHandButton);


   QPushButton *leftHandButton = new QPushButton("Measure Left Hand", this);
   connect(leftHandButton, &QPushButton::clicked, this, [this]() { measureHands("Left"); });
   measureLayout->addWidget(leftHandButton);


   QPushButton *rightFootButton = new QPushButton("Measure Right Foot", this);
   connect(rightFootButton, &QPushButton::clicked, this, [this]() { measureFeet("Right"); });
   measureLayout->addWidget(rightFootButton);


   QPushButton *leftFootButton = new QPushButton("Measure Left Foot", this);
   connect(leftFootButton, &QPushButton::clicked, this, [this]() { measureFeet("Left"); });
   measureLayout->addWidget(leftFootButton);




   // Finish Button to go back to History or Menu page
   QPushButton* finishButton = new QPushButton("Finish Measurement", this);
   connect(finishButton, &QPushButton::clicked, this, &MainWindow::finishMeasurement);
   measureLayout->addWidget(finishButton);




   // Create a "Back" button
   QPushButton *measureBackButton = new QPushButton("Back", this);
   connect(measureBackButton, &QPushButton::clicked, [this]() {
       stackedWidget->setCurrentIndex(2); // Go back to Menu page
   });




   // Add Back button to layout
   measureLayout->addWidget(measureBackButton);


   // Set the layout for the measure page
   measurePage->setLayout(measureLayout);




   // Add the history page to stacked widget
   stackedWidget->addWidget(measurePage);


   //qDebug() << "Measure Now page index: " << stackedWidget->indexOf(measurePage);


}


//void MainWindow::measureHands(const QString &side) {
//   // Logic for measuring hands
//   qDebug() << "Measuring " << side << " Hand";
//   // Handle the measurement for the specified side (Left/Right)
//   // You can store the result in a database or QVector later
//}


//void MainWindow::measureFeet(const QString &side) {
//   // Logic for measuring feet
//   qDebug() << "Measuring " << side << " Foot";
//   // Handle the measurement for the specified side (Left/Right)
//   // Store the result or update UI
//}


void MainWindow::measureHands(const QString &side) {
   // Simulate data for measurement
   int reading = QRandomGenerator::global()->bounded(50, 150); // Generate a random reading


   qDebug() << "Measuring" << side << "Hand with reading:" << reading;


   // Assuming Scanner is initialized as a member variable
//   if (scanner) {
//       char sideChar = (side == "Left") ? 'L' : 'R';
//       scanner->registerReading(sideChar, 'H', reading); // H for Hand
//   } else {
//       qWarning() << "Scanner not initialized.";
//   }


   // TODO: Update UI or indicate that the hand measurement was successful
   QMessageBox::information(this, "Measurement Complete", side + " Hand measurement recorded.");
}


void MainWindow::measureFeet(const QString &side) {
   // Simulate data for measurement
   int reading = QRandomGenerator::global()->bounded(50, 150); // Generate a random reading


   qDebug() << "Measuring" << side << "Foot with reading:" << reading;


   // Assuming Scanner is initialized as a member variable
//   if (scanner) {
//       char sideChar = (side == "Left") ? 'L' : 'R';
//       scanner->registerReading(sideChar, 'F', reading); // F for Foot
//   } else {
//       qWarning() << "Scanner not initialized.";
//   }


   // TODO: Update UI or indicate that the foot measurement was successful
   QMessageBox::information(this, "Measurement Complete", side + " Foot measurement recorded.");
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


void MainWindow::startMeasurement()
{
   if (!batteryDepletionTimer || !batteryLabel) {
       qDebug() << "Battery components are not initialized!";
       return;
   }


   if (!batteryDepletionTimer->isActive()) {
       batteryDepletionTimer->start(2400);
       qDebug() << "Battery depletion timer started.";
   }
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


void MainWindow::finishMeasurement()
{
   if (batteryDepletionTimer && batteryDepletionTimer->isActive()) {
       batteryDepletionTimer->stop();
   }
   QMessageBox::information(this, "Measurement Complete", "The measurement is complete.");
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
    historyTable->setColumnCount(3); // Example: Date, Time, Summary
    historyTable->setHorizontalHeaderLabels({"Date", "Time", "Summary"});


    /*
    // !!!Check if the database connection is open
    if (db.isOpen()) {
        QSqlQuery query("SELECT * FROM History ORDER BY date DESC");




        int row = 0;
        while (query.next()) {
            historyTable->insertRow(row);
            historyTable->setItem(row, 0, new QTableWidgetItem(query.value("date").toString()));
            historyTable->setItem(row, 1, new QTableWidgetItem(query.value("metric").toString()));
            historyTable->setItem(row, 2, new QTableWidgetItem(query.value("value").toString()));
            historyTable->setItem(row, 3, new QTableWidgetItem(query.value("notes").toString()));
            row++;
        }
    } else {
        // Database connection is not open, show placeholder data
        historyTable->insertRow(0);
        historyTable->setItem(0, 0, new QTableWidgetItem("11-29-2024"));
        historyTable->setItem(0, 1, new QTableWidgetItem("Blood Pressure"));
        historyTable->setItem(0, 2, new QTableWidgetItem("120/80"));
        historyTable->setItem(0, 3, new QTableWidgetItem("Normal"));




        historyTable->insertRow(1);
        historyTable->setItem(1, 0, new QTableWidgetItem("11-28-2024"));
        historyTable->setItem(1, 1, new QTableWidgetItem("Heart Rate"));
        historyTable->setItem(1, 2, new QTableWidgetItem("72 bpm"));
        historyTable->setItem(1, 3, new QTableWidgetItem("Normal"));
    }




    */


    // !!!Populate with placeholder data
    historyTable->setRowCount(3); // Example: 3 rows
    historyTable->setItem(0, 0, new QTableWidgetItem("2024-11-01"));
    historyTable->setItem(0, 1, new QTableWidgetItem("10:00 AM"));
    historyTable->setItem(0, 2, new QTableWidgetItem("Good health"));


    historyTable->setItem(1, 0, new QTableWidgetItem("2024-11-15"));
    historyTable->setItem(1, 1, new QTableWidgetItem("02:30 PM"));
    historyTable->setItem(1, 2, new QTableWidgetItem("Minor concerns"));


    historyTable->setItem(2, 0, new QTableWidgetItem("2024-11-27"));
    historyTable->setItem(2, 1, new QTableWidgetItem("08:45 PM"));
    historyTable->setItem(2, 2, new QTableWidgetItem("Stable"));


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
        stackedWidget->setCurrentIndex(2); // Go back to Menu page
    });


    // Add Back button to layout
    historyLayout->addWidget(historyBackButton);


    // Set the layout for the history page
    historyPage->setLayout(historyLayout);


    // Add the history page to stacked widget
    stackedWidget->addWidget(historyPage);

    //qDebug() << "History page index: " << stackedWidget->indexOf(historyPage);

}

void MainWindow::setupProfilesPage()
{
    if (profilesPage != nullptr) {
        stackedWidget->setCurrentWidget(profilesPage);
        return; // If already set up, skip reinitialization
    }

    profilesPage = new QWidget(this);
    QVBoxLayout *profilesLayout = new QVBoxLayout;


    QLabel *profilesLabel = new QLabel("Manage Profiles", this);
    QFont labelFont("Arial", 16, QFont::Bold);
    profilesLabel->setFont(labelFont);
    profilesLayout->addWidget(profilesLabel);


    // Table to display profiles
    profilesTable = new QTableWidget(this);
    profilesTable->setColumnCount(4); // Example columns: First Name, Last Name, Weight, Height
    profilesTable->setHorizontalHeaderLabels({"First Name", "Last Name", "Weight", "Height"});
    profilesLayout->addWidget(profilesTable);

    //edit & delete buttons
    QPushButton *editProfileButton = new QPushButton("Edit", this);
    QPushButton *deleteProfileButton = new QPushButton("Delete", this);

    // Back Button
    QPushButton *profilesBackButton = new QPushButton("Back");
    connect(profilesBackButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(2); // Go back to Menu page
    });

    profilesLayout->addWidget(editProfileButton);
    profilesLayout->addWidget(deleteProfileButton);
    profilesLayout->addWidget(profilesBackButton);


    profilesPage->setLayout(profilesLayout);
    stackedWidget->addWidget(profilesPage);

    populateProfilesTable();



    connect(editProfileButton, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
    connect(deleteProfileButton, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
}

void MainWindow::populateProfilesTable()
{
    if (!profilesTable) return;

    qDebug() << "Updating profile table for user: " << currentFirstName << currentLastName;


    profilesTable->clearContents(); // Clear existing data
    profilesTable->setRowCount(1); // Display only the current user

    profilesTable->setItem(0, 0, new QTableWidgetItem(currentFirstName));
    profilesTable->setItem(0, 1, new QTableWidgetItem(currentLastName));
    profilesTable->setItem(0, 2, new QTableWidgetItem(QString::number(currentWeight))); // Assuming currentWeight is stored
    profilesTable->setItem(0, 3, new QTableWidgetItem(QString::number(currentHeight))); // Assuming currentHeight is stored

    profilesTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent direct editing
}


/*
void MainWindow::setupBodyScreen()
{
    QWidget *bodyWidget = new QWidget(this);
    QVBoxLayout *bodyLayout = new QVBoxLayout;


    QLabel *bodyScreenLabel = new QLabel("Body Screen", this);
    QFont labelFont4("Arial", 16, QFont::Bold);
    bodyScreenLabel->setFont(labelFont4);
    bodyLayout->addWidget(bodyScreenLabel);


    // Define the font style for the labels
    QFont labelFont5("Arial", 16, QFont::Bold);


    // Create labels for each organ/part (based on database data)
    QLabel *heartLabel = new QLabel("Heart: Normal");
    heartLabel->setStyleSheet("color: green;");
    heartLabel->setFont(labelFont5);
    bodyLayout->addWidget(heartLabel);


    QLabel *liverLabel = new QLabel("Liver: Above Normal");
    liverLabel->setStyleSheet("color: blue;");
    liverLabel->setFont(labelFont5);
    bodyLayout->addWidget(liverLabel);



    QLabel *kidneyLabel = new QLabel("Kidney: Below Normal");
    kidneyLabel->setStyleSheet("color: red;");
    kidneyLabel->setFont(labelFont5);
    bodyLayout->addWidget(kidneyLabel);


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
*/
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




void MainWindow::showChartPage()
{
    // Placeholder for chart page
    stackedWidget->setCurrentIndex(7);
}


void MainWindow::showIndicatorsPage()
{
    // Placeholder for indicators page
    stackedWidget->setCurrentIndex(8);
}


void MainWindow::showRecommendationsPage()
{
    // Placeholder for recommendations page
    stackedWidget->setCurrentIndex(9);
}


void MainWindow::showHistoryPage()
{
    // Navigate back to the History Page
    stackedWidget->setCurrentIndex(3);
}





void MainWindow::onLogoutButtonClicked()
{
    // Clear session-specific data
    userNameLabel->clear();

    // Navigate to the Login Page
    stackedWidget->setCurrentIndex(0);
}




void MainWindow::onLoginButtonClicked()
{
    QString selectedUser = userDropdown->currentText();

    if (selectedUser == "Select User") {
        QMessageBox::warning(this, "Login Error", "Please select a user!");
        return;
    }

    // Split the name to update the Menu page greeting:
    QStringList nameParts = selectedUser.split(' ');
    if (nameParts.size() == 2) {
        currentFirstName = nameParts[0];
        currentLastName = nameParts[1];
    }

    // Debugging output
    qDebug() << "Logged in user: " << currentFirstName << currentLastName;
    qDebug() << "Weight: " << currentWeight << "Height: " << currentHeight;


    // Proceed to the Menu Page
    stackedWidget->setCurrentIndex(2);
    userNameLabel->setText("Hello, " + selectedUser);
}






// Slot for handling create profile button click
void MainWindow::onCreateProfButtonClicked()
{
    // Switch to the create profile page (index 1)
    stackedWidget->setCurrentIndex(1);
}




// Slot for handling save profile button click
void MainWindow::onSaveProfButtonClicked()
{
    // Fetch input values
    QString fname = fNameInput->text().trimmed();
    QString lname = lNameInput->text().trimmed();
    QString weight = weightInput->text().trimmed();
    QString height = heightInput->text().trimmed();
    QString dob = dobInput->text().trimmed();
    QString country = countryInput->text().trimmed();

    // Validate inputs
    if (fname.isEmpty() || lname.isEmpty() || weight.isEmpty() || height.isEmpty() || dob.isEmpty() || country.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "All fields are required!");
        return;
    }

    // Combine fname and lname to create a full name
    QString fullName = fname + " " + lname;

    // Check for duplicate profiles
    if (!userProfiles.contains(fullName)) {
        userProfiles.append(fullName);  // Add the new user to the list
        populateUserDropdown();         // Update the dropdown in the Welcome page
        QMessageBox::information(this, "Success", "Profile created successfully!");
    } else {
        QMessageBox::warning(this, "Duplicate Entry", "A profile with this name already exists!");
        return;
    }

    // Clear input fields after saving
    fNameInput->clear();
    lNameInput->clear();
    weightInput->clear();
    heightInput->clear();
    dobInput->clear();
    countryInput->clear();

    // Navigate back to the Welcome page
    stackedWidget->setCurrentIndex(0);

}



void MainWindow::updateGreeting(const QString &firstName, const QString &lastName)
{
    userGreetingLabel->setText("Hello, ");
    userNameLabel->setText(firstName + " " + lastName); // Display full name
}


void MainWindow::measureWrist()
{
    // // Simulate wrist measurement logic
    // qDebug() << "Wrist measurement started.";
    // // You could update the UI here, e.g., showing a status or progress
    // // Example: QLabel to show status
    // statusLabel->setText("Wrist measurement in progress...");
}


void MainWindow::measureAnkle()
{
    // // Simulate ankle measurement logic
    // qDebug() << "Ankle measurement started.";
    // // You could similarly update the UI here
    // statusLabel->setText("Ankle measurement in progress...");
}


void MainWindow::addMeasurementToHistory(const QString &result) {
    // // Assuming you have a QTableWidget for the history page
    // QTableWidget *historyTable = historyPage->findChild<QTableWidget *>("historyTable");

    // // Add a new row to the table with the result
    // int row = historyTable->rowCount();
    // historyTable->insertRow(row);

    // // Add data to the new row
    // historyTable->setItem(row, 0, new QTableWidgetItem(QDate::currentDate().toString()));  // Date
    // historyTable->setItem(row, 1, new QTableWidgetItem(result));  // Measurement result
}





void MainWindow::onHistoryRowClicked(int row, int column)
{
    // Access the QTableWidget (history table)
    QTableWidget *historyTable = qobject_cast<QTableWidget *>(sender());
    if (!historyTable) return; // Safety check


    // Retrieve data from the clicked row (for example, the first column)
    QString date = historyTable->item(row, 0)->text();
    QString time = historyTable->item(row, 1)->text();
    QString summary = historyTable->item(row, 2)->text();


    // Debug: Print the row data to confirm functionality !!!
    qDebug() << "Row clicked:" << row << ", Column:" << column;
    qDebug() << "Date:" << date << ", Time:" << time << ", Summary:" << summary;


    // Example: Navigate to the Body Screen and display detailed info
    setupBodyScreen(); // Ensure this method sets up the Body Screen if not already done
    stackedWidget->setCurrentIndex(6); // Assuming the Body Screen index is 5


    // Optionally, pass the clicked data to the Body Screen (e.g., via labels or widgets)
}

void MainWindow::onEditProfileClicked()
{
    // Ensure the profile has been selected
    if (currentFirstName.isEmpty() || currentLastName.isEmpty()) {
        QMessageBox::warning(this, "Edit Error", "No profile selected. Please log in first.");
        return;
    }

    // Create a dialog or input fields to edit the profile
    bool ok;
    double newWeight = QInputDialog::getDouble(this, "Edit Weight",
                                              "Enter new weight (kg):", currentWeight,
                                              0.0, 200.0, 2, &ok);
    if (ok) {
        currentWeight = newWeight; // Update weight
    } else {
        return; // If user cancels, do nothing
    }

    double newHeight = QInputDialog::getDouble(this, "Edit Height",
                                               "Enter new height (cm):", currentHeight,
                                               50.0, 300.0, 2, &ok);
    if (ok) {
        currentHeight = newHeight; // Update height
    } else {
        return; // If user cancels, do nothing
    }

    // Update the profile table
    populateProfilesTable();

    // Inform the user of the successful update
    QMessageBox::information(this, "Profile Updated", "The profile has been successfully updated.");

    // Optionally, update other areas of the app, such as the dropdown
    populateUserDropdown(); // Ensure the dropdown reflects the updated data
}


void MainWindow::onDeleteProfileClicked()
{
    // Confirm the deletion
    int result = QMessageBox::question(this, "Delete Profile",
        "Are you sure you want to delete this profile?",
        QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::No) {
        return; // Exit if the user cancels
    }

    // Find the full name of the current user
    QString fullName = currentFirstName + " " + currentLastName;

    // Remove the user from the QVector
    int userIndex = userProfiles.indexOf(fullName);
    if (userIndex != -1) {
        userProfiles.removeAt(userIndex);
    }

    // Clear current user data
    currentFirstName.clear();
    currentLastName.clear();
    currentWeight = 0.0;
    currentHeight = 0.0;

    // Update the dropdown on the Welcome Page
    populateUserDropdown();

    // Show success message
    QMessageBox::information(this, "Profile Deleted", "The profile has been deleted.");

    // Navigate back to the Welcome Page
    stackedWidget->setCurrentIndex(0); // Assuming Welcome Page is index 0
}

