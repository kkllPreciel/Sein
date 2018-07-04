/**
 *  @file     vertex_buffer.h
 *  @brief    レンダラーの頂点バッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/07/04
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

#include "device_context.h"

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  頂点バッファ用インターフェース
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
       *  @brief  マップする
       *  @param  context:コンテキスト
       *  @param  size_in_bytes:頂点配列のサイズ
       *  @param  vertices:頂点配列へのポインタ
       */
      virtual void Map(IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const vertices) = 0;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;
    };
  };
};