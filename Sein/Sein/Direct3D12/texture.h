/**
 *  @file     texture.h
 *  @brief    テクスチャに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>
#include "resource.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  テクスチャ用インターフェイス
     */
    class ITexture : public IResource
    {
    public:
      /**
       *  @brief  デストラクタ
       */
      virtual ~ITexture() override = default;

      /**
       *  @brief  テクスチャリソースを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  properties:リソースのヒーププロパティ
       *  @param  width:リソースの横幅
       *  @param  height:リソースの縦幅
       *  @param  flag:リソース操作オプションフラグ
       */
      virtual void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false) = 0;
    };

    /**
     *  @brief  テクスチャ用クラス
     */
    class Texture final : public ITexture
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Texture();

      /**
       *  @brief  デストラクタ
       */
      ~Texture() override;

      /**
       *  @brief  テクスチャリソースを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  properties:リソースのヒーププロパティ
       *  @param  width:リソースの横幅
       *  @param  height:リソースの縦幅
       *  @param  flag:リソース操作オプションフラグ
       */
      void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false) override;

      /**
       *  @brief  リソースを開放する
       */
      void Release() noexcept override;

      /**
       *  @brief  リソースを取得する
       *  @return リソースの参照
       */
      ID3D12Resource& Get() const noexcept override;

    private:
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> buffer; ///< バッファ
    };
  };
};