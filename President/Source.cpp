#include <iostream>
#include <time.h>
#include <string>
#include <string.h>
#include <fstream>
namespace Time
{
	std::string getDate()
	{
		time_t now(time(0));
		tm new_time;
		localtime_s(&new_time, &now);
		return std::to_string(new_time.tm_mday) + '/' + std::to_string(new_time.tm_mon + 1) + '/' +
			std::to_string(new_time.tm_year + 1900) + ' ' + std::to_string(new_time.tm_hour) + ':' + std::to_string(new_time.tm_min);
	}
}
#define file_name "votes.txt"
class President
{
	std::string name;
	std::string country;
	size_t workPractise;
public:
	President() : name(""), country(""), workPractise(0) {}

	President(const std::string& name, const std::string& country, const size_t& workPractise)
	{
		setName(name);
		setCountry(country);
		setWorkPractise(workPractise);
	}

	void setName(const std::string& name) { this->name = name; }
	std::string getName() const { return this->name; }
	void setCountry(const std::string& country) { this->country = country; }
	std::string getCountry() const { return this->country; }
	void setWorkPractise(const size_t& workPractise) { this->workPractise = workPractise; }
	size_t getWorkPractise() const { return this->workPractise; }

	void showAllVotes() const
	{
		std::cout << readVotes() << std::endl;
	}

	std::string readVotes() const
	{
		std::ifstream fin(file_name);

		std::string text;
		if (fin.is_open())
		{

			while (!fin.eof())
			{
				std::string line;
				std::getline(fin, line);
				text.append(line + "\n");
			}
		}

		fin.close();
		return text;
	}
};
struct Candidate
{
	President* candidate;
	size_t* voteCount;
};
class Person
{
	std::string name;
	std::string surname;
	size_t age;
	std::string speciality;
	std::string sendVoteDateTime;
	Candidate* candidate;
public:
	static Candidate** candidates;
	static size_t candidate_count;

	Person() :name(""), surname(""), age(0), speciality(""),
		sendVoteDateTime(""), candidate(NULL) {}

	Person(const std::string& name, const std::string& surname, const size_t& age, const std::string& speciality)
	{
		setName(name);
		setSurname(surname);
		setAge(age);
		setSpeciality(speciality);
	}

	void setName(const std::string& name) { this->name = name; }
	std::string getName() const { return this->name; }
	void setSurname(const std::string& surname) { this->surname = surname; }
	std::string getSurame() const { return this->surname; }
	void setSpeciality(const std::string& speciality) { this->speciality = speciality; }
	std::string getSpeciality() const { return this->speciality; }
	void setAge(const size_t& age) { this->age = age; }
	size_t getAge() const { return this->age; }
	void setVotedDateTime() { this->sendVoteDateTime = Time::getDate(); }
	std::string getVotedDateTime() const { return this->sendVoteDateTime; }

	void sendVote(President* candidate)
	{
		if (this->candidate == NULL)
		{
			if (checkCandidateInList(candidate))
			{
				Candidate* item = getCandidate(candidate);

				(*item->voteCount)++;
				this->candidate = new Candidate;
				this->candidate->candidate = item->candidate;
				this->candidate->voteCount = item->voteCount;
			}
			else
			{
				addCandidateToList(candidate);
				Candidate* item = getCandidate(candidate);

				this->candidate = new Candidate;
				this->candidate->candidate = item->candidate;
				this->candidate->voteCount = item->voteCount;
			}
			setVotedDateTime();
			writeToFile();
		}
	}
	bool checkCandidateInList(President* candidate) const
	{
		for (size_t i = 0; i < candidate_count; i++)
		{
			if (candidates[i]->candidate == candidate)
				return true;
		}
		return false;
	}
	void addCandidateToList(President* candidate)
	{
		size_t new_candidate_count = candidate_count + 1;

		auto new_candidates = new Candidate * [new_candidate_count];

		if (new_candidates)
		{
			if (candidate_count)
			{
				for (size_t i = 0; i < candidate_count; i++)
				{
					new_candidates[i] = candidates[i];
				}
				delete[] candidates;
			}

			new_candidates[candidate_count] = new Candidate;
			new_candidates[candidate_count]->candidate = candidate;
			new_candidates[candidate_count]->voteCount = new size_t(1);

			candidates = new_candidates;
			candidate_count = new_candidate_count;

		}
	}
	void deleteCandidateFromList(President* candidate)
	{
		if (candidate_count)
		{
			size_t new_candidate_count = candidate_count - 1;
			auto new_candidates = new Candidate * [new_candidate_count];

			for (size_t i = 0, j = 0; i < candidate_count; i++)
			{
				if (candidate == candidates[i]->candidate)
					delete candidates[i];
				else
					new_candidates[j++] = candidates[i];
			}
			delete[] candidates;
			candidate_count--;
			candidates = new_candidates;
		}
	}
	Candidate* getCandidate(President* candidate) const
	{
		for (size_t i = 0; i < candidate_count; i++)
		{
			if (candidates[i]->candidate == candidate)
				return candidates[i];
		}
		return NULL;
	}
	size_t getVoteCount() const { return *this->candidate->voteCount; }
	void writeToFile()
	{
		std::ofstream fout(file_name, std::ios_base::app);

		if (fout.is_open())
		{
			std::string text;
			text.append("Name: " + this->name + "\n");
			text.append("Surname: " + this->surname + "\n");
			text.append("Age: " + std::to_string(this->age) + "\n");
			text.append("Speciality: " + this->speciality + "\n");
			text.append("Voted to: " + this->candidate->candidate->getName() + "\n");
			text.append("Voted date time: " + this->sendVoteDateTime + "\n");
			text.append("\n");
			fout << text;
		}
		fout.close();
	}
	~Person()
	{
		(*this->candidate->voteCount)--;

		if ((*this->candidate->voteCount) == 0)
		{
			deleteCandidateFromList(this->candidate->candidate);
		}
		delete this->candidate;
	}
};
Candidate** Person::candidates = NULL;
size_t Person::candidate_count = NULL;
void startProgram() {
	President* presidentA = new President("President 1", "Country 1", 25);
	President* presidentB = new President("President 2", "Country 2", 21);

	Person* p1 = new Person("Arif", "Bagirli", 18, "Software Developer");
	Person* p2 = new Person("Riko", "Bgrff", 20, "IT Specialist");

	p1->sendVote(presidentA);
	std::cout << "Voted!" << std::endl;
	std::cout << "Time: " << p1->getVotedDateTime() << std::endl;
	std::cout << "Vote count: " << p1->getVoteCount() << std::endl;
	std::cin.get();
	p2->sendVote(presidentA);
	std::cout << "Voted!" << std::endl;
	std::cout << "Time: " << p2->getVotedDateTime() << std::endl;
	std::cout << "Vote count: " << p2->getVoteCount() << std::endl;
	std::cin.get();


	delete p1;
	delete p2;
	std::cout << "Candidate 1: " << presidentA->getName() << std::endl;
	std::cout << "Candidate 2: " << presidentB->getName() << std::endl;
	std::cin.get();
	system("CLS");
	presidentA->showAllVotes();
	delete presidentA;
	delete presidentB;
}
int main(){
	startProgram();
}