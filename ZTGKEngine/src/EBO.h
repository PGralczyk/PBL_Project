#pragma once

class EBO {
private:
	unsigned int id;
	unsigned int count;
public:
	EBO(const unsigned int* data, unsigned int count);
	~EBO();
	void Bind() const;
	void Unbind() const;

};
