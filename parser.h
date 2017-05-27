#ifndef Parser_H
#define Parser_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>
void printo(std::string op) { std::cout << op << " "; }
struct Node {
  // This vector save the pointer of locale root when left parentheses is
  // encoutered, for parentheses break the priority.
  // Use raw pointer here should have no problem
  void inTraverse() {
    if (this->left != nullptr) {
      this->left->inTraverse();
    }
    printo(op_);
    if (this->right != nullptr) {
      this->right->inTraverse();
    }
  }
  void postTraverse() {
    if (this->left != nullptr) {
      this->left->postTraverse();
    }
    if (this->right != nullptr) {
      this->right->postTraverse();
    }
    printo(op_);
  }

  static void addNode(std::unique_ptr<Node>& root, std::string val) {
    if (root->left == nullptr && val == "(") {
      std::unique_ptr<Node> tRoot = std::make_unique<Node>();
      root->left = std::move(tRoot);
      Node::RootStack.push_back(&(root->left));
    } else if (root->right == nullptr && val == "(") {
      std::unique_ptr<Node> tRoot = std::make_unique<Node>();
      root->right = std::move(tRoot);
      Node::RootStack.push_back(&(root->right));
    } else if (val == ")") {
      Node::RootStack.pop_back();
    } else if (root->left == nullptr) {
      root->left = std::make_unique<Node>();
      root->left->op_ = val;
    } else if (root->op_.empty()) {
      root->op_ = val;
    } else if (root->right == nullptr) {
      root->right = std::make_unique<Node>();
      root->right->op_ = val;
    } else if (val == "+" || val == "-") {
      std::unique_ptr<Node> tmp = std::make_unique<Node>();
      tmp->op_ = val;
      tmp->left = std::move(root);
      root = std::move(tmp);
    } else if (val == "*" || val == "/") {
      std::unique_ptr<Node> tmp = std::make_unique<Node>();
      tmp->op_ = val;
      tmp->left = std::move(root->right);
      root->right = std::move(tmp);
    } else {
      addNode(root->right, val);
    }
  }
  Node() = default;
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op_;

  static std::vector<std::unique_ptr<Node>*> RootStack;
  static std::vector<std::string> ops;
  static std::vector<std::string>::iterator Lookahead;
};
std::vector<std::unique_ptr<Node>*> Node::RootStack{};
std::vector<std::string> Node::ops{};
std::vector<std::string>::iterator Node::Lookahead;

// Another systax translator
void match(std::string op) {
  if (*Node::Lookahead == op) {
    ++Node::Lookahead;
  } else {
    std::cerr << "syntax error!" << std::endl;
    exit(1);
  }
}
bool is_number(const std::string& s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {
                         return !std::isdigit(c);
                       }) == s.end();
}
void term() {
  if (is_number(*Node::Lookahead)) {
    printo(*Node::Lookahead);
    match(*Node::Lookahead);
  } else {
    std::cerr << "syntax error" << std::endl;
    exit(0);
  }
}
void expr() {
  using namespace std;
  term();
  while (true) {
    if (*Node::Lookahead == "+") {
      match("+");
      term();
      printo("+");
      continue;
    } else if (*Node::Lookahead == "-") {
      match("-");
      term();
      printo("-");
      continue;
    }
    break;
  }
}
void preProc(std::string& expr) {
  using namespace std;
  // simple lexical processing
  Node::ops.push_back("");
  for (size_t i = 0; i != expr.size(); ++i) {
    if (isdigit(expr[i])) {
      if (i > 0 && !isdigit(expr[i - 1])) {
        Node::ops.push_back("");
      }
      Node::ops.back().push_back(expr[i]);
    } else {
      Node::ops.push_back("");
      Node::ops.back().push_back(expr[i]);
    }
  }
  for_each(Node::ops.begin(), Node::ops.end(), [](string e) { cout << e; });
  cout << endl;
  Node::Lookahead = Node::ops.begin();
}

#endif