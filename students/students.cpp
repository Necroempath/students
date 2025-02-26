#define _CRT_SECURE_NO_WARNINGS
#define low_factor 0.7
#define file_name "students.bin"
#include <iostream>

enum MenuOption
{
	SHOW_STUDENTS = 1,
	ADD_STUDENT,
	REMOVE_STUDENTS,
	EXIT
};

struct Adress
{
	char country[20];
	char city[30];
	char street[30];
};

struct Student
{
	Adress adress;
	unsigned short id;
	char name[20];
	unsigned short age;
	float grade;
};

void resizeArray(Student*& students, unsigned short& capacity, const float factor = 1.5);

void printMenu();

void showStudents(const Student* students, unsigned short count);

Student inputStudent();

inline bool addStudent(Student* students, unsigned short& count, const Student student);

bool isValidStudent(const Student* students, unsigned short count, const Student student);

int getID();

bool removeStudent(Student*& students, unsigned short& count, const unsigned short capacity, const int id);

bool write_students(Student student);

bool write_students(Student* students, unsigned short count);

bool read_students(Student* students, unsigned short count);

int get_size();

void restore_students();

int main()
{
	unsigned short capacity = 10;
	bool exit = false;
	unsigned short option;

	Student* students = new Student[capacity];
	Student student;
	//restore_students();
	unsigned short count = get_size() / sizeof(Student);
	read_students(students, count);

	while (!exit)
	{
		printMenu();

		std::cout << "\nOption: ";
		std::cin >> option;
		std::cin.ignore();
		
		switch (option)
		{
		case SHOW_STUDENTS:
			showStudents(students, count);
			break;
		case ADD_STUDENT:
			student = inputStudent();

			if (isValidStudent(students, count, student)) {
				if (count == capacity)
					resizeArray(students, capacity);

				student.id = count;
				addStudent(students, count, student);
				write_students(students, count);

				std::cout << "\nNew student has been added\n\n";
			}
			else {
				std::cout << "\nInvalid student data\n\n";
			}
			break;
		case REMOVE_STUDENTS:
			if (!count)
			{
				std::cout << "No students to remove" << "\n\n";
				break;
			}
			std::cout << "Input ID of student you want to delete: \t";

			if (removeStudent(students, count, capacity, getID())) {

				write_students(students, count);
				std::cout << "Student has been deleted\n";

				if (count < capacity * low_factor)
				{
					resizeArray(students, capacity, low_factor);
				}
			}
			else {
				std::cout << "No student by given id has found\n";
			}
			break;
		case EXIT:
			exit = true;
			break;
		default:
			std::cout << "Ivalid menu choice\n";
			break;
		}
	}

	return 0;
}

void restore_students()
{
	FILE* file = fopen(file_name, "wb");

	if (!file) {
		return;
	}
	Student students[5] = {
		{{"USA", "New York", "5th Avenue"}, 0, "Alice", 20, 4.5},
		{{"Canada", "Toronto", "Bay Street"}, 1, "Bob", 22, 3.8},
		{{"UK", "London", "Baker Street"}, 2, "Charlie", 21, 4.2},
		{{"Germany", "Berlin", "Hauptstrasse"}, 3, "David", 23, 4.7},
		{{"France", "Paris", "Champs-Ulysies"}, 4, "Eve", 19, 4.9}
	};

	fwrite(students, sizeof(Student), 5, file);

	fclose(file);
}

int get_size()
{
	FILE* file = fopen(file_name, "rb");

	if (!file) {
		return -1;
	}

	fseek(file, 0, SEEK_END); //ДА, СВЯТОЙ РАФАЭЛЬ, СОГРЕШИЛ. НО Я МОГУ ВСЕ ОБЪЯСНИТЬ!!!
	int size = ftell(file);
	
	fclose(file);

	return size;
}

bool write_students(Student student)
{
	FILE* file = fopen(file_name, "wb");

	if (!file) {
		return 0;
	}

	fwrite(&student, sizeof(Student), 1, file);
	fclose(file);

	return 1;
}

bool write_students(Student* students, unsigned short count)
{
	FILE* file = fopen(file_name, "wb");

	if (!file) {
		return 0;
	}

	fwrite(students, sizeof(Student), count, file);
	fclose(file);

	return 1;
}

bool read_students(Student* students, unsigned short count)
{
	FILE* file = fopen(file_name, "rb");

	if (!file) {
		return 0;
	}

	fread(students, sizeof(Student), count, file);
	fclose(file);

	return 1;
}

void printMenu()
{
	std::cout << "------MENU------\n";
	std::cout << "1. Show Studens" << '\n';
	std::cout << "2. Add student" << '\n';
	std::cout << "3. Remove student" << '\n';
	std::cout << "4. Exit" << '\n';
}

void resizeArray(Student*& students, unsigned short& capacity, const float factor)
{
	if (factor <= 0)
		return;

	const unsigned int newCapacity = capacity * factor;
	Student* temp = new Student[newCapacity];
	const unsigned int count = capacity < newCapacity ? capacity : newCapacity;

	for (int i = 0; i < count; i++)
	{
		temp[i] = students[i];
	}

	delete[] students;
	students = temp;
	capacity = newCapacity;
}

void showStudents(const Student* students, unsigned short count)
{
	if (!count) {
		std::cout << "No studnets exist" << "\n\n";
		return;
	}

	for (unsigned short i = 0; i < count; i++)
	{
		std::cout << "\n----------N" << i + 1 << "----------" << "\n\n";

		std::cout << "Country:  " << students[i].adress.country << "\n";
		std::cout << "City:\t" << students[i].adress.city << "\n";
		std::cout << "Street:\t" << students[i].adress.street << "\n\n";

		std::cout << "ID:\t" << students[i].id << "\n";
		std::cout << "Name:\t" << students[i].name << "\n";
		std::cout << "Age:\t" << students[i].age << "\n";
		std::cout << "Grade:\t" << students[i].grade << "\n\n";
	}
}

Student inputStudent()
{
	Student student;

	std::cout << "Country: ";
	std::cin >> student.adress.country;
	std::cout << "City: ";
	std::cin >> student.adress.city;
	std::cout << "Street: ";
	std::cin >> student.adress.street;

	std::cout << "Student Name: ";
	std::cin >> student.name;
	std::cout << "Age: ";
	std::cin >> student.age;
	std::cout << "Avr grade: ";
	std::cin >> student.grade;

	std::cin.ignore();

	return student;
}

bool isValidStudent(const Student* students, const unsigned short count, const Student student)
{
	if (strlen(student.adress.country) <= 2 ||
		strlen(student.adress.city) <= 2 ||
		strlen(student.adress.street) <= 2)
		return 0;
	if (strlen(student.name) <= 2 ||
		student.age < 16 || student.age > 60 ||
		student.grade < 0 || student.grade > 10)
		return 0;
	
	return 1;
}

inline bool addStudent(Student* students, unsigned short& count, const Student student)
{ 
	if (!students) {
		return 0;
	}

	students[count++] = student;
	return 1;
}

bool removeStudent(Student*& students, unsigned short& count, const unsigned short capacity, const int id)
{
	if (!students || count == 0) {
		return 0;
	}

	unsigned short index = count; 

	for (unsigned short i = 0; i < count; i++)
	{
		if (students[i].id == id) {
			index = i;
			break;
		}
	}

	if (index == count)
		return 0;

	for (unsigned short i = index; i < count - 1; i++)
	{
		students[index] = students[index + 1];
	}

	count--;
	return 1;
}

int getID()
{
	int id;
	std::cin >> id;
	std::cin.ignore();
	return id;
}