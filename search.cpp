#include "search.h"
#include <utility>
#include <stdexcept>
#include <algorithm>

Vertex::Vertex( const Position& position_, int cost_, int h_, int heuristic_, const VertexPtr& parent_)
    : position( position_), cost( cost_), h( h_), heuristic( heuristic_), parent( parent_) {
}

Vertex::Vertex() {
}

bool PoolCompare::operator()( const VertexPtr& operand1, const VertexPtr& operand2) const {
    return operand1->position < operand2->position;
}

VertexPtr VertexPool::Find( const Position& position) {
    VertexPtr to_find( new Vertex( position, 0, 0, 0, VertexPtr()));
    Pool::iterator place = pool_.find( to_find);
    if ( place == pool_.end() ) {
        return VertexPtr();
    } else {
        return *place;
    }
}

void VertexPool::Insert( const VertexPtr& vertex) {
#ifdef _DEBUG
    std::pair<Pool::iterator, bool> result = 
#endif
	pool_.insert( vertex);
#ifdef _DEBUG
    if ( !result.second ) {
        throw std::logic_error( "Vertex already in pool");
    }
#endif
    return;
}

bool OpenedSetCompare::operator()( const VertexPtr& operand1, const VertexPtr& operand2) const {
    return (operand1->heuristic < operand2->heuristic)
           || ((operand1->heuristic == operand2->heuristic) && (operand1->cost > operand2->cost))
           || ((operand1->heuristic == operand2->heuristic) && (operand1->cost == operand2->cost) && (operand1->position < operand2->position)); 
}

bool OpenedSet::FindAndErase( const VertexPtr& vertex) {
    Set::iterator place = set_.find( vertex);
    if ( place == set_.end() ) {
        return false;
    }
    set_.erase( place);
    return true;
}

void OpenedSet::Insert( const VertexPtr& vertex) {
#ifdef _DEBUG
    std::pair<Set::iterator, bool> result = 
#endif
	set_.insert( vertex);
#ifdef _DEBUG
    if ( !result.second ) {
        throw std::logic_error( "Vertex already opened");
    }
#endif
    return;
}

VertexPtr OpenedSet::ExtractMin() {
    VertexPtr result;
#ifdef _DEBUG
    if ( set_.empty() ) {
        throw std::logic_error( "Nothing to extract");
    }
#endif
    Set::iterator min = set_.begin();
    result = *min;
    set_.erase( min);
    return result;
}

bool OpenedSet::Empty() {
    return set_.empty();
}

const int AStarSearcher::ITERATION_COUNT = 10000;

std::pair<bool, VertexPtr> AStarSearcher::SideSearch( OpenedSet& opened_set, VertexPool& pool, Position& goal, VertexPool& check) {
    int step = 0;
    ++step;
    VertexPtr current;
    while ( !opened_set.Empty() ) {
        if ( !(step % ITERATION_COUNT) ) {
            return std::make_pair(false, VertexPtr());
        }
        step++;
        current = opened_set.ExtractMin();
        Position position = current->position;
        if ( (position == goal) || (check.Find( position).use_count()) ) {
            return std::make_pair(true, current);
        }
        std::vector<Position::Move> moves = position.GetPossibleMoves();
        for ( std::vector<Position::Move>::iterator move = moves.begin(); move != moves.end(); ++move ) {
            position.Swap( move->from, move->to);
            int cost = current->cost + 1;
            VertexPtr next_vertex = pool.Find( position);
            if ( !next_vertex.use_count() ) {
                int h = position.UpdateDistance( goal, current->h, move->from, move->to);
                next_vertex.reset( new Vertex( position, cost, h, h + cost, current));
                pool.Insert( next_vertex);
                opened_set.Insert( next_vertex);
            } else if ( opened_set.FindAndErase( next_vertex)) {
                if ( next_vertex->cost > cost ) {
                    next_vertex->heuristic = next_vertex->h + cost;
                    next_vertex->cost = cost;
                    next_vertex->parent = current;
                }
                opened_set.Insert( next_vertex);
            }
            position.Swap( move->from, move->to);
        }
    }
    return std::make_pair(false, VertexPtr());
}


std::vector<Position> AStarSearcher::Search( const Position& source_, const Position& goal_, long long limit, std::string& error_msg) {
	error_msg = "";
	std::vector<Position> way;
    Position source = source_, goal = goal_;
    if ( !goal.IsSimular( source) ) {
        error_msg = "Positions not simular";
		return way;
    }
    OpenedSet opened_set_source;
    VertexPool pool_source;
    int dist = source.Distance( goal);
    VertexPtr source_vertex( new Vertex( source, 0, dist, dist, VertexPtr()));
    pool_source.Insert( source_vertex);
    opened_set_source.Insert( source_vertex);
    OpenedSet opened_set_goal;
    VertexPool pool_goal;
    dist = goal.Distance( source);
    VertexPtr goal_vertex( new Vertex( goal, 0, dist, dist, VertexPtr()));
    pool_goal.Insert( goal_vertex);
    opened_set_goal.Insert( goal_vertex);
    std::pair<bool, VertexPtr> search_result;
    VertexPtr source_end, goal_end;
	long long iteration = 0;
    while (iteration < limit) {
        search_result = SideSearch( opened_set_source, pool_source, goal, pool_goal);
        if ( search_result.first ) {
            source_end = search_result.second;
            goal_end = pool_goal.Find( source_end->position);
            break;
        }
		iteration += ITERATION_COUNT;
        search_result = SideSearch( opened_set_goal, pool_goal, source, pool_source);
        if ( search_result.first ) {
            goal_end = search_result.second;
            source_end = pool_source.Find( goal_end->position);
            break;
        }
		iteration += ITERATION_COUNT;
    }
	if (iteration >= limit) {
		error_msg = "Limit exceed";
		return way;
	}        

    while ( source_end.use_count() ) {
        way.push_back( source_end->position);
        source_end = source_end->parent;
    }
    std::reverse( way.begin(), way.end());
    goal_end = goal_end->parent;
    while ( goal_end.use_count() ) {
        way.push_back( goal_end->position);
        goal_end = goal_end->parent;
    }
    return way;
}
