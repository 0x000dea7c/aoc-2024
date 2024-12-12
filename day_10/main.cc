#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <chrono>

static int
dfs (int r, int c, std::vector<std::vector<int>> const &lines, int prev)
{
  if (lines[r][c] - prev <= 1)
    {
      if (prev == 8 && lines[r][c] == 9)
        return 1;
      int count = 0;
      if (r - 1 >= 0 && (lines[r - 1][c] - lines[r][c]) == 1)
        count += dfs (r - 1, c, lines, lines[r][c]);
      if (c + 1 < lines[0].size () && (lines[r][c + 1] - lines[r][c]) == 1)
        count += dfs (r, c + 1, lines, lines[r][c]);
      if (c - 1 >= 0 && (lines[r][c - 1] - lines[r][c]) == 1)
        count += dfs (r, c - 1, lines, lines[r][c]);
      if (r + 1 < lines.size () && (lines[r + 1][c] - lines[r][c]) == 1)
        count += dfs (r + 1, c, lines, lines[r][c]);
      return count;
    }
  return 0;
}

int
main ()
{
  // part1: 266'933ns
  // part2: 75'981ns
  std::vector<std::pair<int, int>> starting_pts;
  std::vector<std::vector<int>> lines;
  auto start = std::chrono::high_resolution_clock::now ();
  {
    int line_num = 0;
    std::ifstream ifs ("input.txt");
    assert (ifs);
    std::string line;
    while (std::getline (ifs, line))
      {
        lines.push_back ({});
        for (size_t i = 0; i < line.length (); ++i)
          {
            int c = line[i] - '0';
            if (c == 0)
              starting_pts.emplace_back (line_num, i);
            lines.back ().push_back (c);
          }
        ++line_num;
      }
  }
  int scores = 0;
  for (auto const &[r, c] : starting_pts)
    {
      int score = dfs (r, c, lines, lines[r][c]);
      scores += score;
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "scores = " << scores << std::endl;
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count () << " ns"
            << std::endl;
  return 0;
}
