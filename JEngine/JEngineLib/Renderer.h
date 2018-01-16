#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

#include "Maths.h"
#include "RAIIGL.h"

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

		int maxVertices,
			maxIndices;
			
	public:
		Renderer(const std::vector<VertexFormat> &> _vertices, GLenum _drawMode = DEFAULT_DRAW_MODE, bool _enableCullFace = DEFAULT_CULL_FACE);
		Renderer(const std::vector<VertexFormat> &> _vertices, const std::vector<GLuint> & _indices, GLenum _drawMode = DEFAULT_DRAW_MODE, bool _enableCullFace = DEFAULT_CULL_FACE);
		~Renderer();

		bool initialise(int _maxVertices = -1, int _maxIndices = -1); //-1 gives size of arrays

		void updateData();

		void draw(const mat4 & _model) const;
	};

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> &> _vertices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(!enableIndices, "Cannot instantiate index-enabled renderer without indices array");
	}

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> &> _vertices, const std::vector<GLuint> & _indices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), indices(_indices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(enableIndices, "Cannot instantiate index-disabled renderer with indices array");		
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
	inline bool Renderer<VertexFormat, enableIndices>::initialise(int _maxVertices, int _maxIndices)
	{
		assert(Engine::get().isCurrentThreadMain());

		//Setup VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Setup VBO
		maxVertices = _maxVertices == -1 ? vertices.size() : _maxVertices;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(VertexFormat), &vertices.front(), GL_STATIC_DRAW);

		//Setup EBO
		if (enableIndices)
		{
			maxIndices = _maxIndices == -1 ? indices.size() : _maxIndices;

			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
		}

		//Setup Vertex Attributes
		VertexFormat::setupVertexAttributes();

		return true;
	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::updateData()
	{
		assert(vertices.size() <= maxVertices); //TODO: Dynamically change max vertices if size is too big (reallocate with glBufferData) CLEAR FIRST

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), &vertices[0]);

		if (enableIndices)
		{
			assert(indices.size() <= maxIndices); //TODO: Dynamically change max indices if size is too big (reallocate with glBufferData) CLEAR FIRST

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size(); &indices[0]);
		}

	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::draw(const mat4 & _model) const
	{
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
			glDrawArrays(drawMode, 0, static_cast<GLsizei>(vertices.size()));
		}


		if (enableCullFace)
		{
			RAIIGL::_EnableCullFace::end();
		}
	}
}