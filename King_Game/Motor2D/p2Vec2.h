#ifndef _P2VEC2_H
#define _P2VEC2_H
#include <math.h>

template <class T>
class p2Vec2
{
public:
	p2Vec2();
	p2Vec2(T,T);
	p2Vec2(T a[2]);
	p2Vec2(p2Vec2& other);
	p2Vec2 operator+(p2Vec2 other);
	p2Vec2 operator-(p2Vec2 other);
	int operator+=(p2Vec2 other);
	int operator-=(p2Vec2 other);
	void operator=(p2Vec2 other);
	bool operator==(p2Vec2 other);
	void normalize(); 
	void zero();
	bool is_zero();
	//int distance_to(p2Vec2 other);
	~p2Vec2() {};
private:
	T x, y;
};

template<class T>
p2Vec2<T>::p2Vec2() {
	x = 0;
	y = 0;
}

template<class T>
p2Vec2<T>::p2Vec2(p2Vec2& other) {
	x = other.x;
	y = other.y;
}

template <class T>
p2Vec2<T>::p2Vec2(T px, T py) {
	x = px;
	y = py;
}

template <class T>
p2Vec2<T>::p2Vec2(T a[2]) {
	x = a[0];
	y = a[1];
}

template<class T>
p2Vec2<T> p2Vec2<T>::operator+(p2Vec2 other) {
	vec3 result;

	result.x = this->x + other.x;
	result.y = this->y + other.y;

	return result;
}

template<class T>
p2Vec2<T> p2Vec2<T>::operator-(p2Vec2 other) {
	vec3 result;
	
	result.x = this->x - other.x;
	result.y = this->y - other.y;

	return result;
}

template <class T>
int p2Vec2<T>::operator+=(p2Vec2 other) {
	x += other.x;
	y += other.y;
}

template<class T>
int p2Vec2<T>::operator-=(p2Vec2 other) {
	x -= other.x;
	y -= other.y;
}

template<class T>
void p2Vec2<T>::operator=(p2Vec2 other) {
	x = other.x;
	y = other.y;
}

template<class T>
bool p2Vec2<T>::operator==(p2Vec2 other) {
	if ((x == other.x) && (y == other.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
void p2Vec2<T>::normalize() {
	double module;
	module = sqrt(x*x + y*y);
	x = x / module;
	y = y / module;
}

template<class T>
void p2Vec2<T>::zero() {
	x = 0;
	y = 0;
}

template<class T>
bool p2Vec2<T>::is_zero() {
	if ((x == 0) && (y == 0)) { return true; }
	else { return false; }
}

typedef p2Vec2<int> ivec2;
typedef p2Vec2<float> fVec2;

/*
template<class T>
int p2Vec2<T>::distance_to(p2Vec2 other) {
	return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
}
*/

#endif // !_P2VEC2_H

