#include <iostream>
#include <vector>

using namespace std;

struct Server {
	int row;
	int slot;
	int capacity;
};

int main() {
	int num_servers, num_pools;
	cin >> num_servers >> num_pools;

	vector<Server> servers;
	for (int i = 0; i < num_servers; ++i) {
		Server cur;
		cin >> cur.row >> cur.slot >> cur.capacity;
		servers.push_back(cur);
	}

	for (int i = 0; i < num_servers; ++i) {
		cout << servers[i].row << servers[i].slot << i % num_pools;
	}

	return 0;
}
