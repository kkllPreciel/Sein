/**
 *  @file     depth_stencil_view.h
 *  @brief    深度ステンシルビューに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/05/26
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  深度ステンシルビュークラス
     */
    class DepthStencilView
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      DepthStencilView();

      /**
       *  @brief  デストラクタ
       */
      ~DepthStencilView();

      /**
       *  @brief  深度ステンシルビューの生成を行う
       *  @param  device:Direct3D12のデバイス
       *  @param  descriptor_handle:ディスクリプターハンドル
       *  @param  width:横幅
       *  @param  height:縦幅
       */
      void Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, unsigned int width, unsigned int height);

      /**
       *  @brief  リソースを開放する
       */
      void Release();

    private:
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      DepthStencilView(const DepthStencilView& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      DepthStencilView& operator = (const DepthStencilView& other) = delete;

    private:
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> depthStencil; ///< リソース(TODO:テクスチャクラスに移動する)
    };
  };
};