// student_ops.h
#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H
#include <string>
using namespace std;

bool validRoll(string roll);
bool hasDigit(string s);
void addStudent();
void searchStudent(string roll);
void searchByName(string name);
void updateStudent(string roll);
void deleteStudent(string roll);
void listStudents();

#endif
