import sys
from collections import defaultdict

assert len(sys.argv) == 3
input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, 'r') as f:
    num_rows, num_slots, num_unavailable, num_pools, num_servers = map(int, f.readline().split())
    slots = [[True] * num_slots for _ in range(num_rows)]
    for _ in range(num_unavailable):
        r, s = map(int, f.readline().split())
        assert 0 <= r < num_rows
        assert 0 <= s < num_slots
        slots[r][s] = False
    server_size = []
    server_capacity = []
    for _ in range(num_servers):
        z, c = map(int, f.readline().split())
        assert 1 <= z <= num_slots
        server_size.append(z)
        server_capacity.append(c)

pools = [[] for i in range(num_pools)]

with open(output_file, 'r') as f:
    for i in range(num_servers):
        line = f.readline()
        if line.strip() == 'x':
            continue
        r, s, p = map(int, line.split())
        assert 0 <= r < num_rows
        assert 0 <= s <= num_slots - server_size[i]
        assert 0 <= p < num_pools
        for j in range(s, s + server_size[i]):
            assert slots[r][j]
            slots[r][j] = False
        pools[p].append((r, server_capacity[i]))

score = 10**10
for pool in pools:
    if not pool:
        score = 0
        break
    rows = defaultdict(int)
    total_capacity = 0
    for server_row, server_capacity in pool:
        rows[server_row] += server_capacity
        total_capacity += server_capacity
    guarantee = total_capacity - max(rows.values())
    score = min(score, guarantee)

print(score)
