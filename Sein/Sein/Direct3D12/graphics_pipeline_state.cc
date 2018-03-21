/**
 *  @file     graphics_pipeline_state.cc
 *  @brief    グラフィックスパイプラインステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/21
 *  @version  1.0
 */

 // include
#include <functional>
#include "graphics_pipeline_state.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  グラフィックスパイプラインステート用クラス
       */
      class GraphicsPipelineState final : public IGraphicsPipelineState
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        GraphicsPipelineState() : pipeline_state_(nullptr, [](ID3D12PipelineState* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~GraphicsPipelineState() override
        {
          Release();
        }

        /**
         *  @brief  コマンドリストにグラフィックスパイプラインステートを設定する
         *  @param  command_list:グラフィックスパイプラインステートを設定するコマンドリスト
         */
        void SetPipelineState(ID3D12GraphicsCommandList* command_list) const override
        {
          command_list->SetPipelineState(pipeline_state_.get());
        }

        /**
         *  @brief  パイプラインステートを設定する
         *  @param  pipeline_state:パイプラインステート
         */
        void SetPipelineState(ID3D12PipelineState* pipeline_state)
        {
          pipeline_state_.reset(pipeline_state);
        }

        /**
         *  @brief  グラフィックスパイプラインステートを取得する
         *  @return グラフィックスパイプラインステート
         */
        const ID3D12PipelineState& Get() const override
        {
          return *pipeline_state_;
        }

        /**
         *  @brief  グラフィックスパイプラインステートを開放する
         */
        void Release() noexcept override
        {
          pipeline_state_.reset();
        }

      private:
        std::unique_ptr<ID3D12PipelineState, std::function<void(ID3D12PipelineState*)>> pipeline_state_;  ///< パイプラインステート
      };
    };

    /**
     *  @brief  グラフィックスパイプラインステートを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  graphics_pipeline_state_desc:グラフィックスパイプラインステートの設定
     *  @return グラフィックスパイプラインステートへのシェアードポインタ
     */
    std::shared_ptr<IGraphicsPipelineState> IGraphicsPipelineState::Create(ID3D12Device* const device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_pipeline_state_desc)
    {
      // グラフィックスパイプラインステートの生成
      ID3D12PipelineState* temporary;
      if (FAILED(device->CreateGraphicsPipelineState(&graphics_pipeline_state_desc, IID_PPV_ARGS(&temporary))))
      {
        ID3D12DebugDevice* debugInterface;
        if (SUCCEEDED(device->QueryInterface(&debugInterface)))
        {
          debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
          debugInterface->Release();
        }

        throw std::exception("パイプラインステートの生成に失敗しました。");
      }

      auto pipeline_state = std::make_shared<GraphicsPipelineState>();
      pipeline_state->SetPipelineState(temporary);
      return pipeline_state;
    }
  };
};