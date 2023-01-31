#include <iostream>
#include <memory>

#include "getter.h"
#include "Parser.h"
#include "ObrKommand.h"

using namespace std;
using namespace error;

int main(int argc, char const *argv[]) {
  int N;
  try
  {
    N = get_arg(argc, argv);
  }
  catch (getter &e)
  {
    cerr << "Usage error: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  ObrKommand logger { };
  Parser parser { N, logger };
  string buf { };

  cin >> buf;
  while (cin) {
    try {
      parser << buf;
    }
    catch (Parser::InvalidToken &e) {
      cerr << "Error occured: " << e.what() << endl;
    }
    cin >> buf;
  }

  return EXIT_SUCCESS;
}
