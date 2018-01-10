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
	private:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;

		bool enableCullFace;

		GLenum drawMode;

		std::vector<VertexFormat> vertices;
		std::vector<GLuint> indices;

	public:
		Renderer(const std::vector<VertexFormat> & _vertices, GLenum _drawMode = DEFAULT_DRAW_MODE, bool _enableCullFace = DEFAULT_CULL_FACE);
		Renderer(const std::vector<VertexFormat> & _vertices, const std::vector<GLuint> & _indices, GLenum _drawMode = DEFAULT_DRAW_MODE, bool _enableCullFace = DEFAULT_CULL_FACE);
		~Renderer();

		bool initialise();

		void draw(const mat4 & _model) const;
	};

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> & _vertices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(!enableIndices, "Cannot instantiate index-enabled renderer without indices array");
		
		assert(Engine::getEngine().isCurrentThreadMain());
	}

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> & _vertices, const std::vector<GLuint> & _indices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), indices(_indices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(enableIndices, "Cannot instantiate index-disabled renderer with indices array");
	
		assert(Engine::getEngine().isCurrentThreadMain());
	}

	template<typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::~Renderer()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		if (enableIndices)
		{
			glDeleteBuffers(1, &EBO);
		}
	}

	template<typename VertexFormat, bool enableIndices>
	inline bool Renderer<VertexFormat, enableIndices>::initialise()
	{
		assert(Engine::getEngine().isCurrentThreadMain());

		//Setup VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Setup VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices.front(), GL_STATIC_DRAW);

		//Setup EBO
		if (enableIndices)
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
		}

		//Setup Vertex Attributes
		VertexFormat::setupVertexAttributes();

		return true;
	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::draw(const mat4 & _model) const
	{
		assert(Engine::getEngine().isCurrentThreadMain());

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