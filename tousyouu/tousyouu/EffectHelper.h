//***************************************************************************************
// EffectsHelper.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 定义一些实用的特效助手类
// Define utility effect helper classes.
//*************************************************************************************** 

#pragma once

#ifndef EFFECT_HELPER_H
#define EFFECT_HELPER_H

#include "WinMin.h"
#include <string_view>
#include <memory>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include "Property.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11GeometryShader;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
//
// EffectHelper
//

// レンダリングパスの説明
// 指定された名前で追加されたシェーダーを設定することによってレンダリングパスを指定する
struct EffectPassDesc
{
    std::string_view nameVS;
    std::string_view nameDS;
    std::string_view nameHS;
    std::string_view nameGS;
    std::string_view namePS;
    std::string_view nameCS;
};

// 定数バッファの変数
// 非COMコンポーネント
struct IEffectConstantBufferVariable
{
    // unsigned intを設定する、boolも設定可能
    virtual void SetUInt(uint32_t val) = 0;
    // signed intを設定する
    virtual void SetSInt(int val) = 0;
    // 浮動小数点数を設定する
    virtual void SetFloat(float val) = 0;

    // unsigned intベクトルを設定する、1から4つの成分を設定可能
    // シェーダー変数がbool型の場合も使用可能
    // 設定する成分数に基づいてdataの最初の数成分を読み取る
    virtual void SetUIntVector(uint32_t numComponents, const uint32_t data[4]) = 0;

    // signed intベクトルを設定する、1から4つの成分を設定可能
    // 設定する成分数に基づいてdataの最初の数成分を読み取る
    virtual void SetSIntVector(uint32_t numComponents, const int data[4]) = 0;

    // 浮動小数点数ベクトルを設定する、1から4つの成分を設定可能
    // 設定する成分数に基づいてdataの最初の数成分を読み取る
    virtual void SetFloatVector(uint32_t numComponents, const float data[4]) = 0;

    // unsigned int行列を設定する、行と列の数は1から4可能
    // パディングなしのデータを渡すこと、例えば3x3行列はUINT[3][3]型で直接渡すことができる
    virtual void SetUIntMatrix(uint32_t rows, uint32_t cols, const uint32_t* noPadData) = 0;

    // signed int行列を設定する、行と列の数は1から4可能
    // パディングなしのデータを渡すこと、例えば3x3行列はINT[3][3]型で直接渡すことができる
    virtual void SetSIntMatrix(uint32_t rows, uint32_t cols, const int* noPadData) = 0;

    // 浮動小数点数行列を設定する、行と列の数は1から4可能
    // パディングなしのデータを渡すこと、例えば3x3行列はFLOAT[3][3]型で直接渡すことができる
    virtual void SetFloatMatrix(uint32_t rows, uint32_t cols, const float* noPadData) = 0;

    // その他の型を設定する、設定範囲を指定可能
    virtual void SetRaw(const void* data, uint32_t byteOffset = 0, uint32_t byteCount = 0xFFFFFFFF) = 0;

    // プロパティを設定する
    virtual void Set(const Property& prop) = 0;

    // 最後に設定された値を取得する、読み取り範囲を指定可能
    virtual HRESULT GetRaw(void* pOutput, uint32_t byteOffset = 0, uint32_t byteCount = 0xFFFFFFFF) = 0;

    virtual ~IEffectConstantBufferVariable() {}
};

// レンダリングパス
// 非COMコンポーネント
class EffectHelper;
struct IEffectPass
{
    // ラスタライザステートを設定する
    virtual void SetRasterizerState(ID3D11RasterizerState* pRS) = 0;
    // ブレンドステートを設定する
    virtual void SetBlendState(ID3D11BlendState* pBS, const float blendFactor[4], uint32_t sampleMask) = 0;
    // 深度ステンシルステートを設定する
    virtual void SetDepthStencilState(ID3D11DepthStencilState* pDSS, uint32_t stencilValue) = 0;

    // 頂点シェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> VSGetParamByName(std::string_view paramName) = 0;
    // ドメインシェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> DSGetParamByName(std::string_view paramName) = 0;
    // ハルシェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> HSGetParamByName(std::string_view paramName) = 0;
    // ジオメトリシェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> GSGetParamByName(std::string_view paramName) = 0;
    // ピクセルシェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> PSGetParamByName(std::string_view paramName) = 0;
    // コンピュートシェーダーのuniform引数を取得して値を設定する
    virtual std::shared_ptr<IEffectConstantBufferVariable> CSGetParamByName(std::string_view paramName) = 0;
    // 所属するエフェクトヘルパーを取得する
    virtual EffectHelper* GetEffectHelper() = 0;
    // パス名を取得する
    virtual const std::string& GetPassName() = 0;

    // シェーダー、定数バッファ（引数含む）、サンプラー、シェーダーリソース、読み書き可能リソースをレンダリングパイプラインに適用する
    virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;

    // コンピュートシェーダーをディスパッチする
    // スレッド数を渡し、内部でコンピュートシェーダーのスレッドグループの次元に基づいて適切なスレッドグループ数を呼び出す
    virtual void Dispatch(ID3D11DeviceContext* deviceContext, uint32_t threadX = 1, uint32_t threadY = 1, uint32_t threadZ = 1) = 0;

    virtual ~IEffectPass() {};
};

// エフェクトヘルパー
// シェーダー、サンプラー、シェーダーリソース、定数バッファ、シェーダー引数、読み書き可能リソース、レンダリングステートを管理する
class EffectHelper
{
public:

    EffectHelper();
    ~EffectHelper();
    // コピー禁止、ムーブ可能
    EffectHelper(const EffectHelper&) = delete;
    EffectHelper& operator=(const EffectHelper&) = delete;
    EffectHelper(EffectHelper&&) = default;
    EffectHelper& operator=(EffectHelper&&) = default;

    // コンパイル済みシェーダーファイルのキャッシュパスを設定して作成する
    // キャッシュを無効にするには""を設定する
    // forceWriteをtrueにすると、プログラムを実行するたびにキャッシュを強制的に上書き保存する
    // デフォルトではコンパイル済みシェーダーはキャッシュされない
    // シェーダーが修正中の場合はforceWriteを有効にするべき
    void SetBinaryCacheDirectory(std::wstring_view cacheDir, bool forceWrite = false);

    // シェーダーをコンパイルするか、シェーダーバイトコードを読み込む。以下の順序で行われる：
    // 1. シェーダーバイトコードファイルのキャッシュパスが有効で、強制上書きが無効の場合、${cacheDir}/${shaderName}.csoを読み込んで追加することを試みる
    // 2. それ以外の場合はfilenameを読み込む。バイトコードの場合は直接追加する
    // 3. filenameがhlslソースコードの場合、コンパイルして追加する。シェーダーバイトコードファイルのキャッシュが有効の場合、${cacheDir}/${shaderName}.csoにシェーダーバイトコードを保存する
    // 注意：
    // 1. 異なるシェーダーコードで、同じスロットを使用する定数バッファがある場合、対応する定義は完全に一致している必要がある
    // 2. 異なるシェーダーコードで、グローバル変数が存在する場合、定義は完全に一致している必要がある
    // 3. 異なるシェーダーコードで、サンプラー、シェーダーリソース、または読み書き可能リソースが同じスロットを使用する場合、対応する定義は完全に一致している必要がある。そうでなければ、スロットによる設定のみ使用可能
    HRESULT CreateShaderFromFile(std::string_view shaderName, std::wstring_view filename, ID3D11Device* device,
        LPCSTR entryPoint = nullptr, LPCSTR shaderModel = nullptr, const D3D_SHADER_MACRO* pDefines = nullptr, ID3DBlob** ppShaderByteCode = nullptr);

    // シェーダーをコンパイルするのみ
    static HRESULT CompileShaderFromFile(std::wstring_view filename, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppShaderByteCode, ID3DBlob** ppErrorBlob = nullptr,
        const D3D_SHADER_MACRO* pDefines = nullptr, ID3DInclude* pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE);

    // コンパイル済みのシェーダーバイナリ情報を追加し、名前を設定する
    // この関数はシェーダーバイナリコーディングをファイルに保存しない
    // 注意：
    // 1. 異なるシェーダーコードで、同じregisterを使用する定数バッファがある場合、対応する定義は完全に一致している必要がある
    // 2. 異なるシェーダーコードで、グローバル変数が存在する場合、定義は完全に一致している必要がある
    // 3. 異なるシェーダーコードで、サンプラー、シェーダーリソース、または読み書き可能リソースが同じ槽を使用する場合、対応する定義は完全に一致している必要がある
    HRESULT AddShader(std::string_view name, ID3D11Device* device, ID3DBlob* blob);

    // ストリーム出力を持つジオメトリシェーダーを追加し、名前を設定する
    // この関数はシェーダーバイナリコーディングをファイルに保存しない
    // 注意：
    // 1. 異なるシェーダーコードで、同じスロットを使用する定数バッファがある場合、対応する定義は完全に一致している必要がある
    // 2. 異なるシェーダーコードで、グローバル変数が存在する場合、定義は完全に一致している必要がある
    // 3. 異なるシェーダーコードで、サンプラー、シェーダーリソース、または読み書き可能リソースが同じ槽を使用する場合、対応する定義は完全に一致している必要がある。そうでなければ、スロットによる設定のみ使用可能
    HRESULT AddGeometryShaderWithStreamOutput(std::string_view name, ID3D11Device* device, ID3D11GeometryShader* gsWithSO, ID3DBlob* blob);

    // すべての内容をクリアする
    void Clear();

    // レンダリングパスを作成する
    HRESULT AddEffectPass(std::string_view effectPassName, ID3D11Device* device, const EffectPassDesc* pDesc);
    // 特定のレンダリングパスを取得する
    std::shared_ptr<IEffectPass> GetEffectPass(std::string_view effectPassName);

    // 定数バッファの変数を取得して値を設定する
    std::shared_ptr<IEffectConstantBufferVariable> GetConstantBufferVariable(std::string_view name);

    // スロットによってサンプラーステートを設定する
    void SetSamplerStateBySlot(uint32_t slot, ID3D11SamplerState* samplerState);
    // 名前によってサンプラーステートを設定する（同じスロットに複数の名前が存在する場合は、スロットによる設定のみ使用可能）
    void SetSamplerStateByName(std::string_view name, ID3D11SamplerState* samplerState);
    // 名前によってサンプラーステートスロットをマッピングする（見つからない場合は-1を返す）
    int MapSamplerStateSlot(std::string_view name);

    // スロットによってシェーダーリソースを設定する
    void SetShaderResourceBySlot(uint32_t slot, ID3D11ShaderResourceView* srv);
    // 名前によってシェーダーリソースを設定する（同じスロットに複数の名前が存在する場合は、スロットによる設定のみ使用可能）
    void SetShaderResourceByName(std::string_view name, ID3D11ShaderResourceView* srv);
    // 名前によってシェーダーリソーススロットをマッピングする（見つからない場合は-1を返す）
    int MapShaderResourceSlot(std::string_view name);

    // スロットによって読み書き可能リソースを設定する
    void SetUnorderedAccessBySlot(uint32_t slot, ID3D11UnorderedAccessView* uav, uint32_t* pInitialCount = nullptr);
    // 名前によって読み書き可能リソースを設定する（同じスロットに複数の名前が存在する場合は、スロットによる設定のみ使用可能）
    void SetUnorderedAccessByName(std::string_view name, ID3D11UnorderedAccessView* uav, uint32_t* pInitialCount = nullptr);
    // 名前によって読み書き可能リソーススロットをマッピングする（見つからない場合は-1を返す）
    int MapUnorderedAccessSlot(std::string_view name);


    // デバッグオブジェクト名を設定する
    void SetDebugObjectName(std::string name);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};



#endif
