#include <chrono>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <vector>

// don't look at this file it is embarrasing, i need to modify all of it
// it gives the right result but this program is an insult to the computer

using B32 = std::int32_t;
using U16 = std::uint16_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using U0 = void;

#if 0 // part 1
int
main ()
{
  // part 1 -> pretty bad, 282,177 ns
  auto start = std::chrono::high_resolution_clock::now ();
  uint32_t safe_reports = 0;
  std::ifstream ifs ("input.txt");
  assert (ifs);
  std::string line;
  while (std::getline (ifs, line))
    {
      bool safe = true;
      std::istringstream ss (line);
      int num1, num2;
      ss >> num1 >> num2;
      int prev = num1;
      bool increasing = (num2 > prev) ? true : false;
      auto abs = std::abs (num2 - prev);
      if (abs < 1 || abs > 3)
        continue;
      prev = num2;
      while (ss >> num2)
        {
          if ((increasing && num2 < prev) || (!increasing && num2 > prev))
            {
              safe = false;
              break;
            }
          auto abs = std::abs (num2 - prev);
          if (abs < 1 || abs > 3)
            {
              safe = false;
              break;
            }
          prev = num2;
        }
      if (safe)
        ++safe_reports;
    }
  auto end = std::chrono::high_resolution_clock::now ();
  __builtin_printf ("safe reports: %u\n", safe_reports);
  __builtin_printf ("finished in %zu nanoseconds\n",
                    std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count ());
  return 0;
}
#else // part 2
B32
check (std::vector<I32> const &data)
{
  B32 increasing = (data[1] - data[0] > 0) ? true : false;
  for (U32 i = 1; i < data.size (); ++i)
    {
      auto add = data[i] - data[i - 1];
      auto add_abs = std::abs (add);
      if (add == 0 || (increasing && add < 0) || (!increasing && add > 0) || add_abs > 3)
        return false;
    }
  return true;
}

int
main ()
{
  // not only it's bad coded, but also takes lots of time 529,006 ns
  auto start = std::chrono::high_resolution_clock::now ();
  std::vector<std::vector<I32>> lines;
  U32 safe_reports = 0;
  {
    std::ifstream ifs ("input.txt");
    assert (ifs);
    std::string line;
    while (std::getline (ifs, line))
      {
        std::vector<I32> vals;
        std::istringstream ss (line);
        I32 num;
        while (ss >> num)
          vals.push_back (num);
        lines.emplace_back (vals);
      }
  }
  for (U32 line = 0; line < lines.size (); ++line)
    {
      for (U32 i = 0; i < lines[line].size (); ++i)
        {
          auto line_cpy = lines[line];
          line_cpy.erase (line_cpy.begin () + i);
          if (check (line_cpy))
            {
              ++safe_reports;
              break;
            }
        }
    }
  auto end = std::chrono::high_resolution_clock::now ();
  __builtin_printf ("safe reports: %u\n", safe_reports);
  __builtin_printf ("finished in %zu nanoseconds\n",
                    std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count ());
  return 0;
}
#endif
