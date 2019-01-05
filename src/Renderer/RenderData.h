#pragma once

#include <glad.h>
#include "../Maths/GlmCommon.h"

class Model;
class RenderData
{
        friend class Model;
    public:
        void bind() const
        {
            glBindVertexArray(vao);
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
        }

        GLuint getIndicesCount() const
        {
            return indicesCount;
        }

		void setMode(GLenum m)
		{
			this->m_mode = m;
		}

		GLenum getMode() const
		{
			return m_mode;
		}

		glm::mat4 getModelMatrix() const
		{
			return m_modelMatrix;
		}

		
    private:
        GLuint vao = 0;
		GLuint textureID = 0;
        GLuint indicesCount = 0;
		glm::mat4 m_modelMatrix;

		GLenum m_mode = GL_TRIANGLES;
};