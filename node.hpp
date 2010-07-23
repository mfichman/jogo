#pragma once

#include <tr1/memory>
#include <vector>
#include <string>

class Node {
public:
    typedef std::tr1::shared_ptr<Node> Ptr; 

    /* Creates a new node */
    Node(const std::string& value);

    /* Creates a new node */
    Node();

    /* Adds a child node to this node */
    void child(Node::Ptr child); 

    /* Returns the child node at the given index */
    Node::Ptr child(size_t index) const;

    /* Returns the number of child nodes */
    size_t nchild() const;

    /* Returns the value of the node */
    const std::string& value() const;
    
private:
    std::vector<Node::Ptr> child_;
    std::string value_;
};
