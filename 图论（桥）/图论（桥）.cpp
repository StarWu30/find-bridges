// 图论（桥）.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "function.h";

int main()
{
	int vexnum;
	int edgenum;
	LARGE_INTEGER freq;
	LARGE_INTEGER begintime;
	LARGE_INTEGER endtime;

	QueryPerformanceFrequency(&freq);
	Graph graph;
	graph.init_edge_file("edge15.txt");
	graph.init_graph();
	/*cin >> vexnum >> edgenum;
	Graph graph(vexnum,edgenum);
	graph.init_edge();
	graph.init_graph();*/
	//graph.print_edge();
	
	QueryPerformanceCounter(&begintime);
	graph.DFS_alledge();
	
	QueryPerformanceCounter(&endtime);
	cout << "DFS所耗时间：" 
		<<  (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart 
		<< "ms" << endl;
	cout << "桥的数量为：" << graph.get_bridge_num() << endl;
	graph.print_bridge();

	// 清空桥
	graph.bridges.clear();

	/*QueryPerformanceCounter(&begintime);
	graph.unionfind_1();
	graph.tarjan_all();
	QueryPerformanceCounter(&endtime);
	cout << "并查集1所耗时间："
		<< (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart
		<< "ms" << endl;
	cout << "桥的数量为：" << graph.get_bridge_num() << endl;
	graph.print_bridge();*/


	/*QueryPerformanceCounter(&begintime);
	graph.kruskal_all();
	QueryPerformanceCounter(&endtime);
	cout << "并查集2所耗时间："
		<< (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart
		<< "ms" << endl;
	cout << "桥的数量为：" << graph.get_bridge_num() << endl;
	graph.print_bridge();*/
	return 0;
}


