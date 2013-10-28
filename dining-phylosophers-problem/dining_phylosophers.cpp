#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <condition_variable>
 
std::mutex m;
std::condition_variable cv;
 
void what_is_he_doing(int pos, int action, int new_state)
{
  std::cout << "Phylosopher " << pos << " is ";
  if (action == 2)
    std::cout << "still eating...";
  else if (action == 1)
    std::cout << "eating...";
  else
    std::cout << "thinking...";
  if (!action)
    std::cout << ".......\n";
  else if (new_state)
    std::cout << "...interrupt.\n";
  else
    std::cout << "...done.\n";
}

void phylosopher_thread(int pos, int* friends, int doing, int new_state)
{
  m.lock();
  if (friends[pos])
    what_is_he_doing(pos, 2, new_state);
  else
  {
    what_is_he_doing(pos, doing, new_state);
    friends[pos] = doing;
  }
  m.unlock();
  friends[pos] = new_state;
}

void waiter()
{
  int* phylosophers = new int[5];
  std::memset(phylosophers, 0, 5*sizeof(int));
  int pos = 0;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();  
  std::default_random_engine generator (seed);                                  
  std::uniform_int_distribution<int> distribution(0, 1);
  while (true)
  {
    int eat = distribution(generator);
    if (eat)
    {
      int state = distribution(generator);
      int other = (pos + 2) % 5;
      int rand1 = distribution(generator);
      int rand2 = distribution(generator);
      if (!state)
        other = (other+1) % 5;
      std::thread ph1(phylosopher_thread, pos, phylosophers, eat, rand1);
      std::thread ph2(phylosopher_thread, other, phylosophers, state, rand2);
      std::mutex mtx;
      std::unique_lock<std::mutex> lck(mtx);
      cv.wait_for(lck,std::chrono::seconds(1));
      ph1.join();
      ph2.join();
      std::cout << "\n";
    }
    if (pos < 5)
      ++pos;
    else
      pos = 0;
  }
  delete[] phylosophers;
}
 
int main()
{
  waiter();
  return 0;
}
