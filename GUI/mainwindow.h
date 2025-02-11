
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Queries/Queries.h"
#include "rental/Data.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(Queries &Query, const Data &data, QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    // The 4 buttons at the left side, used to change between "screens" + import/export button
    void on_pushButton_Home_clicked();
    void on_pushButton_Car_clicked();
    void on_pushButton_Customer_clicked();
    void on_pushButton_AssignCar_clicked();
    void on_pushButton_AssignUnassign_clicked();



    // The search bars at the top of the cars and customers tables
    void on_pushButton_SearchCar_clicked();
    void on_pushButton_SearchCustomer_clicked();



    // Buttons used for car modification
    //Top 3:
    void on_pushButton_EditCar_clicked();
    void on_pushButton_AddCar_clicked();
    void on_pushButton_DeleteCar_clicked();
    //Bottom 3:
    void on_pushButton_AddCarAdd_clicked();
    void on_pushButton_DeleteCarDelete_clicked();
    void on_pushButton_EditCarEdit_clicked();



    // Buttons used for customer modification
    //Top 3:
    void on_pushButton_AddCustomer_clicked();
    void on_pushButton_EditCustomer_clicked();
    void on_pushButton_DeleteCustomer_clicked();
    //Bottom 3:
    void on_pushButton_AddCustomerAdd_clicked();
    void on_pushButton_DeleteCustomerDelete_clicked();
    void on_pushButton_EditCustomerEdit_clicked();



    // buttons used to assign / unassign cars to customer (visa versa)
    void on_pushButton_Assign_clicked();
    void on_pushButton_Unassign_clicked();



    // buttons to import/export data
    void on_pushButton_import_clicked();
    void on_pushButton_Export_clicked();



    // Additional function used to update the tables displaying the data retreived from the model
    void Update_Car_table();
    void Update_Customer_table();
    void Update_Rental_table();
    void Update_All_Tables();
    void Insert_Customer(const Customer &iterator);
    void Insert_Car(const Car &iterator);


private:
    Ui::MainWindow *ui;

    //controller
    Queries &Query;

    //model
    const Data &data;
};

#endif // MAINWINDOW_H
