#pragma once
#include "GameObject.h"

#define BLOCK_SIZE 8

//ブロックタイプ
enum BlockType {
    STONE,
    WOOD,
    BRICK,
    PUMPKIN,
};

class Block :
    public GameObject
{
public:
    BlockType blockType = BlockType::STONE;

    Block() = default;
    ~Block() = default;
};

