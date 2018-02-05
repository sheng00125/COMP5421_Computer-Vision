#include "pixelnode.h"

cv::Mat pixelNode::img;
// first seed initialization
pixelNode::pixelNode(){

}

pixelNode::pixelNode(int w, int h, int idx) {
    this->column = w;
    this->row = h;
    this->totalCost = 0;
    this->parent = NULL;
    this->index = idx;
    this->linkCost = new double[8];
}

pixelNode::~pixelNode(){
    delete parent;
    delete[] linkCost;
}

void pixelNode::setParent(pixelNode* p) {
    this->parent = p;
    return;
}

void pixelNode::computeNeighborsCost() {
}

// Access private data member
pixelNode* pixelNode::getParent(){
    return this->parent;
}

int pixelNode::getCol(){
    return this->column;
}

int pixelNode::getRow(){
    return this->row;
}

double pixelNode::getTotalCost(){
    return this->totalCost;
}

STATE pixelNode::getState(){
    return this->state;
}

int pixelNode::getIndex(){
    return this->index;
}







