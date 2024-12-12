#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <chrono>

inline static int
get_key (int r, int c)
{
  return (r << 8) | c;
}

static int
dfs (int r, int c, std::vector<std::vector<int>> const &lines, int prev, std::unordered_set<int> &visited)
{
  visited.emplace (get_key (r, c));
  if (lines[r][c] - prev <= 1)
    {
      if (prev == 8 && lines[r][c] == 9)
        return 1;
      int count = 0;
      if (r - 1 >= 0 && visited.count (get_key (r - 1, c)) == 0 && (lines[r - 1][c] - lines[r][c]) == 1)
        count += dfs (r - 1, c, lines, lines[r][c], visited);
      if (c + 1 < lines[0].size () && visited.count (get_key (r, c + 1)) == 0 && (lines[r][c + 1] - lines[r][c]) == 1)
        count += dfs (r, c + 1, lines, lines[r][c], visited);
      if (c - 1 >= 0 && visited.count (get_key (r, c - 1)) == 0 && (lines[r][c - 1] - lines[r][c]) == 1)
        count += dfs (r, c - 1, lines, lines[r][c], visited);
      if (r + 1 < lines.size () && visited.count (get_key (r + 1, c)) == 0 && (lines[r + 1][c] - lines[r][c]) == 1)
        count += dfs (r + 1, c, lines, lines[r][c], visited);
      return count;
    }
  return 0;
}

int
main ()
{
  // part1: 266'933ns
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
      std::unordered_set<int> visited;
      int score = dfs (r, c, lines, lines[r][c], visited);
      scores += score;
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "scores = " << scores << std::endl;
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count () << " ns"
            << std::endl;
  return 0;
}
