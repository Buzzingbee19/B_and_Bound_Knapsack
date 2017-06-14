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
#include <deque>


using namespace std;

int bound(int index, knapsack& k);
void branchAndBound (knapsack& k, string myfile);

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName, input = ".input", namie;

   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   cout << "INPUT YOUR INPUT FILE NAME!" << endl;
   cin >> fileName;
   namie = fileName + input;
   cout << namie << endl;
   //fileName = "knapsack12.input";
   fin.open(namie.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      cout << "Reading knapsack instance" << endl;
      knapsack k(fin);
      branchAndBound(k, fileName);
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
   int cost = k.getCostLimit() - k.totalCost;
   int val = k.totalValue;
   while (cost > 0)
   {
      if (cost < k.items[index].cost) {
         val = val + (int)(cost * k.items[index].costdensity);
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

void branchAndBound (knapsack& k, string myfile)
{
   clock_t timestart = clock(), timenow; //Set the start of the clock for timeout
   int time = 600, size = k.getNumObjects(), bnd, ind = 0, Z, bestZ = 0;
   float timeelapsed = 0;
   deque <knapsack> d;
   d.push_back(k);

   while (!d.empty() || timeelapsed < time)
   {
      bnd = bound(ind, d.front());
      knapsack newkap(d.front());
      ind = newkap.Check();
      if (ind >= size)
         break;
      newkap.setBound(bnd);
      Z = newkap.getValue();
      if (Z > bestZ) {
         bestZ = Z;
         knapsack bestest(newkap);
         bestest.printSolution(myfile);
      }
      if (bnd > bestZ || ind+1 < size)
      {
         d.push_back(newkap);
         if (newkap.totalCost + newkap.items[ind].cost < newkap.getCostLimit()) {
            newkap.select(newkap.items[ind].index);
            d.push_back(newkap);
         }
      }
      d.pop_front();
      timenow = clock();
      timeelapsed = (float)(timenow - timestart)/CLOCKS_PER_SEC;
   }

   cout << "Best solution found" << endl;

}