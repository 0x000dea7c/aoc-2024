#include <unordered_map>
#include <vector>
#include <cassert>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iterator>
#include <algorithm>

using I32 = std::int32_t;
using U32 = std::uint32_t;
using B32 = std::uint32_t;

#if 0                           // part 1

int main() {
  // i am a disaster, 37 618 461ns!!!!!!!!!!1
  std::unordered_map<I32, std::vector<I32>> rules;
  std::vector<std::vector<I32>> lines;
  auto start = std::chrono::high_resolution_clock::now();

  {
    U32 line_count = 0;
    std::ifstream ifs("input.txt");
    assert(ifs);
    std::string line;
    while (std::getline(ifs, line)) {
      auto it = line.find('|');
      if (it != std::string::npos) {
        auto num1 = line.substr(it - 2, 2);
        auto num2 = line.substr(it + 1, 2);
        rules[std::stoi(num1)].emplace_back(std::stoi(num2));
      } else if (!line.empty()) {
        std::string::size_type pos = 0, last_pos = 0;
        std::string num;
        lines.push_back({});
        while ((pos = line.find(',', last_pos)) != std::string::npos) {
          num = line.substr(pos - 2, 2);
          lines[line_count].emplace_back(std::stoi(num));
          last_pos = pos + 1;
        }
        num = line.substr(line.length() - 2, 2);
        lines[line_count].emplace_back(std::stoi(num));
        ++line_count;
      }
    }
  }

  U32 sum = 0;
  auto const n = (I32)lines.size();
  for (I32 i = 0; i < n; ++i) {
    auto const m = (I32)lines[i].size();
    B32 correct = true;

    // process line
    for (I32 j = 0; j < m; ++j) {
      // process forward
      for (I32 k = j + 1; k < m; ++k) {
        if (std::find(rules[lines[i][j]].begin(), rules[lines[i][j]].end(), lines[i][k]) == rules[lines[i][j]].end()) {
          correct = false;
          break;
        }
      }

      // process backward
      for (I32 k = j - 1; k > 0; --k) {
        if (std::find(rules[lines[i][k]].begin(), rules[lines[i][k]].end(), lines[i][j]) == rules[lines[i][k]].end()) {
          correct = false;
          break;
        }
      }
    }

    if (correct) {
      sum += (m % 2 == 0) ? lines[i][m / 2 - 1] : lines[i][m / 2];
    }

  }

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "result is " << sum << '\n';
  std::cout << "time elapsed " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
  return 0;
}

#else  // part 2

int main() {
  // i wanna kms 64 430 293ns
  std::unordered_map<I32, std::vector<I32>> rules;
  std::vector<std::vector<I32>> lines;
  auto start = std::chrono::high_resolution_clock::now();

  {
    U32 line_count = 0;
    std::ifstream ifs("input.txt");
    assert(ifs);
    std::string line;
    while (std::getline(ifs, line)) {
      auto it = line.find('|');
      if (it != std::string::npos) {
        auto num1 = line.substr(it - 2, 2);
        auto num2 = line.substr(it + 1, 2);
        rules[std::stoi(num1)].emplace_back(std::stoi(num2));
      } else if (!line.empty()) {
        std::string::size_type pos = 0, last_pos = 0;
        std::string num;
        lines.push_back({});
        while ((pos = line.find(',', last_pos)) != std::string::npos) {
          num = line.substr(pos - 2, 2);
          lines[line_count].emplace_back(std::stoi(num));
          last_pos = pos + 1;
        }
        num = line.substr(line.length() - 2, 2);
        lines[line_count].emplace_back(std::stoi(num));
        ++line_count;
      }
    }
  }

  U32 sum = 0;
  auto const n = (I32)lines.size();
  for (I32 i = 0; i < n; ++i) {
    auto const m = (I32)lines[i].size();
    B32 correct = true;

    // process line
    for (I32 j = 0; j < m; ++j) {
      // process forward
      for (I32 k = j + 1; k < m; ++k) {
        if (std::find(rules[lines[i][k]].begin(), rules[lines[i][k]].end(), lines[i][j]) != rules[lines[i][k]].end()) {
          std::swap(lines[i][j], lines[i][k]);
          correct = false;
        }
      }

      // process backward
      for (I32 k = j - 1; k > 0; --k) {
        if (std::find(rules[lines[i][j]].begin(), rules[lines[i][j]].end(), lines[i][k]) != rules[lines[i][j]].end()) {
          std::swap(lines[i][j], lines[i][k]);
          correct = false;
        }
      }
    }

    if (!correct) {
      sum += (m % 2 == 0) ? lines[i][m / 2 - 1] : lines[i][m / 2];
    }

  }

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "result is " << sum << '\n';
  std::cout << "time elapsed " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
  return 0;
}


#endif
