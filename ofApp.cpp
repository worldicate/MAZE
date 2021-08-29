#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush

    ofBackground(255,255,255);
   
    load_flag = 0;
 
    isdfs = false;
    isbfs = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}


////////////////////////////////////////////////////////////////
///미로의 사이즈를 아래와 같이 size 인자로 바꿀 수 있습니다!!!!
/////////////////////////////////////////////////////////////////

#define size 5


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127, 23, 31);  // Set the drawing color to brownl

      // Draw shapes for ceiling and floor
      // ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
     // ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(size * (0.5));

    int i, j;
    int pointx = size;
    int pointy = size;

    if (load_flag) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (maze[i][j].top == -1) {
                    ofDrawLine(pointx - size, pointy - size, pointx + size, pointy - size);

                }
                if (maze[i][j].down == -1) {
                    ofDrawLine(pointx - size, pointy + size, pointx + size, pointy + size);
                }
                if (maze[i][j].left == -1) {
                    ofDrawLine(pointx - size, pointy - size, pointx - size, pointy + size);

                }
                if (maze[i][j].right == -1) {
                    ofDrawLine(pointx + size, pointy - size, pointx + size, pointy + size);

                }
                pointx += size * 2;
            }
            pointx = size;
            pointy += size * 2;
        }
    }
    if (isdfs) {
        ofSetLineWidth(size * (0.7));
        drawDFS(&g);

    }

    if (isbfs) {
        ofSetLineWidth(size * (0.7));
        drawBFS(&g);
    }
    if (isdfs || isbfs) {
        ofSetLineWidth(size);
        ofSetColor(ofColor::red);
        ofDrawRectangle(size - 2, size - 1, size, size);
        ofSetColor(ofColor::green);
        ofDrawRectangle(size - 2 + 2 * size * (WIDTH - 1), size - 1 + size * 2 * (HEIGHT - 1), size, size);

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        //draw_flag = 0;
        load_flag = 0;
        // Free the dynamically allocated memory exits.
        freeMemory();
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
        
        isdfs = true;
        isbfs = false;
        dfs(&g);
        /*for (int j = 0; j < num_of_dot; j++) {
        //    ofSetColor(0);
            cout << "Selcted Dot Coordinate is (" << dots[j].x << ", " << dots[j].y << ")" << endl;q
         //   ofDrawCircle(dots[j].x, dots[j].y, 10);
        }*/

     
    }
    if (key == 'b'){
        // 2nd week portion.
        if (!load_flag) return;
        isbfs = true;;
        isdfs = false;
        bfs(&g);
    }
    if (key == 'e'){
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a Maze file (.maz)");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
         //   load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    
 
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
        
    string fileName = openFileResult.getName();
    printf("file name is %s \n", fileName.c_str());
    string  filePath = openFileResult.getPath();
    printf("Open\n");
    ofFile file(fileName);
    if (!file.exists()) {
        printf("Target file does not exists.");
        return;
    }
    else {
        printf("We found the target file.");
        load_flag = 1;
        isbfs = false;
        isdfs = false;
        //     draw_flag = 1;
    }
    printf("\n");
    readFile(filePath.c_str());

}

bool ofApp::readFile(const char* filename) {
    HEIGHT = 0;
    WIDTH = 0;
    int res;
    int flag = 0;
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("file open fail"); return false;
    }
    while ((res = fgetc(fp)) != EOF) {
        //   printf("%c", res);
        if (flag == 0) {
            if (res == '\n') { flag++; }
            if (res == '-') WIDTH++;
        }
        else {
            if (res == '\n') flag++;
        }
    }
    fclose(fp);

    flag /= 2;
    HEIGHT = flag;

    //printf("%d %d \n", HEIGHT, WIDTH);
    maze = (Vertex**)malloc(sizeof(Vertex*) * HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        maze[i] = (Vertex*)malloc(sizeof(Vertex) * WIDTH);
    }
    makedrawmaze(filename);
    //  printmaze();
    makemazegraph();
    /*for (int i = 0; i < WIDTH * HEIGHT; i++) {
        printedge(&g, i);

    }*/
    return true;

}
void ofApp::freeMemory() {
    for (int i = 0; i < HEIGHT; i++) {
        free(maze[i]);
    }
    free(maze);

    graphdestroy(&g);
   free(BStoreAllpath);
    free(BStoreMinpath);
    free(StoreAllpath);
    free(StoreMinpath);
    free(visited);
    free(parent);
}

void  ofApp::graphdestroy(graph* g) {
    g->nedges = 0;
    g->nvertices = 0;
    for (int i = 0; i < g->nvertices; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
}


void ofApp::printmaze() {

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d 행 %d열 top %d down %d left %d right %d\n", i, j, maze[i][j].top, maze[i][j].down, maze[i][j].left, maze[i][j].right);
        }
    }
}

void  ofApp::makegraph(graph* g, int num) {

    g->nvertices = num;
    g->nedges = 0;
    g->edges = (edgenode**)malloc(sizeof(edgenode*) *(num));
  //free 잘 조정해줘야 할듯
    for (int i = 0; i < num; i++) {
        g->edges[i] = (edgenode*)malloc(sizeof(edgenode));
        g->edges[i]->next = NULL;
        g->edges[i]->y = -1;
    }
    visited = (int*) malloc(sizeof(int) * (WIDTH * HEIGHT));
    BStoreAllpath = (Cor*)malloc(sizeof(Cor) * (WIDTH * HEIGHT) * (4));
    BStoreMinpath = (Cor*)malloc(sizeof(Cor) * (WIDTH * HEIGHT));
    parent = (int*)malloc(sizeof(int) * (WIDTH * HEIGHT));
    StoreMinpath = (Cor*)malloc(sizeof(Cor) * (WIDTH * HEIGHT));
    StoreAllpath = (Cor*)malloc(sizeof(Cor) * (WIDTH * HEIGHT) * (4));
}

void  ofApp::addedge(graph* g, int fromV, int toV) {
    edgenode* cur = g->edges[fromV];
    edgenode* newnode = (edgenode*)malloc(sizeof(edgenode));
    g->nedges++;
    newnode->next = cur->next;
    newnode->y = cur->y;
    cur->next = newnode;
    cur->y = toV;
    /*
    cur = g->edges[toV];
    newnode = (edgenode*)malloc(sizeof(edgenode));
    newnode->next = cur->next;
    newnode->y = cur->y;
    cur->next = newnode;
    cur->y = fromV;*/
}
//실제 쓰이지 않음. 그래프 정상적으로 작동되는지 만들어봄
void ofApp::printedge(graph* g, int fromV) {
    edgenode* cur = g->edges[fromV];
    printf("vertex :%d ->", fromV);

    while (cur->next != NULL) {
        printf("edge %d ", cur->y);
        cur = cur->next;
    }
    printf("\n");
}


void ofApp::makedrawmaze(const char* filename) {

    FILE* fp = fopen(filename, "r");
    int res;
    if (fp == NULL) { printf("file open fail"); return; }
    for (int i = 0; i < WIDTH; i++) {
        maze[0][i].top = -1;
        maze[HEIGHT - 1][i].down = -1;
    }
    for (int i = 0; i < HEIGHT; i++) {
        maze[i][0].left = -1;
        maze[i][WIDTH - 1].right = -1;
    }

    for (int i = 0; i < 2 * HEIGHT + 1; i++) {

        for (int j = 0; j < 2 * WIDTH + 1; j++) {
            res = fgetc(fp);
            //	printf("%c %d %d \n", res, i,j);
            if (i == 0 || i == 2 * HEIGHT) continue;
            if (j == 0 || j == 2 * WIDTH) continue;
            if (i % 2 == 0) {

                if (res == '+') continue;
                if (res == '-') {
                    maze[(i / 2) - 1][j / 2].down = -1;
                    maze[i / 2][j / 2].top = -1;
                }
                else if (res == ' ') {
                    maze[(i / 2) - 1][j / 2].down = 1;
                    maze[i / 2][j / 2].top = 1;
                }
            }
            else if (i % 2 != 0) {
                //if (res == '+') continue;
                if (j % 2 != 0) continue;
                if (res == '|') {
                    //	printf("find %d %d", i, j);
                    maze[i / 2][j / 2 - 1].right = -1;
                    maze[i / 2][j / 2].left = -1;
                }
                else {
                    maze[i / 2][j / 2 - 1].right = 1;
                    maze[i / 2][j / 2].left = 1;
                }

            }
        }
        res = fgetc(fp);
    }
    fclose(fp);
}
void ofApp::makemazegraph() {
   
    makegraph(&g, WIDTH*HEIGHT);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (maze[i][j].top == 1) {
                if (i - 1 < 0) continue;
                addedge(&g, i * WIDTH + j, (i - 1) * WIDTH + j);
            }
            if (maze[i][j].down == 1) {
                if (i + 1 >= HEIGHT) continue;
                addedge(&g, i * WIDTH + j, (i + 1) * WIDTH + j);
            }
            if (maze[i][j].left == 1) {
                if (j - 1 < 0) continue;
                addedge(&g, i * WIDTH + j, i * WIDTH + j - 1);
            }
            if (maze[i][j].right == 1) {
                if (j + 1 >= WIDTH) continue;
                addedge(&g, i * WIDTH + j, i * WIDTH + j + 1);
            }
        }
    }


}


//DFS 동작을 실행한 후,탐색 결과를 그리는 자료구조
void ofApp::drawDFS(graph* g) 
{
    // DRAW ALL PATHS
    for (int i = 0; i < allidx; i++) {
        ofSetColor(ofColor::gray);
        ofDrawLine(size + (StoreAllpath[i].prenode % WIDTH) * 2 * size, size + (StoreAllpath[i].prenode / WIDTH) * 2 * size, size + (StoreAllpath[i].nextnode % WIDTH) * size * 2, size + (StoreAllpath[i].nextnode / WIDTH) * 2 * size);
 
    }

    // DRAW SHORTEST PATH
    for (int i = 0; i < minidx; i++) {
        ofSetColor(ofColor::black);
        ofDrawLine(size + (StoreMinpath[i].prenode % WIDTH) * 2 * size, size + (StoreMinpath[i].prenode / WIDTH) * 2 * size, size + (StoreMinpath[i].nextnode % WIDTH) * size * 2, size + (StoreMinpath[i].nextnode / WIDTH) * 2 * size);
    }
    ofSetColor(ofColor::red);
}

// DFS 동작 실행하는 자료구조
void ofApp::dfs(graph *g) {
      
      // init a stack S 
      // 스택을 초기화 한다. 
        Stack s;
        StackInit(&s);;
     
        int flag;
        //S.push ( 0 );
        // 시작점인 0번 정점을 스택에 넣는다. 
        Spush(&s, 0);
      
        edgenode* p;

        allidx = 0;
        minidx = 0;
        
        // 모든 visited 배열을 0으로 초기화 한다. 
       for (int i = 0; i < WIDTH*HEIGHT; i++) { visited[i] = 0; }

       //mark v as visited;
       // 시작점인 0번 정점을 방문했다고 표시한다. 
       visited[0] = 1;

        //while ( S != empty )
        // 스택이 EMPTY 되지 않을 때까지 반복
        while (!Sempty(&s)) {
            flag = 0;
            int temp = Speek(&s);
            //if ( S.top == target ) return;
            // 스택의 top이 도착점이면 탐색을 중지한다. 
            if (temp == WIDTH * HEIGHT - 1) {
                flag = 1; break;
            }

            p = g->edges[temp];
            //if (S.top has an unvisited adjacent node)
            // 스택의 탑이 인접한 노드를 갖고 있다면
            while (p->next != NULL) {
                int y = p->y;
                //y= an unvisited, adjacent node to S.top;
                // 그 노드가 아직 방문되지 않았다면
                if (visited[y] == 0) {

                    //mark y as visited;
                    // 해당 노드를 방문 했다고 표시한다. 
                    visited[y] = 1;
                  
                    //S.push(u);
                    Spush(&s, y); flag = 1;
                    // 해당 경로를 다음과 같이 저장한다. 
                    StoreMinpath[minidx++] = { temp,y };
                    StoreAllpath[allidx++] = { temp,y };
                    break;
                }
                p = p->next;
            }
            //else {  S.pop();}
            // 인접한 노드가 없다면 스택에서 pop 한다.( flag는 인접한 노드가 있으면 1을,  없으면 0을 나타낸다.)
            if (!flag) {
                Spop(&s);
                //최단 경로를 저장하기 위한 배열에서도 pop을 해주기 위해 인덱스를 1개 감소시킨다. 
                minidx--;
            }
        }

}



void ofApp:: StackInit(Stack* s) {
    s->top = NULL;
}
bool ofApp:: Sempty(Stack* s) {
    if (s->top == NULL) return true;
    else return false;
}
void ofApp::Spush(Stack* s, int data) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->next = s->top;
    newnode->data = data;
    s->top = newnode;
}
int ofApp::Spop(Stack* s) {
    
    int Deldata = s->top->data;
    Node* Delnode = s->top;
    s->top = s->top->next;
    free(Delnode);
    return Deldata;
}
int ofApp:: Speek(Stack* s) {
    return s->top->data;

}

// BFS 동작 실행하는 자료구조
void ofApp::bfs(graph* g) {
    //Init a queue Q
    // 큐를 초기화 한다. 
    Queue q;
    edgenode* p;
    Queueinit(&q);
    //Q.enqueue(0);
    // 시작점인 0번 정점을 큐를 넣는다. 
    Enqueue(&q, 0);

    //각 정점의 parent를 설정할 배열을 초기화 한다. 
    memset(parent, -1, sizeof(int) * (WIDTH * HEIGHT));
    parent[0] = 0;

    minqidx = 0;
    allqidx = 0;

    // 모든 visited 배열을 0으로 초기화 한다. 
    for (int i = 0; i < HEIGHT * WIDTH; i++) { visited[i] = 0; }
    //mark 0 as visited;
    //시작점인 0번 정점을 방문했다고 표시한다. 
    visited[0] = 1;
    //while ( Q != EMPTY) 
    //// 큐가 EMPTY 되지 않을 때까지 반복
    while (!Qempty(&q)) {
        //temp = Q.dequeue();
        // 한 정점을 dequeue한다. 
        int temp = Dequeue(&q);
     
        // dequeue한 정점이 도착점이면 탐색을 중지한다. 
        if (temp == WIDTH * HEIGHT - 1) {
            break;
        }

        p = g->edges[temp];
        // 해당 정점이 인접한 노드를 갖고 있다면
        while (p->next != NULL) {
            int y = p->y;
            // 해당 정점의 인접한 노드가 방문되지 않았다면
            if (visited[y] == 0) {
                //mark y as visited
                //인접한 정점을 방문했다고 표시한다. 
                visited[y] = 1;
                // 인접한 정점을 큐에 enqueue한다.
               // Q.enqueue(y);
                Enqueue(&q, y);

                // 해당 경로를 다음과 같이 저장한다. 
                parent[y] = temp;
                BStoreAllpath[allqidx++] = { temp,y };
            }
            p = p->next;
        }
    }

    // 정점의 이전 경로 정점의 정보를 갖고있는 parent 배열을 통해 ,최단 거리 경로를 저장한다.
    int v = WIDTH * HEIGHT - 1;
    while (parent[v] != v) {
        BStoreMinpath[minqidx++] = { v,parent[v] };
        v = parent[v];
    }


}

//BFS 동작을 실행한 후, 탐색 결과를 그리는 자료구조
void ofApp::drawBFS(graph* g)
{
    //draw all paths
    for (int i = 0; i < allqidx; i++) {
        ofSetColor(ofColor::gray);
        ofDrawLine(size + (BStoreAllpath[i].prenode % WIDTH) * 2 * size, size + (BStoreAllpath[i].prenode / WIDTH) * 2 * size, size + (BStoreAllpath[i].nextnode % WIDTH) * size * 2, size + (BStoreAllpath[i].nextnode / WIDTH) * 2 * size);

    }
    // draw shortest path
    for (int i = 0; i < minqidx; i++) {
        ofSetColor(ofColor::black);
        ofDrawLine(size + (BStoreMinpath[i].prenode % WIDTH) * 2 * size, size + (BStoreMinpath[i].prenode / WIDTH) * 2 * size, size + (BStoreMinpath[i].nextnode % WIDTH) * size * 2, size + (BStoreMinpath[i].nextnode / WIDTH) * 2 * size);
    }

}

void ofApp::Queueinit(Queue* q) {
    q->front = NULL;
    q->rear = NULL;

}
bool ofApp::Qempty(Queue* q) {
    if (q->front == NULL) return true;
    else return false;
}
void ofApp::Enqueue(Queue* q, int data) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->next = NULL;
    newnode->data = data;
    if (Qempty(q)) {
        q->front = newnode;
        q->rear = newnode;
    }
    else {
        q->rear->next = newnode;
        q->rear = newnode;
    }

}
int ofApp::Dequeue(Queue* q) {
    int Deldata = q->front->data;
    Node* Delnode = q->front;
    q->front = q->front->next;
    free(Delnode);
    return Deldata;
}
