//Edward Patrick Willey and Patrick Buzza
// Project 1a: Solving knapsack using exhaustive search
//

#include "d_except.h"
#include "d_random.h"
#include "knapsack.h"
#include <stack>

int bound(int index, knapsack k);

void branchAndBound(knapsack &k, string myfile, int greedybound);
int greedyKnapsack(knapsack k, string myfile);

int main() {
  int bound;
  char x;
  ifstream fin;
  stack<int> moves;
  string fileName, input = ".input", name;

  // Read the name of the graph from the keyboard or
  // hard code it here for testing.

  cout << "INPUT YOUR INPUT FILE NAME!" << endl;
  cin >> fileName;
  name = fileName + input;
  cout << name << endl;
  fin.open(name.c_str());
  if (!fin.is_open()) {
    cerr << "Cannot open " << fileName << endl;
    exit(1);
  }

  try {
    cout << "Reading knapsack instance" << endl;
    knapsack k(fin);
    bound = greedyKnapsack(k, fileName);
    branchAndBound(k, fileName, bound);
  }
  catch (indexRangeError &ex) {
    cout << ex.what() << endl;
    exit(1);
  }
  catch (rangeError &ex) {
    cout << ex.what() << endl;
    exit(1);
  }
}

int bound(int index, knapsack k) {
  int cost = k.getCostLimit() - k.getTotalCost();
  int val = k.getTotalValue();
  while (cost > 0) {
    if (cost < k.items[index].cost) {
      val = val + (int) (cost * k.items[index].costdensity);
      cost = 0;
    } else {
      val = val + k.items[index].value;
      cost = cost - k.items[index].cost;
    }
    index++;
  }
  return val;
}

void branchAndBound(knapsack &k, string myfile, int greedybound) {
  clock_t timestart = clock(), timenow; //Set the start of the clock for timeout
  int time = 10, size = k.getNumObjects(), bnd, ind = 0, Z, bestZ = greedybound;
  float timeelapsed = 0;
  deque<knapsack> d;
  d.push_back(k);

  while (!d.empty() || timeelapsed < time) {
    knapsack *newknap = new knapsack(d.front());
    ind = newknap->Check();
    if (ind >= size)
      break;
    bnd = bound(ind, *newknap);
    newknap->setBound(bnd);
    Z = newknap->getTotalValue();
    if (Z > bestZ) {
      bestZ = Z;
      knapsack *bestest = new knapsack(*newknap);
      bestest->setBound(bnd);
      bestest->printSolution(myfile);
      delete bestest;
    }
    if (bnd > bestZ) {
      d.push_back(*newknap);
      if (newknap->getTotalCost() + newknap->items[ind].cost <
          newknap->getCostLimit()) {
        newknap->select(newknap->items[ind].index);
        d.push_back(*newknap);
      }
    }
    d.pop_front();
    timenow = clock();
    timeelapsed = (float) (timenow - timestart) / CLOCKS_PER_SEC;
    if (timeelapsed > time)
      return;
    delete newknap;
  }
  cout << "Best solution found" << endl;

}

int greedyKnapsack(knapsack k, string myfile) {
  int tempcost = 0, tempvalue = 0, j = 0;
  sort(k.items.begin(), k.items.end());
  bool done = false;

  while (!done) {
    if (tempcost + k.getCost(k.items[j].index) < k.getCostLimit()) {
      k.select(k.items[j].index);
      tempcost += k.getCost(k.items[j].index);
      tempvalue += k.getValue(k.items[j].index);
      j++;
    } else {
      done = true;
    }
  }
  k.setBound(tempvalue);
  k.printSolution(myfile);
  return tempvalue;
}