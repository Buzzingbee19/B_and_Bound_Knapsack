//Edward Patrick Willey and Patrick Buzza
// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <fstream>
#include <queue>

//test

using namespace std;

#include "d_except.h"
#include "d_random.h"
#include "knapsack.h"
#include <cmath>
#include <stack>
#include <algorithm>

struct Node
{
	int level, profit;
	int bound;
	float weight;
};

void exhaustiveKnapsack(knapsack& k, int time);
void greedyKnapsack(knapsack& k, int time);
void dynamicKnapsack(knapsack &k, int time);
int bound(Node u, knapsack &k);

int main()
{
    char x;
    ifstream fin;
    stack <int> moves;
    string fileName;

    // Read the name of the graph from the keyboard or
    // hard code it here for testing.

    //fileName = "knapsack512.input";

    cout << "Enter filename" << endl;
    cin >> fileName;

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
        //greedyKnapsack(k, 600);
		dynamicKnapsack(k, 600);


        cout << endl << "Best solution found" << endl;
        k.printSolution();


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

void greedyKnapsack(knapsack& k, int time)
{
	clock_t timestart = clock(); //Set the start of the clock for timeout
	clock_t timenow;
	int timeelapsed = 0, tempcost = 0, tempvalue = 0;
	int size = k.getNumObjects();
	vector<bool> tempstring, bestobject; //Strings to hold the current and best forms of packing the stack
	//vector<double> costdensity;
	tempstring.resize(size);
	bestobject.resize(size);

	cout << "Sort" << endl;
	sort(k.items.begin(), k.items.end()); //sorts struct in decending order

	/*for (int l = 0; l < size; l++)
	{
		cout << "index: "<<  k.items[l].index << " CD: " << k.items[l].costdensity << endl;
	}*/

	int j = 0;
	bool done = false;

	while( done != true && timeelapsed < time)  //as long as the list isnt over and the time isnt up
	{

		if (tempcost < k.getCostLimit()) //if the last added didnt go over the limit
		{
			k.select(k.items[j].index);

			tempcost += k.getCost(k.items[j].index);
			tempvalue += k.getValue(k.items[j].index);

			j++; ///
		}
		else if(tempcost > k.getCostLimit()) //if the last added went over the limit remove it and move on
		{
			j--;

			k.unSelect(k.items[j].index);
		
			j = j + 2;
		}

		if (j >= k.getNumObjects())
		{
			cout << "All objects checked" << endl;
			done = true;
		}

		timenow = clock();
		timeelapsed = (float)(timenow - timestart) / CLOCKS_PER_SEC;


	}


	
}

void exhaustiveKnapsack(knapsack& k, int time)
{

   //initialize all variables
   clock_t timestart = clock(); //Set the start of the clock for timeout
   clock_t timenow;
   int size, bestcost, bestvalue = 0, tempcost, tempvalue, x, z;
   randomNumber r;
   size = k.getNumObjects();
   vector<bool> tempstring, bestobject; //Strings to hold the current and best forms of packing the stack
   tempstring.resize(size);
   bestobject.resize(size);
   float timeelapsed;
   int temp = size-1;
 //  k.sortWeighted();

   //loops through each possible combination of objects, checking their legality
   for (int i = 0; i < pow(2,size); i++) {

      //re-initializes variables for each attempted combo of objects
      tempcost = 0;
      tempvalue = 0;
      z = temp;
      x = i;

      //converts the loop iteration ino a unique binary string, and selects
      // those items.
      while (x > 0)
      {
         if (x % 2)
            k.select(z);
         else
            k.unSelect(z);
         z--;
         x=x/2;
      }

      //chooses the objects to be placed in the bag
      for (int j = 0; j < size; j++) {
         if (k.isSelected(j)) {
            tempcost += k.getCost(j);
            tempvalue += k.getValue(j);
            tempstring[j] = true;
         }
         else
            tempstring[j] = false;
      }

      // checks to determine if the set of objects is a legal set, and if it
      // is the most valuable set to this point
      if (tempcost <= k.getCostLimit() & tempvalue > bestvalue) {
         bestvalue = tempvalue;
         bestobject = tempstring;
         bestcost = tempcost;
      }

      //updates the amount of time the function has taken
      timenow = clock();
      timeelapsed = (float)(timenow - timestart) / CLOCKS_PER_SEC;

      //checks to see if the function has exceeded it's time limit or has
      // checked every object pairing
      if (timeelapsed >= time || i == pow(2,size)-1)
      {
         for (int l = 0; l < size; l++)
         {
            k.unSelect(l);
            if (bestobject[l])
               k.select(l);
         }
         //updates the knapsack's cost and value for the best case
         k.totalValue = bestvalue;
         k.totalCost = bestcost;
  //       k.sortOrder();
         return;
      }
   }
}

void dynamicKnapsack(knapsack& k, int time)
{
	clock_t timestart = clock(); //Set the start of the clock for timeout
	clock_t timenow;
	int timeelapsed = 0, tempcost = 0, tempvalue = 0, maxProfit = 0;
	int size = k.getNumObjects();
	vector<bool> tempstring, bestobject; //Strings to hold
	//the current and best forms of packing the stack
	//vector<double>costdensity;
	tempstring.resize(size);
	bestobject.resize(size);

	cout << "Sort" << endl;
	sort(k.items.begin(), k.items.end()); //sorts struct in decending order
	/*for (int l = 0; l < size; l++)
	{
	cout << "index: "<<  k.items[l].index << " CD: " << k.items[l].costdensity << endl;
	}*/

	queue<Node> Q;
	Node u, v;

	//dummy node
	u.level = -1;
	u.profit = u.weight = 0;
	Q.push(u);

	while (!Q.empty())
	{
		u = Q.front();
		Q.pop();

		// If it is starting node, assign level 0
		if (u.level == -1)
			v.level = 0;

		// If there is nothing on next level
		if (u.level == k.getNumObjects() - 1)
			continue;

		v.level = u.level + 1;

		v.weight = u.weight + k.getCost(k.items[v.level].index);
		v.profit = u.profit + k.getValue(k.items[v.level].index);

		if (v.weight <= k.getCostLimit() && v.profit > maxProfit)
		{
			k.select(k.items[v.level].index);
			maxProfit = v.profit;
		}

		v.bound = bound(v, k);

		if (v.bound > maxProfit)
			Q.push(v);

		v.weight = u.weight;
		v.profit = u.profit;
		v.bound = bound(v, k);

		if (v.bound > maxProfit)
			Q.push(v);

	}
	
	//something down here to save the list it creates

	Node temp;
	cout << maxProfit;
	while (!Q.empty())
	{
		cout << "Print" << endl;
		temp = Q.front();
		cout << temp.level << endl;
		Q.pop();
	}

	int hold;
	cin >> hold;


	}


int bound(Node u, knapsack &k)
{
	if (u.weight >= k.getCostLimit())
		return 0; //if weight is over the limit

	int profit_bound = u.profit;

	int j = u.level + 1;

	int totalweight = u.weight;

	while ((j < k.getNumObjects()) && (totalweight + k.getCost(k.items[j].index) <= k.getCostLimit()))
	{
		totalweight += k.getCost(k.items[j].index);
		profit_bound += k.getValue(k.items[j].index);
		j++;
	}

	if (j < k.getNumObjects())
		profit_bound += (k.getCostLimit() - totalweight)*(k.getValue(k.items[j].index) / k.getCost(k.items[j].index));

	return profit_bound;


}