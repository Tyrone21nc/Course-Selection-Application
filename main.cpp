/*
 ============================================================================
 Name        : main.cpp
 Author      : Romain Dzeinse
 Version     : Original
 Copyright   : Romain Dzeinse Copywrite 2025
 Description : This file includes information about the run process and the 
                overall grand picture
 Date        : 12/30/25
 ============================================================================
*/


#include "Student.h"
#include <regex>    // we include the regex library to validate the user id string
#include <cstdlib>  // for rand()
#include <thread>   // used along with chrono to set the time lapse
#include <chrono>   // for time lapse


string generateCourseID(){
    // this function will randomly generate a course id with these requirements 
    // 1. must start with either X or Z capitalized
    // 2. must proceed with another cap letter
    // 3. then proceed with 5 numbers randing from 0-9
    string s = "";
    s += (rand() % 2 == 0 ? 'X' : 'Z');   // randomly choose 'X' or 'Z' and add it to s

    // any capital letter. It does 'A' because that will get the ascii value and start 
    // adding values from 65 ('A' in decimal) and then mod by 26 (letters of alphabet),
    // to stay in range. EX: 65 + 2 = 97(in dec) = 'C', or 65 + 0 = 'A', 
    // or 65 + 25 = 87 = 'Z'. The range is [0-25] inclusive.
    s += char('A' + rand() % 26);
    for (int i = 0; i < 5; i++)
        s += char('0' + rand() % 10);    // same concept here, except we're starting at 48 (decimal) and not exeding 57(which is '9') 
    return s;   // return our new string
}


SchoolDirectory sd = SchoolDirectory();
Course course1 = Course("CMSC 447", "Prof1", generateCourseID(), 3, COMP, 2);
Course course2 = Course("CMSC 421", "Prof2", generateCourseID(), 4, COMP, 2);

bool validStudentID(string id){
    // a valid student id starts either 'A' or 'B' as the first character
    // regex rule("(A|B)[A-Z][0-9][0-9][0-9][0-9]");
    regex rule("(A|B)[A-Z][0-9]{4}");
    return (regex_match(id, rule));
}


// ***********Student options**************
void stuViewCourses(){
    sd.viewCourses();
}
void stuViewCourseStatus(string courseID){
    // cout << "IN STU VIEW Course Status" << endl;
    Course *c = sd.getCourse(courseID); // get the actual course, if it's there

    if(c == nullptr) { return; } // course doesn't exist


    if(c->empty()){  // course exists but it's empty
        cout << UNDERLINE + c->getName() + " is empty: 0/" << c->getCap() << CLEARCOLORING << endl;
        // cout << "This means it is empty" << endl;
    }
    else{
        // cout << "I enter here" << endl;
        c->size();
    }
}
year_values enterYear(){
    string y;
    cout << "Enter your year [1-6]:" << endl;
    cout << "1. Freshman" << endl;
    cout << "2. Sophomore" << endl;
    cout << "3. Junior" << endl;
    cout << "4. Senior" << endl;
    cout << "5. Super Senior" << endl;
    cout << "6. Graduate" << endl;
    cin >> y;
    if(y == "1"){
        return FRESHMAN;
    }
    else if(y == "2"){
        return SOPHOMORE;
    }
    else if(y == "3"){
        return JUNIOR;
    }
    else if(y == "4"){
        return SENIOR;
    }
    else if(y == "5"){
        return SUPERSENIOR;
    }
    else if(y == "6"){
        return GRAD;
    }
    return NONE2;
}
department_values enterMajor(string major){
    string m;
    cout << "Enter your " + major + " [1-7]:" << endl;
    cout << "1. Engineer (general)" << endl;
    cout << "2. English" << endl;
    cout << "3. Math" << endl;
    cout << "4. Computer Science" << endl;
    cout << "5. Chemistry" << endl;
    cout << "6. Biology" << endl;
    cout << "7. Physics" << endl;
    cout << "8. NONE" << endl;
    cin >> m;
    if(m == "1"){
        return ENGI;
    }
    else if(m == "2"){
        return ENGL;
    }
    else if(m == "3"){
        return MATH;
    }
    else if(m == "4"){
        return COMP;
    }
    else if(m == "5"){
        return CHEM;
    }
    else if(m == "6"){
        return BIOL;
    }
    else if(m == "7"){
        return PHYS;
    }
    return NONE;
}
void stuEnroll(string studentID, string courseID){
    if(!sd.getCourse(courseID)){ return; }  // course doesn't exist

    if(sd.getCourse(courseID)->findStudent(studentID)){    // this means the student is already enrolled or waitlisted for the course
        cout << "You've already enrolled/waitlisted for this course." << endl;
        return;
    }
    Student *stu = sd.getStudent(studentID);

    sd.getCourse(courseID)->enroll(stu->getName(), stu->getAge(), stu->getYear(), 
    stu->getCreditCount(), stu->getMajor(), stu->getMinor(), stu->getAccommodations(), 
    studentID);
}
void stuDrop(string studentID, string courseID){
    // cout << "In stuDrop" << endl;
    if(!sd.getCourse(courseID)){ return; }  // this means the course doesn't exist
    if(!sd.getCourse(courseID)->findStudent(studentID)){    // this means the student is not enrolled or waitlisted for the course
        cout << "You're not enrolled in this course." << endl;
        return;
    }
    cout << "We get here for dropped course" << endl;
    sd.getCourse(courseID)->unEnroll(studentID);
}
string userEnterCourseID(){
    sd.viewCourses2();
    string cID;
    cout << "Enter course id: ";
    cin >> cID;
    return cID;
}
void studentVersion(string studentID){
    // cout << "currently in student version" << endl;
    if(!sd.isStudentThere(studentID)){
        cout << "Not found in the student directory. Try again" << endl;
        return;
    }
    Student *stu = sd.getStudent(studentID);

    cout << "\nWelcome back " << TEXTGREEN << stu->getName() << CLEARCOLORING + ", nice to see you again" << endl;
    cout << "\t" + UNDERLINE << "Student Profile" << CLEARCOLORING << endl;
    cout << "Student ID: \t" + stu->getID() << endl;
    cout << "Year: \t\t" + findYearName(stu->getYear()) << endl;
    cout << "Major: \t\t" + findMajorName(stu->getMajor()) << endl;
    cout << "Minor: \t\t" + findMajorName(stu->getMinor()) << endl;
    cout << "Credit Count: \t" << stu->getCreditCount() << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;

    string options;
    do{
        cout << "Here are some things you can do." << endl;
        cout << "1. View courses" << endl;
        cout << "2. View course status" << endl;    // this will include both the current number of filled in positions and the waitlist filled positions
        cout << "3. Enroll in a course" << endl;
        cout << "4. Drop a course" << endl;
        cout << "5. Back to sign in" << endl;
        cout << "6. Dislay students" << endl;
        cout << "7. Dislay waitlist" << endl;
        // cout << "5. Display course enroll list" << endl; // this option will only be given to the staff
        cout << "What would you like to do:\n>>> ";
        cin >> options;
        if(options == "1"){
            stuViewCourses();   // shows available courses
            // options = "4";
        }
        else if(options == "2"){
            string cID = userEnterCourseID();
            if(cID.length() >= 7){  // this check prevents the program from stoping completely when we enter a choice such as DH104, with size less than 7
                stuViewCourseStatus(cID);
                // options = "4";
            }
        }
        else if(options == "3"){
            string cID = userEnterCourseID();
            if(cID.length() >= 7){  // this check prevents the program from stoping completely when we enter a choice such as DH104, with size less than 7
                stuEnroll(studentID, cID);
                // options = "4";
            }
        }
        else if(options == "4"){
            string cID = userEnterCourseID();
            if(cID.length() >= 7){  // this check prevents the program from stoping completely when we enter a choice such as DH104, with size less than 7
                stuDrop(studentID, cID);
                // options = "4";
            }
        }
        else if(options == "6"){
            // cout << "display students" << endl;
            string cID = userEnterCourseID();
            if(cID.length() >= 7){  // this check prevents the program from stoping completely when we enter a choice such as DH104, with size less than 7
                if(sd.getCourse(cID)->empty()){
                    cout << "No students enrolled" << endl;
                }
                else{
                    sd.getCourse(cID)->displayStudents();
                }
                // options = "4";
            }
        }
        else if(options == "7"){
            string cID = userEnterCourseID();
            if(cID.length() >= 7){  // this check prevents the program from stoping completely when we enter a choice such as DH104, with size less than 7
                if(sd.getCourse(cID)->isWaitListOpen()){
                    sd.getCourse(cID)->showWaitList();
                }
                else{
                    cout << "Waitlist is empty" << endl;
                }
                // options = "4";
            }
        }
        cout << endl;
    }
    while(options != "5");

    cout << "Going back to sign in." << endl << endl;
}


void signIn(){
    string studentID = "";
    // cout << "currently in Sign in" << endl;
    do{
        cout << "*****" + INVERT + "Sign In" + CLEARCOLORING + "*****" << endl;
        cout << "'q' to quit" << endl;
        cout << "'h' to help" << endl;
        cout << "Enter your school ID to sign in.\n>>> ";
        cin >> studentID;
        if(validStudentID(studentID)){
            studentVersion(studentID);
            // after doing the student version, we return back here and I set the credential variable's value to q to quit the loop
            // credential = "q";
        }
        else if(studentID == "h"){
            cout << "\n++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Help:" << endl;
            cout << "- valid student ID's start with A/B \nand continue with any uppercase \nletter, then followed by 4 numbers\nExample: AB1001, ..., AB1010" << endl;
            cout << "- type q to go back to the main menu" << endl;
            cout << "++++++++++++++++++++++++++++++++++++" << endl << endl;
        }
    }
    while(studentID != "q");
    cout << "Going back to main menu" << endl << endl;
}
void run(){
    string command;
    cout << "Welcome to the Course Selection Application." << endl;
    
    do{
        cout << "This is the main menu" << endl;
        cout << "1. Sign in" << endl;
        cout << "2. Clear terminal" << endl;
        cout << "3. Exit" << endl;
        cout << "Select a command:\n>>> ";
        cin >> command;
        if(command == "1"){
            signIn();
        }
        if(command == "2"){
            cout << "Clearing terminal..." << endl;
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            cout << TERMINALCLEAR;
        }
        cout << endl;
    }
    // cin.ignore();
    // getline(cin, command);
    while(command != "3");
    cout << "Thank you for enrolling in the course" << endl;
}




int main(){
    Student s1 = Student("James Lucas", 21, JUNIOR, 75, ENGL, NONE, true, "AB1001");
    sd.addStudent(s1);
    Student s2 = Student("Sandra Henandez", 20, SOPHOMORE, 62, ENGI, COMP, true, "AB1002");
    sd.addStudent(s2);
    Student s3 = Student("Susan Rank", 20, SOPHOMORE, 67, CHEM, COMP, false, "AB1003");
    sd.addStudent(s3);
    Student s4 = Student("James Fisher", 21, JUNIOR, 71, MATH, COMP, false, "AB1004");
    sd.addStudent(s4);
    Student s5 = Student("Jane Fisher", 19, FRESHMAN, 5, CHEM, PHYS, true, "AB1005");
    sd.addStudent(s5);
    Student s6 = Student("Paul Heis", 19, FRESHMAN, 3, MATH, ENGL, true, "AB1006");
    sd.addStudent(s6);
    Student s7 = Student("Jason Palmer", 21, JUNIOR, 77, ENGL, ENGI, false, "AB1007");
    sd.addStudent(s7);
    Student s8 = Student("Chris Rank", 22, SENIOR, 75, PHYS, COMP, false, "AB1008");
    sd.addStudent(s8);
    Student s9 = Student("Top G", 122, SUPERSENIOR, 113, BIOL, NONE, true, "AB1009");
    sd.addStudent(s9);
    Student s10 = Student("Bottom G", 19, FRESHMAN, 10, PHYS, BIOL, false, "AB1010");
    sd.addStudent(s10);

    sd.addCourse(course1);
    sd.addCourse(course2); 

    run();



    return 0;
}
