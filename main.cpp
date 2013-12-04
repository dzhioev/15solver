#include "position.h"
#include "search.h"

#include <vector>
#include <limits>

int main() {
  srand(time(0));
  Position finish(std::vector<std::vector<int>>{
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, Position::BLANK}});
  Position start = finish.GetShuffled(10000);
  std::string msg;
  auto result = AStarSearcher().Search(
      start, finish, std::numeric_limits<int>::max(), msg);
  std::cout << "Solution length: " << result.size() << std::endl;
}
