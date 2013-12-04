#pragma once
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "position.h"

struct Vertex;
typedef boost::shared_ptr<Vertex> VertexPtr;

struct Vertex {
    Vertex();
	Vertex( const Position& position_, int cost_, int h_, int heuristic_, const VertexPtr& parent_);
    Position position;
    int cost;
    int h;
    int heuristic;
    VertexPtr parent;
};


class PoolCompare {
public:
    bool operator()( const VertexPtr& operand1, const VertexPtr& operand2) const;
};

class VertexPool {
public:
    typedef std::set<VertexPtr, PoolCompare> Pool; 
    VertexPtr Find( const Position& position);
    void Insert( const VertexPtr& vertex);
private:
    Pool pool_;
};

class OpenedSetCompare {
public:
    bool operator()( const VertexPtr& operand1, const VertexPtr& operand2) const;
};

class OpenedSet {
public:
    typedef std::set<VertexPtr, OpenedSetCompare> Set;
    bool FindAndErase( const VertexPtr& vertex);
    void Insert( const VertexPtr& vertex);
    VertexPtr ExtractMin();
    bool Empty();
private:
    Set set_;

};

class AStarSearcher {
public:
    std::vector<Position> Search( const Position& source, const Position& goal, long long limit, std::string& error_msg);
    std::pair<bool, VertexPtr> SideSearch( OpenedSet& opened_set, VertexPool& pool, Position& goal, VertexPool& check);
	const static int ITERATION_COUNT;
};

#endif /* _SEARCH_H_ */

