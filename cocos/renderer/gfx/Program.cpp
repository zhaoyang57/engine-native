/****************************************************************************
 LICENSING AGREEMENT
 
 Xiamen Yaji Software Co., Ltd., (the “Licensor”) grants the user (the “Licensee”) non-exclusive and non-transferable rights to use the software according to the following conditions:
 a.  The Licensee shall pay royalties to the Licensor, and the amount of those royalties and the payment method are subject to separate negotiations between the parties.
 b.  The software is licensed for use rather than sold, and the Licensor reserves all rights over the software that are not expressly granted (whether by implication, reservation or prohibition).
 c.  The open source codes contained in the software are subject to the MIT Open Source Licensing Agreement (see the attached for the details);
 d.  The Licensee acknowledges and consents to the possibility that errors may occur during the operation of the software for one or more technical reasons, and the Licensee shall take precautions and prepare remedies for such events. In such circumstance, the Licensor shall provide software patches or updates according to the agreement between the two parties. The Licensor will not assume any liability beyond the explicit wording of this Licensing Agreement.
 e.  Where the Licensor must assume liability for the software according to relevant laws, the Licensor’s entire liability is limited to the annual royalty payable by the Licensee.
 f.  The Licensor owns the portions listed in the root directory and subdirectory (if any) in the software and enjoys the intellectual property rights over those portions. As for the portions owned by the Licensor, the Licensee shall not:
 - i. Bypass or avoid any relevant technical protection measures in the products or services;
 - ii. Release the source codes to any other parties;
 - iii. Disassemble, decompile, decipher, attack, emulate, exploit or reverse-engineer these portion of code;
 - iv. Apply it to any third-party products or services without Licensor’s permission;
 - v. Publish, copy, rent, lease, sell, export, import, distribute or lend any products containing these portions of code;
 - vi. Allow others to use any services relevant to the technology of these codes;
 - vii. Conduct any other act beyond the scope of this Licensing Agreement.
 g.  This Licensing Agreement terminates immediately if the Licensee breaches this Agreement. The Licensor may claim compensation from the Licensee where the Licensee’s breach causes any damage to the Licensor.
 h.  The laws of the People's Republic of China apply to this Licensing Agreement.
 i.  This Agreement is made in both Chinese and English, and the Chinese version shall prevail the event of conflict.
 ****************************************************************************/

#include "Program.h"
#include "GFXUtils.h"

#include <unordered_map>
#include <stdlib.h>
#include <string.h>

namespace {

    uint32_t _genID = 0;

    std::string logForOpenGLShader(GLuint shader)
    {
        GLint logLength = 0;

        GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength < 1)
            return "";

        char *logBytes = (char*)malloc(sizeof(char) * logLength);
        GL_CHECK(glGetShaderInfoLog(shader, logLength, nullptr, logBytes));
        std::string ret(logBytes);

        free(logBytes);
        return ret;
    }

    std::string logForOpenGLProgram(GLuint program)
    {
        GLint logLength = 0;

        GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength < 1)
            return "";

        char *logBytes = (char*)malloc(sizeof(char) * logLength);
        GL_CHECK(glGetProgramInfoLog(program, logLength, nullptr, logBytes));
        std::string ret(logBytes);

        free(logBytes);
        return ret;
    }

    bool _createShader(GLenum type, const std::string& src, GLuint* outShader)
    {
        assert(outShader != nullptr);
        GLuint shader = glCreateShader(type);
        const GLchar* sources[] = { src.c_str() };
        GL_CHECK(glShaderSource(shader, 1, sources, nullptr));
        GL_CHECK(glCompileShader(shader));

        GLint status;
        GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

        if (!status)
        {
            GLsizei length;
            GL_CHECK(glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length));
            GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);

            GL_CHECK(glGetShaderSource(shader, length, nullptr, src));
            RENDERER_LOGE("ERROR: Failed to compile shader:\n%s", src);

            std::string shaderLog = logForOpenGLShader(shader);
            RENDERER_LOGE("%s", shaderLog.c_str());

            free(src);

            *outShader = 0;
            return false;
        }

        *outShader = shader;
        return true;
    }

#define DEF_TO_INT(pointer)  (*(int*)(pointer))
#define DEF_TO_FLOAT(pointer)  (*(float*)(pointer))

    void setUniform1i(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        assert(count == 1);
        if (elementType == cocos2d::renderer::UniformElementType::INT)
        {
            glUniform1i(location, DEF_TO_INT(value));
        }
        else
        {
            float floatVal = *((float*)value);
            GLint intVal = (GLint)floatVal;
            glUniform1i(location, intVal);
        }
    }

    void setUniform1iv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform1iv(location, count, (const GLint*)value);
    }

    void setUniform2iv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform2iv(location, count, (const GLint*)value);
    }

    void setUniform3iv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform3iv(location, count, (const GLint*)value);
    }

    void setUniform4iv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform4iv(location, count, (const GLint*)value);
    }

    void setUniform1f(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        assert(count == 1);
        glUniform1f(location, DEF_TO_FLOAT(value));
    }

    void setUniform1fv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform1fv(location, count, (const GLfloat*)value);
    }

    void setUniform2fv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform2fv(location, count, (const GLfloat*)value);
    }

    void setUniform3fv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform3fv(location, count, (const GLfloat*)value);
    }

    void setUniform4fv(GLint location, GLsizei count , const void* value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniform4fv(location, count, (const GLfloat*)value);
    }

    void setUniformMatrix2fv(GLint location, GLsizei count, const void *value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniformMatrix2fv(location, count, GL_FALSE, (const GLfloat*)value);
    }

    void setUniformMatrix3fv(GLint location, GLsizei count, const void *value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniformMatrix3fv(location, count, GL_FALSE, (const GLfloat*)value);
    }

    void setUniformMatrix4fv(GLint location, GLsizei count, const void *value, cocos2d::renderer::UniformElementType elementType)
    {
        glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)value);
    }

    /**
     * _type2uniformCommit
     */
    std::unordered_map<GLenum, cocos2d::renderer::Program::Uniform::SetUniformCallback> _type2uniformCommit = {
        { GL_INT, setUniform1i },
        { GL_FLOAT, setUniform1f },
        { GL_FLOAT_VEC2, setUniform2fv },
        { GL_FLOAT_VEC3, setUniform3fv },
        { GL_FLOAT_VEC4, setUniform4fv },
        { GL_INT_VEC2, setUniform2iv },
        { GL_INT_VEC3, setUniform3iv },
        { GL_INT_VEC4, setUniform4iv },
        { GL_BOOL, setUniform1i },
        { GL_BOOL_VEC2, setUniform2iv },
        { GL_BOOL_VEC3, setUniform3iv },
        { GL_BOOL_VEC4, setUniform4iv },
        { GL_FLOAT_MAT2, setUniformMatrix2fv },
        { GL_FLOAT_MAT3, setUniformMatrix3fv },
        { GL_FLOAT_MAT4, setUniformMatrix4fv },
        { GL_SAMPLER_2D, setUniform1i },
        { GL_SAMPLER_CUBE, setUniform1i }
    };

    /**
     * _type2uniformArrayCommit
     */
    std::unordered_map<GLenum, cocos2d::renderer::Program::Uniform::SetUniformCallback> _type2uniformArrayCommit = {
        { GL_INT, setUniform1iv },
        { GL_FLOAT, setUniform1fv },
        { GL_FLOAT_VEC2, setUniform2fv },
        { GL_FLOAT_VEC3, setUniform3fv },
        { GL_FLOAT_VEC4, setUniform4fv },
        { GL_INT_VEC2, setUniform2iv },
        { GL_INT_VEC3, setUniform3iv },
        { GL_INT_VEC4, setUniform4iv },
        { GL_BOOL, setUniform1iv },
        { GL_BOOL_VEC2, setUniform2iv },
        { GL_BOOL_VEC3, setUniform3iv },
        { GL_BOOL_VEC4, setUniform4iv },
        { GL_FLOAT_MAT2, setUniformMatrix2fv },
        { GL_FLOAT_MAT3, setUniformMatrix3fv },
        { GL_FLOAT_MAT4, setUniformMatrix4fv },
        { GL_SAMPLER_2D, setUniform1iv },
        { GL_SAMPLER_CUBE, setUniform1iv }
    };
} // namespace {

RENDERER_BEGIN

void Program::Uniform::setUniform(const void* value, UniformElementType elementType) const
{
    GLsizei count = size == -1 ? 1 : size;
    _callback(location, count, value, elementType);
}

Program::Program()
: _device(nullptr)
, _id(0)
, _linked(false)
{

}

Program::~Program()
{
    GL_CHECK(glDeleteProgram(_glID));
}

bool Program::init(DeviceGraphics* device, const char* vertSource, const char* fragSource)
{
    assert(device);
    assert(vertSource);
    assert(fragSource);

    _device = device;
    _vertSource = vertSource;
    _fragSource = fragSource;
    _id = _genID++;
    _linked = false;
    return true;
}

void Program::link()
{
    if (_linked) {
        return;
    }

    GLuint vertShader;
    bool ok = _createShader(GL_VERTEX_SHADER, _vertSource, &vertShader);
    if (!ok)
        return;

    GLuint fragShader;
    ok = _createShader(GL_FRAGMENT_SHADER, _fragSource, &fragShader);
    if (!ok)
    {
        glDeleteShader(vertShader);
        return;
    }

    GLuint program = glCreateProgram();
    GL_CHECK(glAttachShader(program, vertShader));
    GL_CHECK(glAttachShader(program, fragShader));
    GL_CHECK(glLinkProgram(program));

    GLint status = GL_TRUE;
    GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &status));

    if (status == GL_FALSE)
    {
        RENDERER_LOGE("ERROR: Failed to link program: %u", program);
        std::string programLog = logForOpenGLProgram(program);
        RENDERER_LOGE("%s", programLog.c_str());
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    _glID = program;

    // parse attribute
    GLint numAttributes;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    if (numAttributes > 0)
    {
        GLint length;
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);

        if (length > 0)
        {
            GLchar* attribName = (GLchar*) malloc(length + 1);
            Attribute attribute;
            for (GLint i = 0; i < numAttributes; ++i) {
                // Query attribute info.
                glGetActiveAttrib(program, i, length, nullptr, &attribute.size, &attribute.type, attribName);
                attribName[length] = '\0';
                attribute.name = attribName;
                // Query the pre-assigned attribute location
                attribute.location = glGetAttribLocation(program, attribName);

                _attributes.push_back(std::move(attribute));
            }
            free(attribName);
        }
    }

    // Query and store uniforms from the program.
    GLint activeUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms > 0)
    {
        GLint length;
        glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
        if (length > 0)
        {
            GLchar* uniformName = (GLchar*) malloc(length + 1);

            Uniform uniform;
            for (int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                GL_CHECK(glGetActiveUniform(program, i, length, nullptr, &uniform.size, &uniform.type, uniformName));
                uniformName[length] = '\0';
                bool isArray = false;
                // remove possible array '[]' from uniform name
                if (length > 3)
                {
                    char* c = strrchr(uniformName, '[');
                    if (c)
                    {
                        *c = '\0';
                        isArray = true;
                    }
                }

                uniform.name = uniformName;
                GL_CHECK(uniform.location = glGetUniformLocation(program, uniformName));

                GLenum err = glGetError();
                if (err != GL_NO_ERROR)
                {
                    RENDERER_LOGE("error: 0x%x  uniformName: %s", (int)err, uniformName);
                }
                assert(err == GL_NO_ERROR);

                if (!isArray)
                {
                    uniform.size = -1;
                    auto iter = _type2uniformCommit.find(uniform.type);
                    assert(iter != _type2uniformCommit.end());
                    uniform._callback = iter->second;
                }
                else
                {
                    auto iter = _type2uniformArrayCommit.find(uniform.type);
                    assert(iter != _type2uniformArrayCommit.end());
                    uniform._callback = iter->second;
                }
                _uniforms.push_back(std::move(uniform));
            }
            
            free(uniformName);
        }
    }

    _linked = true;
}

RENDERER_END
