# Hospital Management System

A console-based Hospital Management System developed using **C++** and **MySQL**. This project helps manage essential hospital operations, including doctors, patients, appointments, medicines, and medical records through a structured menu-driven interface.

## Features

### 1. Dashboard
- View total doctors
- View total patients
- View total appointments
- View total medicines
- View total medicine stock
- View low-stock medicines

### 2. Doctor Management
- Add new doctors
- View doctor information
- Update doctor details
- Delete doctor records
- Search doctors by specialization

### 3. Patient Management
- Register new patients
- View patient information
- Update patient details
- Delete patient records
- Search patients by ID

### 4. Appointment Management
- Book new appointments
- View appointment records
- Update appointment status
- Manage appointment statuses:
  - Booked
  - Completed
  - Cancelled
- Prevent duplicate appointments for the same doctor on the same date, excluding cancelled appointments

### 5. Medicine Management
- Add new medicines
- View medicine information
- Update medicine details
- Delete medicine records
- Search medicines by name
- Identify low-stock medicines

### 6. Medical Records Management
- Add medical records
- View medical records
- Search medical records
- Update medical records
- Delete medical records

## Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Application development |
| MySQL | Database management |
| MySQL Connector/C | C++ and MySQL connectivity |
| MySQL Workbench | Database design and management |
| Visual Studio Code | Code development |

## Project Structure

```text
Hospital-Management-System/
│
├── Database/
│   ├── Hospital_management_db.sql
│   └── sample.sql
│
├── .gitattributes
├── .gitignore
├── main.cpp
├── hospital.exe
├── libmysql.dll
└── README.md
```

## Requirements

Before running this project, make sure you have installed:

- C++ compiler (G++)
- MySQL Server 8.0
- MySQL Connector/C
- MySQL Workbench (recommended)
- Visual Studio Code or another C++ IDE

## Database Setup

### 1. Create Database and Tables

1. Open MySQL Workbench or your preferred MySQL client.
2. Open `Database/Hospital_management_db.sql`.
3. Execute the SQL script to create the database and all five tables.

The database contains the following tables:

- Doctors
- Patients
- Appointments
- Medicine
- Medical_Records

### 2. Insert Sample Data

1. Open `Database/sample.sql`.
2. Execute the script to insert sample medicine records into the `Medicine` table.

The sample data includes:
- Napa
- Seclo
- Histacin

### 3. Verify Database

Execute the following SQL commands:

```sql
USE Hospital_management_db;

SHOW TABLES;

SELECT * FROM Medicine;
```

## Database Configuration

Configure your MySQL connection details in `main.cpp`:

- Host
- Username
- Password
- Database name: `Hospital_management_db`

**Important:** Do not upload your actual database password or other sensitive credentials to GitHub.

## Compilation and Execution

Open a terminal in the project directory.

### Compile

Update the MySQL include and library paths according to your system, then run:

```bash
g++ main.cpp -o hospital -I"C:\Program Files\MySQL\MySQL Server 8.0\include" -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" -lmysql
```

### Run

On Windows PowerShell:

```powershell
.\hospital.exe
```

**Note:** Make sure `libmysql.dll` is available in the executable's directory or in your system PATH.

## Security and Validation

This project includes several practices to improve data handling and reliability:

- Prepared statements for user-input-dependent database queries
- Input validation
- Invalid ID handling
- Duplicate appointment prevention
- Medicine stock validation
- Database error handling

## ER Diagram

The ER diagram illustrates the relationships between Doctors, Patients, Appointments, and Medical Records, along with the independent Medicine table.

[View ER Diagram Source](Database/ER_Diagram.md)
  

## Project Objective

The objective of this project is to develop a console-based Hospital Management System that simplifies hospital operations through organized management of doctors, patients, appointments, medicines, and medical records.

It also demonstrates practical applications of C++, MySQL database integration, prepared statements, and input validation in software development.

## Author

**Bijoy Ahmed**

- GitHub: [Bijoy-Ahmed-56](https://github.com/Bijoy-Ahmed-56)
