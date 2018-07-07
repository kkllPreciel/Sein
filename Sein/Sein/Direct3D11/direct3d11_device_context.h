/**
 *  @file     direct3d11_device_context.h
 *  @brief    Direct3D11のデバイスコンテキストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/06
 *  @version  1.0
 */

#pragma once

// include
#include "../Renderer/device_context.h"
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11のデバイスコンテキスト用インターフェース
     */
    class IDeviceContext : public Renderer::IDeviceContext
    {
    public:
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
       *  @brief  Direct3D11用デバイスコンテキストを取得する
       *  @return Direct3D11用デバイスコンテキスト
       */
      virtual const ID3D11DeviceContext& GetD3D11DeviceContext() = 0;

      /**
       *  @brief  デバイスコンテキストを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  context_type:デバイスコンテキストの種別
       *  @return デバイスコンテキストへのシェアードポインタ
       */
      static std::shared_ptr<IDeviceContext> Create(ID3D11Device* const device, const IDeviceContext::Type& context_type);
    };
  };
};