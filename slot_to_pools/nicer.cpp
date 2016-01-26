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

int num_servers, num_pools, num_rows;
vector<Server> servers;

typedef vector<int> Allocation; // server_id -> pool

int evaluate(const Allocation& assign) {
	// min over all rows
	int global_min = 1 << 30;
	assert(assign.size() == num_servers);
	assert(servers.size() == num_servers);
	for (int dead_row = 0; dead_row < num_rows; ++dead_row) {
		vector<int> capa_per_pool(num_pools, 0);
		for (int server_id = 0; server_id < num_servers; ++server_id) {
			if (servers[server_id].allocated &&
			    servers[server_id].row != dead_row) {
				int pool = assign[server_id];
				capa_per_pool[pool] += servers[server_id].capacity;
			}
		}
		for (int pool = 0; pool < num_pools; ++pool) {
			global_min = min(capa_per_pool[pool], global_min);
		}
	}
	return global_min;
}

void print(const Allocation& assign) {
	for (int i = 0; i < num_servers; ++i) {
		if (servers[i].allocated) {
			cout << servers[i].row << " "
			     << servers[i].slot << " "
			     << assign[i] << endl;
		}
		else cout << "x" << endl;
	}
}

Allocation improvement_step1(Allocation initial) {
	int best_value = evaluate(initial);
	Allocation best_found = initial;
	for (int i = 0; i < num_servers; ++i) {
		int first_pool = initial[i];
		for (int p = 0; p < num_pools; ++p) {
			initial[i] = p;
			int cur_value = evaluate(initial);
			if (cur_value > best_value)
				best_found = initial;
		}
		initial[i] = first_pool;
	}
	return best_found;
}

Allocation improvement_step2(Allocation initial) {
	int best_value = evaluate(initial);
	Allocation best_found = initial;
	for (int i = 0; i < num_servers; ++i) {
		for (int j = 0; j < i; ++j) {
			swap(initial[i], initial[j]);
			int cur_value = evaluate(initial);
			if (cur_value > best_value)
				best_found = initial;
			swap(initial[i], initial[j]);
		}
	}
	return best_found;
}

Allocation optimise(Allocation cur) {
	int value_so_far = evaluate(cur);
	cout << value_so_far << endl;
	while (true) {
		Allocation improved = improvement_step2(improvement_step1(cur));
		int new_value = evaluate(improved);
		cout << new_value << endl;
		assert(new_value >= value_so_far);
		if (new_value == value_so_far)
			return cur;
		value_so_far = new_value;
		cur = improved;
	}
}

int main() {
	cin >> num_servers >> num_pools >> num_rows;

	for (int i = 0; i < num_servers; ++i) {
		Server cur;
		cin >> cur.row >> cur.slot >> cur.capacity;
		assert((cur.row == -1) == (cur.slot == -1) &&
		       (cur.slot == -1) == (cur.capacity == -1));
		cur.allocated = (cur.row != -1);
		servers.push_back(cur);
	}

	Allocation sample_assign(num_servers, 0);
	for (int i = 0; i < num_servers; ++i) {
		sample_assign[i] = i % num_pools;
	}

//	cout << evaluate(sample_assign) << endl;
	Allocation improved = optimise(sample_assign);
	print(improved);

	return 0;
}
