//Edward Patrick Willey and Patrick Buzza
// Project 1a: Solving knapsack using exhaustive search
//

#include "d_except.h"
#include "d_random.h"
#include "knapsack.h"
#include <cmath>
#include <stack>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <deque>


using namespace std;

int bound(int index, knapsack& k);
void branchAndBound (knapsack& k);

int main()
{
    char x;
    ifstream fin;
    stack <int> moves;
    string fileName;

    // Read the name of the graph from the keyboard or
    // hard code it here for testing.

    fileName = "knapsack8.input";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try
    {
       cout << "Reading knapsack instance" << endl;
       knapsack k(fin);

		cout << "test" << endl;
		 branchAndBound(k);
    }

    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}

int bound(int index, knapsack& k)
{
	int cost = k.CostBound - k.totalCost;
	int val = k.totalValue;
	while (cost > 0)
	{
		if (cost < k.items[index].cost) {
			val = val + cost * k.items[index].costdensity;
			cost = 0;
		}
		else {
			val = val + k.items[index].value;
			cost = cost - k.items[index].cost;
		}
		index++;
	}
	return val;
}

void branchAndBound (knapsack& k)
{
	clock_t timestart = clock(); //Set the start of the clock for timeout
	clock_t timenow;
	int timeelapsed = 0, tempcost = 0, tempvalue = 0, maxProfit = 0;
	int size = k.getNumObjects();
	int bnd, ind = 0, Z, bestZ;
	vector<bool> bestobject; //Strings to hold
	bestobject.resize(size);

	deque <knapsack> d;
	d.push_back(k);
	while (!d.empty())
	{
		bnd = bound(ind, d.front());
		k.setBound(bnd);
		knapsack newkap(d.front);
		Z = newkap.getValue();
		if (Z > bestZ) {
			bestZ = Z;
			knapsack bestest(newkap);
		}
		if (bnd > bestZ)
		{
			d.push_back(newkap);
			if (newkap.totalCost + newkap.items[ind].cost < newkap.getCostLimit()) {
				newkap.select(ind);
				d.push_back(newkap);
			}
		}
		d.pop_front();
	}

	cout << endl << "Best solution found" << endl;
	bestest.printSolution();

}