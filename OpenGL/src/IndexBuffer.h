#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererId; // same as buffer Id, making more generic
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};