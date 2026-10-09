#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// Details of one student
struct Student {
    int id;
    string name;
    string course;
    float marks;
};

const string FILE_NAME = "students.txt";

// Avoid breaking the file format if someone types '|'
string fixText(string text) {
    for (char &ch : text) {
        if (ch == '|') ch = '/';
    }
    return text;
}

// Read records saved during an earlier run
vector<Student> readRecords() {
    vector<Student> list;
    ifstream file(FILE_NAME);

    Student s;
    char separator;

    while (file >> s.id >> separator) {
        if (separator != '|') break;

        string name, course, marksText;
        getline(file, name, '|');
        getline(file, course, '|');
        getline(file, marksText);

        try {
            s.name = name;
            s.course = course;
            s.marks = stof(marksText);

            if (s.id > 0 && s.marks >= 0 && s.marks <= 100) {
                list.push_back(s);
            }
        } catch (...) {
            // Ignore a damaged line and continue reading.
        }
    }

    return list;
}

// Save the current list to the file
bool writeRecords(const vector<Student>& list) {
    ofstream file(FILE_NAME);

    if (!file) {
        return false;
    }

    for (const Student& s : list) {
        file << s.id << '|'
             << fixText(s.name) << '|'
             << fixText(s.course) << '|'
             << s.marks << '\n';
    }

    return file.good();
}

// Find a student and return its position in the vector
int findStudent(const vector<Student>& list, int id) {
    for (int i = 0; i < static_cast<int>(list.size()); i++) {
        if (list[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Take integer input without leaving invalid characters in cin
int getNumber(const string& message) {
    int number;

    while (true) {
        cout << message;

        if (cin >> number) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }

        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read a name or course, including spaces
string getText(const string& message) {
    string text;

    while (true) {
        cout << message;
        getline(cin, text);

        if (text.find_first_not_of(" \t") != string::npos) {
            return fixText(text);
        }

        cout << "This field cannot be blank. Try again.\n";
    }
}

// Marks should be between 0 and 100
float getMarks() {
    float marks;

    while (true) {
        cout << "Enter marks out of 100: ";

        if (cin >> marks && marks >= 0 && marks <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return marks;
        }

        cout << "Enter marks from 0 to 100 only.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void showStudent(const Student& s) {
    cout << left
         << "ID: " << s.id
         << " | Name: " << s.name
         << " | Course: " << s.course
         << " | Marks: " << fixed << setprecision(2) << s.marks
         << " | Result: " << (s.marks >= 40 ? "Pass" : "Needs Improvement")
         << '\n';
}

void addStudent(vector<Student>& list) {
    cout << "\n--- Add Student ---\n";

    Student s;
    s.id = getNumber("Enter student ID: ");

    if (s.id <= 0) {
        cout << "ID must be greater than zero.\n";
        return;
    }

    if (findStudent(list, s.id) != -1) {
        cout << "This ID is already in use.\n";
        return;
    }

    s.name = getText("Enter name: ");
    s.course = getText("Enter course/branch: ");
    s.marks = getMarks();

    list.push_back(s);

    if (writeRecords(list)) {
        cout << "Student added successfully.\n";
    } else {
        list.pop_back();
        cout << "Could not save the record. Please check file permissions.\n";
    }
}

void showAll(const vector<Student>& list) {
    cout << "\n--- Student Records ---\n";

    if (list.empty()) {
        cout << "No student records are available.\n";
        return;
    }

    for (const Student& s : list) {
        showStudent(s);
    }

    cout << "\nTotal students: " << list.size() << '\n';
}

void searchStudent(const vector<Student>& list) {
    cout << "\n--- Search Student ---\n";

    int id = getNumber("Enter ID to search: ");
    int pos = findStudent(list, id);

    if (pos == -1) {
        cout << "No student found with this ID.\n";
    } else {
        showStudent(list[pos]);
    }
}

void updateStudent(vector<Student>& list) {
    cout << "\n--- Update Student ---\n";

    int id = getNumber("Enter ID to update: ");
    int pos = findStudent(list, id);

    if (pos == -1) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Current details:\n";
    showStudent(list[pos]);

    Student oldStudent = list[pos];

    list[pos].name = getText("Enter new name: ");
    list[pos].course = getText("Enter new course/branch: ");
    list[pos].marks = getMarks();

    if (writeRecords(list)) {
        cout << "Student details updated.\n";
    } else {
        list[pos] = oldStudent;
        cout << "Could not save changes. Old details restored.\n";
    }
}

void deleteStudent(vector<Student>& list) {
    cout << "\n--- Delete Student ---\n";

    int id = getNumber("Enter ID to delete: ");
    int pos = findStudent(list, id);

    if (pos == -1) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Selected record:\n";
    showStudent(list[pos]);

    cout << "Are you sure you want to delete it? (y/n): ";
    char answer;
    cin >> answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (answer != 'y' && answer != 'Y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    Student removed = list[pos];
    list.erase(list.begin() + pos);

    if (writeRecords(list)) {
        cout << "Student deleted.\n";
    } else {
        list.insert(list.begin() + pos, removed);
        cout << "Could not save changes. Record restored.\n";
    }
}

void sortStudents(const vector<Student>& list) {
    vector<Student> sortedList = list;

    sort(sortedList.begin(), sortedList.end(),
         [](const Student& a, const Student& b) {
             if (a.marks == b.marks) {
                 return a.name < b.name;
             }
             return a.marks > b.marks;
         });

    cout << "\n--- Students by Marks (Highest First) ---\n";

    if (sortedList.empty()) {
        cout << "No records to sort.\n";
        return;
    }

    for (const Student& s : sortedList) {
        showStudent(s);
    }
}

void showMenu() {
    cout << "\n====================================\n";
    cout << "     STUDENT RECORD MANAGEMENT\n";
    cout << "====================================\n";
    cout << "1. Add student\n";
    cout << "2. Show all students\n";
    cout << "3. Search student\n";
    cout << "4. Update student\n";
    cout << "5. Delete student\n";
    cout << "6. Sort by marks\n";
    cout << "7. Exit\n";
}

int main() {
    vector<Student> students = readRecords();

    cout << "Welcome to Student Record Management!\n";
    cout << students.size() << " record(s) loaded.\n";

    int choice;

    do {
        showMenu();
        choice = getNumber("Choose an option: ");

        switch (choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                showAll(students);
                break;
            case 3:
                searchStudent(students);
                break;
            case 4:
                updateStudent(students);
                break;
            case 5:
                deleteStudent(students);
                break;
            case 6:
                sortStudents(students);
                break;
            case 7:
                cout << "Thank you. Goodbye!\n";
                break;
            default:
                cout << "Choose a number between 1 and 7.\n";
        }
    } while (choice != 7);

    return 0;
}
