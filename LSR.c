#include <stdio.h>

#define MAX 20
#define INF 9999

int n;
int cost[MAX][MAX];

void dijkstra(int source)
{
    int dist[MAX], visited[MAX], parent[MAX];
    int i, j, min, u;

    for (i = 0; i < n; i++)
    {
        dist[i] = cost[source][i];
        parent[i] = source;
        visited[i] = 0;
    }

    dist[source] = 0;
    visited[source] = 1;

    for (i = 1; i < n; i++)
    {
        min = INF;
        u = -1;

        for (j = 0; j < n; j++)
        {
            if (!visited[j] && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        for (j = 0; j < n; j++)
        {
            if (!visited[j] && cost[u][j] != INF &&
                dist[u] + cost[u][j] < dist[j])
            {
                dist[j] = dist[u] + cost[u][j];
                parent[j] = u;
            }
        }
    }

    // 🔹 Print in table format
    printf("\nShortest paths from router %d:\n", source);
    printf("-------------------------------------------------\n");
    printf("| Destination | Cost | Path                     |\n");
    printf("-------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("| %-11d | %-4d | ", i, dist[i]);

        if (i == source)
        {
            printf("%d", source);
        }
        else
        {
            int path[MAX];
            int count = 0;
            int p = i;

            while (p != source)
            {
                path[count++] = p;
                p = parent[p];
            }
            path[count++] = source;

            for (j = count - 1; j >= 0; j--)
            {
                printf("%d", path[j]);
                if (j != 0)
                    printf(" -> ");
            }
        }
        printf(" |\n");
    }
    printf("-------------------------------------------------\n");
}
int main()
{
    int i, j, source;

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter cost matrix (use %d for no connection):\n", INF);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &cost[i][j]);

            if (cost[i][j] == 0 && i != j)
                cost[i][j] = INF;
        }
    }

    printf("Enter source router: ");
    scanf("%d", &source);

    dijkstra(source);

    return 0;
}
