#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_RECORDS = 100;

struct Student {
    string roll;     // Format: BSAI-YY-XXX [cite: 46, 59]
    string name;
    string dept;
    double cgpa;
    string status;   // active / inactive [cite: 48]
};

struct Course {
    string code;
    string title;
    int credits;
    int seats;
    string prereq;   // course code or NONE [cite: 48]
};

struct Enrollment {
    string roll;
    string courseCode;
    string semester;
    string status;   // enrolled / dropped [cite: 48]
};

struct Attendance {
    string roll;
    string courseCode;
    string date;     // DD-MM-YYYY [cite: 48]
    string status;   // P / A / L [cite: 48]
};

struct Fee {
    string roll;
    string semester;
    double amountDue;
    double amountPaid;
    string dueDate;  // DD-MM-YYYY [cite: 48]
    string paidDate; // DD-MM-YYYY [cite: 48]
};

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};


int parseCSVLine(const string& line, string fields[], int maxFields) {
    int fieldCount = 0;
    string currentField = "";
    bool inQuotes = false;

    for (size_t i = 0; i < line.length() && fieldCount < maxFields; i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields[fieldCount++] = currentField;
            currentField = "";
        } else {
            currentField += c;
        }
    }
    if (fieldCount < maxFields && !currentField.empty()) {
        fields[fieldCount++] = currentField;
    }
    return fieldCount;
}

void appendTXT(const string& filename, const string& row) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << row << "\n";
        file.close();
    }
}


bool isValidRoll(const string& roll) {
    // Format: BSAI-YY-XXX [cite: 46, 59]
    if (roll.length() != 11) return false;
    if (roll.substr(0, 5) != "BSAI-") return false; [cite: 59]
    if (roll[7] != '-') return false;
    for (int i = 5; i <= 6; i++) if (roll[i] < '0' || roll[i] > '9') return false; [cite: 59]
    for (int i = 8; i <= 10; i++) if (roll[i] < '0' || roll[i] > '9') return false; [cite: 59]
    return true;
}

bool hasDigits(const string& name) {
    for (size_t i = 0; i < name.length(); i++) {
        if (name[i] >= '0' && name[i] <= '9') return true;
    }
    return false;
}

void addStudent() {
    string roll, name, dept;
    double cgpa;

    cout << "\nEnter Roll Number (BSAI-YY-XXX): ";
    cin >> roll;
    if (!isValidRoll(roll)) {
        cout << "Error: Invalid Roll Number Format!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, name);
    if (hasDigits(name)) {
        cout << "Error: Name cannot contain digits!\n"; [cite: 59]
        return;
    }

    cout << "Enter Department: ";
    cin >> dept;
    cout << "Enter CGPA (0.0 - 4.0): ";
    cin >> cgpa;
    if (cgpa < 0.0 || cgpa > 4.0) {
        cout << "Error: Out of bounds CGPA value!\n"; [cite: 59]
        return;
    }

    // Save database row formatting
    string row = roll + "," + name + "," + dept + "," + to_string(cgpa).substr(0,4) + ",active";
    appendTXT("students.txt", row); [cite: 59]
    cout << "Student successfully recorded.\n";
}

// Custom Interactive Prefix Search Filter Module [cite: 39]
void searchAsYouType() {
    cout << "\n--- Search As You Type Loop (Press Enter with '.' to exit) ---\n"; [cite: 39]
    string query = "";
    char ch;
    cin.ignore();

    while (true) {
        cout << "Current prefix: [" << query << "] - Type next char: ";
        cin.get(ch);
        if (ch == '\n') continue;
        if (ch == '.') break;
        query += ch;

        // Open file and filter rows manually via string prefix validation [cite: 39]
        ifstream file("students.txt");
        string line;
        cout << "\n--- Filtered Results ---\n";
        while (getline(file, line)) {
            string fields[5];
            parseCSVLine(line, fields, 5);
            // Prefix matching logic using length check and substr [cite: 39]
            if (fields[1].length() >= query.length() && fields[1].substr(0, query.length()) == query) {
                cout << fields[0] << " | " << fields[1] << " | " << fields[2] << " | Status: " << fields[4] << "\n";
            }
        }
        file.close();
        cout << "-----------------------\n";
    }
}

// ============================================================================
// M6: FEE TRACKER & MANUAL DATE ARITHMETIC (fee_tracker.h / .cpp) [cite: 46, 73]
// ============================================================================
int daysInMonth(int m, int y) {
    int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m == 2) {
        // Pure arithmetic leap calculation (No ctime allowed) [cite: 32, 72]
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) return 29;
    }
    return days[m];
}

int calculateTotalDays(string dateStr) {
    // DD-MM-YYYY [cite: 72]
    int d = stoi(dateStr.substr(0, 2)); [cite: 72]
    int m = stoi(dateStr.substr(3, 2)); [cite: 72]
    int y = stoi(dateStr.substr(6, 4)); [cite: 72]

    int totalDays = y * 365 + d;
    for (int i = 1; i < m; i++) {
        totalDays += daysInMonth(i, y);
    }
    totalDays += (y / 4) - (y / 100) + (y / 400); // Leap accumulations
    return totalDays;
}

int daysBetween(string start, string end) {
    return calculateTotalDays(end) - calculateTotalDays(start); [cite: 72]
}

void processLateFines() {
    string due = "10-06-2026";
    string paid = "26-06-2026";
    int diff = daysBetween(due, paid); [cite: 72]
    cout << "\nDue Date: " << due << " | Paid Date: " << paid << "\n";
    cout << "Days Late: " << diff << "\n";
    if (diff > 0) {
        int weeks = diff / 7; [cite: 72]
        double fineRate = 0.02; // 2% late charges per week accrued [cite: 72]
        cout << "Calculated Penalty: " << (weeks * fineRate * 100) << "% over " << weeks << " completed weeks overdue.\n"; [cite: 72]
    }
}

// ============================================================================
// M7: REPORT GENERATOR ENGINE & SELECTION SORT (reports.h / .cpp) [cite: 46, 74]
// ============================================================================
void printMeritList() {
    Student students[MAX_RECORDS];
    int count = 0;

    ifstream file("students.txt");
    string line;
    while (getline(file, line) && count < MAX_RECORDS) {
        string fields[5];
        parseCSVLine(line, fields, 5);
        students[count].roll = fields[0];
        students[count].name = fields[1];
        students[count].dept = fields[2];
        students[count].cgpa = fields[3].empty() ? 0.0 : stod(fields[3]);
        students[count].status = fields[4];
        count++;
    }
    file.close();

    // Selection Sort Strategy to rank CGPA in descending order [cite: 59, 78]
    for (int i = 0; i < count - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (students[j].cgpa > students[maxIdx].cgpa) {
                maxIdx = j;
            }
        }
        Student temp = students[i];
        students[i] = students[maxIdx];
        students[maxIdx] = temp;
    }

    // Print Formatted Tabular Report via iomanip [cite: 46, 72]
    cout << "\n-------------------------------------------------------------\n";
    cout << setw(6) << "Rank" << " | " << setw(12) << "Roll" << " | " << setw(20) << "Student Name" << " | " << setw(6) << "CGPA" << "\n";
    cout << "-------------------------------------------------------------\n";
    int rank = 1;
    for (int i = 0; i < count; i++) {
        if (students[i].status == "active") {
            cout << setw(6) << rank++ << " | "
                 << setw(12) << students[i].roll << " | "
                 << setw(20) << students[i].name << " | "
                 << setw(6) << fixed << setprecision(2) << students[i].cgpa << "\n";
        }
    }
    cout << "-------------------------------------------------------------\n";
}

// ============================================================================
// NESTED MENU SYSTEM ENTRY (main.cpp) [cite: 46]
// ============================================================================
void studentMenu() {
    int choice;
    while (true) {
        cout << "\n[2-Level] Student Management Subsystem\n";
        cout << "1. Create Student Entry\n";
        cout << "2. Search Character Stream (Search As You Type)\n";
        cout << "0. Return to Main Dashboard\n";
        cout << "Selection: ";
        cin >> choice;

        if (choice == 0) break;
        else if (choice == 1) addStudent();
        else if (choice == 2) searchAsYouType();
        else cout << "Unknown Command!\n";
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\n=========================================\n";
        cout << "   CAMPUS ANALYTICS ENGINE (BS AI PF)    \n"; [cite: 1, 2]
        cout << "=========================================\n";
        cout << "1. Student Profile Operations\n";
        cout << "2. Run Fine Matrix Computations\n";
        cout << "3. Render Campus Academic Merit Dashboard\n";
        cout << "0. Shutdown System Application\n";
        cout << "=========================================\n";
        cout << "Action: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Closing down engine modules safely.\n";
            break;
        }

        switch (choice) {
            case 1: studentMenu(); break;
            case 2: processLateFines(); break;
            case 3: printMeritList(); break;
            default: cout << "Invalid Selection. Try again.\n";
        }
    }
    return 0;
}
