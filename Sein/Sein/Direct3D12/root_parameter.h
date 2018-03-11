/**
 *  @file     root_parameter.h
 *  @brief    ルートパラメータに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/11
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>
#include "descriptor_range.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ルートパラメータ用インターフェイス
     */
    class IRootParameter
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IRootParameter() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IRootParameter() = default;

      /**
       *  @brief  ルートパラメータを取得する
       *  @return ルートパラメータ
       */
      virtual const D3D12_ROOT_PARAMETER& Get() const = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IRootParameter(const IRootParameter& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IRootParameter& operator = (const IRootParameter& other) = delete;

      /**
       *  @brief  タイプがディスクリプターテーブルのルートパラメータを作成する
       *  @param  descriptor_range_type:ディスクリプターレンジの種別
       *  @param  descriptor_count:ディスクリプターの個数
       *  @return ディスクリプターレンジインターフェイスへのシェアードポインタ
       */
      static std::shared_ptr<IRootParameter> CreateForDescriptorTable(const IDescriptorRange& descriptor_range, const D3D12_SHADER_VISIBILITY& shader_visibility);
    };
  };
};