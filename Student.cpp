/*
 ============================================================================
 Name        : Student.cpp
 Author      : Romain Dzeinse
 Version     : Original
 Copyright   : Romain Dzeinse Copywrite 2025
 Description : This file includes the definitions of functions and operations
 Date        : 12/30/25
 ============================================================================
*/


#include "Student.h"
// **********************************Student Below*************************************
Student::Student(string name, int age, year_values yv, int creditCount, department_values major, department_values minor, bool accommodate, string id){
    m_name = name;
    m_age = age;
    m_year = yv;
    m_creditCount = creditCount;
    m_major = major;
    m_minor = minor;
    m_accommodations = accommodate;
    m_ID = id;
    m_priorVal = 0;
}
Student::~Student(){
    m_name = "";
    m_age = 0;
    m_ID = "";
    m_year = NONE2;
    m_creditCount = 0;
    m_major = NONE;
    m_minor = NONE;
    m_accommodations = false;
    m_priorVal = 0;

    // cout << "\033[45mSTUDENT DESTRCUTOR called\033[0m" << endl;
}
// **********************************PriorQueue Below*************************************
PriorQueue::PriorQueue(int size){
    if(size > 0){
        m_index = 0;
        m_capacity = size;
        m_priors = new Student*[m_capacity]();
    }
    else{
        m_index = 0;
        m_priors = nullptr;
        m_capacity = 0;
    }
}
PriorQueue::~PriorQueue(){
    if(m_priors){
        for(int i=0; i<m_index; i++){
            if(m_priors[i]){
                delete m_priors[i];
                m_priors[i] = nullptr;
            }
        }
        delete [] m_priors;
        m_priors = nullptr;
    }

    // m_front = nullptr;
    m_index = 0;
    m_capacity = 0;
    // cout << "\033[45mPRIORQUEUE DESTRCUTOR called\033[0m" << endl;
}
bool PriorQueue::empty(){
    return (m_priors[0] == nullptr);
} 
Student* PriorQueue::peek(){
    if(empty()){
        return nullptr;
    }
    return m_priors[0];
}
void PriorQueue::enqueue(string name, int age, year_values yv, int creditCount, department_values major, department_values minor, bool accommodate, string id){
    // displayWaitlist();
    
    if(m_index < m_capacity){
        m_priors[m_index] = new Student(name, age, yv, creditCount, major, minor, accommodate, id);
        m_priors[m_index]->m_priorVal = calculatePrior(yv, creditCount, accommodate, (minor!=NONE)? true:false);
        m_index++;

        mergeSort(m_priors, 0, m_index-1);    // to sort the values in the queue
        // cout << "*************************" << endl;
    }else{
        cout << "\033[41mWaitlist is FULL. Cannot be added on the waitlist\033[0m" << endl;
    }
    // m_front = m_priors[0];
    // cout << "size is:" << m_index << endl;
    // cout << "front: " << m_front->m_name << endl;
}
void PriorQueue::dequeue(){
    if(empty()){
        return;
    }
    Student *temp = m_priors[0];
    for(int i=0; i<m_index-1; i++){ // we iterate until the one before our index
        // we move our next value to the current
        m_priors[i] = m_priors[i+1];
    }
    m_priors[m_index-1] = nullptr;  // we set the current index to nptr
    // m_front = m_priors[0];  // we update our front
    m_index--;
    delete temp;
    temp = nullptr;
}
void PriorQueue::dequeueByIndex(int index){
    if(empty() || index < 0 || index >= m_index) return;

    delete m_priors[index];

    for(int i = index; i < m_index - 1; i++){
        m_priors[i] = m_priors[i + 1];
    }

    m_priors[m_index - 1] = nullptr;
    m_index--;
}
int PriorQueue::getStudent2(string studentID){  // returns the student's index in the priority queue
    if(m_priors[0]->getID() == studentID){ return 0; }
    int index = -1;
    for(int i=0; i<m_index; i++){
        if(m_priors[i] && m_priors[i]->getID() == studentID){
            index = i;
        }
    }
    return index;
}
Student* PriorQueue::getStudent(string id){ // we search by ID bc id's are unique
    for(int i=0; i<m_index; i++){
        if(m_priors[i] && m_priors[i]->m_ID == id){
            // cout << "We found the student in the waitlist" << endl;
            // cout << "We return: " << m_priors[i]->getName() << endl;
            return m_priors[i];
        }
    }
    return nullptr;
}
bool PriorQueue::editStudent(string id, department_values major){
    Student *s = getStudent(id);
    s->m_major = major;
    return true;
}
void PriorQueue::displayWaitlist(){
    if(empty()){ cout << "Waitlist is empty" << endl; }
    for(int i=0; i<m_index; i++){
        cout << "Position:" << i+1 << " || priority:" << m_priors[i]->m_priorVal << "  Name:\033[33m" + m_priors[i]->m_name << "\033[0m  ID:" << m_priors[i]->m_ID << "  year:" << findYearName(m_priors[i]->m_year) << "  Major:" << findMajorName(m_priors[i]->m_major)  << endl;
    }
}
void PriorQueue::merge(Student **arr, int left, int right, int mid){
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student **L = new Student*[n1]();   // we populate with def values (nullptr)
    Student **R = new Student*[n2]();

    for(int i=0; i<n1; i++){    // then we populate with values from arr
        L[i] = arr[left + i];
    }
    for(int j=0; j<n2; j++){
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while(i < n1 && j < n2){
        if (L[i]->m_priorVal >= R[j]->m_priorVal){  // this allows the values to be ordered from max to smallest
            // to change to small to large, just change the comparison operator: from >= to <=
            arr[k++] = L[i++];
        }
        else{
            arr[k++] = R[j++];
        }
    }

    while(i < n1){
        arr[k] = L[i];
        k++; i++;
    }
    while(j < n2){
        arr[k] = R[j];
        k++; j++;
    }

    delete[] L;
    delete[] R;
}
void PriorQueue::mergeSort(Student **arr, int left, int right){
    if(left >= right){ return; }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);
    merge(arr, left, right, mid);
}


// **********************************Course Below*************************************
Course::Course(string name, string prof, string courseID, int credits, department_values dept, int capacity){
    m_name = name;
    m_credits = credits;
    m_dept = dept;
    m_capacity = capacity;
    m_prof = prof;
    m_id = courseID;
    
    m_num = 0;
    m_students = new Student*[m_capacity]();
    m_waitList = nullptr;
}
Course::~Course(){
    if(m_students){
        for(int i=0; i<m_num; i++){
            if(m_students[i]){
                delete m_students[i];
                m_students[i] = nullptr;
            }
        }
        delete [] m_students;
        m_students = nullptr;
    }
    if(m_waitList){
        delete m_waitList;
        m_waitList = nullptr;
    }
    m_name = "";
    m_prof = "";
    m_id = "";
    m_credits = 0;
    m_dept = NONE;
    m_capacity = 0;
    m_num = 0;


    cout << "\033[45mCOURSE DESTRCUTOR called\033[0m" << endl;
}
void Course::size(){
    if(full()){
        cout << "Course is full. " << (m_num) << "/" << m_capacity << " students enrolled" << endl;
        if(m_waitList && !m_waitList->empty())
            cout << "Waitlist is " << UNDERLINE << (m_waitList->m_index) << "/" << m_waitList->m_capacity << CLEARCOLORING << " full." << endl;
    }
    else{
        cout << "There are currently "+ UNDERLINE << (m_num) << "/" << (m_capacity) << " students " + CLEARCOLORING + "enrolled in " + m_name << endl;
    }
}
void Course::displayWaitlist(){
    if(m_waitList){
        m_waitList->displayWaitlist();
        cout << endl;
    }
}
void Course::enroll(string name, int age, year_values yv, int creditCount, department_values major, department_values minor, bool accommodate, string id){
    // cout << "in enroll again" << endl;
    if(!full()){
        // cout << "NOT full here" << endl;
        // cout << "the value of m_num is: " << m_num << endl;
        m_students[m_num] = new Student(name, age, yv, creditCount, major, minor, accommodate, id);
        m_num++;
        cout << "\033[32mYou have been successfully enrolled, " + name + "\033[0m, to " + m_name << endl;
    }
    else{
        if(m_waitList == nullptr){
            m_waitList = new PriorQueue(DEFWAITLIST);
        }
        m_waitList->enqueue(name, age, yv, creditCount, major, minor, accommodate, id);
        cout << "Course is full. You have just been added onto the waitlist" << endl;
        cout << "You are number >>" << m_waitList->m_index << "<< in the waitlist" << endl;
        // m_waitList->displayWaitlist();
    }
}
void Course::displayStudents(){
    cout << endl;
    // cout << "we are here. capa is: " << m_capacity << endl;
    for(int i=0; i<m_capacity; i++){
        if(m_students[i]){
            cout << "______________" << (i+1) << "_______________" << endl;
            cout << "  *|" + m_students[i]->m_ID + "|" << m_students[i]->m_age << "|" << findMajorName(m_students[i]->m_major) + "|*" << endl;
            cout << "  *|" << findYearName(m_students[i]->m_year) << "|" << m_students[i]->m_creditCount << "|" << (m_students[i]->m_accommodations? "ACCOMMOD":"NO ACCOMMOD") << "|*" << endl;
            cout << "   |_______\033[33m" << m_students[i]->m_name << "\033[0m______|          " << endl;
            cout << endl;
        }
    }
    cout << endl;
}
void Course::unEnroll(string studentID){
    /*
        1 - search student, in both course list and waitlist, return true if we find the student
            1.1 - if we find the student in course list, we delete the student and move the other 
                    students from the right, to the left, to fill the gap.
                    1.1.1 if before deletion waitlist was initialized(meaning there were students 
                        in there), after deletion, we must move the student next on the 
                        waitlist and onto the student list at the end. Note: If waitlist was 
                        initialized, automatically, the student list must have already been full
            1.2 - if we find the student in the waitlist, we delete the student and move the other
                    student from the right, to the left, to fill the gap of the waitlist. Just delete
                    the student using the provided delete function from PriorQueue, which takes care 
                    of the second part already.
            1.3 - if we don't find the student either, we return nothing and tell the user the 
                    student was not found in either the student list nor the waitlist
    */
    // 1.
    // cout << "In course ID" << endl;
    int myStudentIndex = getStudent2(studentID); // we get the index
    if(myStudentIndex != -1){  // if we find the student, 1.1. Note: -1 means invalid
        // cout << "This means student is in the student list" << endl;
        bool studentListWasFull = full(); // we check if we were full before deletion
        bool waitlistWasEmpty = waitlistEmpty(); // we check if we waitlist was empty before deletion       
        if(studentListWasFull){
            // cout << "Student List was full" << endl;
        }
        if(!waitlistWasEmpty){
            // cout << "Waitlist was not empty" << endl;
        }
        delete m_students[myStudentIndex];
        m_students[myStudentIndex] = nullptr;
        // cout << TEXTBLUE + "You have been successfully unenrolled from the course" + CLEARCOLORING << endl;
        // move everything to the left 1
        for(int i=myStudentIndex; i<m_num-1; i++){
            m_students[i] = m_students[i+1];
        }
        m_students[m_num-1] = nullptr;
        m_num--;    // we decrement the size, then we call enroll

        // 1.1.1: to know if we were empty before, we check if the waitlist was ever not nptr, meaning initialized, meaning not empty
        if(studentListWasFull && !waitlistWasEmpty){
            // cout << "This means waitlist was NOT empty AND student list WAS full" << endl;
            // if(m_waitList){
            //     cout << "Little if statement: " << m_waitList->m_priors[0]->getName() << endl;
            // }
            Student *myStudent = m_waitList->peek();    // we get the value with the highest priority, 
            // we make sure we get the value not what it points to, because we're about to delete it, in the next line
            Student student = *myStudent;
            // cout << "after we peek" << endl;
            m_waitList->dequeue();  // then we dequeue it
            // cout << "We dequeue and it's fine" << endl;

            enroll(student.m_name, student.m_age, student.m_year, student.m_creditCount, student.m_major, student.m_minor, student.m_accommodations, student.m_ID);
            // cout << "NOW HERE" << endl;
            // cout << "Student from waitlist is:"+myStudent.getName() << endl;
            // cout << "Student at the new position in student list is:"+m_students[m_num-1]->getName() << endl;
        }
        return;
    }
    // 1.2 we check if waitlist is available and that the student is in the waitlist
    if(m_waitList){
        // cout << "This means we are in the waitlist" << endl;
        int positionInWaitlist = m_waitList->getStudent2(studentID);
        if(positionInWaitlist != -1){
            // cout << "Right before we dequeue by index. The index is:" << positionInWaitlist << endl;
            m_waitList->dequeueByIndex(positionInWaitlist);
            cout << TEXTBLUE + "You have been successfully removed from the waitlist" + CLEARCOLORING << endl;
        }
        return;
    }
    cout << HIGHLIGHTRED + "Cannot unenroll, student was not found." + CLEARCOLORING << endl;
}
bool Course::findStudent(string ID){
    if(empty()){ return false; }    // if student list is empty, return false
    // if we're here, there are students
    for(int i=0; i<m_capacity; i++){ // we iterate through the student array
        if(m_students[i] && m_students[i]->m_ID == ID){ // if we have a valid spot, and that valid spot has the same id as ID
            return true;    // we return true, meaning we found the student
        }
    }
    // we didn't find the student in the student array, we now search in waitlist IF waitlist is not nptr
    if(m_waitList){
        // cout << "course is full so we now search in waitlist because waitlist is not nptr" << endl;
        return (m_waitList->getStudent(ID) != nullptr)? true: false; // we find the student in the waitlist. If it's there it'll return a valid student object, else, nptr
    }

    // if we get here that means we didn't find the student, so we return false
    return false;
}
Student* Course::getStudent(string studentID){
    for(int i=0; i<m_capacity; i++){
        if(m_students[i] && m_students[i]->m_ID == studentID){
            return m_students[i];
        }
    }
    return nullptr;
}
int Course::getStudent2(string studentID){ // this function returns the index of the student
    int index = -1;
    for(int i=0; i<m_capacity; i++){
        if(m_students[i] && m_students[i]->m_ID == studentID){  // if we find student, we set index equal to it
            index = i;
            break;  // then we break from the loop
        }
    }
    return index;   // and just return index
}


// **********************************School Directory Below*************************************
SchoolDirectory::SchoolDirectory():m_index(0), m_cap(DEFDIRECTORYSIZE), m_courseCap(DEFCOURSESMAX){
    m_allStudents = new Student*[m_cap]();
    m_courses = new Course*[m_courseCap]();
}
SchoolDirectory::~SchoolDirectory(){
    if(m_allStudents){
        for(int i=0; i<m_index; i++){
            if(m_allStudents[i]){
                delete m_allStudents[i];
                m_allStudents[i] = nullptr;
            }
        }
        delete [] m_allStudents;
        m_allStudents = nullptr;
    }
    m_cap = 0;
    m_index = 0;

    if(m_courses){
        for(int i=0; i<m_courseIndex; i++){
            if(m_courses[i]){
                delete m_courses[i];
                m_courses[i] = nullptr;
            }
        }
        delete [] m_courses;
        m_courses = nullptr;
    }
    m_courseCap = 0;
    m_courseIndex = 0;


    cout << "\033[45mSCHOOL DIRECTORY DESTRCUTOR called\033[0m" << endl;
}
void SchoolDirectory::addStudent(const Student &s){  // const prevents this function from modifying s
    if(m_index >= m_cap){ return; }

    // copy the values of s onto the new student. It allocates it's own memory from the data taken from s  
    m_allStudents[m_index] = new Student(s);
    m_index++;
    // cout << "\033[42mStudent Added\033[0m" << endl;
}
void SchoolDirectory::removeStudent(int index){
    if(index < 0 || index >= m_index) { return; }
    // we delete the value
    delete m_allStudents[index];   // even if the element is nptr, this is okay

    // shift our remaining values in the array left by 1
    for(int i=index; i<m_index-1; i++){
        m_allStudents[i] = m_allStudents[i + 1];
    }

    m_allStudents[m_index - 1] = nullptr;
    m_index--;
}

void SchoolDirectory::addCourse(const Course &c){
    if(m_courseIndex >= m_courseCap){ return; }

    m_courses[m_courseIndex] = new Course(c);
    m_courseIndex++;
}
void SchoolDirectory::removeCourse(int index){
    if(index < 0 || index >= m_courseIndex) { return; }
    delete m_courses[index];

    for(int i=index; i<m_courseIndex-1; i++){
        m_courses[i] = m_courses[i + 1];
    }

    m_courses[m_courseIndex - 1] = nullptr;
    m_courseIndex--;
}

void SchoolDirectory::displaySize(){
    cout << "\033[33mThe Directory holds " << m_index << "/" << m_cap << " students.\033[0m" << endl;
}
bool SchoolDirectory::isStudentThere(string id){
    for(int i=0; i<m_index; i++){
        if(m_allStudents[i]->m_ID == id){
            return true;
        }
    }
    return false;
}
Student* SchoolDirectory::getStudent(string id){
    for(int i=0; i<m_index; i++){
        if(m_allStudents[i]->m_ID == id){
            return m_allStudents[i];
        }
    }
    cout << "Student ID doesn't exist. Try again!" << endl;
    return nullptr;
}
bool SchoolDirectory::isCourseThere(string id){
    for(int i=0; i<m_courseIndex; i++){
        if(m_courses[i]->m_id == id){
            return true;
        }
    }
    return false;
}
Course* SchoolDirectory::getCourse(string id){
    for(int i=0; i<m_courseIndex; i++){
        if(m_courses[i]->m_id == id){
            return m_courses[i];
        }
    }
    cout << "Course ID doesn't exist. Try again!" << endl;
    return nullptr;
}
void SchoolDirectory::viewCourses(){
    if(m_courses){
        cout << "\t\t\t\t\t\033[4mNow viewing courses\033[0m" << endl;
        bool courseFlag = false;
        for(int i=0; i<m_courseCap; i++){
            if(m_courses[i]){
                cout << "\t\t\t\t\t\t\t\tCourse: " + m_courses[i]->getName() + "\n\t\t\t\t\t\t\t\tProfessor: " + m_courses[i]->getProf() 
                + "\n\t\t\t\t\t\t\t\tCourse ID: "+ TEXTGREEN + m_courses[i]->getCourseID() + CLEARCOLORING + "\n\t\t\t\t\t\t\t\tCredits: " + to_string(m_courses[i]->getCredits()) 
                + "\n\t\t\t\t\t\t\t\tDepartment: " <<  findMajorName(m_courses[i]->getDepartment()) << "\n\t\t\t\t\t\t\t\tEnrollement Slots: " 
                + to_string(m_courses[i]->getCap()) << endl << endl;
                courseFlag = true;
            }
        }
        if(courseFlag == false){
            cout << "\nNo courses added" << endl;
        }
        return;
    }
    cout << "No courses" << endl;
}
void SchoolDirectory::viewCourses2(){
    cout << "\033[4mNow viewing courses\033[0m" << endl;
    bool courseFlag = false;
    if(m_courses){
        for(int i=0; i<m_courseCap; i++){
            if(m_courses[i]){
                cout << "Course:" + TEXTGREEN + m_courses[i]->getName() + CLEARCOLORING + "  Professor:" + m_courses[i]->getProf() 
                + "  Course ID:"+ TEXTGREEN + m_courses[i]->getCourseID() + CLEARCOLORING + "  Credits:" + to_string(m_courses[i]->getCredits()) 
                + "  Department:" <<  findMajorName(m_courses[i]->getDepartment()) << "  Enrollement Slots:" 
                + to_string(m_courses[i]->getCap()) << endl << endl;
                courseFlag = true;
            }
        }
        cout << endl;
        if(courseFlag == false){    // this is for the case where we added and then removed the courses
            cout << "\nNo courses added" << endl;
        }
        return;
    }
    cout << "\nNo courses added" << endl;
}
void SchoolDirectory::displayCourseSize(){
    cout << "\033[33mThe Directory holds " << m_courseIndex << "/" << m_courseCap << " courses.\033[0m" << endl;
}


// **********************************Extraneous Below*************************************
int calculatePrior(int year, int credit, bool accomm, bool hasMinor){
    int val1 = 0, val2 = 0;
    if(accomm){
        val1 = 2;
    }
    if(hasMinor){
        val2 = 2;
    }
    return (year + credit + val1 + val2);
}
string findMajorName(department_values types){
    if(types == ENGI){
        return "Engineering";
    }
    else if(types == ENGL){
        return "English";
    }
    else if(types == MATH){
        return "Math";
    }
    else if(types == COMP){
        return "Computer Science";
    }
    else if(types == CHEM){
        return "Chemistry";
    }
    else if(types == BIOL){
        return "Biology";
    }
    else if(types == PHYS){
        return "Physics";
    }
    return "NONE";
}
string findYearName(year_values types){
    if(types == FRESHMAN){
        return "Freshman";
    }
    else if(types == SOPHOMORE){
        return "Sophomore";
    }
    else if(types == JUNIOR){
        return "Junior";
    }
    else if(types == SENIOR){
        return "Senior";
    }
    else if(types == SUPERSENIOR){
        return "Supersenior";
    }
    else if(types == GRAD){
        return "Grad";
    }
    return "NONE";
}
