#include <QtWidgets>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>


MainWindow::MainWindow(Queries &Query, const Data &data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Query(Query)
    , data(data)
{
    ui->setupUi(this);

    this->setFixedSize(800,570);

    QPixmap pix("GUI/Pictures/mema.jpg");
    QPixmap pix2("GUI/Pictures/cars.jpg");
    ui->label_picture->setPixmap(pix);
    ui->label2->setPixmap(pix2);

    ui->lineEdit_StartTimeAssign->setPlaceholderText("yyyy-mm-dd (Not needed when unassigning)");
    ui->lineEdit_EndTimeAssign->setPlaceholderText("yyyy-mm-dd (Not needed when unassigning)");
    ui->lineEdit_Path->setPlaceholderText("c:\\example\\example");
    ui->lineEdit_SearchCar->setPlaceholderText("Search for Car");
    ui->lineEdit_SearchCustomer->setPlaceholderText("Search for Customer");


    ui->label_CarCustomerStat->setAlignment(Qt::AlignCenter);
    ui->label_RentalStat->setAlignment(Qt::AlignCenter);
    QString Stats1 = "Cars: " + QString::number(Query.NumOfCars()) + "                                 Customers: " + QString::number(Query.NumOfCustomers());
    QString Stats2 = "Currently Rented: " + QString::number(Query.NumOfRentals()) + "         Completed Rentals: " + QString::number(data.Finished_Rentals);
    ui->label_CarCustomerStat->setText(Stats1);
    ui->label_RentalStat->setText(Stats2);


    Query.UpdateData();
    Update_All_Tables();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Home_clicked() {

    QString Stats1 = "Cars: " + QString::number(Query.NumOfCars()) + "                                 Customers: " + QString::number(Query.NumOfCustomers());
    QString Stats2 = "Currently Rented: " + QString::number(Query.NumOfRentals()) + "         Completed Rentals: " + QString::number(data.Finished_Rentals);
    ui->label_CarCustomerStat->setText(Stats1);
    ui->label_RentalStat->setText(Stats2);

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_4->setCurrentIndex(0);
}

// Buttons that swap between different "screens"
void MainWindow::on_pushButton_Car_clicked()            {   ui->stackedWidget->setCurrentIndex(2); Update_Car_table();      }
void MainWindow::on_pushButton_Customer_clicked()       {   ui->stackedWidget->setCurrentIndex(1); Update_Customer_table(); }
void MainWindow::on_pushButton_AssignCar_clicked()      {   ui->stackedWidget->setCurrentIndex(3); Update_Rental_table();   }
void MainWindow::on_pushButton_AddCustomer_clicked()    {   ui->stackedWidget_3->setCurrentIndex(0);                        }
void MainWindow::on_pushButton_EditCustomer_clicked()   {   ui->stackedWidget_3->setCurrentIndex(1);                        }
void MainWindow::on_pushButton_EditCar_clicked()        {   ui->stackedWidget_2->setCurrentIndex(1);                        }
void MainWindow::on_pushButton_AddCar_clicked()         {   ui->stackedWidget_2->setCurrentIndex(0);                        }
void MainWindow::on_pushButton_DeleteCar_clicked()      {   ui->stackedWidget_2->setCurrentIndex(2);                        }
void MainWindow::on_pushButton_DeleteCustomer_clicked() {   ui->stackedWidget_3->setCurrentIndex(2);                        }
void MainWindow::on_pushButton_AssignUnassign_clicked() {   ui->stackedWidget_4->setCurrentIndex(1);                        }



/** This fucntion for the search bar check every variable of every instances of the customer classes,
 * to see if any data matches what is passed in. This can display more than one customer,
 * if a non-key value is search for.
 * */
void MainWindow::on_pushButton_SearchCustomer_clicked() {
    std::string search = ui->lineEdit_SearchCustomer->text().toStdString();

    //clears the table so only the customers searched for, shows up
    ui->textEdit->clear();

    for (auto &customer : data.Customers) {
        if (std::atoi(search.c_str()) == customer.ID                  ||
            strstr(customer.Name.c_str(), search.c_str())  ||
            strstr(customer.Email.c_str(), search.c_str()) ||
            std::atoi(search.c_str()) == customer.PhoneNum)
        {
            Insert_Customer(customer);
        }
    }
}


// Bottom Add button at customer "screen"
void MainWindow::on_pushButton_AddCustomerAdd_clicked() {

    // checks if any of the textboxes has been left empty
    if (ui->lineEdit_NameAdd->text().isEmpty()  ||
        ui->lineEdit_EmailAdd->text().isEmpty() ||
        ui->lineEdit_PhoneNumAdd->text().isEmpty()){

        // Displais an error window if a textbox is empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {

        std::string name = ui->lineEdit_NameAdd->text().toStdString();
        std::string email = ui->lineEdit_EmailAdd->text().toStdString();
        int phoneNum = ui->lineEdit_PhoneNumAdd->text().toInt();

        // Adds the customer to the database
        Query.AddCustomer(phoneNum, name, email);

        // updates the data model
        Query.UpdateData();

        // displais the updated data at the gui table
        Update_Customer_table();

        //clears the textboxes
        ui->lineEdit_NameAdd->clear();
        ui->lineEdit_EmailAdd->clear();
        ui->lineEdit_PhoneNumAdd->clear();
    }
}


// Bottom Delete button at customer "screen"
void MainWindow::on_pushButton_DeleteCustomerDelete_clicked() {

    // checks if any of the textboxes has been left empty
    if (ui->lineEdit_CustomeridDelete->text().isEmpty()){

        // Displais an error window if a textbox is empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {
        int customerId = ui->lineEdit_CustomeridDelete->text().toInt();

        // Tries to remove a customer from the database,
        // if it fails (foreign key used in another table) then an error box pops up
        try {
            Query.RemoveCustomer(customerId);
        } catch (std::exception &error){
            QMessageBox::critical(nullptr, error.what(), "Make sure to unassign the customer first! ");
        }

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to display the data from the model
        Update_Customer_table();

        //clears the textbox
        ui->lineEdit_CustomeridDelete->clear();
    }
}


// Bottom Edit button at customer "screen"
void MainWindow::on_pushButton_EditCustomerEdit_clicked() {

    // checks if any of the textboxes has been left empty
    if (ui->lineEdit_NameEdit->text().isEmpty()     ||
        ui->lineEdit_EmailEdit->text().isEmpty()    ||
        ui->lineEdit_PhoneNumEdit->text().isEmpty() ||
        ui->lineEdit_CustomeridEdit->text().isEmpty())
    {
        // Creates an error box if textboxes are left empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {

        std::string name = ui->lineEdit_NameEdit->text().toStdString();
        std::string email = ui->lineEdit_EmailEdit->text().toStdString();
        int phoneNum = ui->lineEdit_PhoneNumEdit->text().toInt();
        int id = ui->lineEdit_CustomeridEdit->text().toInt();

        // Edits an existing customer
        Query.EditCustomer(id, phoneNum, name, email);

        // updates the model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Customer_table();

        // clears the textboxes
        ui->lineEdit_NameEdit->clear();
        ui->lineEdit_EmailEdit->clear();
        ui->lineEdit_PhoneNumEdit->clear();
        ui->lineEdit_CustomeridEdit->clear();
    }
}


// Bottom Assign button at Rental "screen"
void MainWindow::on_pushButton_Assign_clicked() {

    // checks if any of the textboxes has been left empty
    if (ui->lineEdit_CarRegNumAssign->text().isEmpty()  ||
        ui->lineEdit_StartTimeAssign->text().isEmpty()  ||
        ui->lineEdit_EndTimeAssign->text().isEmpty()    ||
        ui->lineEdit_CustomeridAssign->text().isEmpty())
    {
        // creates an error box if any boxes are left empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {

        std::string regNum = ui->lineEdit_CarRegNumAssign->text().toStdString();
        std::string time = ui->lineEdit_StartTimeAssign->text().toStdString();
        std::string endtime = ui->lineEdit_EndTimeAssign->text().toStdString();
        int customerId = ui->lineEdit_CustomeridAssign->text().toInt();

        // tries to assign a car to a customer (visa versa)
        // If it fails it creates an error box displaying the error, and whats wrong
        try {
            Query.AssignCar(customerId, regNum, time, endtime);
        } catch (std::exception &error){
            QMessageBox::critical(nullptr, error.what(), "You tried to assign a non-existing member!");
        }

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Rental_table();

        // clears the textboxes
        ui->lineEdit_CarRegNumAssign->clear();
        ui->lineEdit_StartTimeAssign->clear();
        ui->lineEdit_EndTimeAssign->clear();
        ui->lineEdit_CustomeridAssign->clear();
    }
}


// Bottom Unassign button at rental "screen"
void MainWindow::on_pushButton_Unassign_clicked(){

    // checks if any of the textboxes has been left empty
    if (ui->lineEdit_CarRegNumAssign->text().isEmpty() ||
        ui->lineEdit_CustomeridAssign->text().isEmpty())
    {
        // Creats an error box if textboxes are left empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {

        std::string regNum = ui->lineEdit_CarRegNumAssign->text().toStdString();
        int customerId = ui->lineEdit_CustomeridAssign->text().toInt();

        // unnassigns an existing assigned car/customer
        Query.UnAssignCar(customerId, regNum);

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Rental_table();

        // increments the number of finished rentals (for statistics)
        Query.IncrementRentals();

        //clears the textboxes
        ui->lineEdit_CarRegNumAssign->clear();
        ui->lineEdit_StartTimeAssign->clear();
        ui->lineEdit_EndTimeAssign->clear();
        ui->lineEdit_CustomeridAssign->clear();
    }
}


// Import button at home -> import/export "screen"
void MainWindow::on_pushButton_import_clicked() {
    // checks if any of the textboxe has been left empty
    // if its empty an error box shows up
    if (ui->lineEdit_Path->text().isEmpty()){
        QMessageBox::critical(nullptr, "Error", "No path was provided!");
    } else {
        std::string Path = ui->lineEdit_Path->text().toStdString();

        // tells the controller to import json from the given path (Path)
        // if program failed to open the file a wild error box will appear
        if (!Query.InsertDataFrom(Path))
            QMessageBox::critical(nullptr, "Error", "Failed to open a .josn file");

        else{

            // Updates the data model to match the database
            Query.UpdateData();

            // updates the gui table to match the data model
            Update_All_Tables();

            // clears the textbox
            ui->lineEdit_Path->clear();
        }
    }
}


// Export button at home -> import/export "screen"
void MainWindow::on_pushButton_Export_clicked(){
    // checks if any of the textboxe has been left empty
    // if not an error box will be displaied
    if (ui->lineEdit_Path->text().isEmpty()){
        QMessageBox::critical(nullptr, "Error", "No path was provided!");
    } else {
        std::string Path = ui->lineEdit_Path->text().toStdString();

        // exports data from database to a .json file provided by the Path string
        Query.ExportData(Path);

        //clears the textbox
        ui->lineEdit_Path->clear();
    }
}


/** This fucntion for the search bar check every variable of every instances of the car classes,
 * to see if any data matches what is passed in. This can display more than one car,
 * if a non-key value is search for.
 * */
void MainWindow::on_pushButton_SearchCar_clicked() {
    std::string search = ui->lineEdit_SearchCar->text().toStdString();
    ui->textEdit_2->clear();

    for (auto &car : data.Cars) {
        if (strstr(car.RegNum.c_str(), search.c_str())  ||
            strstr(car.Colour.c_str(), search.c_str())  ||
            strstr(car.Brand.c_str(), search.c_str())   ||
            std::atoi(search.c_str()) == car.Seat)
        {
            Insert_Car(car);
        }
    }
}


// Bottom add button at car "screen"
void MainWindow::on_pushButton_AddCarAdd_clicked()
{   // checks if any of the textboxes has been left empty
    if (ui->lineEdit_CarRegNumAdd->text().isEmpty() ||
        ui->lineEdit_BrandAdd->text().isEmpty()     ||
        ui->lineEdit_ColourAdd->text().isEmpty()    ||
        ui->spinBox_SeatsAdd->text().isEmpty()){

        // An error box appears if boxes are empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {

        std::string regNum = ui->lineEdit_CarRegNumAdd->text().toStdString();
        std::string brand = ui->lineEdit_BrandAdd->text().toStdString();
        std::string colour = ui->lineEdit_ColourAdd->text().toStdString();
        int seat = ui->spinBox_SeatsAdd->text().toInt();

        // Adds the car to the database
        Query.AddCar(regNum, seat, colour, brand);

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Car_table();

        // clears the textboxes
        ui->lineEdit_CarRegNumAdd->clear();
        ui->lineEdit_BrandAdd->clear();
        ui->lineEdit_ColourAdd->clear();
        ui->spinBox_SeatsAdd->clear();
    }
}


// Bottom delete button at car "screen"
void MainWindow::on_pushButton_DeleteCarDelete_clicked(){
    // checks if any of the textbox has been left empty
    if (ui->lineEdit_CarRegNumDelete->text().isEmpty()){

        // shows error box if textbox is empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {
        std::string regNum = ui->lineEdit_CarRegNumDelete->text().toStdString();

        // tries to remove a Car from the database
        // failes if its key is used in another table as a foreign key
        try {
            Query.RemoveCar(regNum);

        // creates a worning box displaying the error
        } catch (std::exception &error){
            QMessageBox::critical(nullptr, error.what(), "Make sure to unassign the Car first!");
        }

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Car_table();

        // clears the textbox
        ui->lineEdit_CarRegNumDelete->clear();
    }
}

// Bottom edit button at car "screen"
void MainWindow::on_pushButton_EditCarEdit_clicked(){
    // checks if any of the textboxe has been left empty
    if (ui->lineEdit_CarRegNumEdit->text().isEmpty()||
        ui->lineEdit_BrandEdit->text().isEmpty()    ||
        ui->lineEdit_ColourEdit->text().isEmpty()   ||
        ui->spinBox_SeatsEdit->text().isEmpty())
    {
        // Error message pops up if textboxes are empty
        QMessageBox::critical(nullptr, "Error", "Missing inputs!");
    } else {
        std::string regNum = ui->lineEdit_CarRegNumEdit->text().toStdString();
        std::string brand = ui->lineEdit_BrandEdit->text().toStdString();
        std::string colour = ui->lineEdit_ColourEdit->text().toStdString();
        int seat = ui->spinBox_SeatsEdit->text().toInt();

        //Edits an existing car
        Query.EditCar(regNum, seat, colour, brand);

        // updates the data model to match the database
        Query.UpdateData();

        // updates the gui table to match the data model
        Update_Car_table();

        // clears the textboxes
        ui->lineEdit_CarRegNumEdit->clear();
        ui->lineEdit_BrandEdit->clear();
        ui->lineEdit_ColourEdit->clear();
        ui->spinBox_SeatsEdit->clear();
    }
}


// Updates all the tables to match the data model
void MainWindow::Update_All_Tables(){
    Update_Rental_table();
    Update_Customer_table();
    Update_Car_table();
}


// updates just the customer to match the data model
void MainWindow::Update_Customer_table(){
    ui->textEdit->clear();
    for (auto &customer : data.Customers) {
        std::string temp = "ID: " + std::to_string(customer.ID) +
                           ", Name: " + customer.Name +
                           ", Email: " + customer.Email +
                           ", Phonenumber: " + std::to_string(customer.PhoneNum);
        ui->textEdit->append(temp.c_str());
    }
}


// updates just the customer to match the data model
void MainWindow::Update_Car_table(){
    ui->textEdit_2->clear();
    for (auto &car : data.Cars) {
        std::string temp = "RegNum: " + car.RegNum +
                           ", Brand: " + car.Brand +
                           ", Number Of Seats: " + std::to_string(car.Seat) +
                           ", Colour: " + car.Colour;
        ui->textEdit_2->append(temp.c_str());
    }
}


// updates just the customer to match the data model
void MainWindow::Update_Rental_table(){
    ui->textEdit_3->clear();
    for (auto &rental : data.Rentals) {
        std::string temp = "RegNum: " + rental.Cars_RegNum +
                           ", CustomerId: " + std::to_string(rental.Customers_ID) +
                           ", Start Time " + rental.Time +
                           ", End Time: " + rental.EndTime;
        ui->textEdit_3->append(temp.c_str());
    }
}


// inserts a specific customer to the gui table
void MainWindow::Insert_Customer(const Customer &iterator) {
    std::string temp = "ID: " + std::to_string(iterator.ID) +
                       ", Name: " + iterator.Name +
                       ", Email: " + iterator.Email +
                       ", Phonenumber: " + std::to_string(iterator.PhoneNum);
    ui->textEdit->append(temp.c_str());
}


// inserts a specific car to the gui table
void MainWindow::Insert_Car(const Car &iterator) {
    std::string temp = "RegNum: " + iterator.RegNum +
                       ", Brand: " + iterator.Brand +
                       ", Seat: " + std::to_string(iterator.Seat) +
                       ", Colour: " + iterator.Colour;
    ui->textEdit_2->append(temp.c_str());
}