/**
 *  @file     device.h
 *  @brief    レンダラーのデバイスに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/07/02
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
     *  @brief  デバイス用インターフェース
     */
    class IDevice
    {
    public:
      /**
       *  @brief  デバイス種別
       */
      enum class Type : std::uint32_t {
        Direct3D11, ///< Direct3D11
        Direct3D12, ///< Direct3D12
      };

      /**
       *  @brief  コンストラクタ
       */
      IDevice() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDevice() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDevice(const IDevice& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDevice& operator = (const IDevice& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  デバイスコンテキストを作成する
       *  @param  context_type:デバイスコンテキストの種別
       *  @return デバイスコンテキストへのスマートポインタ
       */
      virtual std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) = 0;
    };

    /**
     *  @brief  デバイスを作成する
     *  @param  type:デバイスの種別
     *  @param  handle:ウィンドウハンドル
     *  @param  width:ウィンドウの横幅
     *  @param  height:ウィンドウの縦幅
     *  @return デバイスへのスマートポインタ
     */
    std::shared_ptr<IDevice> CreateDevice(const IDevice::Type& type, const void* const handle, const std::uint32_t& width, const std::uint32_t& height);
  };
};