//
// Created by Jack Glass on 2020-09-21.
//

#include <algorithm>
#include <stdexcept>
#include "handle.h"
#include "shader.h"
#include "shader_program.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

using namespace gl;

/*************************************************************************
 * Constructor destructor pairs
 *************************************************************************/
// Shader
template<>
Handle<Shader>::Handle() = delete;

template<>
Handle<Shader>::Handle(GLenum type)
  : m_handleId(glCreateShader(type))
{}

template<>
Handle<Shader>::~Handle()
{
 glDeleteShader(m_handleId);
}

// ShaderProgram
template<>
Handle<ShaderProgram>::Handle()
  : m_handleId(glCreateProgram())
{}

template<>
Handle<ShaderProgram>::Handle(GLenum) = delete;

template<>
Handle<ShaderProgram>::~Handle()
{
  glDeleteProgram(m_handleId);
}

// VertexArray
template<>
Handle<VertexArray>::Handle()
  : m_handleId(0)
{
  glGenVertexArrays(1, &m_handleId);
}

template<>
Handle<VertexArray>::Handle(GLenum) = delete;

template<>
Handle<VertexArray>::~Handle()
{
  glDeleteVertexArrays(1, &m_handleId);
}

// VertexBuffer
template<>
Handle<VertexBuffer>::Handle()
  : m_handleId(0)
{
  glGenBuffers(1, &m_handleId);
}

template<>
Handle<VertexBuffer>::Handle(GLenum) = delete;

template<>
Handle<VertexBuffer>::~Handle()
{
  glDeleteBuffers(1, &m_handleId);
}

// Everything else
template<class glType>
Handle<glType>::Handle() {
  throw std::runtime_error("No handle implemented for this type");
}

template<class glType>
Handle<glType>::~Handle(){
  throw std::runtime_error("No handle implemented for this type");
}

/*************************************************************************
 * Common functions for all
 *************************************************************************/
template<class glType>
Handle<glType>::Handle(Handle&& other) noexcept
  : m_handleId(std::move(other.m_handleId))
{
  other.m_handleId = 0;
}


