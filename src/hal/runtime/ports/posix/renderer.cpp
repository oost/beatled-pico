#include <simd/simd.h>

#include "math.h"
#include "renderer.h"
#include "shaders/triangle.metal.h"

const int Renderer::kMaxFramesInFlight = 3;

Renderer::Renderer(MTL::Device *pDevice)
    : _pDevice(pDevice->retain()), _angle(0.f), _frame(0) {
  _pCommandQueue = _pDevice->newCommandQueue();
  buildShaders();
  buildDepthStencilStates();
  buildBuffers();

  _semaphore = dispatch_semaphore_create(Renderer::kMaxFramesInFlight);
}

Renderer::~Renderer() {
  _pShaderLibrary->release();
  _pDepthStencilState->release();
  _pVertexDataBuffer->release();
  for (int i = 0; i < kMaxFramesInFlight; ++i) {
    _pInstanceDataBuffer[i]->release();
  }
  for (int i = 0; i < kMaxFramesInFlight; ++i) {
    _pCameraDataBuffer[i]->release();
  }
  _pIndexBuffer->release();
  _pPSO->release();
  _pCommandQueue->release();
  _pDevice->release();
}

namespace shader_types {
struct VertexData {
  simd::float3 position;
  simd::float3 normal;
};

struct InstanceData {
  simd::float4x4 instanceTransform;
  simd::float3x3 instanceNormalTransform;
  simd::float4 instanceColor;
};

struct CameraData {
  simd::float4x4 perspectiveTransform;
  simd::float4x4 worldTransform;
  simd::float3x3 worldNormalTransform;
};
} // namespace shader_types

void Renderer::buildShaders() {
  using NS::StringEncoding::UTF8StringEncoding;

  const char *shaderSrc = reinterpret_cast<char *>(shaders_triangle_metal);

  NS::Error *pError = nullptr;
  MTL::Library *pLibrary = _pDevice->newLibrary(
      NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError);
  if (!pLibrary) {
    __builtin_printf("%s", pError->localizedDescription()->utf8String());
    assert(false);
  }

  MTL::Function *pVertexFn = pLibrary->newFunction(
      NS::String::string("vertexMain", UTF8StringEncoding));
  MTL::Function *pFragFn = pLibrary->newFunction(
      NS::String::string("fragmentMain", UTF8StringEncoding));

  MTL::RenderPipelineDescriptor *pDesc =
      MTL::RenderPipelineDescriptor::alloc()->init();
  pDesc->setVertexFunction(pVertexFn);
  pDesc->setFragmentFunction(pFragFn);
  pDesc->colorAttachments()->object(0)->setPixelFormat(
      MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
  pDesc->setDepthAttachmentPixelFormat(
      MTL::PixelFormat::PixelFormatDepth16Unorm);

  _pPSO = _pDevice->newRenderPipelineState(pDesc, &pError);
  if (!_pPSO) {
    __builtin_printf("%s", pError->localizedDescription()->utf8String());
    assert(false);
  }

  pVertexFn->release();
  pFragFn->release();
  pDesc->release();
  _pShaderLibrary = pLibrary;
}

void Renderer::buildDepthStencilStates() {
  MTL::DepthStencilDescriptor *pDsDesc =
      MTL::DepthStencilDescriptor::alloc()->init();
  pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
  pDsDesc->setDepthWriteEnabled(true);

  _pDepthStencilState = _pDevice->newDepthStencilState(pDsDesc);

  pDsDesc->release();
}
void Renderer::buildBuffers() {
  using simd::float3;
  const float s = 0.5f;

  shader_types::VertexData verts[] = {
      //   Positions          Normals
      {{-s, -s, +s}, {0.f, 0.f, 1.f}},  {{+s, -s, +s}, {0.f, 0.f, 1.f}},
      {{+s, +s, +s}, {0.f, 0.f, 1.f}},  {{-s, +s, +s}, {0.f, 0.f, 1.f}},

      {{+s, -s, +s}, {1.f, 0.f, 0.f}},  {{+s, -s, -s}, {1.f, 0.f, 0.f}},
      {{+s, +s, -s}, {1.f, 0.f, 0.f}},  {{+s, +s, +s}, {1.f, 0.f, 0.f}},

      {{+s, -s, -s}, {0.f, 0.f, -1.f}}, {{-s, -s, -s}, {0.f, 0.f, -1.f}},
      {{-s, +s, -s}, {0.f, 0.f, -1.f}}, {{+s, +s, -s}, {0.f, 0.f, -1.f}},

      {{-s, -s, -s}, {-1.f, 0.f, 0.f}}, {{-s, -s, +s}, {-1.f, 0.f, 0.f}},
      {{-s, +s, +s}, {-1.f, 0.f, 0.f}}, {{-s, +s, -s}, {-1.f, 0.f, 0.f}},

      {{-s, +s, +s}, {0.f, 1.f, 0.f}},  {{+s, +s, +s}, {0.f, 1.f, 0.f}},
      {{+s, +s, -s}, {0.f, 1.f, 0.f}},  {{-s, +s, -s}, {0.f, 1.f, 0.f}},

      {{-s, -s, -s}, {0.f, -1.f, 0.f}}, {{+s, -s, -s}, {0.f, -1.f, 0.f}},
      {{+s, -s, +s}, {0.f, -1.f, 0.f}}, {{-s, -s, +s}, {0.f, -1.f, 0.f}},
  };

  uint16_t indices[] = {
      0,  1,  2,  2,  3,  0,  /* front */
      4,  5,  6,  6,  7,  4,  /* right */
      8,  9,  10, 10, 11, 8,  /* back */
      12, 13, 14, 14, 15, 12, /* left */
      16, 17, 18, 18, 19, 16, /* top */
      20, 21, 22, 22, 23, 20, /* bottom */
  };

  const size_t vertexDataSize = sizeof(verts);
  const size_t indexDataSize = sizeof(indices);

  MTL::Buffer *pVertexBuffer =
      _pDevice->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
  MTL::Buffer *pIndexBuffer =
      _pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

  _pVertexDataBuffer = pVertexBuffer;
  _pIndexBuffer = pIndexBuffer;

  memcpy(_pVertexDataBuffer->contents(), verts, vertexDataSize);
  memcpy(_pIndexBuffer->contents(), indices, indexDataSize);

  _pVertexDataBuffer->didModifyRange(
      NS::Range::Make(0, _pVertexDataBuffer->length()));
  _pIndexBuffer->didModifyRange(NS::Range::Make(0, _pIndexBuffer->length()));

  const size_t instanceDataSize =
      kMaxFramesInFlight * kNumInstances * sizeof(shader_types::InstanceData);
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    _pInstanceDataBuffer[i] =
        _pDevice->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
  }

  const size_t cameraDataSize =
      kMaxFramesInFlight * sizeof(shader_types::CameraData);
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    _pCameraDataBuffer[i] =
        _pDevice->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged);
  }
}

void Renderer::draw(MTK::View *pView) {
  using simd::float3;
  using simd::float4;
  using simd::float4x4;

  NS::AutoreleasePool *pPool = NS::AutoreleasePool::alloc()->init();

  _frame = (_frame + 1) % Renderer::kMaxFramesInFlight;
  MTL::Buffer *pInstanceDataBuffer = _pInstanceDataBuffer[_frame];

  MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();
  dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
  Renderer *pRenderer = this;
  pCmd->addCompletedHandler(^void(MTL::CommandBuffer *pCmd) {
    dispatch_semaphore_signal(pRenderer->_semaphore);
  });

  _angle += 0.002f;

  // Update instance positions:

  const float scl = 0.2f;
  shader_types::InstanceData *pInstanceData =
      reinterpret_cast<shader_types::InstanceData *>(
          pInstanceDataBuffer->contents());

  float3 objectPosition = {0.f, 0.f, -10.f};

  float4x4 rt = math::makeTranslate(objectPosition);
  float4x4 rr1 = math::makeYRotate(-_angle);
  float4x4 rr0 = math::makeXRotate(_angle * 0.5);
  float4x4 rtInv = math::makeTranslate(
      {-objectPosition.x, -objectPosition.y, -objectPosition.z});
  float4x4 fullObjectRot = rt * rr1 * rr0 * rtInv;

  size_t ix = 0;
  size_t iy = 0;
  size_t iz = 0;
  for (size_t i = 0; i < kNumInstances; ++i) {
    if (ix == kInstanceRows) {
      ix = 0;
      iy += 1;
    }
    if (iy == kInstanceRows) {
      iy = 0;
      iz += 1;
    }

    float4x4 scale = math::makeScale((float3){scl, scl, scl});
    float4x4 zrot = math::makeZRotate(_angle * sinf((float)ix));
    float4x4 yrot = math::makeYRotate(_angle * cosf((float)iy));

    float x = ((float)ix - (float)kInstanceRows / 2.f) * (2.f * scl) + scl;
    float y = ((float)iy - (float)kInstanceColumns / 2.f) * (2.f * scl) + scl;
    float z = ((float)iz - (float)kInstanceDepth / 2.f) * (2.f * scl);
    float4x4 translate =
        math::makeTranslate(math::add(objectPosition, {x, y, z}));

    pInstanceData[i].instanceTransform =
        fullObjectRot * translate * yrot * zrot * scale;
    pInstanceData[i].instanceNormalTransform =
        math::discardTranslation(pInstanceData[i].instanceTransform);

    float iDivNumInstances = i / (float)kNumInstances;
    float r = iDivNumInstances;
    float g = 1.0f - r;
    float b = sinf(M_PI * 2.0f * iDivNumInstances);
    pInstanceData[i].instanceColor = (float4){r, g, b, 1.0f};

    ix += 1;
  }
  pInstanceDataBuffer->didModifyRange(
      NS::Range::Make(0, pInstanceDataBuffer->length()));

  // Update camera state:

  MTL::Buffer *pCameraDataBuffer = _pCameraDataBuffer[_frame];
  shader_types::CameraData *pCameraData =
      reinterpret_cast<shader_types::CameraData *>(
          pCameraDataBuffer->contents());
  pCameraData->perspectiveTransform =
      math::makePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f);
  pCameraData->worldTransform = math::makeIdentity();
  pCameraData->worldNormalTransform =
      math::discardTranslation(pCameraData->worldTransform);
  pCameraDataBuffer->didModifyRange(
      NS::Range::Make(0, sizeof(shader_types::CameraData)));

  // Begin render pass:

  MTL::RenderPassDescriptor *pRpd = pView->currentRenderPassDescriptor();
  MTL::RenderCommandEncoder *pEnc = pCmd->renderCommandEncoder(pRpd);

  pEnc->setRenderPipelineState(_pPSO);
  pEnc->setDepthStencilState(_pDepthStencilState);

  pEnc->setVertexBuffer(_pVertexDataBuffer, /* offset */ 0, /* index */ 0);
  pEnc->setVertexBuffer(pInstanceDataBuffer, /* offset */ 0, /* index */ 1);
  pEnc->setVertexBuffer(pCameraDataBuffer, /* offset */ 0, /* index */ 2);

  pEnc->setCullMode(MTL::CullModeBack);
  pEnc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

  pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, 6 * 6,
                              MTL::IndexType::IndexTypeUInt16, _pIndexBuffer, 0,
                              kNumInstances);

  pEnc->endEncoding();
  pCmd->presentDrawable(pView->currentDrawable());
  pCmd->commit();

  pPool->release();
}