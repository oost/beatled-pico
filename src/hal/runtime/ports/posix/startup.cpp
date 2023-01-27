#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "app_delegate.h"
#include "hal/startup.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

int startup() {
  NS::AutoreleasePool *pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

  MyAppDelegate del;

  NS::Application *pSharedApplication = NS::Application::sharedApplication();
  pSharedApplication->setDelegate(&del);
  pSharedApplication->run();

  pAutoreleasePool->release();
  return 0;
}
