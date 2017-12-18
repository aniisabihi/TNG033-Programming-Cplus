/************************************************
* TNG033: Dugga 2                               *
* Name: Aniisa Bihi                             *
* Personal number: 9512272221                   *
* ***********************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <String>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <utility>

using namespace std;

//map template argument
typedef map<string, set<string> > maps;

/********************
* Class Exam        *
*********************/

class Exam{

public:

    //Default constructor
    Exam();

    //Copy constructor
    Exam(const Exam& C);

    //Assignment operator
    const Exam& operator=(const Exam &E);

    //Get function for student's name
    string get_name(){return name;}

    //A function get_total_points
    int get_total_points(int n) const;

    //Default values
private:
    string name;
    vector<int> points;

    //stream extraction operator
    friend istream& operator >>(istream& is, Exam& E);

    //stream insertion operator
    friend ostream& operator <<(ostream& os, const Exam& E);

};

/*************************************
* Class Exam                         *
* Member functions implementation    *
**************************************/

//Default values
Exam::Exam()
{
    name = " ";
    points.clear();
}

//Copy constructor
Exam::Exam(const Exam& C)
{
    name = C.name;
    points = C.points;
}

//Assignment operator
const Exam& Exam::operator=(const Exam &E)
{
    name = E.name;
    points = E.points;
    return *this;
}

//Get total points from exam
int Exam::get_total_points(int n) const
{
    int sum = 0;

    if(n>=0)
    {
        //using std::for_each, found on http://stackoverflow.com/questions/3221812/sum-of-elements-in-a-stdvector
        for_each(points.begin(), points.end(), [&](int m)
                 {
                     if(m>=n)
                        sum += m;
                 });
    }
    return sum;
}

//stream extraction
istream& operator>>(istream& is, Exam& E)
{
    //get line
    string line;
    getline(is,E.name);
    getline(is,line);
    stringstream iss(line);

    istream_iterator<int> iteratorIn((iss));
    istream_iterator<int> iteratorInEnd;
    E.points.clear();

    copy(iteratorIn, iteratorInEnd, back_inserter(E.points));

    return is;
}

//stream insertion
ostream& operator<<(ostream& os,  const Exam& E)
{
    os << E.name << endl;
    copy(E.points.begin(), E.points.end(), ostream_iterator<int>(os, " "));

    return os;
}


/*****************************
* Class Exam_Grader          *
******************************/

class Exam_Grader{

public:

    //Destructor
    virtual ~Exam_Grader();

    //Constructor
    explicit Exam_Grader(string examinersName, istream& is);

    //Copy constructor set to delete so you cant make copies of instances of the class
    Exam_Grader(const Exam_Grader& E) = delete;

    //returns exam codes
    unsigned get_code(){return Code;}

    //grades reports
    virtual void do_grading_report() = 0;

    //writes reports
    ostream& write_grading_report(ostream& os);

    //how many students got each grade
    int number_of_students(const string& total);


protected:

    //Grades, examinator, exam codes
    const string F = "F";
    const string three = "3";
    const string four = "4";
    const string five = "5";
    const string examinator;
    static unsigned int examCode;
    unsigned int Code;
    maps grades;

    //Vector with students
    vector<Exam> eachStudent;
    istream& in;
    friend ostream& operator<<(ostream& os, const Exam_Grader& E);
};

/*************************************
* Class Exam_Grader                  *
* Member functions implementation    *
**************************************/

//Virtual Destructor
Exam_Grader::~Exam_Grader(){}

//Starts examCode from 0
unsigned int Exam_Grader::examCode = 0;

//Initializes examinator and in, reads how many exams there are and counts them
Exam_Grader::Exam_Grader(string examinersName, istream& is)
    :examinator(examinersName), in(is)
{
    istream_iterator<Exam> E(is);
    istream_iterator<Exam> E_end;
    copy(E, E_end, back_inserter(eachStudent));
    ++examCode;
    Code = examCode;
}

//Writes out the grades for number of students
ostream& Exam_Grader::write_grading_report(ostream& os)
{
    os << "Grading report: " << examCode;

    if((grades.size()))
    {
        for_each(grades.begin(), grades.end(), [&] (pair<string, set<string>> total)
        {
            os << "\n" "*** Grade: " << total.first << ":" << number_of_students(total.first) << " student(s)" << endl;
            copy(grades[total.first].begin(), grades[total.first].end(), ostream_iterator<string>(os, "\n"));
        });
    }
    else
        os << " <--- not created!!" << endl;

    return os;
}

//counts how many students
int Exam_Grader::number_of_students(const string& total)
{
    unsigned int number = 0;

    if(number < total.size()){number = grades[total].size();}

    return number;
}

//stream insertion
ostream& operator<<(ostream& os, const Exam_Grader& E)
{
    os << "Examiner: " << E.examinator << endl;
    os << "Exam's code: " << E.examCode << endl;
    os << "*** Students' exams **" << endl;
    copy(E.eachStudent.begin(), E.eachStudent.end(), ostream_iterator<Exam>(os, "\n"));

    return os;
}


/****************************************
* Class Exam_Grader_Level1              *
*****************************************/

class Exam_Grader_Level1: public Exam_Grader
{
public:

    //Constructor
    Exam_Grader_Level1(const string& name, ifstream& is, int b = 0)
        :Exam_Grader(name, is), bonusPoints(b){};

    //set the bonus function
    int set_bonus(const int bonus);

    //get the bonus function
    int get_bonus() const;

    //grading the exams
    void do_grading_report();


private:
    int bonusPoints;
};

/*************************************
* Class Exam_Grader_Level1           *
* Member functions implementation    *
**************************************/

//Sets the bonus to the given integer "bonus"
int Exam_Grader_Level1::set_bonus(const int bonus)
{
    return bonusPoints = bonus;
}

//Gets the bonus value
int Exam_Grader_Level1::get_bonus() const
{
    return bonusPoints;
}

//Gives out grade depending on sum of points
void Exam_Grader_Level1::do_grading_report()
{
    for_each(eachStudent.begin(), eachStudent.end(), [&] (Exam E)
         {
             if(E.get_total_points(0)+get_bonus() >= 20){grades[five].insert(E.get_name());}
             else if(E.get_total_points(0)+get_bonus() >= 15){grades[four].insert(E.get_name());}
             else if(E.get_total_points(0)+get_bonus() >= 10){grades[three].insert(E.get_name());}
             else grades[F].insert(E.get_name());

         });
}

/****************************************
* Class Exam_Grader_Level2              *
*****************************************/

class Exam_Grader_Level2 : public Exam_Grader
{

public:

    //Constructor
    Exam_Grader_Level2(const string& name, ifstream& is, int m = 2)
        :Exam_Grader(name, is), minimumNumbers(m){};

    //Get function for minimum number of points
    int get_min_points() const;

    //Grading the exams that ovverides the past one in level1
    void do_grading_report() override;

private:

    //Minimum number of Points
    int minimumNumbers;
};

/*************************************
* Class Exam_Grader_Level2           *
* Member functions implementation    *
**************************************/

//Gets the minimum number of points
int Exam_Grader_Level2::get_min_points() const
{
    return minimumNumbers;
}

//Gives out grade depending on sum of points
void Exam_Grader_Level2::do_grading_report()
{

    for_each(eachStudent.begin(), eachStudent.end(), [&] (Exam E)
         {
             if(E.get_total_points(minimumNumbers) >= 25){grades[five].insert(E.get_name());}
             else if(E.get_total_points(minimumNumbers) >= 18){grades[four].insert(E.get_name());}
             else if(E.get_total_points(minimumNumbers) >= 10){grades[three].insert(E.get_name());}
             else grades[F].insert(E.get_name());
         });
}


/*******************************
* MAIN: deliver unmodified     *
********************************/

int main()
{
    /*************************************
    * TEST PHASE 0                       *
    * Class Exam                         *
    **************************************/
    cout << "\nTEST PHASE 0\n" << endl;

    ifstream in_File("tentor1.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam e;

    in_File >> e; //read first exam's data from file

    cout << e << endl;

    cout << e.get_name() << "\t"
         << "Sum points = " << e.get_total_points(0) << endl;

    in_File.close();


    /*************************************
    * TEST PHASE 1                       *
    * Exam_Grader_Level1                 *
    **************************************/
    cout << "\nTEST PHASE 1\n" << endl;

    in_File.open("tentor1.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level1 g1("Aida Nordman", in_File, 1);

    cout << g1 << endl;

    in_File.close();

    cout << "Grading with bonus = " << g1.get_bonus()
         << "p ..." << endl;
    cout << "===============================\n\n";

    g1.do_grading_report();
    g1.write_grading_report(cout);


    /*************************************
    * TEST PHASE 3                       *
    * Exam_Grader_Level2                 *
    **************************************/
    cout << "\nTEST PHASE 3\n" << endl;

    in_File.open("tentor2.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level2 g2("Jens Tor", in_File, 3);

    cout << g2 << endl;

    in_File.close();

    cout << "Grading with min points = " << g2.get_min_points()
         << " ..." << endl;
    cout << "==============================="  << endl;

    g2.do_grading_report();
    g2.write_grading_report(cout);

    /*************************************
    * TEST PHASE 4                       *
    * Exam_Grader_Level2                 *
    **************************************/
    cout << "\nTEST PHASE 4\n" << endl;

    in_File.open("tentor3.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level2 g3("Nicklas Lundin", in_File);

    cout << g3 << endl;

    in_File.close();

    //Note: no grading report created for g3

    g3.write_grading_report(cout);

    /*************************************
    * TEST PHASE 5                       *
    * Exam_Grader::number_of_students    *
    **************************************/
    cout << "\nTEST PHASE 5\n" << endl;

    cout << "Exam Code " << setw(10) << "Grade 5 "
         << setw(10) << "Grade 4 "
         << setw(10) << "Grade 3 "
         << setw(10) << "Grade F " << endl;

    cout << setw(5) << g1.get_code()
         << setw(10) << g1.number_of_students("5")
         << setw(10) << g1.number_of_students("4")
         << setw(10) << g1.number_of_students("3")
         << setw(10) << g1.number_of_students("F") << endl;

    cout << setw(5) << g2.get_code()
         << setw(10) << g2.number_of_students("5")
         << setw(10) << g2.number_of_students("4")
         << setw(10) << g2.number_of_students("3")
         << setw(10) << g2.number_of_students("F") << endl;

    //Note: no grading report created for g3
    cout << setw(5) << g3.get_code()
         << setw(10) << g3.number_of_students("5")
         << setw(10) << g3.number_of_students("4")
         << setw(10) << g3.number_of_students("3")
         << setw(10) << g3.number_of_students("F") << endl;

    cout << "\nBye ..." << endl;

    return 0;
}



