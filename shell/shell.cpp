// Jaime Bohorquez
// NID: ja409704
// COP 4600 - Operating Systems, Spring 2021
// Programmed using Atom + iTerm2 on Mac OS Big Sur
// Filename: shell.cpp

// Library imports.
#include <iostream>
#include <string>

using namespace std;

void parse(string str);

int main(void)
{
  string response;
  while (1)
  {
    cout << "# ";
    getline(cin, response);
    parse(response);
  }
  return 0;
}

void parse(string str)
{
  // TODO: Split the string to extract the command and its args.
  if (str.compare("byebye") == 0)
  {
    exit(0);
  }
}
