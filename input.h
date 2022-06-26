#ifndef INPUT_INPUT_H
#define INPUT_INPUT_H


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>



struct Resources {
	std::vector<int> r; // renewable
	std::vector<int> n; // nonrenewable
	std::vector<int> d; // doubly constraint
};

struct Mode {
	int jobnr;
	int mode;
	int duration;
	Resources usedResources;
};

struct Job {
	int jobnr;
	int amountModes;
	int amountSuccessors;
	std::vector<int> successors;
	std::map<int, Mode> modes;
};

struct Problem {
	int projects;
	int amountJobs; // incl. supersource/sink
	int horizon;
	int renewableResources;
	int nonRenewableResources;
	int doublyConstraintResources;

	int pronr;
	int relDate;
	int dueDate;
	int tardCost;
	int mpmTime;

	Resources availableResources;

	std::map<int, Job> jobs;
};


std::vector<std::string> split(std::string str, std::string token) {
	std::vector<std::string>result;
	while (str.size()) {
		int index = str.find(token);
		if (index != std::string::npos) {
			if (str.substr(0, index).length())
				result.push_back(str.substr(0, index));
			str = str.substr(index + token.size());
		}
		else {
			if (str.length()) {
				result.push_back(str);
				str = "";
			}
		}
	}
	return result;
}


Problem readFile(std::string path) {
	Problem problem;

	
	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line) && line.find("projects") == std::string::npos);
		problem.projects = std::stoi(split(line, ":")[1]);
		while (getline(file, line) && line.find("jobs") == std::string::npos);
		problem.amountJobs = std::stoi(split(line, ":")[1]);
		while (getline(file, line) && line.find("horizon") == std::string::npos);
		problem.horizon = std::stoi(split(line, ":")[1]);
		while (getline(file, line) && line.find("renewable") == std::string::npos);
		problem.renewableResources = std::stoi(split(split(line, ":")[1], " ")[0]);
		while (getline(file, line) && line.find("nonrenewable") == std::string::npos);
		problem.nonRenewableResources = std::stoi(split(split(line, ":")[1], " ")[0]);
		while (getline(file, line) && line.find("doubly constrained") == std::string::npos);
		problem.doublyConstraintResources = std::stoi(split(split(line, ":")[1], " ")[0]);

		while (getline(file, line) && line.find("pronr.") == std::string::npos);
		getline(file, line);
		auto v = split(line, " ");
		problem.pronr = std::stoi(v[0]);
		problem.relDate = std::stoi(v[2]);
		problem.dueDate = std::stoi(v[3]);
		problem.tardCost = std::stoi(v[4]);
		problem.mpmTime = std::stoi(v[5]);

		while (getline(file, line) && line.find("jobnr.") == std::string::npos);
		while (getline(file, line) && line.find("***") == std::string::npos) {
			auto v = split(line, " ");
			Job job;
			job.jobnr = std::stoi(v[0]);
			job.amountModes = std::stoi(v[1]);
			job.amountSuccessors = std::stoi(v[2]);
			for (int i = 3; i < v.size(); ++i)
				job.successors.push_back(std::stoi(v[i]));
			problem.jobs[job.jobnr] = job;
		}

		while (getline(file, line) && line.find("---") == std::string::npos);
		std::string last = "0";
		while (getline(file, line) && line.find("***") == std::string::npos) {
			auto v = split(line, " ");
			if (line.find("         ") != std::string::npos) {
				std::reverse(v.begin(), v.end());
				v.push_back(last);
				std::reverse(v.begin(), v.end());
			}
			else { last = v[0]; }
			Mode mode;
			mode.jobnr = std::stoi(v[0]);
			mode.mode = std::stoi(v[1]);
			mode.duration = std::stoi(v[2]);
			for (int i = 3; i < 3 + problem.renewableResources; ++i)
				mode.usedResources.r.push_back(std::stoi(v[i]));
			for (int i = 3 + problem.renewableResources; i < 3 + problem.renewableResources + problem.nonRenewableResources; ++i)
				mode.usedResources.n.push_back(std::stoi(v[i]));
			for (int i = 3 + problem.renewableResources + problem.nonRenewableResources; i < 3 + problem.renewableResources + problem.nonRenewableResources + problem.doublyConstraintResources; ++i)
				mode.usedResources.d.push_back(std::stoi(v[i]));
			problem.jobs[mode.jobnr].modes[mode.mode] = mode;
		}

		getline(file, line);
		getline(file, line);
		getline(file, line);
		v = split(line, " ");

		for (int i = 0; i < problem.renewableResources; ++i)
			problem.availableResources.r.push_back(std::stoi(v[i]));
		for (int i = problem.renewableResources; i < problem.renewableResources + problem.nonRenewableResources; ++i)
			problem.availableResources.n.push_back(std::stoi(v[i]));
		for (int i = problem.renewableResources + problem.nonRenewableResources; i < problem.renewableResources + problem.nonRenewableResources + problem.doublyConstraintResources; ++i)
			problem.availableResources.d.push_back(std::stoi(v[i]));


		file.close();
	}
	
	else std::cerr << "Unable to open file";
	
	return problem;
}

#endif //INPUT_INPUT_H
