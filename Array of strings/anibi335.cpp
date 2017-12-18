/*****************************************************************************
* TNG033: Dugga 1                                                            *
* Name: Aniisa Bihi                                                          *
* Personal number:19951227-2221                                              *
* ****************************************************************************/

#include <iostream>
#include <string>

using namespace std;


/****************************************
* Class Date                            *
*****************************************/

class Date{

    public:

        //Default constructor
        Date() = default;

        //Creates Date from the values that are given
        Date(int d, int m, int y);

        //Copy constructor
        Date(const Date &d)
        :day(d.day), month(d.month), year(d.year){}

        //Assignment operator
        //Assigns *this with a a new date (D)
        const Date& operator=(const Date &D);

        //operator++, postfix
        const Date& operator++(int);

        //operator++, prefix
        const Date& operator++();

        //operator--, postfix
        const Date operator--(int);

        //operator--, prefix
        const Date& operator--();

        //Operator < that compares two dates to see if one is smaller than the other
        bool operator<(const Date& D) const;

        //Operator > that compares two date to see if one is larger than the other
        bool operator>(const Date& D) const {return (D < *this);}

        //Operator == that compares two dates to see if they're equal to each other
        bool operator==(const Date& D) const;

        //Operator != that compares two dates to see if they're not equal to each other
        bool operator!=(const Date& D) const {return !(*this == D);}

        //Operator <= that compares two dates to see if one is smaller or equal to the other
        bool operator<=(const Date& D) const {return !(D < *this);}

        //Operator >= that compares two dates to see if one is larger or equal to the other
        bool operator>=(const Date& D) const {return !(*this < D);}

        //operator- that returns the number of days between two dates
        int operator-(const Date& D) const;

        //operator+ that returns the date that is n days after the original date
        Date operator+(const int D) const;

        //stream insertion operator
        friend ostream& operator<<(ostream& os, const Date& dt);

        //Default values
        private:
        int day {1};
        int month {1};
        int year {2000};

        //Array that stores the amount of days for each month of the year
        int daysofmonth[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

//Constructor
//Creates Date from the values that are given
Date::Date(int d, int m, int y){
    day = d;
    month = m;
    year = y;
}

//Assignment operator
//Assigns *this with a a new date (D)
const Date& Date::operator=(const Date &D){
    day = D.day; //Assigns day to D.days value
    month = D.month; //Assigns month to D.month value
    year = D.year; //Assigns year to D.year value
    return *this;
}

//operator++, postfix
const Date& Date::operator++(int){
    Date next(day, month, year);
    ++(*this);
    return *this;
}

//operator++, prefix
const Date& Date::operator++(){

    //checks first if the year is a leap year and february, if so it changes the days of the month to 29 and changes date from there
    //code for leap year was found on a c++ forum, http://www.cplusplus.com/forum/general/24335/
    if(this->year % 4 == 0 && this->month == 2)
        {
            if(this->year % 100 == 0)
            {
                if(this->year % 400 == 0)
                {
                    if (this->day < 29)
                        this->day += 1; //next day
                    else
                    {
                        day = 1; //new month, day 1
                        ++month;
                    }
                }
            }
        }

    //if it's not a leap year
    else
        {
            //Matches this months day with the array daysofmonth
           if (this->day < daysofmonth[this->month])
                this->day += 1; //next day
            else {
                day = 1;
                if (month == 12)
                {
                    month = 1;
                    ++year; //new year, first month, day 1
                }
                else
                    ++month; //new month, day 1
            }
        }
    return *this;
}

//operator--, postfix
const Date Date::operator--(int){
    Date temp(day, month, year);
    --(*this);
    return temp;
}

//operator--, prefix
const Date& Date::operator--(){

    //checks first if the year is a leap year and march, if so it changes the days of the february to 29 and changes date from there
    //code for leap year was found on a c++ forum, http://www.cplusplus.com/forum/general/24335/
    if(this->year % 4 == 0 && this->month == 2)
    {
        if(this->year % 100 == 0)
        {
            if(this->year % 400 == 0)
            {
                //leap year gives february 29 days so the day befor 1st of march is 29th of february
                if ((this->day = 1) && (this->month = 3))
                {
                    this->day = 29; //last of february, leap year
                    this->month = 2;
                }
            }
        }
    }

    //The day before
    else if (this->day > 1)
        this->day -= 1;

    else
    {
        //if it's the 1st of january then the day before is 31st of december the year before
        if (this->month == 1)
        {
            this->day = 1;
            --this->year;
        }
        else
            --this->month;
        this->day = daysofmonth[this->month];
    }
    return *this;
}


//Operator <, works for > as well
bool Date::operator<(const Date& D) const{

    //True if this year is less than D's year
    if( this->year < D.year)
    {
        return true;
    }

    //True if it's the same year but this month is less than D's month
    else if((this->year == D.year) && (this->month < D.month))
    {
        return true;
    }

    //True if it's the same year and month but this day is less than D's day
    else if((this->year == D.year) && (this->month == D.month) && (this->day < D.day))
    {
        return true;
    }
    return false;
}

//Operator ==
bool Date::operator==(const Date& D) const
{
    //For the dates to be equal to each other, they have to match in year, month and day
    if((this->year == D.year) && (this->month == D.month) && (this->day == D.day))
       return true;
    return false;
}

//Operator -
int Date::operator-(const Date& D) const{
    //If the dates are equal to each other there's 0 days between them
    if(*this == D) return 0;

    //Date D
    Date temp(D);

    //diff is a counter for the amount of days between the two dates
    int diff = 0;

    //As long as this is less than temp then add on to counter diff until temp = *this
    if(*this < temp)
    {
        while(*this<temp)
        {
          ++diff;
          temp = --temp; //temp changes to the date before until temp = *this
        }
    }

    //As long as this is lager than temp then add on to counter diff until temp=*this
    else if(*this > temp)
    {
        while(*this > temp)
        {
            ++diff;
            temp = ++temp; //temp changes to the date after until temp =*this
        }
    }
    return diff;
}

//Operator +, returns the date d days after
Date Date::operator+(int d) const{

    Date temp(*this);

    //same date
    if(d<0)
    {
        return temp;
    }

    //go on to the next day until d=0 and d days are over
    for(int i=0; i<d; i++)
    {
        temp++;
    }
    return temp;
}

//stream insertion
ostream& operator<<(ostream& os, const Date& dt)
{
    //skriver ut datumet
    os << dt.day << '-' << dt.month << '-' << dt.year;
    return os;
}


/****************************************
* Class Boosted_Array                   *
*****************************************/

class Boosted_Array{

    public:

        //Default constructor
        Boosted_Array();

        //Constructor
        Boosted_Array(const Date d, int n);

        //Instance of the class
        Boosted_Array(const Date d, int n, const string& text);

        //Copy constructor
        Boosted_Array(const Boosted_Array &A);

        //Assignment operator
        const Boosted_Array& operator=(const Boosted_Array &A);

        //Destructor
        ~Boosted_Array() {delete [] arr;}

        //Begin function that returns the start date, first slot in array
        const Date begin() const {return this->date;}

        //End function that returns the end date, last slot in array
        const Date end() const {return this->date + (this->length-1);}

        //Resizes Boosted_array to a given date, number of slots
        Boosted_Array& resize_to(const Date& d);

        //stream insertion (operator<<)
        friend ostream& operator<<(ostream& os, const Boosted_Array& A);

        //Subscript operator []
        string& operator[](const Date& d);
        const string& operator[](const Date& d) const;

        //operator+ that merges two Boosted_arrays
        Boosted_Array operator+(const Boosted_Array& A) const;

        //Private initializer for variables
        private:
            Date date;
            int length;
            string *arr;
            void copy_arr(string *A, int n);
};


//Constructor, assigns dates and n slots in array initialized with ""
 Boosted_Array::Boosted_Array(const Date d, int n)
 {
    date = d;
    length = n;
    arr = new string[n];
    for(int i=0; i<n; i++)
    {
        arr[i] = "";
    }
 }

 //Instance of the class, assigns dates and n slots in array initialized with text
 Boosted_Array::Boosted_Array(const Date d, int n, const string& text)
 {
    date = d;
    length = n;
    arr = new string[n];
    while(n--)
    {
        arr[n] = text;
    }
 }

//copy array function that is called in the copy constructor and the assignment constructor
void Boosted_Array::copy_arr(string *A,int n)
{
    //if string exists then delete it;
    if(arr)
    {
        delete[] arr;
    }
    length = n; //length will get the value of n
    arr = new string[n]; //arr will be a string with n slots

    //while n is decreasing to 0, arr will be copied to A
    while(n--)
    {
        arr[n] = A[n];
    }
}

//Copy constructor
Boosted_Array::Boosted_Array(const Boosted_Array &A)
    : date(A.date), length(A.length), arr(nullptr)
{
    copy_arr(A.arr, A.length);
}

//Assignment operator
const Boosted_Array& Boosted_Array::operator=(const Boosted_Array &A)
{
    //assigns given name and uses copy_arr for assignment
    date = A.date;
    copy_arr(A.arr, A.length);
    return *this;
}

//Resizes Boosted_array to a given date, number of slots
Boosted_Array& Boosted_Array::resize_to(const Date& d)
{
    //same size, no resize
    if(d <= this->date)
    {
        return *this;
    }

    int new_size = 1+(d-this->date);
    int n = this->length; //how many slots
    string *temp = this->arr; //array slots
    this->arr = new string[new_size]; //assigns new_size

    while(n--)
    {
        this->arr[n] = temp[n];
    }
    delete[] temp;

    //if the new size has more slots than this has, then initializes the remaining new slots with ""
    if(new_size > this->length)
    {
        for(n = this->length; n<new_size; ++n)
        {
            this->arr[n] = "";
        }
    }
    this->length = new_size;
    return *this;
}


//Subscript operator [] that returns a array without it's first date(first slot)
//Returns a reference that can be changed
string& Boosted_Array::operator[](const Date& d)
{
    if((d<this->begin()) || (d>this->end()))
    {
        cout << "Out of range" << endl;
    }
    //Index is the array without this first date
    int index = d-this->begin();
    return this->arr[index]; //return the array without this first date
}

//Subscript operator [] that returns a array without it's first date(first slot)
//Returns a const that can't be changed
const string& Boosted_Array::operator[](const Date& d) const
{
   if(d<this->begin() ||  d>this->end())
    {
        cout << "Out of range" << endl;
    }
    int index = d-this->begin();
    return this->arr[index];
}

//Merges two Boosted_Arrays together
Boosted_Array Boosted_Array::operator+(const Boosted_Array& A) const
{
    Date start = min(this->begin(), A.begin()); //takes the oldest date between the two and assigns it to start
    Date last = max(this->end(), A.end()); //takes the newest date between the two and assigns it to end
    Boosted_Array temp(start, 1 + (last-start)); // dates start from start and are assigned number of slots

    //while start is <= to last
    for(; start <= last; ++start)
    {
        string t = "";
        //merges the two arrays together while checking if some slots have the same date
        if (start >= this->begin() && start <= this->end())
            t += (*this)[start];
        if (start >= A.begin() && start <= A.end())
            t += A[start];
        temp[start] = t;
    }
    return temp;
}

//Stream insertion
ostream& operator<<(ostream& os, const Boosted_Array& A)
{
    auto end = A.end();
    for(Date d = A.begin(); d <= end; ++d)
    {
        os << "[" << d  << "]" << " = " << A[d] << endl;
    }
    return os;
}

/***************************************
* MAIN: Please, deliver unmodified     *
****************************************/

int main()
{
    /*************************************
    * TEST PHASE 0                       *
    * Constructor and operator<<         *
    **************************************/

    cout << "\nTEST PHASE 0\n" << endl;

    Boosted_Array AA0(Date(), 2);

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    Boosted_Array AA1(Date(1,6, 2016), 11, "Hej!! ");

    cout << "\n** AA1 ** " << endl;
    cout << AA1 << endl;

    /*************************************
    * TEST PHASE 1                       *
    * Copy constructor                   *
    **************************************/
    cout << "\nTEST PHASE 1\n" << endl;

    const Boosted_Array AA2(AA1);

    cout << "** AA2 ** " << endl;
    cout << AA2 << endl;

    /*************************************
    * TEST PHASE 3                       *
    * Assignment operator                *
    **************************************/
    cout << "\nTEST PHASE 3\n" << endl;

    AA0 = AA2;

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    /*************************************
    * TEST PHASE 4                       *
    * Subscript operator: operator[]     *
    * begin(), end()                     *
    **************************************/
    cout << "\nTEST PHASE 4\n" << endl;

    AA0[Date(1,6,2016)] = "Anna's birthday";
    AA0[Date(2,6,2016)] = "Ole's birthday";
    AA0[Date(3,6,2016)] = "Justin's birthday";
    AA0[Date(4,6,2016)] = "Ruth's birthday";
    AA0[Date(5,6,2016)] = "Rita's birthday";
    AA0[Date(6,6,2016)] = "Shop socks";
    AA0[Date(7,6,2016)] = "Cinema";
    AA0[Date(8,6,2016)] = "Reading";
    AA0[Date(9,6,2016)] = "Grade duggor";
    AA0[Date(10,6,2016)] = "Party";
    AA0[Date(11,6,2016)] = "Party";

    cout << "** AA0 ** " << endl;
    for(Date d = AA0.begin(); d <= AA0.end(); ++d)
    {
        cout << AA0[d] << endl;
    }

      //Test if it works
//    for(Date d = AA0.begin(); d <= AA0.end(); d++)
//    {
//        cout << AA0[d] << endl;
//    }

    cout << endl << AA2[Date(5,6,2016)] << endl;

    /*************************************
    * TEST PHASE 5                       *
    * operator+                          *
    **************************************/
    cout << "\nTEST PHASE 5\n" << endl;

    cout << "** AA2 + AA0 ** " << endl;
    cout << AA2 + AA0 << endl;

    /*************************************
    * TEST PHASE 6                       *
    * resize_to()                        *
    **************************************/
    cout << "\nTEST PHASE 6\n" << endl;

    AA0.resize_to(Date(14,6,2016));

    AA0[Date(12,6,2016)] = "Lecture in TNG033";
    AA0[Date(13,6,2016)] = "Lecture in TND012";
    AA0[Date(14,6,2016)] = "Read exjobb";

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    //Note: first resize_to should not modify AA0
    AA0.resize_to(Date(1,1,2015)).resize_to(Date(15,6,2016));

    AA0[Date(15,6,2016)] = "Holiday";

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    /*************************************
    * TEST PHASE 7                       *
    * operator+                          *
    **************************************/

    Boosted_Array AA4(Date(12,6,2016), 14, "<--");

    AA1 = AA0 + AA4;

    cout << "\n** AA1 ** " << endl;
    cout << AA1 << endl;


    cout << "\nBye ..." << endl;

    return 0;
}
