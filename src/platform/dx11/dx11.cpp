#include "dx11.hpp"
#ifdef WINDOWS
#include "pch.hpp"
#include <d3dcompiler.h>
#include "dx11Init.hpp"

using namespace Platform;
using Microsoft::WRL::ComPtr;

#define REMOVE_NEWLINE(buffer, size) std::string( buffer, size - 2 )

#ifdef DEBUG

// Returns true when succesful at getting message
bool GetResultMessage( HRESULT hResult, std::string& message ) {
    LPSTR errorText = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hResult,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPSTR)&errorText,
        0,
        NULL
    );

    if( errorText != NULL ) {
        message = std::string(errorText);
        message = REMOVE_NEWLINE( message.c_str(), message.size() );
        LocalFree( errorText );
        return true;
    } else {
        LOG_ERROR("DirectX 11 > Failed to get hResult message!");
        return false;
    }

}

    // check if result is S_OK
    #define CHECK_RESULT(hResult) hResult == S_OK
    
    // Log result
    #define LOG_RESULT(hResult) std::string __message;\
        if( GetResultMessage( hResult, __message ) ) LOG_ERROR("DirectX 11 > %s", __message.c_str())

    // Log result only if failed
    #define LOG_FAILED(function) auto __result = function;\
        if( !CHECK_RESULT(__result) ) {\
            LOG_RESULT(__result);\
        }\
        static_assert(true, "")

#else
    #define CHECK_RESULT(hResult)
    #define LOG_RESULT(hResult)
    #define LOG_FAILED(function)
#endif

bool RendererAPIDirectX11::Initialize( void* initData ) {
    DirectX11InitData* data = (DirectX11InitData*)initData;
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    #ifdef DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    HRESULT hResult = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &data->sd,
        &m_swapchain,
        &m_device,
        nullptr,
        &m_context
    );
    if( !CHECK_RESULT( hResult ) ) {
        LOG_RESULT( hResult );
        return false;
    }

    ComPtr<ID3D11Resource> backBuffer = nullptr;
    hResult = m_swapchain->GetBuffer( 0, __uuidof( ID3D11Resource ), (void**)&backBuffer );
    if( !CHECK_RESULT( hResult ) ) {
        LOG_RESULT( hResult );
        return false;
    }
    hResult = m_device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_backBufferTargetView
    );
    if( !CHECK_RESULT( hResult ) ) {
        LOG_RESULT( hResult );
        backBuffer.Reset();
        return false;
    }
    backBuffer.Reset();

    m_context->OMSetRenderTargets(
        1,
        m_backBufferTargetView.GetAddressOf(),
        nullptr
    );

    m_d3dviewport = {};

    return true;
}
RendererAPIDirectX11::~RendererAPIDirectX11() {
    m_backBufferTargetView.Reset();
    m_context.Reset();
    m_swapchain.Reset();
    m_device.Reset();
}
void RendererAPIDirectX11::ClearBuffer() {
    m_context->ClearRenderTargetView(
        m_backBufferTargetView.Get(),
        glm::value_ptr( m_clearColor )
    );
}
void RendererAPIDirectX11::SwapBuffers() {
    const u32 SYNC_INTERVAL = 1;
    const u32 FLAGS         = 0;
    HRESULT hResult = m_swapchain->Present( SYNC_INTERVAL, FLAGS );
    if( !CHECK_RESULT( hResult ) ) {
        LOG_RESULT( m_device->GetDeviceRemovedReason() );
    }
}
void RendererAPIDirectX11::SetClearColor( const glm::vec4& color )  {
    m_clearColor = color;
}
void RendererAPIDirectX11::SetViewport( const glm::vec2& viewport ) {
    m_viewport = viewport;
    
}
void RendererAPIDirectX11::EnableWireframe() {
    
}
void RendererAPIDirectX11::DisableWireframe() {
    
}
void RendererAPIDirectX11::EnableBlending() {
    
}
void RendererAPIDirectX11::DisableBlending() {
    
}
void RendererAPIDirectX11::BlendEquation( BlendEq Color, BlendEq Alpha ) {
    
}
void RendererAPIDirectX11::SetConstantBlendColor( const glm::vec4& blendColor ) {
    
}
void RendererAPIDirectX11::SetPackAlignment( PixelAlignment alignment ) {
    
}
void RendererAPIDirectX11::SetUnpackAlignment( PixelAlignment alignment ) {
    
}
void RendererAPIDirectX11::SetActiveTexture( u32 activeTexture ) const {
    
}
void RendererAPIDirectX11::DrawVertexArray( const VertexArray* va ) {
    if( va->HasIndexBuffer() ) {
        // glDrawElements(
        //     GL_TRIANGLES,
        //     va->GetIndexBuffer()->Count(),
        //     BufferDataTypeToGLenum( va->GetIndexBuffer()->DataType() ),
        //     nullptr
        // );
    } else {
        // auto& vb = va->GetVertexBuffers()[0];
        // m_pContext->Draw(
        //     vb->Count(),
        //     0
        // );
    }
}
void RendererAPIDirectX11::BlendFunction(
    BlendFactor srcColor,
    BlendFactor dstColor,
    BlendFactor srcAlpha,
    BlendFactor dstAlpha
) {
    
}

VertexArrayDirectX11::~VertexArrayDirectX11() {
    for( auto buffer : m_vertexBuffers ) {
        delete( buffer );
    }
    SAFE_DEL( m_indexBuffer );
}
void VertexArrayDirectX11::UseArray() const {
    for( auto const& buffer : m_vertexBuffers ) {
        buffer->UseBuffer();
    }
    if( m_hasIndexBuffer ) {
        m_indexBuffer->UseBuffer();
    }
}
void VertexArrayDirectX11::Unbind() const {
    // TODO:
}
void VertexArrayDirectX11::AddVertexBuffer( VertexBuffer* vertexBuffer ) {
    m_vertexBuffers.push_back(vertexBuffer);
}
void VertexArrayDirectX11::SetIndexBuffer( IndexBuffer* indexBuffer ) {
    m_indexBuffer    = indexBuffer;
    m_hasIndexBuffer = true;
}

VertexBufferDirectX11::VertexBufferDirectX11( usize size, const void* data ) {
    D3D11_BUFFER_DESC pDesc = {};
    pDesc.ByteWidth         = size;
    pDesc.Usage             = D3D11_USAGE_DEFAULT;
    pDesc.BindFlags         = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA pInitialData = {};
    pInitialData.pSysMem = data;

    HRESULT hResult = RendererAPIDirectX11::GetDevice()->CreateBuffer(
        &pDesc,
        &pInitialData,
        &m_buffer
    );
    if( !CHECK_RESULT(hResult) ) {
        LOG_RESULT(hResult);
    }

    m_stride  = (u32*)malloc( 1 );
    m_offsets = (u32*)malloc( 1 );
}
VertexBufferDirectX11::~VertexBufferDirectX11() {
    m_buffer.Reset();
    free( m_offsets );
    free( m_stride );
}
void VertexBufferDirectX11::UseBuffer() const {
    RendererAPIDirectX11::GetContext()->IASetVertexBuffers(
        0,
        1,
        m_buffer.GetAddressOf(),
        m_stride,
        m_offsets
    );
}
void VertexBufferDirectX11::SetLayout( BufferLayout layout ) {
    m_stride[0] = layout.Stride();

    // free old offsets and allocate new memory for new offsets
    free( m_offsets );
    m_offsets = (u32*)malloc( layout.Elements().size() * sizeof(u32) );
    for( usize i = 0; i < layout.Elements().size(); i++ ) {
        m_offsets[i] = layout.Elements()[i].offset;
    }

    m_bufferLayout = layout;
}

IndexBufferDirectX11::IndexBufferDirectX11( BufferDataType type, usize count, const void* data ) {
    
}
IndexBufferDirectX11::~IndexBufferDirectX11() {
    
}
void IndexBufferDirectX11::UseBuffer() const {
    
}

ShaderDirectX11::ShaderDirectX11( const std::string& vertex, const std::string& fragment ) {
    m_success = false;
    ComPtr<ID3DBlob> bytecode = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hResult = D3DCompile(
        vertex.c_str(),
        vertex.size(),
        NULL,
        NULL,
        NULL,
        "main",
        "vs_4_0",
        0,
        0,
        &bytecode,
        &errorBlob
    );

    if( !CHECK_RESULT(hResult) ) {
        LOG_ERROR("DirectX 11 > Failed to compile vertex shader!");
        if( errorBlob ) {
            std::string message = REMOVE_NEWLINE( (char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize() );
            LOG_ERROR("DirectX 11 > %s", message.c_str());
            errorBlob.Reset();
        }
        return;
    }

    hResult = RendererAPIDirectX11::GetDevice()->CreateVertexShader(
        bytecode->GetBufferPointer(),
        bytecode->GetBufferSize(),
        nullptr,
        &m_vshader
    );

    if( !CHECK_RESULT(hResult) ) {
        LOG_RESULT(hResult);
        errorBlob.Reset();
        bytecode.Reset();
        return;
    }

    errorBlob.Reset();
    bytecode.Reset();

    hResult = D3DCompile(
        fragment.c_str(),
        fragment.size(),
        NULL,
        NULL,
        NULL,
        "main",
        "ps_4_0",
        0,
        0,
        &bytecode,
        &errorBlob
    );

    if( !CHECK_RESULT(hResult) ) {
        LOG_ERROR("DirectX 11 > Failed to compile fragment shader!");
        if( errorBlob ) {
            std::string message = REMOVE_NEWLINE( (char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize() );
            LOG_ERROR("DirectX 11 > %s", message.c_str());
            errorBlob.Reset();
        }
        return;
    }

    hResult = RendererAPIDirectX11::GetDevice()->CreatePixelShader(
        bytecode->GetBufferPointer(),
        bytecode->GetBufferSize(),
        nullptr,
        &m_fshader
    );

    if( !CHECK_RESULT(hResult) ) {
        LOG_RESULT(hResult);
        errorBlob.Reset();
        bytecode.Reset();
        return;
    }

    errorBlob.Reset();
    bytecode.Reset();

    m_success = true;
}
ShaderDirectX11::~ShaderDirectX11() {
    m_vshader.Reset();
    m_fshader.Reset();
}
void ShaderDirectX11::UseShader() const {
    RendererAPIDirectX11::GetContext()->VSSetShader(
        m_vshader.Get(),
        0, 0
    );
    RendererAPIDirectX11::GetContext()->PSSetShader(
        m_fshader.Get(),
        0, 0
    );
}
bool ShaderDirectX11::GetUniform( const std::string& uniformName, UniformID& id ) const {
    
}
void ShaderDirectX11::UniformFloat( const UniformID& id, f32 value ) {
    
}
void ShaderDirectX11::UniformInt( const UniformID& id, i32 value ) {
    
}
void ShaderDirectX11::UniformVec2( const UniformID& id, const glm::vec2& value ) {
    
}
void ShaderDirectX11::UniformVec3( const UniformID& id, const glm::vec3& value ) {
    
}
void ShaderDirectX11::UniformVec4( const UniformID& id, const glm::vec4& value ) {
    
}
void ShaderDirectX11::UniformMat4( const UniformID& id, const glm::mat4x4& value ) {
    
}
void ShaderDirectX11::UniformMat3( const UniformID& id, const glm::mat3x3& value ) {
    
}

UniformBufferDirectX11::UniformBufferDirectX11( usize size, const void* data ) {

}
UniformBufferDirectX11::~UniformBufferDirectX11() {
    
}
void UniformBufferDirectX11::BufferData( usize size, const void* data ) {
    
}
void UniformBufferDirectX11::BufferSubData( usize offset, usize size, const void* data ) {
    
}
void UniformBufferDirectX11::SetBindingPoint( usize point ) {
    
}
void UniformBufferDirectX11::SetBindingPointRange( usize offset, usize size, usize point ) {
    
}

Texture2DDirectX11::Texture2DDirectX11(
    const glm::ivec2& dimensions,
    const void* data,
    TextureFormat format,
    TextureInternalFormat internalFormat,
    BufferDataType dataType,
    i32 mipmapLevel,
    bool storeData
) {

}
Texture2DDirectX11::~Texture2DDirectX11() {
    
}
void Texture2DDirectX11::UseTexture( usize unit ) const {
    
}
void Texture2DDirectX11::SetHorizontalWrap( TextureWrapMode wrap ) {
    
}
void Texture2DDirectX11::SetVerticalWrap( TextureWrapMode wrap ) {
    
}
void Texture2DDirectX11::SetMinificationFilter( TextureFilterMin filter ) {
    
}
void Texture2DDirectX11::SetMagnificationFilter( TextureFilterMag filter ) {
    
}

#endif