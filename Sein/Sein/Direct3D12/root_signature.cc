/**
 *  @file     root_signature.cc
 *  @brief    ルートシグネチャに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/13
 *  @version  1.0
 */

 // include
#include "root_signature.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ルートシグネチャ用クラス
       */
      class RootSignature final : public IRootSignature
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        RootSignature() : root_signature_(nullptr, [](ID3D12RootSignature* p) {p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~RootSignature() override
        {
          Release();
        }

        /**
         *  @brief  ルートシグネチャを設定する
         *  @param  root_signature:ルートシグネチャ
         */
        void SetRootSignature(ID3D12RootSignature* root_signature)
        {
          root_signature_.reset(root_signature);
        }

        /**
         *  @brief  パイプラインステートの設定にルートシグネチャを設定する
         *  @param  pipeline_state_desc:ルートシグネチャを設定するパイプラインステート
         */
        void SetGraphicsPipelineStateDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const override
        {
          pipeline_state_desc->pRootSignature = root_signature_.get();
        }
  
        /**
         *  @brief  コマンドリストにルートシグネチャを設定する
         *  @param  command_list:ルートシグネチャを設定するコマンドリスト
         */
        void SetGraphicsRootSignature(ID3D12GraphicsCommandList* command_list) const override
        {
          command_list->SetGraphicsRootSignature(root_signature_.get());
        }

        /**
         *  @brief  ルートシグネチャを取得する
         *  @return ルートシグネチャ
         */
        const ID3D12RootSignature& Get() const override
        {
          return *root_signature_;
        }
        
        /**
         *  @brief  ルートシグネチャを開放する
         */
        void Release() noexcept override
        {
          root_signature_.reset();
        }

      private:
        std::unique_ptr<ID3D12RootSignature, void(*)(ID3D12RootSignature*)> root_signature_; ///< ルートシグネチャ
      };
    };

    /**
     *  @brief  ルートシグネチャを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  root_signature_desc:ルートシグネチャの設定
     *  @return ルートシグネチャへのシェアードポインタ
     */
    std::shared_ptr<IRootSignature> IRootSignature::Create(ID3D12Device* const device, const D3D12_ROOT_SIGNATURE_DESC& root_signature_desc)
    {
      // ルートシグネチャのシリアル化
      ID3D10Blob* signature;
      if (FAILED(D3D12SerializeRootSignature(
        &root_signature_desc,         // ルートシグネチャの設定
        D3D_ROOT_SIGNATURE_VERSION_1, // ルートシグネチャのバージョン
        &signature,                   // シリアライズしたルートシグネチャへアクセスするためのインターフェイス(ポインタ)
        nullptr                       // シリアライザのエラーメッセージへアクセスするためのインターフェイス(ポインタ)
      )))
      {
        throw std::exception("ルートシグネチャのシリアライズに失敗しました。");
      }

      // ルートシグネチャの生成
      ID3D12RootSignature* root_signature;
      if (FAILED(device->CreateRootSignature(
        0,                              // マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
        signature->GetBufferPointer(),  // シリアル化されたシグネチャ設定へのポインタ
        signature->GetBufferSize(),     // メモリのブロックサイズ
        IID_PPV_ARGS(&root_signature)   // 作成したルートシグネチャの格納先ポインタ
      )))
      {
        signature->Release();
        throw std::exception("ルートシグネチャの生成に失敗しました。");
      }

      auto instance = std::make_unique<RootSignature>();
      instance->SetRootSignature(root_signature);
      return instance;
    }
  };
};