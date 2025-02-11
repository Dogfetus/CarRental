#include "Queries.h"
#include "Data/Json.h"
#include <iostream>

Queries::Queries(Data &data, ::Database &database) : data(data), database(database){

}


// Adds a car to the database
void Queries::AddCar(std::string &regNum, int &seat, std::string &colour, std::string &brand) {

    Car car;

    car.RegNum = regNum;
    car.Seat = seat;
    car.Colour = colour;
    car.Brand =  brand;
    database.replace(car);
}


// edits a specific car in the database
void Queries::EditCar(std::string &regNum, int &seat, std::string &colour, std::string &brand){

    Car car;

    car.RegNum = regNum;
    car.Seat = seat;
    car.Colour = colour;
    car.Brand = brand;

    database.update(car);

}


// removes a specific car from the database
void Queries::RemoveCar(std::string &regNum){
    database.remove<Car>(regNum);
}


// finds a specific car from the database
void Queries::SearchCar(std::string &regNum) {
    database.get_all<Car>(regNum);
}


// returns the total amount of cars currently in the database
int Queries::NumOfCars(){
    return database.count<Car>();
}


// Adds a customer to the database where id isn't necessary (Autoincrement)
int Queries::AddCustomer(int &phoneNum, std::string &name, std::string &email){

    Customer customer;

    customer.ID = -1;
    customer.Name = name;
    customer.Email = email;
    customer.PhoneNum = phoneNum;

    return database.insert(customer);
}


// Edits a specific customer in the database
void Queries::EditCustomer(int &customerId, int &phoneNum, std::string &name, std::string &email){

    Customer customer;

    customer.ID = customerId;
    customer.Name = name;
    customer.Email = email;
    customer.PhoneNum = phoneNum;

    database.update(customer);
}


// removes a specific customer from the database
void Queries::RemoveCustomer(int &customerId){
    database.remove<Customer>(customerId);

}


// Finds a specific customer from the database
void Queries::SearchCustomer(int &customerId){
    database.get_all<Customer>(customerId);

}


// returns total number of customers currently in the database
int Queries::NumOfCustomers() {
    return database.count<Customer>();
}


//Assigns car to customer (visa versa)
void Queries::AssignCar(int &customerId, std::string &regNum, std::string &time, std::string &endtime){

    Rental rental;
    rental.Customers_ID = customerId;
    rental.Time = time;
    rental.EndTime = endtime;
    rental.Cars_RegNum = regNum;

    database.replace(rental);
}


// Unassigns car from customer (visa versa)
void Queries::UnAssignCar(int &customerId, std::string &regNum) {
    database.remove<Rental>(customerId, regNum);
}


// returns the number of current rentals in the database
int Queries::NumOfRentals() {
    return database.count<Rental>();
}


// Updates all the vectors in the Data model so it matches with the database
void Queries::UpdateData(){
    data.Cars = database.get_all<Car>();
    data.Customers = database.get_all<Customer>();
    data.Rentals = database.get_all<Rental>();
}


// Inserts data from a .json file pathed by the string &json parameter
bool Queries::InsertDataFrom(const std::string &json) {
   if (!ImportFromJson(json, data))
       return false;

   for (auto &car : data.Cars)
       database.replace(car);

   for (auto &customer : data.Customers)
       database.replace(customer);

   for (auto &rental : data.Rentals)
       database.replace(rental);
    return true;
}


// Function to remove all infomration from the database
void Queries::RemoveAll() {
    database.remove_all<Rental>();
    database.remove_all<Car>();
    database.remove_all<Customer>();
}


// Exports all the data from the database to a json file pathed by the string &json parameter
bool Queries::ExportData(const std::string &json){
    UpdateData();
    return ExportToJson(json, data);
}


// Increments the number of finished Rentals (unnasigned) from the database
void Queries::IncrementRentals() {
    data.Finished_Rentals++;
}