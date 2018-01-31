/**
 *  @file     texture.cc
 *  @brief    テクスチャに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

 // include
#include "texture.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    Texture::Texture() : buffer(nullptr, [](IUnknown* p) {p->Release(); })
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    Texture::~Texture()
    {
      Release();
    }

    /**
     *  @brief  テクスチャリソースを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  properties:リソースのヒーププロパティ
     *  @param  width:リソースの横幅
     *  @param  height:リソースの縦幅
     *  @param  flag:リソース操作オプションフラグ
     */
    void Texture::Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag) noexcept(false)
    {
      Release();

      // リソースの設定
      D3D12_RESOURCE_DESC resource_desc = {};
      resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // リソースの種別(今回は必ずバッファ)
      resource_desc.Alignment = 0;                                  // アラインメント
      resource_desc.Width = width;                                  // リソースの幅
      resource_desc.Height = height;                                // リソースの高さ
      resource_desc.DepthOrArraySize = 1;                           // リソースの深さ(テクスチャ等に使用する物、今回は必ず1)
      resource_desc.MipLevels = 1;                                  // ミップマップのレベル(今回は必ず1)
      resource_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // リソースデータフォーマット(R8G8B8A8等)
      resource_desc.SampleDesc.Count = 1;                           // ピクセル単位のマルチサンプリング数
      resource_desc.SampleDesc.Quality = 0;                         // マルチサンプリングの品質レベル
      resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;          // テクスチャレイアウトオプション
      resource_desc.Flags = flag;                                   // リソース操作オプションフラグ

      // リソース(バッファ)の生成
      ID3D12Resource* resource = nullptr;
      if (FAILED(device->CreateCommittedResource(
        &properties,                        // ヒープの設定
        D3D12_HEAP_FLAG_NONE,               // ヒープオプション(TODO:適切なオプションを設定する)
        &resource_desc,                     // リソースの設定
        D3D12_RESOURCE_STATE_COPY_DEST,     // リソースの状態(TODO:適切な状態を設定する)
        nullptr,                            // クリアカラーのデフォルト値
        IID_PPV_ARGS(&resource))))
      {
        throw "テクスチャ用リソースの作成に失敗しました。";
      }
      buffer.reset(resource);
    }
    
    /**
     *  @brief  リソースを開放する
     */
    void Texture::Release() noexcept
    {
      buffer.reset(nullptr);
    }
    
    /**
     *  @brief  リソースを取得する
     *  @return リソースの参照
     */
    ID3D12Resource& Texture::Get() const noexcept
    {
      return *buffer;
    }
  };
};