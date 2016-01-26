#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <ctime>
#include <random>

using namespace std;

// First size, then capacity
typedef pair<int, int> PII;
typedef pair<int, PII> Server;

const int Max_Rows  = 1000;
const int Max_Slots = 1000;
const int Max_Pools = 1000;

const int slot_empty = -1;
const int slot_unavailable = -2;


// Rows, Slots, Unavailable Slots, Pools to be created, Number of servers to be
// allocated
int R, S, U, P, M;

int initial_grid[Max_Rows][Max_Slots];


vector<Server> list_of_servers;
vector<int> capacity_server;

/**
* Removes all servers placed into the grid, if slots are unavailable, this still remains though
*/
void reset_initial_grid(){
  for(int i=0; i<Max_Rows; i++){
    for(int j=0; j<Max_Slots; i++){
      initial_grid[i][j] = (initial_grid[i][j] == slot_unavailable)? slot_unavailable: slot_empty;
    }
  }
}


// Reads input into the initial grid and stores the server in the given vector
void read_input(){
  //Makes sure this is empty
  list_of_servers.clear();

  cin >> R >> S >> U >> P >> M;

  for(int i=0; i<R; i++){
    for(int j=0; j<S; j++){
      initial_grid[i][j] = slot_empty;
    }
  }

  for(int i=0; i<U; i++){
    int r, s;
    cin >> r >> s;
    initial_grid[r][s] = slot_unavailable;
  }

  for(int i=1; i<=M; i++){
    int s, c;
    cin >> s >> c;
    list_of_servers.push_back(make_pair(i, make_pair(s, c)));
    capacity_server.push_back(c);
  }
}


bool place_server(Server serv,int row, int left_slot){
  // get data
  int index = serv.first;
  int s = serv.second.first;

  // check
  if(left_slot + s > Max_Slots) return false;
  for(int i=0; i<s; i++){
    if(initial_grid[row][left_slot + i] != slot_empty) return false;
  }
  for(int i=0; i<s; i++){
    initial_grid[row][left_slot + i] = index;
  }
  return true;
}


map<int, PII> pos_servers;

void print_grid_to_file(){
  pos_servers.clear();

  for(int i=0; i<R; i++){
    for(int j=0; j<S; j++){
      int num = initial_grid[i][j];
      if(num > 0){
        // A server has been placed
        if(j == 0 || initial_grid[i][j-1] != num){
          PII ps = make_pair(i, j);
          pos_servers.insert(pair<int, PII> (num, ps));
        }
      }
    }
  }

  cout << M << " " << P << " " << R << endl;
  for(int i=1; i<=M; i++){
    if(pos_servers.count(i) > 0){
      int cap = capacity_server[i-1];
      PII ps = pos_servers[i];
      cout << ps.first << " " << ps.second << " " << cap << endl;
    }else{
      cout << "-1 -1 -1" << endl;
    }
  }
}

/**
* Method first shuffles the servers and then tries to place them as leftmost as
* possible. Writes its output to the initial_grid
*/
void distribute_servers_randomly(){

  unsigned seed = (unsigned) time(0);
  shuffle(list_of_servers.begin(), list_of_servers.end(), default_random_engine(seed));

  int num_server = 0;

  for(int i=0; i<R; i++){
    for(int j=0; j<S; j++){
      Server cur_serv = list_of_servers[num_server];
      if(place_server(cur_serv, i, j)){
          // this is how far we can place
          j += cur_serv.second.first - 1;
          num_server += 1;
          if(num_server == M) break;
      }
    }
    if(num_server == M) break;
  }
}

int main(){

  read_input();
  distribute_servers_randomly();
  print_grid_to_file();

  return 0;
}
