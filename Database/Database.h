#ifndef PROJECT_DATABASE_H
#define PROJECT_DATABASE_H

#include <../sqlite_orm/sqlite_orm.h>
#include "../rental/Car.h"
#include "../rental/Rental.h"
#include "../rental/Customer.h"

inline auto init_Database(const std::string &filename) {

    using namespace sqlite_orm;
    auto database = make_storage(filename,
                                         // Car table
                                         make_table("Car",
                                                    make_column("RegNum", &Car::RegNum, primary_key()),
                                                    make_column("Brand", &Car::Brand),
                                                    make_column("Seat", &Car::Seat),
                                                    make_column("Colour", &Car::Colour)
                                         ),

                                         // Customer table
                                         make_table("Customer",
                                                    make_column("CustomerId", &Customer::ID, autoincrement(), primary_key()),
                                                    make_column("Name", &Customer::Name),
                                                    make_column("PhoneNum", &Customer::PhoneNum),
                                                    make_column("Email", &Customer::Email)
                                         ),

                                         // Rental table
                                         make_table("Rental",
                                                    make_column("Time", &Rental::Time),
                                                    make_column("RegNum", &Rental::Cars_RegNum),
                                                    make_column("Customer_id", &Rental::Customers_ID),
                                                    make_column("EndTime", &Rental::EndTime),
                                                    primary_key(&Rental::Customers_ID, &Rental::Cars_RegNum),
                                                    foreign_key(&Rental::Cars_RegNum).references(&Car::RegNum),
                                                    foreign_key(&Rental::Customers_ID).references(&Customer::ID)
                                         )
    );

    // Create the databse if it does not already exist
    database.sync_schema();
    return database;
}

using Database = decltype(init_Database(""));

#endif //PROJECT_DATABASE_H
