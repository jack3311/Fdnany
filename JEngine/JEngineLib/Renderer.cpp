#include "Renderer.h"

namespace JEngine
{
	void VertexFormatStandard::setupVertexAttributes()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatStandard), (GLvoid *)offsetof(VertexFormatStandard, VertexFormatStandard::position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormatStandard), (GLvoid *)offsetof(VertexFormatStandard, VertexFormatStandard::normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormatStandard), (GLvoid *)offsetof(VertexFormatStandard, VertexFormatStandard::texCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

}
