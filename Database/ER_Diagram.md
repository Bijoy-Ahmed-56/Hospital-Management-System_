# Hospital Management System ER Diagram

```mermaid
erDiagram
    Doctors ||--o{ Appointments : handles
    Patients ||--o{ Appointments : books
    Doctors ||--o{ Medical_Records : creates
    Patients ||--o{ Medical_Records : has

    Doctors {
        int Doctor_id PK
        varchar Name
        varchar Specialization
        varchar Phone
    }

    Patients {
        int Patient_id PK
        varchar patient_name
        int Age
        varchar Gender
        varchar Phone
    }

    Appointments {
        int Appointment_id PK
        int Patient_id FK
        int Doctor_id FK
        date Appointment_date
        varchar Appointment_Status
    }

    Medicine {
        int Med_id PK
        varchar medicine_Name
        decimal Price
        int Stock_quantity
    }

    Medical_Records {
        int Record_id PK
        int Patient_id FK
        int Doctor_id FK
        varchar Diagnosis
        text Treatment
        date Record_date
    }
```
