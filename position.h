#pragma once
#ifndef _POSITION_H_
#define _POSITION_H_

#include <vector>
#include <iostream>

using std::vector;

// Класс для хранения и обработки состояния игрового поля.
// Элементами поля могут быть положительные целые числа (не обязательно различные),
// камни (Position::STONE) и пустые места( Position::BLANK).
class Position {
public:
    struct Move;
    Position();
    Position( int height, int width);
    Position( int height, int width, const vector<int>& position);
    Position( const vector< vector<int> >& position);
    void SetSize( int height, int width);
    void SetField( int v, int h, int value);
    void SetField( int index, int value);
    int GetField( int v, int h) const;
    int GetField( int index) const;
    bool operator==( const Position& operand) const;
    bool operator!=( const Position& operand) const;
    bool operator<( const Position& operand) const;
	// Делает move_count случайных допустимых ходов.
    void Shuffle( int move_count);
    Position GetShuffled( int step_count) const;
    vector<Move> GetPossibleMoves() const;
    void Swap( int vfrom, int hfrom, int vto, int hto);
    void Swap( int from, int to);
    Position GetSwaped( int vfrom, int gfrom, int vto, int gto) const;
    Position GetSwaped( int from, int to) const;
	// Сверка двух позиций на подобность (все камни совпадают,
	// кол-ва элементов с одним и тем же номером совполают).
    bool IsSimular( const Position& to) const;
	// Эвристическая оценка расстония до вершины to)
    int Distance( const Position& to) const;
	// То же, что и Distance, но работает быстрее, за счет использования
	// информации с предыдущего хода
    int UpdateDistance( const Position& to, int old_distance, int move_from, int move_to) const;
	int Height() const {return height_;}
	int Width() const {return width_;}
    friend std::ostream& operator<<(std::ostream &stream, const Position& t);
    const static int BLANK;
    const static int STONE;

    
private:
    vector<int> position_;
    int height_;
    int width_;
};

struct Position::Move {
    Move();
    Move( int f, int t);
    int from;
    int to;
    bool operator==( const Move& operand) const;
};

std::ostream& operator<<(std::ostream& stream, const Position& t);

#endif /* _POSITION_H_ */
