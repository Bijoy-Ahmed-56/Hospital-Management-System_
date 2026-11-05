create database if not exists Hospital_management_db;
use Hospital_management_db;

create table if not exists Doctors(
	Doctor_id int auto_increment primary key,
    Name varchar(100) not null,
    Specialization varchar(100) not null,
    Phone varchar(15)
);

create table if not exists Patients(
	Patient_id int primary key auto_increment,
    patient_name varchar(100) not null,
    Age int ,
    Gender varchar(10),
    Phone varchar(15)
);


CREATE TABLE IF NOT EXISTS Appointments(
    Appointment_id INT AUTO_INCREMENT PRIMARY KEY,
    Patient_id INT,
    Doctor_id INT,
    Appointment_date DATE,
    Appointment_Status VARCHAR(20) NOT NULL DEFAULT 'Booked',
    FOREIGN KEY (Patient_id) REFERENCES Patients(Patient_id),
    FOREIGN KEY (Doctor_id) REFERENCES Doctors(Doctor_id)
);

create table if not exists Medicine(
	Med_id int auto_increment primary key,
    medicine_Name varchar(100) not null,
    Price Decimal(10,2) not null,
    Stock_quantity int not null
);

CREATE TABLE IF NOT EXISTS Medical_Records (
    Record_id INT AUTO_INCREMENT PRIMARY KEY,
    Patient_id INT NOT NULL,
    Doctor_id INT NOT NULL,
    Diagnosis VARCHAR(255) NOT NULL,
    Treatment TEXT NOT NULL,
    Record_date DATE NOT NULL,
    FOREIGN KEY (Patient_id) REFERENCES Patients(Patient_id),
    FOREIGN KEY (Doctor_id) REFERENCES Doctors(Doctor_id)
);
