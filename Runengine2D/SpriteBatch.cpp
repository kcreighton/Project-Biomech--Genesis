#include "SpriteBatch.h"
#include <algorithm>

namespace Runengine2D
{

	SpriteBatch::SpriteBatch() :
		_vertexBufferObject(0),
		_vertexArrayObject(0),
		_sortType(),
		_glyphList(),
		_renderBatches()
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* = GlyphSortType::TEXTURE*/)
	{
		_sortType = sortType;
		_renderBatches.clear();
		// Have to delete any glyphs that remain so we don't have memory leaks!
		for (int i = 0; i < _glyphList.size(); i++) {
			delete _glyphList[i];
		}
		_glyphList.clear();
	}

	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphList.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch()
	{
		// Bind our VAO. This sets up the opengl state we need, including the 
		// vertex attribute pointers and it binds the VBO
		glBindVertexArray(_vertexArrayObject);

		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		// This will store all the vertices that we need to upload
		std::vector <Vertex> vertices;
		// Resize the buffer to the exact size we need so we can treat it like an array
		vertices.resize(_glyphList.size() * 6);

		if (_glyphList.empty()) {
			return;
		}

		int offset = 0; // current offset
		int cv = 0; // current vertex

		//Add the first batch
		_renderBatches.emplace_back(offset, 6, _glyphList[0]->texture);
		vertices[cv++] = _glyphList[0]->topLeft;
		vertices[cv++] = _glyphList[0]->bottomLeft;
		vertices[cv++] = _glyphList[0]->bottomRight;
		vertices[cv++] = _glyphList[0]->bottomRight;
		vertices[cv++] = _glyphList[0]->topRight;
		vertices[cv++] = _glyphList[0]->topLeft;
		offset += 6;

		//Add all the rest of the glyphs
		for (int cg = 1; cg < _glyphList.size(); cg++) {

			// Check if this glyph can be part of the current batch
			if (_glyphList[cg]->texture != _glyphList[cg - 1]->texture) {
				// Make a new batch
				_renderBatches.emplace_back(offset, 6, _glyphList[cg]->texture);
			}
			else {
				// If its part of the current batch, just increase numVertices
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphList[cg]->topLeft;
			vertices[cv++] = _glyphList[cg]->bottomLeft;
			vertices[cv++] = _glyphList[cg]->bottomRight;
			vertices[cv++] = _glyphList[cg]->bottomRight;
			vertices[cv++] = _glyphList[cg]->topRight;
			vertices[cv++] = _glyphList[cg]->topLeft;
			offset += 6;
		}

		// Bind our VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
		// Orphan the buffer (for speed)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		// Generate the Vertex Array Object if it isn't already generated
		if (_vertexArrayObject == 0) { glGenVertexArrays(1, &_vertexArrayObject); }

		// Bind the Vertex Array Object. All subsequent opengl calls will modify it's state.
		glBindVertexArray(_vertexArrayObject);

		//G enerate the Vertex Array Object if it isn't already generated
		if (_vertexBufferObject == 0) { glGenBuffers(1, &_vertexBufferObject); }
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);

		//Tell opengl what attribute arrays we need
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the position attribute pointer passed into the shader from a Vertex Class
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//This is the color attribute pointer passed into the shader from a Vertex Class
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer passed into the shader from a Vertex Class
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphList.begin(), _glyphList.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphList.begin(), _glyphList.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphList.begin(), _glyphList.end(), compareTexture);
				break;
		}
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}