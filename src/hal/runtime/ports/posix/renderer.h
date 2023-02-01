#ifndef SRC__RUNTIME__SIMULATOR__RENDERER__H_
#define SRC__RUNTIME__SIMULATOR__RENDERER__H_

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

static constexpr size_t kInstanceRows = 10;
static constexpr size_t kInstanceColumns = 10;
static constexpr size_t kInstanceDepth = 10;
static constexpr size_t kNumInstances =
    (kInstanceRows * kInstanceColumns * kInstanceDepth);
static constexpr size_t kMaxFramesInFlight = 3;

class Renderer {
public:
  Renderer(MTL::Device *pDevice);
  ~Renderer();
  void buildShaders();
  void buildDepthStencilStates();
  void buildBuffers();
  void draw(MTK::View *pView);

private:
  MTL::Device *_pDevice;
  MTL::CommandQueue *_pCommandQueue;
  MTL::Library *_pShaderLibrary;
  MTL::RenderPipelineState *_pPSO;
  MTL::DepthStencilState *_pDepthStencilState;
  MTL::Buffer *_pVertexDataBuffer;
  MTL::Buffer *_pInstanceDataBuffer[kMaxFramesInFlight];
  MTL::Buffer *_pCameraDataBuffer[kMaxFramesInFlight];
  MTL::Buffer *_pIndexBuffer;
  float _angle;
  int _frame;
  dispatch_semaphore_t _semaphore;
  static const int kMaxFramesInFlight;
};

#endif // SRC__RUNTIME__SIMULATOR__RENDERER__H_