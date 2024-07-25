#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define DOB_FORMAT "%Y-%m-%d"

enum Columns { ID = 0, NAME, DOB, ROLL_NO, DEPARTMENT, ADDRESS };
enum SortBy { SortByAge = 0, SortByName };
class Student {
private:
  int m_Id, m_RollNo;
  float m_Age;
  std::string m_Name, m_DOB, m_Department, m_Address;

  /*
   * Parses input string as row with format:
   * id, name,date_birth,roll_no,department,address
   * eg: 123,Sandesh,2020-01-02,456,Computer Science,"Sorakhutte,Kathmandu"
   **/
  void ParseRow(std::string row) {
    std::string buffer;
    int col = ID;
    bool inside_quotes = false;

    for (const char c : row) {
      // toogle flag to see if input chars are inside quotes
      if (c == '"' || c == '\'') {
        inside_quotes = !inside_quotes;
        continue;
      }

      if (inside_quotes) {
        buffer += c;
      } else if (c != ',' && !inside_quotes) {
        buffer += c;
      } else {
        switch (col) {
        case ID:
          m_Id = std::stoi(buffer);
          break;
        case NAME:
          m_Name = buffer;
          break;
        case DOB:
          m_DOB = buffer;
          break;
        case ROLL_NO:
          m_RollNo = std::stoi(buffer);
          break;
        case DEPARTMENT:
          m_Department = buffer;
          break;
        case ADDRESS:
          m_Address = buffer;
          break;
        }
        buffer.clear();
        col++;
      }
    }
    // avoid last being field will be missed if there is no trailing comma in
    // the end
    if (col == ADDRESS && !buffer.empty()) {
      m_Address = buffer;
    }
  }

  void calculateAge(const std::string dob) {

    struct tm tm_DOB = {0}, *tm_Now;
    std::time_t time_DOB, time_Now;

    // extract DOB as struct tm
    std::istringstream DOB_Stream(dob);
    DOB_Stream >> std::get_time(&tm_DOB, DOB_FORMAT);

    // get current time as struct tm
    time_Now = time(nullptr);
    tm_Now = localtime(&time_Now);

    float age = ((float)(tm_Now->tm_year * 12 + tm_Now->tm_mon) -
                 (float)(tm_DOB.tm_year * 12 + tm_DOB.tm_mon)) /
                12;
    m_Age = age;
  }

public:
  Student() {}
  Student(std::string row) {
    ParseRow(row);
    calculateAge(m_DOB);
  }

  void Print() const {
    std::cout << m_Id << "\t" << m_Name << "\t" << m_DOB << "\t" << m_RollNo
              << "\t" << m_Department << "\t" << m_Address << std::endl;
  }
  int GetId() const { return m_Id; }
  std::string GetName() const { return m_Name; }
  std::string GetDOB() const { return m_DOB; }
  int GetRoll() const { return m_RollNo; }
  std::string GetDepartment() const { return m_Department; }
  std::string GetAddress() const { return m_Address; }
  float GetAge() const { return m_Age; }
};

class CSVFile {
private:
  std::vector<Student> m_Rows;
  std::ifstream m_InputStream;
  std::string m_FileName;
  int m_Size;

  void QuickSort(int lb, int ub, SortBy option) {
    if (lb >= ub)
      return;
    int j = Partitoner(lb, ub, option);
    QuickSort(lb, j - 1, option);
    QuickSort(j + 1, ub, option);
  }

  /*
   * Quick Sort Partitoner Algorithm
   * sorts the pivot to it's place and returns it's position in sorted list
   */
  int Partitoner(int lb, int ub, SortBy option) {
    int left_cursor, right_cursor;
    Student temp, pivot;
    left_cursor = lb;
    right_cursor = ub;

    // using first element in the list as pivot
    pivot = m_Rows[lb];

    bool ShouldIncrementLeftCursor, ShouldDecrementRightCursor;

    while (left_cursor < right_cursor) {
      if (option == SortByAge) {
        ShouldIncrementLeftCursor =
            m_Rows[left_cursor].GetAge() <= pivot.GetAge() && left_cursor < ub;
        ShouldDecrementRightCursor =
            m_Rows[right_cursor].GetAge() > pivot.GetAge() && right_cursor > lb;

      } else {
        ShouldIncrementLeftCursor =
            m_Rows[left_cursor].GetName() <= pivot.GetName() &&
            left_cursor < ub;
        ShouldDecrementRightCursor =
            m_Rows[right_cursor].GetName() > pivot.GetName() &&
            right_cursor > lb;
      }
      while (ShouldIncrementLeftCursor) {
        left_cursor++;

        if (option == SortByAge) {
          ShouldIncrementLeftCursor =
              m_Rows[left_cursor].GetAge() <= pivot.GetAge() &&
              left_cursor < ub;
        } else {
          ShouldIncrementLeftCursor =
              m_Rows[left_cursor].GetName() <= pivot.GetName() &&
              left_cursor < ub;
        }
      }

      while (ShouldDecrementRightCursor) {
        right_cursor--;

        if (option == SortByAge) {
          ShouldDecrementRightCursor =
              m_Rows[right_cursor].GetAge() > pivot.GetAge() &&
              right_cursor > lb;
        } else {
          ShouldDecrementRightCursor =
              m_Rows[right_cursor].GetName() > pivot.GetName() &&
              right_cursor > lb;
        }
      }

      if (left_cursor < right_cursor) {
        temp = m_Rows[left_cursor];
        m_Rows[left_cursor] = m_Rows[right_cursor];
        m_Rows[right_cursor] = temp;
      }
    }
    m_Rows[lb] = m_Rows[right_cursor];
    m_Rows[right_cursor] = pivot;
    return right_cursor;
  }

public:
  CSVFile(std::string filename) : m_FileName(filename) {
    m_InputStream.open(filename);
    std::string line;
    // Skipping first row
    std::getline(m_InputStream, line);
    line.clear();

    // reading all rows from the CSV
    while (std::getline(m_InputStream, line)) {
      m_Rows.push_back(Student(line));
    }
    m_Size = m_Rows.size();
  }

  ~CSVFile() {
    if (m_InputStream.is_open()) {
      m_InputStream.close();
    }
  }

  /*
   * Writes all the contents of m_Rows in file filename
   */
  void Export(std::string filename) {
    std::ofstream OutputStream;
    OutputStream.open(filename);
    // writing column headers for CSV
    const std::string header = "id,name,date_birth,roll_no,department,address";
    OutputStream << header << std::endl;

    for (const Student std : m_Rows) {
      OutputStream << std.GetId() << "," << std.GetName() << "," << std.GetDOB()
                   << "," << std.GetRoll() << "," << std.GetDepartment() << ","
                   << std.GetAddress() << std::endl;
    }

    OutputStream.close();
  }
  /*
   * Sorts the contents of m_Rows based on option.
   * option can be either of SortByAge or SortByName
   */
  void Sort(SortBy option) { QuickSort(0, m_Size - 1, option); }

  /*
   * Prints the CSVFile to the console
   */
  void Print() {
    const std::string header = "id,name,date_birth,roll_no,department,address";
    std::cout << header << std::endl;
    for (Student std : m_Rows) {
      std.Print();
    }
  }
};

void PrintHelp();

int main(int argc, char **argv) {
  std::string filename;
  int SortOption = 0;

  // If cli args are passed then use them as:
  // csvsort FILENAME SORTBY
  // eg: csvsort input.csv age
  if (argc > 2) {
    filename = argv[1];
    std::string sort_opt = argv[2];
    if (sort_opt == "age") {
      SortOption = 1;
    } else if (sort_opt == "name") {
      SortOption = 2;
    } else if (sort_opt == "both") {
      SortOption = 3;
    }
  } else {
    // Prompt for input if no cli args are passed
    std::cout << "Enter input file: ";
    std::cin >> filename;
    std::cout << "Sort by: " << std::endl;
    std::cout << "\t1. Age" << std::endl;
    std::cout << "\t2. Name" << std::endl;
    std::cout << "\t3. Both" << std::endl;
    std::cout << "Choice(1-3): ";
    std::cin >> SortOption;
  }

  if (SortOption < 1 || SortOption > 3) {
    std::cout << "Invalid Choice!";
    return 1;
  }
  CSVFile file = CSVFile(filename);
  if (SortOption == 1 || SortOption == 3) {
    file.Sort(SortByAge);
    file.Export("SortedByAge.csv");
    std::cout << "Sorting by age successfull." << std::endl;
  }
  if (SortOption == 2 || SortOption == 3) {
    file.Sort(SortByName);
    file.Export("SortedByName.csv");
    std::cout << "Sorting by name successfull." << std::endl;
  }

  return 0;
}

void PrintHelp() {
  // TODO: Complete this
}
