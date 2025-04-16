#include "../header/graphvisual.h"


/* bool areCollinear(GraphNode* a, GraphNode* b, GraphNode* c)
{
    return (b->pos.y - a->pos.y) * (c->pos.x - b->pos.x) == (c->pos.y - b->pos.y) * (b->pos.x - a->pos.x);
} */

Graph::Graph()
{
    for (int i = 0; i < 200; i++)
    {
        checkValidNode[i] = nullptr;
    }
}

Graph::~Graph() {
    clearGraph();
}

void Graph::clearGraph() {
    for (int i = 0; i < int(nodes.size()); i++) {
        delete nodes[i];
    }
    this->nodes.clear();
    this->edges.clear();

    this->frameCounting = 0;
    for (int i = 0; i < 200; i++)
    {
        checkValidNode[i] = nullptr;
    }
}

int Graph::findParent(std::vector<int>& parent, int node) {
    if (parent[node] != node)
        parent[node] = findParent(parent, parent[node]);
    return parent[node];
}

void Graph::unionNodes(std::vector<int>& parent, std::vector<int>& rank, int u, int v) {
    int root_u = findParent(parent, u);
    int root_v = findParent(parent, v);
    if (rank[root_u] > rank[root_v]) {
        parent[root_v] = root_u;
    } else if (rank[root_u] < rank[root_v]) {
        parent[root_u] = root_v;
    } else {
        parent[root_v] = root_u;
        rank[root_u]++;
    }
}

int Graph::getNodeIndex(GraphNode* node) {
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it != nodes.end()) {
        return (int)std::distance(nodes.begin(), it);
    }
    return -1; // Return -1 if node is not found (should not happen)
}

void Graph::addNode(GraphNode* node) {
    nodes.push_back(node);
}

void Graph::addNode() {
    auto* node = new GraphNode();
    int i = 0;
    for (i; i < 200; i++)
    {
        if (checkValidNode[i] == nullptr)
        {
            break;
        }
    }
    node->data = i;
    checkValidNode[i] = node;
    node->pos = {500.f + rand() % 950, 100.f + rand() % 650};

    /* do {
        node->pos = {500.f + rand() % 950, 100.f + rand() % 650}; // Randomize node position
    } while (!isNodeValid(node)); // Repeat until the node's position is valid
 */
    addNode(node);
}

/* bool Graph::isNodeValid(GraphNode* newNode)
{
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            if (areCollinear(nodes[i], nodes[j], newNode)) {
                return false; // The new node creates a collinear line
            }
        }
    }
    return true; // The new node is valid
} */


int Graph::addEdge(int from, int to, int weight) 
{
    if (checkValidNode[from] != nullptr && checkValidNode[to] != nullptr && from != to && weight != 0)
    {
        GraphNode* fr = checkValidNode[from];
        GraphNode* t = checkValidNode[to];
    
        bool edgeExists = false;
        // Check if the edge already exists
        for (auto& edge : fr->neighbors) 
        {
            if (edge.neighborNode == t) 
            {
                edgeExists = true;
                edge.weight = weight;
                break;
            }
        }
            
        for (auto& edge : t->neighbors) 
        {
            if (edge.neighborNode == fr) 
            {
                edgeExists = true;
                edge.weight = weight;
                break;
            }
        }

        for (int i = 0; i < edges.size(); i++)
        {
            auto [w, u, v, cond] = edges[i];

            if (u == fr && v == t)
            {
                edges[i] = make_tuple(weight, u, v, cond);
                break;
            }

            else if (u == t && v == fr)
            {
                edges[i] = make_tuple(weight, u, v, cond);
                break;
            }
        }

        if (!edgeExists) 
        {
            fr->neighbors.push_back({t, weight, 0});
            t->neighbors.push_back({fr, weight, 0});
            edges.push_back(make_tuple(weight, fr, t, false));
            return 1;
        }
    }

    return 0;
}

int Graph::addUserEdge(int from, int to, int weight)
{
    if (checkValidNode[from] == nullptr || checkValidNode[to] == nullptr)
    {
        return -1;
    }
    return addEdge(from, to, weight);
}

void Graph::randomize(int nodeCount, int edgeCount, int maxWeight) {
    clearGraph();

    // Add all nodes
    for (int i = 0; i < nodeCount; ++i) 
    {
        addNode();
    }

    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<vector<bool>> check(nodeCount, vector<bool>(nodeCount, false));
    // Generate all possible unique edges
    std::vector<std::pair<int, int>> possibleEdges;
    for (int from = 0; from < nodeCount; ++from) {
        for (int to = 0; to < nodeCount; ++to) {
            if (from != to && !check[from][to]) { // No self-loops
                possibleEdges.emplace_back(from, to);
                check[from][to] = check[to][from] = true;
            }
        }
    }

    // Shuffle edges randomly
    std::random_shuffle(possibleEdges.begin(), possibleEdges.end());

    // Add edges to the graph
    for (int i = 0; i < edgeCount && i < possibleEdges.size(); ++i) 
    {
        int from = possibleEdges[i].first;
        int to = possibleEdges[i].second;
        int weight = rand() % maxWeight + 1;
        std::cout << from << " " << to << " " << weight << std::endl;
        addEdge(from, to, weight);
    }
}

void Graph::initFromFile(const char* filename) {
    clearGraph();
    std::ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file" << endl;
        return;
    }
    int n;
    file >> n;

    for (int i = 0; i < n; ++i) {
        addNode();
    }

    cout << "Add node success" << endl;
    
    int weight;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> weight;
            addEdge(i, j, weight);
        }
    }

    cout << "Add edge success" << endl;

    file.close();
}

void Graph::resetMark() {
    for(auto node : nodes) {
        for(auto &edge : node->neighbors) {
            edge.highlight = false;
        }
        node->color = false;
    }

    for (int i = 0; i < edges.size(); i++)
    {
        auto [weight, u, v, cond] = edges[i];
        edges[i] = make_tuple(weight, u, v, false);
    }
}
int Graph::connectedComponent() {
    resetMark();
    std::vector<bool> visited(nodes.size(), false);
    int currentColor = 0;
    makeNodeEdge();
    for (int i = 0; i < nodes.size(); ++i) {
        if (!visited[i]) {
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            nodes[i]->color = currentColor;

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                for (auto& edge : nodes[node]->neighbors) {
                    int neighborIndex = getNodeIndex(edge.neighborNode);
                    if (!visited[neighborIndex]) {
                        q.push(neighborIndex);
                        visited[neighborIndex] = true;
                        nodes[neighborIndex]->color = currentColor;
                    }
                }
            }
            currentColor++;
        }
    }
    return currentColor;
}
int Graph::mstKruskal() {
    resetMark();
    //std::vector<std::tuple<int, int, int, bool>> edges;

    /* std::vector<std::vector<bool>> check(nodes.size(), std::vector<bool>(nodes.size(), false));
    
    for (int i = 0; i < nodes.size(); ++i) {
        for (auto& edge : nodes[i]->neighbors) {
            int neighbor = getNodeIndex(edge.neighborNode);
            if (!check[i][neighbor])
            {
                edges.push_back({ edge.weight, i, neighbor, false });
                check[i][neighbor] = check[neighbor][i] = true;
            }
        }
    } */


    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(nodes.size());
    std::vector<int> rank(nodes.size(), 0);

    for (int i = 0; i < nodes.size(); ++i) {
        parent[i] = i;
    }

    // Kruskal's algorithm
    int i = 0, ma = nodes.size() - 1;
    for (i; i < edges.size() && i < ma; i++) {
        auto [weight, frm, to, cond] = edges[i];
        int u = getNodeIndex(frm);
        int v = getNodeIndex(to);
        if (findParent(parent, u) != findParent(parent, v)) {
            unionNodes(parent, rank, u, v);
            
            // Highlight the nodes and the edges in the MST
            nodes[u]->color = true;
            nodes[v]->color = true;

            /* for (auto& edge : nodes[u]->neighbors) {
                if (getNodeIndex(edge.neighborNode) == v) {
                    edge.highlight = true;
                    break;
                }
            }
            for (auto& edge : nodes[v]->neighbors) {
                if (getNodeIndex(edge.neighborNode) == u) {
                    edge.highlight = true;
                    break;
                }
            } */

            edges[i] = std::make_tuple(weight, frm, to, true);
            nodes[u]->color = true;
        }
    }
    return i;
}

void Graph::drawGraph(Font font, std::vector<Color> color) {
    if(!color.empty()) {
        /* for (auto node : nodes) {
            for (auto edge : node->neighbors) {
                DrawLineEx(node->pos, edge.neighborNode->pos, 2, color[node->color]);
                Vector2 mid = { (node->pos.x + edge.neighborNode->pos.x) / 2, (node->pos.y + edge.neighborNode->pos.y) / 2 };
                DrawTextEx(font, TextFormat("%d", edge.weight), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
            }
        } */

        for (auto [weight, u, v, cond]:edges)
        {
            DrawLineEx(u->pos, v->pos, 2, color[u->color]);
            Vector2 mid = { (u->pos.x + v->pos.x) / 2, (u->pos.y + v->pos.y) / 2 };
            DrawTextEx(font, TextFormat("%d", weight), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
        }

        for(int i = 0; i < nodes.size(); i++) {
            Vector2 size = MeasureTextEx(font, TextFormat("%d", i), 15, 2);
            DrawCircleV(nodes[i]->pos, 15, color[nodes[i]->color]);
            printf("Color: (%d, %d, %d, %d)\n", color[nodes[i]->color].r, color[nodes[i]->color].g, color[nodes[i]->color].b, color[nodes[i]->color].a);
            DrawTextEx(font, TextFormat("%d", nodes[i]->data), {nodes[i]->pos.x - size.x/2, nodes[i]->pos.y - size.y/2}, 15, 2, BLACK);
        }

    } 
    else 
    {
       /*  for (auto node : nodes) {
            for (auto edge : node->neighbors) {
                DrawLineEx(node->pos, edge.neighborNode->pos, 2, edge.highlight ? RED : THEME.LINE);
                Vector2 mid = { (node->pos.x + edge.neighborNode->pos.x) / 2, (node->pos.y + edge.neighborNode->pos.y) / 2 };
                DrawTextEx(font, TextFormat("%d", edge.weight), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
            }
        } */

        for (auto [weight, u, v, cond]:edges)
        {
            DrawLineEx(u->pos, v->pos, 2, cond ? RED : THEME.LINE);
            Vector2 mid = { (u->pos.x + v->pos.x) / 2, (u->pos.y + v->pos.y) / 2 };
            DrawTextEx(font, TextFormat("%d", weight), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
        }

        for(int i = 0; i < nodes.size(); i++) {
            Vector2 size = MeasureTextEx(font, TextFormat("%d", i), 15, 2);
            DrawCircleV(nodes[i]->pos, 15, nodes[i]->color ? RED : THEME.NODE);
            DrawTextEx(font, TextFormat("%d", nodes[i]->data), {nodes[i]->pos.x - size.x/2, nodes[i]->pos.y - size.y/2}, 15, 2, BLACK);
        }
    }
}

void Graph::updatePositions() {
    float repulsion_const = 500000.0f;
    float attraction_const = 0.1f;

    for (auto node : nodes) {
        Vector2 force = {0, 0};

        for (auto other : nodes) {
            if (node != other) {
                float dx = node->pos.x - other->pos.x;
                float dy = node->pos.y - other->pos.y;
                float dis = sqrt(dx * dx + dy * dy);

                if (dis > 0) {
                    float repulsion = repulsion_const / (dis * dis);
                    force.x += repulsion * dx / dis;
                    force.y += repulsion * dy / dis;
                }
            }
        }

        //Spring Force
        for (auto edge : node->neighbors) {
            float dx = node->pos.x - edge.neighborNode->pos.x;
            float dy = node->pos.y - edge.neighborNode->pos.y;
            float dis = sqrt(dx * dx + dy * dy);

            if (dis > 0) {
                float attraction = attraction_const * (dis - edge.weight);
                force.x -= attraction * dx / dis;
                force.y -= attraction * dy / dis;
            }
        }

        if(this->frameCounting > 2000) continue;
        
        node->pos.x += force.x;
        node->pos.y += force.y;

        node->pos.x = std::max(std::min(1200.f, node->pos.x), 380.f);
        node->pos.y = std::max(std::min(650.f, node->pos.y), 150.f);
        frameCounting++;
    }
}

GraphVisualize::GraphVisualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isUpdateChosen = false;
    this->isDeleteChosen = false;
    this->playbackState = Playing;
    std::string infor = "";
    this->numComponent = 0;

    this->createButton = Button({11.5, 434.5, 110, 30}, "Create", -1, BLACK, 20, font);
    this->connectedComponentButton = Button({11.5, 488, 110, 30}, "Component", -1, BLACK, 20, font);
    this->mstKruskalButton = Button({11.5, 541.5, 110, 30}, "MST", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 511, 110, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({156.5, 552 , 110, 30}, "LoadFile", -1, BLACK, 20, font);
    this->inputEdges = InputStr(151.5, 422, 120, 25, "Num edge", 20, this->font);
    this->inputNodes = InputStr(151.5, 466, 120, 25, "Num vertex", 20, this->font);

    this->fromInput = InputStr(20, 200, 110, 30, "From", 20, this->font);      // Input for "From"
    this->toInput = InputStr(150, 200, 110, 30, "To", 20, this->font);        // Input for "To"
    this->weightInput = InputStr(280, 200, 110, 30, "Weight", 20, this->font); // Input for "Weight"
    this->submitButton = Button({410, 200, 110, 30}, "Update Edge", -1, BLACK, 20, font);

    this->nodeNumberInput = InputStr(50, 290, 110, 30, "Node #", 20, this->font);
    this->updateValueInput = InputStr(180, 290, 110, 30, "Node Value", 20, this->font);
    this->updateSubmitButton = Button({310, 290, 110, 30}, "Update Node", -1, BLACK, 20, font);
    
    //this->nodeNumberInput = InputStr(50, 290, 110, 30, "Node #", 20, this->font);
    //this->updateValueInput = InputStr(180, 290, 110, 30, "Node Value", 20, this->font);
    this->deleteSubmitButton = Button({310, 290, 110, 30}, "Delete Node", -1, BLACK, 20, font);

    this->cancelButton = Button({250, 320, 110, 30}, "Cancel", -1, BLACK, 20, font);

    this->addVertexButton = Button({11.5, 370, 110, 30}, "Add Vertex", -1, BLACK, 20, font);
    this->updateButton = Button({156.5, 370, 110, 30}, "Update", -1, BLACK, 20, font);
    this->deleteButton = Button({301.5, 370, 110, 30}, "Delete", -1, BLACK, 20, font);

}

std::vector<Color> GraphVisualize::generateRandomColors(int n) {
    std::vector<Color> colors;
    colors.reserve(n);

    for (int i = 0; i < n; ++i) {
        unsigned char r = GetRandomValue(0, 255);
        unsigned char g = GetRandomValue(0, 255);
        unsigned char b = GetRandomValue(0, 255);
        colors.push_back(Color{r, g, b, 255});
    }

    return colors;
}

void GraphVisualize::drawButton() 
{
/* 
    fromInput.draw();
    fromInput.update();
    toInput.draw();
    toInput.update();
    weightInput.draw();
    weightInput.update();
    submitButton.draw(70);

    // Draw buttons in the lower blank space
    addVertexButton.draw();
    updateButton.draw();
    deleteButton.draw();


    this->createButton.draw(50);
    this->connectedComponentButton.draw(50);
    this->mstKruskalButton.draw(50);

    if(this->isCreateChosen) {
        inputNodes.draw();
        inputNodes.update();
        inputEdges.draw();
        inputEdges.update();
        DrawLineEx({126, 501}, {297, 501}, 1.3, THEME.SEPERATOR);
        this->randomButton.draw(50);
        this->loadFileButton.draw(50);
    } */
   drawForm();
   drawButtons();
}

void GraphVisualize::drawForm() 
{
    fromInput.draw();
    fromInput.update();
    toInput.draw();
    toInput.update();
    weightInput.draw();
    weightInput.update();
    submitButton.draw(50);
}

void GraphVisualize::drawButtons() 
{
    addVertexButton.draw();
    updateButton.draw();
    deleteButton.draw();

    createButton.draw(50);
    connectedComponentButton.draw(50);
    mstKruskalButton.draw(50);
    
    if (isCreateChosen) 
    {
        inputNodes.draw();
        inputNodes.update();
        inputEdges.draw();
        inputEdges.update();
        DrawLineEx({126, 501}, {297, 501}, 1.3, THEME.SEPERATOR);
        randomButton.draw(50);
        loadFileButton.draw(50);
    }

    if (isUpdateChosen || isDeleteChosen)
    {
        nodeNumberInput.draw();
        nodeNumberInput.update();
        cancelButton.draw(50);
    }

    if (isUpdateChosen)
    {
        //cout << "Form update" << endl;
        isCreateChosen = false;
        isDeleteChosen = false;
        updateValueInput.draw();
        updateValueInput.update();
        updateSubmitButton.draw(30);
    }

    if (isDeleteChosen)
    {
        isCreateChosen = false;
        isUpdateChosen = false;
        deleteSubmitButton.draw(30);
    }
}

void GraphVisualize::drawGraph() {
    this->graph.drawGraph(this->font, this->colorComponent);
}

void GraphVisualize::draw() {
    drawSideBar(0, "", {}, this->infor, this->progressBar, this->font);
    drawButton();
    drawGraph();
}

int GraphVisualize::handle(float deltaTime) {
    //cout << isUpdateChosen << endl;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (this->submitButton.getIsHovered()) 
        {
            // Retrieve values from the inputs
            int from = std::stoi(fromInput.getText());
            int to = std::stoi(toInput.getText());
            int weight = std::stoi(weightInput.getText());
    
            // Add edge to the graph using these values
            graph.addUserEdge(from, to, weight);
        }

        if (this->addVertexButton.getIsHovered())
        {
            graph.addNode();
        }

        if (this->updateButton.getIsHovered())
        {
            this->isUpdateChosen = true;
        }

        if (this->deleteButton.getIsHovered())
        {
            this->isDeleteChosen = true;
        }

        /* if (this->isUpdateChosen && this->updateSubmitButton.getIsHovered())
        {
            int nodeNumber = std::stoi(nodeNumberInput.getText());
            int n_val = std::stoi(updateValueInput.getText());
            graph.updateNodeValue(nodeNumber, n_val);
            isUpdateChosen = false;
            cout << "Update" << endl;
        } */

        /* if (this->isDeleteChosen && this->deleteSubmitButton.getIsHovered())
        {
            int nodeNumber = std::stoi(nodeNumberInput.getText());
            graph.deleteNode(nodeNumber);
            isDeleteChosen = false;
        }

        if (this->cancelButton.getIsHovered())
        {
            isCreateChosen = false;
            isUpdateChosen = false;
            isDeleteChosen = false;
        }
     */
        if (this->createButton.getIsHovered()) 
        {
            this->graph.resetMark();
            this->isCreateChosen = true;
            this->numComponent = 0;
            this->colorComponent.clear();
        }
        if (this->connectedComponentButton.getIsHovered()) {
            this->graph.resetMark();
            this->isCreateChosen = false;
            connectedComponent();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (this->mstKruskalButton.getIsHovered()) {
            this->graph.resetMark();
            this->isCreateChosen = false;
            this->numComponent = 0;
            this->colorComponent.clear();
            timeElapsed = 0.0f;
            playbackState = Playing;
            mstKruskal();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    if (this->updateSubmitButton.handle())
    {
        int nodeNumber = std::stoi(nodeNumberInput.getText());
        int n_val = std::stoi(updateValueInput.getText());
        graph.updateNodeValue(nodeNumber, n_val);
        isUpdateChosen = false;
        //cout << "Update" << endl;
    }

    if (this->deleteSubmitButton.handle())
    {
        int nodeNumber = std::stoi(nodeNumberInput.getText());
        graph.deleteNode(nodeNumber);
        isDeleteChosen = false;
    }

    if (this->cancelButton.handle())
    {
        isCreateChosen = false;
        isUpdateChosen = false;
        isDeleteChosen = false;
    }


    if (this->randomButton.handle()) {
        //cout << "Randommmmmmmmmmmm" << endl;
        this->frames.clear();
        frame_count = 0;
        timeElapsed = 0.0f;
        playbackState = Playing;
        randomize();
        //graph.makeNodeEdge();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    if (this->loadFileButton.handle()) {
        this->frames.clear();
        frame_count = 0;
        timeElapsed = 0.0f;
        playbackState = Playing;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //if(!loadFile()) return 5;
        loadFile();
        //graph.makeNodeEdge();
        return 5;
    }

    this->graph.updatePositions();

    return 0;
}

void GraphVisualize::randomize() {
    std::string v = this->inputNodes.getText();
    std::string e = this->inputEdges.getText();
    if (v.empty() || e.empty() ) return;
    this->inputEdges.changePlaceHolder(e);
    this->inputNodes.changePlaceHolder(v);
    this->infor = TextFormat("Randomize graph with v = %s, e = %s", v.c_str(), e.c_str());
    this->graph.randomize(std::stoi(v), std::stoi(e), 100);
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}

int GraphVisualize::loadFile() {
    std::string path;

    /* // Prompt the user for the file path
    std::cout << "Name of the file to load (Enter to load default graph): ";
    std::getline(std::cin, path);

    // Check if the input is empty
    if (path.empty()) {
        std::cout << "No file selected." << std::endl;
        return 0;
    } */
    path = "Sample/Graph.txt";
    cout << "GEt here\n";
    // Update infor and attempt to load the file
    this->infor = TextFormat("Load graph from file %s", path.c_str());
    cout << "Begin load file\n";
    this->graph.initFromFile(path.c_str());
    cout << "Load success\n";
    // Update progress bar
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
    
    return 1;
}


/* int GraphVisualize::loadFile() {
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                           { "Text Files (.txt .text)", "*.txt *.text",
                               "All Files", "*" },
                           pfd::opt::force_path);
    if (f.result().empty()) {
        return 0;
    }
       
    auto path = f.result().back();
    this->infor = TextFormat("Load graph from file %s", path.c_str());
    this->graph.initFromFile(path.c_str());
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
    return 1;
} */

void GraphVisualize::connectedComponent() {
    this->infor = TextFormat("Find connected Component");
    this->numComponent = this->graph.connectedComponent();
    this->colorComponent = generateRandomColors(this->numComponent);
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}

void GraphVisualize::mstKruskal() {
    this->infor = TextFormat("Find minimun spanning tree with Kruskal algorithm");
    std::cout << "Krukal" << std::endl;
    int stop = frames.size() + this->graph.mstKruskal();
    std::cout << "Done krukal" << std::endl;
    addFrame();
    std::cout << "Done add frame" << std::endl;
    std::cout << frames.size() << std::endl;
    std::cout << frame_count << std::endl;
    while (frame_count <= stop)
    {
        float dTime = GetFrameTime();
        update(dTime);
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        drawFrame();
        //draw();
        EndDrawing();
    }
    this->progressBar.updateMaxStep(1);
    this->progressBar.updateStep(1);
}

void Graph::updateNodeValue(int nodeNum, int n_val)
{
    GraphNode* tmp = getNode(nodeNum);
    GraphNode* n_tmp = getNode(n_val);
    if (tmp == nullptr || n_tmp != nullptr)
    {
        return;
    }
    tmp->data = n_val;
    updateCheckArray(n_val, nodeNum, tmp);
}

void Graph::updateCheckArray(int change, int old, GraphNode* node)
{
    checkValidNode[change] = node;
    if (old != -1) checkValidNode[old] = nullptr;
}

void Graph::deleteNode(int nodeNumber)
{
    GraphNode* tmp = getNode(nodeNumber);
    if (tmp == nullptr)
    {
        return;
    }
    vector<GraphNode*> q;

    for (auto e:tmp->neighbors)
    {
        q.push_back(e.neighborNode);
    }

    for (auto node:q)
    {
        int t = 0;
        for (t; t < node->neighbors.size(); t++)
        {
            if (node->neighbors[t].neighborNode == tmp)
            {
                node->neighbors.erase(node->neighbors.begin() + t);
                t--;
            }
        }
    }
    int i = 0;
    for (i; i < edges.size(); i++)
    {
        auto [weight, u, v, cond] = edges[i];

        std::cout << getNodeIndex(u) << " " << getNodeIndex(v) << " " << weight << std::endl;

        if (u == tmp || v == tmp)
        {
            edges.erase(edges.begin() + i);
            cout << "Delete: " << getNodeIndex(u) << " " << getNodeIndex(v) << " " << weight << std::endl;
            i--;
        }
    }

    for (int i = 0; i < nodes.size(); i++)
    {
        if (tmp == nodes[i])
        {
            nodes.erase(nodes.begin() + i);
        }
    }

    updateCheckArray(nodeNumber, -1, nullptr);
}

GraphNode* Graph::getNode(int index)
{
    return checkValidNode[index];
}


void Graph::makeNodeEdge()
{
    for (int i = 0; i < edges.size(); i++)
    {
        auto [weight, u, v, cond] = edges[i];
        u->neighbors.push_back({v, weight, cond});
        v->neighbors.push_back({u, weight, cond});
    }
}

void GraphVisualize::update(float deltaTime) 
{
    if (playbackState == Playing) 
    {
        timeElapsed += deltaTime;
        //std::cout << "Time elapsed: " << timeElapsed << "  ";
        //std::cout << "Duration: " << duration << std::endl;

        // Move to the next frame based on frameDuration
        if (timeElapsed > duration && frame_count < frames.size() - 1) 
        {
            std::cout << "Frame count: " << frame_count << std::endl;
            frame_count++;
            //displayFrame(animationFrames[currentFrame]); // Render next frame
            timeElapsed = 0.0f;
        } 
        else if (timeElapsed > duration && frame_count == frames.size() - 1) 
        {
            frame_count++;
            playbackState = Paused; // Stop at the last frame
            timeElapsed = 0.0f;
        }
    }

    int flag = progressBar.handle();

    if (flag == 0)
    {
        playbackState = (playbackState == Playing) ? Paused : Playing;
    }

    else if (flag == -1)
    {
        playbackState = Paused;
        frame_count--;
    }

    else if (flag == 1)
    {
        playbackState = Paused;
        frame_count++;
    }

   /*  // Handle playback controls
    if (playPauseButton.handle()) {
        playbackState = (playbackState == Playing) ? Paused : Playing;
        playPauseButton.setContent(playbackState == Playing ? "Pause" : "Play");
    }

    if (skipBackButton.handle() && currentFrame > 0) {
        currentFrame--;
        displayFrame(animationFrames[currentFrame]);
    }

    if (skipForwardButton.handle() && currentFrame < animationFrames.size() - 1) {
        currentFrame++;
        displayFrame(animationFrames[currentFrame]);
    } */
}


std::vector<std::tuple<int, GraphNode*, GraphNode*, bool>> Graph::getEdges()
{
    return edges;
}

void GraphVisualize::addFrame()
{
    std::vector<std::tuple<int, GraphNode*, GraphNode*, bool>> Edges = graph.getEdges();

    frame_count = frames.size();

    for (int i = 0; i < Edges.size(); i++)
    {
        auto [weight, u, v, cond] = Edges[i];
        Frame frame;
        frame.type = 0;
        int t = graph.getNodeIndex(u);
        int w = graph.getNodeIndex(v);
        frame.comp = {t, w, cond, weight};
        frames.push_back(frame);
    }
}

std::vector<GraphNode*> Graph::getNodes()
{
    return this->nodes;
}

void GraphVisualize::drawFrame()
{
    drawSideBar(0, "", {}, this->infor, this->progressBar, this->font);
    drawButton();

    //if (frame_count >= frames.size()) return;

    std::vector<GraphNode*> Nodes = graph.getNodes();

    for (int i = 0; i < frames.size(); i++)
    {
        Frame frame = frames[i];

        if (i < frame_count)
        {
            DrawLineEx(Nodes[frame.comp[0]]->pos, Nodes[frame.comp[1]]->pos, 2, frame.comp[2] ? RED : THEME.LINE);
            Vector2 mid = { (Nodes[frame.comp[0]]->pos.x + Nodes[frame.comp[1]]->pos.x) / 2, (Nodes[frame.comp[0]]->pos.y + Nodes[frame.comp[1]]->pos.y) / 2 };
            DrawTextEx(font, TextFormat("%d", frame.comp[3]), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
        }

        else if (i == frame_count)
        {
            DrawLineEx(Nodes[frame.comp[0]]->pos, Nodes[frame.comp[1]]->pos, 2, BLUE);
            Vector2 mid = { (Nodes[frame.comp[0]]->pos.x + Nodes[frame.comp[1]]->pos.x) / 2, (Nodes[frame.comp[0]]->pos.y + Nodes[frame.comp[1]]->pos.y) / 2 };
            DrawTextEx(font, TextFormat("%d", frame.comp[3]), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
        }

        else
        {
            DrawLineEx(Nodes[frame.comp[0]]->pos, Nodes[frame.comp[1]]->pos, 2, THEME.LINE);
            Vector2 mid = { (Nodes[frame.comp[0]]->pos.x + Nodes[frame.comp[1]]->pos.x) / 2, (Nodes[frame.comp[0]]->pos.y + Nodes[frame.comp[1]]->pos.y) / 2 };
            DrawTextEx(font, TextFormat("%d", frame.comp[3]), {mid.x, mid.y}, 15, 2, THEME.WEIGHT);
        }
    }

    for (int i = 0; i < Nodes.size(); i++) 
    {
        Vector2 size = MeasureTextEx(font, TextFormat("%d", i), 15, 2);
        DrawCircleV(Nodes[i]->pos, 15, THEME.NODE);
        DrawTextEx(font, TextFormat("%d", Nodes[i]->data), {Nodes[i]->pos.x - size.x/2, Nodes[i]->pos.y - size.y/2}, 15, 2, BLACK);
    }
}