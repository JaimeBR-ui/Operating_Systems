// Jaime Bohorquez
// NID: ja409704
// COP 4600 - Operating Systems, Spring 2021
// Programmed using Atom + iTerm2 on Mac OS Big Sur
// Filename: shell.cpp

// This program is a simple shell simulator that can perform many of the basic
// functions of a bourne shell. The main part of this assignment is to get
// familiar with using system calls such as fork() and exec().

// Stream imports.
#include <fstream>
#include <iostream>
// System imports.
#include <signal.h>
#include <unistd.h>
// Datastructure imports.
#include <string>
#include <vector>
#include <unordered_set>

// Global variables.
std::vector<std::string> path{"Users", "Jaime"};
std::vector<std::string> history; // From oldest to newest.
std::unordered_set<int> child_pids;

// Function headers.
void load_history(void);
void parse(const std::string str);
std::string * ptostr(std::vector<std::string> path);
std::string * join(std::vector<std::string> v, int start);
std::vector<std::string> * split(std::string str, char split_char);

int main(void)
{
  // We save the input from the shell in here.
  std::string response;
  load_history(); // Load from history.txt
  while (1)
  {
    std::cout << "# ";
    getline(std::cin, response);
    history.push_back(response);
    parse(response);
  }
  return 0;
}

void parse(std::string str)
{
  // Splits the string to extract the command and its args.
  std::vector<std::string> result = * split(str, ' ');
  // The first string is the command and the remaining are the parameters.
  std::string command = result.front();
  if (!command.compare("byebye"))
  {
    // Open file stream.
    std::ofstream history_file;
    history_file.open ("history.txt");
    // Save history in file.
    for (std::string s : history)
      history_file << s << '\n';
    // Close and exit.
    history_file.close();
    exit(0);
  }
  else if (!command.compare("currentdir"))
  {
    std::string currpath = (* ptostr(path));
    // List all the elements in the specified directory.
    for (const auto & entry : std::__fs::filesystem::directory_iterator(currpath))
        std::cout << entry.path() << std::endl;
  }
  else if (!command.compare("history"))
  {
    if (result.size() > 1 && result[1].compare("-c") == 0)
      history.clear();
    else
      // Print in the order of most recent command first.
      for (int i = history.size() - 1; i >= 0; i--)
      {
        std::cout << (history.size() - i - 1) << ':' ;
        std::cout << ' ' << history[i] << std::endl;
      }
  }
  else if (!command.compare("movetodir"))
  {
    if (result.size() == 1 || result[1] == "")
      std::cout << "Usage: movetodir [directory-path]" << std::endl;
    else if (!result[1].compare(".."))
    {
      if (path.size() > 0)
        path.pop_back();
    }
    else
    {
      try
      {
        path.push_back(result[1]);
        std::__fs::filesystem::directory_iterator((* ptostr(path)));
      }
      catch (std::__1::__fs::filesystem::filesystem_error e)
      {
        path.pop_back();
        std::cout << "Directory does not exist" << std::endl;
      }
    }
  }
  else if (!command.compare("whereami"))
  {
    std::cout << (*ptostr(path)) << std::endl;
  }
  else if (!command.compare("replay"))
  {
    int track = 0;
    if (result.size() > 1)
    {
      try
      {
        track = std::stoi(result[1]);
        if (history.size() - 1 > track)
        {
          track = history.size() - track - 2;
          parse(history.at(track));
        }
      }
      catch(std::invalid_argument ia)
      {
        std::cout << "Please provide a valid number." << std::endl;
      }
    }
    if (result.size() == 0 || history.size() < track)
      std::cout << "Usage: replay [instruction-number]" << std::endl;
  }
  else if (!command.compare("start") || !command.compare("background"))
  {
    // Runs binaries. You can specify a relative or absolute path.
    int status, background = 1;

    if (!command.compare("start"))
      background = 0;
    // Fork the process.
    int pid = fork();
    // If we are not executing an absolute path then add path to binary.
    std::string temp = "";
    if (result[1][0] != '/')
      temp = *ptostr(path);
    temp += result[1];
    result[1] = temp;
    if (pid)
    {
      // Pid != 0 means we are running parent process.
      child_pids.insert(pid);
      if (background == 0)
        waitpid(pid, &status, 0); // Wait for the child to exit.
    }
    else
    {
      // Pid == 0. This is the child process.
      int fd = 0;
      std::cout << "PID: " << getpid() << std::endl;
      for (int i : child_pids)
        std::cout << i << std::endl;

      std::string joined_command = (* join(result, 2));
      char ** full = (char **) malloc(sizeof(char *) * (result.size()));

      for (int i = 1; i < result.size(); i++)
      {
        // Place executable as a char array.
        char * executable = (char *)malloc(sizeof(char)* (result[i].length() + 1));
        strcpy(executable, result[i].c_str());
        full[i - 1] = executable;
      }
      // Set the last element in array to null.
      full[result.size() - 1] = nullptr;
      char *env[] = {(char *)0 };
      int result = execve(full[0], full, env);
      // std::cout << result << std::endl; // returns -1 if execution failed.
      free(full);
    }

    // if (background == 0)
    //   std::cout << status << std::endl;
    if (pid == 0)
      exit(0);
  }
  else if (!command.compare("dalek"))
  {
    // Kill a specified PID.
    if (result.size() == 1)
    {
      std::cout << "Usage to kill a process: dalek [PID]" << std::endl;
    }
    else
    {
      int resp = 0, pid = 0;
      // Check if the user provided a valid argument.
      try
      {
        pid = std::stoi(result[1]);
        resp = kill(pid, 0);
        child_pids.erase(pid);
      }
      catch(std::invalid_argument ia)
      {
        std::cout << "Please provide a valid number." << std::endl;
      }
      // Process was unable to be ended.
      if (resp < 0)
        std::cout << "Unable to kill process " << pid << std::endl;
    }
  }
  else if (!command.compare("repeat"))
  {
    if (result.size() > 2)
    {
      int amount;
      // Check if argument given is valid.
      try
      {
        amount = std::stoi(result[1]);
      }
      catch(std::invalid_argument ia)
      {
        std::cout << "Please provide a valid number." << std::endl;
        return;
      }
      // Format the command to be passed into parser.
      std::string rec_comm = * join(result, 2);
      rec_comm = "background " + rec_comm;
      std::cout << "PIDs: " << std::endl;
      // Run command $amount times.
      for (int i = 0; i < amount; i++)
        parse(rec_comm);
    }
    else
    {
      std::cout << "Usage: repeat [numberOfTimes] [args]" << std::endl;
    }
  }
  else if (!command.compare("dalekall"))
  {
    // Store processes that were sucessfully exterminated.
    std::vector<int> killed;
    for (const int pid : child_pids)
      if (kill(pid, 0) >= 0)
        killed.push_back(pid);
    // Printing the amount of processes killed.
    std::string out = "Exterminating ";
    out += std::to_string(killed.size());
    out += " processe(s): ";
    std::cout << out << std::endl;
    // Print all the killed processes.
    for (const int pid : killed)
      std::cout << std::to_string(pid) << std::endl;

    child_pids.clear();
  }
}

std::vector<std::string> * split(std::string str, char split_char)
{
  // Splits the string by spaces and return the fragments inside a vector.
  std::string word = "";
  std::vector<std::string> * result = new std::vector<std::string>;

  for (auto x : str)
  {
    if (x == split_char)
    {
      result->push_back(word);
      word = "";
    }
    else
    {
      word = word + x;
    }
  }
  result->push_back(word);

  return result;
}

std::string * join(std::vector<std::string> v, int start)
{
  std::string result = "";
  for (int i = start; i < v.size(); i++)
  {
    result += v.at(i);
    if (i - 1 < v.size())
      result += ' ';
  }
  // std::cout << result << std::endl;
  return new std::string(result);
}

void load_history(void)
{
  std::string line;
  std::ifstream history_file ("history.txt");
  if (history_file.is_open()) // Opened sucessfully.
  {
    // Load all the history to the history vector.
    while (getline(history_file,line))
      history.push_back(line);

    history_file.close();
  }
}

std::string * ptostr(std::vector<std::string> path)
{
  std::string stringified_path = "/"; // Stack string.
  // Append path to a string.
  for (std::string directory : path)
    stringified_path += directory + '/';
  // Return a string created in the heap.
  return new std::string(stringified_path);
}
