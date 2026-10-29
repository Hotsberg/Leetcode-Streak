import sys

sys.setrecursionlimit(200000)

class BIT:
    def __init__(self, size):
        self.size = size
        self.tree = [0] * (size + 2)

    def update(self, idx, val):
        while idx <= self.size:
            self.tree[idx] ^= val
            idx += idx & (-idx)

    def range_update(self, l, r, val):
        self.update(l, val)
        self.update(r + 1, val)

    def query(self, idx):
        res = 0
        while idx > 0:
            res ^= self.tree[idx]
            idx -= idx & (-idx)
        return res

class Solution(object):
    def palindromePath(self, n, edges, s, queries):
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)

        LOG = 16
        up = [[0] * LOG for _ in range(n)]
        depth = [0] * n
        tin = [0] * n
        tout = [0] * n
        initial_path_mask = [0] * n
        timer = [0]
        chars = list(s)

        def dfs(u, p, d, current_mask):
            timer[0] += 1
            tin[u] = timer[0]
            depth[u] = d
            up[u][0] = p
            for i in range(1, LOG):
                up[u][i] = up[up[u][i - 1]][i - 1]

            char_mask = 1 << (ord(chars[u]) - ord('a'))
            current_mask ^= char_mask
            initial_path_mask[u] = current_mask

            for v in adj[u]:
                if v != p:
                    dfs(v, u, d + 1, current_mask)
            
            tout[u] = timer[0]

        dfs(0, 0, 0, 0)

        def get_lca(u, v):
            if depth[u] < depth[v]:
                u, v = v, u
            for i in range(LOG - 1, -1, -1):
                if depth[u] - (1 << i) >= depth[v]:
                    u = up[u][i]
            if u == v:
                return u
            for i in range(LOG - 1, -1, -1):
                if up[u][i] != up[v][i]:
                    u = up[u][i]
                    v = up[v][i]
            return up[u][0]

        bit = BIT(timer[0] + 2)
        ans = []

        for q in queries:
            parts = q.split()
            if parts[0] == "update":
                u = int(parts[1])
                c = parts[2]
                old_mask = 1 << (ord(chars[u]) - ord('a'))
                new_mask = 1 << (ord(c) - ord('a'))
                delta = old_mask ^ new_mask
                chars[u] = c
                bit.range_update(tin[u], tout[u], delta)
            else:
                u = int(parts[1])
                v = int(parts[2])
                lca = get_lca(u, v)
                
                mask_u = initial_path_mask[u] ^ bit.query(tin[u])
                mask_v = initial_path_mask[v] ^ bit.query(tin[v])
                char_lca = 1 << (ord(chars[lca]) - ord('a'))
                
                path_mask = mask_u ^ mask_v ^ char_lca
                ans.append((path_mask & (path_mask - 1)) == 0)

        return ans
