#ifndef GETTER_H
#define GETTER_H

#include <cstring>
#include <stdexcept>

namespace error
{
  class getter: public std::logic_error
  {
  public:
    explicit getter(const std::string &input);
  };

  int get_arg(int argc, char const *argv[]);
}

#endif // GETTER_H
