#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

#include "Maths.h"

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

		void draw(const fmat4x4 & _model) const;
	};

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> & _vertices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(!enableIndices, "Cannot instantiate index-enabled renderer without indices array");
	}

	template <typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::Renderer(const std::vector<VertexFormat> & _vertices, const std::vector<GLuint> & _indices, GLenum _drawMode, bool _enableCullFace) :
		vertices(_vertices), indices(_indices), drawMode(_drawMode), enableCullFace(_enableCullFace)
	{
		static_assert(enableIndices, "Cannot instantiate index-disabled renderer with indices array");
	}

	template<typename VertexFormat, bool enableIndices>
	inline Renderer<VertexFormat, enableIndices>::~Renderer()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	template<typename VertexFormat, bool enableIndices>
	inline bool Renderer<VertexFormat, enableIndices>::initialise()
	{
		//Setup VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Setup VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices.front(), GL_STATIC_DRAW);

		//Setup EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

		//Setup Vertex Attributes
		VertexFormat::setupVertexAttributes();

		return true;
	}

	template<typename VertexFormat, bool enableIndices>
	inline void Renderer<VertexFormat, enableIndices>::draw(const fmat4x4 & _model) const
	{
		if (enableCullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}


		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if (enableIndices)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, (void*)(0));
		}
		else
		{
			glDrawArrays(drawMode, 0, vertices.size());
		}


		if (enableCullFace)
		{
			glDisable(GL_CULL_FACE);
		}
	}
}