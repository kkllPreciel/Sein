/**
 *  @file     descriptor_range.h
 *  @brief    ディスクリプターレンジに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/10
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ディスクリプターレンジ用インターフェイス
     */
    class IDescriptorRange
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IDescriptorRange() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IDescriptorRange() = default;

      /**
       *  @brief  ディスクリプターレンジを取得する
       *  @return ディスクリプターレンジ
       */
      virtual const D3D12_DESCRIPTOR_RANGE& Get() const = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDescriptorRange(const IDescriptorRange& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDescriptorRange& operator = (const IDescriptorRange& other) = delete;

      /**
       *  @brief  ディスクリプターレンジを作成する
       *  @param  descriptor_range_type:ディスクリプターレンジの種別
       *  @param  descriptor_count:ディスクリプターの個数
       *  @return ディスクリプターレンジインターフェイスへのシェアードポインタ
       */
      static std::shared_ptr<IDescriptorRange> Create(const D3D12_DESCRIPTOR_RANGE_TYPE descriptor_range_type, std::uint32_t descriptor_count);
    };
  };
};