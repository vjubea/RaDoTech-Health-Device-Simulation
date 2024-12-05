#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "scanner.h"
#include "model.h"
#include "snapshot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    curScanner = nullptr;
    displayedSnap = nullptr;


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

    setupHistoryPage();

    setupBodyScreen();


    setupChartPage();
    setupIndicatorsPage();
    setupRecommendationsPage();


    // Connect buttons to respective slots
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
    connect(createProfButton, &QPushButton::clicked, this, &MainWindow::onCreateProfButtonClicked);
    connect(saveProfButton, &QPushButton::clicked, this, &MainWindow::onSaveProfButtonClicked);
    connect(measureNowButton, &QPushButton::clicked, [this] () { stackedWidget->setCurrentIndex(3);});
    connect(profilesButton, &QPushButton::clicked, [this]() {
    setupProfilesPage();
    stackedWidget->setCurrentIndex(4);});
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
    noAccLabel = new QLabel("Don't have an account?", this);
    userLabel = new QLabel("Username:", this);
    pwLabel = new QLabel("Password:", this);
    userInput = new QLineEdit(this);
    pwInput = new QLineEdit(this);
    loginButton = new QPushButton("Log In", this);
    createProfButton = new QPushButton("Create New Profile", this);


    // Create a QFont for customizing the font
    QFont labelFont("Arial", 16, QFont::Bold);


    // Apply the font to the labels
    welcomeLabel->setFont(labelFont);
    pleaseLogInStatement->setFont(labelFont);
    userLabel->setFont(labelFont);
    pwLabel->setFont(labelFont);
    noAccLabel->setFont(labelFont);


    // Create layout for login page
    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addWidget(welcomeLabel);
    loginLayout->addWidget(pleaseLogInStatement);
    loginLayout->addWidget(userLabel);
    loginLayout->addWidget(userInput);
    loginLayout->addWidget(pwLabel);
    loginLayout->addWidget(pwInput);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(noAccLabel);
    loginLayout->addWidget(createProfButton);


    QWidget *loginPage = new QWidget(this);
    loginPage->setLayout(loginLayout);


    // Add the login page to stacked widget
    stackedWidget->addWidget(loginPage);
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
    pwLabel2 = new QLabel("Password:", this);
    fNameInput = new QLineEdit(this);
    lNameInput = new QLineEdit(this);
    weightInput = new QLineEdit(this);
    heightInput = new QLineEdit(this);
    dobInput = new QLineEdit(this);
    countryInput = new QLineEdit(this);
    pwInput2 = new QLineEdit(this);
    saveProfButton = new QPushButton("Save Profile", this);


    QFont labelFont2("Arial", 16, QFont::Bold);

    // Apply the font to the labels
    fNameLabel->setFont(labelFont2);
    lNameLabel->setFont(labelFont2);
    weightLabel->setFont(labelFont2);
    heightLabel->setFont(labelFont2);
    dobLabel->setFont(labelFont2);
    countryLabel->setFont(labelFont2);
    pwLabel2->setFont(labelFont2);


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
    createProfLayout->addWidget(pwLabel2);
    createProfLayout->addWidget(pwInput2);
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
    userGreetingLabel = new QLabel("Hello, ");
    userNameLabel = new QLabel("");


    QHBoxLayout *horizantalLayout_2 = new QHBoxLayout();
    horizantalLayout_2->addWidget(userGreetingLabel);
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


    // Instruction Label
    QLabel *instructionLabel = new QLabel("Follow the instructions to measure your body parts:", this);
    QFont labelFont5("Arial", 16, QFont::Bold);
    instructionLabel->setFont(labelFont5);
    measureLayout->addWidget(instructionLabel);


    // Buttons for body parts
    QPushButton *wristButton = new QPushButton("Measure Wrist", this);
    connect(wristButton, &QPushButton::clicked, this, &MainWindow::measureWrist);
    measureLayout->addWidget(wristButton);


    QPushButton *ankleButton = new QPushButton("Measure Ankle", this);
    connect(ankleButton, &QPushButton::clicked, this, &MainWindow::measureAnkle);
    measureLayout->addWidget(ankleButton);


    // Add more buttons for other body parts as needed...


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

    // Set the layout for the history page
    measurePage->setLayout(measureLayout);


    // Add the history page to stacked widget
    stackedWidget->addWidget(measurePage);

    //qDebug() << "Measure Now page index: " << stackedWidget->indexOf(measurePage);

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
    /*
    VLAD: Each Entry in History Table should have 2 columns, as Date and Time of snapshot are merged as timestamp
    in format "yyyy-MM-dd hh:mm". The other one should be the Profile to whom the Snapshot belongs:
    I could make the Database return the snapshots by the newest first if needed. Just text me.
        Ex:
        08-12=2024 07:29        AnotherProfile
        08-10=2024 11:29        VladProfile
    */
    QTableWidget *historyTable = new QTableWidget(this);
    historyTable->setColumnCount(3); // Example: Date, Time, Summary
    historyTable->setHorizontalHeaderLabels({"Date", "Time", "Summary"});
    /* 
    VLAD: The Snapshot page should display bargraph with H1-F6 with their corresponding body part name, 
    with a bar showing the average of Right and Left Readings.
    We should pass the snapshot obj represented by the historyTable object we click.
    */
    
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
    QWidget *profilesPage = new QWidget(this);
    QVBoxLayout *profilesLayout = new QVBoxLayout;


    QLabel *profilesLabel = new QLabel("Manage Profiles", this);
    QFont labelFont("Arial", 16, QFont::Bold);
    profilesLabel->setFont(labelFont);
    profilesLayout->addWidget(profilesLabel);


    // Table to display profiles
    QTableWidget *profilesTable = new QTableWidget(this);
    profilesTable->setColumnCount(5); // Example columns: First Name, Last Name, Weight, Height, Actions
    profilesTable->setHorizontalHeaderLabels({"First Name", "Last Name", "Weight", "Height", "Actions"});
    profilesLayout->addWidget(profilesTable);


    //!!!check this
    // Populate profiles from database
    // QSqlQuery query("SELECT * FROM Profiles");
    // int row = 0;
    // while (query.next()) {
    //     profilesTable->insertRow(row);
    //     profilesTable->setItem(row, 0, new QTableWidgetItem(query.value("fname").toString()));
    //     profilesTable->setItem(row, 1, new QTableWidgetItem(query.value("lname").toString()));
    //     profilesTable->setItem(row, 2, new QTableWidgetItem(query.value("weight").toString()));
    //     profilesTable->setItem(row, 3, new QTableWidgetItem(query.value("height").toString()));




    //     // Add Edit/Delete buttons
    //     QPushButton *editProfileButton = new QPushButton("Edit");
    //     QPushButton *deleteProfileButton = new QPushButton("Delete");
    //     connect(editProfileButton, &QPushButton::clicked, [this, row]() { editProfile(row); });
    //     connect(deleteProfileButton, &QPushButton::clicked, [this, row]() { deleteProfile(row); });




    //     QWidget *actionWidget = new QWidget;
    //     QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    //     actionLayout->addWidget(editProfileButton);
    //     actionLayout->addWidget(deleteProfileButton);




    //     profilesTable->setCellWidget(row, 4, actionWidget);




    //     row++;
    // }


    // Back Button
    QPushButton *profilesBackButton = new QPushButton("Back");
    connect(profilesBackButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(2); // Go back to Menu page
    });
    profilesLayout->addWidget(profilesBackButton);


    profilesPage->setLayout(profilesLayout);
    stackedWidget->addWidget(profilesPage);
}



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


void MainWindow::setupChartPage()
{
    // Create the page widget
    QWidget *chartWidget = new QWidget(this);
    QVBoxLayout *chartLayout = new QVBoxLayout(chartWidget);


    // Define the font style for the labels
    QFont labelFont5("Arial", 16, QFont::Bold);


    // Add the label
    QLabel *chartLabel = new QLabel("Chart Page");
    chartLabel->setFont(labelFont5);
    chartLabel->setAlignment(Qt::AlignCenter); // Center the text
    chartLayout->addWidget(chartLabel);


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
    QString username = userInput->text();
    QString password = pwInput->text();


    //!!!   check this
    // Comment out database check for now if needed
    // QSqlQuery query("SELECT * FROM Users WHERE username = :username AND password = :password");
    // query.bindValue(":username", username);
    // query.bindValue(":password", password);
    // if (query.exec() && query.next())
    // {
    //     // User found
    // }


    // Check if the entered credentials match
    if (userCredentials.contains(username) && userCredentials.value(username) == password)
    {
        // Set the user's name (for now, just using the username as a placeholder)
        userNameLabel->setText(username); // Replace with full name from a real database if available


        // Navigate to the Menu Page (index 2)
        stackedWidget->setCurrentIndex(2);
    }
    else
    {
        // Show a warning if the login fails
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
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
    QString firstName = fNameInput->text();
    QString lastName = lNameInput->text();
    QString weight = weightInput->text();
    QString height = heightInput->text();
    QString dob = dobInput->text();


    if (firstName.isEmpty() || lastName.isEmpty() || weight.isEmpty() ||
        height.isEmpty() || dob.isEmpty()) {
            QMessageBox::warning(this, "Save Failed", "Please fill out all fields.");
            return;
    }
    // Model attempts creating Profile, logs error if failure
    bool created = model.createProfile(firstName, lastName, weight.toFloat(), height.toFloat(), dob);

    if (!created) return; // Change to handle below:
    //    if (query.exec()) {
    //        QMessageBox::information(this, "Profile Saved", "Profile created successfully!");
    //        stackedWidget->setCurrentIndex(0); // Go back to login page
    //    } else {
    //        QMessageBox::critical(this, "Database Error", "Failed to save profile: " + query.lastError().text());
    //    }
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

    //includes calls to...
    curScanner->registerReading('l','h',0);
    //and...
    curScanner->registerReading('r','h',0);
    // but call each a total of 6 times (left and right), with 0 incrementing up by one for each call. -1 is the value, and should be replaced with a random.
}


void MainWindow::measureAnkle()
{
    // // Simulate ankle measurement logic
    // qDebug() << "Ankle measurement started.";
    // // You could similarly update the UI here
    // statusLabel->setText("Ankle measurement in progress...");

    //includes calls to...
    curScanner->registerReading('l','f',0);
    //and...
    curScanner->registerReading('r','f',0);
    // but call each a total of 6 times (left and right), with 0 incrementing up by one for each call. -1 is the value, and should be replaced with a random.

}


void MainWindow::finishMeasurement()
{
    if(curScanner->finishScan()){
        displayedSnap = curScanner->getFinishedSnap();
        delete curScanner;

        // snapshot can be shown from the curSnap, which has all the general params directly accesible, and the specific hand/leg readings in DB

        // // Optionally, show a confirmation to the user
        // statusLabel->setText("Measurement complete. Results saved.");


        // // Navigate to the History page (or you could show the results in a different view)
        // stackedWidget->setCurrentIndex(1);  // Assuming index 1 is the History page
    }
    else{
        // scenario where scan has incomplete params (or db saving fails, for some reason)

    }

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



void MainWindow::editProfile() //!!!check this
{
    QString fname, lname, bday = "";
    float height = -1.0f;
    float weight = -1.0f;

    //GATHER PARAMS FROM ENTRY BOXES HERE; if nothing keep at defaults to indicate no change

    if(model.editCurProfile(fname,lname,weight,height,bday)){
        // ON SUCCESS, REFRESH DISPLAY IF NEEDED
    }
    else{
        // ON FAILURE MESSAGE MAY BE DISPLAYED
    }



}


void MainWindow::deleteProfile() //!!!check this
{
    if(model.deleteCurrentProfile()){
        // ON SUCCESSFUL DELETION PLACE USER BACK IN SELECT/CREATE PROFILE MENU

    }
    else{
        // ON FAILURE MESSAGE MAY BE DISPLAYED
    }
}
