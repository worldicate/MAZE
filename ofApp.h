#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
    // flag variables
    int draw_flag;
    int load_flag;
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
 

	int HEIGHT;//�̷��� ����
	int WIDTH;//�̷��� �ʺ�
	// �޸� ���� �Լ�
	void freeMemory();
	bool readFile(const char* filename);

	////maze �׸��� ���� �ڷᱸ��///
	typedef struct _vertex {
		int top;
		int down;
		int right;
		int left;
	}Vertex;
	Vertex** maze;
	void printmaze();
	void ofApp::makedrawmaze(const char* filename);
	////////////////////////////////////////////////////


	////graph ���� �Լ�////////////////////
	typedef struct _edgenode {
		int y; /* adjancency info */
		struct _edgenode* next; /* next edge in list */
	} edgenode;

	typedef struct _graph {
		edgenode** edges;
		int nvertices;
		int nedges;
	} graph;
	graph g;

	void makegraph(graph* g, int num);
	void graphdestroy(graph* g);
	void addedge(graph* g, int fromV, int toV);
	void printedge(graph* g, int fromV);
	void makemazegraph();
	/////////////////////////////////////

	//////////////dfs,bfs �ϱ����� �ڷᱸ��//////////////////
	int *visited;
	int* parent;
	bool isdfs;
	bool isbfs;
	void drawBFS(graph*g);
	void drawDFS(graph*g);
	void dfs(graph* g);
	void bfs(graph* g);
	/////////////////////////////////////////////

	/////////////////////////////////
	//�Ÿ� ����� �� ���̴� �ڷᱸ��///
	typedef struct _cor {
		int prenode;
		int nextnode;
	}Cor;

	Cor *StoreMinpath;
	Cor* StoreAllpath;
	Cor *BStoreMinpath;
	Cor *BStoreAllpath;
	int minidx;
	int allidx;
	int allqidx;
	int minqidx;
	////////////////////

	//stack ���� �Լ� //
	typedef struct _node {
		int data;
		struct _node* next;
	}Node;

	typedef struct _stack {
		Node* top;
	}Stack;

	void StackInit(Stack* s);
	bool Sempty(Stack* s);
	void Spush(Stack* s, int data);
	int Spop(Stack* s);
	int Speek(Stack* s);

	///////////////
	
	typedef struct _queue {
		Node* front;
		Node* rear;
	}Queue;


	// queue ���� �Լ� //
	void Queueinit(Queue* q);
	bool Qempty(Queue* q);
	void Enqueue(Queue* p, int data);
	int Dequeue(Queue* q);
	
	////////
};
