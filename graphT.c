#include <stdio.h>
#include <stdlib.h>

#define QUEUE_MAX_SIZE 100
#define WORD_MAX_SIZE 20

typedef struct Graph {
    int** matrix;
    char** words;
} Graph_t;

void initGraph(Graph_t* graph, int size) {
    graph->matrix = (int**)malloc(size*sizeof(int *));
    graph->words = (char**)malloc(sizeof(char*)*size);

    for(int i = 0; i < size; i++) {
        graph->words[i] = (char*)malloc(WORD_MAX_SIZE*sizeof(char));
        graph->matrix[i] = (int*)calloc(size,sizeof(int));
    }
}

typedef struct Queue {
	int qu[QUEUE_MAX_SIZE];
	int tail;
} Qu;

void initQueue(Qu* queue) {
    queue->tail = 0;
}

int addElementQueue(Qu* queue, int value) {
    for (int i = 0; i < queue->tail; i++) {
        if (value == queue->qu[i]) {
            return 0;
        }
    }
	if (queue->tail == QUEUE_MAX_SIZE) {
		return 1;
	}
	queue->qu[queue->tail] = value;
	queue->tail++;
    return 0;
}

int deleteElementQueue(Qu* queue) {
	int deletedElement = queue->qu[0];

	for (int i = 0; i <= queue->tail; i++) {
		queue->qu[i] = queue->qu[i + 1];
	}

	queue->tail--;
	return deletedElement;
}

int isEmpty(Qu* queue) {
	if (queue->tail) {
		return 0;
	}
	else {
		return 1;
	}

}

int fillGraph (Graph_t* graph, int size) {
    int recievingVerticesNum;
    int vertexValue;

    for (int i = 0; i < size; i++) {
        printf("Value(max is %d) of the %d vertex: ",WORD_MAX_SIZE ,i + 1);
        scanf("%s", graph->words[i]);
    }

    for (int i = 0; i < size; i++) {
        system("clear");

        for (int k = 0; k < size; k++) {
            printf("%d.%s ", k + 1, graph->words[k]);
        }

        printf("\nNumber of outgoing or linked edges of the %d.%s vertex: ", i + 1, graph->words[i]);
        scanf("%d", &recievingVerticesNum);
        
        if (recievingVerticesNum != 0) {
            printf("\nEnter the %d values of the vertices: ", recievingVerticesNum);
        }
        for (int j = 0; j < recievingVerticesNum; j++) {
            scanf("%d", &vertexValue);
            if (vertexValue > 0 && vertexValue <= size) {
                graph->matrix[i][vertexValue - 1]++;
            } 
            else if(vertexValue == 0) {
                continue;
            }
            else {
                return 1;
            }
        }
    }
    system("clear");
    return 0;
}

void drawDirected (Graph_t* graph, int size) {
    FILE* S = fopen("Graph.dot", "w");                   //write in .dot file
    int checkUncon = 0;
    fprintf(S, "digraph Hey {\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < graph->matrix[i][j]; k++) {
                fprintf(S, "\t%s -> %s;\n", graph->words[i], graph->words[j]);
            }
            if (graph->matrix[i][j] != 0) {
                checkUncon++;
            }
        }
        if (checkUncon == 0) {
            fprintf(S, "\t%s;\n", graph->words[i]);
        }
        checkUncon = 0;
    }
    fprintf(S, "}");
    fclose(S);

    system("dot -Tpng Graph.dot -o dirGraph.png");       //call system for png
}

void drawUndirected(Graph_t* graph, int size) {
    int copyGraph[size][size];                              //copy graphMatrix 
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            copyGraph[i][j] = graph->matrix[i][j];
        }
    }

    FILE* S = fopen("Graph.dot", "w");                 //write in .dot file
    int checkUncon = 0;
    fprintf(S, "graph Hey {\n");                        
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < copyGraph[i][j]; k++) {     
                fprintf(S, "\t%s -- %s;\n", graph->words[i], graph->words[j]);
                if (i != j) {
                    copyGraph[j][i]--;
                }
            }
            if (graph->matrix[i][j] != 0) {
                checkUncon++;
            }
        }
        if (checkUncon == 0) {
            fprintf(S, "\t%s;\n", graph->words[i]);
        }
        checkUncon = 0;
    }
    fprintf(S, "}");
    fclose(S);

    system("dot -Tpng Graph.dot -o undirGraph.png");  //call system for png
}

void showVerticeConnectivity(Graph_t* graph, int size) {
    Qu conVertices;
    initQueue(&conVertices);
    Qu unconVertices;
    initQueue(&unconVertices);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (graph->matrix[i][j] != 0 && i != j) {
                addElementQueue(&conVertices, j);
                addElementQueue(&conVertices, i);
            }
        }
    }

    int checkUncon = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < conVertices.tail; j++) {
            if (i == conVertices.qu[j]) {
                checkUncon++;
            }
        }
        if(checkUncon == 0) {
            addElementQueue(&unconVertices, i);
        }
        checkUncon = 0;
    }

    printf("Have linked vertice: ");
    for (int i = 0; i < conVertices.tail; i++) {
        printf("%s ", graph->words[conVertices.qu[i]]);
    }

    printf("\nSingle vertice: ");
    for (int i = 0; i < unconVertices.tail; i++) {
        printf("%s ", graph->words[unconVertices.qu[i]]);
    }
    printf("\n");
}

void showGraphConnectivity (Graph_t* graph, int size) {
    Qu vertexCheck;
    initQueue(&vertexCheck);
    int* nodes = (int*)calloc(size, sizeof(int)); 
    addElementQueue(&vertexCheck, 0); 

    while (!isEmpty(&vertexCheck)) { 
        int node = deleteElementQueue(&vertexCheck); 
        nodes[node] = 2; 
        for (int j = 0; j < size; j++) { 
            if (graph->matrix[node][j] > 0 && nodes[j] == 0) { 
                addElementQueue(&vertexCheck, j); 
                nodes[j] = 1; 
            }
        }
    }

    int unconnected = 0;
    for (int i = 0; i < size; i++) {
        if (nodes[i] < 2) {
            unconnected++;
        }
    }
    if (unconnected) {
        printf("Unconnected graph");
    } 
    else {
        printf("Connected graph");
    }

    free(nodes);
}

int main(void) {
    int size;
    printf("Vertices number: ");
    scanf("%d", &size);

    Graph_t graph;
    initGraph(&graph, size);

    int graphType;
    do {
        system("clear");
        printf("1. Directed\n2. Undirected\nEnter: ");
        scanf("%d", &graphType);
        getchar();
    } while (graphType != 1 && graphType != 2);    

    if(fillGraph(&graph, size) == 1) {
        system("clear");
        printf("Wrong value");
        return 1;
    };

    system("clear");
    
    switch(graphType) {
    case 1:
        drawDirected(&graph, size);
        break;
    case 2:
        drawUndirected(&graph, size);
        break;
    }

    printf("Matrix:\n");
    for (int j = 0; j < size; j++) {
        for(int i = 0; i < size; i++) {
            printf("%d ", graph.matrix[j][i]);
        }
        printf("\n");
    }

    printf("\n");
    showVerticeConnectivity(&graph, size);

    printf("\n");
    showGraphConnectivity(&graph, size);

    for(int i = 0; i < size; i++) {
        free(graph.matrix[i]);
        free(graph.words[i]);
    }
    free(graph.matrix);
    free(graph.words);

    return 0;
}