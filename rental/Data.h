#ifndef PROJECT_DATA_H
#define PROJECT_DATA_H
#include <vector>
#include "rental/Car.h"
#include "rental/Customer.h"
#include "rental/Rental.h"


class Data {

public:
    std::vector<Car> Cars;
    std::vector<Customer> Customers;
    std::vector<Rental> Rentals;

    // total unfinished rentals (starts at 0)
    int Finished_Rentals = 0;

};


#endif //PROJECT_DATA_H
