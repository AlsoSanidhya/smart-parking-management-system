#include <stdio.h>

#define NODES 7
#define INF 1000000

int main(void)
{
    // Node order: Entrance, Junction, P1, P2, P3, P4, P5.
    // A zero means there is no direct lane between those nodes.
    int lanes[NODES][NODES] = {
        { 0, 10,  0, 35,  0,  0,  0}, // Entrance
        {10,  0, 20, 12, 35, 18, 30}, // Junction
        { 0, 20,  0,  0,  0,  0,  0}, // P1
        {35, 12,  0,  0,  0,  0,  0}, // P2
        { 0, 35,  0,  0,  0,  0,  0}, // P3
        { 0, 18,  0,  0,  0,  0,  0}, // P4
        { 0, 30,  0,  0,  0,  0,  0}  // P5
    };

    int distance[NODES];
    int visited[NODES] = {0};

    for (int i = 0; i < NODES; i++)
    {
        distance[i] = INF;
    }
    distance[0] = 0; // Distance from the entrance to itself.

    for (int step = 0; step < NODES; step++)
    {
        int closest = -1;
        int best = INF;

        for (int i = 0; i < NODES; i++)
        {
            if (!visited[i] && distance[i] < best)
            {
                best = distance[i];
                closest = i;
            }
        }

        if (closest == -1)
        {
            break;
        }

        visited[closest] = 1;

        for (int next = 0; next < NODES; next++)
        {
            if (!visited[next] && lanes[closest][next] > 0)
            {
                int newDistance = distance[closest] + lanes[closest][next];

                if (newDistance < distance[next])
                {
                    distance[next] = newDistance;
                }
            }
        }
    }

    for (int i = 2; i < NODES; i++)
    {
        printf("P%d: %d m\n", i - 1, distance[i]);
    }

    return 0;
}