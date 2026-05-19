#include <stdio.h>

#define MAX 20
#define INF 999

int main() {
    int n, cost[MAX][MAX], dist[MAX], visited[MAX] = {0};
    int parent[MAX], i, j, source, count, min, u;

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter cost matrix:\n");

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);

            if(cost[i][j] == 0 && i != j)
                cost[i][j] = INF;
        }
    }

    printf("Enter source router: ");
    scanf("%d", &source);

    // Initialization
    for(i = 0; i < n; i++) {
        dist[i] = cost[source][i];
        parent[i] = source;
    }

    dist[source] = 0;
    visited[source] = 1;

    // Dijkstra Algorithm
    for(count = 1; count < n; count++) {

        min = INF;

        for(i = 0; i < n; i++) {
            if(!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }

        visited[u] = 1;

        for(i = 0; i < n; i++) {
            if(!visited[i] &&
               dist[u] + cost[u][i] < dist[i]) {

                dist[i] = dist[u] + cost[u][i];
                parent[i] = u;
            }
        }
    }

    // Output
    printf("\nDestination\tCost\tPath\n");

    for(i = 0; i < n; i++) {

        printf("%d\t\t%d\t", i, dist[i]);

        int path[MAX], k = 0, p = i;

        while(p != source) {
            path[k++] = p;
            p = parent[p];
        }

        path[k++] = source;

        for(j = k - 1; j >= 0; j--) {
            printf("%d", path[j]);

            if(j != 0)
                printf("->");
        }

        printf("\n");
    }

    return 0;
}
