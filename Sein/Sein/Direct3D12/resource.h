/**
 *  @file     resource.h
 *  @brief    リソースに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/20
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  リソース用インターフェイス
     */
    class IResource
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IResource() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IResource() = default;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  リソースを取得する
       *  @return リソースの参照
       */
      virtual ID3D12Resource& Get() const noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IResource(const IResource& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IResource& operator = (const IResource& other) = delete;
    };
  };
};