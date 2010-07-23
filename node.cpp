#include <node.hpp>
#include <cassert>

using namespace std;

Node::Node(const std::string& value) :
    value_(value) {
}

Node::Node() {

}

void Node::child(Node::Ptr child) {
    child_.push_back(child);
}

Node::Ptr Node::child(size_t index) const {
    assert("Invalid child index" && index < child_.size());
    return child_[index];
}

size_t Node::nchild() const {
    return child_.size();
}

const std::string& Node::value() const {
    return value_;
}
