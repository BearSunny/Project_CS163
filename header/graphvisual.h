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
};

class Graph {
    private:
        std::vector<GraphNode*> nodes;
        int frameCounting = 0;
        int findParent(std::vector<int>& parent, int node);
        void unionNodes(std::vector<int>& parent, std::vector<int>& rank, int u, int v);
        int getNodeIndex(GraphNode* node);
    public:
    void clearGraph();
    ~Graph();

    void addNode(GraphNode* node);
    void addNode();
    int addEdge(int from, int to, int weight);
    void randomize(int nodeCount, int maxX, int maxY);
    void initFromFile(const char* filename);
    void resetMark();
    int connectedComponent();
    void mstKruskal();
    void drawGraph(Font font, std::vector<Color> color={});
    void updatePositions();
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
        
        Button connectedComponentButton;
        Button mstKruskalButton;
        
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
        void draw();
        int handle();
};