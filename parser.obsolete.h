#ifndef Parser_H
#define Parser_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>
template <typename T>
struct Node {
  // This vector save the pointer of locale root when left parentheses is
  // encoutered, for parentheses break the priority.
  // Use raw pointer here should have no problem
  static std::vector<std::unique_ptr<Node<T>>*> RootStack;

  void inTraverse() {
    if (this->left != nullptr) {
      this->left->inTraverse();
    }
    std::cout << op_ << " ";
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
    std::cout << op_ << " ";
  }

  static void addNode(std::unique_ptr<Node<T>>& root, T val) {
    if (root->left == nullptr && val == "(") {
      std::unique_ptr<Node<T>> tRoot = std::make_unique<Node<T>>();
      root->left = std::move(tRoot);
      Node<T>::RootStack.push_back(&(root->left));
    } else if (root->right == nullptr && val == "(") {
      std::unique_ptr<Node<T>> tRoot = std::make_unique<Node<T>>();
      root->right = std::move(tRoot);
      Node<T>::RootStack.push_back(&(root->right));
    } else if (val == ")") {
      Node<T>::RootStack.pop_back();
    } else if (root->left == nullptr) {
      root->left = std::make_unique<Node<T>>();
      root->left->op_ = val;
    } else if (root->op_.empty()) {
      root->op_ = val;
    } else if (root->right == nullptr) {
      root->right = std::make_unique<Node<T>>();
      root->right->op_ = val;
    } else if (val == "+" || val == "-") {
      std::unique_ptr<Node<T>> tmp = std::make_unique<Node<T>>();
      tmp->op_ = val;
      tmp->left = std::move(root);
      root = std::move(tmp);
    } else if (val == "*" || val == "/") {
      std::unique_ptr<Node<T>> tmp = std::make_unique<Node<T>>();
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
  T op_;
};
template <typename T>
std::vector<std::unique_ptr<Node<T>>*> Node<T>::RootStack{};

//Another systax tra



#endif