/**
 *  @file     vertex_buffer.h
 *  @brief    頂点バッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  頂点バッファ用インターフェイス
     */
    class IVertexBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IVertexBuffer() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IVertexBuffer() = default;

      /**
       *  @brief  マップする
       *  @param  stride_in_bytes:1頂点のサイズ
       *  @param  vertices:頂点配列へのポインタ
       */
      virtual void Map(const std::uint32_t stride_in_bytes, const void* const vertices) = 0;

      /**
       *  @brief  頂点バッファビューを取得する
       *  @return 頂点バッファビュー
       */
      virtual const D3D12_VERTEX_BUFFER_VIEW& GetView() = 0;

      /**
       *  @brief  頂点バッファを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IVertexBuffer(const IVertexBuffer& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IVertexBuffer& operator = (const IVertexBuffer& other) = delete;

      /**
       *  @brief  頂点バッファを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのユニークポインタ
       */
      static std::unique_ptr<IVertexBuffer> Create(ID3D12Device* const device, const std::uint32_t size_in_bytes);
    };
  };
};