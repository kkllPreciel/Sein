/**
 *  @file     direct3d11_pipeline_state.h
 *  @brief    Direct3D11のパイプラインステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/07
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
     *  @brief  Direct3D11のパイプラインステート用インターフェース
     */
    class IPipelineState
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IPipelineState() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IPipelineState() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IPipelineState(const IPipelineState& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IPipelineState& operator = (const IPipelineState& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  パイプラインステートを作成する
       *  @param  device:Direct3D11のデバイス
       *  @return パイプラインステートへのシェアードポインタ
       */
      static std::shared_ptr<IPipelineState> Create(ID3D11Device* const device);
    };
  };
};