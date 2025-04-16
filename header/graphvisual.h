#pragma once

#include "GraphButton.h"
//#include "portable-file-dialogs.h"

enum PlaybackState
{
    Playing,
    Paused
};

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

struct Frame
{
    int type;
    std::vector<int> comp; 
};

class Graph {
    private:
        std::vector<GraphNode*> nodes;
        std::vector<std::tuple<int, GraphNode*, GraphNode*, bool>> edges;
        int frameCounting = 0;
        GraphNode* checkValidNode[200];
        int findParent(std::vector<int>& parent, int node);
        void unionNodes(std::vector<int>& parent, std::vector<int>& rank, int u, int v);
    public:
    Graph();
    GraphNode* getNode(int index);
    int getNodeIndex(GraphNode* node);
    void clearGraph();
    ~Graph();

    void addNode(GraphNode* node);
    void addNode();
    //bool isNodeValid(GraphNode* newNode);
    int addEdge(int from, int to, int weight);
    int addUserEdge(int from, int to, int weight);
    void randomize(int nodeCount, int maxX, int maxY);
    void initFromFile(const char* filename);
    void resetMark();
    int connectedComponent();
    int mstKruskal();
    void drawGraph(Font font, std::vector<Color> color={});
    void updatePositions();
    void updateNodeValue(int nodeNum, int n_val);
    void updateCheckArray(int change, int old, GraphNode* node);
    void deleteNode(int nodeNumber);
    void makeNodeEdge();
    std::vector<GraphNode*> getNodes();
    std::vector<std::tuple<int, GraphNode*, GraphNode*, bool>> getEdges();
};

class GraphVisualize {
    private:

        Graph graph;
        int numComponent;
        std::vector<Color> colorComponent;
        
        std::vector<Frame> frames;
        int frame_count = 0;

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

        float timeElapsed = 0.0f;
        float duration = 3.0f;
        PlaybackState playbackState;
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
        int handle(float deltaTime);
        void update(float deltaTime);
        void drawFrame();
        void addFrame();
};

//bool areCollinear(GraphNode* a, GraphNode* b, GraphNode* c);