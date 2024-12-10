#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cmath>

inline int get_key(int i, int j) {
  return (static_cast<int32_t>(i << 8)) | static_cast<int32_t>(j);
}

inline static void
place_antinodes(std::unordered_set<int>& antinodes, std::pair<int, int> const& a1, std::pair<int, int> const& a2, int m, int n) {
  // WIP: i know i need to normalise the direction vector... but this is all half assed rn bc my brain
  // refuses to work
  int dr = a2.first - a1.first;
  int dc = a2.second - a1.second;
  double magnitude = sqrt(dr * dr + dc * dc); // sqrt(y² + x²)
  assert(magnitude != 0.0);
  std::cout << "magnitude = " << magnitude << std::endl;
  std::cout << "dr = " << dr << std::endl;
  int dr_round = dr / round(magnitude);
  int dc_round = dc / round(magnitude);
  assert(dr_round != 0);
  assert(dc_round != 0);
  int rup = a1.first - dr_round;
  int cup = a1.second - dc_round;
  int rdown = a2.first + dr_round;
  int cdown = a2.second + dc_round;
  while (rup >= 0 && rup < m && cup >= 0 && cup < n) { // inside matrix
    //std::cout << "adding up: (" << rup << ", " << cup << ")" << std::endl;
    antinodes.emplace(get_key(rup, cup));
    rup -= dr_round;
    cup -= dc_round;
  }
  while (rdown >= 0 && rdown < m && cdown >= 0 && cdown < n) { // inside matrix
    //std::cout << "adding down: (" << rdown << ", " << cdown << ")" << std::endl;
    antinodes.emplace(get_key(rdown, cdown));
    rdown += dr_round;
    cdown += dc_round;
  }
}

int
main () {
  // 25,905ns (i don't know how part1 worked tbh... i think my impl was wrong)
  std::vector<std::string> lines;
  std::unordered_map<int, std::vector<std::pair<int, int>>> antennas;
  {
    int r = 0;
    std::string line;
    std::ifstream ifs("input.txt");
    assert(ifs);
    while (std::getline(ifs, line)) {
      for (std::string::size_type c = 0; c < line.length(); ++c) {
        if ((line[c] >= 'a' && line[c] <= 'z') || (line[c] >= 'A' && line[c] <= 'Z') || (line[c] >= '0' && line[c] <= '9')) {
          antennas[line[c]].emplace_back(r, c);
        }
      }
      lines.push_back(line);
      ++r;
    }
  }
  auto start = std::chrono::high_resolution_clock::now();
  std::unordered_set<int> antinodes;
  auto const m = lines.size();
  auto const n = lines[0].length();
  for (auto const& [freq, vec] : antennas) {
    //std::cout << "processing antenna " << (char)freq << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = i + 1; j < vec.size(); ++j) {
        // try every combo?
        place_antinodes(antinodes, vec[i], vec[j], m, n);
      }
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "number of unique antinodes: " <<  antinodes.size() << '\n';
  std::cout << "finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";
  return 0;
}
