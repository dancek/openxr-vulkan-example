#pragma once

#include <glm/fwd.hpp>

#include <vulkan/vulkan.h>

#include <vector>

class Context;
class DataBuffer;
class Headset;
class MeshData;
struct Model;
class Pipeline;
class RenderProcess;

/*
 * The renderer class facilitates rendering with Vulkan. It is initialized with a constant list of models to render and
 * holds the vertex/index buffer, the pipelines that define the rendering techniques to use, as well as a number of
 * render processes. Note that all resources that need to be duplicated in order to be able to render several frames in
 * parallel is held by this number of render processes.
 */
class Renderer final
{
public:
  Renderer(const Context* context, const Headset* headset, const MeshData* meshData, const std::vector<Model*>& models);
  ~Renderer();

  void render(const glm::mat4& cameraMatrix, size_t swapchainImageIndex, float time);
  void submit(bool useSemaphores) const;

  bool isValid() const;
  VkCommandBuffer getCurrentCommandBuffer() const;
  VkSemaphore getCurrentDrawableSemaphore() const;
  VkSemaphore getCurrentPresentableSemaphore() const;

private:
  bool valid = true;

  const Context* context = nullptr;
  const Headset* headset = nullptr;

  VkCommandPool commandPool = VK_NULL_HANDLE;
  VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
  VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
  std::vector<RenderProcess*> renderProcesses;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
  Pipeline *gridPipeline = nullptr, *diffusePipeline = nullptr;
  DataBuffer* vertexIndexBuffer = nullptr;
  std::vector<Model*> models;
  size_t indexOffset = 0u;
  size_t currentRenderProcessIndex = 0u;
};