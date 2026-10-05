/*
 * PBLE 2: Campus Shortest Route Finder - Dijkstra's Algorithm
 * Graph representation : Adjacency Matrix
 * Technique            : Greedy method (array based Dijkstra)
 */

#include <stdio.h>

#define MAX 20
#define INF 9999

char names[MAX][30];      // location names
int graph[MAX][MAX];      // adjacency matrix
int dist[MAX];            // shortest distance from source
int parent[MAX];          // predecessor of each vertex on the shortest path
int visited[MAX];         // 1 if the shortest distance is finalized

int n = 0;                // number of locations
int source = -1;          // source location (0 based)
int graphEntered = 0;     // flag: graph has been entered
int computed = 0;         // flag: Dijkstra has been run

/* ---------- Option 1: Enter campus graph ---------- */
void enterGraph()
{
    int i, j, choice, roads, u, v, w;

    printf("\n1. Use sample campus graph\n2. Enter my own graph\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        n = 5;
        char sample[5][30] = {"Main Gate", "Library", "Computer Department",
                              "Laboratory", "Auditorium"};
        for (i = 0; i < n; i++)
        {
            int k = 0;
            while (sample[i][k] != '\0')
            {
                names[i][k] = sample[i][k];
                k++;
            }
            names[i][k] = '\0';
        }

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                graph[i][j] = (i == j) ? 0 : INF;

        // undirected roads: u, v, distance (1 based)
        int edges[6][3] = {
            {1, 2, 4},  // Main Gate - Library
            {1, 3, 8},  // Main Gate - Computer Department
            {2, 3, 3},  // Library - Computer Department
            {3, 4, 2},  // Computer Department - Laboratory
            {4, 5, 3},  // Laboratory - Auditorium
            {2, 5, 10}  // Library - Auditorium
        };
        for (i = 0; i < 6; i++)
        {
            u = edges[i][0] - 1;
            v = edges[i][1] - 1;
            graph[u][v] = graph[v][u] = edges[i][2];
        }

        graphEntered = 1;
        computed = 0;
        source = -1;
        printf("Sample graph loaded.\n");
        return;
    }

    printf("Enter number of locations (max %d): ", MAX);
    scanf("%d", &n);
    if (n <= 0 || n > MAX)
    {
        printf("Invalid number of locations.\n");
        n = 0;
        return;
    }

    printf("Enter the names of the locations:\n");
    for (i = 0; i < n; i++)
    {
        printf("Location %d: ", i + 1);
        scanf(" %29[^\n]", names[i]);
    }

    // 0 on diagonal, INF where there is no direct road
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            graph[i][j] = (i == j) ? 0 : INF;

    printf("Enter number of roads: ");
    scanf("%d", &roads);

    printf("Enter each road as: location1 location2 distance (locations numbered 1 to %d)\n", n);
    for (i = 0; i < roads; i++)
    {
        printf("Road %d: ", i + 1);
        scanf("%d %d %d", &u, &v, &w);

        if (u < 1 || u > n || v < 1 || v > n)
        {
            printf("Invalid location number, try again.\n");
            i--;
            continue;
        }
        if (w < 0)
        {
            printf("Distance cannot be negative, try again.\n");
            i--;
            continue;
        }
        graph[u - 1][v - 1] = w;
        graph[v - 1][u - 1] = w;   // roads are two-way
    }

    graphEntered = 1;
    computed = 0;
    source = -1;
    printf("Graph entered successfully.\n");
}

/* ---------- Option 2: Display adjacency matrix ---------- */
void displayMatrix()
{
    int i, j;

    if (!graphEntered)
    {
        printf("\nPlease enter the graph first (option 1).\n");
        return;
    }

    printf("\nAdjacency Matrix (INF = no direct road):\n\n");
    printf("%-22s", "");
    for (j = 0; j < n; j++)
        printf("%5d", j + 1);
    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("%2d. %-18s", i + 1, names[i]);
        for (j = 0; j < n; j++)
        {
            if (graph[i][j] == INF)
                printf("%5s", "INF");
            else
                printf("%5d", graph[i][j]);
        }
        printf("\n");
    }
}

/* ---------- Option 3: Select source ---------- */
void selectSource()
{
    int i, s;

    if (!graphEntered)
    {
        printf("\nPlease enter the graph first (option 1).\n");
        return;
    }

    printf("\nLocations:\n");
    for (i = 0; i < n; i++)
        printf("%d. %s\n", i + 1, names[i]);

    printf("Select source location number: ");
    scanf("%d", &s);

    if (s < 1 || s > n)
    {
        printf("Invalid source location.\n");
        return;
    }
    source = s - 1;
    computed = 0;
    printf("Source selected: %s\n", names[source]);
}

/* ---------- Option 4: Dijkstra's algorithm ---------- */
void dijkstra()
{
    int i, count, u, v, min;

    if (!graphEntered)
    {
        printf("\nPlease enter the graph first (option 1).\n");
        return;
    }
    if (source == -1)
    {
        printf("\nPlease select a source location first (option 3).\n");
        return;
    }

    // initialization
    for (i = 0; i < n; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;

    for (count = 0; count < n - 1; count++)
    {
        // pick the unvisited vertex with minimum distance (greedy choice)
        min = INF;
        u = -1;
        for (i = 0; i < n; i++)
        {
            if (!visited[i] && dist[i] < min)
            {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1)      // remaining vertices are unreachable
            break;

        visited[u] = 1;

        // relax all edges going out of u
        for (v = 0; v < n; v++)
        {
            if (!visited[v] && graph[u][v] != INF &&
                dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    computed = 1;
    printf("\nShortest distances calculated from %s.\n", names[source]);
}

/* prints the path from source to v using the parent array */
void printPath(int v)
{
    if (parent[v] == -1)
    {
        printf("%s", names[v]);
        return;
    }
    printPath(parent[v]);
    printf(" -> %s", names[v]);
}

/* ---------- Option 5: Display shortest paths ---------- */
void displayShortestPaths()
{
    int i;

    if (!computed)
    {
        printf("\nPlease run option 4 (Find Shortest Distance) first.\n");
        return;
    }

    printf("\nSource: %s\n\n", names[source]);
    printf("%-22s %-10s %s\n", "Destination", "Distance", "Shortest Path");
    printf("-----------------------------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        if (i == source)
            continue;

        printf("%-22s ", names[i]);
        if (dist[i] == INF)
        {
            printf("%-10s %s\n", "INF", "No path available");
        }
        else
        {
            printf("%-10d ", dist[i]);
            printPath(i);
            printf("\n");
        }
    }
}

/* ---------- Option 6: Display distances only ---------- */
void displayDistances()
{
    int i;

    if (!computed)
    {
        printf("\nPlease run option 4 (Find Shortest Distance) first.\n");
        return;
    }

    printf("\nShortest distance from %s:\n\n", names[source]);
    printf("%-22s %s\n", "Location", "Distance");
    printf("------------------------------\n");
    for (i = 0; i < n; i++)
    {
        printf("%-22s ", names[i]);
        if (dist[i] == INF)
            printf("INF\n");
        else
            printf("%d\n", dist[i]);
    }
}

/* ---------- main ---------- */
int main()
{
    int choice;

    printf("=====================================\n");
    printf("   CAMPUS SHORTEST ROUTE FINDER\n");
    printf("        (Dijkstra's Algorithm)\n");
    printf("=====================================\n");

    do
    {
        printf("\n------------- MENU -------------\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths\n");
        printf("6. Display Distance from Source to All Locations\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: 
			enterGraph();         
			 break;
        case 2:
			 displayMatrix();      
			 break;
        case 3:
			 selectSource();     
		    break;
        case 4:
			 dijkstra();          
		     break;
        case 5:
			 displayShortestPaths(); 
			 break;
        case 6:
			 displayDistances();   
			  break;
        case 7:
			printf("\nExiting program. Thank you!\n"); 
			break;
        default:
			printf("\nInvalid choice, please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

/*
 * TIME COMPLEXITY
 * The outer loop runs (V - 1) times. In each iteration we scan all V vertices
 * to find the minimum (O(V)) and then relax all V possible neighbours using
 * the adjacency matrix (O(V)).
 *   Total time = O(V) * (O(V) + O(V)) = O(V^2)
 * Space complexity = O(V^2) for the adjacency matrix + O(V) for dist[],
 * parent[] and visited[].
 */
