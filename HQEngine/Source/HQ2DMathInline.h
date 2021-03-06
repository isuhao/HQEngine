/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef _2DMATH_INL_
#define _2DMATH_INL_

template <typename T>
inline bool HQRect<T>::IsPointInside(T _x, T _y) const
{
	if(_x < this->x || _x > (this->x + this->width))
		return false;
	if(_y < this->y || _y > (this->y + this->height))
		return false;
	return true;
}

template <typename T>
inline bool HQRect<T>::IsPointInside(const HQPoint<T> &point) const
{
	if(point.x < this->x || point.x > (this->x + this->width))
		return false;
	if(point.y < this->y || point.y > (this->y + this->height))
		return false;
	return true;
}
#endif
