#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
};

std::vector<TodoTask> Tasks {};
const char delimiter { '~' };

void SaveToFile(std::string filename)
{
}

void LoadFromFile(std::string filename)
{
	std::ifstream input_stream(filename);
	std::vector<std::string> tokens;
	std::string token;

	while(std::getline(input_stream, token, delimiter))
	{
		if(!token.empty())
			tokens.push_back(token);
	}

	for(int i {0}; i < tokens.size() - 5; i += 5)
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

		Tasks.push_back(TodoTask(d, tokens[i+3], tokens[i+4]));
	}
}

int main()
{
	LoadFromFile("ToDo.txt");
	
	for(const auto& task : Tasks)
	{
		task.show();
	}
	return 0;
}
