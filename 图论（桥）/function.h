#pragma once
#include "pch.h"

// ��
class Edge {
public:
	int head;
	int tail;
	Edge() {
		head = tail = -1;
	}
	Edge(int h, int t) {
		head = h;
		tail = t;
	}
};

// ͼ
class Graph {
public:
	int vexnum;					// ������
	int edgenum;				// ����
	int index;
	bool *visited;	
	Edge *edge;						
	vector<int> *nodes;
	vector<Edge> bridges;

	int *father;
	int *plies;
	int *dfn;
	int *low;

	// �޲ι��캯��
	Graph() {
		vexnum = 0;
		edgenum = 0;
	}

	// �вι��캯��
	Graph(int v, int e) {
		vexnum = v;
		edgenum = e;
		nodes = new vector<int>[vexnum];
		edge = new Edge[edgenum];
		visited = new bool[vexnum];
		father = new int[vexnum];
		plies = new int[vexnum];
		dfn = new int[vexnum];
		low = new int[vexnum];
	}
	~Graph() {
		delete []nodes;
		delete []edge;
		delete []visited;
		delete []father;
		delete []plies;
		delete []dfn;
		delete []low;
	}

	// ����������ӵĶ���
	void init_edge() {
		for (int i = 0; i < edgenum; i++) {
			cin >> edge[i].head >> edge[i].tail;
		}
	}

	//  ��ʼ��visited����
	void init_visited() {
		if (vexnum > 0) {
			for (int i = 0; i < vexnum; i++) {
				visited[i] = false;
			}
		}
	}

	// ��ʼ��ͼ
	void init_graph() {
		for (int i = 0; i < edgenum; i++) {
			insert_edge(i);
		}

	}

	// �����
	void insert_edge(int i) {
		nodes[edge[i].head].push_back(edge[i].tail);
		if (edge[i].head != edge[i].tail) 
			nodes[edge[i].tail].push_back(edge[i].head);
	}

	// ɾ����
	void delete_edge(int i) {
		nodes[edge[i].head].erase(find(nodes[edge[i].head].begin(), nodes[edge[i].head].end(), edge[i].tail));
		if(edge[i].head != edge[i].tail)
			nodes[edge[i].tail].erase(find(nodes[edge[i].tail].begin(), nodes[edge[i].tail].end(), edge[i].head));
	}

	// �����������
	void DFS(int v) {
		visited[v] = true;
		for (int i = 0; i < nodes[v].size(); i++) {
			if (!visited[nodes[v][i]]) {
				DFS(nodes[v][i]);
			}
		}
	}

	// �����б�ʹ��DFS
	void DFS_alledge() {
		for (int i = 0; i < edgenum; i++) {
			init_visited();
			// �ų��Ի�
			if (edge[i].head == edge[i].tail)
				continue;
			delete_edge(i);
			DFS(edge[i].head);
			// ���β���û�б����ʹ��������߾�����
			if (!visited[edge[i].tail])
				bridges.push_back(edge[i]);
			insert_edge(i);
		}
	}

	// ��ʼ�����׺Ͳ�����ֵ
	void init_union() {
		for (int i = 0; i < vexnum; i++) {
			father[i] = i;
			plies[i] = 0;
		}
	}

	// ���׽ڵ�--·��ѹ��
	int get_father(int v) {
		while (v != father[v]) {
			v = father[v];
			father[v] = father[father[v]];
		}
		return v;
	}

	// ��-���Ż�
	void union_1(int v, int w) {
		int parents_v = get_father(v);
		int parents_w = get_father(w);
		if (plies[parents_v] > plies[parents_w]) {
			father[parents_w] = parents_v;
		} else if (plies[parents_v] < plies[parents_w]) {
			father[parents_v] = parents_w;
		} else {
			father[parents_v] = parents_w;
			plies[parents_w]++;
		}
	}

	// ���鼯
	void unionfind_1() {
		init_union();
		for (int i = 0; i < edgenum; i++) {
			// �ų��Ի�
			if (edge[i].head == edge[i].tail)
				continue;
				union_1(edge[i].head, edge[i].tail);
		}
	}

	// ��ʼ��tarjan�㷨�ı���
	void init_tarjan() {
		index = 0;
		for (int i = 0; i < vexnum; i++) {
			dfn[i] = low[i] = father[i] = 0;
		}
	}

	// tarjan�㷨
	void tarjan(int i, int Father) {
		// ��¼ÿһ����ĸ���
		father[i] = Father;
		dfn[i] = low[i] = ++index;
		for (int j = 0; j < nodes[i].size(); j++) {
			int k = nodes[i][j];
			if (!dfn[k]) {
				tarjan(k, i);
				low[i] = min(low[i], low[k]);
				if (dfn[i] < low[k]) {
					bridges.push_back(Edge(i, k));
				}
			} else if (Father != k) {
				/* ����k��i�ĸ��׵Ļ���
				 * ��ô�����������е��رߣ�
				   ���ر���ôһ��������*/
				low[i] = min(low[i], dfn[k]);
			}
		}
	}

	// ��������ͨ��ʹ��tarjan�㷨
	void tarjan_all() {
		set<int> piece;
		for (int i = 0; i < vexnum; i++) {
			piece.insert(get_father(i));
		}
		init_tarjan();
		for (set<int>::iterator it = piece.begin(); it != piece.end(); it++) {
			tarjan(*it, *it);
		}
	}

	// ����ߵ���Ϣ
	void print_edge() {
		for (int i = 0; i < vexnum; i++) {
			cout << i << ":";
			for (vector<int>::iterator it = nodes[i].begin(); it != nodes[i].end(); it++) {
				cout << *it << " ";
			}
			cout << endl;
		}
	}

	// ����ŵ���Ϣ
	void print_bridge() {
		if (!bridges.size()) {
			return;
		}
		cout << "�ŷֱ�Ϊ��" << endl;
		for (vector<Edge>::iterator it = bridges.begin(); it != bridges.end(); it++) {
			cout << it->head << " --- " << it->tail << endl;
		}
	}

	// �����ŵ�����
	int get_bridge_num() {
		return bridges.size();
	}
};