#ifndef PROJECT_JSON_H
#define PROJECT_JSON_H

// Exports the already existing data from the model to a .json file
bool ExportToJson(const std::string &json, Data &data);

// Imports json formatted data and adds it to the already existing data in the model
bool ImportFromJson(const std::string &json, Data &data);

#endif //PROJECT_JSON_H
