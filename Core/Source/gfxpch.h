#include <memory>
#include <fstream>
#include <windows.h>
#include <string>
#include <sstream>
#include <commdlg.h>
#include <tuple>
#include <assert.h>
#include <xhash>
#include <vector>
#include <random>
#include <unordered_map>
#include <filesystem>
#include <queue>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <stb_image/stb_image.h>