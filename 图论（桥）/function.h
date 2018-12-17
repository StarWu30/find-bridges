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
	stack<int> path;
	vector<int> road;
	Edge *edge;
	vector<int> point;
	vector<int> *nodes;

	vector<Edge> bridges;
	vector<Edge> sides;
	vector<Edge> others;

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
		delete[]nodes;
		delete[]edge;
		delete[]visited;
		delete[]father;
		delete[]plies;
		delete[]dfn;
		delete[]low;
	}
	void new_variate() {
		nodes = new vector<int>[vexnum];
		edge = new Edge[edgenum];
		visited = new bool[vexnum];
		father = new int[vexnum];
		plies = new int[vexnum];
		dfn = new int[vexnum];
		low = new int[vexnum];

	}
	// 输入边所连接的顶点
	void init_edge() {
		for (int i = 0; i < edgenum; i++) {
			cin >> edge[i].head >> edge[i].tail;
		}
	}

	void init_edge_file(string filename) {
		ifstream infile;
		infile.open(filename, ios::in);
		if (!infile.is_open()) {
			cout << "Open file failure" << endl;
			exit(-1);
		}
		infile >> vexnum >> edgenum;
		new_variate();
		for (int i = 0; i < edgenum; i++) {
			infile >> edge[i].head >> edge[i].tail;
		}
		infile.close();
		
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
		int j = v;
		while (v != father[v]) {
			v = father[v];
			father[v] = father[father[v]];
		}
		
		while (j != v) {
			int t = father[j];
			father[j] = v;
			j = t;
		}
		return v;
	}

	// 并-秩优化
	void union_1(int v, int w) {
		int parents_v = get_father(v);
		int parents_w = get_father(w);
		if (parents_v == parents_w) {
			return;
		}
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
		dfn[i] = low[i] = ++index;    // index的初始值为0
		for (int j = 0; j < nodes[i].size(); j++) {
			int k = nodes[i][j];
			if (!dfn[k]) {			// 该点没有访问过
				tarjan(k, i);	    // 递归调用tarjan   
				low[i] = min(low[i], low[k]);
				if (dfn[i] < low[k]) {   // 符合条件就为桥
					bridges.push_back(Edge(i, k));
				}
			} else if (Father != k) {
				/* 假如k是i的父亲的话，
				 * 那么这就是无向边中的重边，
				   有重边那么一定不是桥*/
				low[i] = min(low[i], dfn[k]);  // 回溯过程中更新low[i]的值
			}
		}
	}

	// 对所有连通块使用tarjan算法
	void tarjan_all() {
		set<int> piece;   // 一个用于保存并查集根结点的集合
		for (int i = 0; i < vexnum; i++) {	// 根结点的数量为连通分量的数量
			piece.insert(get_father(i));
		}
		init_tarjan();
		// 对每个连通分量使用tarjan算法
		for (set<int>::iterator it = piece.begin(); it != piece.end(); it++) { 
			tarjan(*it, *it);
		}
	}

	void nodes_clear() {
		for (int i = 0; i < vexnum; i++) {
			nodes[i].clear();
		}
	}
	
	
	void kruskal() {
		init_union();     // 初始化father数组
		for (int i = 0; i < edgenum; i++) {
			// 排除自环
			if (edge[i].head == edge[i].tail) {
				continue;
			}
			// father不一样，合并
			if (get_father(edge[i].head) != get_father(edge[i].tail)) {
				union_1(edge[i].head, edge[i].tail);
				// 得到最小生成树的边
				sides.push_back(edge[i]);
				continue;
			}
			// 记录非生成树的边
			others.push_back(edge[i]);
		}
		// 构建最小生成树
		/*for (int i = 0; i < sides.size(); i++) {
			//cout << sides[i].head << "-" << sides[i].tail << endl;
			nodes[sides[i].head].push_back(sides[i].tail);
			nodes[sides[i].tail].push_back(sides[i].head);
		}*/
	}

	

	void kruskal_all() {
		// 通过kruskal算法找到生成树和非生成树的边
		kruskal();
		init_union();

		int *record = new int[vexnum];
		// 用非生成树的边进行并查集
		for (int i = 0; i < others.size(); i++) {
			union_1(others[i].head, others[i].tail);
		}
		
		// 记录非生成树的根结点
		for (int i = 0; i < others.size(); i++) {
			record[others[i].head] = father[others[i].head];
			record[others[i].tail] = father[others[i].tail];
		}
		// 对生成树的边进行遍历
		for (int i = 0; i < sides.size(); i++) {
			for (int j = 0; j < sides.size(); j++) {
				// 删除掉一条边进行合并
				if (i == j) {
					continue;
				}
				union_1(sides[j].head, sides[j].tail);
			}

			// 如果边的两点不在同一个集合，为桥
			if (get_father(sides[i].head) != get_father(sides[i].tail)) {
				bridges.push_back(sides[i]);
			}
			// 初始化生成树的根结点
			for (int j = 0; j < sides.size(); j++) {
				if (i == j) {
					continue;
				}
				father[sides[j].head] = sides[j].head;
				father[sides[j].tail] = sides[j].tail;
			}
			// 还原非生成树的根结点
			for (int j = 0; j < others.size(); j++) {
				father[others[j].head] = record[others[j].head];
				father[others[j].tail] = record[others[j].tail];
			}
		}
		
	}
	

	/*void DFS_once(int v, int w) {
		visited[v] = true;
		path.push(v);
		road.push_back(v);
		for (int i = 0; i < nodes[v].size(); i++) {
			
			
			
			if (!visited[nodes[v][i]]) {
				DFS_once(nodes[v][i], w);
			} 
			// 如果找到顶点，就结束DFS
			if (!path.empty() && path.top() == w) {
				break;
			} else if (!path.empty()) {
				while (!path.empty() && path.top() != v) {
					path.pop();
				}
			}
				
		}
	}*/
	
	// 找环是通过堆栈来找的
	// 找环的速度太慢了，日后有时间优化
	/*void kruskal_all() {
		kruskal();
		init_visited();
		vector<int> street;
		bool *evisited;
		evisited = new bool[sides.size()];
		for (int i = 0; i < sides.size(); i++) {
			evisited[i] = false;
		}
		// 添加非生成树上的边
		for (vector<Edge>::iterator it = others.begin(); it != others.end(); it++) {

			// 插入边
			//nodes[it->head].push_back(it->tail);
			//nodes[it->tail].push_back(it->head);

			DFS_once(it->head, it->tail);
			//cout << "添加边：" << it->head << "-" << it->tail << endl;
			if (path.top() == it->tail) {
				while (!path.empty()) {
					street.push_back(path.top());
					path.pop();
				}
				
				//cout << "路径为：";
				for (int i = 0; i < street.size() - 1; i++) {
					//cout << street[i] << "-" << street[i + 1] << " ";
					for (int j = 0; j < sides.size(); j++) {
						if ((sides[j].head == street[i] && sides[j].tail == street[i + 1]) ||
							(sides[j].tail == street[i] && sides[j].head == street[i + 1])) {
							evisited[j] = true;
							break;
						}
					}
				}
				//cout << endl;
				street.clear();
			} else {
				while (!path.empty()) {
					path.pop();
				}
			}
			
			for (int i = 0; i < road.size(); i++) {
				visited[road[i]] = false;
			}
			road.clear();
			// 删除边
			//nodes[it->head].erase(find(nodes[it->head].begin(),
			//	nodes[it->head].end(), it->tail));
			//nodes[it->tail].erase(find(nodes[it->tail].begin(),
			//	nodes[it->tail].end(), it->head));
		}
		for (int i = 0; i < sides.size(); i++) {
			if (!evisited[i]) {
				bridges.push_back(sides[i]);
			}
		}
	}*/

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