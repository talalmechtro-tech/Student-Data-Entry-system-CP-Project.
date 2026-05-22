#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ==========================================
// MANDATORY GENERIC FUNCTIONS (Section 3)
// ==========================================

void initializeDatabase() {
    ifstream file("cp project data.csv");
    if (!file) {
        ofstream newFile("cp project data.csv");
        newFile << "RollNo,Name,FatherName,Class,Section,University\n";  // CSV Headers
        newFile.close();
        cout << "[SYSTEM] Database initialized. New CSV file created with student headers.\n";
    } else {
        cout << "[SYSTEM] Database link secure. CSV file already exists.\n";
    }
    file.close();
}

bool isUnique(string id) {
    ifstream file("cp project data.csv");
    if (!file) return true;

    string line;
    getline(file, line); 

    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string currentRollNo;
        getline(ss, currentRollNo, ',');

        if (currentRollNo == id) {
            file.close();
            return false; 
        }
    }
    file.close();
    return true; 
}

void appendRecord(string data) {
    ofstream file("cp project data.csv", ios::app);
    if (!file) {
        cout << "? Error: Could not open database file for writing.\n";
        return;
    }
    file << data << "\n";
    file.close();
    cout << "? Record successfully appended to the database.\n";
}

void searchByID(string id) {
    ifstream file("cp project data.csv");
    if (!file) {
        cout << "? Error: Database file missing.\n";
        return;
    }

    string line;
    getline(file, line); 
    bool found = false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find(id) == 0) { 
            stringstream ss(line);
            string rollNo, name, fatherName, className, section, university;

            getline(ss, rollNo, ',');
            getline(ss, name, ',');
            getline(ss, fatherName, ',');
            getline(ss, className, ',');
            getline(ss, section, ',');
            getline(ss, university, ',');

            cout << "\n?? Student Record Found:\n";
            cout << "========================================\n";
            cout << "Roll No (Key): " << rollNo << "\n";
            cout << "Student Name:  " << name << "\n";
            cout << "Father's Name: " << fatherName << "\n";
            cout << "Class/Degree:  " << className << "\n";
            cout << "Section:       " << section << "\n";
            cout << "University:    " << university << "\n";
            cout << "========================================\n";
            found = true;
            break; 
        }
    }

    if (!found) {
        cout << "? Record with Unique Key '" << id << "' not found in the database.\n";
    }
    file.close();
}

void updateRecord(string id, string newData) {
    ifstream file("cp project data.csv");
    ofstream tempFile("temp.csv"); 
    
    if (!file || !tempFile) {
        cout << "? System Error: File streaming mismatch.\n";
        return;
    }

    string line;
    bool updated = false;

    getline(file, line); 
    tempFile << line << "\n";

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string currentRollNo;
        getline(ss, currentRollNo, ',');

        if (currentRollNo == id) {
            tempFile << newData << "\n"; 
            updated = true;
        } else {
            tempFile << line << "\n";    
        }
    }

    file.close();
    tempFile.close();

    remove("cp project data.csv");
    rename("temp.csv", "cp project data.csv");

    if (updated) {
        cout << "?? Data Integrity Maintained: Record for ID " << id << " successfully updated.\n";
    } else {
        cout << "? Update Operations Halting: Target ID not found.\n";
    }
}

// ==========================================
// MAIN CONTROLLER (SIMULATION MODE)
// ==========================================
int main() {
    initializeDatabase();

    int choice;
    while (true) {
        cout << "\n========================================\n";
        cout << "   STUDENT SYSTEM (SIMULATION MODE)     \n";
        cout << "========================================\n";
        cout << "1. Search Student Record (Manual Test Input)\n";
        cout << "2. Update Student Record manually\n";
        cout << "3. Add New Student Entry manually\n";
        cout << "4. Exit System\n";
        cout << "Choose option (1-4): ";
        cin >> choice;

        if (choice == 1) {
            string simulatedHardwareID;
            // Hardware ke bajaye hum keyboard se input le rahe hain taake test ho sake
            cout << "Enter RollNo to Search (e.g. 200101): ";
            cin >> simulatedHardwareID;
            
            searchByID(simulatedHardwareID);

        } else if (choice == 2) {
            string id, name, fatherName, className, section, university;
            cout << "Enter Student RollNo to Update: ";
            cin >> id;
            
            if (isUnique(id)) {
                cout << "? This RollNo does not exist in our records.\n";
                continue;
            }

            cin.ignore();
            cout << "--- Enter Updated Student Details ---\n";
            cout << "Enter Student Name: "; getline(cin, name);
            cout << "Enter Father's Name: "; getline(cin, fatherName);
            cout << "Enter Class/Degree: "; getline(cin, className);
            cout << "Enter Section: "; getline(cin, section);
            cout << "Enter University: "; getline(cin, university);

            string assembledData = id + "," + name + "," + fatherName + "," + className + "," + section + "," + university;
            updateRecord(id, assembledData);

        } else if (choice == 3) {
            string id, name, fatherName, className, section, university;
            cout << "Enter New Student RollNo (Unique Key): ";
            cin >> id;

            if (!isUnique(id)) {
                cout << "? Duplicate unique key anomaly! Record deployment blocked.\n";
                continue;
            }

            cin.ignore();
            cout << "Enter Student Name: "; getline(cin, name);
            cout << "Enter Father's Name: "; getline(cin, fatherName);
            cout << "Enter Class/Degree: "; getline(cin, className);
            cout << "Enter Section: "; getline(cin, section);
            cout << "Enter University: "; getline(cin, university);

            string assembledData = id + "," + name + "," + fatherName + "," + className + "," + section + "," + university;
            appendRecord(assembledData);

        } else if (choice == 4) {
            cout << "\nExiting System Engine...\n";
            break;
        } else {
            cout << "? Command unregistered. Enter a value from 1 to 4.\n";
        }
    }

    return 0;
}

