#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <unordered_set>
#include <string>

using I32 = std::int32_t;
using U32 = std::uint32_t;
using U0 = void;
using namespace std::string_literals;

#if 0				// first part
static const std::string word = "XMAS"s;
static const auto word_len = (I32)word.length();

static const std::vector<std::vector<I32>> directions {
  {1, 0},				// forward
  {-1, 0},				// backward
  {0, 1},				// upward
  {0, -1},				// downward
  {-1, -1},				// diag up left
  {-1, 1},				// diag down left
  {1, -1},				// diag up right
  {1, 1}				// diag down right
};

int main () {
  // takes about 2 596 019ns
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<std::string> lines;
  {
    std::string line;
    std::ifstream ifs ("input.txt");
    assert(ifs);
    while (std::getline (ifs, line))
      lines.emplace_back(line);
  }
  U32 count = 0;
  auto const n = (I32)lines.size();
  for (I32 i = 0; i < n; ++i) {
    auto const line_len = (I32)lines[i].length();
    for (I32 j = 0; j < line_len; ++j) {
      if (lines[i][j] == word[0]) { // found a potential XMAS, start looking
	for (auto const &direction : directions) {
	  I32 y = i, x = j, index = 0;
	  // check in every direction
	  // but first see if you're in bounds and if so far the rsult is correct
	  while (x >= 0 && x < line_len && y >= 0 && y < n && index < word_len && lines[y][x] == word[index]) {
	    x += direction[0];
	    y += direction[1];
	    ++index;
	  }
	  if (index == word_len) {
	    ++count;
	  }
	}
      }
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " nanoseconds\n";
  std::cout << "Result is: " << count << '\n';
  return 0;
}
#else  // second part
static const std::string word = "MAS"s;
static const auto word_len = (I32)word.length();

int main () {
  // this is obv terrible 865 953ns
  std::vector<std::string> lines;
  auto start = std::chrono::high_resolution_clock::now();
  {
    std::string line;
    std::ifstream ifs ("input.txt");
    assert(ifs);
    while (std::getline (ifs, line))
      lines.emplace_back(line);
  }

  I32 count = 0;
  auto const n = (I32)lines.size();

  for (I32 i = 0; i < n; ++i) {
    auto const line_len = (I32)lines[i].length();
    for (I32 j = 0; j < line_len; ++j) {
      auto ch = lines[i][j];
      if (ch == 'M' || ch == 'S') {
	auto last_ch = (ch == 'M') ? 'S' : 'M';
	if (j + 2 < line_len && i + 2 < n) {
	  if (lines[i + 1][j + 1] == 'A' && lines[i + 2][j + 2] == last_ch) {
	    // check the other diagonal X
	    auto ch_diago = lines[i][j + 2];
	    auto last_ch_diago = (ch_diago == 'M') ? 'S' : 'M';
	    if (ch_diago == 'M' || ch_diago == 'S') {
	      if (lines[i + 2][j] == last_ch_diago) {
		++count;
	      }
	    }
	  }
	}
      }
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " nanoseconds\n";
  std::cout << "Result is: " << count << '\n';
  return 0;
}
#endif
