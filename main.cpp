#include "GUI/mainwindow.h"
#include <QApplication>
#include "Database/Database.h"
#include "Queries/Queries.h"
#include "rental/Data.h"


int main(int argc, char *argv[]){

    auto database = init_Database("Database/Database.sqlite");

    // Model
    Data data;

    /**Query works as the controller for all datamanagement to the database.
     * This means that the GUI can just edit or delete data from the database through this controller.
     * Otherwhise reading is no problemo for the GUI.
     * */
    Queries Query(data, database);

    // GUI (View)
    QApplication a(argc, argv);
    MainWindow w(Query, data);
    w.show();


    return a.exec();
}