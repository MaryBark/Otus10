#include "getter.h"


error::getter::getter(const std::string &input):
    std::logic_error(input) { }

int error::get_arg(int argc, const char *argv[]) {
    int N;
    size_t idx;

    if (argc != 2) throw(getter("One agument expected!"));

    try
    {
        N = std::stoi(argv[1], &idx);
    }
    catch (std::invalid_argument &e)
    {
        throw getter("Argument must be integer!");
    }
    catch (std::out_of_range &e)
    {
        throw getter("Argument is out of range!");
    }

    if (idx < std::strlen(argv[1]))
    {
        throw getter("Argument must be integer!");
    }
    if (N < 1)
    {
        throw getter("Argument must be greater than zero!");
    }

    return N;
}
