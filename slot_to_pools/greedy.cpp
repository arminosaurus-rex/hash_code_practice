// {{{
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <complex>
#include <numeric>
#define REP(i, n) for (int i = 0; i < (int) (n); ++i)
#define FOR(i, a, b) for (int i = (int) (a); i <= (int) (b); ++i)
#define FORD(i, a, b) for (int i = (int) (a); i >= (int) (b); --i)
#define FORE(it, c) for (__typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)
#define SIZE(x) ((int) ((x).size()))
#define DEBUG(x) { cerr << #x << ": " << (x) << endl; }
#define SQR(x) ((x) * (x))
#define INF 1023456789
using namespace std;

template<typename T, typename U> ostream& operator << (ostream& os, const pair<T, U>& p) {
    os << "(" << p.first << "," << p.second << ")"; return os;
}

template<typename T> ostream& operator << (ostream& os, const vector<T>& v) {
    os << "["; FORE(i, v) { if (i != v.begin()) os << ", "; os << *i; } os << "]"; return os;
}

typedef long long LL;
typedef pair<int, int> PI;
typedef pair<int, PI> TRI;
typedef vector<int> VI;
typedef vector<VI> VVI;
// }}}

int main() {
    // input
    int num_servers, num_pools, num_rows;
    cin >> num_servers >> num_pools >> num_rows;
    VI row(num_servers), slot(num_servers), capacity(num_servers), pool(num_servers, -1);
    REP(i, num_servers)
        cin >> row[i] >> slot[i] >> capacity[i];

    // list of servers in a row (decreasing capacity)
    vector<vector<PI> > R(num_rows);
    REP(i, num_servers) if (row[i] != -1)
        R[row[i]].push_back(PI(capacity[i], i));
    REP(i, num_rows)
        sort(R[i].rbegin(), R[i].rend());

    // rows will be considered with decreasing total capacity in them
    vector<PI> rows_order(num_rows);
    REP(i, num_rows) {
        int row_total = 0;
        FORE(j, R[i])
            row_total += j->first;
        rows_order[i] = PI(row_total, i);
    }
    sort(rows_order.rbegin(), rows_order.rend());

    // current capacity in pools + PQ for smallest pool
    VI total(num_pools, 0);
    priority_queue<PI, vector<PI>, greater<PI> > Q;
    REP(i, num_pools)
        Q.push(PI(0, i));

    // greedy allocation
    REP(i, num_rows) {
        FORE(j, R[rows_order[i].second]) {
            int p = Q.top().second;
            Q.pop();
            pool[j->second] = p;
            total[p] += j->first;
            Q.push(PI(total[p], p));
        }
    }

    // output
    REP(i, num_servers)
        if (row[i] == -1) {
            cout << "x" << endl;
        } else {
            cout << row[i] << " " << slot[i] << " " << pool[i] << endl;
        }
}
