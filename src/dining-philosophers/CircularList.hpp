#pragma once

#include <memory>

#include <functional>

template <class T>
class CircularList {
private:
  struct Node {
    std::shared_ptr<Node> prev = nullptr;
    std::shared_ptr<Node> next = nullptr;

    T value;

    Node() {}
    Node(const T& avalue) : value(avalue) {}
    Node(T&& avalue) : value(std::move(avalue)) {}
  };

  using NodePtr = std::shared_ptr<Node>;
public:
  class Iterator;

  CircularList() {}

  CircularList(size_t count) {
    if (!count) return;

    m_head = std::make_shared<Node>();
    NodePtr prev = m_head;

    for (size_t i = 1; i < count; i++) {
      insertBetween(std::make_shared<Node>(), prev);
      prev = prev->next;
    }

    prev->next = m_head;
    m_head->prev = prev;
  }

  CircularList(std::initializer_list<T> initData) {
    NodePtr prevNode = nullptr;
    for (auto& el : initData) {
      NodePtr node = std::make_shared<Node>(el);

      node->prev = prevNode;
      if (prevNode) {
        prevNode->next = node;
      } else {
        m_head = node;
      }

      prevNode = node;
    }

    if (!prevNode) return;

    prevNode->next = m_head;
    m_head->prev = prevNode;
  }

  void insert(T&& value) {
    insertBetween(
      std::make_shared<Node>(std::move(value)),
      m_head ? m_head->prev : nullptr,  
      m_head
    );
  }

  void insert(const T& value) {
    insertBetween(
      std::make_shared<Node>(value), 
      m_head ? m_head->prev : nullptr, 
      m_head
    );
  }

  void forEach(std::function<void(T&)> func) const { 
    auto it = begin();
    
    if (it == end()) return;

    do {
      func(*it);
      std::advance(it, 1);
    } while (it != begin());
  }

  Iterator begin() const {
    return Iterator(m_head);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }
private:
  NodePtr m_head = nullptr;

  void insertBetween(NodePtr&& node, const NodePtr& before = nullptr, const NodePtr& after = nullptr) {
    if (before) {
      node->prev = before;
      before->next = node;
    }

    if (after) {
      node->next = after;
      after->prev = node;
    }

    if (!before && !after) {
      m_head = node;
      m_head->next = m_head;
      m_head->prev = m_head;
    }
  }

public:
  class Iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = T&;
	using pointer = T*;
    using difference_type = ptrdiff_t;

    Iterator() {}
    ~Iterator() {}
    Iterator(const NodePtr& node) : m_current(node) {}
    Iterator(const Iterator& other) : m_current(other.m_current) {}

    Iterator& operator= (const Iterator& other) {
      m_current = other.m_current;
      return *this;
    }

    Iterator& operator++ () {
      m_current = m_current->next;
      return *this;
    }

    Iterator operator++ (int) {
      auto tmp = *this;
      m_current = m_current->next;
      return tmp;
    }

    Iterator& operator-- () {
      m_current = m_current->prev;
      return *this;
    }

    Iterator operator-- (int) {
      auto tmp = *this;
      m_current = m_current->prev;
      return tmp;
    }

    reference operator* () const {
      return m_current->value;
    }

    pointer operator-> () const {
      return &m_current->value;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return lhs.m_current == rhs.m_current;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return !(lhs == rhs);
    }

    friend void swap(Iterator& lhs, Iterator& rhs) {
      std::swap(lhs.m_current, rhs.m_current);
    }
  private:
    NodePtr m_current = nullptr;
  };
};
