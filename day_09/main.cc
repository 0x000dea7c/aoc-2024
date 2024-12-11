#include <string>
#include <fstream>
#include <cassert>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <list>
#include <chrono>
#include <vector>

// using lists because (i thought initially) that removing/inserting nodes in the middle would be too expensive...
// so list it is for now

struct chunk
{
  chunk (int id, int len, bool is_file) : _id{id}, _len{len}, _is_file{is_file} {}
  int _id;
  int _len;
  bool _is_file;
};

using iterator = std::list<chunk>::iterator; // fucking disgusting

static void
merge (std::list<chunk> &disk, iterator it)
{
  if (!it->_is_file)
    {
      // fuck, i need to merge with the next OR previous..
      auto next = std::next (it);
      if (next != disk.end () && !next->_is_file)
        {
          it->_len += next->_len;
          disk.erase (next);
        }
      if (it != disk.begin ()) // HEADS UP! if i do prev before this check, ub
        {
          auto prev = std::prev (it);
          if (!prev->_is_file)
            {
              it->_len += prev->_len;
              disk.erase (prev);
            }
        }
    }
}

static void
rearrange (std::list<chunk> &disk, iterator &file_it, iterator &space_it)
{
  auto rem = space_it->_len - file_it->_len;
  if (rem > 0)
    {
      // there's remainder, so need to split into two nodes
      space_it->_len = file_it->_len;
      disk.insert (std::next (space_it, 1), {-1, rem, false});
    }
  space_it->_id = file_it->_id;
  space_it->_is_file = true;
  file_it->_id = -1;
  file_it->_is_file = false;
  merge (disk, file_it);
  merge (disk, space_it);
}

int
main ()
{
  // part 1: 841,850ns (0.84ms)
  // part 2: 116,432,008ns (116ms)
  // both solutions suck, i could go run a marathon and this shit is still working
  std::string line;
  {
    std::ifstream ifs ("input.txt");
    assert (ifs);
    std::getline (ifs, line);
  }
  auto start = std::chrono::high_resolution_clock::now ();
  std::list<chunk> disk;
  {
    int file_id = 0;
    bool is_file = true;
    for (size_t i = 0; i < line.length (); ++i)
      {
        int len = line[i] - '0';
        int id = (is_file) ? file_id++ : -1;
        disk.emplace_back (id, len, is_file);
        is_file = !is_file;
      }
  }
  // store file ids here, then sort in reverse order to i can scan from left to right
  // directly preserving the problem requirement where i want to look from right to left
  // the reason why i need to store iterators is bc i will be needing to access the info
  // later on!
  std::vector<iterator> file_ids;
  for (auto it = disk.begin (); it != disk.end (); ++it)
    if (it->_is_file)
      file_ids.push_back (it);
  // ok, sort this sonofabitch
  std::sort (file_ids.begin (), file_ids.end (), [] (iterator &a, iterator &b) { return a->_id > b->_id; });
  // ok, now i can traverse peacefully
  for (auto &file_id_it : file_ids)
    {
      auto it = disk.begin ();
      while (it != file_id_it) // stop when you consumed all of the spaces available
        {
          if (!it->_is_file && it->_len >= file_id_it->_len)
            {
              rearrange (disk, file_id_it, it);
              break;
            }
          ++it;
        }
    }
  uint64_t checksum = 0;
  size_t pos = 0;
  for (auto const &chunk : disk)
    {
      if (chunk._is_file)
        {
          for (size_t i = 0; i < chunk._len; ++i)
            {
              checksum += (chunk._id * pos);
              ++pos;
            }
        }
      else
        {
          pos += chunk._len;
        }
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "result in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count ()
            << " nanoseconds" << std::endl;
  std::cout << "checksum: " << checksum << std::endl;
  return 0;
}
