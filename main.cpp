#include <iostream>
#include <mysql.h>
#include <string>

using namespace std;
void doctor_management(MYSQL *conn);

void add_doctors(MYSQL *conn);
void view_doctors(MYSQL *conn);
void delete_doctors(MYSQL *conn);

void patient_management(MYSQL *conn);

int main()
{
    MYSQL *conn;
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "localhost", "root", "MySql777", "Hospital_management_db", 3306, NULL, 0);

    if (conn)
    {
        cout << "\n========================================\n";
        cout << "  Database Connected Successfully!\n";
        cout << "========================================\n"
             << endl;
    }
    else
    {
        cout << "Database Connection Failed: " << mysql_error(conn) << endl;
        return 0;
    }

    int choice;
    do
    {
        cout << "\nHospital Management System:\n";
        cout << "1. Doctor Management\n";
        cout << "2. Patient Management\n";
        cout << "3. Appointment Management\n";
        cout << "4. Medicine Management\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            // cout << "\n[Doctor Management Module - Under Development]\n";
            doctor_management(conn); // sub menu
            break;
        case 2:
            // cout << "\n[Patient Management Module - Under Development]\n";
            patient_management(conn);
            break;
        case 3:
            cout << "\n[Appointment Management Module - Under Development]\n";
            break;
        case 4:
            cout << "\n[Medicine Management Module - Under Development]\n";
            break;
        case 5:
            cout << "\nExiting system... Thank you!\n";
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 5);

    mysql_close(conn);
    return 0;
}

void doctor_management(MYSQL *conn)
{
    int choice;
    do
    {
        cout << "doctor management menu : \n";
        cout << "1. Add doctor: " << endl;
        cout << "2. view all doctors:" << endl;
        cout << "3. delete doctor:" << endl;
        cout << "4. back to main menu: " << endl;

        cout << "enter your choice: " << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            add_doctors(conn);
            break;
        case 2:
            view_doctors(conn);
            break;
        case 3:
            delete_doctors(conn);
            break;
        case 4:
            cout << "retruning to main menu: " << endl;
            break;

        default:
            break;
        }
    } while (choice != 4);
}

// add doctor function //
void add_doctors(MYSQL *conn)
{
    string name, spec, phone;
    cout << "add new doctor: " << endl;
    cout << "enter doctor name: " << endl;
    cin.ignore();
    getline(cin, name);
    cout << "enter doctor Specialization:" << endl;
    getline(cin, spec);
    cout << "enter doctor phone: " << endl;
    getline(cin, phone);
    string query = "insert into Doctors (Name , Specialization, Phone) values ('" + name + "', '" + spec + "', '" + phone + "')";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Doctor added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Doctor: "
             << mysql_error(conn) << endl;
    }
}

// View Doctors Function (SELECT query)
void view_doctors(MYSQL *conn)
{
    string query = "select * from Doctors";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n=====================================================\n";
        cout << "                    DOCTOR LIST                      \n";
        cout << "=====================================================\n";
        cout << "ID\t | Name\t\t | Specialization\t | Phone\n";
        cout << "-----------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << row[0] << "\t | " << row[1] << "\t | " << row[2] << "\t | " << row[3] << endl;
        }
        cout << "=====================================================\n";
        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Failed to fetch doctors: " << mysql_error(conn) << endl;
    }
}

void delete_doctors(MYSQL *conn)
{
    cout << "delete doctor : " << endl;
    int id;
    cout << "enter doctor id you want to delete:" << endl;
    cin >> id;
    string query = "delete from Doctors where id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Doctor Deleted Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Delete Doctor: " << mysql_error(conn) << endl;
    }
}

// Patient management function
void add_patient(MYSQL *conn);
void view_patient(MYSQL *conn);
void delete_patient(MYSQL *conn);
void patient_management(MYSQL *conn)
{
    int choice;
    do
    {
        cout << "Patient management system: " << endl;
        cout << "1. Add patient : " << endl;
        cout << "2. view patient details: " << endl;
        cout << "3 Delete patient details:" << endl;
        cout << "4. returning to main menu: " << endl;
        cout << "enter your choice: " << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            add_patient(conn);
            break;
        case 2:
            view_patient(conn);
            break;
        case 3:
            delete_patient(conn);
            break;
        case 4:
            cout << "Returning to main menu : " << endl;
            break;
        default:
            break;
        }
    } while (choice != 4);
}

void add_patient(MYSQL *conn)
{
    string name, gender, phone;
    int age;
    cout << "add patient details: " << endl;
    cin.ignore();
    cout << "add name : " << endl;
    getline(cin, name);
    cout << "Enter patient age : " << endl;
    // getline(cin, age);
    cin >> age;
    cin.ignore();
    cout << "add patient gender: " << endl;
    getline(cin, gender);
    cout << "enter patient phone" << endl;
    getline(cin, phone);

    string query = "insert into Patients (name, Gender , Phone , Age) values ('" + name + "', '" + gender + "', '" + phone + "', " + to_string(age) + " )";

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] patient added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add patient: "
             << mysql_error(conn) << endl;
    }
}

void view_patient(MYSQL *conn)
{
    string query = "select * from Patients";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res == nullptr)
        {
            cout << "\n[Error] " << mysql_error(conn) << endl;
            return;
        }

        MYSQL_ROW row;

        cout << "\n============================================================\n";
        cout << "                       PATIENT LIST                         \n";
        cout << "============================================================\n";
        cout << "ID\t | Name\t\t | Age\t | Gender\t | Phone\n";
        cout << "------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << (row[0] ? row[0] : "NULL") << "\t | "
                 << (row[1] ? row[1] : "NULL") << "\t | "
                 << (row[2] ? row[2] : "NULL") << "\t | "
                 << (row[3] ? row[3] : "NULL") << "\t | "
                 << (row[4] ? row[4] : "NULL") << endl;
        }

        cout << "============================================================\n";

        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Failed to fetch patients: "
             << mysql_error(conn) << endl;
    }
}

void delete_patient(MYSQL *conn)
{
    cout << "delete patient details: " << endl;
    int id;
    cout << " enter patient id" << endl;
    cin >> id;
    string query = "delete from Patients where id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] patient Deleted Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Delete patient: " << mysql_error(conn) << endl;
    }
}