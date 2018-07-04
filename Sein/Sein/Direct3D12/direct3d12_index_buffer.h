/**
 *  @file     index_buffer.h
 *  @brief    頂点インデックスバッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/03
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
     *  @brief  頂点インデックスバッファ用インターフェイス
     */
    class IIndexBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IIndexBuffer() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IIndexBuffer() = default;

      /**
       *  @brief  マップする
       *  @param  format:頂点インデックスのフォーマット
       *  @param  indices:頂点インデックス配列へのポインタ
       */
      virtual void Map(const DXGI_FORMAT format, const void* const indices) = 0;

      /**
       *  @brief  頂点インデックスバッファビューを取得する
       *  @return 頂点インデックスバッファビュー
       */
      virtual const D3D12_INDEX_BUFFER_VIEW& GetView() = 0;

      /**
       *  @brief  頂点インデックスバッファを開放する
       */
      virtual void Release() noexcept = 0;

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
       *  @brief  頂点インデックスバッファを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
       *  @return 頂点インデックスバッファへのユニークポインタ
       */
      static std::unique_ptr<IIndexBuffer> Create(ID3D12Device* const device, const std::uint32_t size_in_bytes);
    };
  };
};