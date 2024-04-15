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
	OLBuffer(unsigned int alloc_width, unsigned int alloc_height);
	OLBuffer(T* existing_buffer, unsigned int buf_width, unsigned int buf_height);

	OLBuffer(const OLBuffer&) = delete;
	OLBuffer(const OLBuffer&&) = delete;
	void operator=(const OLBuffer&) = delete;
	void operator=(const OLBuffer&&) = delete;

	inline T& access(unsigned int x, unsigned int y);
	inline T& access(unsigned int i);
	inline T& unsafeAccess(unsigned int x, unsigned int y);
	inline T& unsafeAccess(unsigned int i);

	inline unsigned int getWidth();
	inline unsigned int getHeight();
	inline unsigned int getLength();

	~OLBuffer();
};