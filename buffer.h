#pragma once

template <typename T>
class OLBuffer
{
private:
	unsigned int width;
	unsigned int height;
	unsigned int length;
	T* address;
public:
	OLBuffer(unsigned int alloc_width, unsigned int alloc_height)
	{
		width = alloc_width;
		height = alloc_height;
		length = width * height;

		address = new T[length];
	}

	OLBuffer(T* existing_buffer, unsigned int buf_width, unsigned int buf_height)
	{
		width = buf_width;
		height = buf_height;
		length = width * height;

		address = existing_buffer;
	}

	OLBuffer(const OLBuffer&) = delete;
	OLBuffer(const OLBuffer&&) = delete;
	void operator=(const OLBuffer&) = delete;
	void operator=(const OLBuffer&&) = delete;

	inline T& access(unsigned int x, unsigned int y)
	{
		if (x >= width || y >= height) return address[0];
		return address[(y * width) + x];
	}

	inline T& access(unsigned int i)
	{
		if (i >= length) return address[0];
		return address[i];
	}

	inline T& unsafeAccess(unsigned int x, unsigned int y)
	{
		return address[(y * width) + x];
	}

	inline T& unsafeAccess(unsigned int i)
	{
		return address[i];
	}

	inline void fill(T value)
	{
		for (unsigned int i = 0; i < length; i++)
			address[i] = value;
	}

	inline unsigned int getWidth() { return width; }
	inline unsigned int getHeight() { return height; }
	inline unsigned int getLength() { return length; }

	~OLBuffer()
	{
		delete[] address;
	}
};

enum OLDepthWrite
{
	NEVER,
	LESS,
	LESSEQUAL,
	EQUAL,
	GREATEREQUAL,
	GREATER,
	ALWAYS,
};