#include <string>
#include <fstream>
#include <cassert>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <chrono>

int
main() {
  // part 1: 841,850ns
  std::string line;
  {
    std::ifstream ifs("input.txt");
    assert(ifs);
    std::getline(ifs, line);
  }
  int file_id = 0;
  bool is_block_file = true;
  std::vector<std::pair<int, int>> result;
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < line.length(); ++i) {
    int count = line[i] - '0';
    int id = (is_block_file) ? file_id++ : -1;
    int c  = (is_block_file) ? line[i] - '0' : '.';
    for (size_t j = 0; j < count; ++j) {
      result.emplace_back(c, id);
    }
    is_block_file = !is_block_file;
  }
  auto const n = result.size();
  int left = 0, right = n - 1;
  for (size_t i = 0; i < n; ++i) {
    if (result[i].first == '.') {
      left = i;
      break;
    }
  }
  while (left < right) {
    if (result[right].first != '.') {
      std::swap(result[left], result[right]);
      while (result[left].first != '.') {
        ++left;
      }
      --right;
    } else {
      --right;
    }
  }
  uint64_t checksum = 0;
  for (int i = 0; i <= left - 1; ++i) {
    checksum += result[i].second * i;
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "result in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nanoseconds" << std::endl;
  std::cout << "checksum: " << checksum << std::endl;
  return 0;
}
