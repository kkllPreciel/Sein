/**
 *  @file     index_buffer.h
 *  @brief    レンダラーのインデックスバッファに関するヘッダファイル
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
     *  @brief  インデックスバッファ用インターフェース
     */
    class IIndexBuffer
    {
    public:
      /**
       *  @brief  インデックスの型
       */
      enum class Type : std::uint32_t {
        k16Bit,
        k32Bit,
      };

      /**
       *  @brief  コンストラクタ
       */
      IIndexBuffer() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IIndexBuffer() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IIndexBuffer(const IIndexBuffer& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IIndexBuffer& operator = (const IIndexBuffer& other) = delete;

      /**
       *  @brief  マップする
       *  @param  context:コンテキスト
       *  @param  size_in_bytes:インデックス配列のサイズ
       *  @param  indices:インデックス配列へのポインタ
       */
      virtual void Map(IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const indices) = 0;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;
    };
  };
};