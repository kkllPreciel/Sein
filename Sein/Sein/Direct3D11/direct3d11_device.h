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

#include "direct3d11_device_context.h"
#include "direct3d11_pipeline_state.h"
#include "direct3d11_shader.h"

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
       *  @brief  デバイスコンテキストを作成する
       *  @param  context_type:デバイスコンテキストの種別
       *  @return デバイスコンテキスト用インターフェースへのシェアードポインタ
       */
      virtual std::shared_ptr<IDeviceContext> CreateDeviceContext(IDeviceContext::Type context_type) = 0;

      /**
       *  @brief  パイプラインステートを作成する
       *  @param  pipeline_desc:パイプラインの設定
       *  @return パイプラインステート用インターフェースへのシェアードポインタ
       */
      virtual std::shared_ptr<IPipelineState> CreatePipelineState(IPipelineState::Desc pipeline_desc) = 0;

      /**
       *  @brief  シェーダーを作成する
       *  @param  shader_desc:シェーダーの設定
       *  @return シェーダー用インターフェースへのシェアードポインタ
       */
      virtual std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) = 0;

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