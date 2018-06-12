/**
 *  @file     direct3d11_index_buffer.h
 *  @brief    Direct3D11のインデックスバッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/05
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11のインデックスバッファ用インターフェイス
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
      virtual void Map(ID3D11DeviceContext* const context, const std::uint32_t size_in_bytes, const void* const indices) = 0;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  インデックスバッファを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  size_in_bytes:インデックスバッファのサイズ(インデックスサイズ * インデックス数)
       *  @param  index_type:インデックスの型
       *  @return インデックスバッファへのシェアードポインタ
       */
      static std::shared_ptr<IIndexBuffer> Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, IIndexBuffer::Type index_type);
    };
  };
};