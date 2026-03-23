#include <stdio.h>

#define MAX 10
#define INF 999

void linkStateRouting(int cost[MAX][MAX], int n, int source) {
    int dist[MAX], prev[MAX], selected[MAX];
    int count = 1, minDist, nextNode;

    // Initialization
    for (int i = 0; i < n; i++) {
        dist[i] = cost[source][i];
        prev[i] = source;
        selected[i] = 0;
    }

    dist[source] = 0;
    selected[source] = 1;

    while (count < n) {
        minDist = INF;
        nextNode = -1;

        // Select minimum distance node
        for (int i = 0; i < n; i++) {
            if (!selected[i] && dist[i] < minDist) {
                minDist = dist[i];
                nextNode = i;
            }
        }

        if (nextNode == -1) break;

        selected[nextNode] = 1;
        count++;

        // Update distances
        for (int i = 0; i < n; i++) {
            if (!selected[i] && minDist + cost[nextNode][i] < dist[i]) {
                dist[i] = minDist + cost[nextNode][i];
                prev[i] = nextNode;
            }
        }
    }

    // Final Routing Table
    printf("\n=========== ROUTING TABLE (%d) ===========\n", source);
    printf("Dest | Cost | Path\n");
    printf("-----|------|----------------\n");

    for (int i = 0; i < n; i++) {
        if (i == source) continue;

        printf("R%-3d | %-4d | %d", i, dist[i], i);

        int j = i;
        while (j != source) {
            j = prev[j];
            printf(" <- %d", j);
        }
        printf("\n");
    }

    printf("=========================================\n");
}

int main() {
    int n, cost[MAX][MAX], source;

    printf("--- Link State Routing ---\n");
    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (999 = INF):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            if (i == j) cost[i][j] = 0;
        }
    }

    printf("Enter source (0 to %d): ", n - 1);
    scanf("%d", &source);

    linkStateRouting(cost, n, source);

    return 0;
}
