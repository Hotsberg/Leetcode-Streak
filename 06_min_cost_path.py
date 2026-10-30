import heapq

class Solution(object):
    def shortestPath(self, n, edges, labels, k):
        adj = [[] for _ in range(n)]
        for u, v, w in edges:
            adj[u].append((v, w))
            
        INF = float('inf')
        dist = [[INF] * (k + 1) for _ in range(n)]
        
        dist[0][1] = 0
        pq = [(0, 0, 1)]
        
        while pq:
            cost, u, c = heapq.heappop(pq)
            
            if u == n - 1:
                return cost
                
            if cost > dist[u][c]:
                continue
                
            for v, w in adj[u]:
                nc = c + 1 if labels[u] == labels[v] else 1
                if nc <= k and cost + w < dist[v][nc]:
                    dist[v][nc] = cost + w
                    heapq.heappush(pq, (cost + w, v, nc))
                    
        return -1
