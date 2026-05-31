#include <iostream>   // For input and output operations
#include <fstream>    // For file handling
#include <sstream>    // For string stream operations
#include <string>     // For string data type
#include <cstdlib>    // For remove() and rename()

using namespace std;

// ==========================================
// MANDATORY GENERIC FUNCTIONS (Section 3)
// ==========================================

// Function to initialize the database file.
// If the CSV file does not exist, it creates one
// and adds column headers.
void initializeDatabase() {
    ifstream file("cp project data.csv");

    // Check if file exists
    if (!file) {
        ofstream newFile("cp project data.csv");

        // Write CSV column headers
        newFile << "RollNo,Name,FatherName,Class,Section,University\n";

        newFile.close();

        cout << "[SYSTEM] Database initialized. New CSV file created with student headers.\n";
    } else {
        cout << "[SYSTEM] Database link secure. CSV file already exists.\n";
    }

    file.close();
}

// Function to check whether a Roll Number already exists.
// Returns true if Roll Number is unique.
// Returns false if Roll Number already exists.
bool isUnique(string id) {
    ifstream file("cp project data.csv");

    if (!file)
        return true;

    string line;

    // Skip header line
    getline(file, line);

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string currentRollNo;

        // Extract Roll Number from current record
        getline(ss, currentRollNo, ',');

        // Compare with given ID
        if (currentRollNo == id) {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

// Function to append a new student record
// to the CSV database file.
void appendRecord(string data) {

    ofstream file("cp project data.csv", ios::app);

    if (!file) {
        cout << "? Error: Could not open database file for writing.\n";
        return;
    }

    // Add record at end of file
    file << data << "\n";

    file.close();

    cout << "? Record successfully appended to the database.\n";
}

// Function to search for a student record
// using Roll Number.
void searchByID(string id) {

    ifstream file("cp project data.csv");

    if (!file) {
        cout << "? Error: Database file missing.\n";
        return;
    }

    string line;

    // Skip header row
    getline(file, line);

    bool found = false;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        // Check whether line starts with entered Roll Number
        if (line.find(id) == 0) {

            stringstream ss(line);

            string rollNo,
                   name,
                   fatherName,
                   className,
                   section,
                   university;

            // Extract all fields from CSV row
            getline(ss, rollNo, ',');
            getline(ss, name, ',');
            getline(ss, fatherName, ',');
            getline(ss, className, ',');
            getline(ss, section, ',');
            getline(ss, university, ',');

            // Display student information
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

    // If record not found
    if (!found) {
        cout << "? Record with Unique Key '" << id
             << "' not found in the database.\n";
    }

    file.close();
}

// Function to update an existing student record.
// Uses a temporary file technique.
void updateRecord(string id, string newData) {

    ifstream file("cp project data.csv");
    ofstream tempFile("temp.csv");

    if (!file || !tempFile) {
        cout << "? System Error: File streaming mismatch.\n";
        return;
    }

    string line;
    bool updated = false;

    // Copy header row
    getline(file, line);
    tempFile << line << "\n";

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string currentRollNo;

        // Extract Roll Number
        getline(ss, currentRollNo, ',');

        // Replace matching record
        if (currentRollNo == id) {
            tempFile << newData << "\n";
            updated = true;
        }
        else {
            tempFile << line << "\n";
        }
    }

    file.close();
    tempFile.close();

    // Delete old database file
    remove("cp project data.csv");

    // Rename temp file to original file
    rename("temp.csv", "cp project data.csv");

    if (updated) {
        cout << "?? Data Integrity Maintained: Record for ID "
             << id << " successfully updated.\n";
    } else {
        cout << "? Update Operations Halting: Target ID not found.\n";
    }
}

// ==========================================
// MAIN CONTROLLER (SIMULATION MODE)
// ==========================================

int main() {

    // Ensure database exists before program starts
    initializeDatabase();

    int choice;

    // Infinite loop for menu system
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

        // SEARCH OPERATION
        if (choice == 1) {

            string simulatedHardwareID;

            // User enters Roll Number manually
            cout << "Enter RollNo to Search (e.g. 200101): ";
            cin >> simulatedHardwareID;

            searchByID(simulatedHardwareID);
        }

        // UPDATE OPERATION
        else if (choice == 2) {

            string id,
                   name,
                   fatherName,
                   className,
                   section,
                   university;

            cout << "Enter Student RollNo to Update: ";
            cin >> id;

            // Check if record exists
            if (isUnique(id)) {
                cout << "? This RollNo does not exist in our records.\n";
                continue;
            }

            cin.ignore();

            cout << "--- Enter Updated Student Details ---\n";

            cout << "Enter Student Name: ";
            getline(cin, name);

            cout << "Enter Father's Name: ";
            getline(cin, fatherName);

            cout << "Enter Class/Degree: ";
            getline(cin, className);

            cout << "Enter Section: ";
            getline(cin, section);

            cout << "Enter University: ";
            getline(cin, university);

            // Create updated CSV row
            string assembledData =
                id + "," +
                name + "," +
                fatherName + "," +
                className + "," +
                section + "," +
                university;

            updateRecord(id, assembledData);
        }

        // ADD NEW STUDENT OPERATION
        else if (choice == 3) {

            string id,
                   name,
                   fatherName,
                   className,
                   section,
                   university;

            cout << "Enter New Student RollNo (Unique Key): ";
            cin >> id;

            // Prevent duplicate Roll Numbers
            if (!isUnique(id)) {
                cout << "? Duplicate unique key anomaly! Record deployment blocked.\n";
                continue;
            }

            cin.ignore();

            cout << "Enter Student Name: ";
            getline(cin, name);

            cout << "Enter Father's Name: ";
            getline(cin, fatherName);

            cout << "Enter Class/Degree: ";
            getline(cin, className);

            cout << "Enter Section: ";
            getline(cin, section);

            cout << "Enter University: ";
            getline(cin, university);

            // Create CSV formatted record
            string assembledData =
                id + "," +
                name + "," +
                fatherName + "," +
                className + "," +
                section + "," +
                university;

            appendRecord(assembledData);
        }

        // EXIT PROGRAM
        else if (choice == 4) {

            cout << "\nExiting System Engine...\n";
            break;
        }

        // INVALID INPUT
        else {

            cout << "? Command unregistered. Enter a value from 1 to 4.\n";
        }
    }

    return 0;
}
