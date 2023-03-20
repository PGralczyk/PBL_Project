#pragma once

class VBO
{
private:
	unsigned int id;
public:
	VBO(const void* data, unsigned int size);
	~VBO();
	void Bind() const;
	void Unbind() const;

};
