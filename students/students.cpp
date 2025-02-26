#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define low_factor 0.7
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
	int id;
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

void write_toFile();

int main()
{
	unsigned short capacity = 10;
	unsigned short count = 0;
	unsigned short id_list_size = 0;
	bool exit = false;
	unsigned short option;

	Student* students = new Student[capacity];
	Student student;

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

void write_studetns(Student student)
{
	FILE* file = fopen("students.bin", "wb");


	fwrite(&student, sizeof(Student), 1, file);
	fclose(file);
}

void write_studetns(Student* student, unsigned short count)
{
	FILE* file = fopen("students.bin", "wb");


	fwrite(student, sizeof(Student), count, file);
	fclose(file);
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