#pragma once

#include "GraphButton.h"
//#include "portable-file-dialogs.h"

struct GraphNode {
    struct Edge {
        GraphNode* neighborNode;
        int weight;
        bool highlight = 0;
    };
    std::vector<Edge> neighbors;
    Vector2 pos;
    int color = 0;
    int data;
};

class Graph {
    private:
        std::vector<GraphNode*> nodes;
        int frameCounting = 0;
        GraphNode* checkValidNode[200];
        int findParent(std::vector<int>& parent, int node);
        void unionNodes(std::vector<int>& parent, std::vector<int>& rank, int u, int v);
        int getNodeIndex(GraphNode* node);
    public:
        Graph();
        GraphNode* getNode(int index);
    void clearGraph();
    ~Graph();

    void addNode(GraphNode* node);
    void addNode();
    int addEdge(int from, int to, int weight);
    int addUserEdge(int from, int to, int weight);
    void randomize(int nodeCount, int maxX, int maxY);
    void initFromFile(const char* filename);
    void resetMark();
    int connectedComponent();
    void mstKruskal();
    void drawGraph(Font font, std::vector<Color> color={});
    void updatePositions();
    void updateNodeValue(int nodeNum, int n_val);
    void updateCheckArray(int change, int old, GraphNode* node);
    void deleteNode(int nodeNumber);
};

class GraphVisualize {
    private:
        Graph graph;
        int numComponent;
        std::vector<Color> colorComponent;
        
        ProgressBar progressBar;
        std::string infor;
        
        Button createButton;
        InputStr inputNodes;
        InputStr inputEdges;
        Button randomButton;
        Button loadFileButton;
        bool isCreateChosen;
        bool isUpdateChosen;
        bool isDeleteChosen;
        
        Button connectedComponentButton;
        Button mstKruskalButton;

        InputStr fromInput;
        InputStr toInput;
        InputStr weightInput;
        InputStr nodeNumberInput;
        InputStr updateValueInput;
        Button submitButton;
        Button addVertexButton;
        Button updateButton;
        Button deleteButton;
        Button updateSubmitButton;
        Button deleteSubmitButton;
        Button cancelButton;

        
        Font font = LoadFontEx("./Font/SF-Pro-Display-Regular.otf", 40, 0, 255);
        std::vector<Color> generateRandomColors(int n);
    public:
        GraphVisualize(Font font = FONT);
        void randomize();
        int loadFile();
        void connectedComponent();
        void mstKruskal();
        void drawGraph();
        void drawButton();
        void drawForm();
        void drawButtons();
        void draw();
        int handle();
};