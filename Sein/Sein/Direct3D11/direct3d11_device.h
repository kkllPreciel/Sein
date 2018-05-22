/**
 *  @file     direct3d11_device.h
 *  @brief    Direct3D11のデバイスに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11デバイス用インターフェイス
     */
    class IDevice
    {
    public:
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
       *  @brief  デバイスを作成する
       *  @param  handle:ウィンドウハンドル
       *  @param  width:ウィンドウの横幅
       *  @param  height:ウィンドウの縦幅
       *  @return デバイス用インターフェイスへのシェアードポインタ
       */
      static std::shared_ptr<IDevice> Create(const HWND handle, const std::uint32_t width, const std::uint32_t height);
    };
  };
};