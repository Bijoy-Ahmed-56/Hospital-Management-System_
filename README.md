# Hospital Management System

A console-based Hospital Management System developed using **C++ and MySQL**. This project helps manage hospital operations, including doctors, patients, appointments, medicines, and medical records.

## Features

### 1. Dashboard

- Total Doctors
- Total Patients
- Total Appointments
- Total Medicines
- Total Medicine Stock
- Low Stock Medicines

### 2. Doctor Management

- Add new doctor
- View all doctors
- Update doctor information
- Delete doctor
- Search doctors by specialization

### 3. Patient Management

- Add new patient
- View all patients
- Update patient information
- Delete patient
- Search patient by ID

### 4. Appointment Management

- Book appointments
- View all appointments
- Update appointment status
- Appointment statuses: Booked, Completed, Cancelled
- Prevent duplicate doctor appointments on the same date

### 5. Medicine Management

- Add new medicine
- View all medicines
- Update medicine information
- Delete medicine
- Search medicine by name
- View low-stock medicines

### 6. Medical Records Management

- Add medical records
- View all medical records
- Search medical records
- Update medical records
- Delete medical records

## Technologies Used

- C++
- MySQL
- MySQL Connector/C
- Visual Studio Code
- MySQL Workbench

## Project Structure

```text
Hospital-Management-System/
│
├── database/
│   ├── Hospital_management_db.sql
│   └── sample_data.sql
│
├── main.cpp
├── README.md
└── libmysql.dll
```

## Database Setup

1. Open MySQL Workbench.
2. Open `database/Hospital_management_db.sql`.
3. Execute the SQL script to create the database and tables.
4. Optionally, execute `database/sample_data.sql` to insert sample medicine data.

**Note:** Run the sample data script only once to avoid duplicate records.

## Requirements

- C++ Compiler (G++)
- MySQL Server 8.0
- MySQL Connector/C
- Visual Studio Code or another C++ IDE

## Configuration

Before running the application, configure your MySQL connection details in `main.cpp`:

- Host
- Username
- Password
- Database name: `Hospital_management_db`

Do not upload your actual database password to GitHub.

## Compile and Run

Compile the project using:

```bash
g++ main.cpp -o hospital -I"C:\Program Files\MySQL\MySQL Server 8.0\include" -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" -lmysql
```

Run the application:

```bash
.\hospital.exe
```

Make sure `libmysql.dll` is available in the executable's directory or system PATH.

## Security and Validation

- Prepared Statements for user-input-dependent SQL queries
- Input validation
- Invalid ID handling
- Appointment double-booking prevention
- Medicine stock validation
- Database error handling

## Project Objective
