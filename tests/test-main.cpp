#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <ezipc/utility/log.hpp>
#include <ezipc/utility/string.hxx>

TEST_CASE("Main")
{
    using namespace ezipc;

    logger<> s{&std::cout};
    s << "hello, world 0!\n"
      << "hello, world 1!\n"
      << "hello, world 2!\n"
      << "hello, world 3!\n"
      << "hello, world 4!\n";

    s << "---------- devider ------------ \n";

    for (auto v : {0, 1, 2, 3, 4}) {
        s << format_string("hell, world %d", v);
    }
    s << std::endl;
    s << std::setw(3);

    REQUIRE(true);
}