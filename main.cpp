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
    cin >> id;

    int choice;
    cout << "\nWhat do you want to update?\n";
    cout << "1. Update Name Only\n";
    cout << "2. Update Specialization Only\n";
    cout << "3. Update Phone Only\n";
    cout << "4. Update All Fields\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    string query = "";

    if (choice == 1)
    {
        string name;
        cout << "Enter new doctor name: ";
        getline(cin, name);
        query = "UPDATE Doctors SET Name = '" + name + "' WHERE Doctor_id = " + to_string(id);
    }
    else if (choice == 2)
    {
        string spec;
        cout << "Enter new specialization: ";
        getline(cin, spec);
        query = "UPDATE Doctors SET Specialization = '" + spec + "' WHERE Doctor_id = " + to_string(id);
    }
    else if (choice == 3)
    {
        string phone;
        cout << "Enter new phone: ";
        getline(cin, phone);
        query = "UPDATE Doctors SET Phone = '" + phone + "' WHERE Doctor_id = " + to_string(id);
    }
    else if (choice == 4)
    {
        string name, spec, phone;
        cout << "Enter new doctor name: ";
        getline(cin, name);
        cout << "Enter new specialization: ";
        getline(cin, spec);
        cout << "Enter new phone: ";
        getline(cin, phone);
        query = "UPDATE Doctors SET Name = '" + name + "', Specialization = '" + spec + "', Phone = '" + phone + "' WHERE Doctor_id = " + to_string(id);
    }
    else
    {
        cout << "\n[Error] Invalid choice!\n";
        return;
    }

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Doctor Information Updated Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Update Doctor: " << mysql_error(conn) << endl;
    }
}

void delete_doctors(MYSQL *conn)
{
    cout << "delete doctor : " << endl;
    int id;
    cout << "enter doctor id you want to delete:" << endl;
    cin >> id;
    string query = "delete from Doctors where Doctor_id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
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
        cout << "\nError: " << mysql_error(conn) << endl;
    }
}

void search_doctor(MYSQL *conn)
{
    string spec;
    cout << "enter doctor Specialization: " << endl;
    cin.ignore();
    getline(cin, spec);
    string query = "SELECT * FROM Doctors WHERE Specialization = '" + spec + "'";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res == nullptr)
        {
            cout << "connectin error..." << mysql_error(conn) << endl;
        }
        else
        {
            MYSQL_ROW row;
            int count = 0;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                cout << "doctor_id : " << row[0] << endl;
                cout << "Name : " << row[1] << endl;
                cout << "Specialization : " << row[2] << endl;
                cout << "Phone : "
                     << row[3] << endl
                     << endl;
                count++;
            }
            if (count == 0)
            {
                cout << "No value found: " << endl;
            }

            mysql_free_result(res);
        }
    }
    else
    {
        cout << "error..." << mysql_error(conn);
    }
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

    string query = "insert into Patients (patient_name, Gender , Phone , Age) values ('" + name + "', '" + gender + "', '" + phone + "', " + to_string(age) + " )";

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

    string query = "";
    switch (choice)
    {
    case 1:
    {
        string name;
        cout << "Enter patient new name: ";
        getline(cin, name);

        if (name.empty())
        {
            cout << "\nPatient name cannot be empty!\n";
            return;
        }

        query = "update Patients set patient_name = '" + name + "' where Patient_id = " + to_string(id);
        query = "update Patients set patient_name = '" + name + "' where Patient_id = " + to_string(id);
        break;
    }
    case 2:
    {
        cout << "Enter new age: ";
        int age;

        if (!(cin >> age) || age <= 0 || age > 120)
        {
            cout << "\nInvalid age! Enter age between 1 and 120.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "update Patients set Age = " + to_string(age) + " where Patient_id = " + to_string(id);
        query = "update Patients set Age = " + to_string(age) + " where Patient_id = " + to_string(id);
        break;
    }
    case 3:
    {
        cout << "enter new phone:" << endl;
        string phone;
        getline(cin, phone);
        if (phone.empty())
        {
            cout << "\nPhone number cannot be empty!\n";
            return;
        }

        query = "update Patients set Phone = '" + phone + "' where Patient_id = " + to_string(id);
        break;
    }

    case 4:
    {
        string name, phone;
        int age;

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

        query = "UPDATE Patients SET patient_name = '" + name +
                "', Phone = '" + phone +
                "', Age = " + to_string(age) +
                " WHERE Patient_id = " + to_string(id);
        break;
    }

    default:
        cout << "invalid choice: " << endl;
        break;
    }
    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
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
        cout << "Error: " << mysql_error(conn) << endl;
    }
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
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "\nPatient deletion cancelled.\n";
        return;
    }
    string query = "delete from Patients where Patient_id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
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
        cout << "Error: " << mysql_error(conn) << endl;
    }
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

    string query = "select * from Patients where Patient_id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        // cout << "Search succesfull: " << endl;
        MYSQL_RES *res = mysql_store_result(conn);
        if (res == nullptr)
        {
            cout << "\n[Error] Failed to retrieve patient details: "
                 << mysql_error(conn) << endl;
            return;
        }
        else
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                cout << "patient details: \n"
                     << endl;
                cout << "patient id " << row[0] << endl;
                cout << "name " << row[1] << endl;
                cout << "age " << row[2] << endl;
                cout << "gender " << row[3] << endl;
                cout << "phone " << row[4] << endl
                     << endl;
            }
            else
            {
                cout << "no value found: " << endl;
            }
            mysql_free_result(res);
        }
    }
    else
    {
        cout << "error : " << mysql_error(conn);
    }
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

    // Validate Doctor ID
    cout << "Enter Doctor ID: ";
    if (!(cin >> doctor_id) || doctor_id <= 0)
    {
        cout << "\nInvalid Doctor ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Validate Patient ID
    cout << "Enter Patient ID: ";
    if (!(cin >> patient_id) || patient_id <= 0)
    {
        cout << "\nInvalid Patient ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Get appointment date
    cout << "Enter Appointment Date (YYYY-MM-DD): ";
    getline(cin, app_date);

    // Validate date format
    if (app_date.length() != 10 ||
        app_date[4] != '-' || app_date[7] != '-')
    {
        cout << "\nInvalid date format! Use YYYY-MM-DD.\n";
        return;
    }

    // Validate date characters
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

    // Validate days in month and leap year
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

    // Prevent past appointments
    if (app_date < string(today))
    {
        cout << "\nCannot book appointment for a past date!\n";
        return;
    }

    // Check whether doctor exists
    string doctor_query =
        "SELECT Doctor_id FROM Doctors WHERE Doctor_id = " +
        to_string(doctor_id);

    if (mysql_query(conn, doctor_query.c_str()) != 0)
    {
        cout << "\n[Error] Failed to check doctor: "
             << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] Failed to get doctor result: "
             << mysql_error(conn) << endl;
        return;
    }

    bool doctor_exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);

    if (!doctor_exists)
    {
        cout << "\n[Error] Doctor ID does not exist!\n";
        return;
    }

    // Check whether patient exists
    string patient_query =
        "SELECT Patient_id FROM Patients WHERE Patient_id = " +
        to_string(patient_id);

    if (mysql_query(conn, patient_query.c_str()) != 0)
    {
        cout << "\n[Error] Failed to check patient: "
             << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] Failed to get patient result: "
             << mysql_error(conn) << endl;
        return;
    }

    bool patient_exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);

    if (!patient_exists)
    {
        cout << "\n[Error] Patient ID does not exist!\n";
        return;
    }

    // Check existing appointment for this doctor on this date
    string check_query =
        "SELECT Appointment_id FROM Appointments "
        "WHERE Doctor_id = " +
        to_string(doctor_id) +
        " AND Appointment_date = '" + app_date +
        "' AND Appointment_Status != 'Cancelled'";

    if (mysql_query(conn, check_query.c_str()) != 0)
    {
        cout << "\n[Error] Failed to check appointment: "
             << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] Failed to get appointment result: "
             << mysql_error(conn) << endl;
        return;
    }

    bool appointment_exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);

    if (appointment_exists)
    {
        cout << "\n[Error] This doctor already has an appointment "
                "on this date!\n";
        return;
    }

    // Insert new appointment
    string query =
        "INSERT INTO Appointments "
        "(Doctor_id, Patient_id, Appointment_date) VALUES (" +
        to_string(doctor_id) + ", " +
        to_string(patient_id) + ", '" +
        app_date + "')";

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Appointment Booked Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to book appointment: "
             << mysql_error(conn) << endl;
    }
}

// View Appointments Function (SELECT query)
void viewAppointments(MYSQL *conn)
{
    string query = "SELECT * FROM Appointments";

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

    cout << "\n=====================================================\n";
    cout << "                 APPOINTMENT LIST                    \n";
    cout << "=====================================================\n";
    cout << "Appt ID | Patient ID | Doctor ID | Date       | Status\n";
    cout << "-----------------------------------------------------\n";

    MYSQL_ROW row;
    int count = 0;

    while ((row = mysql_fetch_row(res)))
    {
        cout << (row[0] ? row[0] : "NULL") << "\t | "
             << (row[1] ? row[1] : "NULL") << "\t     | "
             << (row[2] ? row[2] : "NULL") << "\t    | "
             << (row[3] ? row[3] : "NULL") << " | "
             << (row[4] ? row[4] : "NULL") << endl;

        count++;
    }

    if (count == 0)
    {
        cout << "No appointments found." << endl;
    }

    cout << "=====================================================\n";
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
    string query = "SELECT Doctor_id, Appointment_date FROM Appointments WHERE Appointment_id = " + to_string(id);

    if (mysql_query(conn, query.c_str()) != 0)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == nullptr)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == nullptr)
    {
        mysql_free_result(res);
        cout << "\nAppointment ID not found!\n";
        return;
    }

    int old_doc_id = atoi(row[0]);
    string old_date = row[1] ? row[1] : "";
    mysql_free_result(res);

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
    bool changeDoctor = false, changePatient = false, changeDate = false;

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

        query = "SELECT Doctor_id FROM Doctors WHERE Doctor_id = " + to_string(doc_id);
        if (mysql_query(conn, query.c_str()) != 0)
        {
            cout << "\nDatabase Error: " << mysql_error(conn) << endl;
            return;
        }

        res = mysql_store_result(conn);
        if (res == nullptr)
        {
            cout << "\nDatabase Error: " << mysql_error(conn) << endl;
            return;
        }

        bool exists = mysql_num_rows(res) > 0;
        mysql_free_result(res);

        if (!exists)
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

        query = "SELECT Patient_id FROM Patients WHERE Patient_id = " + to_string(pat_id);
        if (mysql_query(conn, query.c_str()) != 0)
        {
            cout << "\nDatabase Error: " << mysql_error(conn) << endl;
            return;
        }

        res = mysql_store_result(conn);
        if (res == nullptr)
        {
            cout << "\nDatabase Error: " << mysql_error(conn) << endl;
            return;
        }

        bool exists = mysql_num_rows(res) > 0;
        mysql_free_result(res);

        if (!exists)
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

        // Validate date format
        if (date.length() != 10 || date[4] != '-' || date[7] != '-')
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

        int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            days[1] = 29;

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

    // Check duplicate appointment for the same doctor and date
    query = "SELECT Appointment_id FROM Appointments WHERE Doctor_id = " +
            to_string(finalDoctor) +
            " AND Appointment_date = '" + finalDate +
            "' AND Appointment_Status != 'Cancelled' AND Appointment_id != " +
            to_string(id);

    if (mysql_query(conn, query.c_str()) != 0)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    if (res == nullptr)
    {
        cout << "\nDatabase Error: " << mysql_error(conn) << endl;
        return;
    }

    bool duplicate = mysql_num_rows(res) > 0;
    mysql_free_result(res);

    if (duplicate)
    {
        cout << "\nThis doctor already has an active appointment on that date!\n";
        return;
    }

    // Build update query
    query = "UPDATE Appointments SET ";

    if (changeDoctor)
        query += "Doctor_id = " + to_string(doc_id);

    if (changePatient)
    {
        if (changeDoctor)
            query += ", ";
        query += "Patient_id = " + to_string(pat_id);
    }

    if (changeDate)
    {
        if (changeDoctor || changePatient)
            query += ", ";
        query += "Appointment_date = '" + date + "'";
    }

    query += " WHERE Appointment_id = " + to_string(id);

    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
            cout << "\nAppointment updated successfully!\n";
        else
            cout << "\nNo changes made; values may be unchanged.\n";
    }
    else
    {
        cout << "\nError updating appointment: " << mysql_error(conn) << endl;
    }
}

// Cancel Appointment Function (DELETE query)
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

    string checkQuery = "SELECT Appointment_Status FROM Appointments WHERE Appointment_id = " + to_string(id);

    if (mysql_query(conn, checkQuery.c_str()) != 0)
    {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL)
    {
        cout << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (row == NULL)
    {
        cout << "No appointment found with this ID." << endl;
        mysql_free_result(result);
        return;
    }

    if (row[0] != NULL && string(row[0]) == "Cancelled")
    {
        cout << "This appointment is already cancelled." << endl;
        mysql_free_result(result);
        return;
    }

    mysql_free_result(result);

    char confirm;
    cout << "Are you sure you want to cancel this appointment? (Y/N): ";
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "Cancellation aborted." << endl;
        return;
    }

    string query = "UPDATE Appointments SET Appointment_Status = 'Cancelled' WHERE Appointment_id = " + to_string(id);

    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
            cout << "Appointment cancelled successfully!" << endl;
        else
            cout << "Appointment status was not changed." << endl;
    }
    else
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}

void appointment_status_update(MYSQL *conn)
{
    int app_id;
    cout << "enter appointment id: " << endl;
    cin >> app_id;
    string query = "select Appointment_id from Appointments where Appointment_id =" + to_string(app_id);

    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res != nullptr)
        {
            if (mysql_num_rows(res) == 0)
            {
                cout << "appointment not found: " << endl;
                mysql_free_result(res);
                return;
            }
            mysql_free_result(res);
        }
    }
    else
    {
        cout << "query failed : " << mysql_error(conn) << endl;
        return;
    }
    cout << "\n1. Booked " << endl;
    cout << "2. Completed " << endl;
    cout << "3. Cancelled " << endl;
    cout << "enter your choice : " << endl;
    int choice;
    cin >> choice;

    string status;
    if (choice == 1)
    {
        status = "Booked";
    }
    else if (choice == 2)
    {
        status = "Completed";
    }
    else if (choice == 3)
    {
        status = "Cancelled";
    }
    else
    {
        cout << "invalid choice : " << endl;
        return;
    }

    string update_query = "update Appointments set Appointment_Status = '" + status + "' where Appointment_id=" + to_string(app_id);
    if (mysql_query(conn, update_query.c_str()) == 0)
    {
        cout << "Status update succesfully : " << endl;
    }
    else
    {
        cout << "Update unsuccesfull " << mysql_error(conn) << endl;
    }
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
        cout << "\nerror!...medicine name can not be empty : " << endl;
        return;
    }

    cout << "Enter Price: ";
    // cin >> price;
    if (!(cin >> price) || (price <= 0))
    {
        cout << "\ninvalid price\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cout << "Enter Stock Quantity: ";
    // cin >> stock;
    if (!(cin >> stock) || stock < 0)
    {
        cout << "\ninvalid stock\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    string query = "INSERT INTO Medicine (medicine_Name, Price, Stock_quantity) VALUES ('" + name + "', " + to_string(price) + ", " + to_string(stock) + ")";

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Medicine Added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Medicine: " << mysql_error(conn) << endl;
    }
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
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query = "";

    switch (choice)
    {
    case 1:
    {
        string name;
        cout << "Enter new medicine name: ";
        getline(cin, name);
        if (name.empty())
        {
            cout << "\nMedicine name cannot be empty!\n";
            return;
        }
        query = "UPDATE Medicine SET medicine_Name = '" + name + "' WHERE Med_id = " + to_string(id);
        break;
    }
    case 2:
    {
        double price;
        cout << "Enter new price: ";
        // cin >> price;
        if (!(cin >> price) || (price <= 0))
        {
            cout << "\ninvalid new price " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        query = "UPDATE Medicine SET Price = " + to_string(price) + " WHERE Med_id = " + to_string(id);
        break;
    }
    case 3:
    {
        int quantity;
        cout << "Enter new stock quantity: ";
        // cin >> quantity;
        if (!(cin >> quantity) || (quantity < 0))
        {
            cout << "\ninvalid new stock input : " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        query = "UPDATE Medicine SET Stock_quantity = " + to_string(quantity) + " WHERE Med_id = " + to_string(id);
        break;
    }
    case 4:
    {
        string name;
        double price;
        int quantity;

        cout << "Enter new medicine name: ";
        // cin.ignore();
        getline(cin, name);
        if ((name == ""))
        {
            cout << "\ninvalid name input: " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter new price: ";
        // cin >> price;
        if (!(cin >> price) || (price <= 0))
        {
            cout << "\ninvalid new price " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter new stock quantity: ";
        // cin >> quantity;
        if (!(cin >> quantity) || (quantity < 0))
        {
            cout << "\ninvalid new stock input : " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        query = "UPDATE Medicine SET medicine_Name = '" + name + "', Price = " + to_string(price) + ", Stock_quantity = " + to_string(quantity) + " WHERE Med_id = " + to_string(id);
        break;
    }
    default:
        cout << "\n[Error] Invalid choice!\n";
        return;
    }

    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
        {
            cout << "Medicine updated successfully!" << endl;
        }
        else
        {
            cout << "No medicine updated. Check the ID or whether the values changed." << endl;
        }
    }
    else
    {
        cout << "Error updating medicine: " << mysql_error(conn) << endl;
    }
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
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    string query = "DELETE FROM Medicine WHERE Med_id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
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
        cout << "Error: " << mysql_error(conn) << endl;
    }
}

void medicine_search(MYSQL *conn)
{
    string name;
    cout << "enter medicine name : " << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    if (name.empty())
    {
        cout << "\nMedicine name cannot be empty!\n";
        return;
    }

    string query = "select * from Medicine where medicine_Name like '%" + name + "%' ";
    if (mysql_query(conn, query.c_str()) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res == nullptr)
        {
            cout << "error..." << mysql_error(conn);
            return;
        }
        if (mysql_num_rows(res) == 0)
        {
            cout << "\nno medicine found : " << endl;
            mysql_free_result(res);
            return;
        }
        MYSQL_ROW row;
        cout << "              medicine search result      " << endl;
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
        mysql_free_result(res);
    }
    else
    {
        cout << "error..." << mysql_error(conn) << endl;
    }
}

void low_stock(MYSQL *conn)
{
    string query = "SELECT * FROM Medicine WHERE Stock_quantity <= 10";

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
            cout << "\nNo low stock medicines found!\n";
        }
        else
        {
            MYSQL_ROW row;
            cout << "\n--- Low Stock Medicines ---\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << "Medicine ID: " << row[0] << endl;
                cout << "Medicine Name: " << row[1] << endl;
                cout << "Price: " << row[2] << endl;
                cout << "Stock Quantity: " << row[3] << endl;
                cout << "-------------------------\n";
            }
        }

        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Query failed: " << mysql_error(conn) << endl;
    }
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

    string query = "INSERT INTO Medical_Records "
                   "(Patient_id, Doctor_id, Diagnosis, Treatment, Record_date) VALUES (" +
                   to_string(patient_id) + ", " +
                   to_string(doctor_id) + ", '" +
                   diagnosis + "', '" +
                   treatment + "', '" +
                   record_date + "')";

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Medical Record Added Successfully!\n";
    }
    else
    {
        cout << "\n[Error] Failed to Add Medical Record: "
             << mysql_error(conn) << endl;
    }
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
    cin >> patient_id;

    string query = "SELECT * FROM Medical_Records WHERE Patient_id = " + to_string(patient_id);

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
            cout << "\nNo medical records found for Patient ID: " << patient_id << endl;
        }
        else
        {
            MYSQL_ROW row;
            cout << "\n--- Medical Records for Patient ID: " << patient_id << " ---\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << "Record ID: " << row[0] << endl;
                cout << "Patient ID: " << row[1] << endl;
                cout << "Doctor ID: " << row[2] << endl;
                cout << "Diagnosis: " << row[3] << endl;
                cout << "Treatment: " << row[4] << endl;
                cout << "Record Date: " << row[5] << endl;
                cout << "-----------------------------\n";
            }
        }

        mysql_free_result(res);
    }
    else
    {
        cout << "\n[Error] Query failed: " << mysql_error(conn) << endl;
    }
}

void updatemedicalrecord(MYSQL *conn)
{
    int record_id, choice;
    string diagnosis, treatment, record_date;

    cout << "\nEnter Record ID to update: ";
    cin >> record_id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string check_query = "SELECT * FROM Medical_Records WHERE Record_id = " + to_string(record_id);

    if (mysql_query(conn, check_query.c_str()) != 0)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_num_rows(res) == 0)
    {
        cout << "\nNo medical record found with ID: " << record_id << endl;
        mysql_free_result(res);
        return;
    }

    mysql_free_result(res);

    cout << "\n1. Update Diagnosis\n";
    cout << "2. Update Treatment\n";
    cout << "3. Update Record Date\n";
    cout << "4. Update All Fields\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query;

    if (choice == 1)
    {
        cout << "Enter new Diagnosis: ";
        getline(cin, diagnosis);
        query = "UPDATE Medical_Records SET Diagnosis = '" + diagnosis +
                "' WHERE Record_id = " + to_string(record_id);
    }
    else if (choice == 2)
    {
        cout << "Enter new Treatment: ";
        getline(cin, treatment);
        query = "UPDATE Medical_Records SET Treatment = '" + treatment +
                "' WHERE Record_id = " + to_string(record_id);
    }
    else if (choice == 3)
    {
        cout << "Enter new Record Date (YYYY-MM-DD): ";
        getline(cin, record_date);
        query = "UPDATE Medical_Records SET Record_date = '" + record_date +
                "' WHERE Record_id = " + to_string(record_id);
    }
    else if (choice == 4)
    {
        cout << "Enter new Diagnosis: ";
        getline(cin, diagnosis);

        cout << "Enter new Treatment: ";
        getline(cin, treatment);

        cout << "Enter new Record Date (YYYY-MM-DD): ";
        getline(cin, record_date);

        query = "UPDATE Medical_Records SET Diagnosis = '" + diagnosis +
                "', Treatment = '" + treatment +
                "', Record_date = '" + record_date +
                "' WHERE Record_id = " + to_string(record_id);
    }
    else
    {
        cout << "\nInvalid choice!\n";
        return;
    }

    if (mysql_query(conn, query.c_str()) == 0)
    {
        cout << "\n[Success] Medical Record updated successfully!\n";
    }
    else
    {
        cout << "\n[Error] Update failed: " << mysql_error(conn) << endl;
    }
}

void deletemedicalrecord(MYSQL *conn)
{
    int record_id;
    char confirm;

    cout << "\nEnter Record ID to delete: ";
    cin >> record_id;

    string check_query = "SELECT Record_id FROM Medical_Records WHERE Record_id = " + to_string(record_id);

    if (mysql_query(conn, check_query.c_str()) != 0)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (res == nullptr)
    {
        cout << "\n[Error] " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_num_rows(res) == 0)
    {
        cout << "\nNo medical record found with ID: " << record_id << endl;
        mysql_free_result(res);
        return;
    }

    mysql_free_result(res);

    cout << "Are you sure you want to delete this record? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y')
    {
        string query = "DELETE FROM Medical_Records WHERE Record_id = " + to_string(record_id);

        if (mysql_query(conn, query.c_str()) == 0)
        {
            cout << "\n[Success] Medical Record deleted successfully!\n";
        }
        else
        {
            cout << "\n[Error] Delete failed: " << mysql_error(conn) << endl;
        }
    }
    else
    {
        cout << "\nDelete cancelled.\n";
    }
}