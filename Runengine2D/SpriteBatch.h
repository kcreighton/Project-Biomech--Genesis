#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLM/glm.hpp>

#include "Vertex.h"

namespace Runengine2D
{
	enum class GlyphSortType {NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE};

	struct  Glyph
	{
		GLuint texture;
		float depth;

		Vertex topLeft, bottomLeft, topRight, bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture)
		{

		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType _sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect,const glm::vec4& UVRect, GLuint texture, float depth, const Color& color);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vertexBufferObject;
		GLuint _vertexArrayObject;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphList;
		std::vector<RenderBatch> _renderBatches;
	};

}