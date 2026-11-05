#include <iostream>
#include <mysql.h>
#include <string>
#include <limits>
#include <cstring>
#include <limits>
#include <sstream>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <cctype>

using namespace std;

void doctor_management(MYSQL *conn);
// --- Doctors Management Function Prototypes ---
void add_doctors(MYSQL *conn);
void view_doctors(MYSQL *conn);
void update_doctor(MYSQL *conn);
void delete_doctors(MYSQL *conn);
void search_doctor(MYSQL *conn);

void patient_management(MYSQL *conn);
void appointment_management(MYSQL *conn);
void medicine_management(MYSQL *conn);

void dashboard(MYSQL *conn); // dashboard function prototypes

void medical_record_menu(MYSQL *conn);
void dashboard(MYSQL *conn) // query function
{
    string query = "SELECT (SELECT COUNT(*) FROM Doctors) AS Total_Doctors, (SELECT COUNT(*) FROM Patients) AS Total_Patients, (SELECT COUNT(*) FROM Appointments) AS Total_Appointments, (SELECT COUNT(*) FROM Medicine) AS Total_Medicines, (SELECT COALESCE(SUM(Stock_quantity), 0) FROM Medicine) AS Total_Stock, (SELECT COUNT(*) FROM Medicine WHERE Stock_quantity <= 10) AS Low_Stock_Medicines";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res != NULL)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            cout << "\n===== HOSPITAL DASHBOARD =====\n";
            cout << "Total Doctors: " << row[0] << endl;
            cout << "Total Patients: " << row[1] << endl;
            cout << "Total Appointments: " << row[2] << endl;
            cout << "Total Medicines: " << row[3] << endl;
            cout << "Total Medicine Stock: " << row[4] << endl;
            cout << "Low Stock Medicines (10 or less): " << row[5] << endl;
            mysql_free_result(res);
        }
    }
    else
    {
        cout << "Dashboard query failed: " << mysql_error(conn) << endl;
    }
}

int main()
{
    MYSQL *conn;
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "localhost", "root", getenv("DB_PASSWORD"), "Hospital_management_db", 3306, NULL, 0);

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
        cout << "1. Dashboard\n";
        cout << "2. Doctor Management\n";
        cout << "3. Patient Management\n";
        cout << "4. Appointment Management\n";
        cout << "5. Medicine Management\n";
        cout << "6. Medical Records Menu\n";
        cout << "7. Exit\n";
        // cout << "Enter your choice: ";
        // cin >> choice;
        cout << "Enter your choice: ";
        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            dashboard(conn);
            break;
        case 2:
            // cout << "\n[Doctor Management Module - Under Development]\n";
            doctor_management(conn); // sub menu
            break;
        case 3:
            // cout << "\n[Patient Management Module - Under Development]\n";
            patient_management(conn);
            break;
        case 4:
            // cout << "\n[Appointment Management Module - Under Development]\n";
            appointment_management(conn);
            break;
        case 5:
            // cout << "\n[Medicine Management Module - Under Development]\n";
            medicine_management(conn);
            break;
        case 6:
            medical_record_menu(conn);
            break;
        case 7:
            cout << "\nExiting system... Thank you!\n";
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 7);

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
        cout << "3. update doctor info: " << endl;
        cout << "4. delete doctor:" << endl;
        cout << "5. search doctotr : " << endl;
        cout << "6. back to main menu: " << endl;

        cout << "enter your choice: " << endl;
        // cin >> choice;
        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice)
        {
        case 1:
            add_doctors(conn);
            break;
        case 2:
            view_doctors(conn);
            break;
        case 3:
            update_doctor(conn);
            break;
        case 4:
            delete_doctors(conn);
            break;
        case 5:
            search_doctor(conn);
            break;
        case 6:
            cout << "retruning to main menu: " << endl;
            break;

        default:
            cout << "invalid input: " << endl;
            break;
        }
    } while (choice != 6);
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

    string query = "INSERT INTO Doctors (Name, Specialization, Phone) VALUES (?, ?, ?)";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = name.length();
    unsigned long spec_length = spec.length();
    unsigned long phone_length = phone.length();

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = const_cast<char *>(name.c_str());
    bind[0].buffer_length = name_length;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = const_cast<char *>(spec.c_str());
    bind[1].buffer_length = spec_length;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = const_cast<char *>(phone.c_str());
    bind[2].buffer_length = phone_length;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Doctor added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Doctor: "
             << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

// View Doctors Function (SELECT query)
void view_doctors(MYSQL *conn)
{
    string query = "select * from Doctors";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res == nullptr)
        {
            cout << "\n[Error] " << mysql_error(conn) << endl;
            return;
        }

        if (mysql_num_rows(res) == 0)
        {
            cout << "\nNo doctors found in the database.\n";
            mysql_free_result(res);
            return;
        }

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

void update_doctor(MYSQL *conn)
{
    int id;
    cout << "\n--- Update Doctor Information ---\n";
    cout << "Enter Doctor ID to update: ";
    if (!(cin >> id) || id <= 0)
    {
        cout << "\n[Error] Invalid Doctor ID.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int choice;
    cout << "\nWhat do you want to update?\n";
    cout << "1. Update Name Only\n";
    cout << "2. Update Specialization Only\n";
    cout << "3. Update Phone Only\n";
    cout << "4. Update All Fields\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice))
    {
        cout << "\n[Error] Invalid choice!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name, spec, phone, query;

    if (choice == 1)
    {
        cout << "Enter new doctor name: ";
        getline(cin, name);
        query = "UPDATE Doctors SET Name = ? WHERE Doctor_id = ?";
    }
    else if (choice == 2)
    {
        cout << "Enter new specialization: ";
        getline(cin, spec);
        query = "UPDATE Doctors SET Specialization = ? WHERE Doctor_id = ?";
    }
    else if (choice == 3)
    {
        cout << "Enter new phone: ";
        getline(cin, phone);
        query = "UPDATE Doctors SET Phone = ? WHERE Doctor_id = ?";
    }
    else if (choice == 4)
    {
        cout << "Enter new doctor name: ";
        getline(cin, name);
        cout << "Enter new specialization: ";
        getline(cin, spec);
        cout << "Enter new phone: ";
        getline(cin, phone);

        query = "UPDATE Doctors SET Name = ?, Specialization = ?, Phone = ? WHERE Doctor_id = ?";
    }
    else
    {
        cout << "\n[Error] Invalid choice!\n";
        return;
    }

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = name.length();
    unsigned long spec_length = spec.length();
    unsigned long phone_length = phone.length();

    if (choice == 1)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 2)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(spec.c_str());
        bind[0].buffer_length = spec_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 3)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(phone.c_str());
        bind[0].buffer_length = phone_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 4)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = const_cast<char *>(spec.c_str());
        bind[1].buffer_length = spec_length;

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = const_cast<char *>(phone.c_str());
        bind[2].buffer_length = phone_length;

        bind[3].buffer_type = MYSQL_TYPE_LONG;
        bind[3].buffer = &id;
    }

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
            cout << "\n[Success] Doctor Information Updated Successfully!\n";
        else
            cout << "\n[Info] No doctor found or no information changed.\n";
    }
    else
    {
        cout << "\n[Error] Failed to Update Doctor: "
             << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void delete_doctors(MYSQL *conn)
{
    cout << "delete doctor : " << endl;
    int id;

    cout << "enter doctor id you want to delete:" << endl;

    if (!(cin >> id) || id <= 0)
    {
        cout << "\n[Error] Invalid Doctor ID.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query = "DELETE FROM Doctors WHERE Doctor_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
        {
            cout << "\nDoctor deleted successfully!\n";
        }
        else
        {
            cout << "\nNo doctor found with this ID.\n";
        }
    }
    else
    {
        cout << "\nError: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void search_doctor(MYSQL *conn)
{
    string spec;
    cout << "enter doctor Specialization: " << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, spec);

    string query = "SELECT * FROM Doctors WHERE Specialization = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND param_bind[1];
    memset(param_bind, 0, sizeof(param_bind));

    unsigned long spec_length = spec.length();

    param_bind[0].buffer_type = MYSQL_TYPE_STRING;
    param_bind[0].buffer = const_cast<char *>(spec.c_str());
    param_bind[0].buffer_length = spec_length;

    if (mysql_stmt_bind_param(stmt, param_bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND result_bind[4];
    memset(result_bind, 0, sizeof(result_bind));

    int doctor_id;
    char name[256], specialization[256], phone[256];
    unsigned long name_length, spec_result_length, phone_length;

    result_bind[0].buffer_type = MYSQL_TYPE_LONG;
    result_bind[0].buffer = &doctor_id;

    result_bind[1].buffer_type = MYSQL_TYPE_STRING;
    result_bind[1].buffer = name;
    result_bind[1].buffer_length = sizeof(name);
    result_bind[1].length = &name_length;

    result_bind[2].buffer_type = MYSQL_TYPE_STRING;
    result_bind[2].buffer = specialization;
    result_bind[2].buffer_length = sizeof(specialization);
    result_bind[2].length = &spec_result_length;

    result_bind[3].buffer_type = MYSQL_TYPE_STRING;
    result_bind[3].buffer = phone;
    result_bind[3].buffer_length = sizeof(phone);
    result_bind[3].length = &phone_length;

    if (mysql_stmt_bind_result(stmt, result_bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int count = 0;

    while (mysql_stmt_fetch(stmt) == 0)
    {
        cout << "doctor_id : " << doctor_id << endl;
        cout << "Name : " << string(name, name_length) << endl;
        cout << "Specialization : " << string(specialization, spec_result_length) << endl;
        cout << "Phone : " << string(phone, phone_length) << endl
             << endl;
        count++;
    }

    if (count == 0)
    {
        cout << "No value found: " << endl;
    }

    mysql_stmt_close(stmt);
}

// Patient management function
void add_patient(MYSQL *conn);
void view_patient(MYSQL *conn);
void update_patient(MYSQL *conn);
void delete_patient(MYSQL *conn);
void search_patient(MYSQL *conn);
void patient_management(MYSQL *conn)
{
    int choice;
    do
    {
        cout << "Patient management system: " << endl;
        cout << "1. Add patient : " << endl;
        cout << "2. view patient details: " << endl;
        cout << "3. update patient info" << endl;
        cout << "4 Delete patient details:" << endl;
        cout << "5. search patient details: " << endl;
        cout << "6. returning to main menu: " << endl;

        cout << "enter your choice: " << endl;
        // cin >> choice;
        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice)
        {
        case 1:
            add_patient(conn);
            break;
        case 2:
            view_patient(conn);
            break;
        case 3:
            update_patient(conn);
            break;
        case 4:
            delete_patient(conn);
            break;
        case 5:
            search_patient(conn);
            break;
        case 6:
            cout << "Returning to main menu : " << endl;
            break;
        default:
            break;
        }
    } while (choice != 6);
}

void add_patient(MYSQL *conn)
{
    string name, gender, phone;
    int age;

    cout << "add patient details: " << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "add name : " << endl;
    getline(cin, name);
    if (name.empty())
    {
        cout << "\nPatient name cannot be empty!\n";
        return;
    }

    cout << "Enter patient age : " << endl;
    if (!(cin >> age) || age <= 0 || age > 120)
    {
        cout << "\nInvalid patient age!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "add patient gender: " << endl;
    getline(cin, gender);
    if (gender.empty())
    {
        cout << "\nGender cannot be empty!\n";
        return;
    }

    cout << "enter patient phone" << endl;
    getline(cin, phone);
    if (phone.empty())
    {
        cout << "\nPhone number cannot be empty!\n";
        return;
    }

    string query = "INSERT INTO Patients (patient_name, Gender, Phone, Age) VALUES (?, ?, ?, ?)";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = name.length();
    unsigned long gender_length = gender.length();
    unsigned long phone_length = phone.length();

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = const_cast<char *>(name.c_str());
    bind[0].buffer_length = name_length;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = const_cast<char *>(gender.c_str());
    bind[1].buffer_length = gender_length;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = const_cast<char *>(phone.c_str());
    bind[2].buffer_length = phone_length;

    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &age;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] patient added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add patient: "
             << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
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

        if (mysql_num_rows(res) == 0)
        {
            cout << "\nNo patients found in the database.\n";
            mysql_free_result(res);
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

void update_patient(MYSQL *conn)
{
    cout << "Enter Patient ID: ";
    int id;

    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Update patient info" << endl;
    cout << "input 1 for changing name: " << endl;
    cout << "input 2 for changing age: " << endl;
    cout << "input 3 for changing phone: " << endl;
    cout << "input 4 for changing all:" << endl;

    int choice;
    if (!(cin >> choice))
    {
        cout << "\nInvalid choice! Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name, phone, query;
    int age = 0;

    switch (choice)
    {
    case 1:
    {
        cout << "Enter patient new name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "\nPatient name cannot be empty!\n";
            return;
        }

        query = "UPDATE Patients SET patient_name = ? WHERE Patient_id = ?";
        break;
    }

    case 2:
    {
        cout << "Enter new age: ";

        if (!(cin >> age) || age <= 0 || age > 120)
        {
            cout << "\nInvalid age! Enter age between 1 and 120.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Patients SET Age = ? WHERE Patient_id = ?";
        break;
    }

    case 3:
    {
        cout << "enter new phone:" << endl;
        getline(cin, phone);

        if (phone.empty())
        {
            cout << "\nPhone number cannot be empty!\n";
            return;
        }

        query = "UPDATE Patients SET Phone = ? WHERE Patient_id = ?";
        break;
    }

    case 4:
    {
        cout << "Enter new name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "\nPatient name cannot be empty!\n";
            return;
        }

        cout << "Enter new phone: ";
        getline(cin, phone);

        if (phone.empty())
        {
            cout << "\nPhone number cannot be empty!\n";
            return;
        }

        cout << "Enter new age: ";

        if (!(cin >> age) || age <= 0 || age > 120)
        {
            cout << "\nInvalid age! Enter age between 1 and 120.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Patients SET patient_name = ?, Phone = ?, Age = ? WHERE Patient_id = ?";
        break;
    }

    default:
        cout << "invalid choice: " << endl;
        return;
    }

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = name.length();
    unsigned long phone_length = phone.length();

    if (choice == 1)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 2)
    {
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = &age;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 3)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(phone.c_str());
        bind[0].buffer_length = phone_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &id;
    }
    else if (choice == 4)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = const_cast<char *>(phone.c_str());
        bind[1].buffer_length = phone_length;

        bind[2].buffer_type = MYSQL_TYPE_LONG;
        bind[2].buffer = &age;

        bind[3].buffer_type = MYSQL_TYPE_LONG;
        bind[3].buffer = &id;
    }

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
        {
            cout << "Patient updated successfully!" << endl;
        }
        else
        {
            cout << "No patient updated. Check the ID or whether the values changed." << endl;
        }
    }
    else
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void delete_patient(MYSQL *conn)
{
    cout << "delete patient details: " << endl;
    int id;

    cout << "Enter Patient ID: ";
    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    char confirm;
    cout << "Are you sure you want to delete this patient? (Y/N): ";

    if (!(cin >> confirm) || (confirm != 'Y' && confirm != 'y' &&
                              confirm != 'N' && confirm != 'n'))
    {
        cout << "\n[Error] Enter Y or N only.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (confirm == 'N' || confirm == 'n')
    {
        cout << "\nPatient deletion cancelled.\n";
        return;
    }

    string query = "DELETE FROM Patients WHERE Patient_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
        {
            cout << "Patient deleted successfully!" << endl;
        }
        else
        {
            cout << "No patient found with this ID." << endl;
        }
    }
    else
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void search_patient(MYSQL *conn)
{
    int id;
    cout << "Enter Patient ID: ";

    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query = "SELECT * FROM Patients WHERE Patient_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND param[1];
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, param) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int patient_id, age;
    char name[256], gender[100], phone[100];

    unsigned long name_length, gender_length, phone_length;

    MYSQL_BIND result[5];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &patient_id;

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = name;
    result[1].buffer_length = sizeof(name);
    result[1].length = &name_length;

    result[2].buffer_type = MYSQL_TYPE_LONG;
    result[2].buffer = &age;

    result[3].buffer_type = MYSQL_TYPE_STRING;
    result[3].buffer = gender;
    result[3].buffer_length = sizeof(gender);
    result[3].length = &gender_length;

    result[4].buffer_type = MYSQL_TYPE_STRING;
    result[4].buffer = phone;
    result[4].buffer_length = sizeof(phone);
    result[4].length = &phone_length;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int status = mysql_stmt_fetch(stmt);

    if (status == 0)
    {
        name[name_length < 255 ? name_length : 255] = '\0';
        gender[gender_length < 99 ? gender_length : 99] = '\0';
        phone[phone_length < 99 ? phone_length : 99] = '\0';

        cout << "\nPatient Details:\n\n";
        cout << "Patient ID: " << patient_id << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: " << gender << endl;
        cout << "Phone: " << phone << endl;
    }
    else if (status == MYSQL_NO_DATA)
    {
        cout << "\nNo patient found with this ID.\n";
    }
    else
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

// --- Appointment Management Function Prototypes ---
void addAppointment(MYSQL *conn);
void viewAppointments(MYSQL *conn);
void update_appointment(MYSQL *conn);
void deleteAppointment(MYSQL *conn);
void appointment_status_update(MYSQL *conn);

void appointment_management(MYSQL *conn)
{
    int choice;
    do
    {
        cout << "\n--- Appointment Management Menu ---\n";
        cout << "1. Book Appointment\n";
        cout << "2. View All Appointments\n";
        cout << "3. Update appointment: " << endl;
        cout << "4. Cancel Appointment\n";
        cout << "5. appointment status : \n";
        cout << "6. Back to Main Menu\n";

        cout << "Enter your choice: ";
        // cin >> choice;
        if (!(cin >> choice))
        {
            cout << "\nInvalid choice! Please enter a number\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            addAppointment(conn);
            break;
        case 2:
            viewAppointments(conn);
            break;
        case 3:
            update_appointment(conn);
            break;
        case 4:
            deleteAppointment(conn);
            break;
        case 5:
            appointment_status_update(conn);
            break;
        case 6:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);
}

// Book Appointment Function (INSERT query)

void addAppointment(MYSQL *conn)
{
    string app_date;
    int doctor_id, patient_id;

    cout << "\n--- Book New Appointment ---\n";

    cout << "Enter Doctor ID: ";
    if (!(cin >> doctor_id) || doctor_id <= 0)
    {
        cout << "\nInvalid Doctor ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter Patient ID: ";
    if (!(cin >> patient_id) || patient_id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Appointment Date (YYYY-MM-DD): ";
    getline(cin, app_date);

    // Validate date format
    if (app_date.length() != 10 ||
        app_date[4] != '-' || app_date[7] != '-')
    {
        cout << "\nInvalid date format! Use YYYY-MM-DD.\n";
        return;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;

        if (!isdigit(static_cast<unsigned char>(app_date[i])))
        {
            cout << "\nInvalid date! Use YYYY-MM-DD.\n";
            return;
        }
    }

    int year = stoi(app_date.substr(0, 4));
    int month = stoi(app_date.substr(5, 2));
    int day = stoi(app_date.substr(8, 2));

    if (year < 1000 || month < 1 || month > 12)
    {
        cout << "\nInvalid appointment date!\n";
        return;
    }

    int days_in_month[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    if (year % 400 == 0 ||
        (year % 4 == 0 && year % 100 != 0))
    {
        days_in_month[1] = 29;
    }

    if (day < 1 || day > days_in_month[month - 1])
    {
        cout << "\nInvalid appointment date!\n";
        return;
    }

    // Get today's local date
    time_t now = time(nullptr);
    tm *local_time = localtime(&now);

    if (local_time == nullptr)
    {
        cout << "\n[Error] Unable to get current date!\n";
        return;
    }

    char today[11];
    strftime(today, sizeof(today), "%Y-%m-%d", local_time);

    if (app_date < string(today))
    {
        cout << "\nCannot book appointment for a past date!\n";
        return;
    }

    // Check whether doctor exists
    string doctor_query =
        "SELECT Doctor_id FROM Doctors WHERE Doctor_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, doctor_query.c_str(),
                           doctor_query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &doctor_id;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0 ||
        mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    bool doctor_exists = mysql_stmt_num_rows(stmt) > 0;
    mysql_stmt_close(stmt);

    if (!doctor_exists)
    {
        cout << "\n[Error] Doctor ID does not exist!\n";
        return;
    }

    // Check whether patient exists
    string patient_query =
        "SELECT Patient_id FROM Patients WHERE Patient_id = ?";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, patient_query.c_str(),
                           patient_query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &patient_id;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0 ||
        mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    bool patient_exists = mysql_stmt_num_rows(stmt) > 0;
    mysql_stmt_close(stmt);

    if (!patient_exists)
    {
        cout << "\n[Error] Patient ID does not exist!\n";
        return;
    }

    // Check existing appointment
    string check_query =
        "SELECT Appointment_id FROM Appointments "
        "WHERE Doctor_id = ? "
        "AND Appointment_date = ? "
        "AND Appointment_Status != 'Cancelled'";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, check_query.c_str(),
                           check_query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    unsigned long date_length = app_date.length();

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &doctor_id;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = &app_date[0];
    bind[1].buffer_length = date_length;
    bind[1].length = &date_length;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0 ||
        mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    bool appointment_exists = mysql_stmt_num_rows(stmt) > 0;
    mysql_stmt_close(stmt);

    if (appointment_exists)
    {
        cout << "\n[Error] This doctor already has an appointment "
                "on this date!\n";
        return;
    }

    // Insert new appointment
    string query =
        "INSERT INTO Appointments "
        "(Doctor_id, Patient_id, Appointment_date) "
        "VALUES (?, ?, ?)";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &doctor_id;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &patient_id;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = &app_date[0];
    bind[2].buffer_length = date_length;
    bind[2].length = &date_length;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Appointment Booked Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to book appointment: "
             << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

// View Appointments Function (SELECT query)
void viewAppointments(MYSQL *conn)
{
    string query =
        "SELECT a.Appointment_id, p.patient_name, d.Name, "
        "a.Appointment_date, a.Appointment_Status "
        "FROM Appointments a "
        "JOIN Patients p ON a.Patient_id = p.Patient_id "
        "JOIN Doctors d ON a.Doctor_id = d.Doctor_id";

    if (mysql_query(conn, query.c_str()) != 0)
    {
        cout << "\n[Error] Failed to fetch appointments: "
             << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    cout << "\n===============================================================\n";
    cout << "                     APPOINTMENT LIST\n";
    cout << "===============================================================\n";
    cout << "Appt ID | Patient Name | Doctor Name | Date       | Status\n";
    cout << "---------------------------------------------------------------\n";

    MYSQL_ROW row;
    int count = 0;

    while ((row = mysql_fetch_row(res)))
    {
        cout << (row[0] ? row[0] : "NULL") << "      | "
             << (row[1] ? row[1] : "NULL") << " | "
             << (row[2] ? row[2] : "NULL") << " | "
             << (row[3] ? row[3] : "NULL") << " | "
             << (row[4] ? row[4] : "NULL") << endl;

        count++;
    }

    if (count == 0)
    {
        cout << "No appointments found." << endl;
    }

    cout << "===============================================================\n";
    cout << "Total appointments: " << count << endl;

    mysql_free_result(res);
}

void update_appointment(MYSQL *conn)
{
    int id, choice;

    cout << "\nEnter Appointment ID to change: ";
    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Appointment ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Check appointment ID
    string query =
        "SELECT Doctor_id, Appointment_date FROM Appointments "
        "WHERE Appointment_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int old_doc_id;
    char old_date_buffer[11] = {0};
    unsigned long old_date_length = 0;

    MYSQL_BIND result[2];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &old_doc_id;

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = old_date_buffer;
    result[1].buffer_length = sizeof(old_date_buffer) - 1;
    result[1].length = &old_date_length;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int fetch_status = mysql_stmt_fetch(stmt);

    if (fetch_status == MYSQL_NO_DATA)
    {
        mysql_stmt_close(stmt);
        cout << "\nAppointment ID not found!\n";
        return;
    }

    if (fetch_status != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    old_date_buffer[old_date_length < 10 ? old_date_length : 10] = '\0';
    string old_date = old_date_buffer;

    mysql_stmt_close(stmt);

    cout << "\n1. Change Doctor ID\n";
    cout << "2. Change Patient ID\n";
    cout << "3. Change Appointment Date\n";
    cout << "4. Change All\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice) || choice < 1 || choice > 4)
    {
        cout << "\nInvalid choice!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int doc_id = old_doc_id, pat_id = 0;
    string date = old_date;

    bool changeDoctor = false;
    bool changePatient = false;
    bool changeDate = false;

    // Helper: Check whether Doctor or Patient exists
    auto checkExists = [&](string sql, int value) -> int
    {
        MYSQL_STMT *s = mysql_stmt_init(conn);

        if (s == nullptr)
        {
            cout << "\nDatabase Error: " << mysql_error(conn) << endl;
            return -1;
        }

        if (mysql_stmt_prepare(s, sql.c_str(), sql.length()) != 0)
        {
            cout << "\nDatabase Error: " << mysql_stmt_error(s) << endl;
            mysql_stmt_close(s);
            return -1;
        }

        MYSQL_BIND param[1];
        memset(param, 0, sizeof(param));

        param[0].buffer_type = MYSQL_TYPE_LONG;
        param[0].buffer = &value;

        if (mysql_stmt_bind_param(s, param) != 0 ||
            mysql_stmt_execute(s) != 0 ||
            mysql_stmt_store_result(s) != 0)
        {
            cout << "\nDatabase Error: " << mysql_stmt_error(s) << endl;
            mysql_stmt_close(s);
            return -1;
        }

        bool exists = mysql_stmt_num_rows(s) > 0;
        mysql_stmt_close(s);

        return exists ? 1 : 0;
    };

    if (choice == 1 || choice == 4)
    {
        cout << "Enter new Doctor ID: ";

        if (!(cin >> doc_id) || doc_id <= 0)
        {
            cout << "\nInvalid Doctor ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int exists = checkExists(
            "SELECT Doctor_id FROM Doctors WHERE Doctor_id = ?",
            doc_id);

        if (exists == -1)
            return;

        if (exists == 0)
        {
            cout << "\nDoctor ID does not exist!\n";
            return;
        }

        changeDoctor = true;
    }

    if (choice == 2 || choice == 4)
    {
        cout << "Enter new Patient ID: ";

        if (!(cin >> pat_id) || pat_id <= 0)
        {
            cout << "\nInvalid Patient ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int exists = checkExists(
            "SELECT Patient_id FROM Patients WHERE Patient_id = ?",
            pat_id);

        if (exists == -1)
            return;

        if (exists == 0)
        {
            cout << "\nPatient ID does not exist!\n";
            return;
        }

        changePatient = true;
    }

    if (choice == 3 || choice == 4)
    {
        cout << "Enter appointment date (YYYY-MM-DD): ";
        getline(cin, date);

        if (date.length() != 10 ||
            date[4] != '-' || date[7] != '-')
        {
            cout << "\nInvalid date format! Use YYYY-MM-DD.\n";
            return;
        }

        for (int i = 0; i < 10; i++)
        {
            if (i == 4 || i == 7)
                continue;

            if (!isdigit(static_cast<unsigned char>(date[i])))
            {
                cout << "\nInvalid date format!\n";
                return;
            }
        }

        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        if (year < 1000 || month < 1 || month > 12)
        {
            cout << "\nInvalid date!\n";
            return;
        }

        int days[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31};

        if (year % 400 == 0 ||
            (year % 4 == 0 && year % 100 != 0))
        {
            days[1] = 29;
        }

        if (day < 1 || day > days[month - 1])
        {
            cout << "\nInvalid calendar date!\n";
            return;
        }

        time_t now = time(nullptr);
        tm *local = localtime(&now);

        if (local == nullptr)
        {
            cout << "\nUnable to get current date!\n";
            return;
        }

        char today[11];
        strftime(today, sizeof(today), "%Y-%m-%d", local);

        if (date < string(today))
        {
            cout << "\nCannot set an appointment in the past!\n";
            return;
        }

        changeDate = true;
    }

    int finalDoctor = changeDoctor ? doc_id : old_doc_id;
    string finalDate = changeDate ? date : old_date;

    // Check duplicate appointment
    string check_query =
        "SELECT Appointment_id FROM Appointments "
        "WHERE Doctor_id = ? "
        "AND Appointment_date = ? "
        "AND Appointment_Status != 'Cancelled' "
        "AND Appointment_id != ?";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, check_query.c_str(),
                           check_query.length()) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    unsigned long date_length = finalDate.length();

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &finalDoctor;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = &finalDate[0];
    bind[1].buffer_length = date_length;
    bind[1].length = &date_length;

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0 ||
        mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    bool duplicate = mysql_stmt_num_rows(stmt) > 0;
    mysql_stmt_close(stmt);

    if (duplicate)
    {
        cout << "\nThis doctor already has an active appointment on that date!\n";
        return;
    }

    // Build UPDATE query with placeholders
    query = "UPDATE Appointments SET ";

    int param_count = 0;

    if (changeDoctor)
    {
        query += "Doctor_id = ?";
        param_count++;
    }

    if (changePatient)
    {
        if (param_count > 0)
            query += ", ";

        query += "Patient_id = ?";
        param_count++;
    }

    if (changeDate)
    {
        if (param_count > 0)
            query += ", ";

        query += "Appointment_date = ?";
        param_count++;
    }

    query += " WHERE Appointment_id = ?";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    memset(bind, 0, sizeof(bind));

    int index = 0;

    if (changeDoctor)
    {
        bind[index].buffer_type = MYSQL_TYPE_LONG;
        bind[index].buffer = &doc_id;
        index++;
    }

    if (changePatient)
    {
        bind[index].buffer_type = MYSQL_TYPE_LONG;
        bind[index].buffer = &pat_id;
        index++;
    }

    if (changeDate)
    {
        bind[index].buffer_type = MYSQL_TYPE_STRING;
        bind[index].buffer = &date[0];
        bind[index].buffer_length = date.length();
        bind[index].length = &date_length;
        index++;
    }

    bind[index].buffer_type = MYSQL_TYPE_LONG;
    bind[index].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\nDatabase Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
            cout << "\nAppointment updated successfully!\n";
        else
            cout << "\nNo changes made; values may be unchanged.\n";
    }
    else
    {
        cout << "\nError updating appointment: "
             << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void deleteAppointment(MYSQL *conn)
{
    int id;

    cout << "\n--- Cancel Appointment ---\n";
    cout << "Enter Appointment ID to Cancel: ";

    if (!(cin >> id))
    {
        cout << "Invalid input! Enter a numeric ID." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (id <= 0)
    {
        cout << "Appointment ID must be positive." << endl;
        return;
    }

    // Check appointment status
    string checkQuery =
        "SELECT Appointment_Status FROM Appointments "
        "WHERE Appointment_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, checkQuery.c_str(),
                           checkQuery.length()) != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0 ||
        mysql_stmt_execute(stmt) != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    char status[50] = {0};
    unsigned long status_length = 0;
    bool status_is_null = false;

    MYSQL_BIND result[1];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_STRING;
    result[0].buffer = status;
    result[0].buffer_length = sizeof(status) - 1;
    result[0].length = &status_length;
    result[0].is_null = &status_is_null;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int fetch_status = mysql_stmt_fetch(stmt);

    if (fetch_status == MYSQL_NO_DATA)
    {
        cout << "No appointment found with this ID." << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (fetch_status != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (status_is_null)
    {
        cout << "Appointment status is NULL." << endl;
        mysql_stmt_close(stmt);
        return;
    }

    status[status_length < sizeof(status) ? status_length : sizeof(status) - 1] = '\0';

    if (string(status) == "Cancelled")
    {
        cout << "This appointment is already cancelled." << endl;
        mysql_stmt_close(stmt);
        return;
    }

    mysql_stmt_close(stmt);

    // Confirmation
    char confirm;
    cout << "Are you sure you want to cancel this appointment? (Y/N): ";

    if (!(cin >> confirm) ||
        (confirm != 'Y' && confirm != 'y' &&
         confirm != 'N' && confirm != 'n'))
    {
        cout << "Invalid confirmation! Enter Y or N." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (confirm == 'N' || confirm == 'n')
    {
        cout << "Cancellation aborted." << endl;
        return;
    }

    // Update appointment status
    string query =
        "UPDATE Appointments SET Appointment_Status = 'Cancelled' "
        "WHERE Appointment_id = ?";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
            cout << "Appointment cancelled successfully!" << endl;
        else
            cout << "Appointment status was not changed." << endl;
    }
    else
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void appointment_status_update(MYSQL *conn)
{
    int app_id;
    cout << "Enter appointment ID: " << endl;

    if (!(cin >> app_id) || app_id <= 0)
    {
        cout << "Invalid Appointment ID!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Check whether appointment exists
    string query = "SELECT Appointment_id FROM Appointments WHERE Appointment_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Statement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND param[1];
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &app_id;

    if (mysql_stmt_bind_param(stmt, param) != 0 ||
        mysql_stmt_execute(stmt) != 0)
    {
        cout << "Query failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cout << "Result failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_num_rows(stmt) == 0)
    {
        cout << "Appointment not found!" << endl;
        mysql_stmt_close(stmt);
        return;
    }

    mysql_stmt_close(stmt);

    cout << "\n1. Booked" << endl;
    cout << "2. Completed" << endl;
    cout << "3. Cancelled" << endl;
    cout << "Enter your choice: " << endl;

    int choice;

    if (!(cin >> choice))
    {
        cout << "Invalid choice!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    string status;

    if (choice == 1)
        status = "Booked";
    else if (choice == 2)
        status = "Completed";
    else if (choice == 3)
        status = "Cancelled";
    else
    {
        cout << "Invalid choice!" << endl;
        return;
    }

    // Update appointment status
    string update_query =
        "UPDATE Appointments SET Appointment_Status = ? WHERE Appointment_id = ?";

    stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Statement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, update_query.c_str(), update_query.length()) != 0)
    {
        cout << "Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND update_bind[2];
    memset(update_bind, 0, sizeof(update_bind));

    unsigned long status_length = status.length();

    update_bind[0].buffer_type = MYSQL_TYPE_STRING;
    update_bind[0].buffer = const_cast<char *>(status.c_str());
    update_bind[0].buffer_length = status_length;
    update_bind[0].length = &status_length;

    update_bind[1].buffer_type = MYSQL_TYPE_LONG;
    update_bind[1].buffer = &app_id;

    if (mysql_stmt_bind_param(stmt, update_bind) != 0)
    {
        cout << "Bind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "Status updated successfully!" << endl;
    }
    else
    {
        cout << "Update unsuccessful: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

// --- Medicine Management Function Prototypes ---
void addMedicine(MYSQL *conn);
void viewMedicines(MYSQL *conn);
void update_medicine(MYSQL *conn);
void deleteMedicine(MYSQL *conn);
void medicine_search(MYSQL *conn);
void low_stock(MYSQL *conn);

void medicine_management(MYSQL *conn)
{
    int choice;
    do
    {
        cout << "\n--- Medicine Management Menu ---\n";
        cout << "1. Add New Medicine\n";
        cout << "2. View All Medicines\n";
        cout << "3. Update medicine info\n";
        cout << "4. Delete Medicine\n";
        cout << "5. medicine search\n";
        cout << "6. View Low Stock Medicines\n";
        cout << "7. Back to Main Menu\n";

        cout << "Enter your choice: ";
        // cin >> choice;

        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            addMedicine(conn);
            break;
        case 2:
            viewMedicines(conn);
            break;
        case 3:
            update_medicine(conn);
            break;
        case 4:
            deleteMedicine(conn);
            break;
        case 5:
            medicine_search(conn);
            break;
        case 6:
            low_stock(conn);
        case 7:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 7);
}

void addMedicine(MYSQL *conn)
{
    string name;
    double price;
    int stock;

    cout << "\n--- Add New Medicine ---\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Medicine Name: ";
    getline(cin, name);

    if (name.empty())
    {
        cout << "\nError! Medicine name cannot be empty.\n";
        return;
    }

    cout << "Enter Price: ";

    if (!(cin >> price) || price <= 0)
    {
        cout << "\nInvalid price\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter Stock Quantity: ";

    if (!(cin >> stock) || stock < 0)
    {
        cout << "\nInvalid stock\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    string query = "INSERT INTO Medicine (medicine_Name, Price, Stock_quantity) VALUES (?, ?, ?)";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\nStatement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\nPrepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = name.length();

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = const_cast<char *>(name.c_str());
    bind[0].buffer_length = name_length;
    bind[0].length = &name_length;

    bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[1].buffer = &price;

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &stock;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\nBind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Medicine Added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Medicine: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void viewMedicines(MYSQL *conn)
{
    string query = "SELECT * FROM Medicine";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res == nullptr)
        {
            cout << "\nError: " << mysql_error(conn) << endl;
            return;
        }

        if (mysql_num_rows(res) == 0)
        {
            cout << "\nNo medicines found in the database.\n";
            mysql_free_result(res);
            return;
        }

        MYSQL_ROW row;
        cout << "\n=====================================================\n";
        cout << "                     MEDICINE LIST                   \n";
        cout << "=====================================================\n";
        cout << "Med ID\t | Medicine Name\t | Price\t | Stock\n";
        cout << "-----------------------------------------------------\n";
        while ((row = mysql_fetch_row(res)))
        {
            cout << (row[0] ? row[0] : "NULL") << "\t | "
                 << (row[1] ? row[1] : "NULL") << "\t\t | "
                 << (row[2] ? row[2] : "NULL") << "\t | "
                 << (row[3] ? row[3] : "NULL") << endl;
        }
        cout << "=====================================================\n";
        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Failed to fetch medicines: " << mysql_error(conn) << endl;
    }
}

void update_medicine(MYSQL *conn)
{
    int id;
    cout << "\n--- Update Medicine Information ---\n";

    cout << "Enter Medicine ID to update: ";
    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Medicine ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int choice;
    cout << "\nWhat do you want to update?\n";
    cout << "1. Update Name Only\n";
    cout << "2. Update Price Only\n";
    cout << "3. Update Stock Quantity Only\n";
    cout << "4. Update All Fields\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice))
    {
        cout << "\nInvalid choice!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name;
    double price;
    int quantity;

    string query;
    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    unsigned long name_length = 0;
    int param_count = 0;

    switch (choice)
    {
    case 1:
    {
        cout << "Enter new medicine name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "\nMedicine name cannot be empty!\n";
            return;
        }

        query = "UPDATE Medicine SET medicine_Name = ? WHERE Med_id = ?";

        name_length = name.length();

        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;
        bind[0].length = &name_length;

        param_count = 1;
        break;
    }

    case 2:
    {
        cout << "Enter new price: ";

        if (!(cin >> price) || price <= 0)
        {
            cout << "\nInvalid new price!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Medicine SET Price = ? WHERE Med_id = ?";

        bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
        bind[0].buffer = &price;

        param_count = 1;
        break;
    }

    case 3:
    {
        cout << "Enter new stock quantity: ";

        if (!(cin >> quantity) || quantity < 0)
        {
            cout << "\nInvalid new stock input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Medicine SET Stock_quantity = ? WHERE Med_id = ?";

        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = &quantity;

        param_count = 1;
        break;
    }

    case 4:
    {
        cout << "Enter new medicine name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "\nInvalid name input!\n";
            return;
        }

        cout << "Enter new price: ";

        if (!(cin >> price) || price <= 0)
        {
            cout << "\nInvalid new price!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter new stock quantity: ";

        if (!(cin >> quantity) || quantity < 0)
        {
            cout << "\nInvalid new stock input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Medicine SET medicine_Name = ?, Price = ?, Stock_quantity = ? WHERE Med_id = ?";

        name_length = name.length();

        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(name.c_str());
        bind[0].buffer_length = name_length;
        bind[0].length = &name_length;

        bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
        bind[1].buffer = &price;

        bind[2].buffer_type = MYSQL_TYPE_LONG;
        bind[2].buffer = &quantity;

        param_count = 3;
        break;
    }

    default:
        cout << "\n[Error] Invalid choice!\n";
        return;
    }

    // Bind Medicine ID as the last parameter
    bind[param_count].buffer_type = MYSQL_TYPE_LONG;
    bind[param_count].buffer = &id;

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\nStatement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\nPrepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\nBind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
        {
            cout << "\nMedicine updated successfully!" << endl;
        }
        else
        {
            cout << "\nNo medicine updated. Check the ID or whether the values changed." << endl;
        }
    }
    else
    {
        cout << "\nError updating medicine: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void deleteMedicine(MYSQL *conn)
{
    int id;
    cout << "\n--- Delete Medicine ---\n";

    cout << "Enter Medicine ID (Med_id) to Delete: ";

    if (!(cin >> id) || id <= 0)
    {
        cout << "\nInvalid Medicine ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    char confirm;
    cout << "Are you sure you want to delete this medicine? (Y/N): ";

    if (!(cin >> confirm))
    {
        cout << "\nInvalid confirmation!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    string query = "DELETE FROM Medicine WHERE Med_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Statement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "Bind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        if (mysql_stmt_affected_rows(stmt) > 0)
        {
            cout << "Medicine deleted successfully!" << endl;
        }
        else
        {
            cout << "No medicine found with this ID." << endl;
        }
    }
    else
    {
        cout << "Error: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void medicine_search(MYSQL *conn)
{
    string name;
    cout << "Enter medicine name: " << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    if (name.empty())
    {
        cout << "\nMedicine name cannot be empty!\n";
        return;
    }

    string search_name = "%" + name + "%";

    string query = "SELECT * FROM Medicine WHERE medicine_Name LIKE ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "Statement initialization failed: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND param[1];
    memset(param, 0, sizeof(param));

    unsigned long search_length = search_name.length();

    param[0].buffer_type = MYSQL_TYPE_STRING;
    param[0].buffer = const_cast<char *>(search_name.c_str());
    param[0].buffer_length = search_length;
    param[0].length = &search_length;

    if (mysql_stmt_bind_param(stmt, param) != 0)
    {
        cout << "Bind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "Query failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cout << "Result failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_num_rows(stmt) == 0)
    {
        cout << "\nNo medicine found!\n";
        mysql_stmt_close(stmt);
        return;
    }

    // Bind result columns
    int med_id;
    char med_name[256];
    double price;
    int stock;

    unsigned long med_name_length = 0;
    bool name_is_null = false;

    MYSQL_BIND result[4];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &med_id;

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = med_name;
    result[1].buffer_length = sizeof(med_name);
    result[1].length = &med_name_length;
    result[1].is_null = &name_is_null;

    result[2].buffer_type = MYSQL_TYPE_DOUBLE;
    result[2].buffer = &price;

    result[3].buffer_type = MYSQL_TYPE_LONG;
    result[3].buffer = &stock;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "Result binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "\n              Medicine Search Result\n";
    cout << "=====================================================\n";
    cout << "Med ID\t | Medicine Name\t | Price\t | Stock\n";
    cout << "-----------------------------------------------------\n";

    int fetch_status;

    while ((fetch_status = mysql_stmt_fetch(stmt)) == 0 ||
           fetch_status == MYSQL_DATA_TRUNCATED)
    {
        med_name[med_name_length < sizeof(med_name) - 1
                     ? med_name_length
                     : sizeof(med_name) - 1] = '\0';

        cout << med_id << "\t | "
             << med_name << "\t\t | "
             << price << "\t | "
             << stock << endl;
    }

    if (fetch_status != MYSQL_NO_DATA)
    {
        cout << "Fetch error: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void low_stock(MYSQL *conn)
{
    string query = "SELECT * FROM Medicine WHERE Stock_quantity <= ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int limit = 10;

    MYSQL_BIND param[1];
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &limit;

    if (mysql_stmt_bind_param(stmt, param) != 0)
    {
        cout << "\n[Error] Bind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "\n[Error] Execute failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] Result failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_num_rows(stmt) == 0)
    {
        cout << "\nNo low stock medicines found!\n";
        mysql_stmt_close(stmt);
        return;
    }

    int med_id;
    char med_name[256];
    double price;
    int stock;

    unsigned long name_length = 0;
    bool name_is_null = false;

    MYSQL_BIND result[4];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &med_id;

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = med_name;
    result[1].buffer_length = sizeof(med_name);
    result[1].length = &name_length;
    result[1].is_null = &name_is_null;

    result[2].buffer_type = MYSQL_TYPE_DOUBLE;
    result[2].buffer = &price;

    result[3].buffer_type = MYSQL_TYPE_LONG;
    result[3].buffer = &stock;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "\n[Error] Result binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "\n--- Low Stock Medicines ---\n";

    int fetch_status;

    while ((fetch_status = mysql_stmt_fetch(stmt)) == 0 ||
           fetch_status == MYSQL_DATA_TRUNCATED)
    {
        med_name[name_length < sizeof(med_name) - 1
                     ? name_length
                     : sizeof(med_name) - 1] = '\0';

        cout << "Medicine ID: " << med_id << endl;
        cout << "Medicine Name: " << med_name << endl;
        cout << "Price: " << price << endl;
        cout << "Stock Quantity: " << stock << endl;
        cout << "-------------------------\n";
    }

    if (fetch_status != MYSQL_NO_DATA)
    {
        cout << "\n[Error] Fetch failed: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void addmedicalrecord(MYSQL *conn);
void viewmedicalrecord(MYSQL *conn);
void searchmedicalrecord(MYSQL *conn);
void updatemedicalrecord(MYSQL *conn);
void deletemedicalrecord(MYSQL *conn);

void medical_record_menu(MYSQL *conn)
{
    int choice;

    do
    {
        cout << "\n--- Medical Records Menu ---\n";
        cout << "1. Add Medical Record\n";
        cout << "2. View All Medical Records\n";
        cout << "3. Search Medical Record by Patient ID\n";
        cout << "4. Update Medical Record\n";
        cout << "5. Delete Medical record\n";
        cout << "6. Back to Main Menu\n";

        cout << "Enter your choice: ";
        // cin >> choice;

        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            addmedicalrecord(conn);
        }
        else if (choice == 2)
        {
            viewmedicalrecord(conn);
        }
        else if (choice == 3)
        {
            searchmedicalrecord(conn);
        }
        else if (choice == 4)
        {
            updatemedicalrecord(conn);
        }
        else if (choice == 5)
        {
            deletemedicalrecord(conn);
        }
        else if (choice == 6)
        {
            cout << "\nReturning to Main Menu...\n";
        }
        else
        {
            cout << "\nInvalid choice!\n";
        }

    } while (choice != 6);
}

void addmedicalrecord(MYSQL *conn)
{
    int patient_id, doctor_id;
    string diagnosis, treatment, record_date;

    cout << "\n--- Add Medical Record ---\n";

    cout << "Enter Patient ID: ";
    cin >> patient_id;

    cout << "Enter Doctor ID: ";
    cin >> doctor_id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Diagnosis: ";
    getline(cin, diagnosis);

    cout << "Enter Treatment: ";
    getline(cin, treatment);

    cout << "Enter Record Date (YYYY-MM-DD): ";
    getline(cin, record_date);

    // Step 1: Replace String Concatenation with SQL Placeholders
    string query = "INSERT INTO Medical_Records "
                   "(Patient_id, Doctor_id, Diagnosis, Treatment, Record_date) "
                   "VALUES (?, ?, ?, ?, ?)";

    // Step 2: Initialize Prepared Statement
    MYSQL_STMT *stmt;
    stmt = mysql_stmt_init(conn);
    if (stmt == nullptr)
    {
        cout << "Initialization failed" << mysql_error(conn) << endl;
        return;
    }

    // Step 3: Prepare SQL Query
    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] Query Preparation Failed: "
             << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    // Step 4: Bind Variables using MYSQL_BIND
    MYSQL_BIND bind[5];
    memset(bind, 0, sizeof(bind));

    // Step 5: Bind Integer Variables
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &patient_id;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &doctor_id;

    // Step 6: Bind String Variables
    unsigned long diagnosis_length = diagnosis.length();
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    // bind[2].buffer = &diagnosis[0];
    bind[2].buffer = const_cast<char *>(diagnosis.c_str());
    bind[2].buffer_length = diagnosis_length;

    unsigned long treatment_length = treatment.length();
    unsigned long record_date_length = record_date.length();

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = const_cast<char *>(treatment.c_str());
    bind[3].buffer_length = treatment_length;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = const_cast<char *>((record_date.c_str()));
    bind[4].buffer_length = record_date_length;

    // Step 7 : Bind Parameters
    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] Binding Failed: "
             << mysql_stmt_error(stmt) << endl;

        mysql_stmt_close(stmt);
        return;
    }

    // Step 8: Execute Prepared Statement
    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Medical Record Added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Medical Record: "
             << mysql_stmt_error(stmt) << endl;
    }
    // Step 9: Close Prepared Statement
    mysql_stmt_close(stmt);
}

void viewmedicalrecord(MYSQL *conn)
{
    string query = "SELECT * FROM Medical_Records";

    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res == nullptr)
        {
            cout << "\n[Error] " << mysql_error(conn) << endl;
            return;
        }

        if (mysql_num_rows(res) == 0)
        {
            cout << "\nNo Medical Records Found!\n";
        }
        else
        {
            MYSQL_ROW row;
            cout << "\n--- All Medical Records ---\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << "Record ID: " << row[0] << endl;
                cout << "Patient ID: " << row[1] << endl;
                cout << "Doctor ID: " << row[2] << endl;
                cout << "Diagnosis: " << row[3] << endl;
                cout << "Treatment: " << row[4] << endl;
                cout << "Record Date: " << row[5] << endl;
                cout << "---------------------------\n";
            }
        }

        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Query failed: "
             << mysql_error(conn) << endl;
    }
}

void searchmedicalrecord(MYSQL *conn)
{
    int patient_id;

    cout << "\nEnter Patient ID: ";

    if (!(cin >> patient_id) || patient_id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    string query = "SELECT * FROM Medical_Records WHERE Patient_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] Prepare failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND param[1];
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &patient_id;

    if (mysql_stmt_bind_param(stmt, param) != 0)
    {
        cout << "\n[Error] Bind failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "\n[Error] Execute failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cout << "\n[Error] Result failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_num_rows(stmt) == 0)
    {
        cout << "\nNo medical records found for Patient ID: " << patient_id << endl;
        mysql_stmt_close(stmt);
        return;
    }

    int record_id, doctor_id, result_patient_id;
    char diagnosis[1024];
    char treatment[2048];
    char record_date[11];

    unsigned long diagnosis_length = 0;
    unsigned long treatment_length = 0;
    unsigned long date_length = 0;

    bool diagnosis_is_null = false;
    bool treatment_is_null = false;
    bool date_is_null = false;

    MYSQL_BIND result[6];
    memset(result, 0, sizeof(result));

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &record_id;

    result[1].buffer_type = MYSQL_TYPE_LONG;
    result[1].buffer = &result_patient_id;

    result[2].buffer_type = MYSQL_TYPE_LONG;
    result[2].buffer = &doctor_id;

    result[3].buffer_type = MYSQL_TYPE_STRING;
    result[3].buffer = diagnosis;
    result[3].buffer_length = sizeof(diagnosis);
    result[3].length = &diagnosis_length;
    result[3].is_null = &diagnosis_is_null;

    result[4].buffer_type = MYSQL_TYPE_STRING;
    result[4].buffer = treatment;
    result[4].buffer_length = sizeof(treatment);
    result[4].length = &treatment_length;
    result[4].is_null = &treatment_is_null;

    result[5].buffer_type = MYSQL_TYPE_STRING;
    result[5].buffer = record_date;
    result[5].buffer_length = 10;
    result[5].length = &date_length;
    result[5].is_null = &date_is_null;

    if (mysql_stmt_bind_result(stmt, result) != 0)
    {
        cout << "\n[Error] Result binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "\n--- Medical Records for Patient ID: " << patient_id << " ---\n";

    int fetch_status;

    while ((fetch_status = mysql_stmt_fetch(stmt)) == 0 ||
           fetch_status == MYSQL_DATA_TRUNCATED)
    {
        diagnosis[diagnosis_length < sizeof(diagnosis) - 1
                      ? diagnosis_length
                      : sizeof(diagnosis) - 1] = '\0';

        treatment[treatment_length < sizeof(treatment) - 1
                      ? treatment_length
                      : sizeof(treatment) - 1] = '\0';

        record_date[date_length < sizeof(record_date) - 1
                        ? date_length
                        : sizeof(record_date) - 1] = '\0';

        cout << "Record ID: " << record_id << endl;
        cout << "Patient ID: " << result_patient_id << endl;
        cout << "Doctor ID: " << doctor_id << endl;
        cout << "Diagnosis: " << (diagnosis_is_null ? "NULL" : diagnosis) << endl;
        cout << "Treatment: " << (treatment_is_null ? "NULL" : treatment) << endl;
        cout << "Record Date: " << (date_is_null ? "NULL" : record_date) << endl;
        cout << "-----------------------------\n";
    }

    if (fetch_status != MYSQL_NO_DATA)
    {
        cout << "\n[Error] Fetch failed: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}

void updatemedicalrecord(MYSQL *conn)
{
    int record_id, choice;
    string diagnosis, treatment, record_date;

    cout << "\nEnter Record ID to update: ";
    if (!(cin >> record_id) || record_id <= 0)
    {
        cout << "\n[Error] Invalid Record ID. Enter a positive number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string check_query = "SELECT * FROM Medical_Records WHERE Record_id = ? ";
    MYSQL_STMT *check_stmt = mysql_stmt_init(conn);
    if (check_stmt == nullptr)
    {
        cout << "\nstatment initilization failed : " << mysql_error(conn);
        return;
    }

    if (mysql_stmt_prepare(check_stmt, check_query.c_str(), check_query.length()) != 0)
    {
        cout << "statement preperation failed : " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    MYSQL_BIND check_bind[1];
    memset(check_bind, 0, sizeof(check_bind));

    check_bind[0].buffer_type = MYSQL_TYPE_LONG;
    check_bind[0].buffer = &record_id;

    if (mysql_stmt_bind_param(check_stmt, check_bind) != 0)
    {
        cout << "\n[Error] Binding Failed: "
             << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_execute(check_stmt) != 0)
    {
        cout << "\n[Error] Execution Failed: "
             << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_store_result(check_stmt) != 0)
    {
        cout << "\n[Error] Result Storage Failed: "
             << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_num_rows(check_stmt) == 0)
    {
        cout << "\nNo medical record found with ID: "
             << record_id << endl;

        mysql_stmt_close(check_stmt);
        return;
    }
    mysql_stmt_close(check_stmt);

    cout << "\n1. Update Diagnosis\n";
    cout << "2. Update Treatment\n";
    cout << "3. Update Record Date\n";
    cout << "4. Update All Fields\n";

    cout << "Enter your choice: ";
    if (!(cin >> choice))
    {
        cout << "\n[Error] Menu choice must be a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query;

    if (choice == 1)
    {
        cout << "Enter new Diagnosis: ";
        getline(cin, diagnosis);
        if (diagnosis.empty())
        {
            cout << "\n[Error] Diagnosis cannot be empty.\n";
            return;
        }
        query = "UPDATE Medical_Records SET Diagnosis = ? WHERE Record_id = ?";
    }
    else if (choice == 2)
    {
        cout << "Enter new Treatment: ";
        getline(cin, treatment);
        if (treatment.empty())
        {
            cout << "\n[Error] Treatment cannot be empty.\n";
            return;
        }
        query = "UPDATE Medical_Records SET Treatment = ? WHERE Record_id = ?";
    }
    else if (choice == 3)
    {
        cout << "Enter new Record Date (YYYY-MM-DD): ";
        getline(cin, record_date);
        if (record_date.length() != 10 ||
            record_date[4] != '-' ||
            record_date[7] != '-')
        {
            cout << "\n[Error] Invalid date format. Use YYYY-MM-DD.\n";
            return;
        }

        if (!all_of(record_date.begin(), record_date.end(),
                    [i = 0](char c) mutable
                    {
                        return (i == 4 || i == 7)
                                   ? (i++, c == '-')
                                   : (i++, isdigit(static_cast<unsigned char>(c)));
                    }))
        {
            cout << "\n[Error] Invalid date format. Use YYYY-MM-DD.\n";
            return;
        }

        int year = stoi(record_date.substr(0, 4));
        int month = stoi(record_date.substr(5, 2));
        int day = stoi(record_date.substr(8, 2));

        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (year < 1 || month < 1 || month > 12)
        {
            cout << "\n[Error] Invalid date!\n";
            return;
        }

        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        {
            days_in_month[2] = 29;
        }

        if (day < 1 || day > days_in_month[month])
        {
            cout << "\n[Error] Invalid date!\n";
            return;
        }

        query = "UPDATE Medical_Records SET Record_date = ? WHERE Record_id = ?";
    }
    else if (choice == 4)
    {
        cout << "Enter new Diagnosis: ";
        getline(cin, diagnosis);

        if (diagnosis.empty())
        {
            cout << "\n[Error] Diagnosis cannot be empty.\n";
            return;
        }

        cout << "Enter new Treatment: ";
        getline(cin, treatment);
        if (treatment.empty())
        {
            cout << "\n[Error] Treatment cannot be empty.\n";
            return;
        }

        cout << "Enter new Record Date (YYYY-MM-DD): ";
        getline(cin, record_date);
        if (record_date.length() != 10 ||
            record_date[4] != '-' ||
            record_date[7] != '-')
        {
            cout << "\n[Error] Invalid date format. Use YYYY-MM-DD.\n";
            return;
        }

        if (!all_of(record_date.begin(), record_date.end(),
                    [i = 0](char c) mutable
                    {
                        return (i == 4 || i == 7) ? (i++, c == '-') : (i++, isdigit(static_cast<unsigned char>(c)));
                    }))
        {
            cout << "\n[Error] Invalid date format. Use YYYY-MM-DD.\n";
            return;
        }

        int year = stoi(record_date.substr(0, 4));
        int month = stoi(record_date.substr(5, 2));
        int day = stoi(record_date.substr(8, 2));

        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (year < 1 || month < 1 || month > 12)
        {
            cout << "\n[Error] Invalid date!\n";
            return;
        }

        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        {
            days_in_month[2] = 29;
        }

        if (day < 1 || day > days_in_month[month])
        {
            cout << "\n[Error] Invalid date!\n";
            return;
        }

        query = "UPDATE Medical_Records SET Diagnosis = ?, "
                "Treatment = ?, Record_date = ? WHERE Record_id = ?";
    }
    else
    {
        cout << "\nInvalid choice!\n";
        return;
    }

    // Step 1: Initialize Prepared Statement
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] Initialization Failed: "
             << mysql_error(conn) << endl;
        return;
    }

    // Step 2: Prepare SQL Query
    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] Query Preparation Failed: "
             << mysql_stmt_error(stmt) << endl;

        mysql_stmt_close(stmt);
        return;
    }

    // Step 3: Create MYSQL_BIND Array
    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    // String Length
    unsigned long diagnosis_length = diagnosis.length();
    unsigned long treatment_length = treatment.length();
    unsigned long record_date_length = record_date.length();

    // Choice 1: Update Diagnosis
    if (choice == 1)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(diagnosis.c_str());
        bind[0].buffer_length = diagnosis_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &record_id;
    }

    // Choice 2: Update Treatment
    else if (choice == 2)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(treatment.c_str());
        bind[0].buffer_length = treatment_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &record_id;
    }

    // Choice 3: Update Record Date
    else if (choice == 3)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(record_date.c_str());
        bind[0].buffer_length = record_date_length;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = &record_id;
    }

    // Choice 4: Update All Fields
    else if (choice == 4)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = const_cast<char *>(diagnosis.c_str());
        bind[0].buffer_length = diagnosis_length;

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = const_cast<char *>(treatment.c_str());
        bind[1].buffer_length = treatment_length;

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = const_cast<char *>(record_date.c_str());
        bind[2].buffer_length = record_date_length;

        bind[3].buffer_type = MYSQL_TYPE_LONG;
        bind[3].buffer = &record_id;
    }
    // Step 5: Bind Parameters
    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] Binding Failed: "
             << mysql_stmt_error(stmt) << endl;

        mysql_stmt_close(stmt);
        return;
    }
    // Step 6: Execute Prepared Statement
    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Medical Record updated successfully!\n";
    }
    else
    {
        cout << "\n[Error] Update failed: "
             << mysql_stmt_error(stmt) << endl;
    }

    // Step 7: Close Prepared Statement
    mysql_stmt_close(stmt);
}

void deletemedicalrecord(MYSQL *conn)
{
    int record_id;
    char confirm;

    cout << "\nEnter Record ID to delete: ";
    // cin >> record_id;
    if (!(cin >> record_id) || record_id <= 0)
    {
        cout << "\n[Error] Invalid Record ID. Enter a positive number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string check_query = "SELECT Record_id FROM Medical_Records WHERE Record_id = ?";

    MYSQL_STMT *check_stmt = mysql_stmt_init(conn);

    if (check_stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(check_stmt, check_query.c_str(), check_query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    MYSQL_BIND check_bind[1];
    memset(check_bind, 0, sizeof(check_bind));

    check_bind[0].buffer_type = MYSQL_TYPE_LONG;
    check_bind[0].buffer = &record_id;

    if (mysql_stmt_bind_param(check_stmt, check_bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_execute(check_stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_store_result(check_stmt) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    if (mysql_stmt_num_rows(check_stmt) == 0)
    {
        cout << "\nNo medical record found with ID: " << record_id << endl;
        mysql_stmt_close(check_stmt);
        return;
    }

    mysql_stmt_close(check_stmt);

    cout << "Are you sure you want to delete this record? (Y/N): ";

    if (!(cin >> confirm) || (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n'))
    {
        cout << "\n[Error] Enter Y or N only.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'N' || confirm == 'n')
    {
        cout << "\nDelete cancelled.\n";
        return;
    }

    string query = "DELETE FROM Medical_Records WHERE Record_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if (stmt == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &record_id;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cout << "\n[Error] " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) == 0)
    {
        cout << "\n[Success] Medical Record deleted successfully!\n";
    }
    else
    {
        cout << "\n[Error] Delete failed: " << mysql_stmt_error(stmt) << endl;
    }

    mysql_stmt_close(stmt);
}