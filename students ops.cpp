// student_ops.cpp
#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
#include <sstream>
using namespace std;

// Check roll format: BSAI-YY-XXX
bool validRoll(string roll) {
  if (roll.length() != 11)
    return false;
  if (roll.substr(0, 4) != "BSAI")
    return false;
  if (roll[4] != '-' || roll[7] != '-')
    return false;
  for (int i = 5; i <= 6; i++)
    if (roll[i] < '0' || roll[i] > '9')
      return false;
  for (int i = 8; i <= 10; i++)
    if (roll[i] < '0' || roll[i] > '9')
      return false;
  return true;
}

bool hasDigit(string s) {
  for (int i = 0; i < (int)s.length(); i++)
    if (s[i] >= '0' && s[i] <= '9')
      return true;
  return false;
}

void addStudent() {
  string roll, name, dept, cgpa;

  cout << "\n--- Add Student ---" << endl;
  cout << "Roll (BSAI-YY-XXX): ";
  cin >> roll;

  if (!validRoll(roll)) {
    cout << "Invalid roll format!" << endl;
    return;
  }

  readFile("students.txt");
  if (searchInColumn(0, roll) != -1) {
    cout << "Student already exists!" << endl;
    return;
  }

  cout << "Name (no digits): ";
  cin.ignore();
  getline(cin, name);
  if (hasDigit(name)) {
    cout << "Name cannot have digits!" << endl;
    return;
  }

  cout << "Department: ";
  cin >> dept;
  cout << "CGPA (0-4): ";
  cin >> cgpa;

  string line = roll + "," + name + "," + dept + "," + cgpa + ",active";
  appendFile("students.txt", line);
  cout << "Student added!" << endl;
}

void searchStudent(string roll) {
  readFile("students.txt");
  int idx = searchInColumn(0, roll);

  if (idx == -1) {
    cout << "Not found!" << endl;
    return;
  }

  cout << "\nRoll:   " << fileData[idx][0] << endl;
  cout << "Name:   " << fileData[idx][1] << endl;
  cout << "Dept:   " << fileData[idx][2] << endl;
  cout << "CGPA:   " << fileData[idx][3] << endl;
  cout << "Status: " << fileData[idx][4] << endl;
}

// Partial match - finds names containing the search text
void searchByName(string name) {
  readFile("students.txt");
  bool found = false;
  for (int i = 0; i < rowCount; i++) {
    if (fileData[i][1].find(name) != string::npos) {
      cout << fileData[i][0] << " | " << fileData[i][1] << " | "
           << fileData[i][2] << " | " << fileData[i][3] << endl;
      found = true;
    }
  }
  if (!found)
    cout << "No match." << endl;
}

void updateStudent(string roll) {
  readFile("students.txt");
  int idx = searchInColumn(0, roll);

  if (idx == -1) {
    cout << "Not found!" << endl;
    return;
  }

  cout << "Name: " << fileData[idx][1] << endl;
  cout << "Dept: " << fileData[idx][2] << endl;
  cout << "CGPA: " << fileData[idx][3] << endl;

  cout << "1. Name  2. Dept  3. CGPA\nChoice: ";
  int ch;
  cin >> ch;

  if (ch == 1) {
    cout << "New name: ";
    cin.ignore();
    getline(cin, fileData[idx][1]);
  } else if (ch == 2) {
    cout << "New dept: ";
    cin >> fileData[idx][2];
  } else if (ch == 3) {
    cout << "New CGPA: ";
    cin >> fileData[idx][3];
  }

  writeFile("students.txt", "roll,name,dept,cgpa,status");
  cout << "Updated!" << endl;
}

void deleteStudent(string roll) {
  readFile("students.txt");
  int idx = searchInColumn(0, roll);

  if (idx == -1) {
    cout << "Not found!" << endl;
    return;
  }

  fileData[idx][4] = "inactive";
  writeFile("students.txt", "roll,name,dept,cgpa,status");
  cout << roll << " deactivated." << endl;
}

void listStudents() {
  readFile("students.txt");

  cout << "\nRoll         | Name           | Dept | CGPA" << endl;
  cout << "-------------|----------------|------|------" << endl;
  for (int i = 0; i < rowCount; i++) {
    if (fileData[i][4] == "active") {
      cout << fileData[i][0] << " | " << fileData[i][1] << " | "
           << fileData[i][2] << "   | " << fileData[i][3] << endl;
    }
  }
}
