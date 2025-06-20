#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // same as buffer Id, making more abstracted for support of mulitple apis
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	
	void Bind() const;
	void Unbind() const;
};