#include "os/shaderimpl.h"

#include "../renderer.h"
#include "../opengl.h"
#include "../slurp.h"

#include "../log/log.h"

#include "../os/extensionfunctions.h"

// ======================================== PUBLIC METHODS  ========================================

sad::os::ShaderImpl::ShaderImpl() : m_is_on_gpu(false), m_program(0), m_renderer(NULL)
{
    
}

sad::os::ShaderImpl::ShaderImpl(const sad::os::ShaderImpl& o) 
: m_is_on_gpu(false), 
m_program(0), 
m_vertex_program(o.m_vertex_program), 
m_fragment_program(o.m_fragment_program), 
m_renderer(o.m_renderer)
{
    
}


sad::os::ShaderImpl& sad::os::ShaderImpl::operator=(const sad::os::ShaderImpl& o)
{
    tryDestroy();
    m_is_on_gpu = false;
    m_program = 0;
    m_vertex_program = o.m_vertex_program;
    m_fragment_program = o.m_fragment_program;
    m_renderer = o.m_renderer;
    return *this;
}

sad::os::ShaderImpl::~ShaderImpl()
{
    tryDestroy();
}

void sad::os::ShaderImpl::setRenderer(sad::Renderer* r)
{
    m_renderer = r;
}

sad::Renderer* sad::os::ShaderImpl::renderer() const
{
    return m_renderer;
}

void sad::os::ShaderImpl::setVertexProgram(const sad::String& vertexProgram)
{
    m_vertex_program = vertexProgram;
}

void sad::os::ShaderImpl::setFragmentProgram(const sad::String& fragmentProgram)
{
    m_fragment_program = fragmentProgram;
}

bool sad::os::ShaderImpl::loadVertexProgramFromFile(const sad::String& fileName)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }
    sad::Maybe<sad::String> maybe_string = sad::slurp(fileName, r);
    if (maybe_string.exists())
    {
        m_vertex_program = maybe_string.value();
        return true;
    }
    return false;
}

bool sad::os::ShaderImpl::loadFragmentProgramFromFile(const sad::String& fileName)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }
    sad::Maybe<sad::String> maybe_string = sad::slurp(fileName, r);
    if (maybe_string.exists())
    {
        m_fragment_program = maybe_string.value();
        return true;
    }
    return false;
}

void sad::os::ShaderImpl::tryUpload()
{
    if (!m_is_on_gpu)
    {
        sad::Renderer* r = sad::Renderer::ref();
        if (m_renderer)
        {
            r = m_renderer;
        }

        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();

        GLint success = 0;
        const int info_log_length = 1024;
        GLchar info_log[info_log_length];

        GLuint vertex_program = this->tryCompileShader(GL_VERTEX_SHADER, m_vertex_program);
        if (vertex_program)
        {
            GLuint fragment_program = this->tryCompileShader(GL_FRAGMENT_SHADER, m_fragment_program);
            if (fragment_program)
            {
                m_program = f->glCreateProgram();
                f->glAttachShader(m_program, vertex_program);
                f->glAttachShader(m_program, fragment_program);
                f->glLinkProgram(m_program);
                f->glGetProgramiv(m_program, GL_LINK_STATUS, &success);
                if (!success)
                {
                    f->glGetProgramInfoLog(m_program, info_log_length, NULL, info_log);
                    f->glDeleteProgram(m_program);
                    sad::String message = "Unable to link program with error, listed below:\n";
                    message += info_log;
                    SL_LOCAL_WARNING(message, *r);

                    m_program = 0;
                }
                else
                {
                    m_is_on_gpu = true;
                }

                f->glDeleteShader(vertex_program);
                f->glDeleteShader(fragment_program);
            }
        }
    }
}


void sad::os::ShaderImpl::use()
{
    useProgram(m_program, false);
}

void sad::os::ShaderImpl::disable()
{
    useProgram(0, true);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, int v0)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform1i, loc_name, v0);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, unsigned int v0)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform1ui, loc_name, v0);
}


void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, float v0)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform1f, loc_name, v0);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, int v0, int v1)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform2i, loc_name, v0, v1);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform2ui, loc_name, v0, v1);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, float v0, float v1)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform2f, loc_name, v0, v1);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform3i, loc_name, v0, v1, v2);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform3ui, loc_name, v0, v1, v2);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform3f, loc_name, v0, v1, v2);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2, int v3)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform4i, loc_name, v0, v1, v2, v3);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform4ui, loc_name, v0, v1, v2, v3);
}

void sad::os::ShaderImpl::setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2, float v3)
{
    this->invoke(&sad::os::ExtensionFunctions::glUniform4f, loc_name, v0, v1, v2, v3);
}

void sad::os::ShaderImpl::tryDestroy()
{
    if (m_is_on_gpu)
    {
        if (m_renderer)
        {
            sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
            f->glDeleteProgram(m_program);
        }
        m_is_on_gpu = false;
        m_program = 0;
    }
}

// ======================================== PRIVATE METHODS  ========================================

GLuint  sad::os::ShaderImpl::tryCompileShader(GLenum shader_type, const sad::String& program_text) const
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    sad::os::ExtensionFunctions* f = r->opengl()->extensionFunctions();

    GLint success = 0;
    const int info_log_length = 1024;
    GLchar info_log[info_log_length];

    GLuint  program = f->glCreateShader(shader_type);
    const GLchar* source = program_text.c_str();
    f->glShaderSource(program, 1, &source, NULL);
    f->glCompileShader(program);
    f->glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(program, info_log_length, NULL, info_log);
        sad::String message = "Unable to compile ";   
        if (shader_type == GL_VERTEX_SHADER)
        {
            message += "vertex";
        }
        else
        {
            message += "fragment";
        }
        message += "shader with error, listed below\n";
        message += info_log;
        message += "\nCompiled shader was:\n";
        message += program_text;
        SL_LOCAL_WARNING(message, *r);
        f->glDeleteShader(program);
        return 0;
    }
    return program;
}

void sad::os::ShaderImpl::useProgram(GLuint program, bool force)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    try {
        this->tryUpload();
        if ((program != 0) || force)
        {
            sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
            f->glUseProgram(program);
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

sad::os::ExtensionFunctions* sad::os::ShaderImpl::f() const
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    return r->opengl()->extensionFunctions();
}

template<typename T, typename A>
void sad::os::ShaderImpl::invoke(T a, const sad::String& location_name, A v0)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    try {
        this->tryUpload();
        sad::os::ExtensionFunctions* f = this->f();
        GLint location = f->glGetUniformLocation(m_program, location_name.c_str());
        if (location)
        {
            (f->*a)(location, v0);
        }
        else
        {
            sad::String message = sad::String("Uniform variable with name\"") + location_name + "\" is not found in shader program";
            SL_LOCAL_WARNING(message, *r);
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

template<typename T, typename A>
void sad::os::ShaderImpl::invoke(T a, const sad::String& location_name, A v0, A v1)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    try {
        this->tryUpload();
        sad::os::ExtensionFunctions* f = this->f();
        GLint location = f->glGetUniformLocation(m_program, location_name.c_str());
        if (location)
        {
            (f->*a)(location, v0, v1);
        }
        else
        {
            sad::String message = sad::String("Uniform variable with name\"") + location_name + "\" is not found in shader program";
            SL_LOCAL_WARNING(message, *r);
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

template<typename T, typename A>
void sad::os::ShaderImpl::invoke(T a, const sad::String& location_name, A v0, A v1, A v2)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    try {
        this->tryUpload();
        sad::os::ExtensionFunctions* f = this->f();
        GLint location = f->glGetUniformLocation(m_program, location_name.c_str());
        if (location)
        {
            (f->*a)(location, v0, v1, v2);
        }
        else
        {
            sad::String message = sad::String("Uniform variable with name\"") + location_name + "\" is not found in shader program";
            SL_LOCAL_WARNING(message, *r);
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

template<typename T, typename A>
void sad::os::ShaderImpl::invoke(T a, const sad::String& location_name, A v0, A v1, A v2, A v3)
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    try {
        this->tryUpload();
        sad::os::ExtensionFunctions* f = this->f();
        GLint location = f->glGetUniformLocation(m_program, location_name.c_str());
        if (location)
        {
            (f->*a)(location, v0, v1, v2, v3);
        }
        else
        {
            sad::String message = sad::String("Uniform variable with name\"") + location_name + "\" is not found in shader program";
            SL_LOCAL_WARNING(message, *r);
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}
