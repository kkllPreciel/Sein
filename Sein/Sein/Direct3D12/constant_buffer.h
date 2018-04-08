/**
 *  @file     constant_buffer.h
 *  @brief    定数バッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/08
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
     *  @brief  定数バッファ用インターフェイス
     */
    class IConstantBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IConstantBuffer() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IConstantBuffer() = default;

      /**
       *  @brief  マップする
       *  @param  size_in_bytes:マップするデータのサイズ
       *  @param  data:マップするデータへのポインタ
       */
      virtual void Map(const std::uint32_t size_in_bytes, const void* const data) = 0;

      /**
       *  @brief  定数バッファを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IConstantBuffer(const IConstantBuffer& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IConstantBuffer& operator = (const IConstantBuffer& other) = delete;

      /**
       *  @brief  定数バッファを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  descriptor_handle:ディスクリプターハンドル
       *  @param  size_in_bytes:定数バッファのサイズ
       *  @return 定数バッファへのユニークポインタ
       */
      static std::unique_ptr<IConstantBuffer> Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const std::uint32_t size_in_bytes);
    };
  };
};