#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "parser.h"
void buildAndTest();
void syntaxTranslate();
void preProc(std::string &expr);
int main(void) {
  //  buildAndTest();
  std::string str = "1+3-10";
  preProc(str);
  expr();
  std::cout << std::endl;
}
void buildAndTest() {
  using namespace std;

  string expr = "1+3+4+((123*12)+(23+23))";
  // start building the tree

  unique_ptr<Node> root = make_unique<Node>();
  Node::RootStack.push_back(&root);
  for_each(Node::ops.begin(), Node::ops.end(),
           [](string &e) { Node::addNode(*Node::RootStack.back(), e); });
  root->inTraverse();
  cout << endl;

  root->postTraverse();
  cout << endl;
}
void syntaxTranslate() {}
