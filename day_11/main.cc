#include <fstream>
#include <string>
#include <cassert>
#include <list>
#include <iostream>
#include <sstream>
#include <chrono>

#define BLINK_TIMES 25

using namespace std::string_literals;

struct node
{
  node (std::string val) : _val{val} {}
  std::string _val;
};

static void
process_stone (std::list<node> &stones, size_t idx, bool &skip_next)
{
  auto it = std::next (stones.begin (), idx);
  if (it->_val == "0"s)
    {
      it->_val = "1"s;
      skip_next = false;
    }
  else
    {
      auto len = it->_val.length ();
      if (len % 2 == 0)
        {
          auto first_half = it->_val.substr (0, (len / 2));
          auto first_half_num = std::stoi (first_half);
          auto second_half = it->_val.substr (len / 2);
          auto second_half_num = std::stoi (second_half);
          it->_val = first_half_num == 0 ? "0"s : std::to_string (first_half_num);
          stones.insert (std::next (it), second_half_num == 0 ? "0"s : std::to_string (second_half_num));
          skip_next = true;
        }
      else
        {
          uint64_t num = std::stoi (it->_val);
          num *= 2024;
          it->_val = std::to_string (num);
          skip_next = false;
        }
    }
}

int
main ()
{
  // part1: 86s this solution is so fucking bad it made me cut myself, FIXME: improve
  // part2:
  std::list<node> stones;
  auto start = std::chrono::high_resolution_clock::now ();
  {
    std::ifstream ifs ("input.txt"s);
    assert (ifs);
    std::string line;
    while (std::getline (ifs, line))
      {
        std::istringstream ss (line);
        int stone;
        while (ss >> stone)
          stones.push_back ({std::to_string (stone)});
      }
  }
  for (size_t i = 0; i < BLINK_TIMES; ++i)
    {
      bool skip_next = false;
      for (size_t j = 0; j < stones.size (); ++j)
        {
          if (!skip_next)
            process_stone (stones, j, skip_next);
          else
            skip_next = !skip_next;
        }
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "stones = " << stones.size () << std::endl;
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count () << " ms"
            << std::endl;
  return 0;
}
