/**
 *  @file     texture_view.h
 *  @brief    テクスチャ用ビューに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>
#include "texture.h"
#include "descriptor_heap.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  テクスチャ用ビュークラス
     */
    class TextureView
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      TextureView();

      /**
       *  @brief  デストラクタ
       */
      ~TextureView();

      /**
       *  @brief  テクスチャ用ビューを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  texture:テクスチャ
       *  @param  descriptor_heap:ディスクリプターヒープ
       */
      void Create(ID3D12Device* const device, ITexture* const texture, IDescriptorHeap* const descriptor_heap) noexcept(false);

      /**
       *  @brief  テクスチャ用ビューを開放する
       */
      void Release() noexcept;

      /**
       *  @brief  テクスチャを取得する
       *  @return テクスチャへの参照
       */
      const ITexture& GetTexture() const;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      TextureView(const TextureView& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      TextureView& operator = (const TextureView& other) = delete;

    private:
      std::unique_ptr<ITexture> texture_;  ///< テクスチャ
    };
  };
};