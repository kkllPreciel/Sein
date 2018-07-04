/**
 *  @file     graphics_pipeline_state.h
 *  @brief    グラフィックスパイプラインステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/21
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
     *  @brief  グラフィックスパイプラインステート用インターフェイス
     */
    class IGraphicsPipelineState
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IGraphicsPipelineState() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IGraphicsPipelineState() = default;

      /**
       *  @brief  コマンドリストにグラフィックスパイプラインステートを設定する
       *  @param  command_list:グラフィックスパイプラインステートを設定するコマンドリスト
       */
      virtual void SetPipelineState(ID3D12GraphicsCommandList* command_list) const = 0;

      /**
       *  @brief  グラフィックスパイプラインステートを取得する
       *  @return グラフィックスパイプラインステート
       */
      virtual const ID3D12PipelineState& Get() const = 0;

      /**
       *  @brief  グラフィックスパイプラインステートを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IGraphicsPipelineState(const IGraphicsPipelineState& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IGraphicsPipelineState& operator = (const IGraphicsPipelineState& other) = delete;

      /**
       *  @brief  グラフィックスパイプラインステートを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  graphics_pipeline_state_desc:グラフィックスパイプラインステートの設定
       *  @return グラフィックスパイプラインステートへのシェアードポインタ
       */
      static std::shared_ptr<IGraphicsPipelineState> Create(ID3D12Device* const device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_pipeline_state_desc);
    };
  };
};