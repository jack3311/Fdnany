#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

#include "Maths.h"
#include "RAIIGL.h"
#include "Util.h"

namespace JEngine
{
#define DEFAULT_DRAW_MODE GL_TRIANGLES
#define DEFAULT_CULL_FACE true

	template <typename VertexFormat, bool enableIndices>
	class Renderer
	{
	protected:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;

		bool enableCullFace;

		GLenum drawMode;

		std::vector<VertexFormat> vertices;
		std::vector<GLuint> indices;

		unsigned int maxVertices,
			maxIndices;

		bool initialiseBase(const std::vector<VertexFormat> & _vertices, unsigned int _maxVertices);
			
	public:
		Renderer(GLenum _drawMode = DEFAULT_DRAW_MODE, bool _enableCullFace = DEFAULT_CULL_FACE);
		~Renderer();

		bool initialise(const std::vector<VertexFormat> & _vertices, const std::vector<GLuint> & _indices, unsigned int _maxVertices = -1, unsigned int _maxIndices = -1); //-1 gives size of arrays
		bool initialise(const std::vector<VertexFormat> & _vertices, unsigned int _maxVertices = -1); //-1 gives size of arrays

		void flushBufferUpdates();

		void draw() const;
		void draw(const BufferRange & _range) const;

		std::vector<VertexFormat> & getVertices();
		std::vector<GLuint> & getIndices();
	};

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(GLenum _drawMode, bool _enableCullFace) :
		drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
	}

	template<typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::~Renderer()
	{
		assert(Engine::get().isCurrentThreadMain());

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		if (enableIndices)
		{
			glDeleteBuffers(1, &EBO);
		}
	}

	template<typename VertexFormat, bool enableIndices>
	inline bool Renderer<VertexFormat, enableIndices>::initialise(const std::vector<VertexFormat> & _vertices, const std::vector<GLuint> & _indices, unsigned int _maxVertices, unsigned int _maxIndices)
	{
		static_assert(enableIndices, "Cannot initialise index-disabled renderer with indices array");

		assert(Engine::get().isCurrentThreadMain());

		//Configure normally
		if (!initialiseBase(_vertices, _maxVertices))
		{
			return false;
		}

		//Configure index-related
		indices = _indices;

		//Setup EBO
		maxIndices = _maxIndices == -1 ? static_cast<unsigned int>(indices.size()) : _maxIndices;

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

		return true;
	}

	template<typename VertexFormat, bool enableIndices>
	inline bool Renderer<VertexFormat, enableIndices>::initialise(const std::vector<VertexFormat> & _vertices, unsigned int _maxVertices)
	{
		static_assert(!enableIndices, "Cannot initialise index-enabled renderer without indices array");

		return initialiseBase(_vertices, _maxVertices);
	}

	template<typename VertexFormat, bool enableIndices>
	inline bool Renderer<VertexFormat, enableIndices>::initialiseBase(const std::vector<VertexFormat> & _vertices, unsigned int _maxVertices)
	{
		assert(Engine::get().isCurrentThreadMain());

		vertices = _vertices;

		//Setup VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Setup VBO
		maxVertices = _maxVertices == -1 ? static_cast<unsigned int>(vertices.size()) : _maxVertices;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if (vertices.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);
		}

		//Setup Vertex Attributes
		VertexFormat::setupVertexAttributes();

		return true;
	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::flushBufferUpdates()
	{
		assert(Engine::get().isCurrentThreadMain());

		assert(vertices.size() <= maxVertices); //TODO: Dynamically change max vertices if size is too big (reallocate with glBufferData) CLEAR FIRST

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexFormat), &vertices[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		if (enableIndices)
		{
			assert(indices.size() <= maxIndices); //TODO: Dynamically change max indices if size is too big (reallocate with glBufferData) CLEAR FIRST
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), &indices[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
		}

	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::draw() const
	{
		draw(BufferRange{ 0, static_cast<unsigned int>(vertices.size()) });
	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::draw(const BufferRange & _range) const
	{
		//TODO: Split to begin/draw/end

		assert(Engine::get().isCurrentThreadMain());

		if (enableCullFace)
		{
			RAIIGL::_EnableCullFace::begin();
		}


		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if (enableIndices)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(drawMode, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, (void*)(0));
		}
		else
		{
			auto len = static_cast<GLsizei>(_range.getLength()) + 1u;
			glDrawArrays(drawMode, _range.start, len);
		}


		if (enableCullFace)
		{
			RAIIGL::_EnableCullFace::end();
		}
	}

	template<typename VertexFormat, bool enableIndices>
	inline std::vector<VertexFormat> & Renderer<VertexFormat, enableIndices>::getVertices()
	{
		return vertices;
	}

	template<typename VertexFormat, bool enableIndices>
	inline std::vector<GLuint> & Renderer<VertexFormat, enableIndices>::getIndices()
	{
		return indices;
	}
}