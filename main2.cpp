#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using std::cout;
using std::endl;
using std::cin;
using std::vector;
struct pathNode
{
	short x;
	short y;
	float f=0.0;
	float g=0;
	float h=0.0;
	float costMultiplier = 1.0;
	bool block=false;
	pathNode* parent;
	
};
const int row = 40;
const int col = 40;
int sx, sy, ex, ey;

vector<pathNode>nodeList;
std::deque<pathNode*>openList;
vector<pathNode*>closedList;
vector<char> pathGraph;

pathNode* getNode(int x, int y)
{
	int idx = x * col + y;
	return &nodeList[idx];
}
bool isBlock(pathNode* val)
{
	return val->block;
}
bool isOpen(pathNode* val)
{
	for (auto it = openList.begin(); it != openList.end(); ++it)
	{
		if (val==*it)
		{
			return true;
		}
	}
	return false;
}
bool isClosed(pathNode* val)
{
	for (auto it = closedList.begin(); it!=closedList.end(); ++it)
	{
		if (val==*it)
		{
			return true;
		}
	}
	return false;
}
bool sortF(pathNode* a,pathNode* b)
{
	if (a->f<b->f)
	{
		return true;
	}
	return false;
}
void buildPath()
{
	pathNode* node = getNode(ex, ey);
	pathNode* startNode = getNode(sx, sy);
	int endIdx = node->x * col + node->y;
	pathGraph[endIdx] = '0';
	while (startNode!=node)
	{
		node = node->parent;
		int idx = node->x * col + node->y;
		pathGraph[idx] = '0';
	}
}
void drawPath()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int idx = i * col + j;
			cout << pathGraph[idx];
		}
		cout << "" << endl;
	}
}
float getH(pathNode* val)
{
	//对角线公式
	int dx = std::abs(val->x-ex);
	int dy = std::abs(val->y-ey);
	int diag = std::min(dx, dy);
	int stright = dx + dy;

	float result = std::sqrt(2) * diag + stright - 2 * diag;
	return result;
}
bool findPath()
{
	pathNode* startNode = getNode(sx, sy);
	pathNode* endNode = getNode(ex, ey);

	startNode->h = getH(startNode);
	startNode->g = 0;
	startNode->f = startNode->g + startNode->h;

	pathNode*  node= startNode;

	while (node !=endNode)
	{
		int sxx = std::max(0, node->x - 1);
		int exx = std::min(row - 1, node->x + 1);

		int syy = std::max(0, node->y - 1);
		int eyy = std::min(col - 1, node->y + 1);
		
		for (int i = sxx; i <=exx; i++)
		{
			for (int j = syy; j <=eyy; j++)
			{
				pathNode* test = getNode(i, j);
				if (test==node || test->block)
				{
					continue;
				}
				int cost = 1.0;
				if (!((test->x==node->x)|| (test->y==node->y)))
				{
					cost = 1.4;
				}
				float g = node->g + (cost*test->costMultiplier);
				float h = getH(test);
				float f = g + h;
				if (isOpen(test)||isClosed(test))
				{
					if (test->f>f)
					{
						test->f = f;
						test->g = g;
						test->h = h;
						test->parent = node;
					}
				}
				else
				{
					test->f = f;
					test->g = g;
					test->h = h;
					test->parent = node;
					openList.push_back(test);

				}
			}
		}
		closedList.push_back(node);
		if (openList.size()==0)
		{
			cout << "no path found" << endl;
			return false;
		}
		sort(openList.begin(), openList.end(), sortF);
		node = openList.front();
		openList.pop_front();
	}
	buildPath();
	return true;
}


int main()
{
	for (short i = 0; i < row; i++)
	{
		for (short j = 0; j < col; j++)
		{
			pathNode node;
			node.x = i;
			node.y = j;
			int idx = i * col + j;
			char symbol = '-';
			node.block = false;
			float mult = std::sin(i * .25) + std::cos(j * .2 + i * .05);
			node.costMultiplier = std::abs(mult) + 1;
			if (node.costMultiplier>1.5)
			{
				symbol = '@';
			}
			nodeList.push_back(node);
			pathGraph.push_back(symbol);
		}
	}
	drawPath();
	
	cout << "enter startX,startY" << endl;
	inputStart:cin >> sx >> sy;
	auto sNode = getNode(sx, sy);
	if (isBlock(sNode))
	{
		cout << "curr node is an obstacle node,reinput:" << endl;
		goto inputStart;
	}

	cout << "enter endX,endY" << endl;
	inputEnd:cin>> ex >> ey;
	auto eNode = getNode(ex, ey);
	if (isBlock(eNode))
	{
		cout << "curr node is an obstacle node,reinput:" << endl;
		goto inputEnd;
	}
	findPath();
	drawPath();
	exit(0);
};
