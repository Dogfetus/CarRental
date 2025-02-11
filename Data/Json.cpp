#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/filewritestream.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include "rental/Data.h"
#include "Json.h"
#include <iostream>


#define size_of_buffer 65536


bool ImportFromJson(const std::string &json, Data &data){
    using namespace rapidjson;

    // Checks if path input provided a .json file
    if (!(*(json.end()-1) == 'n' &&
          *(json.end()-2) == 'o' &&
          *(json.end()-3) == 's' &&
          *(json.end()-4) == 'j' &&
          *(json.end()-5) == '.'))
    {
        std::cout << "No .json file provided" << std::endl;
        return false;
    }

    // open the file to read "r"
    FILE *f = fopen(json.c_str(), "r");
    if (!f){
        std::cout << "Could not open the file" << std::endl;
        return false;
    }

    // Parses the json data into readable (usable) data
    char Json_Buffer[size_of_buffer] = {0};
    FileReadStream stream(f, Json_Buffer, sizeof(Json_Buffer));
    Document doc;
    doc.ParseStream(stream);
    fclose(f);

    // Creates a instance of a car thne adds all the values before adding it to the model
    for (int i = 0; i < doc[0]["Car"].Size(); i++){
        Car car;
        car.RegNum = doc[0]["Car"][i]["RegNum"].GetString();
        car.Brand = doc[0]["Car"][i]["Brand"].GetString();
        car.Seat = doc[0]["Car"][i]["Seat"].GetInt();
        car.Colour = doc[0]["Car"][i]["Colour"].GetString();
        data.Cars.push_back(car);
    }

    // Creates a instance of a customer thne adds all the values before adding it to the model
    for (int i = 0; i < doc[1]["Customer"].Size(); i++){
        Customer Customer;
        Customer.ID = doc[1]["Customer"][i]["ID"].GetInt();
        Customer.Name = doc[1]["Customer"][i]["Name"].GetString();
        Customer.PhoneNum = doc[1]["Customer"][i]["PhoneNum"].GetInt();
        Customer.Email = doc[1]["Customer"][i]["Email"].GetString();
        data.Customers.push_back(Customer);
    }

    // Creates a instance of a rental thne adds all the values before adding it to the model
    for (int i = 0; i < doc[2]["Rental"].Size(); i++){
        Rental rental;
        rental.Cars_RegNum = doc[2]["Rental"][i]["RegNum"].GetString();
        rental.Customers_ID = doc[2]["Rental"][i]["Customer_id"].GetInt();
        rental.Time = doc[2]["Rental"][i]["Time"].GetString();
        rental.EndTime = doc[2]["Rental"][i]["EndTime"].GetString();
        data.Rentals.push_back(rental);
    }

    return true;
}




bool ExportToJson(const std::string &json, Data &data){
    using namespace rapidjson;

    // creates a buffer for us to use when we create our json formatted text
    StringBuffer JsonBuffer;
    Writer<StringBuffer> JsonBuilder(JsonBuffer);

    // Json creation starts here:
    // Create the array
    JsonBuilder.StartArray();

    // Create the first object (Car)
    JsonBuilder.StartObject();
    JsonBuilder.Key("Car");

    // The Car object holds an array of all the car details and values of each car:
    JsonBuilder.StartArray();
    for (auto &car : data.Cars) {
        JsonBuilder.StartObject();
        JsonBuilder.Key("RegNum");
        JsonBuilder.String(car.RegNum.c_str());
        JsonBuilder.Key("Brand");
        JsonBuilder.String(car.Brand.c_str());
        JsonBuilder.Key("Seat");
        JsonBuilder.Uint(car.Seat);
        JsonBuilder.Key("Colour");
        JsonBuilder.String(car.Colour.c_str());
        JsonBuilder.EndObject();
    }
    JsonBuilder.EndArray();
    JsonBuilder.EndObject();

    // Create the second object (Customer)
    JsonBuilder.StartObject();
    JsonBuilder.Key("Customer");

    // The customer holds an array of all the details and values of each customer:
    JsonBuilder.StartArray();
    for (auto &customer : data.Customers) {
        JsonBuilder.StartObject();
        JsonBuilder.Key("ID");
        JsonBuilder.Uint(customer.ID);
        JsonBuilder.Key("PhoneNum");
        JsonBuilder.Uint(customer.PhoneNum);
        JsonBuilder.Key("Name");
        JsonBuilder.String(customer.Name.c_str());
        JsonBuilder.Key("Email");
        JsonBuilder.String(customer.Email.c_str());
        JsonBuilder.EndObject();
    }
    JsonBuilder.EndArray();
    JsonBuilder.EndObject();

    // Create the third object (Rental)
    JsonBuilder.StartObject();
    JsonBuilder.Key("Rental");

    // The Rental holds an array of all the details and values of each customer:
    JsonBuilder.StartArray();
    for (auto &rental : data.Rentals) {
        JsonBuilder.StartObject();
        JsonBuilder.Key("RegNum");
        JsonBuilder.String(rental.Cars_RegNum.c_str());
        JsonBuilder.Key("Customer_id");
        JsonBuilder.Uint(rental.Customers_ID);
        JsonBuilder.Key("Time");
        JsonBuilder.String(rental.Time.c_str());
        JsonBuilder.Key("EndTime");
        JsonBuilder.String(rental.EndTime.c_str());
        JsonBuilder.EndObject();
    }
    JsonBuilder.EndArray();
    JsonBuilder.EndObject();

    // fianlly ends the array after all the objects
    JsonBuilder.EndArray();


    Document doc;
    doc.Parse(JsonBuffer.GetString());
    FILE *f;

    // Checls if provided path ended in a .json (if not, the .json will be added "automatically")
    if (*(json.end()-1) == 'n' &&
        *(json.end()-2) == 'o' &&
        *(json.end()-3) == 's' &&
        *(json.end()-4) == 'j' &&
        *(json.end()-5) == '.')
    {
        f = fopen(json.c_str(), "w");
    }

    // Creates the .json at the end of the file if the specified path does not include the .json at the end
    else
        f = fopen((json + ".json").c_str(), "w");

    // returns false if file is not found/cant be opened
    if (!f){
        std::cout << "Could not open the file" << std::endl;
        return false;
    }

    // Writes the json data to the opened file.
    char Json_Buffer[size_of_buffer] = {0};
    FileWriteStream FWS(f, Json_Buffer, sizeof(Json_Buffer));
    Writer<FileWriteStream> WriteStream (FWS);
    doc.Accept(WriteStream);
    fclose(f);

    return true;
}