#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 4096
#define ARR_LEN 1000
#define MAX_NUM 100000 //  max number in input data

inline uint32_t
parse_int (char const *str)
{
  uint32_t n = 0;
  while (str && *str >= '0')
    n = n * 10 + (*str++ - '0');
  return n;
}

int
main ()
{
  // NOTE: takes around 110 microseconds to run in my machine, it works only on
  // gnu/linux bc i cba w/ windows and macos spyware
  auto start = std::chrono::high_resolution_clock::now ();
  auto fd = open ("input.txt", O_RDONLY, 0);
  assert (fd > 0);
  std::array<uint32_t, ARR_LEN> list1;
  std::array<uint32_t, MAX_NUM> list2 = {}; // already know the max number if the input list, so this allows for a
                                            // better optimisation, i.e. use an array instead of an unordered_map
                                            // for faster lookups
  {
    char buffer[BUFF_SIZE];
    char num_buffer[16];
    size_t num_pos = 0;
    size_t bytes_read;
    size_t k = 0;
    uint32_t num;
    bool first_num = true;
    while ((bytes_read = read (fd, buffer, BUFF_SIZE)) > 0)
      { // reading chunks is way better than char by char
        for (size_t i = 0; i < bytes_read; ++i)
          {
            char c = buffer[i];
            // parsing numbers by myself is way faster than std::sscanf, can get away w/ this in this dumb example
            if (c >= '0' && c <= '9')
              num_buffer[num_pos++] = c;
            else if (c == ' ' || c == '\n')
              {
                num_buffer[num_pos] = '\0'; // multiple spaces in the file control
                if (!num_buffer[0])
                  {
                    num_pos = 0;
                    continue;
                  }
                num = parse_int (num_buffer); // doesn't improve runtime that much, but
                                              // slightly faster than std::stol
                if (first_num)
                  list1[k++] = num;
                else
                  list2[num]++;
                first_num = !first_num;
                num_pos = 0;
              }
          }
      }
  }
  uint64_t sum = 0;
  for (size_t i = 0; i < ARR_LEN; ++i)
    sum += list1[i] * list2[list1[i]];
  close (fd);
  auto end = std::chrono::high_resolution_clock::now ();
  std::printf ("result is %zu in %zu us\n", sum,
               std::chrono::duration_cast<std::chrono::microseconds> (end - start).count ());
  return EXIT_SUCCESS;
}
