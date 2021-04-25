#include "DoF.h"

void DoF::Init(unsigned width, unsigned height)
{
    //setup buffers
    size_t index = _buffers.size();
    if (index == 0) {
        //CoC
        _buffers.push_back(new Framebuffer());
        _buffers[index]->AddColorTarget(GL_RGBA8);
        _buffers[index]->AddDepthTarget();
        _buffers[index]->Init(width, height);

        //Horz blur pass
        index = _buffers.size();
        _buffers.push_back(new Framebuffer());
        _buffers[index]->AddColorTarget(GL_RGBA8);
        _buffers[index]->Init(width / _scalar, height / _scalar);
            
        //vert blur pass
        index = _buffers.size();
        _buffers.push_back(new Framebuffer());
        _buffers[index]->AddColorTarget(GL_RGBA8);
        _buffers[index]->Init(width / _scalar, height / _scalar);
    }
         
    //Set up shaders
    index = _shaders.size();
    if (index == 0) {
        //CoC
        _shaders.push_back(Shader::Create());
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
        _shaders[index]->LoadShaderPartFromFile("shaders/Post/DoF_CoC_frag.glsl", GL_FRAGMENT_SHADER);
        _shaders[index]->Link();

        //horz blur
        index = _shaders.size();
        _shaders.push_back(Shader::Create());
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
        _shaders[index]->LoadShaderPartFromFile("shaders/Post/DoF_horizontal_blur_frag.glsl", GL_FRAGMENT_SHADER);
        _shaders[index]->Link();

        //vert blur
        index = _shaders.size();
        _shaders.push_back(Shader::Create());
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
        _shaders[index]->LoadShaderPartFromFile("shaders/Post/DoF_vertical_blur_frag.glsl", GL_FRAGMENT_SHADER);
        _shaders[index]->Link();
         
        //composite
        index = _shaders.size();
        _shaders.push_back(Shader::Create());
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
        _shaders[index]->LoadShaderPartFromFile("shaders/Post/DoF_combine_pass_frag.glsl", GL_FRAGMENT_SHADER);
        _shaders[index]->Link();

        //pass
        index = _shaders.size();
        _shaders.push_back(Shader::Create());
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
        _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
        _shaders[index]->Link();
    }

    _shaders[HorzBlurIndex]->SetUniform("u_TexelSize", 1.f / width);
    _shaders[VertBlurIndex]->SetUniform("u_TexelSize", 1.f / height);
    _shaders[HorzBlurIndex]->SetUniform("u_Spread", _radius);
    _shaders[VertBlurIndex]->SetUniform("u_Spread", _radius);
    _shaders[CoCIndex]->SetUniform("u_focusDist", _focusDist);
    _shaders[CoCIndex]->SetUniform("u_focusRange", _focusRange);
}

void DoF::ApplyEffect(PostEffect* buffer)
{
    BindShader(CoCIndex);
    buffer->BindDepthAsTexture(0, 0);
     
    _buffers[CoCIndex]->RenderToFSQ();

    buffer->UnbindTexture(0);
    UnbindShader();

    if (_showCoC)
        return;

    BindShader(PassThroughIndex);
    buffer->BindColorAsTexture(0, 0, 0);

    _buffers[HorzBlurIndex]->RenderToFSQ();

    buffer->UnbindTexture(0);
    UnbindShader();

    for (size_t i(0); i < _blurCount; ++i) {
        //horz
        BindShader(HorzBlurIndex);
        _buffers[HorzBlurIndex]->BindColorAsTexture(0, 0);

        _buffers[VertBlurIndex]->RenderToFSQ();

        _buffers[HorzBlurIndex]->UnbindTexture(0);
        UnbindShader();

        //vert
        BindShader(VertBlurIndex);
        _buffers[VertBlurIndex]->BindColorAsTexture(0, 0);

        _buffers[HorzBlurIndex]->RenderToFSQ();

        _buffers[VertBlurIndex]->UnbindTexture(0);
        UnbindShader();
    }

    BindShader(CompositeIndex);
    buffer->BindColorAsTexture(0, 0, 0);
    _buffers[HorzBlurIndex]->BindColorAsTexture(0, 1);
    _buffers[CoCIndex]->BindColorAsTexture(0, 2);

    _buffers[CoCIndex]->RenderToFSQ();

    _buffers[CoCIndex]->UnbindTexture(0);
    _buffers[HorzBlurIndex]->UnbindTexture(0);
    buffer->UnbindTexture(0);
    UnbindShader();
}

void DoF::Reshape(unsigned width, unsigned height)
{
    _buffers[CoCIndex]->Reshape(width, height);
    _buffers[HorzBlurIndex]->Reshape(width / _scalar, height / _scalar);
    _buffers[VertBlurIndex]->Reshape(width / _scalar, height / _scalar);

    _shaders[HorzBlurIndex]->SetUniform("u_TexelSize", 1.f / width);
    _shaders[VertBlurIndex]->SetUniform("u_TexelSize", 1.f / height);
}

float DoF::GetDist() const
{
    return _focusDist;
}

float DoF::GetRange() const
{
    return _focusRange;
}

float DoF::GetRadius() const
{
    return _radius;
}

size_t DoF::GetBlurCount() const
{
    return _blurCount;
}

bool DoF::GetShowCoC() const
{
    return _showCoC;
}

float DoF::GetScalar() const
{
    return _scalar;
}

void DoF::SetDist(float dist)
{
    _focusDist = dist;
    _shaders[CoCIndex]->SetUniform("u_focusDist", _focusDist);
}

void DoF::SetRange(float range)
{
    _focusRange = range;
    _shaders[CoCIndex]->SetUniform("u_focusRange", _focusRange);
}

void DoF::SetRadius(float radius)
{
    _radius = radius;
    _shaders[HorzBlurIndex]->SetUniform("u_Spread", _radius);
    _shaders[VertBlurIndex]->SetUniform("u_Spread", _radius);
}

void DoF::SetBlurCount(size_t amt)
{
    _blurCount = amt;
}

void DoF::SetShowCoC(bool show)
{
    _showCoC = show;
}

void DoF::SetScalar(float scalar)
{
    _scalar = scalar;
    Reshape(_buffers[CoCIndex]->_width, _buffers[CoCIndex]->_height);
}
