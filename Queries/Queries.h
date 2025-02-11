#ifndef PROJECT_QUERIES_H
#define PROJECT_QUERIES_H
#include "rental/Data.h"
#include "../Database/Database.h"


class Queries {
protected:
    Data &data;
    Database &database;

public:
    Queries(Data &data, Database &database);

    // Cars:
    void AddCar(std::string &regNum, int &seat, std::string &colour, std::string &brand);
    void EditCar(std::string &regNum, int &seat, std::string &colour, std::string &brand);
    void RemoveCar(std::string &regNum);
    void SearchCar(std::string &regNum);
    int NumOfCars();


    // Customers:
    int AddCustomer(int &phoneNum, std::string &name, std::string &email);
    void EditCustomer(int &customerId, int &phoneNum, std::string &name, std::string &email);
    void RemoveCustomer(int &customerId);
    void SearchCustomer(int &customerId);
    int NumOfCustomers();


    // Rentals:
    void AssignCar( int &customerId, std::string &regNum, std::string &time, std::string &endtime);
    void UnAssignCar(int &customerId, std::string &regNum);
    int NumOfRentals();


    //data:
    void UpdateData();
    bool InsertDataFrom(const std::string &json);
    void RemoveAll();
    bool ExportData(const std::string &json);
    void IncrementRentals();

};

#endif //PROJECT_QUERIES_H
