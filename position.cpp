#include "position.h"
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
#include <limits>


const int Position::BLANK = 0;
const int Position::STONE = -1;

Position::Position() {
}

Position::Position( int height, int width)
    : height_( height), width_( width) {
#ifdef _DEBUG
    if ( (height <= 0) || (width <= 0) ) {
        throw std::invalid_argument( (height <= 0) ? "height" : "width");
    }
#endif
    position_.assign( height * width, BLANK);
    return;
}

Position::Position( int height, int width, const vector<int>& position)
    : height_( height), width_( width), position_( position) {
#ifdef _DEBUG
    if ( (height <= 0) || (width <= 0) ) {
        throw std::invalid_argument( (height <= 0) ? "height" : "width");
    }
    if ( height * width != position.size() ) {
        throw std::invalid_argument( "position");
    }
#endif
    return;
}

Position::Position( const vector< vector<int> >& position) {
#ifdef _DEBUG
    if ( !position.size() ) {
        throw std::invalid_argument( "position");
    }
#endif
    height_ = int( position.size());
    width_ = int( position.front().size());
    position_.clear();
    for ( vector< vector<int> >::const_iterator line = position.begin();
          line != position.end(); ++line ) {
#ifdef _DEBUG
        if ( line->size() != width_ ) {
            throw std::invalid_argument( "position");
        }
#endif
        for ( vector<int>::const_iterator field = line->begin();
              field != line->end(); ++field ) {
#ifdef _DEBUG
            if ( (*field < 0) && (*field != STONE) ) {
                throw std::invalid_argument( "position");
            }
#endif
            position_.push_back( *field);
        } 
    }
    return;
}

void Position::SetSize( int width, int height) {
    Position( width, height);
    return;
}

void Position::SetField( int v, int h, int value) {
#ifdef _DEBUG
    if ( (v >= height_) || (v < 0) ) {
        throw std::invalid_argument( "v");
    }
    if ( (h >= width_) || (h < 0) ) {
        throw std::invalid_argument( "h");
    }
    if ( (value < 0) && (value != STONE) ) {
        throw std::invalid_argument( "value");
    }
#endif
    position_[v * width_ + h] = value;
    return;
}

void Position::SetField( int index, int value) {
#ifdef _DEBUG
    if ( (index >= width_ * height_) || (index < 0) ) {
        throw std::invalid_argument( "index");
    }
    if ( (value < 0) && (value != STONE) ) {
        throw std::invalid_argument( "value");
    }
#endif
    position_[index] = value;
    return;
}

int Position::GetField( int v, int h) const {
#ifdef _DEBUG
    if ( (v >= height_) || (v < 0) ) {
        throw std::invalid_argument( "v");
    }
    if ( (h >= width_) || (h < 0) ) {
        throw std::invalid_argument( "h");
    }
#endif
    return position_[v * width_ + h];
}

int Position::GetField( int index) const {
#ifdef _DEBUG
    if ( (index >= width_ * height_) || (index < 0) ) {
        throw std::invalid_argument( "index");
    }
#endif
    return position_[index];
}

bool Position::operator==( const Position& operand) const {
    return (width_ == operand.width_) && (height_ == operand.height_)
        && (position_ == operand.position_);
}

bool Position::operator!=( const Position& operand) const {
    return (width_ != operand.width_) || (height_ != operand.height_)
        || (position_ != operand.position_);
}

bool Position::operator<( const Position& operand) const {
#ifdef _DEBUG
    if ( (width_ != operand.width_) || (height_ != operand.height_) ) {
       throw std::logic_error( "Incomporable positions");
    }
#endif
    for ( size_t index = 0; index < position_.size(); ++index ) {
       if ( position_[index] < operand.position_[index] ) {
           return true;
       } else if ( position_[index] > operand.position_[index] ) {
           return false;
       }
    }
    return false;
} 

void Position::Shuffle( int move_count) {
    for ( int step = 0; step < move_count; ++step ) {
        vector<Move> moves = GetPossibleMoves();
        if ( !moves.size() ) {
            return;
        }
        Move move = moves[rand() % moves.size()];
        Swap( move.from, move.to);
    }
    return;
}

Position Position::GetShuffled( int move_count) const {
    Position result = *this;
    result.Shuffle( move_count);
    return result;
}

vector<Position::Move> Position::GetPossibleMoves() const {
    vector<Move> result;
    for ( int field = 0; field < int( position_.size()); ++field ) {
        if ( position_[field] == BLANK ) {
            if ( field >= width_ ) {
                int to = field - width_;
                if ( (position_[to] != BLANK) && (position_[to] != STONE) ) {
                    result.push_back( Move( field, to));
                }
            }
            if ( (field + 1) % width_ ) {
                int to = field + 1;
                if ( (position_[to] != BLANK) && (position_[to] != STONE) ) {
                    result.push_back( Move( field, to));
                }
            }
            if ( field + width_ < width_ * height_ ) {
                int to = field + width_;
                if ( (position_[to] != BLANK) && (position_[to] != STONE) ) {
                    result.push_back( Move( field, to));
                } 
            }
            if ( field % width_ ) {
                int to = field - 1;
                if ( (position_[to] != BLANK) && (position_[to] != STONE) ) {
                    result.push_back( Move( field, to));
                }
            }
        }
    }
    return result;
}


void Position::Swap( int vfrom, int hfrom, int vto, int hto) {
#ifdef _DEBUG
    if ( (vfrom >= height_) || (vfrom < 0) ) {
        throw std::invalid_argument( "vfrom");
    }
    if ( (hfrom >= width_) || (hfrom < 0) ) {
        throw std::invalid_argument( "hfrom");
    }
    if ( (vto >= height_) || (vto < 0) ) {
        throw std::invalid_argument( "vto");
    }
    if ( (hto >= width_) || (hto < 0) ) {
        throw std::invalid_argument( "hto");
    }
#endif
    std::swap( position_[vfrom * width_ + hfrom], position_[vto * width_ + hto]);
    return;
}

void Position::Swap( int from, int to) {
#ifdef _DEBUG
    if ( (from >= width_ * height_) || (from < 0) ) {
        throw std::invalid_argument( "from");
    }
    if ( (to >= width_ * height_) || (to < 0) ) {
        throw std::invalid_argument( "to");
    }
#endif
    std::swap( position_[from], position_[to]);
    return;
}
    
Position Position::GetSwaped( int vfrom, int hfrom, int vto, int hto) const {
    Position result = *this;
    result.Swap( vfrom, hfrom, vto, hto);
    return result;
}

Position Position::GetSwaped( int from, int to) const {
    Position result = *this;
    result.Swap( from, to);
    return result;
}
    
std::ostream& operator<<(std::ostream &stream, const Position& t) {
    for ( size_t i = 0; i < t.position_.size(); ++i ) {
        if ( t.position_[i] == Position::STONE ) {
            std::cout << "X ";
        } else if ( t.position_[i] == Position::BLANK ) {
            std::cout << ". ";
        } else {
            std::cout << t.position_[i] << ' ';
        }
        if ( !((i + 1) % t.width_) ) {
            std::cout << std::endl;
        } 
    }
    return stream;
}

Position::Move::Move( int f, int t)
    : from( f), to( t) {
    return;
} 

Position::Move::Move()
    : from( 0), to( 0) {
    return;
}

bool Position::Move::operator==( const Move& operand) const {
    return ((from == operand.from) && (to == operand.to))
           || ((from == operand.to) && (to == operand.from));
}

bool Position::IsSimular( const Position& to) const {
    if ( (width_ != to.width_) || (height_ != to.height_) ) {
        return false;
    }
    std::map<int, int> counter;
    for ( int v = 0; v < height_; ++v ) {
        for ( int h = 0; h < width_; ++h ) {
            if ( (position_[v * width_ + h] == STONE) && (to.position_[v * width_ + h] != STONE) ) {
                return false;
            }
            counter[position_[v * width_ + h]]++;
            counter[to.position_[v * width_ + h]]--;
        }
    }
    for ( std::map<int, int>::iterator it = counter.begin();
          it != counter.end(); ++it ) {
        if ( it->second ) {
            return false;
        }
    }
    return true;
};

int Position::Distance( const Position& to) const {
    std::map< int, vector<int> > invert_positions;
    int result = 0;
    for ( int i = 0; i < width_ * height_; ++i ) {
        int value = to.position_[i];
        if ( (value != BLANK) && (value != STONE) ) {
            invert_positions[value].push_back( i);
        }
    }
    for ( int from = 0; from < width_ * height_; ++from ) {
        int value = position_[from];
        if ( (value == BLANK) || (value == STONE) ) {
            continue;
        }
        int min_distance = std::numeric_limits<int>::max(); 
        for( vector<int>::const_iterator to = invert_positions[value].begin();
             to != invert_positions[value].end(); ++to ) {
            min_distance = std::min( min_distance, abs( from % width_ - *to % width_) + abs( from / width_ - *to / width_));
        }
        result += min_distance;
    }
    return result;
}

int Position::UpdateDistance( const Position& to, int old_distance, int move_from, int move_to) const {
    int value, old_from, new_from;
    if ( position_[move_to] != BLANK ) {
        value = position_[move_to];
        old_from = move_from;
        new_from = move_to;
    } else {
        value = position_[move_from];
        old_from = move_to;
        new_from = move_from;
    }
    int old_part = std::numeric_limits<int>::max(), new_part = std::numeric_limits<int>::max();
    for ( int i = 0; i < width_ * height_; ++i ) {
        if ( to.position_[i] == value ) {
            old_part = std::min( old_part, abs( old_from % width_ - i % width_) + abs( old_from / width_ - i / width_));
            new_part = std::min( new_part, abs( new_from % width_ - i % width_) + abs( new_from / width_ - i / width_));
        }
    }
    return old_distance - old_part + new_part;
}
