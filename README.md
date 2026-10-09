# Student Record Management System

## About the project
This is a small menu-driven C++ project for keeping student records. It uses a vector while the program is running and a text file (`students.txt`) to keep records between runs.

## Features
- Add a student with a unique positive ID
- Display all saved students
- Search by ID
- Update name, course and marks
- Delete a record after confirmation
- Sort a copy of the records by marks (highest first)
- Check input for IDs and marks
- Save changes to a text file

## What is used
- C++ structures and functions
- `vector` to hold student records
- `sort()` and a lambda function
- File handling with `ifstream` and `ofstream`
- Input validation with `cin`, `getline()` and `numeric_limits`

## Compile and run

Open the folder in VS Code. Make sure a C++ compiler such as MinGW/G++ is installed.

Windows:
```bash
g++ main.cpp -o student_management
.\student_management.exe
```

Linux/macOS:
```bash
g++ main.cpp -o student_management
./student_management
```

The file `students.txt` is created in the working directory when a record is saved. Do not delete it if you want to keep the saved records.

## File format
One student per line:
`ID|Name|Course|Marks`

The pipe symbol typed inside a name or course is changed to a slash so the record stays readable.
