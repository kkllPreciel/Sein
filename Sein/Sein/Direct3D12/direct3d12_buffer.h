/**
 *  @file     buffer.h
 *  @brief    バッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/07/19
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>
#include "direct3d12_resource.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  バッファクラス
     */
    class Buffer final : public IResource
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Buffer();

      /**
       *  @brief  デストラクタ
       */
      ~Buffer();

      /**
       *  @brief  リソース(バッファ)を生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  properties:リソースのヒーププロパティ
       *  @param  width:リソースのサイズ
       *  @param  flag:リソース操作オプションフラグ
       */
      void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false);

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
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Buffer(const Buffer& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      Buffer& operator = (const Buffer& other) = delete;

    private:
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> buffer; ///< バッファ
    };
  };
};