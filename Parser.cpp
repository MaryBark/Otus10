#include "Parser.h"

error::Parser::InvalidToken::InvalidToken(const std::string &input):
    std::logic_error("unexpected token: " + input)
{ }

error::Parser::Parser(int packSize, IObrKommand &logger):
    packSize(packSize), logger(logger)
{
    commands.reserve(packSize);
}
