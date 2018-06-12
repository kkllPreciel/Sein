/**
 *  @file     direct3d11_device_context.h
 *  @brief    Direct3D11のデバイスコンテキストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/06
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
     *  @brief  Direct3D11のデバイスコンテキスト用インターフェース
     */
    class IDeviceContext
    {
    public:
      /**
       *  @brief  コンテキストの種別
       */
      enum class Type : std::uint32_t {
        kImmediate, ///< 即時
        kDeferred,  ///< 遅延
      };

      /**
       *  @brief  コンストラクタ
       */
      IDeviceContext() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDeviceContext() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDeviceContext(const IDeviceContext& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDeviceContext& operator = (const IDeviceContext& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  デバイスコンテキストを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  context_type:デバイスコンテキストの種別
       *  @return デバイスコンテキストへのシェアードポインタ
       */
      static std::shared_ptr<IDeviceContext> Create(ID3D11Device* const device, IDeviceContext::Type context_type);
    };
  };
};