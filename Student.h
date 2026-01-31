/*
 ============================================================================
 Name        : Student.h
 Author      : Romain Dzeinse
 Version     : Original
 Copyright   : Romain Dzeinse Copywrite 2025
 Description : This file describes the declarations and constants of the application
 Date        : 12/30/25
 ============================================================================
*/


#ifndef STUDENT_H
#define STUDENT_H
#include <iostream>
#include <string>
using namespace std;
enum year_values {NONE2 = 0, FRESHMAN = 1, SOPHOMORE = 2, JUNIOR = 3, SENIOR = 4, SUPERSENIOR = 5, GRAD = 6};
enum department_values {NONE, ENGI, ENGL, MATH, COMP, CHEM, BIOL, PHYS};
const int DEFWAITLIST = 5;
const int DEFDIRECTORYSIZE = 100;
const int DEFCOURSESMAX = 20;
// ANSI escape codes
const string INVERT = "\033[7m";
// text color
const string TEXTBLACK = "\033[30";
const string TEXTRED = "\033[31m";
const string TEXTGREEN = "\033[32m";
const string TEXTYELLOW = "\033[33m";
const string TEXTBLUE = "\033[34m";
const string TEXTMAGENTA = "\033[35m";
const string TEXTCYAN = "\033[36m";
const string TEXTWHITE = "\033[37m";
// text highlight color
const string HIGHLIGHTBLACK = "\033[40";
const string HIGHLIGHTRED = "\033[41m";
const string HIGHLIGHTGREEN = "\033[42m";
const string HIGHLIGHTYELLOW = "\033[43m";
const string HIGHLIGHTBLUE = "\033[44m";
const string HIGHLIGHTMAGENTA = "\033[45m";
const string HIGHLIGHTCYAN = "\033[46m";
const string HIGHLIGHTWHITE = "\033[47m";
// clear
const string CLEARCOLORING = "\033[0m";
// clear terminal escape code
const string TERMINALCLEAR = "\033[2J\033[H";
// underline
const string UNDERLINE = "\033[4m";
const string BOLD = "\033[1m";





int calculatePrior(int , int , bool, bool );
string findMajorName(department_values );
string findYearName(year_values );

class Student{
public:
    friend class PriorQueue;
    friend class SchoolDirectory;
    friend class Course;

    Student(string , int , year_values , int , department_values , department_values , bool, string );
    ~Student();
    // getters
    string getName()const { return m_name; }
    int getAge()const { return m_age; }
    string getID()const { return m_ID; }
    year_values getYear()const { return m_year; }
    int getCreditCount()const { return m_creditCount; }
    department_values getMajor()const { return m_major; }
    department_values getMinor()const { return m_minor; }
    bool getAccommodations()const { return m_accommodations; }
    int getPriorVal()const { return m_priorVal; }

    // setters
    void setName(const string& name) { m_name = name; }
    void setAge(int age) { m_age = age; }
    void setID(const string& id) { m_ID = id; }
    void setYear(year_values year) { m_year = year; }
    void setCreditCount(int count) { m_creditCount = count; }
    void setMajor(department_values major) { m_major = major; }
    void setMinor(department_values minor) { m_minor = minor; }
    void setAccommodations(bool acc) { m_accommodations = acc; }



private:
    string m_name;
    int m_age;
    string m_ID;
    year_values m_year;   // 0 = freshman, ..., 3 = senior, 4 = supersenior/grad 
    int m_creditCount;
    department_values m_major;
    department_values m_minor;
    bool m_accommodations;    // 0 = no accommodations, 1 = accommocations
    int m_priorVal;
};


class PriorQueue{
public:
    friend class Course;

    PriorQueue(int );
    ~PriorQueue();
    void enqueue(string , int , year_values , int , department_values , department_values , bool, string ID);
    void dequeue();
    void dequeueByIndex(int );
    bool empty();
    Student* peek();
    bool editStudent(string , department_values );
    Student* getStudent(string );
    int getStudent2(string studentID);
    void displayWaitlist();
    void mergeSort(Student ** , int , int );
    void merge(Student **, int , int , int);

private:
    Student **m_priors;
    // Student *m_front;
    int m_index;
    int m_capacity;
};


class Course{
public:
    friend class SchoolDirectory;

    Course(string , string , string , int , department_values , int );
    ~Course();
    bool empty(){ return (m_num == 0 && m_students[0] == nullptr); }    // for it to be empty, the num has to be zero and the element at 0 has to be uninitialized
    bool full(){ return (m_num == m_capacity); }
    bool waitlistEmpty(){ return (m_waitList == nullptr || m_waitList->empty()); }
    void size();
    void enroll(string , int , year_values , int , department_values , department_values , bool, string ID);
    void displayStudents();
    void displayWaitlist();
    void unEnroll(string );
    string getName(){ return m_name; }
    string getProf(){ return m_prof; }
    string getCourseID(){ return m_id; }
    int getCredits(){ return m_credits; }
    department_values getDepartment(){ return m_dept; }
    int getCap(){ return m_capacity; }
    
    bool findStudent(string );
    Student* getStudent(string );
    int getStudent2(string );
    bool isWaitListOpen(){ return m_waitList; }
    void showWaitList(){ m_waitList->displayWaitlist(); }


private:
    string m_name;
    string m_prof;
    string m_id;
    int m_credits;
    department_values m_dept;
    int m_capacity;
    int m_num;
    Student **m_students;
    PriorQueue *m_waitList;
};


class SchoolDirectory{
    public:
    SchoolDirectory();
    ~SchoolDirectory();
    void addStudent(const Student &);
    void removeStudent(int );

    void addCourse(const Course &);
    void removeCourse(int );

    void displaySize();
    bool isStudentThere(string );
    Student* getStudent(string );
    bool isCourseThere(string );
    Course* getCourse(string );
    void viewCourses();
    void viewCourses2();
    void displayCourseSize();

    private:
    int m_index;  // also used as the array index
    Student **m_allStudents;
    int m_cap;

    int m_courseIndex;
    Course **m_courses;
    int m_courseCap;
};

#endif