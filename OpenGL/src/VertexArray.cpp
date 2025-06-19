#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererId));
	GLCall(glBindVertexArray(m_RendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays)
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	unsigned int size = elements.size();

	for (unsigned int i = 0; i < size; i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * size * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}


// p p
// c c
// t t