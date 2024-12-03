#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cstdint>
#include <cassert>
#include <cstdio>
#include <map>
#include <set>

using B32 = std::int32_t;
using U16 = std::uint16_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using U0 = void;

#if 0 // part 1
inline I32
parse_int (char const *str, U32 len)
{
  U32 num = 0;
  for (U32 i = 0; i < len && str && *str >= '0'; ++i)
    num = 10 * num + (*str++ - '0');
  return num;
}

int
main ()
{
  // this takes 52 491ns to run, it's embarrasing
  auto start = std::chrono::high_resolution_clock::now ();
  std::string contents;
  {
    std::ifstream ifs ("input.txt");
    assert (ifs);
    std::stringstream ss;
    ss << ifs.rdbuf ();
    contents = ss.str ();
  }
  U32 sum = 0;
  std::string::size_type pos = 0, last_pos = 0;
  U32 n = contents.length ();
  while ((pos = contents.find ("mul(", last_pos)) != std::string::npos)
    {
      B32 err = false;
      U32 num_pos = 0;
      char numstr[3 + 1] = {'\0'};
      I32 num1 = 0, num2 = 0;
      for (auto i = pos + 4; i < pos + 4 + 3 + 1 + 3 + 1 && i < n; ++i)
        {
          char c = contents[i];
          if (c >= '0' && c <= '9')
            numstr[num_pos++] = c;
          else if (c == ',')
            {
              numstr[num_pos++] = '\0';
              num1 = parse_int (numstr, 4);
              num_pos = 0;
            }
          else if (c == ')')
            {
              numstr[num_pos++] = '\0';
              num2 = parse_int (numstr, 4);
              num_pos = 0;
              break;
            }
          else
            {
              err = true;
              break;
            }
        }
      if (!err)
        sum += num1 * num2;
      last_pos = pos + 1;
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::printf ("result is %u\n", sum);
  std::printf ("finished in %zu nanoseconds\n",
               std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count ());
  return EXIT_SUCCESS;
}
#else // part 2
inline I32
parse_int (char const *str, U32 len)
{
  U32 num = 0;
  for (U32 i = 0; i < len && str && *str >= '0'; ++i)
    num = 10 * num + (*str++ - '0');
  return num;
}

int
main ()
{
  // FIXME this takes 855 137ns to run, it's embarrasing
  auto start = std::chrono::high_resolution_clock::now ();
  std::string contents;
  {
    std::ifstream ifs ("input.txt");
    assert (ifs);
    std::stringstream ss;
    ss << ifs.rdbuf ();
    contents = ss.str ();
  }
  std::map<U32, std::pair<I32, I32>> mul_pair_pos;
  std::set<U32> dont_pos, do_pos;
  U32 sum = 0;
  std::string::size_type pos = 0, last_pos = 0;
  U32 n = contents.length ();
  while ((pos = contents.find ("mul(", last_pos)) != std::string::npos)
    {
      B32 err = false;
      U32 num_pos = 0;
      char numstr[3 + 1] = {'\0'};
      I32 num1 = 0, num2 = 0;
      for (auto i = pos + 4; i < pos + 4 + 3 + 1 + 3 + 1 && i < n; ++i)
        {
          char c = contents[i];
          if (c >= '0' && c <= '9')
            numstr[num_pos++] = c;
          else if (c == ',')
            {
              numstr[num_pos++] = '\0';
              num1 = parse_int (numstr, 4);
              num_pos = 0;
            }
          else if (c == ')')
            {
              numstr[num_pos++] = '\0';
              num2 = parse_int (numstr, 4);
              num_pos = 0;
              break;
            }
          else
            {
              err = true;
              break;
            }
        }
      if (!err)
        {
          mul_pair_pos.emplace (pos, std::make_pair<> (num1, num2));
        }
      last_pos = pos + 1;
    }
  pos = last_pos = 0;
  while ((pos = contents.find ("don't()", last_pos)) != std::string::npos)
    {
      dont_pos.emplace (pos);
      last_pos = pos + 1;
    }
  pos = last_pos = 0;
  while ((pos = contents.find ("do()", last_pos)) != std::string::npos)
    {
      do_pos.emplace (pos);
      last_pos = pos + 1;
    }
  // FIXME terrible terrible terrible terrible
  for (auto const &[idx, nums] : mul_pair_pos)
    {
      U32 dont_idx_saved = 0;
      B32 dont_found = false, mult = false;
      for (auto const dont_idx : dont_pos)
        {
          if (dont_idx < idx)
            {
              if (!dont_found || dont_idx > dont_idx_saved)
                {
                  dont_found = true;
                  dont_idx_saved = dont_idx;
                }
            }
        }
      if (dont_found)
        {
          for (auto const do_idx : do_pos)
            {
              if (do_idx < idx && do_idx > dont_idx_saved)
                {
                  mult = true;
                  break;
                }
            }
        }
      else
        mult = true;
      if (mult)
        sum += nums.first * nums.second;
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::printf ("result is %u\n", sum);
  std::printf ("finished in %zu nanoseconds\n",
               std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count ());
  return EXIT_SUCCESS;
}
#endif
