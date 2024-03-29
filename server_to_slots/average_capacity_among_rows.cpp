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
  capacity_server.clear();

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
  if(left_slot + s > S) return false;
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


bool decreasing_relative_capacity(Server serv1, Server serv2){
  int s1 = serv1.second.first; int c1 = serv1.second.second;
  int s2 = serv2.second.first; int c2 = serv2.second.second;
  return (c1 * s2) > (c2 * s1);
}


/**
* Method first shuffles the servers and then tries to place them as leftmost as
* possible. Writes its output to the initial_grid
*/
void distribute_servers_evenly(){

  sort(list_of_servers.begin(), list_of_servers.end(), decreasing_relative_capacity);

  // for(int i=0; i<list_of_servers.size(); i++){
  //   cout << list_of_servers[i].second.first << " " << list_of_servers[i].second.second << endl;
  // }


  int num_server = 0;
  int row = 0;

  while(num_server < M){
    int rt = (row + 1) % R;
    //cout << "Hi = " << num_server << " " << rt << " " << row << endl;
    Server serv = list_of_servers[num_server];
    while(rt != row){
      bool found = false;
      for(int j=0; j<S; j++){
        if(place_server(serv, rt, j)){
          found = true;
          break;
        }
      }
      if(found){
        row = (row + 1) % R;
        break;
      }else{
        rt = (rt + 1) % R;
      }
    }
    num_server += 1;
  }
}

int main(){

  read_input();
  distribute_servers_evenly();
  print_grid_to_file();

  return 0;
}
