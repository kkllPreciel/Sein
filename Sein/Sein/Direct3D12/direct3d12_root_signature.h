/**
 *  @file     root_signature.h
 *  @brief    ルートシグネチャに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/13
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
     *  @brief  ルートシグネチャ用インターフェイス
     */
    class IRootSignature
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IRootSignature() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IRootSignature() = default;

      /**
       *  @brief  パイプラインステートの設定にルートシグネチャを設定する
       *  @param  pipeline_state_desc:ルートシグネチャを設定するパイプラインステート
       */
      virtual void SetGraphicsPipelineStateDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const = 0;

      /**
       *  @brief  コマンドリストにルートシグネチャを設定する
       *  @param  command_list:ルートシグネチャを設定するコマンドリスト
       */
      virtual void SetGraphicsRootSignature(ID3D12GraphicsCommandList* command_list) const = 0;

      /**
       *  @brief  ルートシグネチャを取得する
       *  @return ルートシグネチャ
       */
      virtual const ID3D12RootSignature& Get() const = 0;

      /**
       *  @brief  ルートシグネチャを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IRootSignature(const IRootSignature& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IRootSignature& operator = (const IRootSignature& other) = delete;

      /**
       *  @brief  ルートシグネチャを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  root_signature_desc:ルートシグネチャの設定
       *  @return ルートシグネチャへのシェアードポインタ
       */
      static std::shared_ptr<IRootSignature> Create(ID3D12Device* const device, const D3D12_ROOT_SIGNATURE_DESC& root_signature_desc);
    };
  };
};