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
	// ����������ӵĶ���
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

	// ��-���Ż�
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
		dfn[i] = low[i] = ++index;    // index�ĳ�ʼֵΪ0
		for (int j = 0; j < nodes[i].size(); j++) {
			int k = nodes[i][j];
			if (!dfn[k]) {			// �õ�û�з��ʹ�
				tarjan(k, i);	    // �ݹ����tarjan   
				low[i] = min(low[i], low[k]);
				if (dfn[i] < low[k]) {   // ����������Ϊ��
					bridges.push_back(Edge(i, k));
				}
			} else if (Father != k) {
				/* ����k��i�ĸ��׵Ļ���
				 * ��ô�����������е��رߣ�
				   ���ر���ôһ��������*/
				low[i] = min(low[i], dfn[k]);  // ���ݹ����и���low[i]��ֵ
			}
		}
	}

	// ��������ͨ��ʹ��tarjan�㷨
	void tarjan_all() {
		set<int> piece;   // һ�����ڱ��沢�鼯�����ļ���
		for (int i = 0; i < vexnum; i++) {	// ����������Ϊ��ͨ����������
			piece.insert(get_father(i));
		}
		init_tarjan();
		// ��ÿ����ͨ����ʹ��tarjan�㷨
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
		init_union();     // ��ʼ��father����
		for (int i = 0; i < edgenum; i++) {
			// �ų��Ի�
			if (edge[i].head == edge[i].tail) {
				continue;
			}
			// father��һ�����ϲ�
			if (get_father(edge[i].head) != get_father(edge[i].tail)) {
				union_1(edge[i].head, edge[i].tail);
				// �õ���С�������ı�
				sides.push_back(edge[i]);
				continue;
			}
			// ��¼���������ı�
			others.push_back(edge[i]);
		}
		// ������С������
		/*for (int i = 0; i < sides.size(); i++) {
			//cout << sides[i].head << "-" << sides[i].tail << endl;
			nodes[sides[i].head].push_back(sides[i].tail);
			nodes[sides[i].tail].push_back(sides[i].head);
		}*/
	}

	

	void kruskal_all() {
		// ͨ��kruskal�㷨�ҵ��������ͷ��������ı�
		kruskal();
		init_union();

		int *record = new int[vexnum];
		// �÷��������ı߽��в��鼯
		for (int i = 0; i < others.size(); i++) {
			union_1(others[i].head, others[i].tail);
		}
		
		// ��¼���������ĸ����
		for (int i = 0; i < others.size(); i++) {
			record[others[i].head] = father[others[i].head];
			record[others[i].tail] = father[others[i].tail];
		}
		// ���������ı߽��б���
		for (int i = 0; i < sides.size(); i++) {
			for (int j = 0; j < sides.size(); j++) {
				// ɾ����һ���߽��кϲ�
				if (i == j) {
					continue;
				}
				union_1(sides[j].head, sides[j].tail);
			}

			// ����ߵ����㲻��ͬһ�����ϣ�Ϊ��
			if (get_father(sides[i].head) != get_father(sides[i].tail)) {
				bridges.push_back(sides[i]);
			}
			// ��ʼ���������ĸ����
			for (int j = 0; j < sides.size(); j++) {
				if (i == j) {
					continue;
				}
				father[sides[j].head] = sides[j].head;
				father[sides[j].tail] = sides[j].tail;
			}
			// ��ԭ���������ĸ����
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
			// ����ҵ����㣬�ͽ���DFS
			if (!path.empty() && path.top() == w) {
				break;
			} else if (!path.empty()) {
				while (!path.empty() && path.top() != v) {
					path.pop();
				}
			}
				
		}
	}*/
	
	// �һ���ͨ����ջ���ҵ�
	// �һ����ٶ�̫���ˣ��պ���ʱ���Ż�
	/*void kruskal_all() {
		kruskal();
		init_visited();
		vector<int> street;
		bool *evisited;
		evisited = new bool[sides.size()];
		for (int i = 0; i < sides.size(); i++) {
			evisited[i] = false;
		}
		// ��ӷ��������ϵı�
		for (vector<Edge>::iterator it = others.begin(); it != others.end(); it++) {

			// �����
			//nodes[it->head].push_back(it->tail);
			//nodes[it->tail].push_back(it->head);

			DFS_once(it->head, it->tail);
			//cout << "��ӱߣ�" << it->head << "-" << it->tail << endl;
			if (path.top() == it->tail) {
				while (!path.empty()) {
					street.push_back(path.top());
					path.pop();
				}
				
				//cout << "·��Ϊ��";
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
			// ɾ����
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