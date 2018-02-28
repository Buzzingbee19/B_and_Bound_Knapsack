//Edward Patrick Willey and Patrick Buzza
// Knapsack class
// Version f08.1
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

class knapsack {
 public:
  knapsack(ifstream &fin);
  knapsack(const knapsack &);
  ~knapsack();
  void printSolution(string myfile);
  void select(int);
  void unSelect(int);
  void setBound(int);
  void sortWeighted();
  void sortOrder();
  int getTotalCost() const;
  int getCost(int) const;
  int getTotalValue() const;
  int getValue(int) const;
  int getNumObjects() const;
  int getCostLimit() const;
  int getBound() const;
  int Check();
  bool isSelected(int) const;
  struct item {
    double costdensity;
    int index;
    int cost;
    int value;
    bool checked;
    bool operator<(const item &str) const {
      return (costdensity > str.costdensity);
    }
  };
  vector<item> items;

 private:
  int totalValue;
  int totalCost;
  int numObjects;
  int costLimit;
  int ValueBound;
  vector<int> index;
  vector<int> value;
  vector<int> cost;
  vector<bool> selected;
  vector<double> costdensity;
};