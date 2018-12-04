#pragma once
#include "pch.h"

// 边
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

// 图
class Graph {
public:
	int vexnum;					// 顶点数
	int edgenum;				// 边数
	int index;
	bool *visited;	
	Edge *edge;						
	vector<int> *nodes;
	vector<Edge> bridges;

	int *father;
	int *plies;
	int *dfn;
	int *low;

	// 无参构造函数
	Graph() {
		vexnum = 0;
		edgenum = 0;
	}

	// 有参构造函数
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

	// 输入边所连接的顶点
	void init_edge() {
		for (int i = 0; i < edgenum; i++) {
			cin >> edge[i].head >> edge[i].tail;
		}
	}

	//  初始化visited数组
	void init_visited() {
		if (vexnum > 0) {
			for (int i = 0; i < vexnum; i++) {
				visited[i] = false;
			}
		}
	}

	// 初始化图
	void init_graph() {
		for (int i = 0; i < edgenum; i++) {
			insert_edge(i);
		}

	}

	// 插入边
	void insert_edge(int i) {
		nodes[edge[i].head].push_back(edge[i].tail);
		if (edge[i].head != edge[i].tail) 
			nodes[edge[i].tail].push_back(edge[i].head);
	}

	// 删除边
	void delete_edge(int i) {
		nodes[edge[i].head].erase(find(nodes[edge[i].head].begin(), nodes[edge[i].head].end(), edge[i].tail));
		if(edge[i].head != edge[i].tail)
			nodes[edge[i].tail].erase(find(nodes[edge[i].tail].begin(), nodes[edge[i].tail].end(), edge[i].head));
	}

	// 深度优先搜索
	void DFS(int v) {
		visited[v] = true;
		for (int i = 0; i < nodes[v].size(); i++) {
			if (!visited[nodes[v][i]]) {
				DFS(nodes[v][i]);
			}
		}
	}

	// 对所有边使用DFS
	void DFS_alledge() {
		for (int i = 0; i < edgenum; i++) {
			init_visited();
			// 排除自环
			if (edge[i].head == edge[i].tail)
				continue;
			delete_edge(i);
			DFS(edge[i].head);
			// 如果尾结点没有被访问过，这条边就是桥
			if (!visited[edge[i].tail])
				bridges.push_back(edge[i]);
			insert_edge(i);
		}
	}

	// 初始化父亲和层数的值
	void init_union() {
		for (int i = 0; i < vexnum; i++) {
			father[i] = i;
			plies[i] = 0;
		}
	}

	// 求父亲节点--路径压缩
	int get_father(int v) {
		while (v != father[v]) {
			v = father[v];
			father[v] = father[father[v]];
		}
		return v;
	}

	// 并-秩优化
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

	// 并查集
	void unionfind_1() {
		init_union();
		for (int i = 0; i < edgenum; i++) {
			// 排除自环
			if (edge[i].head == edge[i].tail)
				continue;
				union_1(edge[i].head, edge[i].tail);
		}
	}

	// 初始化tarjan算法的变量
	void init_tarjan() {
		index = 0;
		for (int i = 0; i < vexnum; i++) {
			dfn[i] = low[i] = father[i] = 0;
		}
	}

	// tarjan算法
	void tarjan(int i, int Father) {
		// 记录每一个点的父亲
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
				/* 假如k是i的父亲的话，
				 * 那么这就是无向边中的重边，
				   有重边那么一定不是桥*/
				low[i] = min(low[i], dfn[k]);
			}
		}
	}

	// 对所有连通块使用tarjan算法
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

	// 输出边的信息
	void print_edge() {
		for (int i = 0; i < vexnum; i++) {
			cout << i << ":";
			for (vector<int>::iterator it = nodes[i].begin(); it != nodes[i].end(); it++) {
				cout << *it << " ";
			}
			cout << endl;
		}
	}

	// 输出桥的信息
	void print_bridge() {
		if (!bridges.size()) {
			return;
		}
		cout << "桥分别为：" << endl;
		for (vector<Edge>::iterator it = bridges.begin(); it != bridges.end(); it++) {
			cout << it->head << " --- " << it->tail << endl;
		}
	}

	// 返回桥的数量
	int get_bridge_num() {
		return bridges.size();
	}
};