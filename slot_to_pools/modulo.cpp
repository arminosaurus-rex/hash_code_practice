#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

struct Server {
	int row;
	int slot;
	int capacity;
	bool allocated;
};

typedef vector<int> Allocation; // server_id -> pool

int main() {
	int num_servers, num_pools;
	cin >> num_servers >> num_pools;

	vector<Server> servers;
	for (int i = 0; i < num_servers; ++i) {
		Server cur;
		cin >> cur.row >> cur.slot >> cur.capacity;
		assert((cur.row == -1) == (cur.slot == -1) &&
		       (cur.slot == -1) == (cur.capacity == -1));
		cur.allocated = (cur.row != -1);
		servers.push_back(cur);
	}

	for (int i = 0; i < num_servers; ++i) {
		if (servers[i].allocated) {
			cout << servers[i].row << " "
			     << servers[i].slot << " "
			     << i % num_pools << endl;
		}
		else cout << "x" << endl;
	}

	return 0;
}
