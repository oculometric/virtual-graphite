#include "buffer.h"

template <typename T>
OLBuffer<T>::OLBuffer(unsigned int alloc_width, unsigned int alloc_height)
{
	width = alloc_width;
	height = alloc_height;
	length = width * height;

	address = new T[length];
}

template <typename T>
OLBuffer<T>::OLBuffer(T* existing_buffer, unsigned int buf_width, unsigned int buf_height)
{
	width = buf_width;
	height = buf_height;
	length = width * height;

	address = existing_buffer;
}

template <typename T>
inline T& OLBuffer<T>::access(unsigned int x, unsigned int y)
{
	if (x >= width || y >= height) return address[0];
	return address[(y * width) + x];
}

template <typename T>
inline T& OLBuffer<T>::access(unsigned int i)
{
	if (i >= length) return address[0];
	return address[i];
}

template <typename T>
inline T& OLBuffer<T>::unsafeAccess(unsigned int x, unsigned int y)
{
	return address[(y * width) + x];
}

template <typename T>
inline T& OLBuffer<T>::unsafeAccess(unsigned int i)
{
	return address[i];
}

template <typename T>
inline unsigned int OLBuffer<T>::getWidth() { return width; }

template <typename T>
inline unsigned int OLBuffer<T>::getHeight() { return height; }

template <typename T>
inline unsigned int OLBuffer<T>::getLength() { return length; }

template <typename T>
OLBuffer<T>::~OLBuffer()
{
	delete[] address;
}