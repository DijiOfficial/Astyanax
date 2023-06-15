#pragma once
#include <unordered_map>

class Texture;

class TextureManager final
{
public:
    explicit TextureManager() = default;
    ~TextureManager();
    TextureManager(const TextureManager& other) = delete;
    TextureManager(TextureManager&& other) noexcept = delete;
    TextureManager& operator=(const TextureManager& other) = delete;
    TextureManager& operator=(TextureManager&& other) noexcept = delete;
    
    Texture* GetTexture(const std::string& filename);
private:
    std::unordered_map<std::string, Texture*> m_Textures;
    
};
