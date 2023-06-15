#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include <iostream>

TextureManager::~TextureManager()
{
    for (const auto& pair : m_Textures)
    {
        delete pair.second;
    }
}

Texture* TextureManager::GetTexture(const std::string& filename)
{
    const auto it = m_Textures.find(filename);
    if (it != m_Textures.end())
    {
        std::cout << "Reusing texture: " << filename << std::endl;
        return it->second;
    }
    std::cout << "Loading texture: " << filename << std::endl; 
    Texture* texture = new Texture(filename);
    m_Textures[filename] = texture;
    return texture;
}
