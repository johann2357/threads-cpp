#include "List.h"
#include "Point.h"

int main()
{
  typedef Point<int, double> point_id;
  int r_n = 13;
  int points = 10000;
  int from = 1;
  int to = 100;
  int number_of_points = 10;
  point_id base;
  base(r_n, from, to);
  std::vector< point_id >* test;
  Debug("Create point vector\n");
  test = create_points<int, double> (points, r_n, from, to);
  List< point_id* > result;
  for (auto& i : *test)
  {
    i.calculate_distance(&base);
    result.insert(&i);
  }
  Debug("Print point vectors\n");
  int tmp = 0;
  for (List< point_id* >::iterator it = result.begin(); ((it != result.end()) && (tmp < number_of_points)); ++it)
  {
    print<int, double>(*it);
    ++tmp;
  }
  Debug(result.size_);
  Debug("The End.");
  return 0;
}
