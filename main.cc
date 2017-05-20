#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "parser.h"
void buildAndTest();
int main(void) { buildAndTest(); }
void buildAndTest() {
  using namespace std;

  string expr = "1+3+4+((123*12)+(23+23))";
  // simple lexical processing
  vector<string> split{};
  split.push_back("");
  for (size_t i = 0; i != expr.size(); ++i) {
    if (isdigit(expr[i])) {
      if (i > 0 && !isdigit(expr[i - 1])) {
        split.push_back("");
      }
      split.back().push_back(expr[i]);
    } else {
      split.push_back("");
      split.back().push_back(expr[i]);
    }
  }
  for_each(split.begin(), split.end(), [](string e) { cout << e; });
  cout << endl;

  //start building the tree

  unique_ptr<Node<string>> root = make_unique<Node<string>>();
  Node<string>::RootStack.push_back(&root);
  for_each(split.begin(), split.end(), [&root](string &e) {
    Node<string>::parse(*Node<string>::RootStack.back(), e);
  });
  root->inTraverse();
  cout << endl;
  
  root->postTraverse();
  cout << endl;
}