#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace glr {
namespace {
std::string readCode(const std::string& path) {
    if (path.empty()) return {};
    std::string   code;
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fs.open(path);
        std::stringstream ss;
        ss << fs.rdbuf();
        fs.close();
        code = ss.str();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return code;
}

unsigned int createShader(const GLchar* code, int type) {
    if (!code || !strlen(code)) return 0;
    unsigned int shader_id;
    shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &code, NULL);
    glCompileShader(shader_id);

    int  status;
    char msg[512];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    if (0 == status) {
        glGetShaderInfoLog(shader_id, sizeof(msg), NULL, msg);
        std::cerr << "ERROR: failed to compile the shader" << msg << std::endl;
        throw std::exception("Compile shader failed.");
    }
    return shader_id;
}
} // namespace

VI_OBJECT_META_IMPL(Shader, GLObject);

struct Shader::Data {
    std::string name;
    std::string vs_code;
    std::string gs_code;
    std::string fs_code;
};

Shader::Shader(const std::string& vs_code, const std::string& gs_code, const std::string& fs_code)
  : d(new Data()) {
    d->vs_code = vs_code;
    d->gs_code = gs_code;
    d->fs_code = fs_code;
}

Shader::~Shader() {
}

void Shader::use(State& state) {
    if (!isCreated(state)) GLObject::create(state);
    if (!isCreated(state)) return;
    glUseProgram(getId(state));
}

void Shader::unuse(State& state) {
    auto id = getId(state);
    if (id) {
        GLint current_prog;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_prog);
        if (current_prog == id) glUseProgram(0);
    }
}

std::string Shader::getName() const {
    return d->name;
}

void Shader::setName(const std::string& name) {
    d->name = name;
}

GLuint Shader::onCreate(State& state) {
    unsigned int vs_id = 0, gs_id = 0, fs_id = 0;
    char         msg[512];
    auto         status = 0;
    auto         app_id = glCreateProgram();

    if (!d->vs_code.empty()) {
        vs_id = createShader(d->vs_code.data(), GL_VERTEX_SHADER);
        glAttachShader(app_id, vs_id);
    }
    if (!d->gs_code.empty()) {
        gs_id = createShader(d->gs_code.data(), GL_GEOMETRY_SHADER);
        glAttachShader(app_id, gs_id);
    }
    if (!d->fs_code.empty()) {
        fs_id = createShader(d->fs_code.data(), GL_FRAGMENT_SHADER);
        glAttachShader(app_id, fs_id);
    }
    glLinkProgram(app_id);
    glGetProgramiv(app_id, GL_LINK_STATUS, &status);

    if (0 == status) {
        glGetProgramInfoLog(app_id, sizeof(msg), NULL, msg);
        std::cerr << "ERROR: failed to link the shaders" << msg << std::endl;
        throw std::exception("Link shaders failed.");
    }
    if (vs_id) {
        glDetachShader(app_id, vs_id);
        glDeleteShader(vs_id);
    }
    if (gs_id) {
        glDetachShader(app_id, gs_id);
        glDeleteShader(gs_id);
    }
    if (fs_id) {
        glDetachShader(app_id, fs_id);
        glDeleteShader(fs_id);
    }
    return app_id;
}

void Shader::onRelease(State& state) {
    auto id = getId(state);
    glDeleteProgram(id);
}

Shader* Shader::create(const std::string& vs_path, const std::string& gs_path, const std::string& fs_path) {
    auto vs_code = readCode(vs_path);
    auto gs_code = readCode(gs_path);
    auto fs_code = readCode(fs_path);

    if (vs_code.empty() || fs_code.empty()) return nullptr;

    return new Shader(vs_code, gs_code, fs_code);
}
} // namespace glr