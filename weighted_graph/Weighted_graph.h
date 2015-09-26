/*
* Weighted_graph by Eric Lomore
* April 5th, 2015
*
* This class implements Kruskal's algorithm for a weighted connected graph.
* The class utilizes the Disjoint sets and Binary search tree data structures as
* implemented by Douglas Wilhelm Harder. Minor modifications have been made to these
* data structures to accomodate this implementation.
*
* References: The additional files included in this submission were written by Douglas Wilhelm Harder
*/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <map>
#include "Exception.h"
#include "Disjoint_sets.h"
#include "Binary_search_tree.h"
#include "Binary_search_node.h"
#include <stack>

using namespace Data_structures;

/*
*--------------------------------------------------------
* Class Edge
*
* Edge(int, int, double)
*   This default constructor initializes the member variables for an edge
* 
* v1, v2, weight
*    These member variables hold the two vertices, and the weight of the edge
* 
* Note that we override the operators for this class as the relation
* between Edge objects is defined by their wieght
*--------------------------------------------------------
*/
class Edge {
public:
	int v1;
	int v2;
	double weight;
	Edge(int, int, double);

	bool operator<(const Edge &e2) const {
		return weight < e2.weight;
	}
	bool operator>(const Edge &e2) const {
		return weight > e2.weight;
	}
	bool operator<=(const Edge &e2) const {
		return weight <= e2.weight;
	}
	bool operator>=(const Edge &e2) const {
		return weight >= e2.weight;
	}
	bool operator==(const Edge &e2) const {
		return weight == e2.weight && v1 == e2.v1 && v2 == e2.v2;
	}
	bool operator!=(const Edge &e2) const {
		return weight != e2.weight;
	}
};


Edge::Edge(int i, int j, double d) :
v1(i),
v2(j),
weight(d) {}

class Weighted_graph {
private:
	static const double INF;

	// Do not implement these functions!
	// By making these private and not implementing them, any attempt
	// to make copies or assignments will result in errors
	Weighted_graph(Weighted_graph const &);
	Weighted_graph &operator=(Weighted_graph);

	/*---------------------------------------------------------

		*  Member Variables:
		*
		*  int N
		*    The number of vertices in the weighted graph. 
		*    Initialized by the constructor.
		*
		*  int edge_counter
		*    Holds the total number of edges in the weighted graph
		*
		*  Edge* **edges
		*    Pointer to an array of pointers to arrays, which hold edges as pointers.
		*    Essentially, a 2-dimensional arrays, intiaizlied as pointers of type Edge*
		*    Used to access Edges in O(1) time by vertex indices
		*
		*  Binary_search_tree<Edge*> tree;
		*    A binary search tree of pointers to edges. Holds all of the edges within the graph.
		*    Automatically sorting them due to the nature of a binary search tree.
		*

	*--------------------------------------------------------*/

	// your choice
	int N;
	int edge_counter;
	Edge* **edges;
	Binary_search_tree<Edge*> tree;

public:
	Weighted_graph(int = 10);
	~Weighted_graph();

	int degree(int) const;
	int edge_count() const;

	bool insert_edge(int, int, double);
	bool erase_edge(int, int);
	void clear_edges();
	void inorder(Binary_search_node<Edge*> *);
	std::pair<double, int> minimum_spanning_tree();

	// Friends

	friend std::ostream &operator<<(std::ostream &, Weighted_graph const &);
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();


Weighted_graph::Weighted_graph(int n) {

	edges = new Edge* *[n];
	for (int i = 0; i < n; ++i){
		edges[i] = new Edge*[n];
		for (int j = 0; j < n; ++j){
			edges[i][j] = nullptr;
		}
	}
	N = n;
}

Weighted_graph::~Weighted_graph() {
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			delete edges[i][j];
		}
		delete[] edges[i];
	}
	delete[] edges;
	tree.clear();
}

	/* ---------------------------------------------------------

	*               Member Functions(Accessors) :
	*
	*  int degree() const
	*    Returns the degree of a vertex at the given index.
	*
	*  int edge_count() const
	*    Returns the total number of edges in the weighted graph
	*
	*
	* --------------------------------------------------------- */


int Weighted_graph::degree(int i) const{
	// If argument is outside the range from 0 to n-1 throw illegal argument exception
	if (i < 0 || i > N - 1){
		throw illegal_argument();
	}
	int degree = 0;
	for (int j = 0; j < N; ++j){
		if (edges[i][j] != nullptr || edges[j][i] != nullptr){
			degree++;
		}
	}
	return degree;

}

int Weighted_graph::edge_count() const{
	return edge_counter;
}

		/*---------------------------------------------------------
		*					Member Functions(Mutators) :
		*
		*  bool insert_edge()
		*     Inserts an edge that connects two vertices, with a weight
		*     given as a double.Returns true if the edge is inserted,
		*     false otherwise.
		*
		*  bool erase_edge()
		*     Erases an edge between the given vertices
		*
		*  void clear_edges();
		*     Removes all edges in the graph, and resets the counters.
		*
		*  void inorder(Binary_search_node<Edge*> *);
		*     Supplementary function, does a recursive in order traversal of the graph
		*     printing each node in sorted order by weight
		*
		*  std::pair<double, int> minimum_spanning_tree();
		*     Finds the minimum spanning tree for the connected graph.
		*     Returns the total weight of the minimum spanning tree, and
		*     the total numbers of edges checked to create that spanning tree.
		*     If a forest of spanning trees is found, it is treated as one single solution.
		*
		*---------------------------------------------------------*/

bool Weighted_graph::insert_edge(int i, int j, double d) {
	// If the arguments are outside of their applicable ranges,
	// throw illegal argument exceptipon
	if (i < 0 || i > N - 1 || j < 0 || j > N - 1 || d < 0){
		throw illegal_argument();
	}
	// No vertex should connect to itself
	if (i == j){
		return false;
	}
	// We want a standard approach where i > j always
	// Since a vertex connecting from i to j is identical
	// to one connecting from j to i.
	if (i > j){
		int temp = i;
		i = j;
		j = temp;
	}
	// If the edge already exists, it needs to be updated
	if (edges[i][j] != nullptr){
		// No update necessary, return
		if (edges[i][j]->weight == d){
			return true;
		}
		// Erase the edge, then continuing by adding it again
		erase_edge(i, j);
	}

	Edge* e = new Edge(i, j, d);
	edges[i][j] = e;
	tree.insert(e);

	edge_counter++;
	return true;

}

bool Weighted_graph::erase_edge(int i, int j){
	// If the arguments are outside of their applicable ranges,
	// throw illegal argument exceptipon
	if (i < 0 || i > N - 1 || j < 0 || j > N - 1){
		throw illegal_argument();
	}
	// We want a standard approach where i > j always
	// Since a vertex connecting from i to j is identical
	// to one connecting from j to i.
	if (i > j){
		int temp = i;
		i = j;
		j = temp;
	}
	// If the edge doesn't exist, return false
	if (edges[i][j] == nullptr){
		return false;
	}

	Edge* e = edges[i][j];
	tree.erase(e);

	// Deallocate from memory
	delete e;
	edges[i][j] = nullptr;

	edge_counter--;
	return true;

}

// Clear the edges in the graph
void Weighted_graph::clear_edges(){
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			delete edges[i][j];
			edges[i][j] = nullptr;
		}
	}
	tree.clear();
	edge_counter = 0;
	return;
}

void Weighted_graph::inorder(Binary_search_node<Edge*> *e) {
	// Complete a depth first post order order traverse recursively
	if (e->left() != nullptr) inorder(e->left());
	std::cout << e->retrieve()->v1 << " " << e->retrieve()->v2 << " " << e->retrieve()->weight << std::endl;
	if (e->right() != nullptr) inorder(e->right());

}

// Recall: we stop at |V|-1 edges, or when none are left; whichever comes first.
// If |V|-1, we have a minimum spanning tree. if N, we have a forest of minimum spanning trees
std::pair<double, int> Weighted_graph::minimum_spanning_tree() {
	Disjoint_sets *set = new Disjoint_sets(N);
	int edges_tested = 0;
	Binary_search_node<Edge*> *e = tree.root();
	// inorder(e);

	// Complete an in order traversal of the tree, iteratively
	std::stack<Binary_search_node<Edge*>*> s;
	Binary_search_node<Edge*> *node = tree.root();
	// While the stack isnt empty, or if the node is pointing to null, continue
	while (!s.empty() || node != nullptr) {
		// If the node isn't null, push it to the stack, and dive into the left subtree
		if (node != nullptr) {
			s.push(node);
			node = node->left();
		}
		// If the node is null, then we have reached the leftmost subtree.
		// Pop the most recent node of the stack. This node is "in-order".
		else
		{
			node = s.top();
			s.pop();

			// Update the disjoint sets with the new vertex
			set->set_union(node->retrieve()->v1, node->retrieve()->v2, node->retrieve()->weight);
			// An additional edge has been tested
			edges_tested++;

			// If there is only one disjoint set, we have a minimum spanning tree; we are done.
			if (set->disjoint_sets() == 1){
				double return_weight = set->get_weight();
				delete set;
				return(std::make_pair(return_weight, edges_tested));
			}
			// Dive into the right subtree as we have cleared the left subtree
			node = node->right();
		}
	}
	
	double return_weight = set->get_weight();
	delete set;
	return(std::make_pair(return_weight, edges_tested));
}

std::ostream &operator<<(std::ostream &out, Weighted_graph const &graph) {
	// TODO: Implement a visual output for this class using std output

	return out;
}

#endif
