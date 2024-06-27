#pragma once

#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "WinMin.h"
#include "Geometry.h"
#include "Material.h"
#include "MeshData.h"
#include <d3d11_1.h>
#include <wrl/client.h>

struct Model
{
    Model() = default; // デフォルトコンストラクタ
    ~Model() = default; // デストラクタ
    Model(Model&) = delete; // コピーコンストラクタ禁止
    Model& operator=(const Model&) = delete; // コピー代入禁止
    Model(Model&&) = default; // ムーブコンストラクタ
    Model& operator=(Model&&) = default; // ムーブ代入

    std::vector<Material> materials; // 材料のリスト
    std::vector<MeshData> meshdatas; // メッシュデータのリスト
    DirectX::BoundingBox boundingbox; // バウンディングボックス
    static void CreateFromFile(Model& model, ID3D11Device* device, std::string_view filename); // ファイルからモデルを作成
    static void CreateFromGeometry(Model& model, ID3D11Device* device, const GeometryData& data, bool isDynamic = false); // ジオメトリデータからモデルを作成

    void SetDebugObjectName(std::string_view name); // デバッグ用オブジェクト名を設定

    static BoundingBox ComputeAABB(const std::vector<XMFLOAT3>& vertices); // 頂点からAABBを計算
};


class ModelManager
{
public:
    ModelManager(); // コンストラクタ
    ~ModelManager(); // デストラクタ
    ModelManager(ModelManager&) = delete; // コピーコンストラクタ禁止
    ModelManager& operator=(const ModelManager&) = delete; // コピー代入禁止
    ModelManager(ModelManager&&) = default; // ムーブコンストラクタ
    ModelManager& operator=(ModelManager&&) = default; // ムーブ代入

    static ModelManager& Get(); // シングルトンインスタンスを取得
    void Init(ID3D11Device* device); // 初期化
    Model* CreateFromFile(std::string_view filename); // ファイルからモデルを作成
    Model* CreateFromFile(std::string_view name, std::string_view filename); // 名前とファイルからモデルを作成
    Model* CreateFromGeometry(std::string_view name, const GeometryData& data, bool isDynamic = false); // ジオメトリデータからモデルを作成

    const Model* GetModel(std::string_view name) const; // モデルを取得（読み取り専用）
    Model* GetModel(std::string_view name); // モデルを取得
private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice; // デバイス
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext; // デバイスコンテキスト
    std::unordered_map<size_t, Model> m_Models; // モデルのマップ
};


#endif // MODELMANAGER_H
