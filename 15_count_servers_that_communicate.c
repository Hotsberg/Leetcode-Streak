#include <stdlib.h>

int countServers(int** grid, int gridSize, int* gridColSize) {
    int m = gridSize;
    int n = gridColSize[0];

    int* row_count = (int*)calloc((size_t)m, sizeof(int));
    int* col_count = (int*)calloc((size_t)n, sizeof(int));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                row_count[i]++;
                col_count[j]++;
            }
        }
    }

    int communicating_servers = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                if (row_count[i] > 1 || col_count[j] > 1) {
                    communicating_servers++;
                }
            }
        }
    }

    free(row_count);
    free(col_count);

    return communicating_servers;
}
