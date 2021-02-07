// Jaime Bohorquez
// NID: ja409704
// COP 4600 - Operating Systems, Spring 2021
// Programmed using Atom + iTerm2 on Mac OS Big Sur
// Filename: shell.cpp

// This program is a simple shell simulator that can perform many of the basic
// functions of a bourne shell. The main part of this assignment is to get
// familiar with using system calls such as fork() and exec().

// Library imports.
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

// Global variables.
std::vector<std::string> path{"Users", "Jaime"};
std::vector<std::string> history; // From oldest to newest.

// Function headers.
void load_history(void);
void parse(const std::string str);
std::string * ptostr(std::vector<std::string> path);
void print_vector(const std::vector<std::string> & v);
std::string * join(const std::vector<std::string> & v);
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
  // print_vector(result);
  std::string command = result.front();
  if (!command.compare("byebye"))
  {
    std::ofstream history_file;
    history_file.open ("history.txt");
    for (std::string s : history)
      history_file << s << '\n';
    history_file.close();
    exit(0);
  }
  else if (!command.compare("currentdir"))
  {
    std::string currpath = (* ptostr(path));
    for (const auto & entry : std::__fs::filesystem::directory_iterator(currpath))
        std::cout << entry.path() << std::endl;
  }
  else if (!command.compare("history"))
  {
    if (result.size() > 1 && result[1].compare("-c") == 0)
    {
      history.clear();
    }
    else
    {
      for (int i = history.size() - 1; i >= 0; i--)
        std::cout << (history.size() - i - 1) << ':' << ' ' << history[i] << std::endl;
    }
  }
  else if (!command.compare("movetodir"))
  {
    if (result.size() == 1 || result[1] == "")
    {
      std::cout << "Usage: movetodir [directory-path]" << std::endl;
    }
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

std::string * join(std::vector<std::string> & v)
{
  std::string result;
  for (std::string s : v)
  {
    result += s + ' ';
  }
  return new std::string(result);
}

void print_vector(const std::vector<std::string> & v)
{
  for (std::string s : v)
    std::cout << s << ' ' ;
  std::cout << std::endl;
}

void load_history(void)
{
  std::string line;
  std::ifstream history_file ("history.txt");
  if (history_file.is_open())
  {
    while (getline(history_file,line))
      history.push_back(line);

    history_file.close();
  }
}

std::string * ptostr(std::vector<std::string> path)
{
  std::string stringified_path = "/";
  for (std::string directory : path)
    stringified_path += directory + '/';
  return new std::string(stringified_path);
}
