#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include <chrono>
#include <cstdint>

// 125 || 25
// 12525

// 1 str ok
// 2 shifts 32 bit overflows plus doesn't give right answer shit
// times *= 10 until it reaches smallest, then combine by mult greatest nd adding smallest
// no good.. 160404011322271
//           160404011322271
//           160404011322271
//           254136560217241
bool sums_to(uint64_t target, std::vector<uint64_t> const& values, uint64_t sum, size_t i) {
  if (i == values.size()) {
    return sum == target;
  }

  if (sums_to(target, values, sum + values[i], i + 1)) {
    return true;
  }

  if (sums_to(target, values, sum * values[i], i + 1)) {
    return true;
  }

  // <-- PART2 only contains this section!!!
  uint64_t times = 1;
  while (times <= values[i]) {  // left, fucking left to right!
    times *= 10;
  }
  auto combined = sum * times + values[i];
  // --> END PART2

  return sums_to(target, values, combined, i + 1);
};

int main () {
  // 1st part: took 191'965ns
  // 2nd part: took 25'075'368ns
  std::unordered_map<int, uint64_t> line_target;
  std::unordered_map<int, std::vector<uint64_t>> line_numbers;
  std::unordered_map<int, bool> line_ok;

  {
    int i = 0;
    std::vector<std::string> lines;
    lines.reserve(850);
    std::string line;
    std::ifstream ifs("input.txt");
    assert(ifs);

    while (std::getline(ifs, line)) {
      auto pos = line.find(':');
      auto target = line.substr(0, pos);
      line_target[i] = std::stol(target);
      std::istringstream sstr (line.substr(pos + 1));
      uint64_t n;
      while (sstr >> n) {
        line_numbers[i].push_back(n);
      }
      line_ok[i] = false;
      lines.push_back(line);
      ++i;
    }
  }

  auto start = std::chrono::high_resolution_clock::now();

  for (auto const& [line, values] : line_numbers) {
    if (sums_to(line_target[line], values, values[0], 1)) {
      line_ok[line] = true;
    }
  }

  uint64_t sum = 0;
  for (auto const& [line, ok] : line_ok) {
    if (ok) {
      sum += line_target[line];
    }
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "sum = " << sum << '\n';
  std::cout << "took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

  return 0;
}
