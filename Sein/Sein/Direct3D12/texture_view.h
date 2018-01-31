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
#include "buffer.h"
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
       *  @param  descriptor_heap:ディスクリプターヒープ
       *  @param  width:テクスチャの横幅
       *  @param  height:テクスチャの縦幅
       */
      void Create(ID3D12Device* const device, IDescriptorHeap* const descriptor_heap, const UINT64 width, const UINT64 height) noexcept(false);

      /**
       *  @brief  テクスチャ用ビューを開放する
       */
      void Release() noexcept;

      /**
       *  @brief  データのマップを行う
       *  @param  data:テクスチャデータ
       *  @param  bytesPerPixel:1ピクセルで使用するバイト数
       */
      void Map(const void* const data, const unsigned int bytesPerPixel) noexcept(false);

      /**
       *  @brief  テクスチャを取得する
       *  @return テクスチャへの参照
       */
      const Texture& GetTexture() const;

      /**
       *  @brief  中間バッファを取得する
       *  @return 中間バッファへの参照
       */
      const Buffer& GetTemporaryBuffer() const;

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
      std::unique_ptr<Texture> texture_;  ///< テクスチャ
      std::unique_ptr<Buffer> buffer_;    ///< 中間バッファ
    };
  };
};