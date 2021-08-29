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
///�̷��� ����� �Ʒ��� ���� size ���ڷ� �ٲ� �� �ֽ��ϴ�!!!!
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
            printf("%d �� %d�� top %d down %d left %d right %d\n", i, j, maze[i][j].top, maze[i][j].down, maze[i][j].left, maze[i][j].right);
        }
    }
}

void  ofApp::makegraph(graph* g, int num) {

    g->nvertices = num;
    g->nedges = 0;
    g->edges = (edgenode**)malloc(sizeof(edgenode*) *(num));
  //free �� ��������� �ҵ�
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
//���� ������ ����. �׷��� ���������� �۵��Ǵ��� ����
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


//DFS ������ ������ ��,Ž�� ����� �׸��� �ڷᱸ��
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

// DFS ���� �����ϴ� �ڷᱸ��
void ofApp::dfs(graph *g) {
      
      // init a stack S 
      // ������ �ʱ�ȭ �Ѵ�. 
        Stack s;
        StackInit(&s);;
     
        int flag;
        //S.push ( 0 );
        // �������� 0�� ������ ���ÿ� �ִ´�. 
        Spush(&s, 0);
      
        edgenode* p;

        allidx = 0;
        minidx = 0;
        
        // ��� visited �迭�� 0���� �ʱ�ȭ �Ѵ�. 
       for (int i = 0; i < WIDTH*HEIGHT; i++) { visited[i] = 0; }

       //mark v as visited;
       // �������� 0�� ������ �湮�ߴٰ� ǥ���Ѵ�. 
       visited[0] = 1;

        //while ( S != empty )
        // ������ EMPTY ���� ���� ������ �ݺ�
        while (!Sempty(&s)) {
            flag = 0;
            int temp = Speek(&s);
            //if ( S.top == target ) return;
            // ������ top�� �������̸� Ž���� �����Ѵ�. 
            if (temp == WIDTH * HEIGHT - 1) {
                flag = 1; break;
            }

            p = g->edges[temp];
            //if (S.top has an unvisited adjacent node)
            // ������ ž�� ������ ��带 ���� �ִٸ�
            while (p->next != NULL) {
                int y = p->y;
                //y= an unvisited, adjacent node to S.top;
                // �� ��尡 ���� �湮���� �ʾҴٸ�
                if (visited[y] == 0) {

                    //mark y as visited;
                    // �ش� ��带 �湮 �ߴٰ� ǥ���Ѵ�. 
                    visited[y] = 1;
                  
                    //S.push(u);
                    Spush(&s, y); flag = 1;
                    // �ش� ��θ� ������ ���� �����Ѵ�. 
                    StoreMinpath[minidx++] = { temp,y };
                    StoreAllpath[allidx++] = { temp,y };
                    break;
                }
                p = p->next;
            }
            //else {  S.pop();}
            // ������ ��尡 ���ٸ� ���ÿ��� pop �Ѵ�.( flag�� ������ ��尡 ������ 1��,  ������ 0�� ��Ÿ����.)
            if (!flag) {
                Spop(&s);
                //�ִ� ��θ� �����ϱ� ���� �迭������ pop�� ���ֱ� ���� �ε����� 1�� ���ҽ�Ų��. 
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

// BFS ���� �����ϴ� �ڷᱸ��
void ofApp::bfs(graph* g) {
    //Init a queue Q
    // ť�� �ʱ�ȭ �Ѵ�. 
    Queue q;
    edgenode* p;
    Queueinit(&q);
    //Q.enqueue(0);
    // �������� 0�� ������ ť�� �ִ´�. 
    Enqueue(&q, 0);

    //�� ������ parent�� ������ �迭�� �ʱ�ȭ �Ѵ�. 
    memset(parent, -1, sizeof(int) * (WIDTH * HEIGHT));
    parent[0] = 0;

    minqidx = 0;
    allqidx = 0;

    // ��� visited �迭�� 0���� �ʱ�ȭ �Ѵ�. 
    for (int i = 0; i < HEIGHT * WIDTH; i++) { visited[i] = 0; }
    //mark 0 as visited;
    //�������� 0�� ������ �湮�ߴٰ� ǥ���Ѵ�. 
    visited[0] = 1;
    //while ( Q != EMPTY) 
    //// ť�� EMPTY ���� ���� ������ �ݺ�
    while (!Qempty(&q)) {
        //temp = Q.dequeue();
        // �� ������ dequeue�Ѵ�. 
        int temp = Dequeue(&q);
     
        // dequeue�� ������ �������̸� Ž���� �����Ѵ�. 
        if (temp == WIDTH * HEIGHT - 1) {
            break;
        }

        p = g->edges[temp];
        // �ش� ������ ������ ��带 ���� �ִٸ�
        while (p->next != NULL) {
            int y = p->y;
            // �ش� ������ ������ ��尡 �湮���� �ʾҴٸ�
            if (visited[y] == 0) {
                //mark y as visited
                //������ ������ �湮�ߴٰ� ǥ���Ѵ�. 
                visited[y] = 1;
                // ������ ������ ť�� enqueue�Ѵ�.
               // Q.enqueue(y);
                Enqueue(&q, y);

                // �ش� ��θ� ������ ���� �����Ѵ�. 
                parent[y] = temp;
                BStoreAllpath[allqidx++] = { temp,y };
            }
            p = p->next;
        }
    }

    // ������ ���� ��� ������ ������ �����ִ� parent �迭�� ���� ,�ִ� �Ÿ� ��θ� �����Ѵ�.
    int v = WIDTH * HEIGHT - 1;
    while (parent[v] != v) {
        BStoreMinpath[minqidx++] = { v,parent[v] };
        v = parent[v];
    }


}

//BFS ������ ������ ��, Ž�� ����� �׸��� �ڷᱸ��
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
