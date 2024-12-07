#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <chrono>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

using I32 = std::int32_t;
using I64 = std::int64_t;
using B32 = std::uint32_t;
using I128 = __int128;

#if 0                           // part 1

enum class patrol_direction {
  UP = 0,
  RIGHT,
  DOWN,
  LEFT
};

inline patrol_direction& operator++(patrol_direction &dir) {
  switch (dir){
  case patrol_direction::UP:
    dir = patrol_direction::RIGHT;
    break;
  case patrol_direction::RIGHT:
    dir = patrol_direction::DOWN;
    break;
  case patrol_direction::DOWN:
    dir = patrol_direction::LEFT;
    break;
  case patrol_direction::LEFT:
    dir = patrol_direction::UP;
    break;
  }
  return dir;
}

inline static bool patrol_goes_out_map(I32 *patrol_pos, patrol_direction &dir, std::vector<std::string> &lines) {
  static I32 n = (I32)lines.size();
  static I32 m = (I32)lines[0].size();
  switch (dir) {
  case patrol_direction::UP:
    return patrol_pos[0] == 0;
  case patrol_direction::RIGHT:
    return patrol_pos[1] == m - 1;
  case patrol_direction::DOWN:
    return patrol_pos[0] == n - 1;
  case patrol_direction::LEFT:
    return patrol_pos[1] == 0;
  }
  return false;
}

inline static bool patrol_can_move(I32 *patrol_pos, patrol_direction &dir, std::vector<std::string> &lines) {
  static I32 n = (I32)lines.size();
  static I32 m = (I32)lines[0].size();
  switch (dir) {
  case patrol_direction::UP:
    return patrol_pos[0] > 0 && lines[patrol_pos[0] - 1][patrol_pos[1]] != '#';
  case patrol_direction::RIGHT:
    return patrol_pos[1] < m - 1 && lines[patrol_pos[0]][patrol_pos[1] + 1] != '#';
  case patrol_direction::DOWN:
    return patrol_pos[0] < n - 1 && lines[patrol_pos[0] + 1][patrol_pos[1]] != '#';
  case patrol_direction::LEFT:
    return patrol_pos[1] > 0 && lines[patrol_pos[0]][patrol_pos[1] - 1] != '#';
  }
  return false;
}

inline static void patrol_update_dir (patrol_direction &dir) {
  ++dir;
}

inline static void patrol_move(I32 *patrol_pos, patrol_direction &dir, std::unordered_set<I64> &visited) {
  I32 vec[2] = {0};

  switch (dir) {
  case patrol_direction::UP:
    vec[0] = -1;
    break;
  case patrol_direction::RIGHT:
    vec[1] = 1;
    break;
  case patrol_direction::DOWN:
    vec[0] = 1;
    break;
  case patrol_direction::LEFT:
    vec[1] = -1;
    break;
  }

  patrol_pos[0] += vec[0];
  patrol_pos[1] += vec[1];

  I64 key = static_cast<I64>(patrol_pos[0]) << 32 | patrol_pos[1];

  if (visited.count(key) == 0) {
    visited.emplace(key);
  }
}

int main() {
  // time: ~330 000ns
  std::vector<std::string> lines;
  I32 patrol_pos[] = {0, 0};
  patrol_direction patrol_dir = patrol_direction::UP;
  std::unordered_set<I64> visited;

  auto start = std::chrono::high_resolution_clock::now();

  {
    std::ifstream ifs ("input.txt");
    std::string line;
    line.reserve(10 + 1);
    I32 line_count = 0;
    B32 patrol_found = false;
    std::string::size_type pos = 0;
    assert(ifs);
    while (std::getline(ifs, line)) {
      if (!patrol_found) {
        if ((pos = line.find('^')) != std::string::npos) {
          patrol_dir = patrol_direction::UP;
          patrol_found = true;
          patrol_pos[0] = line_count;
          patrol_pos[1] = pos;
        } else if ((pos = line.find('>')) != std::string::npos) {
          patrol_dir = patrol_direction::RIGHT;
          patrol_found = true;
          patrol_pos[0] = line_count;
          patrol_pos[1] = pos;
        } else if ((pos = line.find('<')) != std::string::npos) {
          patrol_dir = patrol_direction::LEFT;
          patrol_found = true;
          patrol_pos[0] = line_count;
          patrol_pos[1] = pos;
        } else if ((pos = line.find('v')) != std::string::npos) {
          patrol_dir = patrol_direction::DOWN;
          patrol_found = true;
          patrol_pos[0] = line_count;
          patrol_pos[1] = pos;
        }
      }
      lines.emplace_back(line);
      ++line_count;
    }
  }

  I64 start_key = static_cast<I64>(patrol_pos[0]) << 64 | (patrol_pos[1] << 32);
  visited.emplace(start_key);

  while (true) {
    if (patrol_goes_out_map(patrol_pos, patrol_dir, lines)) {
      break;
    } else if (patrol_can_move(patrol_pos, patrol_dir, lines)) {
      patrol_move(patrol_pos, patrol_dir, visited);
    } else {
      patrol_update_dir(patrol_dir);
      if (!patrol_can_move(patrol_pos, patrol_dir, lines)) {
        break;
      }
    }
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "result: " << visited.size() << '\n';
  std::cout << "finished in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

  return 0;
}

#else  // part 2

int main() {
  // fucking embarrasing piece of software, it takes 15 seconds
  I32 constexpr rows = 130, cols = 130;
  std::vector<std::string> lines(rows, std::string(cols, 0));
  I32 count = 0;
  I32 guard_pos[2] = {0};

  std::vector<std::pair<I32, I32>> const directions = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1} // 0 up, 1 right, 2 down, 3 left
  };

  auto start = std::chrono::high_resolution_clock::now();

  {
    I32 i = 0;
    std::ifstream ifs("input.txt");
    assert(ifs);
    std::string line (cols, '.');
    std::string::size_type pos;
    while (std::getline(ifs, line)) {
      if ((pos = line.find('^')) != std::string::npos) {
        guard_pos[0] = i;
        guard_pos[1] = pos;
      }
      lines[i++] = line;
    }
  }

  // this stupid mf line costed me 5 hours
  lines[guard_pos[0]][guard_pos[1]] = '.';

  auto get_key = [](I32 r, I32 c, I32 d) {
    return (static_cast<I128>(r) << 16) | (static_cast<I128>(c) << 8) | static_cast<I128>(d);
  };

  auto run_cycle_check = [&]() {
    std::unordered_set<I128> visited;
    I32 r = guard_pos[0];
    I32 c = guard_pos[1];
    I32 d = 0;
    while (true) {
      auto key = get_key(r, c, d);
      if (visited.count(key) != 0) {
        return true;
      }
      visited.emplace(key);
      auto r2 = r + directions[d].first;
      auto c2 = c + directions[d].second;
      if (!(r2 >= 0 && r2 < rows && c2 >= 0 && c2 < cols)) { // out of bounds
        break;
      }
      if (lines[r2][c2] == '.') {
        r = r2;
        c = c2;
      } else {
        d = (d + 1) % 4;
      }
    }
    return false;
  };

  for (I32 i = 0; i < rows; ++i) {
    for (I32 j = 0; j < cols; ++j) {
      if (lines[i][j] != '.') {
        continue;
      }
      if (i == guard_pos[0] && j == guard_pos[1]) {
        continue;
      }
      lines[i][j] = '#';
      if (run_cycle_check()) {
        ++count;
      }
      lines[i][j] = '.';
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds\n";
  std::cout << "count: " << count << '\n';

  return 0;
}

#endif
