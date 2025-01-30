#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define FILE "ToDo.txt"

struct GenericDate
{
	short day, month;
	int year;
	GenericDate() = default;
	GenericDate(short d, short m, int y)
		: day(d), month(m), year(y) {}
};

class TodoTask
{
	private:
		GenericDate created_date;
		std::string task;
		std::string description;
		bool is_completed {false};

	public:
		TodoTask() = default;

		TodoTask(GenericDate date, std::string t, std::string desc = "")
			: created_date(date), task(t), description(desc)
		{
		}

		const GenericDate getDateTime() const { return created_date; }
		const std::string getTask() const { return task; }
		const std::string getDesc() const { return description; }
		const bool getCompleted() const { return is_completed; }

		void show() const
		{
			std::cout << "\n";

			if(!is_completed)
			{
				std::cout << "Task: " << task;
				std::cout << "\nDesc: " << description;
				std::cout << "\nDate: " << created_date.day << "-" << created_date.month << "-" << created_date.year << "\n";
			}

			std::cout << "\n";
		}

		void setCompleted(bool status) { is_completed = status; }
};

std::vector<TodoTask> tasks {};
const char delimiter { '~' };

void SaveToFile(std::string filename)
{
	std::ofstream output_stream(filename);
	std::string write {};

	for(const auto& task : tasks)
	{
		if(!task.getCompleted())
		{
			write.append(std::to_string(task.getDateTime().year));
			write.append("~");
			write.append(std::to_string(task.getDateTime().month));
			write.append("~");
			write.append(std::to_string(task.getDateTime().day));
			write.append("~");
			write.append(task.getTask());
			write.append("~");
			write.append(task.getDesc());
			write.append("~");
		}
	}
	output_stream << write;
}

void LoadFromFile(std::string filename)
{
	std::ifstream input_stream(filename);
	std::vector<std::string> tokens;
	std::string token;

	while(std::getline(input_stream, token, delimiter))
	{
		tokens.push_back(token);
	}

	for(int i {0}; i < tokens.size() - 4; i += 5)
	{
		GenericDate d;
		try {
			d.year = std::stoi(tokens[i]);
			d.month = std::stoi(tokens[i+1]);
			d.day = std::stoi(tokens[i+2]);
		} catch (const std::invalid_argument &e) {
			std::cerr << "Invalid argument: " << e.what() << " - Token: [" << tokens[i] << "]\n";
		} catch (const std::out_of_range &e) {
			std::cerr << "Out of range error: " << e.what() << " - Token: [" << tokens[i] << "]\n";
		}

		tasks.push_back(TodoTask(d, tokens[i+3], tokens[i+4]));
	}
}

void ShowTask()
{
	for(const auto& task : tasks)
	{
		task.show();
	}

	std::cin.ignore(); std::cin.get();
}

void AddTask()
{
	GenericDate date;
	std::cout << "Date: ";
	std::cin >> date.day >> date.month >> date.year; std::cin.ignore();
	
	std::string task;
	std::cout << "Task: ";
	std::getline(std::cin, task);

	std::string desc;
	std::cout << "Desc: ";
	std::getline(std::cin, desc);

	tasks.push_back(TodoTask(date, task, desc));
}

void MarkAsDone()
{
	std::cout << "Task List\n";
	for(int i {0}; i < std::size(tasks); i++)
	{
		std::cout << i + 1 << " - " << tasks[i].getTask() << '\n';
	}

	int input;
	std::cout << "\n> Ketikkan pilihan Anda: ";
    std::cin >> input;

	if(input < std::size(tasks) && input >= 0)
		tasks[input-1].setCompleted(true);
}

void Menu()
{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif

	std::cout << "MENU\n";
	std::cout << "1. Add\n";
	std::cout << "2. Show\n";
	std::cout << "3. Mark as Done\n";

	std::string input;
	std::cout << "\n> Ketikkan pilihan Anda: ";
    std::cin >> input;

	switch(std::stoi(input))
	{
		case 1:
			AddTask();
			break;
		case 2:
			ShowTask();
			break;
		case 3:
			MarkAsDone();
			break;
		case 4:
			SaveToFile(FILE);
			exit(0);
			break;
		default:
			std::cout << "Invalid!\n";
			std::cin.get();
	}
}

int main()
{
	LoadFromFile(FILE);
	while(true)
		Menu();
	return 0;
}
