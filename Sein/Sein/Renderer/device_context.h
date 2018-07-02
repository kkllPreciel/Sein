/**
 *  @file     device_context.h
 *  @brief    レンダラーのデバイスコンテキストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/07/03
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  デバイスコンテキスト用インターフェース
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
    };
  };
};